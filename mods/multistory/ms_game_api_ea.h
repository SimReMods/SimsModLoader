// generated from ms_api.spec, ported to the EA app build by tools/port_multistory.py - do not edit
#pragma once
#include "simsmod.h"

extern unsigned int ms_exe_base;                 // the live module base of Sims.exe (set at init)
#define MS_EXE(rva) ((void *)(ms_exe_base + (rva)))

// ---- functions (thiscall: the object is the first argument; __fastcall with a dummy edx is the same ABI)
static inline void * MS_ObjectIterator_ctor(void *self, void * a0, void * a1, int a2) { return ((void * (__fastcall *)(void *, void *, void *, void *, int))MS_EXE(0x23da50))(self, 0, a0, a1, a2); }   // public: __thiscall ObjectIterator::ObjectIterator(class House *,struct CTilePt const *,int) 0x63da50
#define MSA_ObjectIterator_ctor 0x23da50
static inline void * MS_ObjectIterator_inc(void *self) { return ((void * (__fastcall *)(void *, void *))MS_EXE(0x23db30))(self, 0); }   // public: class ObjectIterator & __thiscall ObjectIterator::operator++(void) 0x63db30
#define MSA_ObjectIterator_inc 0x23db30
static inline void * MS_Vertex_ctor(void *self, void * a0, int a1) { return ((void * (__fastcall *)(void *, void *, void *, int))MS_EXE(0x27ab00))(self, 0, a0, a1); }   // public: __thiscall Vertex::Vertex(struct CTilePt const &,int) 0x67ab00
#define MSA_Vertex_ctor 0x27ab00
static inline void MS_Vertex_dtor(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ab70))(self, 0); }   // public: __thiscall Vertex::~Vertex(void) 0x67ab70
#define MSA_Vertex_dtor 0x27ab70
static inline void * MS_cRoofLayer_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x2229e0))(self, 0, a0); }   // public: __thiscall cRoofLayer::cRoofLayer(int) 0x6229e0
#define MSA_cRoofLayer_ctor 0x2229e0
static inline void * MS_cArray1_uchar_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x181650))(self, 0, a0); }   // public: __thiscall cArray1<unsigned char>::cArray1<unsigned char>(int) 0x581650
#define MSA_cArray1_uchar_ctor 0x181650
static inline void * MS_cArray1_short_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x181720))(self, 0, a0); }   // public: __thiscall cArray1<short>::cArray1<short>(int) 0x581720
#define MSA_cArray1_short_ctor 0x181720
static inline void * MS_cArray1_ushort_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x1817f0))(self, 0, a0); }   // public: __thiscall cArray1<unsigned short>::cArray1<unsigned short>(int) 0x5817f0
#define MSA_cArray1_ushort_ctor 0x1817f0
static inline void * MS_cArray1_VertexConfig_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x181b30))(self, 0, a0); }   // public: __thiscall cArray1<struct VertexConfig>::cArray1<struct VertexConfig>(int) 0x581b30
#define MSA_cArray1_VertexConfig_ctor 0x181b30
static inline void * MS_cArray1_WallTile_ctor(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x181a60))(self, 0, a0); }   // public: __thiscall cArray1<struct WallTile>::cArray1<struct WallTile>(int) 0x581a60
#define MSA_cArray1_WallTile_ctor 0x181a60
static inline void MS__c2DArray_dtor(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x2669d0))(self, 0); }   // public: __thiscall _c2DArray::~_c2DArray(void) 0x6669d0
#define MSA__c2DArray_dtor 0x2669d0
static inline void MS__c2DArray_ClearBytes(void *self, signed char a0) { ((void (__fastcall *)(void *, void *, signed char))MS_EXE(0x266a90))(self, 0, a0); }   // public: void __thiscall _c2DArray::ClearBytes(signed char) 0x666a90
#define MSA__c2DArray_ClearBytes 0x266a90
static inline int MS__c2DArray_ReadFromDisk(void *self, void * a0, long a1, short a2, void * a3) { return ((int (__fastcall *)(void *, void *, void *, long, short, void *))MS_EXE(0x266ed0))(self, 0, a0, a1, a2, a3); }   // public: int __thiscall _c2DArray::ReadFromDisk(class iResFile *,long,short,void (__cdecl*)(void *,long)) 0x666ed0
#define MSA__c2DArray_ReadFromDisk 0x266ed0
static inline int MS__c2DArray_WriteToDisk(void *self, void * a0, long a1, short a2, bool a3) { return ((int (__fastcall *)(void *, void *, void *, long, short, bool))MS_EXE(0x267220))(self, 0, a0, a1, a2, a3); }   // public: int __thiscall _c2DArray::WriteToDisk(class iResFile *,long,short,bool) 0x667220
#define MSA__c2DArray_WriteToDisk 0x267220
static inline void MS_SwizzleShort(void * a0, long a1) { ((void (__cdecl *)(void *, long))MS_EXE(0x188890))(a0, a1); }   // void __cdecl SwizzleShort(void *,long) 0x588890
#define MSA_SwizzleShort 0x188890
static inline void MS_WallManager_AddVertex(void *self, void * a0, int a1) { ((void (__fastcall *)(void *, void *, void *, int))MS_EXE(0x1be990))(self, 0, a0, a1); }   // public: void __thiscall WallManager::AddVertex(class Vertex *,int) 0x5be990
#define MSA_WallManager_AddVertex 0x1be990
static inline void MS_Wall_BeginComputeCutaway(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27c600))(self, 0); }   // public: void __thiscall Wall::BeginComputeCutaway(void) 0x67c600
#define MSA_Wall_BeginComputeCutaway 0x27c600
static inline void MS_Wall_ChangeWallPattern(void *self, int a0, int a1) { ((void (__fastcall *)(void *, void *, int, int))MS_EXE(0x27cd10))(self, 0, a0, a1); }   // public: void __thiscall Wall::ChangeWallPattern(int,int) 0x67cd10
#define MSA_Wall_ChangeWallPattern 0x27cd10
static inline void MS_Wall_ChangeWallStyle(void *self, int a0, int a1) { ((void (__fastcall *)(void *, void *, int, int))MS_EXE(0x27cd90))(self, 0, a0, a1); }   // public: void __thiscall Wall::ChangeWallStyle(int,int) 0x67cd90
#define MSA_Wall_ChangeWallStyle 0x27cd90
static inline void MS_Wall_FixFences(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ece0))(self, 0); }   // public: void __thiscall Wall::FixFences(void) 0x67ece0
#define MSA_Wall_FixFences 0x27ece0
static inline bool MS_Wall_IsSegmentCountConsistent(void *self) { return ((bool (__fastcall *)(void *, void *))MS_EXE(0x27cdc0))(self, 0); }   // public: bool __thiscall Wall::IsSegmentCountConsistent(void) 0x67cdc0
#define MSA_Wall_IsSegmentCountConsistent 0x27cdc0
static inline void MS_Wall_RefreshFromMap(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ef00))(self, 0); }   // public: void __thiscall Wall::RefreshFromMap(void) 0x67ef00
#define MSA_Wall_RefreshFromMap 0x27ef00
static inline void MS_Wall_SetCutaway(void *self, char a0) { ((void (__fastcall *)(void *, void *, char))MS_EXE(0x27f210))(self, 0, a0); }   // public: void __thiscall Wall::SetCutaway(char) 0x67f210
#define MSA_Wall_SetCutaway 0x27f210
static inline void MS_Wall_SetRotation(void *self, int a0) { ((void (__fastcall *)(void *, void *, int))MS_EXE(0x27f390))(self, 0, a0); }   // public: void __thiscall Wall::SetRotation(int) 0x67f390
#define MSA_Wall_SetRotation 0x27f390
static inline void MS_Vertex_BuildWalls(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27abe0))(self, 0); }   // public: void __thiscall Vertex::BuildWalls(void) 0x67abe0
#define MSA_Vertex_BuildWalls 0x27abe0
static inline void MS_Vertex_ComputeCutaway(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27acf0))(self, 0); }   // public: void __thiscall Vertex::ComputeCutaway(void) 0x67acf0
#define MSA_Vertex_ComputeCutaway 0x27acf0
static inline void MS_Vertex_ComputeCutawaySecondPass(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x27ae70))(self, 0); }   // public: void __thiscall Vertex::ComputeCutawaySecondPass(void) 0x67ae70
#define MSA_Vertex_ComputeCutawaySecondPass 0x27ae70
static inline void MS_Vertex_SetRotation(void *self, int a0) { ((void (__fastcall *)(void *, void *, int))MS_EXE(0x27be00))(self, 0, a0); }   // public: void __thiscall Vertex::SetRotation(int) 0x67be00
#define MSA_Vertex_SetRotation 0x27be00
static inline bool MS_VertexPtr_IsoCompare(void * a0, void * a1) { return ((bool (__cdecl *)(void *, void *))MS_EXE(0x1bf820))(a0, a1); }   // public: static bool __cdecl VertexPtr::IsoCompare(class VertexPtr const &,class VertexPtr const &) 0x5bf820
#define MSA_VertexPtr_IsoCompare 0x1bf820
static inline bool MS_WallPtr_IsoCompare(void * a0, void * a1) { return ((bool (__cdecl *)(void *, void *))MS_EXE(0x1bf860))(a0, a1); }   // public: static bool __cdecl WallPtr::IsoCompare(class WallPtr const &,class WallPtr const &) 0x5bf860
#define MSA_WallPtr_IsoCompare 0x1bf860
static inline void MS_GViewer_DirtyCutaway(void) { ((void (__cdecl *)(void))MS_EXE(0x1dfe90))(); }   // void __cdecl GViewer_DirtyCutaway(void) 0x5dfe90
#define MSA_GViewer_DirtyCutaway 0x1dfe90
static inline void MS_RoomManager_ComputeRooms(void *self, int a0) { ((void (__fastcall *)(void *, void *, int))MS_EXE(0x2274d0))(self, 0, a0); }   // public: void __thiscall RoomManager::ComputeRooms(int) 0x6274d0
#define MSA_RoomManager_ComputeRooms 0x2274d0
static inline int MS_Room_GetLevel(void *self) { return ((int (__fastcall *)(void *, void *))MS_EXE(0x227970))(self, 0); }   // public: int __thiscall Room::GetLevel(void)const 0x627970
#define MSA_Room_GetLevel 0x227970
static inline bool MS_Room_IsOutside(void *self) { return ((bool (__fastcall *)(void *, void *))MS_EXE(0x228150))(self, 0); }   // public: bool __thiscall Room::IsOutside(void) 0x628150
#define MSA_Room_IsOutside 0x228150
static inline void * MS_RoomManager_GetRoom(void *self, unsigned short a0) { return ((void * (__fastcall *)(void *, void *, unsigned short))MS_EXE(0x227d20))(self, 0, a0); }   // public: class Room * __thiscall RoomManager::GetRoom(unsigned short) 0x627d20
#define MSA_RoomManager_GetRoom 0x227d20
static inline bool MS_RoomManager_ResolveDiagonal(void *self, void * a0, void * a1, void * a2, void * a3, void * a4) { return ((bool (__fastcall *)(void *, void *, void *, void *, void *, void *, void *))MS_EXE(0x228800))(self, 0, a0, a1, a2, a3, a4); }   // public: bool __thiscall RoomManager::ResolveDiagonal(struct CTilePt const &,unsigned short *,unsigned short *,enum Room::Sides *,enum Room::Sides *) 0x628800
#define MSA_RoomManager_ResolveDiagonal 0x228800
static inline bool MS_cDirtTool_HasFlatObjects(void *self, void * a0) { return ((bool (__fastcall *)(void *, void *, void *))MS_EXE(0x2571f0))(self, 0, a0); }   // public: bool __thiscall cDirtTool::HasFlatObjects(struct CTilePt const &) 0x6571f0
#define MSA_cDirtTool_HasFlatObjects 0x2571f0
static inline void MS_cRoofLayer_GenerateRoof(void *self, int a0, void * a1) { ((void (__fastcall *)(void *, void *, int, void *))MS_EXE(0x223dd0))(self, 0, a0, a1); }   // public: void __thiscall cRoofLayer::GenerateRoof(int,class cRoofLayer *) 0x623dd0
#define MSA_cRoofLayer_GenerateRoof 0x223dd0
static inline int MS_CPState_GetLevel(void *self) { return ((int (__fastcall *)(void *, void *))MS_EXE(0x007cb0))(self, 0); }   // public: int __thiscall CPState::GetLevel(void) 0x407cb0
#define MSA_CPState_GetLevel 0x007cb0
static inline int MS_CPState_GetMode(void *self) { return ((int (__fastcall *)(void *, void *))MS_EXE(0x007cd0))(self, 0); }   // public: int __thiscall CPState::GetMode(void) 0x407cd0
#define MSA_CPState_GetMode 0x007cd0
static inline void MS_CPState_SetLevel(void *self, int a0, int a1) { ((void (__fastcall *)(void *, void *, int, int))MS_EXE(0x0082b0))(self, 0, a0, a1); }   // public: void __thiscall CPState::SetLevel(int,enum CPState::Synchron) 0x4082b0
#define MSA_CPState_SetLevel 0x0082b0
static inline void MS_cTSWinBtn_SetState(void *self, long a0) { ((void (__fastcall *)(void *, void *, long))MS_EXE(0x577f60))(self, 0, a0); }   // public: void __thiscall cTSWinBtn::SetState(long) 0x977f60
#define MSA_cTSWinBtn_SetState 0x577f60
static inline void MS_cTSWinBtn_SetEnabled(void *self, bool a0) { ((void (__fastcall *)(void *, void *, bool))MS_EXE(0x082750))(self, 0, a0); }   // public: void __thiscall cTSWinBtn::SetEnabled(bool) 0x482750
#define MSA_cTSWinBtn_SetEnabled 0x082750
static inline void MS_cWinPeople_StopTracking(void *self) { ((void (__fastcall *)(void *, void *))MS_EXE(0x0baa70))(self, 0); }   // public: void __thiscall cWinPeople::StopTracking(void) 0x4baa70
#define MSA_cWinPeople_StopTracking 0x0baa70
static inline bool MS_cSoundPlayer_PlayBySource(void *self, void * a0, short a1) { return ((bool (__fastcall *)(void *, void *, void *, short))MS_EXE(0x127290))(self, 0, a0, a1); }   // public: bool __thiscall cSoundPlayer::PlayBySource(char const *,short) 0x527290
#define MSA_cSoundPlayer_PlayBySource 0x127290
static inline bool MS_cTSWin_TSOnKeyDown(void *self, unsigned long a0, unsigned long a1) { return ((bool (__fastcall *)(void *, void *, unsigned long, unsigned long))MS_EXE(0x56d550))(self, 0, a0, a1); }   // public: virtual bool __thiscall cTSWin::TSOnKeyDown(unsigned long,unsigned long) 0x96d550
#define MSA_cTSWin_TSOnKeyDown 0x56d550
static inline void * MS_TreeSim_GetCurElem(void *self) { return ((void * (__fastcall *)(void *, void *))MS_EXE(0x22dac0))(self, 0); }   // public: struct StackElem * __thiscall TreeSim::GetCurElem(void) 0x62dac0
#define MSA_TreeSim_GetCurElem 0x22dac0
static inline void * MS_ObjSelector_GetMasterSelector(void *self) { return ((void * (__fastcall *)(void *, void *))MS_EXE(0x196510))(self, 0); }   // public: class ObjSelector * __thiscall ObjSelector::GetMasterSelector(void) 0x596510
#define MSA_ObjSelector_GetMasterSelector 0x196510
static inline void * MS_ObjectModule_GetObject(void *self, int a0) { return ((void * (__fastcall *)(void *, void *, int))MS_EXE(0x055b80))(self, 0, a0); }   // public: class cXObject * __thiscall ObjectModule::GetObject(int) 0x455b80
#define MSA_ObjectModule_GetObject 0x055b80
static inline void * MS_operator_new(unsigned int a0) { return ((void * (__cdecl *)(unsigned int))MS_EXE(0x636df5))(a0); }   // C runtime, 0xa36df5
static inline void MS_free(void * a0) { ((void (__cdecl *)(void *))MS_EXE(0x655b60))(a0); }   // C runtime, 0xa55b60

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
#define MSO_cSimsApp_m_pCPanel                           0x90
#define MSZ_cSimsApp_m_pCPanel                           0x4
#define MSO_cWinCPanel_m_pViewControl                    0x130
#define MSZ_cWinCPanel_m_pViewControl                    0x4
#define MSO_cNewWallTool_m_ptDragAnchor                  0x30
#define MSZ_cNewWallTool_m_ptDragAnchor                  0x3
#define MSO_cNewWallTool_m_bMouseUp                      0x70
#define MSZ_cNewWallTool_m_bMouseUp                      0x1
#define MSO_cWinViewControl_m_pCPState                   0x128   // Steam 0x134 (63 instructions agree)
#define MSZ_cWinViewControl_m_pCPState                   0x4
#define MSO_cWinViewControl_m_pFirstStoryBtn             0x160   // Steam 0x16c (11 instructions agree)
#define MSZ_cWinViewControl_m_pFirstStoryBtn             0x4
#define MSO_cWinViewControl_m_pSecondStoryBtn            0x164   // Steam 0x170 (11 instructions agree)
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
#define MSG_g_pWorld                                     0xa07198
#define MSG_g_pAppSettings                               0xa06c24
#define MSG_g_pRoomManager                               0xa1b5bc
#define MSG_g_pLot                                       0xa07194
#define MSG_g_TileRotateLUT                              0xa0c6e8
#define MSG_g_pSimsApp                                   0xa03158
#define MSG_g_aWallMaskIsJunction                        0xa1ca90
#define MSG_g_aWallMaskIsEndpoint                        0xa1cb90
#define MSG_g_nUserActionError                           0xa06c58
#define MSG_g_pWinPeople                                 0xa056e0
#define MSG_XViewer__sTool                               0xa06ce8

// ---- RTTI type descriptors: MSR_<Class> = rva (vtable[-1] -> complete object locator -> +0x0C)
#define MSR_cXObject                                     0x9e2a00   // .?AVcXObject@@
#define MSR_cXMTObject                                   0x9e2a30   // .?AVcXMTObject@@

// ---- detour signatures (16 bytes + the mask of relocated dwords), for SimsLoaderApi::Detour
static const OrigSig MS_SIG_40e610 = { 0x00e8c0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x8b, 0x45, 0x0c, 0x8a, 0xd0, 0x53, 0x8b, 0x5d, 0x08, 0x80 }, 0x0000 };   // EA va 0x40e8c0
static const OrigSig MS_SIG_4f80d0 = { 0x0f8100, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0xe7, 0x92, 0xbe, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x4f8100
static const OrigSig MS_SIG_4f8ee0 = { 0x0f8f10, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x1c, 0x53, 0x8b, 0x5d, 0x08, 0x56, 0x57, 0x8b, 0xf9, 0x8b, 0x43 }, 0x0000 };   // EA va 0x4f8f10
static const OrigSig MS_SIG_56b460 = { 0x16b4b0, 16, { 0x55, 0x8b, 0xec, 0x51, 0x53, 0x8b, 0x5d, 0x08, 0x56, 0x57, 0x8b, 0xf9, 0xc7, 0x03, 0x00, 0x00 }, 0x0000 };   // EA va 0x56b4b0
static const OrigSig MS_SIG_5826b0 = { 0x182720, 16, { 0x55, 0x8b, 0xec, 0x81, 0xec, 0x1c, 0x01, 0x00, 0x00, 0xa1, 0x00, 0x9c, 0xdd, 0x00, 0x33, 0xc5 }, 0x0400 };   // EA va 0x582720
static const OrigSig MS_SIG_583e90 = { 0x183f00, 16, { 0x53, 0x56, 0x8b, 0xf1, 0x57, 0x8b, 0x7e, 0x14, 0x85, 0xff, 0x74, 0x12, 0x8b, 0xcf, 0xe8, 0xbd }, 0x0000 };   // EA va 0x583f00
static const OrigSig MS_SIG_585f10 = { 0x185f80, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x53, 0x0d, 0xbf, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x585f80
static const OrigSig MS_SIG_586b40 = { 0x186bb0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x93, 0x0d, 0xbf, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x586bb0
static const OrigSig MS_SIG_587bb0 = { 0x187c20, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x4f, 0x0f, 0xbf, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x587c20
static const OrigSig MS_SIG_5be5c0 = { 0x1be640, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x74, 0x38, 0xbf, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x5be640
static const OrigSig MS_SIG_5be710 = { 0x1be790, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x53, 0x8b, 0xd9, 0x56, 0x57, 0x89, 0x5d, 0xf4, 0x8b, 0x73 }, 0x0000 };   // EA va 0x5be790
static const OrigSig MS_SIG_5bec90 = { 0x1bed10, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x45, 0x08, 0x85, 0xc0, 0x74, 0x0c, 0xc7, 0x44, 0x81, 0x34, 0x01, 0x00 }, 0x0000 };   // EA va 0x5bed10
static const OrigSig MS_SIG_5becc0 = { 0x1bed40, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x45, 0x08, 0x85, 0xc0, 0x74, 0x0c, 0xc7, 0x44, 0x81, 0x3c, 0x01, 0x00 }, 0x0000 };   // EA va 0x5bed40
static const OrigSig MS_SIG_5becf0 = { 0x1bed70, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x34, 0x39, 0xbf, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x5bed70
static const OrigSig MS_SIG_5beff0 = { 0x1bf070, 16, { 0x55, 0x8b, 0xec, 0x51, 0x53, 0x56, 0x57, 0xc6, 0x45, 0xff, 0x01, 0x8d, 0x79, 0x10, 0x33, 0xdb }, 0x0000 };   // EA va 0x5bf070
static const OrigSig MS_SIG_5bf050 = { 0x1bf0d0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x08, 0x53, 0x8d, 0x41, 0x08, 0xb9, 0x02, 0x00, 0x00, 0x00, 0x56 }, 0x0000 };   // EA va 0x5bf0d0
static const OrigSig MS_SIG_5bf650 = { 0x1bf6d0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x08, 0x53, 0x8d, 0x59, 0x10, 0x89, 0x4d, 0xf8, 0x56, 0xb9, 0x02 }, 0x0000 };   // EA va 0x5bf6d0
static const OrigSig MS_SIG_5bf8a0 = { 0x1bf920, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x55, 0x08, 0x85, 0xd2, 0x53, 0xbb, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x45 }, 0x0000 };   // EA va 0x5bf920
static const OrigSig MS_SIG_5bf900 = { 0x1bf980, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x55, 0x0c, 0x85, 0xd2, 0x56, 0x8b, 0xf1, 0xb9, 0x02, 0x00, 0x00, 0x00 }, 0x0000 };   // EA va 0x5bf980
static const OrigSig MS_SIG_5bf9a0 = { 0x1bfa20, 16, { 0x55, 0x8b, 0xec, 0x51, 0x8b, 0xc1, 0xb9, 0x02, 0x00, 0x00, 0x00, 0x53, 0x8b, 0x5d, 0x08, 0x56 }, 0x0000 };   // EA va 0x5bfa20
static const OrigSig MS_SIG_5bfa80 = { 0x1bfb00, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x55, 0x08, 0x85, 0xd2, 0x57, 0xbf, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x45 }, 0x0000 };   // EA va 0x5bfb00
static const OrigSig MS_SIG_616e30 = { 0x216ee0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xe4, 0xf8, 0x83, 0xec, 0x14, 0x53, 0x56, 0x8b, 0xf1, 0xc7, 0x44, 0x24 }, 0x0000 };   // EA va 0x616ee0
static const OrigSig MS_SIG_6237b0 = { 0x223850, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x50, 0x53, 0x8b, 0xd9, 0x56, 0x57, 0x89, 0x5d, 0xf4, 0x8b, 0x4b }, 0x0000 };   // EA va 0x623850
static const OrigSig MS_SIG_625b20 = { 0x225bc0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0xf1, 0x73, 0xbf, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x625bc0
static const OrigSig MS_SIG_625fc0 = { 0x226060, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x40, 0x74, 0xbf, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 };   // EA va 0x626060
static const OrigSig MS_SIG_627420 = { 0x2274d0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x20, 0x80, 0x3d, 0x14, 0xb6, 0xe1, 0x00, 0x00, 0x53, 0x56, 0x57 }, 0x0100 };   // EA va 0x6274d0
static const OrigSig MS_SIG_6289b0 = { 0x228a60, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x53, 0x56, 0x57, 0x0f, 0xb7, 0x7d, 0x08, 0x8b, 0xf1, 0x8b }, 0x0000 };   // EA va 0x628a60
static const OrigSig MS_SIG_628a20 = { 0x228ad0, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x0c, 0x53, 0x56, 0x57, 0x0f, 0xb7, 0x7d, 0x08, 0x8b, 0xf1, 0x8b }, 0x0000 };   // EA va 0x628ad0
static const OrigSig MS_SIG_628b10 = { 0x228bc0, 16, { 0x8b, 0x41, 0x28, 0x85, 0xc0, 0x74, 0x25, 0x83, 0xe8, 0x01, 0x74, 0x19, 0x83, 0xe8, 0x01, 0x74 }, 0x0000 };   // EA va 0x628bc0
static const OrigSig MS_SIG_62fdd0 = { 0x22fe80, 16, { 0x55, 0x8b, 0xec, 0x83, 0xec, 0x10, 0x83, 0x7d, 0x10, 0x00, 0x57, 0x8b, 0xf9, 0x89, 0x7d, 0xfc }, 0x0000 };   // EA va 0x62fe80
static const OrigSig MS_SIG_6572d0 = { 0x257380, 14, { 0x55, 0x8b, 0xec, 0x64, 0xa1, 0x2c, 0x00, 0x00, 0x00, 0x83, 0xec, 0x24, 0x8b, 0x15 }, 0x0000 };   // EA va 0x657380
static const OrigSig MS_SIG_658eb0 = { 0x258f50, 16, { 0x55, 0x8b, 0xec, 0x8b, 0x45, 0x0c, 0x83, 0xec, 0x20, 0x8b, 0x55, 0x08, 0xc1, 0xf8, 0x04, 0xc1 }, 0x0000 };   // EA va 0x658f50
