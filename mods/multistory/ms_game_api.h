// generated from ms_api.spec - do not edit
#pragma once
#include "simsmod.h"

extern unsigned int ms_exe_base;                 // the live module base of Sims.exe (set at init)
#define MS_EXE(rva) ((void *)(ms_exe_base + (rva)))

// ---- functions (thiscall: the object is the first argument; __fastcall with a dummy edx is the same ABI)
static inline void * MS_ObjectIterator_ctor(void *self, void * a0, void * a1, int a2) { return ((void * (__fastcall *)(void *, void *, void *, void *, int))MS_EXE(0x23d9b0))(self, 0, a0, a1, a2); }   // public: __thiscall ObjectIterator::ObjectIterator(class House *,struct CTilePt const *,int) 0x63d9b0
#define MSA_ObjectIterator_ctor 0x23d9b0
static inline void * MS_ObjectIterator_inc(void *self) { return ((void * (__fastcall *)(void *, void *))MS_EXE(0x23da90))(self, 0); }   // public: class ObjectIterator & __thiscall ObjectIterator::operator++(void) 0x63da90
#define MSA_ObjectIterator_inc 0x23da90
static inline void * MS_Vertex_ctor(void *self, void * a0, int a1) { return ((void * (__fastcall *)(void *, void *, void *, int))MS_EXE(0x27aa70))(self, 0, a0, a1); }   // public: __thiscall Vertex::Vertex(struct CTilePt const &,int) 0x67aa70
#define MSA_Vertex_ctor 0x27aa70
static inline void MS_Vertex_dtor(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27aae0))(self, 0); }   // public: __thiscall Vertex::~Vertex(void) 0x67aae0
#define MSA_Vertex_dtor 0x27aae0
static inline void * MS_cRoofLayer_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x222940))(self, 0, a0); }   // public: __thiscall cRoofLayer::cRoofLayer(int) 0x622940
#define MSA_cRoofLayer_ctor 0x222940
static inline void * MS_cArray1_uchar_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x1815e0))(self, 0, a0); }   // public: __thiscall cArray1<unsigned char>::cArray1<unsigned char>(int) 0x5815e0
#define MSA_cArray1_uchar_ctor 0x1815e0
static inline void * MS_cArray1_short_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x1816b0))(self, 0, a0); }   // public: __thiscall cArray1<short>::cArray1<short>(int) 0x5816b0
#define MSA_cArray1_short_ctor 0x1816b0
static inline void * MS_cArray1_ushort_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x181780))(self, 0, a0); }   // public: __thiscall cArray1<unsigned short>::cArray1<unsigned short>(int) 0x581780
#define MSA_cArray1_ushort_ctor 0x181780
static inline void * MS_cArray1_VertexConfig_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x181ac0))(self, 0, a0); }   // public: __thiscall cArray1<struct VertexConfig>::cArray1<struct VertexConfig>(int) 0x581ac0
#define MSA_cArray1_VertexConfig_ctor 0x181ac0
static inline void * MS_cArray1_WallTile_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x1819f0))(self, 0, a0); }   // public: __thiscall cArray1<struct WallTile>::cArray1<struct WallTile>(int) 0x5819f0
#define MSA_cArray1_WallTile_ctor 0x1819f0
static inline void MS__c2DArray_dtor(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x266930))(self, 0); }   // public: __thiscall _c2DArray::~_c2DArray(void) 0x666930
#define MSA__c2DArray_dtor 0x266930
static inline void MS__c2DArray_ClearBytes(void *self, signed char a0) { ((void (__fastcall *)(void *, void *, signed char))MS_EXE(0x2669f0))(self, 0, a0); }   // public: void __thiscall _c2DArray::ClearBytes(signed char) 0x6669f0
#define MSA__c2DArray_ClearBytes 0x2669f0
static inline int MS__c2DArray_ReadFromDisk(void *self, void * a0, long a1, short a2, void * a3) { return ((int (__fastcall *)(void *, void *, void *, long, short, void *))MS_EXE(0x266e30))(self, 0, a0, a1, a2, a3); }   // public: int __thiscall _c2DArray::ReadFromDisk(class iResFile *,long,short,void (__cdecl*)(void *,long)) 0x666e30
#define MSA__c2DArray_ReadFromDisk 0x266e30
static inline int MS__c2DArray_WriteToDisk(void *self, void * a0, long a1, short a2, bool a3) { return ((int (__fastcall *)(void *, void *, void *, long, short, bool))MS_EXE(0x267180))(self, 0, a0, a1, a2, a3); }   // public: int __thiscall _c2DArray::WriteToDisk(class iResFile *,long,short,bool) 0x667180
#define MSA__c2DArray_WriteToDisk 0x267180
static inline void MS_SwizzleShort(void * a0, long a1) { ((void (__cdecl *)(void *, long))MS_EXE(0x188820))(a0, a1); }   // void __cdecl SwizzleShort(void *,long) 0x588820
#define MSA_SwizzleShort 0x188820
static inline void MS_WallManager_AddVertex(void *self, void * a0, int a1) { ((void (__fastcall *)(void *, void *, void *, int))MS_EXE(0x1be910))(self, 0, a0, a1); }   // public: void __thiscall WallManager::AddVertex(class Vertex *,int) 0x5be910
#define MSA_WallManager_AddVertex 0x1be910
static inline void MS_Wall_BeginComputeCutaway(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27c570))(self, 0); }   // public: void __thiscall Wall::BeginComputeCutaway(void) 0x67c570
#define MSA_Wall_BeginComputeCutaway 0x27c570
static inline void MS_Wall_ChangeWallPattern(void *self, int a0, int a1) { ((void (__fastcall *)(void *, void *, int, int))MS_EXE(0x27cc80))(self, 0, a0, a1); }   // public: void __thiscall Wall::ChangeWallPattern(int,int) 0x67cc80
#define MSA_Wall_ChangeWallPattern 0x27cc80
static inline void MS_Wall_ChangeWallStyle(void *self, int a0, int a1) { ((void (__fastcall *)(void *, void *, int, int))MS_EXE(0x27cd00))(self, 0, a0, a1); }   // public: void __thiscall Wall::ChangeWallStyle(int,int) 0x67cd00
#define MSA_Wall_ChangeWallStyle 0x27cd00
static inline void MS_Wall_FixFences(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ec50))(self, 0); }   // public: void __thiscall Wall::FixFences(void) 0x67ec50
#define MSA_Wall_FixFences 0x27ec50
static inline bool MS_Wall_IsSegmentCountConsistent(void *self) { return ((bool (__fastcall *)(void *, void *))MS_EXE(0x27cd30))(self, 0); }   // public: bool __thiscall Wall::IsSegmentCountConsistent(void) 0x67cd30
#define MSA_Wall_IsSegmentCountConsistent 0x27cd30
static inline void MS_Wall_RefreshFromMap(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ee70))(self, 0); }   // public: void __thiscall Wall::RefreshFromMap(void) 0x67ee70
#define MSA_Wall_RefreshFromMap 0x27ee70
static inline void MS_Wall_SetCutaway(void *self, char a0) { ((void (__fastcall *)(void *, void *, char))MS_EXE(0x27f180))(self, 0, a0); }   // public: void __thiscall Wall::SetCutaway(char) 0x67f180
#define MSA_Wall_SetCutaway 0x27f180
static inline void MS_Wall_SetRotation(void *self, int a0) { ((void (__fastcall *)(void *, void *, int))MS_EXE(0x27f300))(self, 0, a0); }   // public: void __thiscall Wall::SetRotation(int) 0x67f300
#define MSA_Wall_SetRotation 0x27f300
static inline void MS_Vertex_BuildWalls(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ab50))(self, 0); }   // public: void __thiscall Vertex::BuildWalls(void) 0x67ab50
#define MSA_Vertex_BuildWalls 0x27ab50
static inline void MS_Vertex_ComputeCutaway(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ac60))(self, 0); }   // public: void __thiscall Vertex::ComputeCutaway(void) 0x67ac60
#define MSA_Vertex_ComputeCutaway 0x27ac60
static inline void MS_Vertex_ComputeCutawaySecondPass(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ade0))(self, 0); }   // public: void __thiscall Vertex::ComputeCutawaySecondPass(void) 0x67ade0
#define MSA_Vertex_ComputeCutawaySecondPass 0x27ade0
static inline void MS_Vertex_SetRotation(void *self, int a0) { ((void (__fastcall *)(void *, void *, int))MS_EXE(0x27bd70))(self, 0, a0); }   // public: void __thiscall Vertex::SetRotation(int) 0x67bd70
#define MSA_Vertex_SetRotation 0x27bd70
static inline bool MS_VertexPtr_IsoCompare(void * a0, void * a1) { return ((bool (__cdecl *)(void *, void *))MS_EXE(0x1bf7a0))(a0, a1); }   // public: static bool __cdecl VertexPtr::IsoCompare(class VertexPtr const &,class VertexPtr const &) 0x5bf7a0
#define MSA_VertexPtr_IsoCompare 0x1bf7a0
static inline bool MS_WallPtr_IsoCompare(void * a0, void * a1) { return ((bool (__cdecl *)(void *, void *))MS_EXE(0x1bf7e0))(a0, a1); }   // public: static bool __cdecl WallPtr::IsoCompare(class WallPtr const &,class WallPtr const &) 0x5bf7e0
#define MSA_WallPtr_IsoCompare 0x1bf7e0
static inline void MS_GViewer_DirtyCutaway(void) { ((void (__cdecl *)(void))MS_EXE(0x1dfe20))(); }   // void __cdecl GViewer_DirtyCutaway(void) 0x5dfe20
#define MSA_GViewer_DirtyCutaway 0x1dfe20
static inline void MS_RoomManager_ComputeRooms(void *self, int a0) { ((void (__fastcall *)(void *, void *, int))MS_EXE(0x227420))(self, 0, a0); }   // public: void __thiscall RoomManager::ComputeRooms(int) 0x627420
#define MSA_RoomManager_ComputeRooms 0x227420
static inline int MS_Room_GetLevel(void *self) { return ((int (__fastcall *)(void *, void *))MS_EXE(0x2278c0))(self, 0); }   // public: int __thiscall Room::GetLevel(void)const 0x6278c0
#define MSA_Room_GetLevel 0x2278c0
static inline bool MS_Room_IsOutside(void *self) { return ((bool (__fastcall *)(void *, void *))MS_EXE(0x2280a0))(self, 0); }   // public: bool __thiscall Room::IsOutside(void) 0x6280a0
#define MSA_Room_IsOutside 0x2280a0
static inline void * MS_RoomManager_GetRoom(void *self, unsigned short a0) { return ((void * (__fastcall *)(void *, void *, unsigned short))MS_EXE(0x227c70))(self, 0, a0); }   // public: class Room * __thiscall RoomManager::GetRoom(unsigned short) 0x627c70
#define MSA_RoomManager_GetRoom 0x227c70
static inline bool MS_RoomManager_ResolveDiagonal(void *self, void * a0, void * a1, void * a2, void * a3, void * a4) { return ((bool (__fastcall *)(void *, void *, void *, void *, void *, void *, void *))MS_EXE(0x228750))(self, 0, a0, a1, a2, a3, a4); }   // public: bool __thiscall RoomManager::ResolveDiagonal(struct CTilePt const &,unsigned short *,unsigned short *,enum Room::Sides *,enum Room::Sides *) 0x628750
#define MSA_RoomManager_ResolveDiagonal 0x228750
static inline bool MS_cDirtTool_HasFlatObjects(void *self, void * a0) { return ((bool (__fastcall *)(void *, void *, void *))MS_EXE(0x257140))(self, 0, a0); }   // public: bool __thiscall cDirtTool::HasFlatObjects(struct CTilePt const &) 0x657140
#define MSA_cDirtTool_HasFlatObjects 0x257140
static inline void MS_cRoofLayer_GenerateRoof(void *self, int a0, void * a1) { ((void (__fastcall *)(void *, void *, int, void *))MS_EXE(0x223d30))(self, 0, a0, a1); }   // public: void __thiscall cRoofLayer::GenerateRoof(int,class cRoofLayer *) 0x623d30
#define MSA_cRoofLayer_GenerateRoof 0x223d30
static inline int MS_CPState_GetLevel(void *self) { return ((int (__fastcall *)(void *, void *))MS_EXE(0x007a00))(self, 0); }   // public: int __thiscall CPState::GetLevel(void) 0x407a00
#define MSA_CPState_GetLevel 0x007a00
static inline int MS_CPState_GetMode(void *self) { return ((int (__fastcall *)(void *, void *))MS_EXE(0x007a20))(self, 0); }   // public: int __thiscall CPState::GetMode(void) 0x407a20
#define MSA_CPState_GetMode 0x007a20
static inline void MS_CPState_SetLevel(void *self, int a0, int a1) { ((void (__fastcall *)(void *, void *, int, int))MS_EXE(0x008000))(self, 0, a0, a1); }   // public: void __thiscall CPState::SetLevel(int,enum CPState::Synchron) 0x408000
#define MSA_CPState_SetLevel 0x008000
static inline void MS_cTSWinBtn_SetState(void *self, long a0) { ((void (__fastcall *)(void *, void *, long))MS_EXE(0x2e9b30))(self, 0, a0); }   // public: void __thiscall cTSWinBtn::SetState(long) 0x6e9b30
#define MSA_cTSWinBtn_SetState 0x2e9b30
static inline void MS_cTSWinBtn_SetEnabled(void *self, bool a0) { ((void (__fastcall *)(void *, void *, bool))MS_EXE(0x082560))(self, 0, a0); }   // public: void __thiscall cTSWinBtn::SetEnabled(bool) 0x482560
#define MSA_cTSWinBtn_SetEnabled 0x082560
static inline void MS_cWinPeople_StopTracking(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x0ba880))(self, 0); }   // public: void __thiscall cWinPeople::StopTracking(void) 0x4ba880
#define MSA_cWinPeople_StopTracking 0x0ba880
static inline bool MS_cSoundPlayer_PlayBySource(void *self, void * a0, short a1) { return ((bool (__fastcall *)(void *, void *, void *, short))MS_EXE(0x127250))(self, 0, a0, a1); }   // public: bool __thiscall cSoundPlayer::PlayBySource(char const *,short) 0x527250
#define MSA_cSoundPlayer_PlayBySource 0x127250
static inline bool MS_cTSWin_TSOnKeyDown(void *self, unsigned long a0, unsigned long a1) { return ((bool (__fastcall *)(void *, void *, unsigned long, unsigned long))MS_EXE(0x2df120))(self, 0, a0, a1); }   // public: virtual bool __thiscall cTSWin::TSOnKeyDown(unsigned long,unsigned long) 0x6df120
#define MSA_cTSWin_TSOnKeyDown 0x2df120
static inline void * MS_TreeSim_GetCurElem(void *self) { return ((void * (__fastcall *)(void *, void *))MS_EXE(0x22da10))(self, 0); }   // public: struct StackElem * __thiscall TreeSim::GetCurElem(void) 0x62da10
#define MSA_TreeSim_GetCurElem 0x22da10
static inline void * MS_ObjSelector_GetMasterSelector(void *self) { return ((void * (__fastcall *)(void *, void *))MS_EXE(0x1964a0))(self, 0); }   // public: class ObjSelector * __thiscall ObjSelector::GetMasterSelector(void) 0x5964a0
#define MSA_ObjSelector_GetMasterSelector 0x1964a0
static inline void * MS_ObjectModule_GetObject(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x0558f0))(self, 0, a0); }   // public: class cXObject * __thiscall ObjectModule::GetObject(int) 0x4558f0
#define MSA_ObjectModule_GetObject 0x0558f0
static inline void * MS_operator_new(unsigned int a0) { return ((void * (__cdecl *)(unsigned int))MS_EXE(0x3a78be))(a0); }   // C runtime, 0x7a78be
static inline void MS_free(void * a0) { ((void (__cdecl *)(void *))MS_EXE(0x3c62c0))(a0); }   // C runtime, 0x7c62c0

// ---- layout (MSO_ offsetof, MSZ_ field size, MSS_ sizeof, MSB_ base-subobject offset) and constants (MSC_)
#define MSO_cFixedWorld_m_nSize                          0x10
#define MSZ_cFixedWorld_m_nSize                          0x4
#define MSO_cFixedWorld_m_pFloors                        0x24
#define MSZ_cFixedWorld_m_pFloors                        0x8
#define MSO_cFixedWorld_m_pRooms                         0x2c
#define MSZ_cFixedWorld_m_pRooms                         0x8
#define MSO_cFixedWorld_m_pFlags                         0x34
#define MSZ_cFixedWorld_m_pFlags                         0x8
#define MSO_cFixedWorld_m_pObjects                       0x3c
#define MSZ_cFixedWorld_m_pObjects                       0x8
#define MSO_cFixedWorld_m_pWalls                         0x44
#define MSZ_cFixedWorld_m_pWalls                         0x8
#define MSO_cFixedWorld_m_pWallVertexMask                0x4c
#define MSZ_cFixedWorld_m_pWallVertexMask                0x8
#define MSO_cFixedWorld_m_pRoof                          0x58
#define MSZ_cFixedWorld_m_pRoof                          0x8
#define MSO_cFixedWorld_m_pLightEntries                  0x68
#define MSZ_cFixedWorld_m_pLightEntries                  0x8
#define MSO_cFixedWorld_m_bHasBeenTo2ndLevel             0x80
#define MSZ_cFixedWorld_m_bHasBeenTo2ndLevel             0x1
#define MSO_cFixedWorld_m_bIs2ndLevelFloorable           0x81
#define MSZ_cFixedWorld_m_bIs2ndLevelFloorable           0x1
#define MSS_cFixedWorld                                  0x84
#define MSO_WallManager_m_nRotation                      0x4
#define MSZ_WallManager_m_nRotation                      0x4
#define MSO_cRotatableWorld_m_nRotation                  0x84
#define MSZ_cRotatableWorld_m_nRotation                  0x4
#define MSO_cDirtTool_m_AnchorTile                       0x34
#define MSZ_cDirtTool_m_AnchorTile                       0x3
#define MSO_cDirtTool_m_nMode                            0x38
#define MSZ_cDirtTool_m_nMode                            0x4
#define MSO_WallManager_m_pVertexLists                   0x8
#define MSZ_WallManager_m_pVertexLists                   0x8
#define MSO_WallManager_m_pWallLists                     0x10
#define MSZ_WallManager_m_pWallLists                     0x8
#define MSO_WallManager_m_bWallsDirty                    0x38
#define MSZ_WallManager_m_bWallsDirty                    0x8
#define MSO_WallManager_m_bWallpaperDirty                0x40
#define MSZ_WallManager_m_bWallpaperDirty                0x8
#define MSS_WallManager                                  0x48
#define MSO_RoomManager_m_Rooms                          0x4
#define MSZ_RoomManager_m_Rooms                          0x8
#define MSO_RoomManager_m_nDirtyLevelMask                0x28
#define MSZ_RoomManager_m_nDirtyLevelMask                0x4
#define MSO_RoomManager_m_pShade                         0x38
#define MSZ_RoomManager_m_pShade                         0x8
#define MSS_RoomManager                                  0x40
#define MSO_House_m_pFixedWorld                          0x1c
#define MSZ_House_m_pFixedWorld                          0x4
#define MSO_HouseStats_m_nFloors                         0x10
#define MSZ_HouseStats_m_nFloors                         0x4
#define MSO_CPState_m_dwFlags                            0xc
#define MSZ_CPState_m_dwFlags                            0x4
#define MSO_cWinViewControl_m_pCPState                   0x134
#define MSZ_cWinViewControl_m_pCPState                   0x4
#define MSO_cWinViewControl_m_pFirstStoryBtn             0x16c
#define MSZ_cWinViewControl_m_pFirstStoryBtn             0x4
#define MSO_cWinViewControl_m_pSecondStoryBtn            0x170
#define MSZ_cWinViewControl_m_pSecondStoryBtn            0x4
#define MSO_cDDDSimsView_m_pCPState                      0x164
#define MSZ_cDDDSimsView_m_pCPState                      0x4
#define MSO_cDDDSimsView_m_nKeyDepth                     0x1a4
#define MSZ_cDDDSimsView_m_nKeyDepth                     0x4
#define MSO_cDDDSimsView_m_pEdgeScroller                 0x1a8
#define MSZ_cDDDSimsView_m_pEdgeScroller                 0x4
#define MSB_cDDDSimsView_cTSWin                          0x0
#define MSO_cSimsSettings_m_pSoundSys                    0x10
#define MSZ_cSimsSettings_m_pSoundSys                    0x4
#define MSO_cSimsSettings_m_pGame                        0x0
#define MSZ_cSimsSettings_m_pGame                        0x4
#define MSO_cXMTObject_m_pMaster                         0x194
#define MSZ_cXMTObject_m_pMaster                         0x4
#define MSO_cXMTObject_m_pNextPart                       0x190
#define MSZ_cXMTObject_m_pNextPart                       0x4
#define MSO_cXMTObject_m_nOffsetLevel                    0x1ac
#define MSZ_cXMTObject_m_nOffsetLevel                    0x4
#define MSO_cXMTObject_m_nLevel                          0x110
#define MSZ_cXMTObject_m_nLevel                          0x4
#define MSO_cXObject_m_pModule                           0xdc
#define MSZ_cXObject_m_pModule                           0x4
#define MSO_cXObject_m_pSelector                         0x120
#define MSZ_cXObject_m_pSelector                         0x4
#define MSB_cXObject_TreeSim                             0x0
#define MSO_ObjSelector_m_pObjd                          0x60
#define MSZ_ObjSelector_m_pObjd                          0x4
#define MSO_OBJD_build_mode_type                         0x8a
#define MSZ_OBJD_build_mode_type                         0x2
#define MSO_StackElem_m_nStackObjectId                   0x4
#define MSZ_StackElem_m_nStackObjectId                   0x2
#define MSO_BehaviorNodeParam_m_Op                       0x0
#define MSZ_BehaviorNodeParam_m_Op                       0x8
#define MSO_BehaviorNodeParam_m_Param                    0x0
#define MSZ_BehaviorNodeParam_m_Param                    0x8
#define MSS_BehaviorNodeParam                            0x8
#define MSO__c2DArray_m_nWidth                           0x4
#define MSZ__c2DArray_m_nWidth                           0x4
#define MSO__c2DArray_m_nHeight                          0x8
#define MSZ__c2DArray_m_nHeight                          0x4
#define MSO__c2DArray_m_pRows                            0xc
#define MSZ__c2DArray_m_pRows                            0x4
#define MSS__c2DArray                                    0x2c
#define MSS_cRoofLayer                                   0x4c
#define MSB_cRoofLayer__c2DArray                         0x4
#define MSS_cRoofTile                                    0x2
#define MSO_cRoofTile_m_nFlags                           0x0
#define MSZ_cRoofTile_m_nFlags                           0x1
#define MSS_WallTile                                     0xe
#define MSO_WallTile_m_nWallMask                         0x0
#define MSZ_WallTile_m_nWallMask                         0x1
#define MSS_VertexConfig                                 0x1
#define MSO_Vertex_m_nRefCount                           0x20
#define MSZ_Vertex_m_nRefCount                           0x4
#define MSO_Vertex_m_nConfig                             0x8
#define MSZ_Vertex_m_nConfig                             0x1
#define MSO_Vertex_m_pWalls                              0x10
#define MSZ_Vertex_m_pWalls                              0x10
#define MSS_Vertex                                       0x60
#define MSO_Wall_m_nRefCount                             0x58
#define MSZ_Wall_m_nRefCount                             0x4
#define MSO_ObjectIterator_m_pCur                        0x4
#define MSZ_ObjectIterator_m_pCur                        0x4
#define MSS_ObjectIterator                               0xc
#define MSS_CTilePt                                      0x3
#define MSO_CTilePt_nX                                   0x0
#define MSZ_CTilePt_nX                                   0x1
#define MSO_CTilePt_nY                                   0x1
#define MSZ_CTilePt_nY                                   0x1
#define MSO_CTilePt_nLevel                               0x2
#define MSZ_CTilePt_nLevel                               0x1
#define MSC_CPState_kBuild                               0
#define MSC_CPState_kImmediate                           0
#define MSC_CPState_kDeferred                            1
#define MSC_kWinMsg_ButtonClick                          1
#define MSC_kObjData_Room                                29
#define MSC_SCOPE_my                                     3
#define MSC_SCOPE_literal                                7
#define MSC_kArry_Floors                                 11
#define MSC_kArry_Walls                                  12
#define MSC_kArry_Objects                                3
#define MSC_kArry_Flags                                  8

// ---- vtable byte offsets (MSV_)
#define MSV_Wall_dtor                                    0x0
#define MSV_cRoofLayer_dtor                              0x0
#define MSV_cTool_OnKeyDown                              0x48
#define MSV_ScrollingStrategy_OnKeyDown                  0x28

// ---- globals: MSG_<name> = rva (use MS_EXE(MSG_...))
#define MSG_g_pWorld                                     0x6675b8
#define MSG_g_pAppSettings                               0x667044
#define MSG_g_pRoomManager                               0x67b9dc
#define MSG_g_pLot                                       0x6675b4
#define MSG_g_TileRotateLUT                              0x66cb08
#define MSG_g_aWallMaskIsJunction                        0x67ceb0
#define MSG_g_aWallMaskIsEndpoint                        0x67cfb0
#define MSG_g_nUserActionError                           0x667078
#define MSG_g_pWinPeople                                 0x665b00
#define MSG_XViewer__sTool                               0x667108

// ---- RTTI type descriptors: MSR_<Class> = rva (vtable[-1] -> complete object locator -> +0x0C)
#define MSR_cXObject                                     0x65c180   // .?AVcXObject@@
#define MSR_cXMTObject                                   0x65c1b0   // .?AVcXMTObject@@

// ---- detour signatures (16 bytes + the mask of relocated dwords), for SimsLoaderApi::Detour
static const OrigSig MS_SIG_40e610 = { 0x00e610, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x8b, 0x45, 0x0c, 0x8a, 0xd0, 0x53, 0x8b, 0x5d, 0x08, 0x80 }, 0x0000 };
static const OrigSig MS_SIG_4f80d0 = { 0x0f80d0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x07, 0x7e, 0x95, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_4f8ee0 = { 0x0f8ee0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x1c, 0x53, 0x8b, 0x5d, 0x08, 0x56, 0x57, 0x8b, 0xf9, 0x8b, 0x43 }, 0x0000 };
static const OrigSig MS_SIG_56b460 = { 0x16b460, 16, { 0x55, 0x8b, 0xec, 0x51, 0x53, 0x8b, 0x5d, 0x08, 0x56, 0x57, 0x8b, 0xf9, 0xc7, 0x03, 0x00, 0x00 }, 0x0000 };
static const OrigSig MS_SIG_5826b0 = { 0x1826b0, 16, { 0x55, 0x8b, 0xec, 0x81, 0xec, 0x1c, 0x01, 0x00, 0x00, 0xa1, 0x80, 0x33, 0xa5, 0x00, 0x33, 0xc5 }, 0x0400 };
static const OrigSig MS_SIG_583e90 = { 0x183e90, 16, { 0x53, 0x56, 0x8b, 0xf1, 0x57, 0x8b, 0x7e, 0x14, 0x85, 0xff, 0x74, 0x12, 0x8b, 0xcf, 0xe8, 0x8d }, 0x0000 };
static const OrigSig MS_SIG_585f10 = { 0x185f10, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x83, 0xf8, 0x95, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_586b40 = { 0x186b40, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0xc3, 0xf8, 0x95, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_587bb0 = { 0x187bb0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x7f, 0xfa, 0x95, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_5be5c0 = { 0x1be5c0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0xa4, 0x23, 0x96, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_5be710 = { 0x1be710, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x53, 0x8b, 0xd9, 0x56, 0x57, 0x89, 0x5d, 0xf4, 0x8b, 0x73 }, 0x0000 };
static const OrigSig MS_SIG_5bec90 = { 0x1bec90, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x45, 0x08, 0x85, 0xc0, 0x74, 0x0c, 0xc7, 0x44, 0x81, 0x34, 0x01, 0x00 }, 0x0000 };
static const OrigSig MS_SIG_5becc0 = { 0x1becc0, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x45, 0x08, 0x85, 0xc0, 0x74, 0x0c, 0xc7, 0x44, 0x81, 0x3c, 0x01, 0x00 }, 0x0000 };
static const OrigSig MS_SIG_5becf0 = { 0x1becf0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x64, 0x24, 0x96, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_5beff0 = { 0x1beff0, 16, { 0x55, 0x8b, 0xec, 0x51, 0x53, 0x56, 0x57, 0xc6, 0x45, 0xff, 0x01, 0x8d, 0x79, 0x10, 0x33, 0xdb }, 0x0000 };
static const OrigSig MS_SIG_5bf050 = { 0x1bf050, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x08, 0x53, 0x8d, 0x41, 0x08, 0xb9, 0x02, 0x00, 0x00, 0x00, 0x56 }, 0x0000 };
static const OrigSig MS_SIG_5bf650 = { 0x1bf650, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x08, 0x53, 0x8d, 0x59, 0x10, 0x89, 0x4d, 0xf8, 0x56, 0xb9, 0x02 }, 0x0000 };
static const OrigSig MS_SIG_5bf8a0 = { 0x1bf8a0, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x55, 0x08, 0x85, 0xd2, 0x53, 0xbb, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x45 }, 0x0000 };
static const OrigSig MS_SIG_5bf900 = { 0x1bf900, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x55, 0x0c, 0x85, 0xd2, 0x56, 0x8b, 0xf1, 0xb9, 0x02, 0x00, 0x00, 0x00 }, 0x0000 };
static const OrigSig MS_SIG_5bf9a0 = { 0x1bf9a0, 16, { 0x55, 0x8b, 0xec, 0x51, 0x8b, 0xc1, 0xb9, 0x02, 0x00, 0x00, 0x00, 0x53, 0x8b, 0x5d, 0x08, 0x56 }, 0x0000 };
static const OrigSig MS_SIG_5bfa80 = { 0x1bfa80, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x55, 0x08, 0x85, 0xd2, 0x57, 0xbf, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x45 }, 0x0000 };
static const OrigSig MS_SIG_616e30 = { 0x216e30, 16, { 0x55, 0x8b, 0xec, 0x83, 0xe4, 0xf8, 0x83, 0xec, 0x14, 0x53, 0x56, 0x8b, 0xf1, 0xc7, 0x44, 0x24 }, 0x0000 };
static const OrigSig MS_SIG_6237b0 = { 0x2237b0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x50, 0x53, 0x8b, 0xd9, 0x56, 0x57, 0x89, 0x5d, 0xf4, 0x8b, 0x4b }, 0x0000 };
static const OrigSig MS_SIG_625b20 = { 0x225b20, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x21, 0x5f, 0x96, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_625fc0 = { 0x225fc0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x70, 0x5f, 0x96, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };
static const OrigSig MS_SIG_627420 = { 0x227420, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x20, 0x80, 0x3d, 0x34, 0xba, 0xa7, 0x00, 0x00, 0x53, 0x56, 0x57 }, 0x0100 };
static const OrigSig MS_SIG_6289b0 = { 0x2289b0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x53, 0x56, 0x57, 0x0f, 0xb7, 0x7d, 0x08, 0x8b, 0xf1, 0x8b }, 0x0000 };
static const OrigSig MS_SIG_628a20 = { 0x228a20, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x53, 0x56, 0x57, 0x0f, 0xb7, 0x7d, 0x08, 0x8b, 0xf1, 0x8b }, 0x0000 };
static const OrigSig MS_SIG_628b10 = { 0x228b10, 16, { 0x8b, 0x41, 0x28, 0x85, 0xc0, 0x74, 0x25, 0x83, 0xe8, 0x01, 0x74, 0x19, 0x83, 0xe8, 0x01, 0x74 }, 0x0000 };
static const OrigSig MS_SIG_62fdd0 = { 0x22fdd0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x10, 0x83, 0x7d, 0x10, 0x00, 0x57, 0x8b, 0xf9, 0x89, 0x7d, 0xfc }, 0x0000 };
static const OrigSig MS_SIG_6572d0 = { 0x2572d0, 14, { 0x55, 0x8b, 0xec, 0x64, 0xa1, 0x2c, 0x00, 0x00, 0x00, 0x83, 0xec, 0x24, 0x8b, 0x15, 0x00, 0x00 }, 0x0000 };
