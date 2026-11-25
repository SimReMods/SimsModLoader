// game_api.h - the part of Sims.exe the visitlots mod touches: class layouts with explicit offsets (only the fields
// the mod uses) and the game's functions called through address thunks. Names are the game's (RTTI); addresses are
// in game_addr.h.
#pragma once
#include <stddef.h>

// GAME_CC=1 builds for the Complete Collection (2003, VC6 + SGI STL): game_addr_cc.h, and GA_O(legacy, cc) picks the
// 2003 offset of a field where the class layout differs (VC6 string 12 bytes, SGI map, members only Legacy has).
#ifndef GAME_CC
#define GAME_CC 0
#endif
// GAME_EA=1: the Legacy Collection from the EA app - the Steam code relinked at other addresses (game_addr_ea.h is
// generated from game_addr.h by tools/port_addrs.py); class layouts are the same.
#ifndef GAME_EA
#define GAME_EA 0
#endif
#if GAME_CC
#define GA_ADDR_FILE "game_addr_cc.h"
#define GA_O(legacy, cc) (cc)
#elif GAME_EA
#define GA_ADDR_FILE "game_addr_ea.h"
#define GA_O(legacy, cc) (legacy)
#else
#define GA_ADDR_FILE "game_addr.h"
#define GA_O(legacy, cc) (legacy)
#endif

#pragma warning(push)
#pragma warning(disable : 4201)     // anonymous structs: the fields below are union members at explicit offsets

// a field `name` of `type` at byte offset `off`, placed inside a `union { ... };`
#define GA_F(off, type, name) struct { char _o_##name[off]; type name; };
#define GA_F0(type, name) struct { type name; };
#define GA_A(off, type, name, n) struct { char _o_##name[off]; type name[n]; };
#define GA_A0(type, name, n) struct { type name[n]; };
#define GA_SIZE(n) char _ga_size[n];

namespace ga
{
    // addresses (link-time VAs, see game_addr.h)
    namespace A
    {
        enum : unsigned
        {
#define GA_FN(n, va) n = va,
#define GA_DATA(n, va) n = va,
#define GA_CALL(n, va, f, c) n = va,
#define GA_CODE(n, va, len, f) n = va,
#define GA_LABEL(n, va, f) n = va,
#define GA_CRT(n, va, site) n = va,
#define GA_RTTI(n, va, s) n = va,
#define GA_VTBL(n, va) n = va,
#define GA_VSLOT(n, vt, slot, fn) n = slot,
#include GA_ADDR_FILE
#undef GA_FN
#undef GA_DATA
#undef GA_CALL
#undef GA_CODE
#undef GA_LABEL
#undef GA_CRT
#undef GA_RTTI
#undef GA_VTBL
#undef GA_VSLOT
        };
    }

    extern unsigned g_exeBase;                          // actual base of Sims.exe (ASLR), set by SimsMod_Init
    inline void *Va(unsigned va) { return (void *)(va - 0x400000u + g_exeBase); }
    template <class T> inline T &Global(unsigned va) { return *(T *)Va(va); }

    // thunks: cdecl / thiscall / virtual call by slot. Reference parameters are passed as pointers (same ABI).
    template <class R, class... T> inline R Call(unsigned va, T... a) { return ((R(__cdecl *)(T...))Va(va))(a...); }
    template <class R, class... T> inline R ThisCall(unsigned va, const void *self, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))Va(va))(self, a...);
    }
    template <class R, class... T> inline R VCall(const void *self, int slot, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))(*(void ***)self)[slot])(self, a...);
    }

    template <class T> struct Vec                      // std::vector<T> as laid out in the exe (MSVC and SGI: 3 pointers)
    {
        T *_Myfirst, *_Mylast, *_Myend;
        size_t size() const { return (size_t)(_Mylast - _Myfirst); }
        T &operator[](size_t i) const { return _Myfirst[i]; }
        T *begin() const { return _Myfirst; }
        T *end() const { return _Mylast; }
    };

    class cXObject;
    class cXPerson;
    class ObjectModule;
    class Family;
    class Neighbor;
    class SimInventory;
    class cWinLotBtn;
    struct Behavior;
    struct StackElem;
    struct FamilyMember;

    enum { kMsgSet_Ok = 0, kMsgSet_OkCancel = 1 };
    enum { kMsgBtn_Ok = 3 };
    enum { kObjType_Person = 2 };
    enum { kObjData_RouteId = 6, kObjData_Room = 29 };
    enum { kPersonDataFamilyNumber = 0x3D, kPersonDataGender = 0x41 };
    enum { kToken_SimData = 2, kSimData_ArriveHour = 7, kSimData_ArriveMinute = 8 };
    enum { kLotFilter_Lodging = 0x1 };

    struct OBJD
    {
        union { GA_F(0x12, short, type) GA_F(0x14, short, master_id) GA_F(0x1C, long, guid) };
    };

    class TreeTableEntry
    {
    public:
        union
        {
            GA_F0(short, m_nGuardTreeId) GA_F(0x02, short, m_nActionTreeId) GA_F(0x14, unsigned int, m_nFlags)
            GA_F(0x18, int, m_nTTAIndex)
        };
    };

    class TreeTable
    {
    public:
        union { GA_F(0x04, int, m_nEntries) GA_F(0x10, TreeTableEntry **, m_ppEntries) };
    };

    class ObjSelector
    {
    public:
        union { GA_F(0x0C, Behavior *, m_pBehavior) };
        TreeTable *GetTreeTable() { return ThisCall<TreeTable *>(A::ObjSelector__GetTreeTable, this); }
    };

    struct StackElem
    {
        int GetTreeID() const { return ThisCall<int>(A::StackElem__GetTreeID, this); }
    };

    class TreeStack
    {
    public:
        union { GA_SIZE(20) };
        StackElem *GetNthFrame(int n) { return ThisCall<StackElem *>(A::TreeStack__GetNthFrame, this, n); }
        int GetStackSize() { return ThisCall<int>(A::TreeStack__GetStackSize, this); }
    };

    union BehaviorNodeParam
    {
        GA_A0(short, m_Param, 4) GA_A0(unsigned char, m_Op, 8)
    };

    struct FTilePt
    {
        union { GA_F0(int, nPosY16) GA_F(4, int, nPosX16) };
    };

    struct FindGoodLocationParams
    {
        union
        {
            GA_F0(bool, bHaveStart) GA_F(0x04, int, nPosY16) GA_F(0x08, int, nPosX16) GA_F(0x0C, int, nLevel)
            GA_F(0x10, int, nPreferredAxis) GA_F(0x14, bool, bSameRoom) GA_F(0x15, bool, bRequireObject)
            GA_F(0x16, bool, bAvoidLockedTiles) GA_F(0x17, bool, bRandomStart) GA_SIZE(24)
        };
        // the exe's inline ctor (COMDAT copy 0x610540): nLevel is left unset
        FindGoodLocationParams()
        {
            bHaveStart = false; nPosY16 = 0; nPosX16 = 0; nPreferredAxis = -1;
            bSameRoom = true; bRequireObject = true; bAvoidLockedTiles = false; bRandomStart = false;
        }
    };

    struct SimInventoryToken
    {
        union { GA_F0(int, m_nTokenType) GA_F(0x04, long, m_nGuid) GA_F(0x08, short, m_nCount) GA_SIZE(12) };
    };

    class SimInventory
    {
    public:
        short SetToNextToken(int nType, short &nIndex, long nGuid)
        {
            return ThisCall<short>(A::SimInventory__SetToNextToken, this, nType, &nIndex, nGuid);
        }
        bool GetTokenAtIndex(short nIndex, SimInventoryToken &token)
        {
            return ThisCall<bool>(A::SimInventory__GetTokenAtIndex, this, nIndex, &token);
        }
        bool RemoveTokenByIndex(short nIndex, short nCount)
        {
            return ThisCall<bool>(A::SimInventory__RemoveTokenByIndex, this, nIndex, nCount);
        }
    };

    class cTSString                                     // std::string-derived (24 bytes; 2003: 12); built and freed by the exe
    {
    public:
        union { GA_SIZE(GA_O(24, 12)) };
        explicit cTSString(const char *psz) { ThisCall<void>(A::cTSString__cTSString, this, psz); }
        ~cTSString() { ThisCall<void>(A::cTSString___cTSString, this); }
    };

    // interactions
    class InteractionData                               // the layout of the game's Interaction
    {
    public:
        union
        {
            GA_F0(int, m_nKind) GA_F(0x04, cXPerson *, m_pFrom) GA_F(0x08, cXObject *, m_pTargetObj)
            GA_F(0x0C, cXObject *, m_pOrigTarget) GA_F(0x10, int, m_nActionID) GA_A(0x14, short, m_nStackVar, 4)
            GA_F(0x1C, int, m_nPriority) GA_F(0x20, short, m_nTreeID) GA_F(GA_O(0x4C, 0x40), unsigned int, m_nFlags)
            GA_SIZE(GA_O(80, 0x44))
        };
        TreeTableEntry *GetEntry() const { return ThisCall<TreeTableEntry *>(A::Interaction__GetEntry, this); }
    };

    class Interaction : public InteractionData          // a local Interaction: the exe's ctor and dtor
    {
    public:
        Interaction(cXPerson *pFrom, cXObject *pTarget, int nActionID, int nPriority)
        {
            ThisCall<void>(A::Interaction__Interaction, this, pFrom, pTarget, nActionID, nPriority);
        }
        ~Interaction() { ThisCall<void>(A::Interaction___Interaction, this); }
        Interaction(const Interaction &) = delete;
        Interaction &operator=(const Interaction &) = delete;
    };

    struct InteractionQueue
    {
        union { GA_A0(InteractionData, m_Item, 8) GA_F(GA_O(0x280, 0x220), int, m_nHead) GA_F(GA_O(0x284, 0x224), int, m_nTail) };
        unsigned int Count() const { return (unsigned int)(m_nTail - m_nHead); }
        InteractionData &At(unsigned int i) { return m_Item[(m_nHead + i) & 7]; }
    };

    struct MotiveNames
    {
        union
        {
            GA_F(0x14, float, Energy) GA_F(0x18, float, Comfort) GA_F(0x1C, float, Hunger)
            GA_F(0x20, float, Hygiene) GA_F(0x24, float, Bladder) GA_F(0x38, float, Social) GA_F(0x3C, float, Fun)
            GA_SIZE(64)
        };
    };

    struct Motives
    {
        union { GA_F0(MotiveNames, m_Motive) GA_SIZE(132) };
    };

    // objects and people
    class cXObject
    {
    public:
        enum { kSize = GA_O(0x190, 0x18C) };
        union
        {
            GA_F(0x008, TreeStack, m_Frames) GA_F(0x02C, short *, m_pAttributes) GA_F(0x030, int, m_nNumAttributes)
            GA_A(0x03A, short, m_Temps, 8) GA_A(0x04A, short, m_aData, 73) GA_F(0x04E, short, m_nContainerId)
            GA_F(0x05A, short, m_nObjectFlags) GA_F(0x07C, short, m_nLockoutCount) GA_F(0x08E, short, m_bHidden)
            GA_F(0x0AE, short, m_nHideInteraction) GA_F(0x0C0, short, m_nCategory)
            GA_F(0x0DC, ObjectModule *, m_pModule) GA_F(0x0F0, short, m_nObjectId) GA_F(0x0F4, int, m_nPosY16)
            GA_F(0x0F8, int, m_nPosX16) GA_F(0x110, int, m_nLevel) GA_F(GA_O(0x11C, 0x118), OBJD *, m_pObjdData)
            GA_F(GA_O(0x120, 0x11C), ObjSelector *, m_pSelector) GA_SIZE(kSize)
        };
        bool IsInWorld() { return ThisCall<bool>(A::cXObject__IsInWorld, this); }
        bool FindGoodLocation(FindGoodLocationParams *pParams, FTilePt *pOut)
        {
            return ThisCall<bool>(A::cXObject__FindGoodLocation, this, pParams, pOut);
        }
        bool CanPlace(const FTilePt &pos, int nLevel, cXObject *pParent, int nSlot)     // virtual
        {
            return VCall<bool>(this, A::Slot_CanPlace, &pos, nLevel, pParent, nSlot);
        }
    };

#define GA_FP(off, type, name) GA_F((off) - cXObject::kSize, type, name)   // cXPerson field at absolute offset `off`
#define GA_AP(off, type, name, n) GA_A((off) - cXObject::kSize, type, name, n)
#define GA_SIZEP(n) GA_SIZE((n) - cXObject::kSize)                          // cXPerson total size `n`
    class cXPerson : public cXObject
    {
    public:
        enum { kEnergy = 5, kComfort = 6, kHunger = 7, kHygiene = 8, kBladder = 9, kSocial = 14, kFun = 15 };
        union
        {
            // 2003: m_PersonData[] starts 4 bytes lower (same indices); the queue and m_Current hold 0x44-byte Interactions
            GA_AP(GA_O(0x590, 0x58C), short, m_PersonData, 256) GA_FP(GA_O(0x5CE, 0x5CA), short, m_nNeighborId)
            GA_FP(GA_O(0x5D0, 0x5CC), short, m_nPersonType) GA_FP(GA_O(0x5D2, 0x5CE), short, m_nCurrentPriority)
            GA_FP(GA_O(0x5D4, 0x5D0), short, m_nGreetStatus) GA_FP(GA_O(0x604, 0x600), short, m_nAge)
            GA_FP(GA_O(0x60A, 0x606), short, m_nFamilyNumber) GA_FP(GA_O(0x612, 0x60E), short, m_nGender)
            GA_FP(GA_O(0x618, 0x614), short, m_bIsGhost) GA_FP(GA_O(0x63E, 0x63A), short, m_bAtWorkOrSchool)
            GA_FP(GA_O(0x790, 0x78C), Motives, m_Motives) GA_FP(GA_O(0x824, 0x820), InteractionQueue, m_ActionQueue)
            GA_FP(GA_O(0xAAC, 0xA48), InteractionData, m_Current) GA_SIZEP(GA_O(0xC90, 0xC14))
        };
        bool IsChild() const { return m_nAge && m_nAge < 18; }          // header inline in the exe too
        short &Hidden() { return m_bHidden; }
        bool IsSleeping() { return ThisCall<bool>(A::cXPerson__IsSleeping, this); }
        void SetMotive(int nMotive, float fValue) { ThisCall<void>(A::cXPerson__SetMotive, this, nMotive, fValue); }
        bool AddAction(Interaction *pAction) { return ThisCall<bool>(A::cXPerson__AddAction, this, pAction); }
        void TryChangeLots(unsigned short nLot, short nHour, short nMinute, short nMode)
        {
            ThisCall<void>(A::cXPerson__TryChangeLots, this, nLot, nHour, nMinute, nMode);
        }
        bool RunOneTickTree(Behavior *pBehavior, short nStackObjectId, short nTreeId, short *pParams)
        {
            return ThisCall<bool>(A::TreeSim__RunOneTickTree, this, pBehavior, nStackObjectId, nTreeId, pParams);
        }
        void Place(const FTilePt &pos, int nLevel, cXObject *pParent, int nSlot)          // virtual
        {
            VCall<void>(this, A::Slot_Place, &pos, nLevel, pParent, nSlot);
        }
        void ForceLocation() { VCall<void>(this, A::Slot_ForceLocation); }
    };

    // dynamic_cast<cXPerson *>(cXObject *) through the exe's own RTTI and __RTDynamicCast
    inline cXPerson *AsPerson(cXObject *pObject)
    {
        return pObject ? Call<cXPerson *>(A::RTDynamicCast, (void *)pObject, 0L, Va(A::TD_cXObject), Va(A::TD_cXPerson), 0)
                       : 0;
    }

    // the lot, the family, the neighbourhood
    class ObjectModule
    {
    public:
        union { GA_F(0x18, Vec<cXObject *>, m_Objects) GA_F(GA_O(0x80, 0x84), Vec<cXPerson *>, m_Persons) };
        cXObject *GetObject(int nId)                    // header inline in the exe (0x4558f0 is an out-of-line copy)
        {
            return nId > 0 && (unsigned)nId < m_Objects.size() + 1 ? m_Objects[nId - 1] : 0;
        }
        cXObject *GetObjectByGUID(long nGuid) { return ThisCall<cXObject *>(A::ObjectModule__GetObjectByGUID, this, nGuid); }
        cXPerson *GetPersonByGUID(long nGuid) { return ThisCall<cXPerson *>(A::ObjectModule__GetPersonByGUID, this, nGuid); }
        cXPerson *GetSelectedPerson() { return ThisCall<cXPerson *>(A::ObjectModule__GetSelectedPerson, this); }
        void SetSelectedPerson(cXPerson *p) { ThisCall<void>(A::ObjectModule__SetSelectedPerson, this, p); }
        cXPerson *AdvanceSelectedPerson() { return ThisCall<cXPerson *>(A::ObjectModule__AdvanceSelectedPerson, this); }
    };

    struct FamilyMember
    {
        union { GA_F0(long, m_nGuid) };
    };

    class Family
    {
    public:
        union { GA_F(0x10C, int, m_nFamilyId) GA_F(0x110, int, m_nHouseNumber) };
        int CountMembers() { return ThisCall<int>(A::Family__CountMembers, this); }
        FamilyMember *GetIndexedMember(int i) { return ThisCall<FamilyMember *>(A::Family__GetIndexedMember, this, i); }
    };

    class House
    {
    public:
        union { GA_F(0x10, Family *, m_pFamily) GA_F(0x14, ObjectModule *, m_pObjectModule) };
    };

    struct PersDataPair
    {
        union { GA_F0(int, m_nField) GA_F(4, int, m_nKind) };
    };

    class Neighbor
    {
    public:
        union { GA_F(0x18, int, m_nPersDataVersion) GA_A(0x74, short, m_PersonData, 256) };
        static int GetLatestPersDataVersion() { return Call<int>(A::Neighbor__GetLatestPersDataVersion); }
        static Vec<PersDataPair> &GetPersistentDataFields(int nVersion)
        {
            return *Call<Vec<PersDataPair> *>(A::Neighbor__GetPersistentDataFields, nVersion);
        }
    };

    class Neighborhood
    {
    public:
        union { GA_A(0x128, short, m_NeighborhoodData, 16) };
        int GetZoningType(int nHouse) { return ThisCall<int>(A::Neighborhood__GetZoningType, this, nHouse); }
        Family *GetFamily(int nId) { return ThisCall<Family *>(A::Neighborhood__GetFamily, this, nId); }
        Family *GetFamilyInHouse(int nHouse) { return ThisCall<Family *>(A::Neighborhood__GetFamilyInHouse, this, nHouse); }
        Neighbor *FindNeighborByGUID(long g) { return ThisCall<Neighbor *>(A::Neighborhood__FindNeighborByGUID, this, g); }
        Neighbor *FindNeighborByID(int nId) { return ThisCall<Neighbor *>(A::Neighborhood__FindNeighborByID, this, nId); }
        SimInventory *GetInventoryByNeighbourID(short nId)
        {
            return ThisCall<SimInventory *>(A::Neighborhood__GetInventoryByNeighbourID, this, nId);
        }
        int Save(long nVersion) { return ThisCall<int>(A::Neighborhood__Save, this, nVersion); }
        void UnloadHouse() { ThisCall<void>(A::Neighborhood__UnloadHouse, this); }
    };

    struct SimGlobals
    {
        union
        {
            GA_F0(short, Hour) GA_F(0x0A, short, Minute) GA_F(0x12, short, CurrentFamily)
            GA_F(0x14, short, CurrentHouse) GA_F(0x40, short, SimlessBuildMode) GA_SIZE(128)
        };
    };

    class cSimulator
    {
    public:
        union { GA_F(0x10, SimGlobals, m_Globals) GA_F(0xE1, bool, m_bLotChangePending) };
        void SetCurrentHour(int nHour, int nMinute) { ThisCall<void>(A::cSimulator__SetCurrentHour, this, nHour, nMinute); }
        bool IsCurrentLotResidential() { return ThisCall<bool>(A::cSimulator__IsCurrentLotResidential, this); }
    };

    class cSimsSettings                                 // the game's settings object (Globs)
    {
    public:
        union
        {
            GA_F0(House *, m_pGame) GA_F(0x1C, cSimulator *, m_pSimulator) GA_F(0x24, Neighborhood *, m_pNeighborhood)
            GA_F(GA_O(0x48, 0x44), int, m_nSaveVersion) GA_F(GA_O(0x68, 0x64), signed char, m_nCurrentLanguage)
        };
    };

    // the app and its UI
    class cTSWin
    {
    public:
        void UpdateCursor() { ThisCall<void>(A::cTSWin__UpdateCursor, this); }
        void SetToolTipsText(const cTSString &s) { ThisCall<void>(A::cTSWin__SetToolTipsText, this, &s); }
    };

    class cTSCursor;
    class cTSCursorManager
    {
    public:
        cTSCursor *GetStandardCursor(int nId) { return ThisCall<cTSCursor *>(A::cTSCursorManager__GetStandardCursor, this, nId); }
    };

    class cTSWinMgrW95
    {
    public:
        union { GA_F(GA_O(0x178, 0x170), cTSCursorManager *, m_pCursorMgr) };
        void SetGlobalCursor(cTSCursor *p, bool b) { ThisCall<void>(A::cTSWinMgrW95__SetGlobalCursor, this, p, b); }
    };

    class cDDDSimsView
    {
    public:
        void StartWatchCursor() { ThisCall<void>(A::cDDDSimsView__StartWatchCursor, this); }
    };

    class CPState
    {
    public:
        enum Mode { kBuild = 0, kBuy = 1, kLive = 2 };
        enum { kDirtyBuyBuild = 0x1000000 };
        union { GA_F(0x0C, int, m_dwFlags) GA_F(GA_O(0x7C, 0x64), bool, m_bBuyAndBuildDisabled) };
        void SetDirty(int nBits) { ThisCall<void>(A::CPState__SetDirty, this, nBits); }
        void DisableSave(bool b) { ThisCall<void>(A::CPState__DisableSave, this, b); }
        void HideButtonsForLocation(bool b) { ThisCall<void>(A::CPState__HideButtonsForLocation, this, b); }
    };

    class cSimsApp
    {
    public:
        union
        {
            GA_F(GA_O(0x088, 0x080), cTSWin *, m_pRootWin) GA_F(GA_O(0x08C, 0x084), cDDDSimsView *, m_pLotView)
            GA_F(GA_O(0x0C4, 0x0B0), CPState *, m_pCPState) GA_F(GA_O(0x161, 0x0F9), char, m_bReqNeighborhood)
            GA_F(GA_O(0x196, 0x132), short, m_nInputCode) GA_F(GA_O(0x198, 0x134), int, m_nInputParam)
            GA_F(GA_O(0x19C, 0x138), short, m_nMoveInFamilyNumber)
        };
        int MessageDialog(const char *pszText, const char *pszTitle, unsigned long nSet, bool b)
        {
            return ThisCall<int>(A::cSimsApp__MessageDialog, this, pszText, pszTitle, nSet, b);
        }
        void RemoveTaxiDialog() { ThisCall<void>(A::cSimsApp__RemoveTaxiDialog, this); }
        void SetChangeLotTrigger(short nHouse, int n, short m) { ThisCall<void>(A::cSimsApp__SetChangeLotTrigger, this, nHouse, n, m); }
        bool SaveGame() { return ThisCall<bool>(A::cSimsApp__SaveGame, this); }
    };

    struct HouseInfo                                    // a lot's header data (LotHouseInfo; 2003: thumbnails first)
    {
        union
        {
            GA_F(GA_O(0x10, 0x20), int, m_nFilterMask) GA_F(GA_O(0x40, 0x44), int, m_nHouseId)
            GA_F(GA_O(0x80, 0x74), int, m_nLotX) GA_F(GA_O(0x84, 0x78), int, m_nLotY) GA_SIZE(GA_O(136, 0x7C))
        };
    };

    class cWinLotBtn
    {
    public:
        union { GA_F(GA_O(0x1E0, 0x194), HouseInfo, m_HouseInfo) };
    };

#if GAME_CC
    // std::map<int, cWinLotBtn *> as laid out in the 2003 exe (SGI _Rb_tree: header node + count; node: colour, parent,
    // left, right, key, value; leaves are 0, the header's left is the leftmost node - the walk
    // cWinNeighborhoodUL::AnyLotMatchesFilter does)
    struct LotMapNode
    {
        int _M_color;
        LotMapNode *_M_parent, *_M_left, *_M_right;
        int first;
        cWinLotBtn *second;
    };
    struct LotMap
    {
        LotMapNode *_M_header;
        size_t _M_node_count;
        template <class F> void ForEach(F f) const      // in-order walk (SGI _Rb_tree_base_iterator::_M_increment), stops when f returns true
        {
            for (LotMapNode *n = _M_header->_M_left; n != _M_header;)
            {
                if (f(n->second))
                    return;
                if (n->_M_right)
                {
                    n = n->_M_right;
                    while (n->_M_left)
                        n = n->_M_left;
                }
                else
                {
                    LotMapNode *p = n->_M_parent;
                    while (n == p->_M_right)
                        n = p, p = p->_M_parent;
                    if (n->_M_right != p)
                        n = p;
                }
            }
        }
    };
#else
    // std::map<int, cWinLotBtn *> as laid out in the exe (MSVC _Tree: head node + size; node: left, parent, right,
    // colour, isnil, key, value - the walk cWinNeighborhoodUL::AnyLotMatchesFilter 0x4a6720 does)
    struct LotMapNode
    {
        LotMapNode *_Left, *_Parent, *_Right;
        char _Color, _Isnil;
        int first;
        cWinLotBtn *second;
    };
    struct LotMap
    {
        LotMapNode *_Myhead;
        size_t _Mysize;
        template <class F> void ForEach(F f) const      // in-order walk, stops when f returns true
        {
            for (LotMapNode *n = _Myhead->_Left; n != _Myhead;)
            {
                if (f(n->second))
                    return;
                if (!n->_Right->_Isnil)
                {
                    n = n->_Right;
                    while (!n->_Left->_Isnil)
                        n = n->_Left;
                }
                else
                {
                    LotMapNode *p = n->_Parent;
                    while (!p->_Isnil && n == p->_Right)
                        n = p, p = p->_Parent;
                    n = p;
                }
            }
        }
    };
#endif

    class cWinNeighborhoodUL
    {
    public:
        union
        {
            GA_A(GA_O(0x148, 0xE4), cTSWin *, m_pFilterBtns, 7) GA_F(GA_O(0x1A4, 0x148), LotMap, m_LotBtnTree)
            GA_F(GA_O(0x1EC, 0x194), int, m_nFilterMask)
        };
        void DrawFilterAt(int x, int y) { ThisCall<void>(A::cWinNeighborhoodUL__DrawFilterAt, this, x, y); }
    };

    class cWinLotPopup
    {
    public:
        union { GA_F(GA_O(0x130, 0xE0), HouseInfo *, m_pHouseInfo) };
    };

    class cWinOptions
    {
    public:
        union { GA_A(GA_O(0x134, 0xE0), cTSWin *, m_apCtrls, GA_O(59, 42)) };
    };

    class ObjTestSim
    {
    public:
        union { GA_F0(cXPerson *, m_pSim) GA_F(0x04, cXObject *, m_pObject) };
    };

    // globals and free functions
    inline cSimsSettings *Settings() { return Global<cSimsSettings *>(A::g_pAppSettings); }
    inline cSimsApp *SimsApp() { return Global<cSimsApp *>(A::g_pSimsApp); }
    inline cTSWinMgrW95 *WinMgr() { return Global<cTSWinMgrW95 *>(A::g_pWinMgr); }
    inline int VisitorControl() { return Global<int>(A::g_bVisitorControl); }
#if GAME_CC
    // inlined everywhere in the 2003 exe (as Legacy's 0x455820 / 0x45d580 do: read / store Globs::m_nCurrentLanguage)
    inline char GetLanguage() { return Settings()->m_nCurrentLanguage; }
    inline void SetLanguage(int nLang) { Settings()->m_nCurrentLanguage = (signed char)nLang; }
#else
    inline char GetLanguage() { return Call<char>(A::GetLanguage); }
    inline void SetLanguage(int nLang) { Call<void>(A::SetLanguage, nLang); }
#endif

    namespace PersonFinder
    {
        inline int GetGUID(unsigned h) { return Call<int>(A::PersonFinder__GetGUID, h); }
        inline int GetHandle(int nGuid) { return Call<int>(A::PersonFinder__GetHandle, nGuid); }
        inline int GetHandleFromPerson(cXPerson *p) { return Call<int>(A::PersonFinder__GetHandleFromPerson, p); }
        inline int CreateDowntownSim(unsigned h) { return Call<int>(A::PersonFinder__CreateDowntownSim, h); }
        inline cXPerson *GetInstance(unsigned h) { return Call<cXPerson *>(A::PersonFinder__GetInstance, h); }
        inline int GetSelected() { return Call<int>(A::PersonFinder__GetSelected); }
        inline int GetPersonInHouseIndex(int i) { return Call<int>(A::PersonFinder__GetPersonInHouseIndex, i); }
    }
}

#pragma warning(pop)

#ifndef GA_NO_USING
using namespace ga;
#endif
