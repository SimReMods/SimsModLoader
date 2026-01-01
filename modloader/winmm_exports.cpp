// generated file - do not edit
// 193 exports; 5 are logging wrappers in winmm.cpp, the rest jump through g_winmm_real[] (filled in winmm.cpp)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" { void *g_winmm_real[193] = { 0 }; }
extern const char *const g_winmm_names[193] = {
    "",
    "mciExecute",
    "CloseDriver",
    "DefDriverProc",
    "DriverCallback",
    "DrvGetModuleHandle",
    "GetDriverModuleHandle",
    "NotifyCallbackData",
    "OpenDriver",
    "PlaySound",
    "PlaySoundA",
    "PlaySoundW",
    "SendDriverMessage",
    "WOW32DriverCallback",
    "WOW32ResolveMultiMediaHandle",
    "WOWAppExit",
    "aux32Message",
    "auxGetDevCapsA",
    "auxGetDevCapsW",
    "auxGetNumDevs",
    "auxGetVolume",
    "auxOutMessage",
    "auxSetVolume",
    "joy32Message",
    "joyConfigChanged",
    "joyGetDevCapsA",
    "joyGetDevCapsW",
    "joyGetNumDevs",
    "joyGetPos",
    "joyGetPosEx",
    "joyGetThreshold",
    "joyReleaseCapture",
    "joySetCapture",
    "joySetThreshold",
    "mci32Message",
    "mciDriverNotify",
    "mciDriverYield",
    "mciFreeCommandResource",
    "mciGetCreatorTask",
    "mciGetDeviceIDA",
    "mciGetDeviceIDFromElementIDA",
    "mciGetDeviceIDFromElementIDW",
    "mciGetDeviceIDW",
    "mciGetDriverData",
    "mciGetErrorStringA",
    "mciGetErrorStringW",
    "mciGetYieldProc",
    "mciLoadCommandResource",
    "mciSendCommandA",
    "mciSendCommandW",
    "mciSendStringA",
    "mciSendStringW",
    "mciSetDriverData",
    "mciSetYieldProc",
    "mid32Message",
    "midiConnect",
    "midiDisconnect",
    "midiInAddBuffer",
    "midiInClose",
    "midiInGetDevCapsA",
    "midiInGetDevCapsW",
    "midiInGetErrorTextA",
    "midiInGetErrorTextW",
    "midiInGetID",
    "midiInGetNumDevs",
    "midiInMessage",
    "midiInOpen",
    "midiInPrepareHeader",
    "midiInReset",
    "midiInStart",
    "midiInStop",
    "midiInUnprepareHeader",
    "midiOutCacheDrumPatches",
    "midiOutCachePatches",
    "midiOutClose",
    "midiOutGetDevCapsA",
    "midiOutGetDevCapsW",
    "midiOutGetErrorTextA",
    "midiOutGetErrorTextW",
    "midiOutGetID",
    "midiOutGetNumDevs",
    "midiOutGetVolume",
    "midiOutLongMsg",
    "midiOutMessage",
    "midiOutOpen",
    "midiOutPrepareHeader",
    "midiOutReset",
    "midiOutSetVolume",
    "midiOutShortMsg",
    "midiOutUnprepareHeader",
    "midiStreamClose",
    "midiStreamOpen",
    "midiStreamOut",
    "midiStreamPause",
    "midiStreamPosition",
    "midiStreamProperty",
    "midiStreamRestart",
    "midiStreamStop",
    "mixerClose",
    "mixerGetControlDetailsA",
    "mixerGetControlDetailsW",
    "mixerGetDevCapsA",
    "mixerGetDevCapsW",
    "mixerGetID",
    "mixerGetLineControlsA",
    "mixerGetLineControlsW",
    "mixerGetLineInfoA",
    "mixerGetLineInfoW",
    "mixerGetNumDevs",
    "mixerMessage",
    "mixerOpen",
    "mixerSetControlDetails",
    "mmDrvInstall",
    "mmGetCurrentTask",
    "mmTaskBlock",
    "mmTaskCreate",
    "mmTaskSignal",
    "mmTaskYield",
    "mmioAdvance",
    "mmioAscend",
    "mmioClose",
    "mmioCreateChunk",
    "mmioDescend",
    "mmioFlush",
    "mmioGetInfo",
    "mmioInstallIOProcA",
    "mmioInstallIOProcW",
    "mmioOpenA",
    "mmioOpenW",
    "mmioRead",
    "mmioRenameA",
    "mmioRenameW",
    "mmioSeek",
    "mmioSendMessage",
    "mmioSetBuffer",
    "mmioSetInfo",
    "mmioStringToFOURCCA",
    "mmioStringToFOURCCW",
    "mmioWrite",
    "mmsystemGetVersion",
    "mod32Message",
    "mxd32Message",
    "sndPlaySoundA",
    "sndPlaySoundW",
    "tid32Message",
    "timeBeginPeriod",
    "timeEndPeriod",
    "timeGetDevCaps",
    "timeGetSystemTime",
    "timeGetTime",
    "timeKillEvent",
    "timeSetEvent",
    "waveInAddBuffer",
    "waveInClose",
    "waveInGetDevCapsA",
    "waveInGetDevCapsW",
    "waveInGetErrorTextA",
    "waveInGetErrorTextW",
    "waveInGetID",
    "waveInGetNumDevs",
    "waveInGetPosition",
    "waveInMessage",
    "waveInOpen",
    "waveInPrepareHeader",
    "waveInReset",
    "waveInStart",
    "waveInStop",
    "waveInUnprepareHeader",
    "waveOutBreakLoop",
    "waveOutClose",
    "waveOutGetDevCapsA",
    "waveOutGetDevCapsW",
    "waveOutGetErrorTextA",
    "waveOutGetErrorTextW",
    "waveOutGetID",
    "waveOutGetNumDevs",
    "waveOutGetPitch",
    "waveOutGetPlaybackRate",
    "waveOutGetPosition",
    "waveOutGetVolume",
    "waveOutMessage",
    "waveOutOpen",
    "waveOutPause",
    "waveOutPrepareHeader",
    "waveOutReset",
    "waveOutRestart",
    "waveOutSetPitch",
    "waveOutSetPlaybackRate",
    "waveOutSetVolume",
    "waveOutUnprepareHeader",
    "waveOutWrite",
    "wid32Message",
    "wod32Message",
};
extern const unsigned short g_winmm_ordinals[193] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194 };
extern const unsigned int g_winmm_count = 193;

extern "C" __declspec(naked) void winmm_thunk_0() { __asm jmp dword ptr [g_winmm_real + 0] }   // #2
extern "C" __declspec(naked) void winmm_thunk_1() { __asm jmp dword ptr [g_winmm_real + 4] }   // mciExecute
extern "C" __declspec(naked) void winmm_thunk_2() { __asm jmp dword ptr [g_winmm_real + 8] }   // CloseDriver
extern "C" __declspec(naked) void winmm_thunk_3() { __asm jmp dword ptr [g_winmm_real + 12] }   // DefDriverProc
extern "C" __declspec(naked) void winmm_thunk_4() { __asm jmp dword ptr [g_winmm_real + 16] }   // DriverCallback
extern "C" __declspec(naked) void winmm_thunk_5() { __asm jmp dword ptr [g_winmm_real + 20] }   // DrvGetModuleHandle
extern "C" __declspec(naked) void winmm_thunk_6() { __asm jmp dword ptr [g_winmm_real + 24] }   // GetDriverModuleHandle
extern "C" __declspec(naked) void winmm_thunk_7() { __asm jmp dword ptr [g_winmm_real + 28] }   // NotifyCallbackData
extern "C" __declspec(naked) void winmm_thunk_8() { __asm jmp dword ptr [g_winmm_real + 32] }   // OpenDriver
extern "C" __declspec(naked) void winmm_thunk_9() { __asm jmp dword ptr [g_winmm_real + 36] }   // PlaySound
extern "C" __declspec(naked) void winmm_thunk_10() { __asm jmp dword ptr [g_winmm_real + 40] }   // PlaySoundA
extern "C" __declspec(naked) void winmm_thunk_11() { __asm jmp dword ptr [g_winmm_real + 44] }   // PlaySoundW
extern "C" __declspec(naked) void winmm_thunk_12() { __asm jmp dword ptr [g_winmm_real + 48] }   // SendDriverMessage
extern "C" __declspec(naked) void winmm_thunk_13() { __asm jmp dword ptr [g_winmm_real + 52] }   // WOW32DriverCallback
extern "C" __declspec(naked) void winmm_thunk_14() { __asm jmp dword ptr [g_winmm_real + 56] }   // WOW32ResolveMultiMediaHandle
extern "C" __declspec(naked) void winmm_thunk_15() { __asm jmp dword ptr [g_winmm_real + 60] }   // WOWAppExit
extern "C" __declspec(naked) void winmm_thunk_16() { __asm jmp dword ptr [g_winmm_real + 64] }   // aux32Message
extern "C" __declspec(naked) void winmm_thunk_17() { __asm jmp dword ptr [g_winmm_real + 68] }   // auxGetDevCapsA
extern "C" __declspec(naked) void winmm_thunk_18() { __asm jmp dword ptr [g_winmm_real + 72] }   // auxGetDevCapsW
extern "C" __declspec(naked) void winmm_thunk_19() { __asm jmp dword ptr [g_winmm_real + 76] }   // auxGetNumDevs
extern "C" __declspec(naked) void winmm_thunk_20() { __asm jmp dword ptr [g_winmm_real + 80] }   // auxGetVolume
extern "C" __declspec(naked) void winmm_thunk_21() { __asm jmp dword ptr [g_winmm_real + 84] }   // auxOutMessage
extern "C" __declspec(naked) void winmm_thunk_22() { __asm jmp dword ptr [g_winmm_real + 88] }   // auxSetVolume
extern "C" __declspec(naked) void winmm_thunk_23() { __asm jmp dword ptr [g_winmm_real + 92] }   // joy32Message
extern "C" __declspec(naked) void winmm_thunk_24() { __asm jmp dword ptr [g_winmm_real + 96] }   // joyConfigChanged
extern "C" __declspec(naked) void winmm_thunk_25() { __asm jmp dword ptr [g_winmm_real + 100] }   // joyGetDevCapsA
extern "C" __declspec(naked) void winmm_thunk_26() { __asm jmp dword ptr [g_winmm_real + 104] }   // joyGetDevCapsW
extern "C" __declspec(naked) void winmm_thunk_27() { __asm jmp dword ptr [g_winmm_real + 108] }   // joyGetNumDevs
extern "C" __declspec(naked) void winmm_thunk_28() { __asm jmp dword ptr [g_winmm_real + 112] }   // joyGetPos
extern "C" __declspec(naked) void winmm_thunk_29() { __asm jmp dword ptr [g_winmm_real + 116] }   // joyGetPosEx
extern "C" __declspec(naked) void winmm_thunk_30() { __asm jmp dword ptr [g_winmm_real + 120] }   // joyGetThreshold
extern "C" __declspec(naked) void winmm_thunk_31() { __asm jmp dword ptr [g_winmm_real + 124] }   // joyReleaseCapture
extern "C" __declspec(naked) void winmm_thunk_32() { __asm jmp dword ptr [g_winmm_real + 128] }   // joySetCapture
extern "C" __declspec(naked) void winmm_thunk_33() { __asm jmp dword ptr [g_winmm_real + 132] }   // joySetThreshold
extern "C" __declspec(naked) void winmm_thunk_34() { __asm jmp dword ptr [g_winmm_real + 136] }   // mci32Message
extern "C" __declspec(naked) void winmm_thunk_35() { __asm jmp dword ptr [g_winmm_real + 140] }   // mciDriverNotify
extern "C" __declspec(naked) void winmm_thunk_36() { __asm jmp dword ptr [g_winmm_real + 144] }   // mciDriverYield
extern "C" __declspec(naked) void winmm_thunk_37() { __asm jmp dword ptr [g_winmm_real + 148] }   // mciFreeCommandResource
extern "C" __declspec(naked) void winmm_thunk_38() { __asm jmp dword ptr [g_winmm_real + 152] }   // mciGetCreatorTask
extern "C" __declspec(naked) void winmm_thunk_39() { __asm jmp dword ptr [g_winmm_real + 156] }   // mciGetDeviceIDA
extern "C" __declspec(naked) void winmm_thunk_40() { __asm jmp dword ptr [g_winmm_real + 160] }   // mciGetDeviceIDFromElementIDA
extern "C" __declspec(naked) void winmm_thunk_41() { __asm jmp dword ptr [g_winmm_real + 164] }   // mciGetDeviceIDFromElementIDW
extern "C" __declspec(naked) void winmm_thunk_42() { __asm jmp dword ptr [g_winmm_real + 168] }   // mciGetDeviceIDW
extern "C" __declspec(naked) void winmm_thunk_43() { __asm jmp dword ptr [g_winmm_real + 172] }   // mciGetDriverData
extern "C" __declspec(naked) void winmm_thunk_44() { __asm jmp dword ptr [g_winmm_real + 176] }   // mciGetErrorStringA
extern "C" __declspec(naked) void winmm_thunk_45() { __asm jmp dword ptr [g_winmm_real + 180] }   // mciGetErrorStringW
extern "C" __declspec(naked) void winmm_thunk_46() { __asm jmp dword ptr [g_winmm_real + 184] }   // mciGetYieldProc
extern "C" __declspec(naked) void winmm_thunk_47() { __asm jmp dword ptr [g_winmm_real + 188] }   // mciLoadCommandResource
extern "C" __declspec(naked) void winmm_thunk_48() { __asm jmp dword ptr [g_winmm_real + 192] }   // mciSendCommandA
extern "C" __declspec(naked) void winmm_thunk_49() { __asm jmp dword ptr [g_winmm_real + 196] }   // mciSendCommandW
extern "C" __declspec(naked) void winmm_thunk_50() { __asm jmp dword ptr [g_winmm_real + 200] }   // mciSendStringA
extern "C" __declspec(naked) void winmm_thunk_51() { __asm jmp dword ptr [g_winmm_real + 204] }   // mciSendStringW
extern "C" __declspec(naked) void winmm_thunk_52() { __asm jmp dword ptr [g_winmm_real + 208] }   // mciSetDriverData
extern "C" __declspec(naked) void winmm_thunk_53() { __asm jmp dword ptr [g_winmm_real + 212] }   // mciSetYieldProc
extern "C" __declspec(naked) void winmm_thunk_54() { __asm jmp dword ptr [g_winmm_real + 216] }   // mid32Message
extern "C" __declspec(naked) void winmm_thunk_55() { __asm jmp dword ptr [g_winmm_real + 220] }   // midiConnect
extern "C" __declspec(naked) void winmm_thunk_56() { __asm jmp dword ptr [g_winmm_real + 224] }   // midiDisconnect
extern "C" __declspec(naked) void winmm_thunk_57() { __asm jmp dword ptr [g_winmm_real + 228] }   // midiInAddBuffer
extern "C" __declspec(naked) void winmm_thunk_58() { __asm jmp dword ptr [g_winmm_real + 232] }   // midiInClose
extern "C" __declspec(naked) void winmm_thunk_59() { __asm jmp dword ptr [g_winmm_real + 236] }   // midiInGetDevCapsA
extern "C" __declspec(naked) void winmm_thunk_60() { __asm jmp dword ptr [g_winmm_real + 240] }   // midiInGetDevCapsW
extern "C" __declspec(naked) void winmm_thunk_61() { __asm jmp dword ptr [g_winmm_real + 244] }   // midiInGetErrorTextA
extern "C" __declspec(naked) void winmm_thunk_62() { __asm jmp dword ptr [g_winmm_real + 248] }   // midiInGetErrorTextW
extern "C" __declspec(naked) void winmm_thunk_63() { __asm jmp dword ptr [g_winmm_real + 252] }   // midiInGetID
extern "C" __declspec(naked) void winmm_thunk_64() { __asm jmp dword ptr [g_winmm_real + 256] }   // midiInGetNumDevs
extern "C" __declspec(naked) void winmm_thunk_65() { __asm jmp dword ptr [g_winmm_real + 260] }   // midiInMessage
extern "C" __declspec(naked) void winmm_thunk_66() { __asm jmp dword ptr [g_winmm_real + 264] }   // midiInOpen
extern "C" __declspec(naked) void winmm_thunk_67() { __asm jmp dword ptr [g_winmm_real + 268] }   // midiInPrepareHeader
extern "C" __declspec(naked) void winmm_thunk_68() { __asm jmp dword ptr [g_winmm_real + 272] }   // midiInReset
extern "C" __declspec(naked) void winmm_thunk_69() { __asm jmp dword ptr [g_winmm_real + 276] }   // midiInStart
extern "C" __declspec(naked) void winmm_thunk_70() { __asm jmp dword ptr [g_winmm_real + 280] }   // midiInStop
extern "C" __declspec(naked) void winmm_thunk_71() { __asm jmp dword ptr [g_winmm_real + 284] }   // midiInUnprepareHeader
extern "C" __declspec(naked) void winmm_thunk_72() { __asm jmp dword ptr [g_winmm_real + 288] }   // midiOutCacheDrumPatches
extern "C" __declspec(naked) void winmm_thunk_73() { __asm jmp dword ptr [g_winmm_real + 292] }   // midiOutCachePatches
extern "C" __declspec(naked) void winmm_thunk_74() { __asm jmp dword ptr [g_winmm_real + 296] }   // midiOutClose
extern "C" __declspec(naked) void winmm_thunk_75() { __asm jmp dword ptr [g_winmm_real + 300] }   // midiOutGetDevCapsA
extern "C" __declspec(naked) void winmm_thunk_76() { __asm jmp dword ptr [g_winmm_real + 304] }   // midiOutGetDevCapsW
extern "C" __declspec(naked) void winmm_thunk_77() { __asm jmp dword ptr [g_winmm_real + 308] }   // midiOutGetErrorTextA
extern "C" __declspec(naked) void winmm_thunk_78() { __asm jmp dword ptr [g_winmm_real + 312] }   // midiOutGetErrorTextW
extern "C" __declspec(naked) void winmm_thunk_79() { __asm jmp dword ptr [g_winmm_real + 316] }   // midiOutGetID
extern "C" __declspec(naked) void winmm_thunk_80() { __asm jmp dword ptr [g_winmm_real + 320] }   // midiOutGetNumDevs
extern "C" __declspec(naked) void winmm_thunk_81() { __asm jmp dword ptr [g_winmm_real + 324] }   // midiOutGetVolume
extern "C" __declspec(naked) void winmm_thunk_82() { __asm jmp dword ptr [g_winmm_real + 328] }   // midiOutLongMsg
extern "C" __declspec(naked) void winmm_thunk_83() { __asm jmp dword ptr [g_winmm_real + 332] }   // midiOutMessage
extern "C" __declspec(naked) void winmm_thunk_84() { __asm jmp dword ptr [g_winmm_real + 336] }   // midiOutOpen
extern "C" __declspec(naked) void winmm_thunk_85() { __asm jmp dword ptr [g_winmm_real + 340] }   // midiOutPrepareHeader
extern "C" __declspec(naked) void winmm_thunk_86() { __asm jmp dword ptr [g_winmm_real + 344] }   // midiOutReset
extern "C" __declspec(naked) void winmm_thunk_87() { __asm jmp dword ptr [g_winmm_real + 348] }   // midiOutSetVolume
extern "C" __declspec(naked) void winmm_thunk_88() { __asm jmp dword ptr [g_winmm_real + 352] }   // midiOutShortMsg
extern "C" __declspec(naked) void winmm_thunk_89() { __asm jmp dword ptr [g_winmm_real + 356] }   // midiOutUnprepareHeader
extern "C" __declspec(naked) void winmm_thunk_90() { __asm jmp dword ptr [g_winmm_real + 360] }   // midiStreamClose
extern "C" __declspec(naked) void winmm_thunk_91() { __asm jmp dword ptr [g_winmm_real + 364] }   // midiStreamOpen
extern "C" __declspec(naked) void winmm_thunk_92() { __asm jmp dword ptr [g_winmm_real + 368] }   // midiStreamOut
extern "C" __declspec(naked) void winmm_thunk_93() { __asm jmp dword ptr [g_winmm_real + 372] }   // midiStreamPause
extern "C" __declspec(naked) void winmm_thunk_94() { __asm jmp dword ptr [g_winmm_real + 376] }   // midiStreamPosition
extern "C" __declspec(naked) void winmm_thunk_95() { __asm jmp dword ptr [g_winmm_real + 380] }   // midiStreamProperty
extern "C" __declspec(naked) void winmm_thunk_96() { __asm jmp dword ptr [g_winmm_real + 384] }   // midiStreamRestart
extern "C" __declspec(naked) void winmm_thunk_97() { __asm jmp dword ptr [g_winmm_real + 388] }   // midiStreamStop
extern "C" __declspec(naked) void winmm_thunk_98() { __asm jmp dword ptr [g_winmm_real + 392] }   // mixerClose
extern "C" __declspec(naked) void winmm_thunk_99() { __asm jmp dword ptr [g_winmm_real + 396] }   // mixerGetControlDetailsA
extern "C" __declspec(naked) void winmm_thunk_100() { __asm jmp dword ptr [g_winmm_real + 400] }   // mixerGetControlDetailsW
extern "C" __declspec(naked) void winmm_thunk_101() { __asm jmp dword ptr [g_winmm_real + 404] }   // mixerGetDevCapsA
extern "C" __declspec(naked) void winmm_thunk_102() { __asm jmp dword ptr [g_winmm_real + 408] }   // mixerGetDevCapsW
extern "C" __declspec(naked) void winmm_thunk_103() { __asm jmp dword ptr [g_winmm_real + 412] }   // mixerGetID
extern "C" __declspec(naked) void winmm_thunk_104() { __asm jmp dword ptr [g_winmm_real + 416] }   // mixerGetLineControlsA
extern "C" __declspec(naked) void winmm_thunk_105() { __asm jmp dword ptr [g_winmm_real + 420] }   // mixerGetLineControlsW
extern "C" __declspec(naked) void winmm_thunk_106() { __asm jmp dword ptr [g_winmm_real + 424] }   // mixerGetLineInfoA
extern "C" __declspec(naked) void winmm_thunk_107() { __asm jmp dword ptr [g_winmm_real + 428] }   // mixerGetLineInfoW
extern "C" __declspec(naked) void winmm_thunk_108() { __asm jmp dword ptr [g_winmm_real + 432] }   // mixerGetNumDevs
extern "C" __declspec(naked) void winmm_thunk_109() { __asm jmp dword ptr [g_winmm_real + 436] }   // mixerMessage
extern "C" __declspec(naked) void winmm_thunk_110() { __asm jmp dword ptr [g_winmm_real + 440] }   // mixerOpen
extern "C" __declspec(naked) void winmm_thunk_111() { __asm jmp dword ptr [g_winmm_real + 444] }   // mixerSetControlDetails
extern "C" __declspec(naked) void winmm_thunk_112() { __asm jmp dword ptr [g_winmm_real + 448] }   // mmDrvInstall
extern "C" __declspec(naked) void winmm_thunk_113() { __asm jmp dword ptr [g_winmm_real + 452] }   // mmGetCurrentTask
extern "C" __declspec(naked) void winmm_thunk_114() { __asm jmp dword ptr [g_winmm_real + 456] }   // mmTaskBlock
extern "C" __declspec(naked) void winmm_thunk_115() { __asm jmp dword ptr [g_winmm_real + 460] }   // mmTaskCreate
extern "C" __declspec(naked) void winmm_thunk_116() { __asm jmp dword ptr [g_winmm_real + 464] }   // mmTaskSignal
extern "C" __declspec(naked) void winmm_thunk_117() { __asm jmp dword ptr [g_winmm_real + 468] }   // mmTaskYield
extern "C" __declspec(naked) void winmm_thunk_118() { __asm jmp dword ptr [g_winmm_real + 472] }   // mmioAdvance
extern "C" __declspec(naked) void winmm_thunk_119() { __asm jmp dword ptr [g_winmm_real + 476] }   // mmioAscend
extern "C" __declspec(naked) void winmm_thunk_120() { __asm jmp dword ptr [g_winmm_real + 480] }   // mmioClose
extern "C" __declspec(naked) void winmm_thunk_121() { __asm jmp dword ptr [g_winmm_real + 484] }   // mmioCreateChunk
extern "C" __declspec(naked) void winmm_thunk_122() { __asm jmp dword ptr [g_winmm_real + 488] }   // mmioDescend
extern "C" __declspec(naked) void winmm_thunk_123() { __asm jmp dword ptr [g_winmm_real + 492] }   // mmioFlush
extern "C" __declspec(naked) void winmm_thunk_124() { __asm jmp dword ptr [g_winmm_real + 496] }   // mmioGetInfo
extern "C" __declspec(naked) void winmm_thunk_125() { __asm jmp dword ptr [g_winmm_real + 500] }   // mmioInstallIOProcA
extern "C" __declspec(naked) void winmm_thunk_126() { __asm jmp dword ptr [g_winmm_real + 504] }   // mmioInstallIOProcW
extern "C" __declspec(naked) void winmm_thunk_127() { __asm jmp dword ptr [g_winmm_real + 508] }   // mmioOpenA
extern "C" __declspec(naked) void winmm_thunk_128() { __asm jmp dword ptr [g_winmm_real + 512] }   // mmioOpenW
extern "C" __declspec(naked) void winmm_thunk_129() { __asm jmp dword ptr [g_winmm_real + 516] }   // mmioRead
extern "C" __declspec(naked) void winmm_thunk_130() { __asm jmp dword ptr [g_winmm_real + 520] }   // mmioRenameA
extern "C" __declspec(naked) void winmm_thunk_131() { __asm jmp dword ptr [g_winmm_real + 524] }   // mmioRenameW
extern "C" __declspec(naked) void winmm_thunk_132() { __asm jmp dword ptr [g_winmm_real + 528] }   // mmioSeek
extern "C" __declspec(naked) void winmm_thunk_133() { __asm jmp dword ptr [g_winmm_real + 532] }   // mmioSendMessage
extern "C" __declspec(naked) void winmm_thunk_134() { __asm jmp dword ptr [g_winmm_real + 536] }   // mmioSetBuffer
extern "C" __declspec(naked) void winmm_thunk_135() { __asm jmp dword ptr [g_winmm_real + 540] }   // mmioSetInfo
extern "C" __declspec(naked) void winmm_thunk_136() { __asm jmp dword ptr [g_winmm_real + 544] }   // mmioStringToFOURCCA
extern "C" __declspec(naked) void winmm_thunk_137() { __asm jmp dword ptr [g_winmm_real + 548] }   // mmioStringToFOURCCW
extern "C" __declspec(naked) void winmm_thunk_138() { __asm jmp dword ptr [g_winmm_real + 552] }   // mmioWrite
extern "C" __declspec(naked) void winmm_thunk_139() { __asm jmp dword ptr [g_winmm_real + 556] }   // mmsystemGetVersion
extern "C" __declspec(naked) void winmm_thunk_140() { __asm jmp dword ptr [g_winmm_real + 560] }   // mod32Message
extern "C" __declspec(naked) void winmm_thunk_141() { __asm jmp dword ptr [g_winmm_real + 564] }   // mxd32Message
extern "C" __declspec(naked) void winmm_thunk_142() { __asm jmp dword ptr [g_winmm_real + 568] }   // sndPlaySoundA
extern "C" __declspec(naked) void winmm_thunk_143() { __asm jmp dword ptr [g_winmm_real + 572] }   // sndPlaySoundW
extern "C" __declspec(naked) void winmm_thunk_144() { __asm jmp dword ptr [g_winmm_real + 576] }   // tid32Message
extern "C" __declspec(naked) void winmm_thunk_147() { __asm jmp dword ptr [g_winmm_real + 588] }   // timeGetDevCaps
extern "C" __declspec(naked) void winmm_thunk_148() { __asm jmp dword ptr [g_winmm_real + 592] }   // timeGetSystemTime
extern "C" __declspec(naked) void winmm_thunk_152() { __asm jmp dword ptr [g_winmm_real + 608] }   // waveInAddBuffer
extern "C" __declspec(naked) void winmm_thunk_153() { __asm jmp dword ptr [g_winmm_real + 612] }   // waveInClose
extern "C" __declspec(naked) void winmm_thunk_154() { __asm jmp dword ptr [g_winmm_real + 616] }   // waveInGetDevCapsA
extern "C" __declspec(naked) void winmm_thunk_155() { __asm jmp dword ptr [g_winmm_real + 620] }   // waveInGetDevCapsW
extern "C" __declspec(naked) void winmm_thunk_156() { __asm jmp dword ptr [g_winmm_real + 624] }   // waveInGetErrorTextA
extern "C" __declspec(naked) void winmm_thunk_157() { __asm jmp dword ptr [g_winmm_real + 628] }   // waveInGetErrorTextW
extern "C" __declspec(naked) void winmm_thunk_158() { __asm jmp dword ptr [g_winmm_real + 632] }   // waveInGetID
extern "C" __declspec(naked) void winmm_thunk_159() { __asm jmp dword ptr [g_winmm_real + 636] }   // waveInGetNumDevs
extern "C" __declspec(naked) void winmm_thunk_160() { __asm jmp dword ptr [g_winmm_real + 640] }   // waveInGetPosition
extern "C" __declspec(naked) void winmm_thunk_161() { __asm jmp dword ptr [g_winmm_real + 644] }   // waveInMessage
extern "C" __declspec(naked) void winmm_thunk_162() { __asm jmp dword ptr [g_winmm_real + 648] }   // waveInOpen
extern "C" __declspec(naked) void winmm_thunk_163() { __asm jmp dword ptr [g_winmm_real + 652] }   // waveInPrepareHeader
extern "C" __declspec(naked) void winmm_thunk_164() { __asm jmp dword ptr [g_winmm_real + 656] }   // waveInReset
extern "C" __declspec(naked) void winmm_thunk_165() { __asm jmp dword ptr [g_winmm_real + 660] }   // waveInStart
extern "C" __declspec(naked) void winmm_thunk_166() { __asm jmp dword ptr [g_winmm_real + 664] }   // waveInStop
extern "C" __declspec(naked) void winmm_thunk_167() { __asm jmp dword ptr [g_winmm_real + 668] }   // waveInUnprepareHeader
extern "C" __declspec(naked) void winmm_thunk_168() { __asm jmp dword ptr [g_winmm_real + 672] }   // waveOutBreakLoop
extern "C" __declspec(naked) void winmm_thunk_169() { __asm jmp dword ptr [g_winmm_real + 676] }   // waveOutClose
extern "C" __declspec(naked) void winmm_thunk_170() { __asm jmp dword ptr [g_winmm_real + 680] }   // waveOutGetDevCapsA
extern "C" __declspec(naked) void winmm_thunk_171() { __asm jmp dword ptr [g_winmm_real + 684] }   // waveOutGetDevCapsW
extern "C" __declspec(naked) void winmm_thunk_172() { __asm jmp dword ptr [g_winmm_real + 688] }   // waveOutGetErrorTextA
extern "C" __declspec(naked) void winmm_thunk_173() { __asm jmp dword ptr [g_winmm_real + 692] }   // waveOutGetErrorTextW
extern "C" __declspec(naked) void winmm_thunk_174() { __asm jmp dword ptr [g_winmm_real + 696] }   // waveOutGetID
extern "C" __declspec(naked) void winmm_thunk_175() { __asm jmp dword ptr [g_winmm_real + 700] }   // waveOutGetNumDevs
extern "C" __declspec(naked) void winmm_thunk_176() { __asm jmp dword ptr [g_winmm_real + 704] }   // waveOutGetPitch
extern "C" __declspec(naked) void winmm_thunk_177() { __asm jmp dword ptr [g_winmm_real + 708] }   // waveOutGetPlaybackRate
extern "C" __declspec(naked) void winmm_thunk_178() { __asm jmp dword ptr [g_winmm_real + 712] }   // waveOutGetPosition
extern "C" __declspec(naked) void winmm_thunk_179() { __asm jmp dword ptr [g_winmm_real + 716] }   // waveOutGetVolume
extern "C" __declspec(naked) void winmm_thunk_180() { __asm jmp dword ptr [g_winmm_real + 720] }   // waveOutMessage
extern "C" __declspec(naked) void winmm_thunk_181() { __asm jmp dword ptr [g_winmm_real + 724] }   // waveOutOpen
extern "C" __declspec(naked) void winmm_thunk_182() { __asm jmp dword ptr [g_winmm_real + 728] }   // waveOutPause
extern "C" __declspec(naked) void winmm_thunk_183() { __asm jmp dword ptr [g_winmm_real + 732] }   // waveOutPrepareHeader
extern "C" __declspec(naked) void winmm_thunk_184() { __asm jmp dword ptr [g_winmm_real + 736] }   // waveOutReset
extern "C" __declspec(naked) void winmm_thunk_185() { __asm jmp dword ptr [g_winmm_real + 740] }   // waveOutRestart
extern "C" __declspec(naked) void winmm_thunk_186() { __asm jmp dword ptr [g_winmm_real + 744] }   // waveOutSetPitch
extern "C" __declspec(naked) void winmm_thunk_187() { __asm jmp dword ptr [g_winmm_real + 748] }   // waveOutSetPlaybackRate
extern "C" __declspec(naked) void winmm_thunk_188() { __asm jmp dword ptr [g_winmm_real + 752] }   // waveOutSetVolume
extern "C" __declspec(naked) void winmm_thunk_189() { __asm jmp dword ptr [g_winmm_real + 756] }   // waveOutUnprepareHeader
extern "C" __declspec(naked) void winmm_thunk_190() { __asm jmp dword ptr [g_winmm_real + 760] }   // waveOutWrite
extern "C" __declspec(naked) void winmm_thunk_191() { __asm jmp dword ptr [g_winmm_real + 764] }   // wid32Message
extern "C" __declspec(naked) void winmm_thunk_192() { __asm jmp dword ptr [g_winmm_real + 768] }   // wod32Message
