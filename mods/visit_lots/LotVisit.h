#pragma once
// LotVisit - new feature: visiting another family's home lot via the Unleashed "Go to Old Town" trip.
//   - the owners stay the lot's family (House::m_pFamily): residents, always autonomous, never selectable;
//   - the travelling Sims ("guests") are visitors (person type 1) and the only selectable Sims; to their scripts the
//     lot reads as a community lot (generic call 28), so "Go to Old Town" becomes "Go Home";
//   - Build/Buy and saving are disabled and the lot file is never written; only neighbourhood data is saved.
#include "game_api.h"

namespace LotVisit
{
    // Before the lot change: nGuestGuid = traveller (single-Sim trip) or 0; nGuestFamily = whole-family trip or 0.
    void PrepareTrip(short nDestHouse, int nGuestGuid, short nGuestFamily);
    // No visit is active while a lot loads (a pending one stays pending).
    void OnLoadStart();
    // Spawns the guests when a visit is pending; ends any visit otherwise.
    cXPerson *OnLotLoaded(short nHouse);     // returns the first guest (0 = no visit)
    // SimData tokens (generic call 21), placed near the mailbox. bVisit: ungreeted guest; otherwise a homecomer.
    void ArriveGuest(cXPerson *pGuest, bool bVisit = true);
    void SelectFirstGuest();
    // Saves the neighbourhood with only the guests' persistent fields refreshed; the hosts' records are restored.
    void SaveGuestsOnly(Neighborhood *pNeighborhood);

    // a residential lot another family lives in
    bool CanVisit(int nHouse);

    // every PostSim: ends the visit (with a popup) when the hosts are asleep/exhausted or away, or after a game day.
    void Tick();
    // Generic call 45: the whole travelling family goes home; pCaller leads the trip if it is a guest.
    void GoHome(cXPerson *pCaller);
    // Interactions a guest must never take: leaving the lot on its own (portals, pet running away), greeting each other.
    bool BlocksInteraction(cXPerson *pSim, cXObject *pTarget, int nActionTreeId);

    // Generic call 28: a community lot while the guest runs the Old Town phone interaction, real zoning otherwise.
    bool GuestSeesCommunity(cXPerson *pGuest);
    // an ungreeted guest stays in the yard: false for a target in another room than the mailbox's
    bool MayEnter(cXPerson *pPerson, cXObject *pTarget);
    // A visitor script removing a guest ends the visit for everyone; true when intercepted.
    bool OnGuestRemoved(cXObject *pVictim, cXObject *pCaller, int nTreeId);
    // Same, via AddToKillQueue (the exe inlined TryKillObject into its primitive dispatcher); the tree is read from the victim's stack.
    bool OnGuestKillQueued(cXObject *pVictim);

    // "Neighborhood" / "Quit" during a visit: shows the warning, true when the player leaves anyway.
    bool ConfirmLeaving();
    bool Active();
    bool IsGuest(cXPerson *pPerson);
    bool IsVisitedHouse(short nHouse);
    // the player's household on a visited lot is the guests
    int GuestCount();
    int GuestGuid(int nIndex);
    // next guest after the selected one (wraps); 0 when none
    cXPerson *NextGuest(cXPerson *pSelected);
}
