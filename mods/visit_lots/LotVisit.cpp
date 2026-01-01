// LotVisit - visiting another family's home lot. See LotVisit.h.
#include <vector>
#include <string.h>
#include <algorithm>
#include "LotVisit.h"
#include "LotVisitText.h"

namespace
{
    bool s_bPending = false;
    bool s_bActive = false;
    short s_nHouse = 0;
    std::vector<int> s_Guests;      // GUIDs of the travelling Sims
    short s_nGuestFamily = 0;       // their family: a guest who left it (died, moved out) is no guest any more
    int s_nElapsedMinutes = 0;      // game minutes since arrival
    int s_nLastMinute = -1;
    int s_nTiredChecks = 0;
    bool s_bLeaving = false;        // going home: nothing may end the visit twice
    unsigned int s_nTick = 0;
    enum Reason { kReasonNone, kReasonTired, kReasonDay, kReasonWork };
    int s_nWarned = kReasonNone;    // the hint the hosts have dropped (the visit ends if it is ignored)
    int s_nWarnedAt = 0;            // elapsed minute of that hint
    int s_nLastGreetPush = -1000;   // elapsed minute an owner was last sent to the door
    bool s_bShownOutPending = false; // a visitor script made a guest leave: end the visit at the next tick
    bool s_bHungryWarned = false;   // the "stomachs growling" hint was given
    int s_nLastPizzaPush = -1000;   // elapsed minute a host was last sent to pay the pizza guy
    int s_nAllGoneAt = -1;          // elapsed minute the last guest died (-1 = guests left)
    int s_nWaitingSince = -1;       // elapsed minute a guest started waiting at the door (-1 = nobody waiting)
    bool s_bSleepyWarned = false;   // the "yawning guests" hint was given
    int s_nHungryWarnedAt = 0, s_nSleepyWarnedAt = 0;   // elapsed minute of those hints
    int s_nLastServePush = -1000;   // elapsed minute a host was last sent to serve a delivered pizza

    // Back home: the Sim who called the cab finishes the trip via its interaction frozen in the lot file. If it died
    // on the visit the others stay out of the world, so for 15 minutes after the return anyone still out is placed
    // near the mailbox.
    bool s_bReturnPending = false;
    short s_nReturnHouse = 0;
    std::vector<int> s_HomeGuests;
    bool s_bHomeWatch = false;
    int s_nHomeMinutes = 0;
    int s_nHomeLastMinute = -1;

    // Hosts not on the lot at load (away on a trip, return frozen in the lot file) arrive later through their own
    // arrival tree, which restores saved motives: the host preset is re-applied for a few ticks so it sticks.
    struct LateHost { int nGuid; int nTicksLeft; };
    std::vector<LateHost> s_LateHosts;

    // The hosts' neighbourhood records at the start of the visit; restored before saving so only the guests' data persists.
    struct HostRecord { int nGuid; int nPersDataVersion; short aPersonData[256]; };
    std::vector<HostRecord> s_HostRecords;

    void Say(LotVisitText::Id nTitle, LotVisitText::Id nText)
    {
        SimsApp()->MessageDialog(LotVisitText::Get(nText), LotVisitText::Get(nTitle), kMsgSet_Ok, false);
    }

    Neighborhood *Nbhd() { return Settings()->m_pNeighborhood; }

    ObjectModule *Module()
    {
        House *pHouse = Settings()->m_pGame;
        return pHouse ? pHouse->m_pObjectModule : 0;
    }

    cXPerson *PersonByGuid(int nGuid)
    {
        ObjectModule *pModule = Module();
        if (!pModule || !nGuid)
            return 0;
        return AsPerson(pModule->GetObjectByGUID(nGuid));
    }

    int GuidOf(cXPerson *pPerson)
    {
        return pPerson ? PersonFinder::GetGUID(PersonFinder::GetHandleFromPerson(pPerson)) : 0;
    }

    void ResetClock();
    int FindTTAByActionTree(cXObject *pObject, int nActionTree);   // TTA index of the entry whose action is that tree

    void Reset()
    {
        ResetClock();
        s_bPending = false;
        s_bActive = false;
        s_nHouse = 0;
        s_Guests.clear();
        s_LateHosts.clear();
        s_HostRecords.clear();
    }

    // Rested and fed, fun and social low: with full motives autonomy has nothing to want and they never greet.
    void PresetHostMotives(cXPerson *pOwner)
    {
        static const int kMotives[] = { cXPerson::kEnergy, cXPerson::kComfort, cXPerson::kHunger,
                                        cXPerson::kHygiene, cXPerson::kBladder };
        for (size_t m = 0; m < sizeof(kMotives) / sizeof(kMotives[0]); ++m)
            pOwner->SetMotive(kMotives[m], 100.0f);
        pOwner->SetMotive(cXPerson::kFun, 10.0f);
        pOwner->SetMotive(cXPerson::kSocial, -30.0f);
    }

    void LateHostsTick()
    {
        for (size_t i = 0; i < s_LateHosts.size();)
        {
            cXPerson *pOwner = PersonByGuid(s_LateHosts[i].nGuid);
            if (pOwner)
            {
                PresetHostMotives(pOwner);
                if (--s_LateHosts[i].nTicksLeft <= 0) { s_LateHosts.erase(s_LateHosts.begin() + i); continue; }
            }
            ++i;
        }
    }
}

void LotVisit::PrepareTrip(short nDestHouse, int nGuestGuid, short nGuestFamily)
{
    s_bReturnPending = false;
    s_bHomeWatch = false;
    if (s_bActive)
    {
        s_bLeaving = true;                              // the lot is about to unload: nothing may end the visit twice
        s_bReturnPending = true;
        s_nReturnHouse = nDestHouse;
        s_HomeGuests = s_Guests;
    }
    s_bPending = false;
    Neighborhood *pNeighborhood = Nbhd();
    if (nDestHouse <= 0 || pNeighborhood->GetZoningType(nDestHouse) != 1)
        return;
    short nFamilyId = nGuestFamily;
    if (!nFamilyId)
    {
        cXPerson *pTraveller = PersonByGuid(nGuestGuid);
        if (!pTraveller)
            return;
        nFamilyId = pTraveller->m_nFamilyNumber;
    }
    Family *pGuestFamily = nFamilyId ? pNeighborhood->GetFamily(nFamilyId) : 0;
    if (!pGuestFamily || pGuestFamily->m_nHouseNumber == nDestHouse)
        return;                                         // going home
    Family *pOwners = pNeighborhood->GetFamilyInHouse(nDestHouse);
    if (!pOwners || pOwners == pGuestFamily || pOwners->m_nFamilyId <= 0)
        return;                                         // not someone else's home
    s_Guests.clear();
    s_nGuestFamily = nFamilyId;
    if (nGuestFamily)
    {
        for (int i = 0; i < pGuestFamily->CountMembers(); ++i)
            if (FamilyMember *pMember = pGuestFamily->GetIndexedMember(i))
                s_Guests.push_back(pMember->m_nGuid);
    }
    else
    {
        s_Guests.push_back(nGuestGuid);
    }
    s_nHouse = nDestHouse;
    s_bPending = !s_Guests.empty();
}

cXPerson *LotVisit::OnLotLoaded(short nHouse)
{
    if (s_bReturnPending && nHouse == s_nReturnHouse)
    {
        s_bHomeWatch = true;
        s_nHomeMinutes = 0;
        s_nHomeLastMinute = -1;
    }
    s_bReturnPending = false;
    if (!s_bPending || nHouse != s_nHouse)
    {
        Reset();
        return 0;
    }
    s_bPending = false;
    s_bActive = true;
    ResetClock();
    // a host not on the lot yet gets the preset when he appears (LateHostsTick)
    s_LateHosts.clear();
    s_HostRecords.clear();
    if (Family *pOwners = Settings()->m_pGame->m_pFamily)
        for (int i = 0; i < pOwners->CountMembers(); ++i)
            if (FamilyMember *pMember = pOwners->GetIndexedMember(i))
            {
                if (Neighbor *pRecord = Nbhd()->FindNeighborByGUID(pMember->m_nGuid))
                {
                    HostRecord saved;
                    saved.nGuid = pMember->m_nGuid;
                    saved.nPersDataVersion = pRecord->m_nPersDataVersion;
                    memcpy(saved.aPersonData, pRecord->m_PersonData, sizeof saved.aPersonData);
                    s_HostRecords.push_back(saved);
                }
                if (cXPerson *pOwner = PersonByGuid(pMember->m_nGuid))
                    PresetHostMotives(pOwner);
                else
                {
                    LateHost late = { pMember->m_nGuid, 10 };
                    s_LateHosts.push_back(late);
                }
            }
    cXPerson *pFirst = 0;
    for (size_t i = 0; i < s_Guests.size(); ++i)
    {
        int nGuid = s_Guests[i];
        if (!PersonByGuid(nGuid))
            PersonFinder::CreateDowntownSim(PersonFinder::GetHandle(nGuid));
        cXPerson *pGuest = PersonByGuid(nGuid);
        if (!pGuest)
            continue;
        pGuest->m_nPersonType = 1;                      // a visitor on the owners' lot
        ArriveGuest(pGuest);
        if (pGuest->m_nGender > 1)
            pGuest->m_nGreetStatus = 1;                 // a pet needs no greeting: it goes where its family goes
        if (!pFirst)
            pFirst = pGuest;
    }
    if (!pFirst)
        Reset();
    return pFirst;
}

// Community lots do this in the ped marker's "Main - Family Spawn" tree; a home lot has no such marker, so the SimData
// tokens are restored (generic call 21) and the guest is placed near the mailbox (as call 32 places pets).
void LotVisit::ArriveGuest(cXPerson *pGuest, bool bVisit)
{
    if (Nbhd()->GetInventoryByNeighbourID(pGuest->m_nNeighborId))
    {
        // call 21 applies the SimData tokens of the Sim whose object id is in Temp 0; it does not test for a missing inventory
        short nSavedTemp0 = pGuest->m_Temps[0];
        pGuest->m_Temps[0] = pGuest->m_nObjectId;
        BehaviorNodeParam params = {};
        params.m_Param[0] = 21;
        ThisCall<int>(A::cXObject__TryGenericSimCall, pGuest, (StackElem *)0, &params);
        pGuest->m_Temps[0] = nSavedTemp0;
    }
    cXObject *pMailbox = 0;
    Vec<cXObject *> &objects = Settings()->m_pGame->m_pObjectModule->m_Objects;
    for (size_t i = 0; i < objects.size() && !pMailbox; ++i)
        if (objects[i] && objects[i]->m_pObjdData && objects[i]->m_pObjdData->guid == (int)0xEF121974)   // MAIL_BOX_PART_A
            pMailbox = objects[i];
    FindGoodLocationParams params;
    FTilePt pt;
    pt.nPosY16 = 0;
    pt.nPosX16 = 0;
    params.bAvoidLockedTiles = true;
    params.bSameRoom = false;
    if (pMailbox)
    {
        params.bHaveStart = true;
        params.nPosY16 = pMailbox->m_nPosY16;
        params.nPosX16 = pMailbox->m_nPosX16;
        params.nLevel = pMailbox->m_nLevel;
    }
    int nLevel = pMailbox ? pMailbox->m_nLevel : 1;
    if (pGuest->FindGoodLocation(&params, &pt) && pGuest->CanPlace(pt, nLevel, 0, 0))
    {
        pGuest->Place(pt, nLevel, 0, 0);
        pGuest->ForceLocation();
    }
    if (!bVisit)
        return;
    pGuest->m_nGreetStatus = 0;                         // vanilla visitor arrival: knock, wait to be greeted
    if (pGuest->m_pAttributes && pGuest->m_nNumAttributes > 0)
        pGuest->m_pAttributes[0] = 0;                   // begin_visiting: a normal visit (not a walk-by)
}

void LotVisit::SelectFirstGuest()
{
    if (!s_bActive)
        return;
    ObjectModule *pModule = Module();
    if (pModule && !IsGuest(pModule->GetSelectedPerson()))
        pModule->SetSelectedPerson(NextGuest(0));
}

bool LotVisit::ConfirmLeaving()
{
    if (!s_bActive)
        return true;
    return SimsApp()->MessageDialog(LotVisitText::Get(LotVisitText::kLeaveVisit), LotVisitText::Get(LotVisitText::kLeaveVisitTitle),
                                    kMsgSet_OkCancel, false) == kMsgBtn_Ok;
}

bool LotVisit::Active()
{
    return s_bActive;
}

bool LotVisit::IsGuest(cXPerson *pPerson)
{
    if (!s_bActive || !pPerson)
        return false;
    if (s_nGuestFamily <= 0 || pPerson->m_nFamilyNumber != s_nGuestFamily)
        return false;                                   // left the family on the visit
    // a death clears the family in the neighbourhood data first; the object keeps its own
    Neighbor *pNeighbor = Nbhd()->FindNeighborByID(pPerson->m_nNeighborId);
    if (!pNeighbor || pNeighbor->m_PersonData[kPersonDataFamilyNumber] != s_nGuestFamily)
        return false;
    return std::find(s_Guests.begin(), s_Guests.end(), GuidOf(pPerson)) != s_Guests.end();
}

bool LotVisit::IsVisitedHouse(short nHouse)
{
    return s_bActive && nHouse == s_nHouse;
}

cXPerson *LotVisit::NextGuest(cXPerson *pSelected)
{
    if (!s_bActive || s_Guests.empty())
        return 0;
    size_t nStart = 0;
    int nSelectedGuid = GuidOf(pSelected);
    for (size_t i = 0; i < s_Guests.size(); ++i)
        if (s_Guests[i] == nSelectedGuid)
            nStart = i + 1;
    for (size_t k = 0; k < s_Guests.size(); ++k)
    {
        cXPerson *pGuest = PersonByGuid(s_Guests[(nStart + k) % s_Guests.size()]);
        if (IsGuest(pGuest))
            return pGuest;
    }
    return 0;
}

int LotVisit::GuestCount()
{
    return s_bActive ? (int)s_Guests.size() : 0;
}

int LotVisit::GuestGuid(int nIndex)
{
    return s_bActive && nIndex >= 0 && nIndex < (int)s_Guests.size() ? s_Guests[nIndex] : 0;
}

void LotVisit::OnLoadStart()
{
    s_bActive = false;
}

bool LotVisit::CanVisit(int nHouse)
{
    Neighborhood *pNeighborhood = Nbhd();
    if (nHouse <= 0 || s_bActive || pNeighborhood->GetZoningType(nHouse) != 1)
        return false;
    Family *pOwners = pNeighborhood->GetFamilyInHouse(nHouse);
    House *pHouse = Settings()->m_pGame;
    Family *pPlayed = pHouse ? pHouse->m_pFamily : 0;
    if (!pOwners || pOwners->m_nFamilyId <= 0 || !pPlayed || pPlayed->m_nFamilyId <= 0 || pOwners == pPlayed)
        return false;
    // somebody has to open the door: a living human still in the family
    for (int i = 0; i < pOwners->CountMembers(); ++i)
        if (FamilyMember *pMember = pOwners->GetIndexedMember(i))
            if (Neighbor *pNeighbor = pNeighborhood->FindNeighborByGUID(pMember->m_nGuid))
                if (pNeighbor->m_PersonData[kPersonDataGender] <= 1
                    && pNeighbor->m_PersonData[kPersonDataFamilyNumber] == pOwners->m_nFamilyId)
                    return true;
    return false;
}

namespace
{
    void SendGuestsHome(cXPerson *pLeader = 0)
    {
        // TryChangeLots goes to the leader's family's home, so the leader must be a living guest
        if (!LotVisit::IsGuest(pLeader))
            pLeader = LotVisit::NextGuest(0);
        Family *pHome = pLeader ? Nbhd()->GetFamily(pLeader->m_nFamilyNumber) : 0;
        if (!pHome || pHome->m_nHouseNumber <= 0)
            return;                                     // no living guest with a home to go to - staying
        s_bLeaving = true;
        int nLiving = 0;
        for (size_t i = 0; i < s_Guests.size(); ++i)
            if (LotVisit::IsGuest(PersonByGuid(s_Guests[i])))
                ++nLiving;
        // the phone's "Go Home": temp0 -1 (home), hour 0, minute 15, mode 1 = everyone of the family on the lot
        pLeader->TryChangeLots(0xFFFF, 0, 15, nLiving > 1 ? 1 : 0);
        Settings()->m_pSimulator->m_bLotChangePending = true;
    }

    void SendGuestsHome(LotVisitText::Id nTitle, LotVisitText::Id nText)
    {
        s_bLeaving = true;
        Say(nTitle, nText);
        SendGuestsHome();
    }

    void ResetClock()
    {
        s_nElapsedMinutes = 0;
        s_nLastMinute = -1;
        s_nTiredChecks = 0;
        s_bLeaving = false;
        s_nWarned = kReasonNone;
        s_nWarnedAt = 0;
        s_nLastGreetPush = -1000;
        s_bShownOutPending = false;
        s_nWaitingSince = -1;
        s_bHungryWarned = false;
        s_nLastPizzaPush = -1000;
        s_nLastServePush = -1000;
        s_bSleepyWarned = false;
        s_nHungryWarnedAt = s_nSleepyWarnedAt = 0;
        s_nAllGoneAt = -1;
    }
}

namespace
{
    const int kGuidNeighborhoodPhonePlugin = (int)0xEAA79D86;  // Unleashed "Call Cab.../Go to Old Town"
    const int kGuidMailbox = (int)0xEF121974;                  // MAIL_BOX_PART_A
    const int kGuidSocialGreet = (int)0x9F4B8924;              // SOCIAL_GREET_1_4 (the "Greet..." socials)
    const int kGuidSocialInteractions = (int)0x3317A9CF;       // SOCIAL_INTERACTIONS (the classic "Greet")
    const int kGuidPizzaDude = (int)0xFC72C0C0;                // NPC_PIZZA_DUDE
    const int kGuidPizzaBox = (int)0x543BE3FE;                 // FOOD_PIZZA_BOX (Food.iff)
    const int kFoodStateServed = 6;                            // Food.iff "states" k3: served (slices may be taken)
    const int kGuidPortalPedestrian = (int)0x81E6BEF9;         // PORTAL_PEDESTRIAN (PedPortal.iff: "Leave" trees)
    const int kGuidPortalCar = (int)0x865A6812;                // PORTAL_CAR
    const int kTreePetRunAway = 8347;                          // Dog/CatGlobals "Action - Run Away"
    const int kTreePizzaDudeGreet = 4101;                      // PizzaDude.iff "do npc greeting" (its "Greet" entry)
    const int kTreeServePizza = 4222;                          // Food.iff "Serve Pizza" (the pizza box's "Serve")

    int GuidOfObject(cXObject *pObject)
    {
        return pObject && pObject->m_pObjdData ? (int)pObject->m_pObjdData->guid : 0;
    }

    cXObject *Mailbox()
    {
        ObjectModule *pModule = Module();
        if (!pModule)
            return 0;
        for (size_t i = 0; i < pModule->m_Objects.size(); ++i)
            if (GuidOfObject(pModule->m_Objects[i]) == kGuidMailbox)
                return pModule->m_Objects[i];
        return 0;
    }
}

bool LotVisit::GuestSeesCommunity(cXPerson *pGuest)
{
    if (!IsGuest(pGuest))
        return false;
    return GuidOfObject(pGuest->m_Current.m_pTargetObj) == kGuidNeighborhoodPhonePlugin
        || GuidOfObject(pGuest->m_Current.m_pOrigTarget) == kGuidNeighborhoodPhonePlugin;
}

bool LotVisit::MayEnter(cXPerson *pPerson, cXObject *pTarget)
{
    if (!pTarget || !IsGuest(pPerson) || pPerson->m_nGreetStatus > 0 || pPerson->m_nGender > 1)
        return true;                                    // a guest pet goes where its family goes
    int nSocial = GuidOfObject(pPerson->m_Current.m_pTargetObj);
    if (nSocial == kGuidSocialGreet || nSocial == kGuidSocialInteractions)
        return true;                                    // being greeted: the social walks the guest to the host
    cXObject *pMailbox = Mailbox();
    return !pMailbox || pTarget->m_aData[kObjData_Room] == pMailbox->m_aData[kObjData_Room];
}

bool LotVisit::OnGuestRemoved(cXObject *pVictim, cXObject *pCaller, int nTreeId)
{
    (void)pCaller;
    cXPerson *pGuest = AsPerson(pVictim);
    // death and the removal cleanup are never a "leave": "kill person for good" (#393), "cleanup function" (#8210)
    if (!IsGuest(pGuest) || nTreeId == 393 || nTreeId == 8210)
    {
        // a traveller who is no guest any more (died, left the family): let the removal happen, drop it from the party
        std::vector<int>::iterator it = std::find(s_Guests.begin(), s_Guests.end(), GuidOf(pGuest));
        if (s_bActive && pGuest && it != s_Guests.end())
            s_Guests.erase(it);
        return false;
    }
    if (!pGuest->IsInWorld())
    {
        // the leaving script already took the guest out of the world: put it back so no "red target" stays behind
        LotVisit::ArriveGuest(pGuest, false);
    }
    s_bShownOutPending = true;                          // popup + trip home from the next tick, not from a primitive
    return true;
}

bool LotVisit::OnGuestKillQueued(cXObject *pVictim)
{
    cXPerson *pGuest = AsPerson(pVictim);
    if (!s_bActive || s_bLeaving || !pGuest)
        return false;
    // also for travellers no longer counted as guests (dead or ghost), so they leave the party and the people panel
    int nFrames = pGuest->m_Frames.GetStackSize();
    int nTree = nFrames ? pGuest->m_Frames.GetNthFrame(nFrames - 1)->GetTreeID() : -1;
    if (pGuest->m_bIsGhost)
        nTree = 393;                                    // a ghost's removal is part of the death, never a "leave"
    return OnGuestRemoved(pVictim, 0, nTree);
}

// The host answers the door: "Greet.../Wave" is run inline like AddAction runs a run-immediately entry, skipping its
// check (the "Greet..." variants need relationship 50+). The tree pushes the greeting social onto both Sims.
namespace
{
    // TTA index (what Interaction / push_interaction take) of the entry running the tree; it differs from the entry position
    int FindTTAByActionTree(cXObject *pObject, int nActionTree)
    {
        TreeTable *pTable = pObject && pObject->m_pSelector ? pObject->m_pSelector->GetTreeTable() : 0;
        if (pTable && pTable->m_ppEntries)
            for (int i = 0; i < pTable->m_nEntries; ++i)
                if (pTable->m_ppEntries[i]->m_nActionTreeId == nActionTree)
                    return pTable->m_ppEntries[i]->m_nTTAIndex;
        return -1;
    }
    const int kTreeGreetWave = 8455;                    // PersonGlobals "Interaction - Greet Wave"

    bool AnswerDoor(cXPerson *pHost, cXPerson *pGuest)
    {
        Interaction *&pInlineAction = Global<Interaction *>(A::g_pInlineRunningAction);
        cXPerson *&pInlinePerson = Global<cXPerson *>(A::g_pInlineRunningPerson);
        if (pInlineAction)
            return false;
        int nTTA = FindTTAByActionTree(pGuest, kTreeGreetWave);
        if (nTTA < 0)
            return false;
        Interaction greet(pHost, pGuest, nTTA, 50);
        if (!greet.GetEntry() || !greet.m_nTreeID)
            return false;
        short nSavedPriority = pHost->m_nCurrentPriority;
        pHost->m_nCurrentPriority = (short)greet.m_nPriority;
        pInlinePerson = pHost;
        pInlineAction = &greet;
        bool bOk = pHost->RunOneTickTree(pGuest->m_pSelector->m_pBehavior, pGuest->m_nObjectId, greet.m_nTreeID,
                                         greet.m_nStackVar);
        pInlineAction = 0;
        pInlinePerson = 0;
        pHost->m_nCurrentPriority = nSavedPriority;
        return bOk;
    }

    void HomeWatchTick()
    {
        int nMinute = Settings()->m_pSimulator->m_Globals.Minute;
        if (s_nHomeLastMinute < 0)
            s_nHomeLastMinute = nMinute;
        if (nMinute != s_nHomeLastMinute)
        {
            s_nHomeMinutes += (nMinute - s_nHomeLastMinute + 60) % 60;
            s_nHomeLastMinute = nMinute;
        }
        bool bAllBack = true;
        for (size_t i = 0; i < s_HomeGuests.size(); ++i)
            if (cXPerson *pSim = PersonByGuid(s_HomeGuests[i]))
                if (!pSim->IsInWorld())
                    bAllBack = false;
        if (bAllBack)
        {
            s_bHomeWatch = false;                       // the normal trip home did it
            return;
        }
        if (s_nHomeMinutes < 15)
            return;
        s_bHomeWatch = false;
        for (size_t i = 0; i < s_HomeGuests.size(); ++i)
            if (cXPerson *pSim = PersonByGuid(s_HomeGuests[i]))
                if (!pSim->IsInWorld())
                    LotVisit::ArriveGuest(pSim, false);
        SimsApp()->RemoveTaxiDialog();
        ObjectModule *pModule = Module();
        if (pModule && !pModule->GetSelectedPerson())
            pModule->AdvanceSelectedPerson();
    }
}

// The hosts drop a hint first; if it is ignored they show the guests the door.
void LotVisit::Tick()
{
    if (s_bHomeWatch)
        HomeWatchTick();
    if (!s_bActive || s_bLeaving)
        return;
    LateHostsTick();
    // a traveller who left the family (death clears it before the Grim Reaper arrives) leaves the party at once
    for (size_t g = 0; g < s_Guests.size();)
    {
        Neighbor *pRecord = Nbhd()->FindNeighborByGUID(s_Guests[g]);
        if (pRecord && pRecord->m_PersonData[kPersonDataFamilyNumber] != s_nGuestFamily)
        {
            s_Guests.erase(s_Guests.begin() + g);
            ObjectModule *pM = Module();
            if (pM && !IsGuest(pM->GetSelectedPerson()))
                pM->SetSelectedPerson(NextGuest(0));
            continue;
        }
        ++g;
    }
    cSimulator *pSimulator = Settings()->m_pSimulator;
    int nMinute = pSimulator->m_Globals.Minute;
    if (s_nLastMinute < 0)
        s_nLastMinute = nMinute;
    if (nMinute != s_nLastMinute)
    {
        s_nElapsedMinutes += (nMinute - s_nLastMinute + 60) % 60;
        s_nLastMinute = nMinute;
    }
    if (s_bShownOutPending)
    {
        SendGuestsHome(LotVisitText::kShownOutTitle, LotVisitText::kShownOut);
        return;
    }
    // the last guest died: after a few minutes for the Grim Reaper, go home if the family has anyone left, else to the neighborhood
    if (s_Guests.empty() && s_nGuestFamily > 0)
    {
        if (s_nAllGoneAt < 0)
            s_nAllGoneAt = s_nElapsedMinutes;
        if (s_nElapsedMinutes - s_nAllGoneAt < 5)
            return;
        Neighborhood *pNeighborhood = Nbhd();
        Family *pFamily = pNeighborhood->GetFamily(s_nGuestFamily);
        int nLeft = 0;
        for (int i = 0; pFamily && i < pFamily->CountMembers(); ++i)
            if (FamilyMember *pMember = pFamily->GetIndexedMember(i))
                if (Neighbor *pNeighbor = pNeighborhood->FindNeighborByGUID(pMember->m_nGuid))
                    if (pNeighbor->m_PersonData[kPersonDataFamilyNumber] == s_nGuestFamily)
                        ++nLeft;
        s_bLeaving = true;
        if (nLeft && pFamily->m_nHouseNumber > 0)
        {
            Say(LotVisitText::kGuestsGoneTitle, LotVisitText::kGuestsGoneHome);
            SimsApp()->SetChangeLotTrigger((short)pFamily->m_nHouseNumber, 0, 0);
        }
        else
        {
            Say(LotVisitText::kGuestsGoneTitle, LotVisitText::kGuestsGoneNbhd);
            SimsApp()->SaveGame();
            SimsApp()->m_bReqNeighborhood = 1;
        }
        return;
    }
    if (++s_nTick % 30)
        return;
    cXPerson *pAnyGuest = NextGuest(0);
    House *pHouse = Settings()->m_pGame;
    Family *pOwners = pHouse ? pHouse->m_pFamily : 0;
    ObjectModule *pModule = Module();
    if (!pAnyGuest || !pOwners || !pModule)
        return;

    int nOwners = 0, nAway = 0, nHome = 0, nTired = 0;
    cXPerson *pDoorman = 0;
    for (int i = 0; i < pOwners->CountMembers(); ++i)
    {
        FamilyMember *pMember = pOwners->GetIndexedMember(i);
        cXPerson *pOwner = pMember ? pModule->GetPersonByGUID(pMember->m_nGuid) : 0;
        if (pOwner && pOwner->m_nGender > 1)
            continue;                                   // pets don't host
        ++nOwners;
        if (!pOwner || pOwner->m_bAtWorkOrSchool || pOwner->Hidden())
        {
            ++nAway;
            continue;
        }
        ++nHome;
        bool bTired = pOwner->IsSleeping() || pOwner->m_Motives.m_Motive.Energy < -60.0f;
        if (bTired)
            ++nTired;
        else if (!pDoorman && !pOwner->IsChild())
            pDoorman = pOwner;
    }

    // guests have no fridge: the hungriest decides, a hint first, home before anybody starves
    float fHungriest = 100.0f;
    for (size_t g = 0; g < s_Guests.size(); ++g)
    {
        cXPerson *pGuest = PersonByGuid(s_Guests[g]);
        // hunger at -100 is death already under way; a starving Sim passes -80 long before
        if (IsGuest(pGuest) && pGuest->IsInWorld() && pGuest->m_Motives.m_Motive.Hunger > -99.5f
            && pGuest->m_Motives.m_Motive.Hunger < fHungriest)      // pets count: a starving pet dies too
            fHungriest = pGuest->m_Motives.m_Motive.Hunger;
    }
    // Grace period: guests may arrive hungry or tired, so nothing ends the visit in the first hour; hints start after
    // 15 minutes and precede any action by at least 30.
    const int kGraceMinutes = 60, kHintAfter = 15, kHintLead = 30;
    bool bGrace = s_nElapsedMinutes < kGraceMinutes;
    if (fHungriest < -50.0f && !s_bHungryWarned && s_nElapsedMinutes >= kHintAfter)
    {
        s_bHungryWarned = true;
        s_nHungryWarnedAt = s_nElapsedMinutes;
        Say(LotVisitText::kHungryWarnTitle, LotVisitText::kHungryWarn);
        return;
    }
    if (fHungriest < -80.0f)
    {
        if (!bGrace && s_bHungryWarned && s_nElapsedMinutes - s_nHungryWarnedAt >= kHintLead)
        {
            SendGuestsHome(LotVisitText::kHungryTitle, LotVisitText::kHungry);
            return;
        }
    }
    else if (fHungriest > -30.0f)
        s_bHungryWarned = false;

    // guests can't claim a bed either: the sleepiest awake guest decides, a hint first
    float fSleepiest = 100.0f;
    for (size_t g = 0; g < s_Guests.size(); ++g)
    {
        cXPerson *pGuest = PersonByGuid(s_Guests[g]);
        if (IsGuest(pGuest) && pGuest->m_nGender <= 1 && pGuest->IsInWorld() && !pGuest->IsSleeping()
            && pGuest->m_Motives.m_Motive.Energy < fSleepiest)      // pets' needs don't end the visit
            fSleepiest = pGuest->m_Motives.m_Motive.Energy;
    }
    if (fSleepiest < -50.0f && !s_bSleepyWarned && s_nElapsedMinutes >= kHintAfter)
    {
        s_bSleepyWarned = true;
        s_nSleepyWarnedAt = s_nElapsedMinutes;
        Say(LotVisitText::kSleepyWarnTitle, LotVisitText::kSleepyWarn);
        return;
    }
    if (fSleepiest < -75.0f)
    {
        if (!bGrace && s_bSleepyWarned && s_nElapsedMinutes - s_nSleepyWarnedAt >= kHintLead)
        {
            SendGuestsHome(LotVisitText::kSleepyTitle, LotVisitText::kSleepy);
            return;
        }
    }
    else if (fSleepiest > -30.0f)
        s_bSleepyWarned = false;

    // the pizza guy is waiting: nobody controls the hosts, so one goes to pay ("Greet" on him is the payment)
    if (pDoorman && s_nElapsedMinutes - s_nLastPizzaPush >= 20)
        for (size_t i = 0; i < pModule->m_Objects.size(); ++i)
        {
            cXObject *pObject = pModule->m_Objects[i];
            cXPerson *pPizzaGuy = GuidOfObject(pObject) == kGuidPizzaDude ? AsPerson(pObject) : 0;
            if (!pPizzaGuy || !pPizzaGuy->IsInWorld())
                continue;
            int nGreetTTA = FindTTAByActionTree(pPizzaGuy, kTreePizzaDudeGreet);
            Interaction pay(pDoorman, pPizzaGuy, nGreetTTA < 0 ? 0 : nGreetTTA, 50);
            if (nGreetTTA >= 0)
                pDoorman->AddAction(&pay);
            s_nLastPizzaPush = s_nElapsedMinutes;
            break;
        }

    // a greeted visitor can only grab a slice once a resident has served the pizza (Food.iff state 6), which a
    // well-fed host never does on his own: send one to serve any unserved box lying around
    if (pDoorman && s_nElapsedMinutes - s_nLastServePush >= 10)
        for (size_t i = 0; i < pModule->m_Objects.size(); ++i)
        {
            cXObject *pBox = pModule->m_Objects[i];
            if (GuidOfObject(pBox) != kGuidPizzaBox || !pBox->m_pAttributes || pBox->m_nNumAttributes < 2)
                continue;
            if (pBox->m_pAttributes[1] == kFoodStateServed || pBox->m_nContainerId != 0)
                continue;
            int nServeTTA = FindTTAByActionTree(pBox, kTreeServePizza);
            Interaction serve(pDoorman, pBox, nServeTTA < 0 ? 0 : nServeTTA, 50);
            if (nServeTTA >= 0)
                pDoorman->AddAction(&serve);
            s_nLastServePush = s_nElapsedMinutes;
            break;
        }

    // all human hosts died during the visit
    if (!nOwners)
    {
        SendGuestsHome(LotVisitText::kHostsGoneTitle, LotVisitText::kHostsGone);
        return;
    }

    // safety net: guests left waiting 90 minutes while a host is awake are let in, unless a greeting is under way
    const int kDoorWaitMinutes = 90;
    bool bWaiting = false;
    for (size_t g = 0; g < s_Guests.size(); ++g)
        if (cXPerson *pGuest = PersonByGuid(s_Guests[g]))
            if (!pGuest->Hidden() && pGuest->m_nGreetStatus <= 0 && pGuest->m_nGender <= 1)   // pets need no greeting
                bWaiting = true;
    bool bGreetInProgress = false;
    for (int i = 0; i < pOwners->CountMembers() && !bGreetInProgress; ++i)
    {
        FamilyMember *pMember = pOwners->GetIndexedMember(i);
        cXPerson *pOwner = pMember ? pModule->GetPersonByGUID(pMember->m_nGuid) : 0;
        if (!pOwner)
            continue;
        if (GuidOfObject(pOwner->m_Current.m_pTargetObj) == kGuidSocialGreet)
            bGreetInProgress = true;
        for (unsigned q = 0; q < pOwner->m_ActionQueue.Count() && !bGreetInProgress; ++q)
            if (GuidOfObject(pOwner->m_ActionQueue.At(q).m_pTargetObj) == kGuidSocialGreet)
                bGreetInProgress = true;
    }
    if (!bWaiting)
        s_nWaitingSince = -1;
    else if (s_nWaitingSince < 0)
        s_nWaitingSince = s_nElapsedMinutes;
    else if (bGreetInProgress && pDoorman && s_nElapsedMinutes - s_nWaitingSince >= kDoorWaitMinutes)
    {
        // a host is greeting (social current or queued): the door safety net waits
    }
    else if (pDoorman && s_nElapsedMinutes - s_nWaitingSince >= kDoorWaitMinutes)
    {
        Say(LotVisitText::kDoorOpenTitle, LotVisitText::kDoorOpen);
        for (size_t g = 0; g < s_Guests.size(); ++g)
            if (cXPerson *pGuest = PersonByGuid(s_Guests[g]))
                if (pGuest->m_nGreetStatus <= 0)
                    pGuest->m_nGreetStatus = 1;         // pets included
        s_nWaitingSince = -1;
    }

    if (pDoorman && s_nElapsedMinutes - s_nLastGreetPush >= 15)
        for (size_t g = 0; g < s_Guests.size(); ++g)
        {
            cXPerson *pGuest = PersonByGuid(s_Guests[g]);
            if (!pGuest || pGuest->Hidden() || pGuest->m_nGreetStatus != 0 || s_nElapsedMinutes < 10)
                continue;
            if (pGuest->m_nGender > 1)
                continue;                               // pets are not greeted
            AnswerDoor(pDoorman, pGuest);
            s_nLastGreetPush = s_nElapsedMinutes;
        }

    s_nTiredChecks = nHome && nTired == nHome ? s_nTiredChecks + 1 : 0;
    int nReason = kReasonNone;
    if (nOwners && nAway == nOwners && s_LateHosts.empty())
        nReason = kReasonWork;                          // hosts still returning from a trip don't count as away
    else if (s_nTiredChecks >= 2)
        nReason = kReasonTired;
    else if (s_nElapsedMinutes >= 22 * 60)
        nReason = kReasonDay;
    if (bGrace && nReason != kReasonNone)
        return;

    if (s_nWarned == kReasonNone)
    {
        if (nReason == kReasonNone)
            return;
        s_nWarned = nReason;
        s_nWarnedAt = s_nElapsedMinutes;
        if (nReason == kReasonWork)
            Say(LotVisitText::kWorkWarnTitle, LotVisitText::kWorkWarn);
        else if (nReason == kReasonTired)
            Say(LotVisitText::kTiredWarnTitle, LotVisitText::kTiredWarn);
        else
            Say(LotVisitText::kDayWarnTitle, LotVisitText::kDayWarn);
        return;
    }
    if (s_nWarned == kReasonDay)
    {
        if (s_nElapsedMinutes >= 24 * 60)
            SendGuestsHome(LotVisitText::kDayTitle, LotVisitText::kDay);     // a whole day here: going home
        return;
    }
    if (nReason != s_nWarned)
    {
        s_nWarned = kReasonNone;
        return;
    }
    if (s_nElapsedMinutes - s_nWarnedAt < 60)
        return;
    if (nReason == kReasonWork)
        SendGuestsHome(LotVisitText::kWorkTitle, LotVisitText::kWork);
    else
        SendGuestsHome(LotVisitText::kTiredTitle, LotVisitText::kTired);
}

// The lot file is never written; the neighbourhood is, with the hosts' records as at the start of the visit.
void LotVisit::SaveGuestsOnly(Neighborhood *pNeighborhood)
{
    int nVersion = Neighbor::GetLatestPersDataVersion();
    Vec<PersDataPair> &table = Neighbor::GetPersistentDataFields(nVersion);
    for (size_t g = 0; g < s_Guests.size(); ++g)
    {
        cXPerson *pGuest = PersonByGuid(s_Guests[g]);
        Neighbor *pRecord = pGuest ? pNeighborhood->FindNeighborByID(pGuest->m_nNeighborId) : 0;
        if (!pRecord)
            continue;
        pRecord->m_nPersDataVersion = nVersion;
        for (PersDataPair *f = table.begin(); f != table.end(); ++f)
            pRecord->m_PersonData[f->m_nField] = pGuest->m_PersonData[f->m_nField];
    }
    for (size_t h = 0; h < s_HostRecords.size(); ++h)
        if (Neighbor *pRecord = pNeighborhood->FindNeighborByGUID(s_HostRecords[h].nGuid))
        {
            pRecord->m_nPersDataVersion = s_HostRecords[h].nPersDataVersion;
            memcpy(pRecord->m_PersonData, s_HostRecords[h].aPersonData, sizeof pRecord->m_PersonData);
        }
    pNeighborhood->Save(Settings()->m_nSaveVersion);
}

void LotVisit::GoHome(cXPerson *pCaller)
{
    if (!s_bActive || s_bLeaving)
        return;
    SendGuestsHome(pCaller);
}

bool LotVisit::BlocksInteraction(cXPerson *pSim, cXObject *pTarget, int nActionTreeId)
{
    if (!s_bActive || !pTarget || !IsGuest(pSim))
        return false;
    int nGuid = GuidOfObject(pTarget);
    if (nGuid == kGuidPortalPedestrian || nGuid == kGuidPortalCar)
        return true;                                    // "Leave" / "Leave world": the portals take Sims off the lot
    if (nActionTreeId == kTreePetRunAway && pTarget == pSim)
        return true;                                    // a guest pet can't run away from someone else's home
    // guests are one family that arrived together and don't greet each other; greetings with the hosts are untouched
    static const int kGreetTrees[] = { 8294, 8449, 8450, 8451, 8452, 8453, 8454, 8455, 8546, 8599, 8608, 8611 };
    cXPerson *pOther = AsPerson(pTarget);
    if (pOther && pOther != pSim && IsGuest(pOther))
        for (size_t i = 0; i < sizeof(kGreetTrees) / sizeof(kGreetTrees[0]); ++i)
            if (kGreetTrees[i] == nActionTreeId)
                return true;
    return false;
}
