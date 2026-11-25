// game_api.h - the Sims.exe classes, fields (explicit offsets) and functions (address thunks) the mod uses.
// GAME_CC=1 builds for the Complete Collection (2003, VC6 + SGI STL); otherwise the Legacy Collection.
#pragma once
#include <stddef.h>

#ifndef GAME_CC
#define GAME_CC 0
#endif
// GAME_EA=1: the Legacy Collection from the EA app - the Steam code relinked at other addresses (game_addr_ea.h is
// generated from game_addr.h by tools/port_addrs.py); classes and fields are the Legacy ones.
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
#pragma warning(disable : 4201)

#define GA_F(off, type, name) struct { char _o_##name[off]; type name; };
#define GA_F0(type, name) struct { type name; };
#define GA_A(off, type, name, n) struct { char _o_##name[off]; type name[n]; };
#define GA_SIZE(n) char _ga_size[n];

namespace ga
{
    namespace A
    {
        enum : unsigned
        {
#define GA_FN(n, va) n = va,
#define GA_DATA(n, va) n = va,
#define GA_CRT(n, va, site) n = va, n##_Site = site,
#define GA_RTTI(n, va, s) n = va,
#define GA_VTBL(n, va, slots) n = va, n##_Slots = slots,
#define GA_VSLOT(n, vt, slot, va) n = slot,
#include GA_ADDR_FILE
#undef GA_FN
#undef GA_DATA
#undef GA_CRT
#undef GA_RTTI
#undef GA_VTBL
#undef GA_VSLOT
        };
    }

    extern unsigned g_exeBase;
    inline void *Va(unsigned va) { return (void *)(va - 0x400000u + g_exeBase); }
    template <class T> inline T &Global(unsigned va) { return *(T *)Va(va); }

    template <class R, class... T> inline R Call(unsigned va, T... a) { return ((R(__cdecl *)(T...))Va(va))(a...); }
    template <class R, class... T> inline R ThisCall(unsigned va, const void *self, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))Va(va))(self, a...);
    }
    template <class R, class... T> inline R VCall(const void *self, int slot, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))(*(void ***)self)[slot])(self, a...);
    }

    template <class T> struct Vec                       // MSVC and SGI STL: the same 3 pointers
    {
        T *_Myfirst, *_Mylast, *_Myend;
        size_t size() const { return (size_t)(_Mylast - _Myfirst); }
        T &operator[](size_t i) const { return _Myfirst[i]; }
        T *begin() const { return _Myfirst; }
        T *end() const { return _Mylast; }
    };

    struct ScreenPt16 { short nY, nX; };
    struct FTilePt { int nPosY16, nPosX16; };
    struct CTilePt
    {
        signed char nX, nY, nLevel;
        CTilePt() : nX(0), nY(0), nLevel(0) {}
        CTilePt(int x, int y, int level) : nX((signed char)x), nY((signed char)y), nLevel((signed char)level) {}
        unsigned Packed() const { return (unsigned char)nX | (unsigned char)nY << 8 | (unsigned char)nLevel << 16; }
    };
    struct Rect { long left, top, right, bottom; };

    struct EventRecord
    {
        union { GA_F(0x0C, ScreenPt16, m_ptWhere) GA_SIZE(20) };
    };

    class cTSString                                     // built and freed by the exe
    {
    public:
#if GAME_CC
        union { GA_F0(char *, m_pszStart) GA_SIZE(12) };    // SGI basic_string: start, finish, end of storage
        const char *c_str() const { return m_pszStart; }
#else
        union { GA_F0(char, m_szBuf[16]) GA_F0(char *, m_pszPtr) GA_F(0x10, unsigned, m_nSize) GA_F(0x14, unsigned, m_nRes) GA_SIZE(24) };
        const char *c_str() const { return m_nRes > 15 ? m_pszPtr : m_szBuf; }
#endif
        explicit cTSString(const char *psz) { ThisCall<void>(A::cTSString__cTSString, this, psz); }
        ~cTSString() { ThisCall<void>(A::cTSString___cTSString, this); }
        cTSString(const cTSString &) = delete;
        cTSString &operator=(const cTSString &) = delete;
    };

    enum eWallSegment
    {
        kWallSeg_West = 0x01,
        kWallSeg_North = 0x02,
        kWallSeg_DiagNE_SW = 0x10,
        kWallSeg_DiagNW_SE = 0x20,
        kWallSeg_AnyDiagonal = 0x30,
    };

    struct TileWalls
    {
        unsigned int m_nWallMask;
        int m_nEdgeStyles;
        unsigned short m_nPattern[6];
        unsigned short m_nFacePattern[8];
        int m_nRotation;
        int m_nNeighborDiag;
        int GetEdgeValue(int eSegment, int eDiagSide) const
        {
            if (eSegment == 16 && !eDiagSide)
                eDiagSide = 4;
            else if (eSegment == 32 && !eDiagSide)
                eDiagSide = 1;
            int nIndex = 0;
            while (!(eSegment & 1))
            {
                eSegment >>= 1;
                nIndex++;
            }
            if (eDiagSide == 1 || eDiagSide == 4)
                nIndex++;
            else if (eDiagSide == 3)
                nIndex += 2;
            return m_nFacePattern[nIndex];
        }
        int GetDiagonalFloorPattern(int nSide) { return ThisCall<int>(A::TileWalls__GetDiagonalFloorPattern, this, nSide); }
        static int DirToWallSeg(int eDir) { return Call<int>(A::TileWalls__DirToWallSeg, eDir); }
    };
    static_assert(sizeof(TileWalls) == 44, "TileWalls");
    inline bool IsFenceStyle(int n) { return n == 2 || n == 12 || n == 13 || n == 14; }

    struct c2DArrayU16
    {
        union { GA_F(0x0C, unsigned short **, m_pRows) };
    };

    class cRotatableWorld
    {
    public:
        union
        {
            GA_A(0x20, c2DArrayU16 *, m_pFloorsByLevel, 3)
            GA_F(0x70, int, m_nRotMinX) GA_F(0x74, int, m_nRotMaxX) GA_F(0x78, int, m_nRotMinY) GA_F(0x7C, int, m_nRotMaxY)
            GA_F(0x84, int, m_nRotation) GA_SIZE(0x88)
        };
        bool IsInLot(const CTilePt &pt) const
        {
            return pt.nX >= m_nRotMinX + 1 && pt.nX <= m_nRotMaxX - 1 && pt.nY >= m_nRotMinY + 1 && pt.nY <= m_nRotMaxY - 1;
        }
        int GetToViewRotation() const { return m_nRotation; }
        CTilePt Rotate(const CTilePt &pt) const         // view -> world tile (the game's 64 x 64 rotation table)
        {
            int x = pt.nX, y = pt.nY;
            switch (-m_nRotation & 3)
            {
            case 1: return CTilePt(63 - y, x, pt.nLevel);
            case 2: return CTilePt(63 - x, 63 - y, pt.nLevel);
            case 3: return CTilePt(y, 63 - x, pt.nLevel);
            }
            return pt;
        }
        TileWalls GetWall(const CTilePt &pt)
        {
            TileWalls w;
            ThisCall<TileWalls *>(A::cRotatableWorld__GetWall, this, &w, pt.Packed());
            return w;
        }
        void RotateCoords(int *pX, int *pY, int nRotation, int nSize)
        {
            ThisCall<void>(A::cRotatableWorld__RotateCoords, this, pX, pY, nRotation, nSize);
        }
    };

    struct WallSegment
    {
        char m_nFlags;
        unsigned char m_nStyle;
        unsigned short m_anPattern[2];
    };
    class Wall
    {
    public:
        union
        {
            GA_F(0x04, CTilePt, m_TileA) GA_F(0x0A, signed char, m_nStepX) GA_F(0x0B, signed char, m_nStepY)
            GA_F(0x10, int, m_nDir) GA_F(0x30, Vec<WallSegment>, m_SegmentsA) GA_F(0x48, char, m_bIconic)
        };
        static CTilePt VertexToTile(const CTilePt &vertex, int nDir)
        {
            CTilePt t;
#if GAME_CC
            ThisCall<CTilePt *>(A::Wall__VertexToTile, 0, &t, &vertex, nDir);   // a member there (this unused)
#else
            Call<CTilePt *>(A::Wall__VertexToTile, &t, &vertex, nDir);
#endif
            return t;
        }
    };
    class WallManager
    {
    public:
        union { GA_A(0x10, Vec<Wall *> *, m_pWallLists, 2) };
        static WallManager *Get() { return Call<WallManager *>(A::WallManager__GetWallManager); }
    };

    // header-inline / inlined by VC6: OBJD sort fields through the product's selector
    struct OBJD
    {
        union
        {
            GA_F(0x4E, short, room_sort) GA_F(0x50, short, function_sort) GA_F(0xBC, short, function_subsort)
            GA_F(0xBE, short, downtown_sort) GA_F(0xC2, short, vacation_sort) GA_F(0xC6, short, community_sort)
            GA_F(0xCE, short, studiotown_sort) GA_F(0xD0, short, magictown_sort)
        };
    };
    class ObjSelector
    {
    public:
        union { GA_F(0x60, OBJD *, m_pObjd) GA_F(0x8C, ObjSelector *, m_pMasterSelector) };
    };

    class cXObject
    {
    public:
        union { GA_F(0x0F4, int, m_nPosY16) GA_F(0x0F8, int, m_nPosX16) GA_F(GA_O(0x120, 0x11C), ObjSelector *, m_pSelector) };
    };
    class cXMTObject
    {
    public:
        union { GA_F(GA_O(0x120, 0x11C), ObjSelector *, m_pSelector) GA_F(GA_O(0x194, 0x190), cXMTObject *, m_pMaster) };
    };

    inline void *DynCast(cXObject *p, unsigned tdTo)
    {
        return p ? Call<void *>(A::RTDynamicCast, (void *)p, 0L, Va(A::TD_cXObject), Va(tdTo), 0) : 0;
    }

#if !GAME_CC
    struct ObjectHandle
    {
        cXObject *Resolve() const { return ThisCall<cXObject *>(A::ObjectHandle__Resolve, this); }
    };
    struct PickRecord
    {
        void *m_pMeshItem;
        void *m_pDrawItem;
    };
    struct PickHitResult
    {
        float m_fDepth;
        PickRecord *m_pRecord;
    };
    struct RenderContext
    {
        bool HitTestScreenPoint(int x, int y, PickHitResult *pHit)
        {
            return ThisCall<bool>(A::RenderContext__HitTestScreenPoint, this, x, y, pHit);
        }
    };
#endif
    class HouseViewer
    {
    public:
        union
        {
            GA_F(0x028, Rect, m_ViewRect) GA_F(0x070, int, m_nTileHeight)
            GA_A(GA_O(0x0D4, 0x0B8) + 0x6C, int, m_anLevelHeightPx, 3)       // m_ZBufferLookup
#if !GAME_CC
            GA_F(0x5C4, RenderContext, m_RenderContext)
#endif
        };
    };

    class cSoundPlayer
    {
    public:
        bool PlayBySource(const char *pszName, short nSource) { return ThisCall<bool>(A::cSoundPlayer__PlayBySource, this, pszName, nSource); }
    };

    class cSimsSettings
    {
    public:
        union { GA_F(0x04, HouseViewer *, m_pHouseViewer) GA_F(0x10, cSoundPlayer *, m_pSoundSys) GA_F(GA_O(0x68, 0x64), signed char, m_nCurrentLanguage) };
    };

    struct GViewer
    {
        static int GetScale() { return Call<int>(A::GViewer_GetScale); }
        static int GetTileHeight();
        static FTilePt PointToTile(const ScreenPt16 &pt, bool bSkipPick)
        {
            FTilePt r;
            Call<FTilePt *>(A::GViewer_PointToTile, &r, &pt, bSkipPick);
            return r;
        }
        static ScreenPt16 TileToPoint(const FTilePt *p, int nLevel) { return Call<ScreenPt16>(A::GViewer_TileToPoint, p, nLevel); }
        static ScreenPt16 SubTileToPoint(const FTilePt *p, int nLevel) { return Call<ScreenPt16>(A::GViewer_SubTileToPoint, p, nLevel); }
#if GAME_CC
        // the id of the object whose screen rectangle holds pt (mode 0: any object), 0 for none
        static int PointToObjectID(ScreenPt16 pt, int nLevel) { return (short)Call<int>(A::GViewer_PointToObjectID, &pt, nLevel, 0); }
#endif
    };
    static_assert(sizeof(ScreenPt16) == 4 && sizeof(FTilePt) == 8 && sizeof(CTilePt) == 3, "point sizes");

    enum eProductKind { kProductEntry_Object = 1, kProductEntry_Floor = 2, kProductEntry_Wall = 3, kProductEntry_WallPattern = 4 };
    enum eCatalogGroup
    {
        kCatalogGroup_Room, kCatalogGroup_Function, kCatalogGroup_Downtown, kCatalogGroup_Vacation, kCatalogGroup_Community,
        kCatalogGroup_Studiotown, kCatalogGroup_Magictown,
    };

    class Product
    {
    public:
        union
        {
            GA_F(0x04, char, m_StrNameAt) GA_F(GA_O(0x3C, 0x24), int, m_nKind) GA_F(GA_O(0x40, 0x28), ObjSelector *, m_pSelector)
            GA_F(GA_O(0x54, 0x3C), int, m_nWallId) GA_F(GA_O(0x58, 0x40), int, m_nWallStyle) GA_F(GA_O(0x5C, 0x44), int, m_nFloorId)
        };
        bool TestArchTool(int n) { return ThisCall<bool>(A::Product__TestArchTool, this, n); }
        bool TestHasFunction(int nMask, int nSubMask) { return ThisCall<bool>(A::Product__TestHasFunction2, this, nMask, nSubMask); }
        const cTSString &GetProductName() const { return *reinterpret_cast<const cTSString *>(&m_StrNameAt); }
        const OBJD *Objd() const { return m_nKind == kProductEntry_Object ? m_pSelector->m_pObjd : 0; }
        bool HasRoomSort() const { return Objd() && Objd()->room_sort; }
        bool HasDowntownSort() const { return Objd() && Objd()->downtown_sort; }
        bool HasVacationSort() const { return Objd() && Objd()->vacation_sort; }
        bool HasCommunitySort() const { return Objd() && Objd()->community_sort; }
        bool HasStudiotownSort() const { return Objd() && Objd()->studiotown_sort; }
        bool HasMagictownSort() const { return Objd() && Objd()->magictown_sort; }
        bool TestInRoom(int n) const { return Objd() && (Objd()->room_sort & n); }
        bool TestInDowntownRoom(int n) const { return Objd() && (Objd()->downtown_sort & n); }
        bool TestInVacationRoom(int n) const { return Objd() && (Objd()->vacation_sort & n); }
        bool TestInCommunityRoom(int n) const { return Objd() && (Objd()->community_sort & n); }
        bool TestInStudiotownRoom(int n) const { return Objd() && (Objd()->studiotown_sort & n); }
        bool TestInMagictownRoom(int n) const { return Objd() && (Objd()->magictown_sort & n); }
        bool TestHasFunction(int n) const { return Objd() && (Objd()->function_sort & n); }
    };

    class Catalog
    {
    public:
        union { GA_F(0x04, Vec<Product *>, m_Products) };
        Product **begin() const { return m_Products.begin(); }
        Product **end() const { return m_Products.end(); }
        Product *FindProduct(ObjSelector *p) { return ThisCall<Product *>(A::Catalog__FindProduct, this, p); }
    };

    struct cWinCatalogStatic                            // cWinCatalog's index -> sort bit tables
    {
        static int Bit(int i) { return i >= 0 && i < 8 ? 1 << i : 0; }
        static int GetRoomMask(int i)
        {
            static const int k[8] = { 8, 32, 2, 128, 1, 4, 16, 64 };
            return i >= 0 && i < 8 ? k[i] : 0;
        }
        static int GetFunctionMask(int i)
        {
            static const int k[8] = { 1, 2, 32, 8, 4, 16, 128, 64 };
            return i >= 0 && i < 8 ? k[i] : 0;
        }
        static int GetFunctionSubMask(int i)
        {
            static const int k[8] = { 1, 2, 4, 8, 32, 64, 144, 255 };
            return i >= 0 && i < 8 ? k[i] : 0;
        }
        static int GetDowntownMask(int i) { return Bit(i); }
        static int GetVacationMask(int i) { return Bit(i); }
        static int GetCommunityMask(int i) { return Bit(i); }
        static int GetStudiotownMask(int i) { return Bit(i); }
        static int GetMagictownMask(int i) { return Bit(i); }
    };

    class cTool
    {
    public:
        enum { kSize = 44 };
        static int GetCurrentLevel() { return Call<int>(A::cTool__GetCurrentLevel); }
        static void SetToolTip(const char *psz) { Call<void>(A::cTool__SetToolTip, psz); }
        static void SetCursor(int nCursor) { Call<void>(A::cTool__SetCursor, nCursor); }
        void DoDeniedSound() { ThisCall<void>(A::cTool__DoDeniedSound, this); }
#if GAME_CC
        static cXObject *ToolGetObject(int nId) { return Call<cXObject *>(A::cTool__ToolGetObject, nId); }
#endif
    };
    enum { kCursor_Finger = 4 };

    class CPState
    {
    public:
        enum Mode { kBuild = 0, kBuy = 1, kLive = 2 };
        enum { kDirtyTool = 0x10, kDirtyArchSubtool = 0x20, kDirtyCatalog = 0x400 };
        union
        {
            GA_F(0x08, int, m_nCPMode) GA_F(0x0C, int, m_dwFlags) GA_F(GA_O(0x64, 0x4C), cTool *, m_pCurrentTool)
            GA_F(GA_O(0x7C, 0x64), bool, m_bBuyAndBuildDisabled) GA_F(GA_O(0x80, 0x68), int, m_nArchTool)
            GA_A(GA_O(0x84, 0x6C), Product *, m_pModeProducts, 12) GA_F(GA_O(0xB4, 0x9C), int, m_nShoppingState)
            GA_F(GA_O(0xB8, 0xA0), int, m_nCatalogGroup) GA_A(GA_O(0xBC, 0xA4), int, m_CatalogSel, 15)
            GA_F(GA_O(0xF8, 0xE0), int, m_nCatalogPage)
        };
        Catalog *GetCatalog() { return ThisCall<Catalog *>(A::CPState__GetCatalog, this); }
        int GetMode() const { return m_nCPMode; }
        int GetCPMode() { return ThisCall<int>(A::CPState__GetCPMode, this); }
        void SetCPMode(int nMode, bool bQuiet) { ThisCall<void>(A::CPState__SetCPMode, this, nMode, bQuiet); }
        void SetMode(int nMode, bool bForce) { ThisCall<void>(A::CPState__SetMode, this, nMode, bForce); }
        int ArchGetCurrentTool() const { return m_nArchTool; }
        void ArchSetCurrentTool(int n) { ThisCall<void>(A::CPState__ArchSetCurrentTool, this, n); }
        void ResetPreviousPages() { ThisCall<void>(A::CPState__ResetPreviousPages, this); }
        void ShopSetSort(int nGroup) { ThisCall<void>(A::CPState__ShopSetSort, this, nGroup); }
        void ShopSetState(int n) { ThisCall<void>(A::CPState__ShopSetState, this, n); }
        void UpdateWorldFromCPState(unsigned nMask) { ThisCall<void>(A::CPState__UpdateWorldFromCPState, this, nMask); }
        void MouseEnteredWorld() { ThisCall<void>(A::CPState__MouseEnteredWorld, this); }
        bool IsBuyAndBuildDisabled() const { return m_bBuyAndBuildDisabled; }
        void SetCurrentTool(cTool *p, int nSubId) { ThisCall<void>(A::CPState__SetCurrentTool, this, p, nSubId); }
    };

    typedef unsigned int TSMsgParam;
    enum { kTSMsg_Command = 3, kWinMsg_ButtonClick = 1, kWinFlag_Visible = 0x1, kBtnTrigger_OnPress = 1 };

    class cTSWin
    {
    public:
        enum { kSize = GA_O(0x128, 0xD4) };
        union
        {
            GA_F(GA_O(0x4C, 0x48), cTSWin *, m_pMsgTarget) GA_F(GA_O(0xD4, 0x7C), Rect, m_rcArea)
            GA_F(GA_O(0xF4, 0x9C), unsigned long, m_nFlags) GA_SIZE(kSize)
        };
        bool GetFlag(unsigned long n) const { return (m_nFlags & n) != 0; }
        void SetToolTipsText(const cTSString &s) { ThisCall<void>(A::cTSWin__SetToolTipsText, this, &s); }
        void TSWinMoveTo(long x, long y) { ThisCall<void>(A::cTSWin__TSWinMoveTo, this, x, y); }
        bool ChildAdd(cTSWin *p) { return VCall<bool>(this, A::Slot_Win_ChildAdd, p); }
        void HideWindow() { VCall<void>(this, A::Slot_Win_HideWindow); }
        void InvalidateSelf() { VCall<void>(this, A::Slot_Win_InvalidateSelf); }
    };

    class cTSWinBtn : public cTSWin
    {
    public:
        enum { kSize = GA_O(0x1E0, 0x194) };
        union
        {
            GA_F(GA_O(0x144, 0xF4) - cTSWin::kSize, long, m_nCommandTrigger) GA_F(GA_O(0x148, 0xF8) - cTSWin::kSize, long, m_nMode)
            GA_SIZE(kSize - cTSWin::kSize)
        };
        void SetState(long n) { ThisCall<void>(A::cTSWinBtn__SetState, this, n); }
        // the button's art by resource id (UIGraphics *.RT); false when the id is unknown
        bool SetImageById(long nResId, long nCols, long nRows);
    };
    static_assert(sizeof(cTSWin) == cTSWin::kSize && sizeof(cTSWinBtn) == cTSWinBtn::kSize, "window sizes");

    class cWinArch : public cTSWin
    {
    public:
        union
        {
            GA_F(GA_O(0x12C, 0xD8) - cTSWin::kSize, int, m_nMode) GA_F(GA_O(0x138, 0xE4) - cTSWin::kSize, cTSWinBtn *, m_pUndoBtn)
            GA_F(GA_O(0x13C, 0xE8) - cTSWin::kSize, cTSWinBtn *, m_pRedoBtn)
        };
        void UpdateViewFromCPState(CPState *p) { ThisCall<void>(A::cWinArch__UpdateViewFromCPState, this, p); }
    };
    class cWinCatalog : public cTSWin
    {
    public:
        union { GA_F(GA_O(0x4B8, 0x464) - cTSWin::kSize, int, m_nItemsPerPage) };
        void UpdateViewFromCPState(CPState *p) { ThisCall<void>(A::cWinCatalog__UpdateViewFromCPState, this, p); }
        void SetProduct(Product *p) { ThisCall<void>(A::cWinCatalog__SetProduct, this, p); }
    };
    class cWinCPanel
    {
    public:
        union { GA_F(GA_O(0x134, 0xE0), cWinCatalog *, m_pCatalog) GA_F(GA_O(0x13C, 0xE8), cWinArch *, m_pArch) };
    };
    class cDDDSimsView
    {
    public:
        union { GA_F(GA_O(0x164, 0x11C), CPState *, m_pCPState) };
    };
    class cSimsApp
    {
    public:
        union
        {
            GA_F(GA_O(0x8C, 0x84), cDDDSimsView *, m_pLotView) GA_F(GA_O(0x90, 0x88), cWinCPanel *, m_pCPanel)
            GA_F(GA_O(0xC4, 0xB0), CPState *, m_pCPState)
        };
    };

    class cTSCursor;
    class cTSCursorManager
    {
    public:
        cTSCursor *GetCursor(unsigned long n) { return ThisCall<cTSCursor *>(A::cTSCursorManager__GetCursor, this, n); }
        bool AddCursorFromFile(const char *pszPath, unsigned long n);
    };
    class cTSWinMgrW95
    {
    public:
        union { GA_F(GA_O(0x178, 0x170), cTSCursorManager *, m_pCursorMgr) };
        void GetCursorScreenPosition(long &x, long &y);
        cTSWin *GetWindowFromPoint(long x, long y) { return ThisCall<cTSWin *>(A::cTSWinMgrW95__GetWindowFromPoint, this, x, y); }
        void PostMessageToTarget(cTSWin *p, unsigned long nClass, TSMsgParam a, TSMsgParam b, TSMsgParam c)
        {
            ThisCall<void>(A::cTSWinMgrW95__PostMessageToTarget, this, p, nClass, a, b, c);
        }
    };

    inline cSimsSettings *Settings() { return Global<cSimsSettings *>(A::g_pAppSettings); }
    inline cSimsApp *SimsApp() { return Global<cSimsApp *>(A::g_pSimsApp); }
    inline cTSWinMgrW95 *WinMgr() { return Global<cTSWinMgrW95 *>(A::g_pWinMgr); }
    inline cRotatableWorld *Lot() { return Global<cRotatableWorld *>(A::g_pLot); }

    inline int GViewer::GetTileHeight() { return Settings()->m_pHouseViewer->m_nTileHeight; }

#if GAME_CC
    // 2003: images are raw ref-counted cTSBuffer pointers; SetImage takes its own reference. Ours is kept (never
    // released), so the art outlives the button whatever SetImage decides about ownership.
    inline bool cTSWinBtn::SetImageById(long nResId, long nCols, long nRows)
    {
        void *pBuffer = 0;
        Call<bool>(A::Image_LoadById, nResId, &pBuffer, 4);
        if (!pBuffer)
            return false;
        VCall<bool>(this, A::Slot_Win_SetImage, pBuffer, nCols, nRows);
        return true;
    }
    // 2003 has no AddCursor(path): a cTSCursor (LoadCursorFromFileA) from the game's heap, owned by the manager
    inline bool cTSCursorManager::AddCursorFromFile(const char *pszPath, unsigned long n)
    {
        void *pCursor = Call<void *>(A::operator_new, 0x18u);
        if (!pCursor)
            return false;
        cTSString path(pszPath);
        ThisCall<void *>(A::cTSCursor__cTSCursor, pCursor, &path);
        if (!((void **)pCursor)[1])                     // no HCURSOR
        {
            VCall<void *>(pCursor, 0, 1u);              // its scalar deleting destructor
            return false;
        }
        return ThisCall<bool>(A::cTSCursorManager__AddCursor, this, pCursor, n);
    }
    // 2003 has no GetCursorScreenPosition: the cursor in the client area of the game's window
    inline void cTSWinMgrW95::GetCursorScreenPosition(long &x, long &y)
    {
        POINT pt = { 0, 0 };
        GetCursorPos(&pt);
        if (HWND h = GetActiveWindow())
            ScreenToClient(h, &pt);
        x = pt.x;
        y = pt.y;
    }
#else
    // RefHandle<ImageRes>: a 4-byte ref-counted handle, returned through a hidden pointer and passed to SetImage by value
    // (the callee releases its copy, so it is moved in)
    inline bool cTSWinBtn::SetImageById(long nResId, long nCols, long nRows)
    {
        void *hImage = 0;
        Call<void *>(A::Image_LoadById, &hImage, nResId, Va(A::g_ImageResDescriptor));
        if (!hImage)
            return false;
        VCall<bool>(this, A::Slot_Win_SetImage, hImage, nCols, nRows);
        return true;
    }
    inline bool cTSCursorManager::AddCursorFromFile(const char *pszPath, unsigned long n)
    {
        cTSString path(pszPath);
        return ThisCall<bool>(A::cTSCursorManager__AddCursor, this, &path, n);
    }
    inline void cTSWinMgrW95::GetCursorScreenPosition(long &x, long &y)
    {
        ThisCall<void>(A::cTSWinMgrW95__GetCursorScreenPosition, this, &x, &y);
    }
#endif

    enum eSimsLanguage8
    {
        LANG8_USEnglish = 1, LANG8_UKEnglish, LANG8_French, LANG8_German, LANG8_Italian, LANG8_Spanish, LANG8_Dutch,
        LANG8_Danish, LANG8_Swedish, LANG8_Norwegian, LANG8_Finnish, LANG8_Hebrew, LANG8_Russian, LANG8_Portuguese,
        LANG8_Japanese, LANG8_Polish, LANG8_SimplifiedChinese, LANG8_TraditionalChinese, LANG8_Thai, LANG8_Korean,
    };
}

#pragma warning(pop)

#ifndef GA_NO_USING
using namespace ga;
#endif
