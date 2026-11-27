// game_addr_cc.h - every Complete Collection (2003, VC6) Sims.exe address the seasons mod uses (link-time VAs, base
// 0x400000, no relocations). Same entry kinds and names as game_addr.h. Entries game_addr.h has that are not here belong
// to the Legacy renderer (Vulkan textures, render targets, the terrain tile mesh, the pick-quad lists); the 2003 code
// draws in software and those parts of the mod are built for the Legacy Collection only. Entries marked "2003" have no
// Legacy counterpart in game_addr.h: functions the Legacy build inlines or does not have, and the 2003 STL / allocator
// helpers the mod must use for memory the game frees. Every call site, code window and label was checked in the 2003
// code, and every wrapped function is reached (called, or in a vtable).
#pragma once

// ---- functions the mod wraps (detoured at their entry) ----
// seasonal ground and weather
GA_FN(cSimulator__Init,                             0x46cfd5)
GA_FN(cTerrain__SetColor,                           0x4c482f)
GA_FN(cColorSwatch__PreCompute,                     0x49ca4a)    // virtual (cColorSwatch vtable slot 3)
GA_FN(cColorSwatch__ComputeMeshPointColorIndex,     0x49ceba)
GA_FN(cDDDSimsView__TSPaint,                        0x4d0e7d)    // virtual
// the seasons globals, snow locations
GA_FN(cXObject__InterpValue,                        0x453181)
GA_FN(cXObject__TryFindGoodLocation,                0x44b408)
// dressing: the door, raincoats, umbrellas
GA_FN(cXObject__SetLocation,                        0x433ef5)
GA_FN(cXPerson__TryChangeSuit,                      0x45dace)
GA_FN(cXPerson__TryGotoRoutingSlot_param,           0x45ad3a)
GA_FN(SAnimator__UpdateOutfitAndHands,              0x58a17c)
GA_FN(cWinClothingChoiceDlg__Init,                  0x50913d)    // virtual
GA_FN(cWinVitaBtnSolo__GetCurrentSuitIndex,         0x56d9a6)
GA_FN(cWinVitaBtnSolo__PreviousSuit,                0x56d8cc)
GA_FN(cWinVitaBtnSolo__NextSuit,                    0x56d82b)
GA_FN(cWinVitaBtnSolo__SetPerson,                   0x56d52d)
GA_FN(Neighbor__GetPersistentDataFields,            0x42385a)

// the extra careers, their job panel art and portraits
GA_FN(LoadCareers,                                  0x403d7c)
GA_FN(SaveCareers,                                  0x404b4f)
GA_FN(Neighborhood__LoadPersistentData,             0x4258ed)
GA_FN(cWinSubpanelJob__Init,                        0x52f3ac)    // virtual
GA_FN(cWinSubpanelJob__Shutdown,                    0x5301a2)    // virtual
GA_FN(cWinSubpanelJob__GetJobPopupComposite,        0x530b47)
GA_FN(cWinPeople__UpdateAtWorkStatus,               0x51eb6f)
GA_FN(cWinPeople__Shutdown,                         0x51af7b)    // virtual
// the ExpansionPack8 objects folder
GA_FN(ObjectFolder__Init,                           0x43af66)
GA_FN(ObjectFolder__ScanForNewFiles,                0x43c6ee)
GA_FN(ObjectFolder__NewFileAdded,                   0x43c422)
GA_FN(ObjectFolder__GetDownloadDirectories,         0x43beaa)
GA_FN(ObjectFolder__AddSelector,                    0x43bb37)
// the season icon in the control panel
GA_FN(cWinViewControl__Init,                        0x5382a4)    // virtual
GA_FN(cWinViewControl__UpdateViewFromCPState,       0x53ad22)

// ---- functions that contain a patch site ----
GA_FN(cXPerson__TryGotoRoutingSlot_slot,            0x45aecf)

// ---- functions the mod calls ----
GA_FN(StringBuffer__StringBuffer,                   0x46f824)
GA_FN(StringBuffer__append,                         0x46f917)    // (const StringBuffer &, int)
GA_FN(StringBuffer__append_sz,                      0x46f847)    // (const char *, int)
GA_FN(StringBuffer__copy,                           0x46f8e4)    // (const StringBuffer &)
GA_FN(StringBuffer__copy_sz,                        0x46f8cb)    // (const char *)
GA_FN(StringBuffer__compare,                        0x46f93d)
GA_FN(StringBuffer__compareNoCase,                  0x46f973)    // (const StringBuffer &)
GA_FN(BuildRamp_WideColorChunk,                     0x49e846)
GA_FN(GViewer_PointToTile,                          0x4a6a9d)
GA_FN(GViewer_GetPixelTopOfTile,                    0x4a6ad9)    // 2003: (CTilePt *) -> world pixel row of the tile top
GA_FN(cFixedWorld__GetWall,                         0x479737)
GA_FN(TileWalls__GetWallBetweenDir,                 0x53c1aa)
GA_FN(ObjectIterator__ObjectIterator,               0x43f371)    // (House *, const CTilePt *, int)
GA_FN(ObjectIterator__operator_inc,                 0x43f3b9)
GA_FN(iResFile__Release,                            0x465a06)
GA_FN(ObjSelector__GetGUID,                         0x45741b)
GA_FN(ObjSelector__GetShortFilename,                0x457f7a)
GA_FN(StackElem__GetLocals,                         0x4752fc)
GA_FN(cXObject__CanPlace,                           0x4333c0)    // virtual (called through the vtable)
GA_FN(cXObject__Place,                              0x4333ef)    // virtual
GA_FN(cXPerson__ForceLocation,                      0x45e685)    // virtual
GA_FN(cXPerson__GetSAnimator,                       0x45964a)    // virtual (cXPerson vtable slot 35)
GA_FN(cXPerson__GosubObjectTree,                    0x45e707)
GA_FN(Room__IsOutside,                              0x468851)
GA_FN(RoomManager__GetRoom,                         0x4670a4)
GA_FN(VBAnimMgr__FindSuit,                          0x5a02d8)
GA_FN(VBAnimMgr__FindSkill,                         0x5a026f)
GA_FN(Skeleton__UpdatePriorities,                   0x599214)
GA_FN(Practice__SetPriority,                        0x598687)
GA_FN(VitaBoy__StartPractice,                       0x5a08e3)
GA_FN(DressingRecord__Redress,                      0x58372c)
GA_FN(SAnimator__Dress,                             0x58d893)
GA_FN(SAnimator__Undress,                           0x58d91d)
GA_FN(EditPerson__EditPerson,                       0x408b9d)
GA_FN(EditPerson___EditPerson,                      0x408c4f)
GA_FN(EditPerson__Begin,                            0x409377)
GA_FN(EditPerson__End,                              0x4094f4)
GA_FN(EditPerson__GetSuit,                          0x4092b7)
GA_FN(EditPerson__SelectEscapeCharSuit,             0x40e630)
GA_FN(EditPerson__SelectEscapeCharTexture,          0x40e6d6)
GA_FN(cSimulator__SetGlobal,                        0x46dae0)
GA_FN(cSimulator__IsPaused,                         0x46d982)
// cSimulator::GetSimRate: not in the 2003 exe (game_api.h computes it from the speed, as the Legacy one does)
GA_FN(cSoundPlayer__PlayBySource,                   0x57a7f7)
GA_FN(cSimsApp__MessageDialog,                      0x4f6994)    // (const char *, const char *, unsigned long, bool)
// GetLanguage: inlined at its callers in the 2003 exe (Globs::m_nCurrentLanguage, see game_api.h)
GA_FN(GlobalDispatch,                               0x40766a)
GA_FN(SimRand,                                      0x46f556)
// careers
GA_FN(GetCareerByID,                                0x404ccb)
GA_FN(cCareer__cCareer,                             0x403586)
GA_FN(cCareer___cCareer,                            0x404c85)    // 2003: the out-of-line ~cCareer TearDownCareers calls
// 2003: ReconLoadObject<cCareer> is inlined into LoadCareers: SimpleReconObject<cCareer>, ReconBuilder::Reconstitute
GA_FN(SimpleReconObject_cCareer__SimpleReconObject, 0x405ee1)    // 2003: (cCareer *, long nType)
GA_FN(ReconBuilder__Reconstitute,                   0x464f5b)    // 2003: (ReconObject *, iResFile *, short, long *)
GA_FN(ReconObject___ReconObject,                    0x465055)    // 2003
GA_FN(ReconStream_InitFlags,                        0x465037)    // passed as the resource fixup
GA_FN(StdResFile__StdResFile,                       0x46f5cd)
GA_FN(StdResFile___StdResFile,                      0x4043b8)    // a jmp to ~SeqResFile
GA_FN(StdResFile__Open,                             0x46f71c)
GA_FN(StringSet__StringSet,                         0x403288)
GA_FN(StringSet___StringSet,                        0x470467)    // ~StringSetBase (StringSet adds no members)
GA_FN(StringSetBase__Load,                          0x470383)    // (iResFile *, short, bool)
GA_FN(StringSetBase__GetString,                     0x4705f2)
GA_FN(Path_BuildInstallPath,                        0x4ee9ce)
GA_FN(Neighborhood__FindNeighborBySelector,         0x4257ba)
GA_FN(PersonFinder__GetPersonInHouseIndex,          0x4e9918)
GA_FN(PersonFinder__AtWork,                         0x4ea8c8)
GA_FN(PersonFinder__GetAge,                         0x4eaa44)
GA_FN(PersonFinder__GetCareer,                      0x4ea83d)
GA_FN(cTSWinBtn__SetForeground,                     0x5e4fb5)    // 2003: (cTSBuffer *, cTSPoint)
GA_FN(cTSWinBtn__GetFont,                           0x5e40a5)
// objects folder
GA_FN(ObjectFolder__ScanDirectory,                  0x43c3ec)
GA_FN(ObjectFolder__AddAllSelectors,                0x43b981)
GA_FN(ConvertSlashes,                               0x43b76d)
GA_FN(ExtractDirectory,                             0x412122)
GA_FN(CTGFileManager__Get,                          0x5b8e25)
GA_FN(CTGFileManager__IsDirMapped,                  0x5b9b60)    // (const char *)
GA_FN(CTGFileManager__AddDirectoryImage,            0x5b92ee)
GA_FN(SetSplashMessage,                             0x4ff6fa)
// windows
GA_FN(cTSWin__cTSWin,                               0x5dbd68)
GA_FN(cTSWin__GetChildWindowFromID,                 0x5dc6f2)
GA_FN(cTSWin__InvalidateSelf,                       0x5de621)    // virtual
GA_FN(cTSWin__SetToolTipsText,                      0x5dd9c8)
GA_FN(cTSWin__TSWinMoveTo,                          0x4cb21d)    // 2003: (cTSPoint) by value
// 2003: the software renderer's buffers and the view's damage list
GA_FN(cTSBuffer__cTSBuffer,                         0x5dfb13)    // 2003
GA_FN(cTSBuffer__AddRef,                            0x5e357a)    // 2003
GA_FN(cTSBuffer__Release,                           0x5e3581)    // 2003 (deletes at 0)
GA_FN(cTSBuffer__MakeNativeColor,                   0x5dfe3b)    // 2003: (r, g, b) -> 16-bit pixel
GA_FN(Texture_LoadFromBmpFile,                      0x4dedc6)    // 2003: (const StringBuffer &, int nFormat), refcount 0
GA_FN(Buffer_Zoom,                                  0x4ba969)    // 2003: (float, cTSBuffer *, const cTSRect *, bool)
GA_FN(DDDDamageList__Add,                           0x5cddcf)    // 2003: (const RECT &)
GA_FN(ZBufferLookup__GetTerrainZ,                   0x4c9f9a)    // 2003: (const CTilePt &, int nScale, bool bClamp)
// 2003: the game heap and its SGI STL containers / strings (memory the game frees)
GA_FN(operator_new,                                 0x634c43)    // 2003
GA_FN(operator_delete,                              0x6321f0)    // 2003
GA_FN(GameString__GameString,                       0x5d4956)    // 2003: std::string(const char *)
GA_FN(GameString___GameString,                      0x5b8770)    // 2003: ~std::string (a jmp to the folded string dtor)
GA_FN(GameString__assign,                           0x5d4bbf)    // 2003: std::string::operator=(const char *)
GA_FN(PersDataVec__push_back,                       0x423fe1)    // 2003: std::vector<PersDataPair>::push_back
GA_FN(CareerVec__push_back,                         0x4055b8)    // 2003: std::vector<cCareer *>::push_back
GA_FN(PathVec__push_back,                           0x4129d9)    // 2003: std::vector<StackString<260> >::push_back

// the CRT's dynamic_cast helper, as called by the game's own dynamic_cast<cColorSwatch *> in cTerrain::SetColor
GA_CRT(RTDynamicCast,                               0x6359f2,   0x4c484e)
GA_RTTI(TD_cXObject,                                0x6c0ee0,   ".?AVcXObject@@")
GA_RTTI(TD_cXPerson,                                0x6c28d0,   ".?AVcXPerson@@")
GA_RTTI(TD_cTerrainRenderer,                        0x6c62a8,   ".?AVcTerrainRenderer@@")
GA_RTTI(TD_cColorSwatch,                            0x6c62e8,   ".?AVcColorSwatch@@")

// virtual calls (cXPerson's vtable, RTTI cXPerson : cXObject : TreeSim; the slots are cXObject's)
GA_VTBL(cXPerson__vftable,                          0x667378)
GA_VSLOT(Slot_CanPlace,      cXPerson__vftable, 21, cXObject__CanPlace)
GA_VSLOT(Slot_Place,         cXPerson__vftable, 22, cXObject__Place)
GA_VSLOT(Slot_ForceLocation, cXPerson__vftable, 29, cXPerson__ForceLocation)

// ---- globals ----
GA_DATA(g_pAppSettings,                             0x6e486c)
GA_DATA(g_pSimsApp,                                 0x6f2a08)
GA_DATA(g_pLot,                                     0x6e54d8)
GA_DATA(g_pWorld,                                   0x6e54dc)
GA_DATA(g_TileRotateLUT,                            0x6de81c)
GA_DATA(g_DirTileOffsetX,                           0x6c2924)
GA_DATA(g_DirTileOffsetY,                           0x6c2948)
GA_DATA(g_TileNeighborOffsets,                      0x6e481c)    // filled at start-up
GA_DATA(g_nScrollOffsetX,                           0x6f0080)
GA_DATA(g_nScrollOffsetY,                           0x6f0084)
GA_DATA(g_nViewScale,                               0x6f0088)
GA_DATA(g_nViewRotation,                            0x6f008c)
GA_DATA(g_pAnimationManager,                        0x6f6b64)
GA_DATA(g_nTickCount,                               0x6e5490)
GA_DATA(g_SuitNames,                                0x6e4850)
GA_DATA(g_Careers,                                  0x6de7f8)
GA_DATA(g_fRenderParam,                             0x6fe838)    // 2003: the damage list's per-rect blit cost
GA_DATA(g_fRenderParamDefault,                      0x6d6180)    // 2003: its normal value

// ---- patch sites ----
// cXPerson::TryGotoRoutingSlot(StackElem *, RoutingSlot *), state 2: after the ShouldInterrupt test, before the room
// compare (`mov eax, [ebp-0x1c]` = pStackObj, `mov ax, [eax+0x84]` its room); the yield tail stores the state variable
// as the frame's yield state, writes the route back and returns nResult
GA_CODE(GRS_State2,       0x45b162, 10, cXPerson__TryGotoRoutingSlot_slot)
GA_LABEL(GRS_State2Next,  0x45b16c, cXPerson__TryGotoRoutingSlot_slot)
GA_LABEL(GRS_Yield,       0x45bb0c, cXPerson__TryGotoRoutingSlot_slot)
