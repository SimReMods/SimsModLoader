// game_addr.h - every Sims.exe address the seasons mod uses (link-time VAs, base 0x400000; Legacy Collection, Steam).
//
//   GA_FN(name, va)                    a function ("A__B" is A::B, "A___B" is A::~B)
//   GA_DATA(name, va)                  a global
//   GA_CALL(name, va, func, callee)    a `call callee` (E8 rel32) inside func that the mod redirects
//   GA_CODE(name, va, len, func)       len bytes of whole instructions inside func that the mod overwrites with a jmp to
//                                      an asm stub (the instructions are listed in gen_sigs.h)
//   GA_LABEL(name, va, func)           an instruction inside func where a stub resumes
//   GA_CRT(name, va, site)             a CRT function, as called at site
//   GA_RTTI(name, va, mangled)         an RTTI type descriptor
//   GA_VTBL(name, va)                  a vtable; GA_VSLOT(name, vtbl, slot, fn): its entry `slot` is fn
// gen_sigs.h holds the 16-byte signature of every GA_FN / GA_CALL / GA_CODE entry, checked before anything is patched
// (tools/gen_sigs.py checks this table against the exe and writes it).
#pragma once

// ---- functions the mod wraps (detoured at their entry) ----
// the game edition the objects see (global GameEdition)
GA_FN(cSimulator__Init,                             0x556f20)
// seasonal ground and weather
GA_FN(cTerrain__SetColor,                           0x578790)
GA_FN(cColorSwatch__PreCompute,                     0x6657d0)
GA_FN(cColorSwatch__ComputeMeshPointColorIndex,     0x664910)
GA_FN(TerrainTileScratch__ComputeDetailFade,        0x6a4b80)
GA_FN(cDDDSimsView__TSPaint,                        0x40f2d0)
// the seasons globals, snow locations
GA_FN(cXObject__InterpValue,                        0x610d90)
GA_FN(cXObject__TryFindGoodLocation,                0x6179e0)
// dressing: the door, raincoats, umbrellas
GA_FN(cXObject__SetLocation,                        0x53d4d0)
GA_FN(cXPerson__TryChangeSuit,                      0x5f6610)
GA_FN(cXPerson__TryGotoRoutingSlot_param,           0x5f8c10)
GA_FN(SAnimator__UpdateOutfitAndHands,              0x5d7f70)
GA_FN(cWinClothingChoiceDlg__Init,                  0x478100)
GA_FN(cWinVitaBtnSolo__GetCurrentSuitIndex,         0x4fa630)
GA_FN(cWinVitaBtnSolo__PreviousSuit,                0x4fa960)
GA_FN(cWinVitaBtnSolo__NextSuit,                    0x4fa860)
GA_FN(cWinVitaBtnSolo__SetPerson,                   0x4fac90)

// the extra careers, their job panel art and portraits
GA_FN(LoadCareers,                                  0x592770)
GA_FN(SaveCareers,                                  0x593d30)
GA_FN(Neighborhood__LoadPersistentData,             0x54f470)
GA_FN(cWinSubpanelJob__Init,                        0x4dd4b0)
GA_FN(cWinSubpanelJob__Shutdown,                    0x4df0f0)
GA_FN(cWinSubpanelJob__GetJobPopupComposite,        0x4dd3f0)
GA_FN(cWinPeople__UpdateAtWorkStatus,               0x4bc340)
GA_FN(cWinPeople__Shutdown,                         0x4ba530)
// the ExpansionPack8 objects folder
GA_FN(ObjectFolder__Init,                           0x574900)
GA_FN(ObjectFolder__ScanForNewFiles,                0x575e80)
GA_FN(ObjectFolder__NewFileAdded,                   0x575280)
GA_FN(ObjectFolder__GetDownloadDirectories,         0x573080)
GA_FN(ObjectFolder__AddSelector,                    0x571ce0)
// the season icon in the control panel
GA_FN(cWinViewControl__Init,                        0x4f5350)

// ---- functions that contain a patch site ----
GA_FN(TerrainTileScratch__EmitAll,                  0x6a5e30)
GA_FN(Neighbor__GetPersistentDataFields,            0x5ab3b0)    // entry signature-checked by visit_lots: patched inside
GA_FN(cWinViewControl__UpdateViewFromCPState,       0x4f8ee0)    // entry detoured by 4 Stories: patched inside
GA_FN(cXPerson__TryGotoRoutingSlot_slot,            0x5f7950)

// ---- functions the mod calls ----
GA_FN(StringBuffer__StringBuffer,                   0x532cf0)
GA_FN(StringBuffer__append,                         0x532d90)    // (const StringBuffer &, int)
GA_FN(StringBuffer__append_sz,                      0x532df0)    // (const char *, int)
GA_FN(StringBuffer__copy,                           0x5331b0)    // (const StringBuffer &)
GA_FN(StringBuffer__copy_sz,                        0x533230)      // (const char *)
GA_FN(StringBuffer__compare,                        0x533030)
GA_FN(StringBuffer__compareNoCase,                  0x5330b0)    // (const StringBuffer &)
GA_FN(RefHandle_ImageRes___RefHandle,               0x735120)
GA_FN(RefHandle_RenderTarget___RefHandle,           0x735180)
GA_FN(RenderDevice__GetCurrentState,                0x7376b0)
GA_FN(Graphics_GetCurrentTarget,                    0x6d8050)
GA_FN(Texture_LoadWithDecoder,                      0x739b10)
GA_FN(Canvas__AllocVertices_VertPosColorUV,         0x404980)
GA_FN(Canvas__SubmitQuad_3_1,                       0x4044e0)
GA_FN(Canvas__DrawTexturedQuad_2,                   0x409b70)
GA_FN(Canvas__DrawTexturedQuad_3,                   0x404830)
GA_FN(BuildRamp_WideColorChunk,                     0x6640e0)
GA_FN(TextureAtlasSet_Build,                        0x6a2e10)
GA_FN(TerrainTileScratch__EmitQuad,                 0x6a5b80)
GA_FN(GViewer_PointToTile,                          0x5e00e0)
GA_FN(RenderContext__HitTestScreenPoint,            0x6456f0)
GA_FN(cFixedWorld__GetWall,                         0x585a30)
GA_FN(TileWalls__GetWallBetweenDir,                 0x63e0e0)
GA_FN(ObjectIterator__ObjectIterator,               0x63d9b0)    // (House *, const CTilePt *, int)
GA_FN(ObjectIterator__operator_inc,                 0x63da90)
GA_FN(iResFile__Release,                            0x5a3960)
GA_FN(ObjSelector__GetGUID,                         0x5963c0)
GA_FN(ObjSelector__GetShortFilename,                0x596640)
GA_FN(StackElem__GetLocals,                         0x62db70)
GA_FN(cXObject__CanPlace,                           0x53a1b0)    // virtual (called through the vtable)
GA_FN(cXObject__Place,                              0x53c260)    // virtual
GA_FN(cXPerson__ForceLocation,                      0x5f1fd0)    // virtual
GA_FN(cXPerson__GetSAnimator,                       0x5f2790)
GA_FN(cXPerson__GosubObjectTree,                    0x5f2af0)
GA_FN(Room__IsOutside,                              0x6280a0)
GA_FN(RoomManager__GetRoom,                         0x627c70)
GA_FN(VBAnimMgr__FindSuit,                          0x6bbfe0)
GA_FN(VBAnimMgr__FindSkill,                         0x6bbed0)
GA_FN(Skeleton__UpdatePriorities,                   0x6d2d90)
GA_FN(Practice__SetPriority,                        0x6d1d40)
GA_FN(VitaBoy__StartPractice,                       0x6c1770)
GA_FN(DressingRecord__Redress,                      0x57dd50)
GA_FN(SAnimator__Dress,                             0x5ce990)
GA_FN(SAnimator__Undress,                           0x5d6bb0)
GA_FN(EditPerson__EditPerson,                       0x5e0e10)
GA_FN(EditPerson___EditPerson,                      0x5e1280)
GA_FN(EditPerson__Begin,                            0x5e12f0)
GA_FN(EditPerson__End,                              0x5e20d0)
GA_FN(EditPerson__GetSuit,                          0x5e5ec0)
GA_FN(EditPerson__SelectEscapeCharSuit,             0x5e61d0)
GA_FN(EditPerson__SelectEscapeCharTexture,          0x5e6330)
GA_FN(cSimulator__SetGlobal,                        0x557340)
GA_FN(cSimulator__IsPaused,                         0x556ff0)
GA_FN(cSimulator__GetSimRate,                       0x556e70)
GA_FN(cSoundPlayer__PlayBySource,                   0x527250)
GA_FN(cSimsApp__MessageDialog,                      0x4572d0)    // (const char *, const char *, unsigned long, bool)
GA_FN(GetLanguage,                                  0x455820)
GA_FN(GlobalDispatch,                               0x539170)
GA_FN(SimRand,                                      0x61ee40)
// careers
GA_FN(GetCareerByID,                                0x5921f0)
GA_FN(cCareer__cCareer,                             0x591670)
GA_FN(cJob___cJob,                                  0x591e40)
GA_FN(ReconLoadObject_cCareer,                      0x590e10)
GA_FN(ReconStream_InitFlags,                        0x5a3320)    // passed as the resource fixup
GA_FN(StdResFile__StdResFile,                       0x5acda0)
GA_FN(StdResFile___StdResFile,                      0x42ae70)
GA_FN(StdResFile__Open,                             0x5acec0)
GA_FN(StringSet__StringSet,                         0x442b80)
GA_FN(StringSet___StringSet,                        0x40a5c0)
GA_FN(StringSetBase__Load,                          0x58d000)    // (iResFile *, short, bool)
GA_FN(StringSetBase__GetString,                     0x58cd10)
GA_FN(Path_BuildInstallPath,                        0x451810)
GA_FN(Neighborhood__FindNeighborBySelector,         0x54b270)
GA_FN(PersonFinder__GetPersonInHouseIndex,          0x43b3b0)
GA_FN(PersonFinder__AtWork,                         0x438980)
GA_FN(PersonFinder__GetAge,                         0x43a350)
GA_FN(PersonFinder__GetCareer,                      0x43a400)
GA_FN(Rect_FromImageSize,                           0x6d82d0)
GA_FN(Texture_LoadFromFileLocked,                   0x4480c0)
GA_FN(Framework_GetGraphicSystem,                   0x6d7f80)
GA_FN(cTSGraphicSystemW95__GetRenderDevice,         0x6fa8d0)    // virtual (the only graphic system)
GA_FN(RenderTarget_CreateShared,                    0x73b730)
GA_FN(RenderPass_BeginOrDefault,                    0x73b0a0)
GA_FN(RenderPass_EndOrRestoreDefault,               0x73c340)
GA_FN(Vk_GetShaderReadOnlyLayout,                   0x73c6a0)
GA_FN(RenderTarget_TakeTextureRef,                  0x73b2e0)
GA_FN(cTSWinBtn__SetForeground,                     0x6e9a10)
GA_FN(cTSWinBtn__GetFont,                           0x6e8fd0)
// objects folder
GA_FN(ObjectFolder__ScanDirectory,                  0x575e40)
GA_FN(ObjectFolder__AddAllSelectors,                0x5719f0)
GA_FN(ConvertSlashes,                               0x5723d0)
GA_FN(ExtractDirectory,                             0x5a3d40)
GA_FN(CTGFileManager__Get,                          0x509200)
GA_FN(CTGFileManager__IsDirMapped,                  0x5094b0)    // (const char *)
GA_FN(CTGFileManager__AddDirectoryImage,            0x504560)
GA_FN(SetSplashMessage,                             0x469480)
// windows
GA_FN(cTSWin__cTSWin,                               0x6dc420)
GA_FN(cTSWin__GetChildWindowFromID,                 0x6ddc60)
GA_FN(cTSWin__InvalidateSelf,                       0x6de200)    // virtual
GA_FN(cTSWin__SetToolTipsText,                      0x6def20)
GA_FN(cTSWin__TSWinMoveTo,                          0x4bc1e0)    // (const cTSPoint &), out-of-line copy of the inline

// the CRT's dynamic_cast helper, as called by the game's own dynamic_cast<cColorSwatch *> in cTerrain::SetColor
GA_CRT(RTDynamicCast,                               0x7ab967,   0x5787ae)
// the exe's operator new / sized operator delete (the mod's own new / delete go to them: one heap with the game)
GA_CRT(OperatorNew,                                 0x7a78be,   0x5929a4)    // as called in LoadCareers
GA_CRT(OperatorDelete,                              0x7a74ef,   0x5f7b66)    // as called in TryGotoRoutingSlot
GA_RTTI(TD_cXObject,                                0xa5c180,   ".?AVcXObject@@")
GA_RTTI(TD_cXPerson,                                0xa5c198,   ".?AVcXPerson@@")
GA_RTTI(TD_cTerrainRenderer,                        0xa5c65c,   ".?AVcTerrainRenderer@@")
GA_RTTI(TD_cColorSwatch,                            0xa5c69c,   ".?AVcColorSwatch@@")

// virtual calls (cXPerson's vtable, RTTI cXPerson : cXObject : TreeSim; the slots are cXObject's)
GA_VTBL(cXPerson__vftable,                          0x992cfc)
GA_VSLOT(Slot_CanPlace,      cXPerson__vftable, 21, cXObject__CanPlace)
GA_VSLOT(Slot_Place,         cXPerson__vftable, 22, cXObject__Place)
GA_VSLOT(Slot_ForceLocation, cXPerson__vftable, 29, cXPerson__ForceLocation)

// ---- globals ----
GA_DATA(g_pAppSettings,                             0xa67044)
GA_DATA(g_pSimsApp,                                 0xa63570)
GA_DATA(g_pLot,                                     0xa675b4)
GA_DATA(g_pWorld,                                   0xa675b8)
GA_DATA(g_TileRotateLUT,                            0xa6cb08)
GA_DATA(g_DirTileOffsetX,                           0xa50f64)
GA_DATA(g_DirTileOffsetY,                           0xa50f88)
GA_DATA(g_TileNeighborOffsets,                      0xa51138)
GA_DATA(g_nScrollOffsetX,                           0xa67110)
GA_DATA(g_nScrollOffsetY,                           0xa67114)
GA_DATA(g_nViewScale,                               0xa67118)
GA_DATA(g_nViewRotation,                            0xa6711c)
GA_DATA(g_pAnimationManager,                        0xa67534)
GA_DATA(g_nTickCount,                               0xa53240)
GA_DATA(g_SuitNames,                                0xa73604)
GA_DATA(g_SwatchParamsActive,                       0xa51a50)
GA_DATA(g_Careers,                                  0xa72bec)
GA_DATA(g_ImageResDescriptor,                       0xa52d98)
GA_DATA(s_PersistentFields,                         0xa72f68)    // Neighbor::GetPersistentDataFields' list
GA_DATA(s_nPersistentFieldsVersion,                 0xa72f78)

// ---- patch sites ----
// TerrainTileScratch::EmitAll inlines BuildVertexColors; both `call EmitQuad` after it (the first reader of the four
// corner colours) go to the mod, which blends the colours toward snow first
GA_CALL(EA_EmitQuad1,     0x6a6237, TerrainTileScratch__EmitAll, TerrainTileScratch__EmitQuad)
GA_CALL(EA_EmitQuad2,     0x6a6246, TerrainTileScratch__EmitAll, TerrainTileScratch__EmitQuad)
// cXPerson::TryGotoRoutingSlot(StackElem *, RoutingSlot *), state 2: after the ShouldInterrupt test, before the room
// compare (`mov ecx, [ebp-0x170]` = pStackObj); the yield tail writes the route back and returns nResult
GA_CODE(GRS_State2,       0x5f7d96, 6, cXPerson__TryGotoRoutingSlot_slot)
GA_LABEL(GRS_State2Next,  0x5f7d9c, cXPerson__TryGotoRoutingSlot_slot)
GA_LABEL(GRS_Yield,       0x5f8b55, cXPerson__TryGotoRoutingSlot_slot)
// Neighbor::GetPersistentDataFields: the two ends of the path that builds the list (`mov [s_nPersistentFieldsVersion],
// edi`, 6 bytes each; the cached path returns without passing them) call the mod, which appends the seasons fields
GA_CODE(PDF_Built1,       0x5ac133, 6, Neighbor__GetPersistentDataFields)
GA_CODE(PDF_Built2,       0x5ac14d, 6, Neighbor__GetPersistentDataFields)
// cWinViewControl::UpdateViewFromCPState: its two epilogues (`pop edi; pop esi; pop ebx; mov esp, ebp`, 5 bytes, then
// `pop ebp; ret 4`; edi = this) jump to the mod, which updates the season icon and returns for the function
GA_CODE(UV_Tail1,         0x4f97d1, 5, cWinViewControl__UpdateViewFromCPState)
GA_CODE(UV_Tail2,         0x4f97ee, 5, cWinViewControl__UpdateViewFromCPState)
