// multistory.cpp - mods\multistory_steam.dll (Steam) / multistory_ea.dll (EA app, GAME_EA=1): lots with 4 stories
// (SimsModLoader mod).
// The game is reached through ms_game_api.h (generated). Stories 3-4 live in one side block per class (g_blk*):
// msstubs.asm redirects the per-story array accesses there, ms_patches.h raises the story limits, ms_pick.asm picks
// the clicked story; the hooks below do the rest.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#include "simsmod.h"
#ifndef GAME_EA
#define GAME_EA 0
#endif
#if GAME_EA
#include "ms_game_api_ea.h"     // ported by tools/port_multistory.py
#include "ms_patches_ea.h"
#include "ms_va_ea.h"
#define MS_EXE_SPAN 0xc00000
#else
#include "ms_game_api.h"
#include "ms_patches.h"
#define MSVA_IconDownPush 0x4f583b
#define MSVA_IconUpPush   0x4f588f
#define MSVA_PickA        0x55f933
#define MSVA_PickB        0x55f94b
#define MSVA_PickRetA     0x55f946
#define MSVA_PickRetB     0x55f950
#define MS_EXE_SPAN 0x800000
#endif

extern "C" __declspec(dllexport) const unsigned SimsMod_Games = GAME_EA ? SIMSMOD_GAME_LEGACY_EA : SIMSMOD_GAME_LEGACY;

extern "C" IMAGE_DOS_HEADER __ImageBase;
unsigned int ms_exe_base;                           // used by ms_game_api.h's wrappers
static const SimsLoaderApi *g_api;
#define ModLog (g_api->Log)

enum { kMaxStory = 4 };
enum { kClsWorld = 0, kClsWallMgr = 1, kClsRoomMgr = 2 };

// ---- the game's heap: everything this DLL allocates may be freed by the game and vice versa -------------------------
static unsigned ExeBase() { if (!ms_exe_base) ms_exe_base = (unsigned)(UINT_PTR)GetModuleHandleA(0); return ms_exe_base; }
void *__cdecl operator new(size_t n) { ExeBase(); return MS_operator_new(n); }
void *__cdecl operator new[](size_t n) { return operator new(n); }
void __cdecl operator delete(void *p) { if (p) { ExeBase(); MS_free(p); } }
void __cdecl operator delete[](void *p) { operator delete(p); }
void __cdecl operator delete(void *p, size_t) { operator delete(p); }
void __cdecl operator delete[](void *p, size_t) { operator delete(p); }

// ---- typed access to game memory (every type is checked against the size measured from the real headers) ------------
#define FLD(T, p, name) (*(T *)((char *)(p) + MSO_##name))
#define CHECK(T, name) static_assert(sizeof(T) == MSZ_##name, #name " type size")
CHECK(int, cFixedWorld_m_nSize); CHECK(unsigned char, cFixedWorld_m_bHasBeenTo2ndLevel); CHECK(unsigned char, cFixedWorld_m_bIs2ndLevelFloorable);
CHECK(int, WallManager_m_nRotation); CHECK(int[2], WallManager_m_bWallsDirty); CHECK(int[2], WallManager_m_bWallpaperDirty);
CHECK(int, RoomManager_m_nDirtyLevelMask); CHECK(void *, House_m_pFixedWorld); CHECK(int, HouseStats_m_nFloors);
CHECK(unsigned int, CPState_m_dwFlags); CHECK(void *, cWinViewControl_m_pCPState); CHECK(void *, cWinViewControl_m_pFirstStoryBtn);
CHECK(void *, cWinViewControl_m_pSecondStoryBtn); CHECK(void *, cDDDSimsView_m_pCPState); CHECK(int, cDDDSimsView_m_nKeyDepth);
CHECK(void *, cDDDSimsView_m_pEdgeScroller); CHECK(void *, cSimsSettings_m_pSoundSys); CHECK(void *, cSimsSettings_m_pGame);
CHECK(void *, cXMTObject_m_pMaster); CHECK(void *, cXMTObject_m_pNextPart); CHECK(int, cXMTObject_m_nOffsetLevel); CHECK(int, cXMTObject_m_nLevel);
CHECK(void *, cXObject_m_pModule); CHECK(void *, cXObject_m_pSelector); CHECK(void *, ObjSelector_m_pObjd); CHECK(short, OBJD_build_mode_type);
CHECK(short, StackElem_m_nStackObjectId); CHECK(unsigned char[8], BehaviorNodeParam_m_Op); CHECK(short[4], BehaviorNodeParam_m_Param);
CHECK(int, _c2DArray_m_nHeight); CHECK(void *, _c2DArray_m_pRows); CHECK(unsigned char, cRoofTile_m_nFlags);
CHECK(unsigned char, WallTile_m_nWallMask); CHECK(int, Vertex_m_nRefCount); CHECK(unsigned char, Vertex_m_nConfig);
CHECK(void *[4], Vertex_m_pWalls); CHECK(int, Wall_m_nRefCount); CHECK(void *, ObjectIterator_m_pCur);
typedef std::map<unsigned short, void *> RoomMap;   // RoomManager::RoomMap (std::map<unsigned short, Room *>)
CHECK(RoomMap, RoomManager_m_Rooms);

struct MsTile { signed char nX, nY, nLevel; };      // CTilePt
static_assert(sizeof(MsTile) == MSS_CTilePt && MSO_CTilePt_nX == 0 && MSO_CTilePt_nY == 1 && MSO_CTilePt_nLevel == 2, "CTilePt");
static_assert(MSS_VertexConfig == 1 && MSS_cRoofTile == 2 && MSS_WallTile == 14, "element sizes");

template <class T> static inline T &Glob(unsigned rva) { return *(T *)MS_EXE(rva); }
static inline void *World() { return Glob<void *>(MSG_g_pWorld); }
static inline void *AppSettings() { return Glob<void *>(MSG_g_pAppSettings); }

// 2D arrays (_c2DArray): rows[x][y]
template <class T> static inline T &At(void *arr, int x, int y) { return ((T **)FLD(void *, arr, _c2DArray_m_pRows))[x][y]; }
static inline int ArrHeight(void *arr) { return FLD(int, arr, _c2DArray_m_nHeight); }
static void *NewArray(void *(*ctor)(void *, int))
{
    void *p = MS_operator_new(MSS__c2DArray);
    ctor(p, 64);                                     // the exe's cArray1<T>(int): size 64 x 64, cleared
    return p;
}
static void DeleteArray(void *p) { if (p) { MS__c2DArray_dtor(p); MS_free(p); } }   // _c2DArray has no vtable
// LightOffsetGrid: a std::vector<LightEntry> (4-byte entries, empty = 0x7F7F7F7F) of 64 * 64 cells
struct MsLightGrid { unsigned *pFirst, *pLast, *pEnd; };
static void *NewLightGrid()
{
    MsLightGrid *g = (MsLightGrid *)MS_operator_new(sizeof(MsLightGrid));
    g->pFirst = (unsigned *)MS_operator_new(4096 * 4);
    for (int i = 0; i < 4096; i++) g->pFirst[i] = 0x7F7F7F7F;
    g->pLast = g->pEnd = g->pFirst + 4096;
    return g;
}
static void FillLightGrid(void *p) { MsLightGrid *g = (MsLightGrid *)p; for (unsigned *e = g->pFirst; e < g->pLast; e++) *e = 0x7F7F7F7F; }
static void DeleteLightGrid(void *p) { if (p) { MS_free(((MsLightGrid *)p)->pFirst); MS_free(p); } }
// virtual call through the object's vtable (thiscall as fastcall with a dummy edx)
template <class R, class... A> static inline R VCall(void *self, unsigned off, A... a)
{
    typedef R (__fastcall *PFN)(void *, void *, A...);
    return ((PFN)(*(void ***)self)[off / 4])(self, 0, a...);
}
static void DeleteRoof(void *p) { if (p) VCall<void *>(p, MSV_cRoofLayer_dtor, 1u); }   // scalar deleting dtor
static inline void *RoofArr(void *roof) { return (char *)roof + MSB_cRoofLayer__c2DArray; }

// ---- side storage ------------------------------------------------------------------------------------------------
alignas(16) static unsigned char g_blkWorld[0x100];
alignas(16) static unsigned char g_blkWallMgr[0x80];
alignas(16) static unsigned char g_blkRoomMgr[0x80];
static_assert(MSS_cFixedWorld + 8 <= sizeof g_blkWorld && MSS_WallManager + 8 <= sizeof g_blkWallMgr &&
              MSS_RoomManager + 8 <= sizeof g_blkRoomMgr, "blocks hold the story 3-4 slots");

// story s (1..4) of a per-story 4-byte array whose slot 0 is at `off`
static inline void *&Slot(void *inst, unsigned char *blk, unsigned off, int s)
{
    return *(void **)((s <= 2 ? (unsigned char *)inst : blk) + off + 4 * (s - 1));
}
#define W(w, field, s) Slot((w), g_blkWorld, MSO_cFixedWorld_##field, (s))
#define WM_VL(m, s) (*(VertexVec **)&Slot((m), g_blkWallMgr, MSO_WallManager_m_pVertexLists, (s)))
#define WM_WL(m, s) (*(WallVec **)&Slot((m), g_blkWallMgr, MSO_WallManager_m_pWallLists, (s)))
#define WM_DIRTY(m, field, s) (*(int *)&Slot((m), g_blkWallMgr, MSO_WallManager_##field, (s)))
#define RM_SHADE(m, s) Slot((m), g_blkRoomMgr, MSO_RoomManager_m_pShade, (s))

// ---- the stubs (msstubs.asm) and the story-pick stubs (ms_pick.asm) ------------------------------------------------
struct MsSite { unsigned int rva, winLen, padLen; void *pStub; const unsigned char *pSig; };
extern "C" unsigned int ms_inst[3];            // the registered instance per class (the base a stub accepts)
extern "C" unsigned int ms_shadow[3];          // its block
extern "C" unsigned int ms_ret[];              // live return address per stub
extern "C" const MsSite ms_sites[];
extern "C" const unsigned int ms_site_count;
extern "C" void ms_pick_a();
extern "C" void ms_pick_b();
extern "C" unsigned int ms_pick_ret_a, ms_pick_ret_b;

// ---- the wall manager's list elements: intrusive refcounted pointers (as in WallManager.cpp) ----------------------
static void ReleaseVertex(void *p)
{
    if (p && --FLD(int, p, Vertex_m_nRefCount) == 0) { MS_Vertex_dtor(p); MS_free(p); }
}
static void ReleaseWall(void *p)
{
    if (p && --FLD(int, p, Wall_m_nRefCount) == 0) VCall<void *>(p, MSV_Wall_dtor, 1u);
}
struct VertexPtr
{
    void *m_p;
    VertexPtr() : m_p(0) {}
    VertexPtr(void *p) : m_p(p) { if (m_p) ++FLD(int, m_p, Vertex_m_nRefCount); }
    VertexPtr(const VertexPtr &o) : m_p(o.m_p) { if (m_p) ++FLD(int, m_p, Vertex_m_nRefCount); }
    ~VertexPtr() { ReleaseVertex(m_p); }
    VertexPtr &operator=(const VertexPtr &o)
    {
        if (o.m_p) ++FLD(int, o.m_p, Vertex_m_nRefCount);
        ReleaseVertex(m_p); m_p = o.m_p; return *this;
    }
};
struct WallPtr
{
    void *m_p;
    WallPtr() : m_p(0) {}
    WallPtr(void *p) : m_p(p) { if (m_p) ++FLD(int, m_p, Wall_m_nRefCount); }
    WallPtr(const WallPtr &o) : m_p(o.m_p) { if (m_p) ++FLD(int, m_p, Wall_m_nRefCount); }
    ~WallPtr() { ReleaseWall(m_p); }
    WallPtr &operator=(const WallPtr &o)
    {
        if (o.m_p) ++FLD(int, o.m_p, Wall_m_nRefCount);
        ReleaseWall(m_p); m_p = o.m_p; return *this;
    }
};
typedef std::vector<VertexPtr> VertexVec;           // WallManager::VertexVec
typedef std::vector<WallPtr> WallVec;               // WallManager::WallVec
static bool VertexIso(const VertexPtr &a, const VertexPtr &b) { return MS_VertexPtr_IsoCompare((void *)&a, (void *)&b); }
static bool WallIso(const WallPtr &a, const WallPtr &b) { return MS_WallPtr_IsoCompare((void *)&a, (void *)&b); }

// ---- hooking helpers ---------------------------------------------------------------------------------------------
static int g_nFailed;
// bisecting aid: MULTISTORY_SKIP=stubs,patches,pick,hooks,stubs:<from>-<to>,patches:<from>-<to>,<detour name part>
static char g_szSkip[512];
static bool Skip(const char *what) { return g_szSkip[0] && strstr(g_szSkip, what) != 0; }
static bool SkipRange(const char *kind, unsigned i)
{
    char key[32]; sprintf(key, "%s:", kind);
    const char *p = strstr(g_szSkip, key);
    if (!p) return false;
    unsigned a = 0, b = 0;
    if (sscanf(p + strlen(key), "%u-%u", &a, &b) != 2) return false;
    return i >= a && i <= b;
}
// detour the exe function at <hex> (MS_SIG_<hex> in ms_game_api.h) to `hook`; *ppOrig = trampoline
#define MS_DETOUR(va, name, hook, orig) Detour_(name, &MS_SIG_##va, (void *)(hook), (void **)(orig))
static void Detour_(const char *name, const OrigSig *sig, void *hook, void **ppOrig)
{
    if (Skip("hooks") || (g_szSkip[0] && strstr(g_szSkip, name))) { ModLog("[multistory] skipped: %s", name); return; }
    void *tramp = 0;
    if (!g_api->Detour(name, sig, hook, &tramp)) { ModLog("[multistory] DETOUR FAILED: %s", name); g_nFailed++; return; }
    if (ppOrig) *ppOrig = tramp;
}
static unsigned char *Exe(unsigned int rva) { return (unsigned char *)(g_api->exeBase + rva); }
static bool WriteExe(unsigned int rva, const void *p, unsigned n)
{
    DWORD old;
    unsigned char *dst = Exe(rva);
    if (!VirtualProtect(dst, n, PAGE_EXECUTE_READWRITE, &old)) return false;
    memcpy(dst, p, n);
    VirtualProtect(dst, n, old, &old);
    FlushInstructionCache(GetCurrentProcess(), dst, n);
    return true;
}
static bool WriteJmp(unsigned int rva, void *target, unsigned total)
{
    unsigned char buf[64];
    if (total < 5 || total > sizeof buf) return false;
    buf[0] = 0xE9;
    *(int *)(buf + 1) = (int)((unsigned char *)target - (Exe(rva) + 5));
    memset(buf + 5, 0xCC, total - 5);
    return WriteExe(rva, buf, total);
}
// a hand-written code patch: va, bytes overwritten, expected bytes, target
#define MS_CODEPATCH(va, len, expect, stub) CodePatch_(va - 0x400000, len, expect, (void *)(stub))
static bool CodePatch_(unsigned int rva, unsigned len, const char *expectHex, void *stub)
{
    unsigned char want[32]; unsigned n = 0;
    for (const char *p = expectHex; *p && n < sizeof want;)
    {
        while (*p == ' ') p++;
        if (!*p) break;
        unsigned v; sscanf(p, "%2x", &v); want[n++] = (unsigned char)v; p += 2;
    }
    if (n != len || memcmp(Exe(rva), want, len))
    {
        ModLog("[multistory] code patch at rva 0x%06x: bytes differ, not patched", rva); g_nFailed++; return false;
    }
    return WriteJmp(rva, stub, len);
}

// ---- story reachability (one bit per story above the ground in the two "2nd level" bytes) ----------------------
static bool Story_IsReachable(void *pState, int nStory)
{
    if (nStory < 1 || nStory > kMaxStory) return false;
    if (nStory == 1) return true;
    unsigned char nBit = (unsigned char)(1 << (nStory - 2));
    void *w = World();
    return (MS_CPState_GetMode(pState) == MSC_CPState_kBuild && (FLD(unsigned char, w, cFixedWorld_m_bIs2ndLevelFloorable) & nBit)) ||
           (FLD(unsigned char, w, cFixedWorld_m_bHasBeenTo2ndLevel) & nBit);
}
static bool Story_CanGoUp(void *pState) { return Story_IsReachable(pState, MS_CPState_GetLevel(pState) + 1); }
static bool Story_CanGoDown(void *pState) { return MS_CPState_GetLevel(pState) > 1; }
// the view control's down / up buttons enabled as the current state allows
static void RefreshStoryButtons()
{
    void *pApp = Glob<void *>(MSG_g_pSimsApp);
    void *pPanel = pApp ? FLD(void *, pApp, cSimsApp_m_pCPanel) : 0;
    void *pViewCtl = pPanel ? FLD(void *, pPanel, cWinCPanel_m_pViewControl) : 0;
    if (!pViewCtl) return;
    void *pState = FLD(void *, pViewCtl, cWinViewControl_m_pCPState);
    void *pDown = FLD(void *, pViewCtl, cWinViewControl_m_pFirstStoryBtn), *pUp = FLD(void *, pViewCtl, cWinViewControl_m_pSecondStoryBtn);
    if (!pState || !pDown || !pUp) return;
    MS_cTSWinBtn_SetEnabled(pDown, Story_CanGoDown(pState));
    MS_cTSWinBtn_SetEnabled(pUp, Story_CanGoUp(pState));
}

// =================================================================================================================
// world: allocation, release, save / load, rooms
// =================================================================================================================
typedef void (__fastcall *PFN_vI)(void *self, void *edx, int);
typedef void (__fastcall *PFN_v)(void *self, void *edx);
typedef int (__fastcall *PFN_iPL)(void *self, void *edx, void *pFile, long);
static PFN_vI s_origSetSize, s_origComputeRooms;
static PFN_v s_origDeleteArrays;
static PFN_iPL s_origLoad, s_origSave;
static void *s_lastFloors0;                     // world->m_pFloors[0] when the stories 3-4 arrays were made

static void FreeUpperWorld()
{
    void *w = g_blkWorld;                           // stories 3-4 live in the block whatever the instance
    for (int s = 3; s <= kMaxStory; s++)
    {
        DeleteArray(W(w, m_pFloors, s));            W(w, m_pFloors, s) = 0;
        DeleteArray(W(w, m_pRooms, s));             W(w, m_pRooms, s) = 0;
        DeleteArray(W(w, m_pFlags, s));             W(w, m_pFlags, s) = 0;
        DeleteArray(W(w, m_pObjects, s));           W(w, m_pObjects, s) = 0;
        DeleteArray(W(w, m_pWalls, s));             W(w, m_pWalls, s) = 0;
        DeleteArray(W(w, m_pWallVertexMask, s));    W(w, m_pWallVertexMask, s) = 0;
        DeleteLightGrid(W(w, m_pLightEntries, s));  W(w, m_pLightEntries, s) = 0;
        DeleteRoof(W(w, m_pRoof, s));               W(w, m_pRoof, s) = 0;
    }
    s_lastFloors0 = 0;
}

static void AllocUpperWorld(void *w)
{
    for (int s = 3; s <= kMaxStory; s++)
    {
        W(w, m_pFloors, s) = NewArray(MS_cArray1_ushort_ctor);
        W(w, m_pRooms, s) = NewArray(MS_cArray1_ushort_ctor);
        W(w, m_pFlags, s) = NewArray(MS_cArray1_uchar_ctor);
        W(w, m_pWalls, s) = NewArray(MS_cArray1_WallTile_ctor);
        W(w, m_pObjects, s) = NewArray(MS_cArray1_short_ctor);
        W(w, m_pWallVertexMask, s) = NewArray(MS_cArray1_VertexConfig_ctor);
        W(w, m_pLightEntries, s) = NewLightGrid();
        void *pRoof = MS_operator_new(MSS_cRoofLayer);
        MS_cRoofLayer_ctor(pRoof, 64);
        W(w, m_pRoof, s) = pRoof;
        for (int y = 0; y < 64; y++) At<unsigned char>(RoofArr(pRoof), 63, y * MSS_cRoofTile + MSO_cRoofTile_m_nFlags) = 0;
    }
    s_lastFloors0 = FLD(void *, w, cFixedWorld_m_pFloors);
}

static void __fastcall Hook_SetSize(void *self, void *, int nSize)
{
    int nOldSize = FLD(int, self, cFixedWorld_m_nSize);
    s_origSetSize(self, 0, nSize);
    if (ms_inst[kClsWorld] && ms_inst[kClsWorld] != (unsigned int)self)
        ModLog("[multistory] SetSize on a second world %p (registered %p)", self, (void *)ms_inst[kClsWorld]);
    if (FLD(void *, self, cFixedWorld_m_pFloors) != s_lastFloors0 || !W(self, m_pFloors, 3))
    {
        // the original reallocated stories 1-2 (or this is the first world): stories 3-4 follow
        if (W(self, m_pFloors, 3)) FreeUpperWorld();
        ms_inst[kClsWorld] = (unsigned int)self;
        AllocUpperWorld(self);
        ModLog("[multistory] world %p: stories 3-4 allocated (size %d)", self, nSize);
    }
    else if (nSize < nOldSize)
    {
        // shrink: the original clears the new last row / column of stories 1-2 only
        for (int s = 3; s <= kMaxStory; s++)
            for (int i = 0; i < nSize; i++)
                for (int k = 0; k < 2; k++)
                {
                    int x = k ? i : nSize - 1, y = k ? nSize - 1 : i;
                    At<unsigned short>(W(self, m_pFloors, s), x, y) = 0;
                    At<unsigned short>(W(self, m_pRooms, s), x, y) = 0;
                    memset(&At<unsigned char>(W(self, m_pWalls, s), x, y * MSS_WallTile), 0, MSS_WallTile);
                    At<short>(W(self, m_pObjects, s), x, y) = 0;
                    At<unsigned char>(W(self, m_pWallVertexMask, s), x, y) = 0;
                    ((MsLightGrid *)W(self, m_pLightEntries, s))->pFirst[((signed char)x << 6) + (signed char)y] = 0x7F7F7F7F;
                    At<unsigned char>(RoofArr(W(self, m_pRoof, s)), x, y * MSS_cRoofTile + MSO_cRoofTile_m_nFlags) = 0;
                }
    }
}

static void __fastcall Hook_DeleteArrays(void *self, void *)
{
    s_origDeleteArrays(self, 0);
    if (ms_inst[kClsWorld] == (unsigned int)self)
    {
        FreeUpperWorld();
        ms_inst[kClsWorld] = 0;
    }
}

static const int kArryTag = 0x41727279;   // 'Arry'

static int __fastcall Hook_Load(void *self, void *, void *pFile, long nVersion)
{
    int r = s_origLoad(self, 0, pFile, nVersion);
    if (r == 0 && W(self, m_pFloors, 3))
    {
        // stories 3-4: chunks base + 100 * (story - 1); a house saved without them gets them empty
        for (int s = 3; s <= kMaxStory; s++)
        {
            short nBase = (short)(100 * (s - 1));
            MS__c2DArray_ClearBytes(W(self, m_pFloors, s), 0);
            MS__c2DArray_ClearBytes(W(self, m_pWalls, s), 0);
            MS__c2DArray_ClearBytes(W(self, m_pObjects, s), 0);
            MS__c2DArray_ClearBytes(W(self, m_pFlags, s), 0);
            MS__c2DArray_ReadFromDisk(W(self, m_pFloors, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Floors), 0);
            MS__c2DArray_ReadFromDisk(W(self, m_pWalls, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Walls), 0);
            MS__c2DArray_ReadFromDisk(W(self, m_pObjects, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Objects), MS_EXE(MSA_SwizzleShort));
            MS__c2DArray_ReadFromDisk(W(self, m_pFlags, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Flags), 0);
            FillLightGrid(W(self, m_pLightEntries, s));
        }
    }
    return r;
}

static int __fastcall Hook_Save(void *self, void *, void *pFile, long nVersion)
{
    int r = s_origSave(self, 0, pFile, nVersion);
    if (r == 0 && nVersion >= 23 && W(self, m_pFloors, 3))
    {
        for (int s = 3; s <= kMaxStory; s++)
        {
            short nBase = (short)(100 * (s - 1));
            if ((r = MS__c2DArray_WriteToDisk(W(self, m_pFloors, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Floors), true)) != 0) break;
            if ((r = MS__c2DArray_WriteToDisk(W(self, m_pWalls, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Walls), true)) != 0) break;
            if ((r = MS__c2DArray_WriteToDisk(W(self, m_pObjects, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Objects), true)) != 0) break;
            if ((r = MS__c2DArray_WriteToDisk(W(self, m_pFlags, s), pFile, kArryTag, (short)(nBase + MSC_kArry_Flags), true)) != 0) break;
        }
    }
    return r;
}

// cFixedWorld::ComputeRooms: the byte patches make the original walk stories 1-4; its "2nd level" flag code only
// knows story 2 (and writes whole bytes), and its roof chain only stories 1-2: both are redone here.
static void __fastcall Hook_WorldComputeRooms(void *self, void *, int nLevel)
{
    unsigned char nBeenBefore = FLD(unsigned char, self, cFixedWorld_m_bHasBeenTo2ndLevel);
    unsigned char nFloorableBefore = FLD(unsigned char, self, cFixedWorld_m_bIs2ndLevelFloorable);
    s_origComputeRooms(self, 0, nLevel);
    if (!W(self, m_pFloors, 3)) return;
    unsigned char nBeen = FLD(unsigned char, self, cFixedWorld_m_bHasBeenTo2ndLevel) & 1;
    unsigned char nFloorable = FLD(unsigned char, self, cFixedWorld_m_bIs2ndLevelFloorable) & 1;
    int n = FLD(int, self, cFixedWorld_m_nSize);
    for (int s = 3; s <= kMaxStory; s++)
    {
        unsigned char nBit = (unsigned char)(1 << (s - 2));
        for (int x = 0; x < n; x++)
            for (int y = 0; y < n; y++)
            {
                if (At<short>(W(self, m_pObjects, s), x, y) || At<unsigned short>(W(self, m_pFloors, s), x, y) ||
                    At<unsigned char>(W(self, m_pWalls, s), x, y * MSS_WallTile + MSO_WallTile_m_nWallMask))
                    nBeen |= nBit;
                if (At<unsigned char>(W(self, m_pFlags, s), x, y) & 1) nFloorable |= nBit;
            }
    }
    FLD(unsigned char, self, cFixedWorld_m_bHasBeenTo2ndLevel) = nBeen;
    FLD(unsigned char, self, cFixedWorld_m_bIs2ndLevelFloorable) = nFloorable;
    for (int s = kMaxStory; s >= 1; s--)        // top story first, each sees the one above
    {
        void *pRoof = W(self, m_pRoof, s);
        if (pRoof) MS_cRoofLayer_GenerateRoof(pRoof, s, s < kMaxStory ? W(self, m_pRoof, s + 1) : 0);
    }
    // a story became reachable (or stopped being): enable the story buttons now, not only on the next panel change
    // (only the buttons: the level dirty flag would also reset the tool's grab point in the middle of a wall drag)
    if (nBeen != nBeenBefore || nFloorable != nFloorableBefore) RefreshStoryButtons();
}

// is the tile one story above (x, y, nLevel) inside an enclosed room (any room that is not the outside; a diagonal
// tile counts when either of its two rooms is enclosed)?
static bool InsideRoomAbove(void *w, void *pRoomMgr, int x, int y, int nLevel)
{
    void *pRoomsAbove = W(w, m_pRooms, nLevel + 1);
    if (!pRoomsAbove || !pRoomMgr) return false;
    unsigned short nIds[2] = { At<unsigned short>(pRoomsAbove, x, y), 0xFFFF };
    if (nIds[0] == 0xFFFB)
    {
        MsTile above = { (signed char)x, (signed char)y, (signed char)(nLevel + 1) };
        int eSideA, eSideB;                         // Room::Sides
        if (!MS_RoomManager_ResolveDiagonal(pRoomMgr, &above, &nIds[0], &nIds[1], &eSideA, &eSideB)) return false;
    }
    for (int i = 0; i < 2; i++)
        if (nIds[i] < 0xFFFB)
        {
            void *pRoom = MS_RoomManager_GetRoom(pRoomMgr, nIds[i]);
            if (pRoom && !MS_Room_IsOutside(pRoom)) return true;
        }
    return false;
}

// cRoofLayer::DiscoverTilesToRoof: the original keeps the roof off tiles under a story-2 floor / stairs only for
// the ground's roof; the same rule for the roofs of stories 2-3 against the story above. On every story below the top
// a tile inside an enclosed room of the story above is covered too, floor or not (walls built straight on the story
// below leave no roof under them; the original needs a floor tile).
static PFN_vI s_origDiscoverTiles;
static void __fastcall Hook_DiscoverTilesToRoof(void *self, void *, int nLevel)
{
    s_origDiscoverTiles(self, 0, nLevel);
    if (nLevel < 1 || nLevel >= kMaxStory || !ms_inst[kClsWorld]) return;
    void *w = (void *)ms_inst[kClsWorld];
    void *pFloorsAbove = W(w, m_pFloors, nLevel + 1), *pObjectsAbove = W(w, m_pObjects, nLevel + 1);
    if (!pFloorsAbove || !pObjectsAbove) return;
    void *pRoomMgr = Glob<void *>(MSG_g_pRoomManager);
    void *arr = RoofArr(self);
    int nHeight = ArrHeight(arr), nSize = FLD(int, w, cFixedWorld_m_nSize);
    void *pGame = FLD(void *, AppSettings(), cSimsSettings_m_pGame);
    for (int nRow = 0; nRow < nHeight; nRow++)
        for (int nCol = 0; nCol < nHeight; nCol++)
        {
            MsTile tile = { (signed char)((unsigned)nCol >> 1), (signed char)((unsigned)nRow >> 1), (signed char)nLevel };
            bool bValid = tile.nX >= 0 && tile.nX < nSize && tile.nY >= 0 && tile.nY < nSize;
            bool bBlocked = bValid && InsideRoomAbove(w, pRoomMgr, tile.nX, tile.nY, nLevel);
            if (nLevel == 1 && !bBlocked) continue;  // the ground: the original already did the floor / stairs test
            if (!bBlocked) bBlocked = At<unsigned short>(pFloorsAbove, tile.nX, tile.nY) != 0;
            if (!bBlocked && bValid && At<short>(pObjectsAbove, tile.nX, tile.nY))
            {
                alignas(4) unsigned char it[MSS_ObjectIterator];
                MS_ObjectIterator_ctor(it, pGame, &tile, 0);
                for (void *pObj; (pObj = FLD(void *, it, ObjectIterator_m_pCur)) != 0; MS_ObjectIterator_inc(it))
                {
                    void *pMaster = MS_ObjSelector_GetMasterSelector(FLD(void *, pObj, cXObject_m_pSelector));
                    if (FLD(short, FLD(void *, pMaster, ObjSelector_m_pObjd), OBJD_build_mode_type) == 3) { bBlocked = true; break; }
                }
            }
            if (bBlocked)
            {
                unsigned char &f = At<unsigned char>(arr, nRow, nCol * MSS_cRoofTile + MSO_cRoofTile_m_nFlags);
                f = (unsigned char)((f & 0xFE) | 8);
            }
        }
}

// =================================================================================================================
// WallManager: the stories 3-4 lists, and the loops over all stories (replacements)
// =================================================================================================================
typedef void *(__fastcall *PFN_ctor)(void *self, void *edx);
static PFN_ctor s_origWMCtor;
static PFN_v s_origWMDtor;

static void *__fastcall Hook_WMCtor(void *self, void *)
{
    void *r = s_origWMCtor(self, 0);
    if (ms_inst[kClsWallMgr] && ms_inst[kClsWallMgr] != (unsigned int)self)
        ModLog("[multistory] a second WallManager %p (registered %p)", self, (void *)ms_inst[kClsWallMgr]);
    for (int s = 3; s <= kMaxStory; s++)
    {
        WM_VL(self, s) = new VertexVec;
        WM_WL(self, s) = new WallVec;
        WM_DIRTY(self, m_bWallsDirty, s) = 1;
        WM_DIRTY(self, m_bWallpaperDirty, s) = 1;
    }
    ms_inst[kClsWallMgr] = (unsigned int)self;
    return r;
}

static void __fastcall Hook_WMDtor(void *self, void *)
{
    if (ms_inst[kClsWallMgr] == (unsigned int)self)
    {
        for (int s = 3; s <= kMaxStory; s++)
        {
            delete WM_VL(self, s); WM_VL(self, s) = 0;
            delete WM_WL(self, s); WM_WL(self, s) = 0;
        }
        ms_inst[kClsWallMgr] = 0;
    }
    s_origWMDtor(self, 0);
}

static PFN_vI s_origAsyncRebuild, s_origAsyncRefresh;
static void __fastcall Hook_AsyncRebuild(void *self, void *, int nLevel)
{
    s_origAsyncRebuild(self, 0, nLevel);     // nLevel 3-4 reach the side flags through a site stub
    if (!nLevel)
        for (int s = 3; s <= kMaxStory; s++) WM_DIRTY(self, m_bWallsDirty, s) = 1;
}
static void __fastcall Hook_AsyncRefresh(void *self, void *, int nLevel)
{
    s_origAsyncRefresh(self, 0, nLevel);
    if (!nLevel)
        for (int s = 3; s <= kMaxStory; s++) WM_DIRTY(self, m_bWallpaperDirty, s) = 1;
}

static void __fastcall Rep_SortLevelListsByDrawOrder(void *self, void *, int nLevel)
{
    int nFirst = nLevel ? nLevel : 1, nLast = nLevel ? nLevel : kMaxStory;
    for (int s = nFirst; s <= nLast; s++)
    {
        VertexVec &verts = *WM_VL(self, s);
        if (verts.size() > 1) std::sort(verts.begin(), verts.end(), VertexIso);
        WallVec &walls = *WM_WL(self, s);
        if (walls.size() > 1) std::sort(walls.begin(), walls.end(), WallIso);
    }
}

static void __fastcall Rep_SetRotation(void *self, void *, int nRotation)
{
    FLD(int, self, WallManager_m_nRotation) = nRotation;
    for (int s = 1; s <= kMaxStory; s++)
    {
        for (WallVec::iterator w = WM_WL(self, s)->begin(); w != WM_WL(self, s)->end(); ++w) MS_Wall_SetRotation(w->m_p, nRotation);
        for (VertexVec::iterator v = WM_VL(self, s)->begin(); v != WM_VL(self, s)->end(); ++v) MS_Vertex_SetRotation(v->m_p, nRotation);
    }
    Rep_SortLevelListsByDrawOrder(self, 0, 0);
}

static void __fastcall Rep_RefreshFromMap(void *self, void *, int nLevel)
{
    int nFirst = nLevel ? nLevel : 1, nLast = nLevel ? nLevel : kMaxStory;
    for (int s = nFirst; s <= nLast; s++)
        for (WallVec::iterator it = WM_WL(self, s)->begin(); it != WM_WL(self, s)->end(); ++it) MS_Wall_RefreshFromMap(it->m_p);
    MS_GViewer_DirtyCutaway();
}

static void __fastcall Rep_SetCutaway(void *self, void *, bool bCutaway, int nLevel)
{
    int nFirst = nLevel ? nLevel : 1, nLast = nLevel ? nLevel : kMaxStory;
    for (int s = nFirst; s <= nLast; s++)
    {
        for (WallVec::iterator w = WM_WL(self, s)->begin(); w != WM_WL(self, s)->end(); ++w) MS_Wall_SetCutaway(w->m_p, (char)bCutaway);
        for (VertexVec::iterator v = WM_VL(self, s)->begin(); v != WM_VL(self, s)->end(); ++v) MS_Vertex_ComputeCutaway(v->m_p);
        for (VertexVec::iterator v = WM_VL(self, s)->begin(); v != WM_VL(self, s)->end(); ++v) MS_Vertex_ComputeCutawaySecondPass(v->m_p);
    }
}

static bool __fastcall Rep_AreAllWallsConsistent(void *self, void *)
{
    for (int s = 1; s <= kMaxStory; s++)
        for (WallVec::iterator it = WM_WL(self, s)->begin(); it != WM_WL(self, s)->end(); ++it)
            if (!MS_Wall_IsSegmentCountConsistent(it->m_p)) return false;
    return true;
}

static void __fastcall Rep_CleanupConstruction(void *self, void *)
{
    for (int s = 1; s <= kMaxStory; s++)
        for (VertexVec::iterator it = WM_VL(self, s)->begin(); it != WM_VL(self, s)->end(); ++it)
            for (int j = 2; j < 4; j++)
            {
                void *pWall = ((void **)((char *)it->m_p + MSO_Vertex_m_pWalls))[j];
                if (pWall)
                {
                    MS_Wall_ChangeWallPattern(pWall, 255, 0);
                    MS_Wall_ChangeWallStyle(pWall, 255, 1);
                    MS_Wall_FixFences(pWall);
                }
            }
}

static void __fastcall Rep_BuildGraphFromMap(void *self, void *, int nLevel)
{
    int nFirst = nLevel ? nLevel : 1, nLast = nLevel ? nLevel : kMaxStory;
    void *w = World();
    const unsigned char *pJunction = (const unsigned char *)MS_EXE(MSG_g_aWallMaskIsJunction);
    const unsigned char *pEndpoint = (const unsigned char *)MS_EXE(MSG_g_aWallMaskIsEndpoint);
    for (int s = nFirst; s <= nLast; s++)
    {
        WM_VL(self, s)->clear();
        WM_WL(self, s)->clear();
        int nSize = FLD(int, w, cFixedWorld_m_nSize);
        void *pMask = W(w, m_pWallVertexMask, s);
        if (!pMask) continue;
        for (int y = 0; y < nSize; y++)
            for (int x = 0; x < nSize; x++)
            {
                unsigned char nMask = At<unsigned char>(pMask, x, y);
                if (nMask && (pJunction[nMask] || pEndpoint[nMask]))
                {
                    MsTile tile = { (signed char)x, (signed char)y, (signed char)s };
                    void *pVertex = MS_operator_new(MSS_Vertex);
                    MS_Vertex_ctor(pVertex, &tile, 0);
                    FLD(unsigned char, pVertex, Vertex_m_nConfig) = nMask;
                    MS_WallManager_AddVertex(self, pVertex, s);
                }
            }
    }
    Rep_SortLevelListsByDrawOrder(self, 0, nLevel);
    for (int s = nFirst; s <= nLast; s++)
        for (VertexVec::iterator it = WM_VL(self, s)->begin(); it != WM_VL(self, s)->end(); ++it) MS_Vertex_BuildWalls(it->m_p);
    Rep_SetRotation(self, 0, FLD(int, self, WallManager_m_nRotation));
    for (int s = nFirst; s <= nLast; s++)
    {
        WM_DIRTY(self, m_bWallsDirty, s) = 0;
        WM_DIRTY(self, m_bWallpaperDirty, s) = 0;
    }
    MS_GViewer_DirtyCutaway();
}

static void __fastcall Rep_HandleDirty(void *self, void *)
{
    for (int s = 1; s <= kMaxStory; s++)
        for (WallVec::iterator it = WM_WL(self, s)->begin(); it != WM_WL(self, s)->end(); ++it) MS_Wall_BeginComputeCutaway(it->m_p);
    int nWallsDirty = 0, nWallpaperDirty = 0;
    for (int s = 1; s <= kMaxStory; s++)
    {
        nWallsDirty += WM_DIRTY(self, m_bWallsDirty, s);
        nWallpaperDirty += WM_DIRTY(self, m_bWallpaperDirty, s);
    }
    if (nWallsDirty)
    {
        if (nWallsDirty == kMaxStory)
            Rep_BuildGraphFromMap(self, 0, 0);
        else
            for (int s = 1; s <= kMaxStory; s++)
                if (WM_DIRTY(self, m_bWallsDirty, s)) Rep_BuildGraphFromMap(self, 0, s);
        for (int s = 1; s <= kMaxStory; s++)
        {
            WM_DIRTY(self, m_bWallsDirty, s) = 0;
            WM_DIRTY(self, m_bWallpaperDirty, s) = 0;
        }
        return;
    }
    if (!nWallpaperDirty) return;
    if (nWallpaperDirty == kMaxStory)
    {
        for (int s = 1; s <= kMaxStory; s++)
            for (WallVec::iterator it = WM_WL(self, s)->begin(); it != WM_WL(self, s)->end(); ++it) MS_Wall_RefreshFromMap(it->m_p);
        MS_GViewer_DirtyCutaway();
    }
    else
    {
        for (int s = 1; s <= kMaxStory; s++)
            if (WM_DIRTY(self, m_bWallpaperDirty, s))
            {
                for (WallVec::iterator it = WM_WL(self, s)->begin(); it != WM_WL(self, s)->end(); ++it) MS_Wall_RefreshFromMap(it->m_p);
                MS_GViewer_DirtyCutaway();
            }
    }
    for (int s = 1; s <= kMaxStory; s++) WM_DIRTY(self, m_bWallpaperDirty, s) = 0;
}

// =================================================================================================================
// RoomManager: the stories 3-4 shade grids, the per-story dirty bits
// =================================================================================================================
typedef void *(__fastcall *PFN_ctorRM)(void *self, void *edx, void *pHouse);
static PFN_ctorRM s_origRMCtor;
static PFN_v s_origRMDtor;
static PFN_vI s_origRMComputeRooms, s_origRoomLighting, s_origRoomScore;

static void *__fastcall Hook_RMCtor(void *self, void *, void *pHouse)
{
    void *r = s_origRMCtor(self, 0, pHouse);
    for (int s = 3; s <= kMaxStory; s++) RM_SHADE(self, s) = NewArray(MS_cArray1_uchar_ctor);
    ms_inst[kClsRoomMgr] = (unsigned int)self;
    return r;
}

static void __fastcall Hook_RMDtor(void *self, void *)
{
    if (ms_inst[kClsRoomMgr] == (unsigned int)self)
    {
        for (int s = 3; s <= kMaxStory; s++) { DeleteArray(RM_SHADE(self, s)); RM_SHADE(self, s) = 0; }
        ms_inst[kClsRoomMgr] = 0;
    }
    s_origRMDtor(self, 0);
}

static void __fastcall Hook_RMComputeRooms(void *self, void *, int nLevel)
{
    s_origRMComputeRooms(self, 0, nLevel);
    if (nLevel >= 3 && nLevel <= kMaxStory) FLD(int, self, RoomManager_m_nDirtyLevelMask) &= ~(1 << (nLevel - 1));
}

static void MarkRoomStory(void *self, int nRoomId)
{
    RoomMap &rooms = FLD(RoomMap, self, RoomManager_m_Rooms);
    RoomMap::iterator it = rooms.find((unsigned short)nRoomId);
    if (it == rooms.end() || !it->second) return;
    int nLevel = MS_Room_GetLevel(it->second);
    if (nLevel >= 3 && nLevel <= kMaxStory) FLD(int, self, RoomManager_m_nDirtyLevelMask) |= 1 << (nLevel - 1);
}
static void __fastcall Hook_RoomLightingChanged(void *self, void *, int nRoomId)
{
    s_origRoomLighting(self, 0, nRoomId);
    MarkRoomStory(self, nRoomId);
}
static void __fastcall Hook_RoomScoreChanged(void *self, void *, int nRoomId)
{
    s_origRoomScore(self, 0, nRoomId);
    MarkRoomStory(self, nRoomId);
}

static void __fastcall Rep_UpdateRooms(void *self, void *)
{
    int nMask = FLD(int, self, RoomManager_m_nDirtyLevelMask);
    if (!nMask) return;
    if (nMask & (nMask - 1))
        MS_RoomManager_ComputeRooms(self, 0);
    else
        for (int s = 1; s <= kMaxStory; s++)
            if (nMask == 1 << (s - 1)) MS_RoomManager_ComputeRooms(self, s);
}

// =================================================================================================================
// house stats, stairs, story buttons and keys
// =================================================================================================================
typedef void (__fastcall *PFN_vP)(void *self, void *edx, void *);
static PFN_vP s_origGetHouseStats;
static void __fastcall Hook_GetHouseStats(void *self, void *, void *pStats)
{
    s_origGetHouseStats(self, 0, pStats);
    unsigned char nBeen = FLD(unsigned char, FLD(void *, self, House_m_pFixedWorld), cFixedWorld_m_bHasBeenTo2ndLevel);
    int nFloors = 1;
    for (int nBit = 0; nBit < kMaxStory - 1; nBit++)
        if (nBeen & (1 << nBit)) nFloors = nBit + 2;
    FLD(int, pStats, HouseStats_m_nFloors) = nFloors;
}

static inline void *MasterPart(void *p) { void *m = FLD(void *, p, cXMTObject_m_pMaster); return m ? m : p; }   // cXMTObject::GetMaster

// terrain tools: cDirtTool::MayModifyTerrain keeps the ground as it is under the four tiles around a terrain corner
// when stories 1-2 have a floor / walls / flat objects there; the same for stories 3-4 (a story-3 floor can overhang
// where stories 1-2 are empty, and raising or lowering the ground there would move that floor). The four tiles are
// the original's walk: the corner tile, then its west, north-west and north neighbours (static steps W, N, E).
typedef bool (__fastcall *PFN_MayModifyTerrain)(void *self, void *edx, const void *pt, bool bCheckFlat);
static PFN_MayModifyTerrain s_origMayModifyTerrain;
static bool UpperStoryBuilt(void *pTool, int x, int y, bool bCheckFlat)
{
    if (x < 0 || x > 63 || y < 0 || y > 63) return false;
    void *w = World();
    int n = FLD(int, w, cFixedWorld_m_nSize);
    int nRot = -FLD(int, Glob<void *>(MSG_g_pLot), cRotatableWorld_m_nRotation) & 3;
    int rx = x, ry = y;
    if (nRot)                                       // cRotatableWorld::Rotate: g_TileRotateLUT[nRot - 1][x][y]
    {
        const signed char *e = &Glob<signed char>(MSG_g_TileRotateLUT) + (((nRot - 1) * 64 + x) * 64 + y) * 2;
        rx = e[0];
        ry = e[1];
    }
    if (rx < 1 || rx >= n - 1 || ry < 1 || ry >= n - 1) return false;   // the original tests only inner tiles
    for (int s = 3; s <= kMaxStory; s++)
    {
        MsTile tile = { (signed char)x, (signed char)y, (signed char)s };
        if (At<unsigned short>(W(w, m_pFloors, s), rx, ry) ||
            At<unsigned char>(W(w, m_pWalls, s), rx, ry * MSS_WallTile + MSO_WallTile_m_nWallMask) ||
            (bCheckFlat && MS_cDirtTool_HasFlatObjects(pTool, &tile)))
            return true;
    }
    return false;
}
static bool __fastcall Hook_MayModifyTerrain(void *self, void *, const void *pt, bool bCheckFlat)
{
    if (!s_origMayModifyTerrain(self, 0, pt, bCheckFlat)) return false;
    const MsTile &corner = *(const MsTile *)pt;
    static const int s_steps[4][2] = { { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, -1 } };
    for (auto &d : s_steps)
    {
        int x = corner.nX + d[0], y = corner.nY + d[1];
        if (UpperStoryBuilt(self, x, y, bCheckFlat))
        {
            FLD(int, self, cDirtTool_m_nMode) = 1;   // as the original's refusal: mode 1, anchored at that tile
            FLD(MsTile, self, cDirtTool_m_AnchorTile) = MsTile{ (signed char)x, (signed char)y, 1 };
            return false;
        }
    }
    return true;
}

// wall tool: cNewWallTool::Click marks the mouse down before it checks the tile; a refused click (no wall can start
// there, common on stories 3-4) left it down, and the next Drag ran without an origin from the unset cursor vertex
// (0,0,0) -> GetWall on story 0. A refused click never sets the drag anchor: mark the mouse up again then.
typedef void (__fastcall *PFN_WallClick)(void *self, void *edx, unsigned ptA, unsigned ptB, int nMods, void *pEvent);
static PFN_WallClick s_origWallClick;
static void __fastcall Hook_WallClick(void *self, void *, unsigned ptA, unsigned ptB, int nMods, void *pEvent)
{
    MsTile &anchor = FLD(MsTile, self, cNewWallTool_m_ptDragAnchor);
    MsTile saved = anchor;
    anchor.nLevel = -128;                           // no click sets this
    s_origWallClick(self, 0, ptA, ptB, nMods, pEvent);
    if (anchor.nLevel == -128)
    {
        anchor = saved;
        FLD(unsigned char, self, cNewWallTool_m_bMouseUp) = 1;
    }
}

// stairs: a part above the top story would lead nowhere
typedef bool (__fastcall *PFN_CanPlace)(void *self, void *edx, const void *pos, int nLevel, void *pParent, int nSlot);
static PFN_CanPlace s_origCanPlace;
static bool __fastcall Hook_CanPlace(void *self, void *, const void *pos, int nLevel, void *pParent, int nSlot)
{
    if (!pParent)
    {
        int nOriginLevel = nLevel - FLD(int, self, cXMTObject_m_nOffsetLevel);
        for (void *p = MasterPart(self); p; p = FLD(void *, p, cXMTObject_m_pNextPart))
            if (nOriginLevel + FLD(int, p, cXMTObject_m_nOffsetLevel) > kMaxStory)
            {
                Glob<int>(MSG_g_nUserActionError) = 2;   // kPlaceErr_LevelOutOfBounds
                return false;
            }
    }
    return s_origCanPlace(self, 0, pos, nLevel, pParent, nSlot);
}

// stairs' SimAntics test "my room >= 1024" (the first room id of story 2) means "upstairs" only for ground stairs:
// for a staircase whose bottom is on story L the literal becomes L << 10
// __RTDynamicCast (the C runtime behind dynamic_cast; declared by the compiler) with the exe's type descriptors
static short StairRelativeRoomBase(void *pSelf, short nLiteral)
{
    void *pElem = MS_TreeSim_GetCurElem((char *)pSelf + MSB_cXObject_TreeSim);
    void *pObj = MS_ObjectModule_GetObject(FLD(void *, pSelf, cXObject_m_pModule), FLD(short, pElem, StackElem_m_nStackObjectId));
    void *pPart = pObj ? __RTDynamicCast(pObj, 0, MS_EXE(MSR_cXObject), MS_EXE(MSR_cXMTObject), 0) : 0;
    if (!pPart) return nLiteral;
    void *pMaster = MasterPart(pPart);
    bool bSpansStories = false;
    for (void *p = pMaster; p; p = FLD(void *, p, cXMTObject_m_pNextPart))
        if (FLD(int, p, cXMTObject_m_nOffsetLevel) != FLD(int, pMaster, cXMTObject_m_nOffsetLevel)) bSpansStories = true;
    if (!bSpansStories) return nLiteral;
    int nBaseLevel = FLD(int, pMaster, cXMTObject_m_nLevel) - FLD(int, pMaster, cXMTObject_m_nOffsetLevel);
    for (void *p = pMaster; p; p = FLD(void *, p, cXMTObject_m_pNextPart))
        if (FLD(int, p, cXMTObject_m_nLevel) - FLD(int, p, cXMTObject_m_nOffsetLevel) < nBaseLevel)
            nBaseLevel = FLD(int, p, cXMTObject_m_nLevel) - FLD(int, p, cXMTObject_m_nOffsetLevel);
    if (nBaseLevel < 1 || nBaseLevel >= kMaxStory) return nLiteral;
    return (short)(nBaseLevel << 10);
}
typedef int (__fastcall *PFN_TryExpr)(void *self, void *edx, void *pParams);
static PFN_TryExpr s_origTryExpression;
static int __fastcall Hook_TryExpression(void *self, void *, void *pParams)
{
    unsigned char *pOp = &FLD(unsigned char, pParams, BehaviorNodeParam_m_Op);
    short *pParam = &FLD(short, pParams, BehaviorNodeParam_m_Param);
    if ((char)pOp[6] == MSC_SCOPE_my && pParam[0] == MSC_kObjData_Room && (char)pOp[7] == MSC_SCOPE_literal && pParam[1] == 1024)
    {
        short nBase = StairRelativeRoomBase(self, 1024);
        if (nBase != 1024)
        {
            alignas(4) unsigned char copy[MSS_BehaviorNodeParam];   // the original only reads its parameters
            memcpy(copy, pParams, sizeof copy);
            (&FLD(short, copy, BehaviorNodeParam_m_Param))[1] = nBase;
            return s_origTryExpression(self, 0, copy);
        }
    }
    return s_origTryExpression(self, 0, pParams);
}

// the view control's first / second story buttons become "go down" / "go up"
typedef bool (__fastcall *PFN_OnCmd)(void *self, void *edx, unsigned long nCommand, unsigned long nParam);
static PFN_OnCmd s_origViewCtlCommand;
static bool __fastcall Hook_ViewCtlCommand(void *self, void *, unsigned long nCommand, unsigned long nParam)
{
    if (nCommand == MSC_kWinMsg_ButtonClick)
    {
        void *pBtn = (void *)nParam;
        void *pState = FLD(void *, self, cWinViewControl_m_pCPState);
        bool bDown = pBtn == FLD(void *, self, cWinViewControl_m_pFirstStoryBtn);
        if (bDown || pBtn == FLD(void *, self, cWinViewControl_m_pSecondStoryBtn))
        {
            if (bDown ? Story_CanGoDown(pState) : Story_CanGoUp(pState))
            {
                MS_cWinPeople_StopTracking(Glob<void *>(MSG_g_pWinPeople));
                MS_CPState_SetLevel(pState, MS_CPState_GetLevel(pState) + (bDown ? -1 : 1), MSC_CPState_kImmediate);
            }
            return true;
        }
    }
    return s_origViewCtlCommand(self, 0, nCommand, nParam);
}

typedef void (__fastcall *PFN_UpdView)(void *self, void *edx, void *pState);
static PFN_UpdView s_origUpdateView;
static void __fastcall Hook_UpdateView(void *self, void *, void *pState)
{
    unsigned int nDirty = FLD(unsigned int, pState, CPState_m_dwFlags);   // the original reads them once at its start
    s_origUpdateView(self, 0, pState);
    if (nDirty & 0x100)
    {
        if (MS_CPState_GetLevel(pState) > 1 && !Story_IsReachable(pState, MS_CPState_GetLevel(pState)))
            MS_CPState_SetLevel(pState, 1, MSC_CPState_kImmediate);
        void *pFirst = FLD(void *, self, cWinViewControl_m_pFirstStoryBtn), *pSecond = FLD(void *, self, cWinViewControl_m_pSecondStoryBtn);
        MS_cTSWinBtn_SetState(pFirst, 0);
        MS_cTSWinBtn_SetState(pSecond, 0);
        MS_cTSWinBtn_SetEnabled(pFirst, Story_CanGoDown(pState));
        MS_cTSWinBtn_SetEnabled(pSecond, Story_CanGoUp(pState));
    }
}

// PgUp / PgDn move one story; Tab cycles up through the reachable stories and wraps to the ground
typedef bool (__fastcall *PFN_KeyDown)(void *self, void *edx, unsigned long nKey, unsigned long nFlags);
static PFN_KeyDown s_origKeyDown;
static bool __fastcall Hook_KeyDown(void *self, void *, unsigned long nKey, unsigned long nFlags)
{
    if (nKey != VK_PRIOR && nKey != VK_NEXT && nKey != VK_TAB) return s_origKeyDown(self, 0, nKey, nFlags);
    void *pState = FLD(void *, self, cDDDSimsView_m_pCPState);
    int nLevel = MS_CPState_GetLevel(pState);
    if (nKey == VK_TAB && !(nLevel >= 2 && Story_CanGoUp(pState)))
        return s_origKeyDown(self, 0, nKey, nFlags);          // ground -> 2, or top -> ground: as the original
    ++FLD(int, self, cDDDSimsView_m_nKeyDepth);
    void *pScroller = FLD(void *, self, cDDDSimsView_m_pEdgeScroller);
    if (pScroller && VCall<bool>(pScroller, MSV_ScrollingStrategy_OnKeyDown, nKey, nFlags)) return true;
    if (nKey == VK_TAB)
    {
        MS_CPState_SetLevel(pState, nLevel + 1, MSC_CPState_kDeferred);
        // the original's Tab case does not consume the key: the tool, then the base window see it
        void *pTool = Glob<void *>(MSG_XViewer__sTool);
        if (pTool && VCall<bool>(pTool, MSV_cTool_OnKeyDown, nKey, nFlags)) return true;
        return MS_cTSWin_TSOnKeyDown((char *)self + MSB_cDDDSimsView_cTSWin, nKey, nFlags);
    }
    if (nKey == VK_PRIOR ? Story_CanGoUp(pState) : Story_CanGoDown(pState))
        MS_CPState_SetLevel(pState, nLevel + (nKey == VK_PRIOR ? 1 : -1), MSC_CPState_kDeferred);
    else
        MS_cSoundPlayer_PlayBySource(FLD(void *, AppSettings(), cSimsSettings_m_pSoundSys), "UI_error", -4);
    return true;
}

// =================================================================================================================
// install
// =================================================================================================================
// the view control's story buttons get "down" / "up" arrows: cWinViewControl::Init loads their art by resource id
// (kLevel1 2023, kLevel2 2024 -> CPanel/Buttons/Lev1.bmp, Lev2.bmp in UIGraphics.far); the mod ships
// UIGraphics/Res_Multistory.RT (+ .h) and UIGraphics/Multistory/StoryDown.bmp, StoryUp.bmp, which the game registers
// at startup like its own RT files (every UIGraphics/*.RT), and the two ids pushed by Init are patched to the new
// ones. Only when every file is there: an unregistered id would leave the buttons without art.
static const int kMsIconStoryDown = 9500, kMsIconStoryUp = 9501;   // UIGraphics/Res_Multistory.h
static void InstallStoryIcons()
{
    static const char *const s_files[] = { "UIGraphics\\Res_Multistory.RT", "UIGraphics\\Res_Multistory.h",
                                           "UIGraphics\\Multistory\\StoryDown.bmp", "UIGraphics\\Multistory\\StoryUp.bmp" };
    char szDir[MAX_PATH];
    DWORD n = GetModuleFileNameA(0, szDir, MAX_PATH);
    while (n && szDir[n - 1] != '\\') n--;
    for (unsigned i = 0; i < sizeof s_files / sizeof s_files[0]; i++)
    {
        char szPath[MAX_PATH];
        _snprintf(szPath, MAX_PATH, "%.*s%s", (int)n, szDir, s_files[i]);
        szPath[MAX_PATH - 1] = 0;
        if (GetFileAttributesA(szPath) == INVALID_FILE_ATTRIBUTES)
        {
            ModLog("[multistory] story icons: %s missing - the buttons keep the original art", s_files[i]);
            return;
        }
    }
    struct { unsigned rva; int nFrom, nTo; } patches[] = {
        { MSVA_IconDownPush - 0x400000, 2023, kMsIconStoryDown },   // push kLevel1 -> m_hFirstStoryImage ("one story down")
        { MSVA_IconUpPush - 0x400000, 2024, kMsIconStoryUp },     // push kLevel2 -> m_hSecondStoryImage ("one story up")
    };
    for (auto &p : patches)
        if (memcmp(Exe(p.rva), &p.nFrom, 4))
        {
            ModLog("[multistory] story icons: bytes at %06x differ - the buttons keep the original art", p.rva);
            return;
        }
    for (auto &p : patches)
        if (!WriteExe(p.rva, &p.nTo, 4)) { ModLog("[multistory] story icons: write failed"); g_nFailed++; return; }
    ModLog("[multistory] story icons: ids %d / %d", kMsIconStoryDown, kMsIconStoryUp);
}

static int InstallSitesAndPatches()
{
    // verify everything first: a single mismatch means a different exe build - install nothing
    for (unsigned i = 0; i < ms_site_count; i++)
        if (memcmp(Exe(ms_sites[i].rva), ms_sites[i].pSig, ms_sites[i].winLen))
        {
            ModLog("[multistory] site %u at rva 0x%06x: bytes differ - nothing installed", i, ms_sites[i].rva);
            return 1;
        }
    for (unsigned i = 0; i < sizeof g_msBytePatches / sizeof g_msBytePatches[0]; i++)
        if (memcmp(Exe(g_msBytePatches[i].rva), g_msBytePatches[i].from, g_msBytePatches[i].len))
        {
            ModLog("[multistory] byte patch %s: bytes differ - nothing installed", g_msBytePatches[i].what);
            return 1;
        }
    for (int c = 0; c < 3; c++) ms_shadow[c] = 0;
    ms_shadow[kClsWorld] = (unsigned int)g_blkWorld;
    ms_shadow[kClsWallMgr] = (unsigned int)g_blkWallMgr;
    ms_shadow[kClsRoomMgr] = (unsigned int)g_blkRoomMgr;
    int nSites = 0, nPatches = 0;
    for (unsigned i = 0; i < ms_site_count; i++)
    {
        if (Skip("stubs,") || !strcmp(g_szSkip, "stubs") || SkipRange("stubs", i)) continue;
        ms_ret[i] = g_api->exeBase + ms_sites[i].rva + ms_sites[i].winLen;
        if (WriteJmp(ms_sites[i].rva, ms_sites[i].pStub, ms_sites[i].winLen + ms_sites[i].padLen)) nSites++;
        else { ModLog("[multistory] site %u: write failed", i); g_nFailed++; }
    }
    for (unsigned i = 0; i < sizeof g_msBytePatches / sizeof g_msBytePatches[0]; i++)
    {
        if (Skip("patches,") || !strcmp(g_szSkip, "patches") || SkipRange("patches", i)) continue;
        if (WriteExe(g_msBytePatches[i].rva, g_msBytePatches[i].to, g_msBytePatches[i].len)) nPatches++;
        else { ModLog("[multistory] byte patch %s: write failed", g_msBytePatches[i].what); g_nFailed++; }
    }
    ms_pick_ret_a = g_api->exeBase + (MSVA_PickRetA - 0x400000);
    ms_pick_ret_b = g_api->exeBase + (MSVA_PickRetB - 0x400000);
    if (!Skip("pick"))
    {
        MS_CODEPATCH(MSVA_PickA, 10, "bb 03 00 00 00 e9 d3 00 00 00", ms_pick_a);
        MS_CODEPATCH(MSVA_PickB, 5, "bb 02 00 00 00", ms_pick_b);
    }
    if (!Skip("icons")) InstallStoryIcons();
    ModLog("[multistory] %d access stubs, %d byte patches installed", nSites, nPatches);
    return 0;
}

static void InstallHooks()
{
    // world
    MS_DETOUR(587bb0, "cFixedWorld::SetSize", Hook_SetSize, &s_origSetSize);
    MS_DETOUR(583e90, "cFixedWorld::DeleteArrays", Hook_DeleteArrays, &s_origDeleteArrays);
    MS_DETOUR(585f10, "cFixedWorld::Load", Hook_Load, &s_origLoad);
    MS_DETOUR(586b40, "cFixedWorld::Save", Hook_Save, &s_origSave);
    MS_DETOUR(5826b0, "cFixedWorld::ComputeRooms", Hook_WorldComputeRooms, &s_origComputeRooms);
    MS_DETOUR(6237b0, "cRoofLayer::DiscoverTilesToRoof", Hook_DiscoverTilesToRoof, &s_origDiscoverTiles);
    // wall manager
    MS_DETOUR(5be5c0, "WallManager::WallManager", Hook_WMCtor, &s_origWMCtor);
    MS_DETOUR(5be710, "WallManager::~WallManager", Hook_WMDtor, &s_origWMDtor);
    MS_DETOUR(5bec90, "WallManager::AsynchronousRebuild", Hook_AsyncRebuild, &s_origAsyncRebuild);
    MS_DETOUR(5becc0, "WallManager::AsynchronousRefresh", Hook_AsyncRefresh, &s_origAsyncRefresh);
    MS_DETOUR(5becf0, "WallManager::BuildGraphFromMap [replacement]", Rep_BuildGraphFromMap, 0);
    MS_DETOUR(5beff0, "WallManager::AreAllWallsConsistent [replacement]", Rep_AreAllWallsConsistent, 0);
    MS_DETOUR(5bf050, "WallManager::CleanupConstruction [replacement]", Rep_CleanupConstruction, 0);
    MS_DETOUR(5bf650, "WallManager::HandleDirty [replacement]", Rep_HandleDirty, 0);
    MS_DETOUR(5bf8a0, "WallManager::RefreshFromMap [replacement]", Rep_RefreshFromMap, 0);
    MS_DETOUR(5bf900, "WallManager::SetCutaway [replacement]", Rep_SetCutaway, 0);
    MS_DETOUR(5bf9a0, "WallManager::SetRotation [replacement]", Rep_SetRotation, 0);
    MS_DETOUR(5bfa80, "WallManager::SortLevelListsByDrawOrder [replacement]", Rep_SortLevelListsByDrawOrder, 0);
    // room manager
    MS_DETOUR(625b20, "RoomManager::RoomManager", Hook_RMCtor, &s_origRMCtor);
    MS_DETOUR(625fc0, "RoomManager::~RoomManager", Hook_RMDtor, &s_origRMDtor);
    MS_DETOUR(627420, "RoomManager::ComputeRooms", Hook_RMComputeRooms, &s_origRMComputeRooms);
    MS_DETOUR(6289b0, "RoomManager::RoomLightingChanged", Hook_RoomLightingChanged, &s_origRoomLighting);
    MS_DETOUR(628a20, "RoomManager::RoomScoreChanged", Hook_RoomScoreChanged, &s_origRoomScore);
    MS_DETOUR(628b10, "RoomManager::UpdateRooms [replacement]", Rep_UpdateRooms, 0);
    // house, stairs, UI
    MS_DETOUR(56b460, "House::GetHouseStats", Hook_GetHouseStats, &s_origGetHouseStats);
    MS_DETOUR(62fdd0, "cXMTObject::CanPlace", Hook_CanPlace, &s_origCanPlace);
    MS_DETOUR(6572d0, "cDirtTool::MayModifyTerrain", Hook_MayModifyTerrain, &s_origMayModifyTerrain);
    MS_DETOUR(658eb0, "cNewWallTool::Click", Hook_WallClick, &s_origWallClick);
    MS_DETOUR(616e30, "cXObject::TryExpression", Hook_TryExpression, &s_origTryExpression);
    MS_DETOUR(4f80d0, "cWinViewControl::TSOnCommand", Hook_ViewCtlCommand, &s_origViewCtlCommand);
    MS_DETOUR(4f8ee0, "cWinViewControl::UpdateViewFromCPState", Hook_UpdateView, &s_origUpdateView);
    MS_DETOUR(40e610, "cDDDSimsView::TSOnKeyDown", Hook_KeyDown, &s_origKeyDown);
}

// first access violations / illegal instructions: where (exe or this DLL, as rva) and the registers, to mod.log
static LONG CALLBACK CrashLogger(EXCEPTION_POINTERS *ep)
{
    static volatile LONG s_n;
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    if (code != EXCEPTION_ACCESS_VIOLATION && code != EXCEPTION_ILLEGAL_INSTRUCTION && code != EXCEPTION_INT_DIVIDE_BY_ZERO)
        return EXCEPTION_CONTINUE_SEARCH;
    if (InterlockedIncrement(&s_n) > 3) return EXCEPTION_CONTINUE_SEARCH;
    CONTEXT *c = ep->ContextRecord;
    unsigned eip = c->Eip, dll = (unsigned)&__ImageBase;
    char where[64];
    if (eip - g_api->exeBase < MS_EXE_SPAN) sprintf(where, "Sims.exe 0x%06x (va 0x%06x)", eip - g_api->exeBase, eip - g_api->exeBase + 0x400000);
    else if (eip - dll < 0x100000) sprintf(where, "multistory.dll +0x%x", eip - dll);
    else sprintf(where, "0x%08x", eip);
    ModLog("[multistory] EXCEPTION 0x%08x at %s, address 0x%08x (%s)", code, where,
           (unsigned)ep->ExceptionRecord->ExceptionInformation[1], ep->ExceptionRecord->ExceptionInformation[0] ? "write" : "read");
    ModLog("[multistory]   eax %08x ebx %08x ecx %08x edx %08x esi %08x edi %08x ebp %08x esp %08x", c->Eax, c->Ebx, c->Ecx,
           c->Edx, c->Esi, c->Edi, c->Ebp, c->Esp);
    unsigned *sp = (unsigned *)c->Esp;
    for (int i = 0; i < 24; i++)
    {
        unsigned v = 0;
        __try { v = sp[i]; } __except (EXCEPTION_EXECUTE_HANDLER) { break; }
        if (v - g_api->exeBase < MS_EXE_SPAN) ModLog("[multistory]   stack +%02x: exe va 0x%06x", i * 4, v - g_api->exeBase + 0x400000);
        else if (v - dll < 0x100000) ModLog("[multistory]   stack +%02x: dll +0x%x", i * 4, v - dll);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

extern "C" __declspec(dllexport) int __cdecl SimsMod_Init(const SimsLoaderApi *api)
{
    if (api->version != SIMSMOD_API_VERSION) return 1;
    g_api = api;
    ms_exe_base = api->exeBase;
    ModLog("[multistory] 4 stories: installing");
    if (GetEnvironmentVariableA("MULTISTORY_SKIP", g_szSkip, sizeof g_szSkip - 1) && g_szSkip[0])
        ModLog("[multistory] MULTISTORY_SKIP=%s", g_szSkip);
    AddVectoredExceptionHandler(1, CrashLogger);
    // detours first: the loader checks 16 bytes at each target, and a stub window may start within them in a small
    // function (AsynchronousRebuild); the windows themselves never overlap a detour's stolen bytes (genmspatches.py)
    InstallHooks();
    if (InstallSitesAndPatches()) return 2;
    ModLog("[multistory] ready%s", g_nFailed ? " WITH ERRORS (see above)" : "");
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) DisableThreadLibraryCalls(hinst);
    return TRUE;
}
