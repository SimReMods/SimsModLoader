// Seasons.h - seasons and weather for normal lots (the engine side of "Seasons", the unofficial expansion): the hooks in
// seasons.cpp call these.
//
// Seasons cycle summer -> autumn -> winter -> spring every kSeasonLength sim days (global DaysRunning), or as the
// Seasons Controller object sets them. The terrain swatch follows the season (cTerrain::SetColor): summer / spring keep
// the stock grass, autumn turns the ground under trees red / orange (a leaf map built from tree objects, blended into
// the terrain colour-row index per mesh corner), winter whitens it with the snow that fell. Weather (rain / snow /
// thunderstorm) is drawn as a screen-space particle overlay over the house view (cDDDSimsView::TSPaint), batched into
// one alpha-blended draw; a thunderstorm is heavy rain with squalls, a darker sky, lightning and thunder.
#pragma once
#include "game_api.h"

enum eSeason { kSeasonSummer = 0, kSeasonAutumn = 1, kSeasonWinter = 2, kSeasonSpring = 3, kSeasonCount = 4 };
enum eWeather { kWeatherClear = 0, kWeatherRain = 1, kWeatherSnow = 2, kWeatherStorm = 3 };

// The seasons contract with SimAntics: simulator globals 38..47 (unused by the game, saved with the lot) are shared with
// objects, and trees may write them (the cXObject::InterpValue hook). The same names are in the SimRE project's
// builtins.simc (enum SimGlobal).
//   Season, Weather, WeatherIntensity: what the Seasons Controller object decides, while its heartbeat is fresh;
//     otherwise the mod's own season clock and clear weather, published here so objects can still follow them.
//   SnowCover, LeafCover: the ground the mod simulates (0..100), always published; saved with the lot, they are also
//     where the ground comes back from on a lot load.
//   Winterwear: 1 while there is enough snow on the ground for coats (on at kWinterwearOnCover, off only once it has
//     melted to kWinterwearOffCover); mod-written, saved with the lot.
//   SeasonsLotLoaded: the mod writes 1 whenever it (re)loads a lot's ground; the controller clears it.
//   SnowCoverSeed, LeafCoverSeed: a tree writes cover + 1 (1..101) to start the ground at that cover; the mod takes it
//     on its next update and writes 0 back.
//   SeasonsHeartbeat: the controller writes minute-of-day + 1 (1..1440) on every update; the mod follows the controller
//     only while it is at most kSeasonsHeartbeatMinutes old.
//   Rainwear: 1 while it rains on a lot whose ground follows the seasons and Winterwear is 0 (read-only for trees).
//   RainwearArt: which raincoats are installed: bit 0 men, bit 1 women, bit 2 boys, bit 3 girls (read-only).
enum eSeasonsGlobal
{
    kGlobalSeason = 38, kGlobalWeather = 39, kGlobalWeatherIntensity = 40, kGlobalSnowCover = 41, kGlobalLeafCover = 42,
    kGlobalSeasonsHeartbeat = 43, kGlobalWinterwear = 44, kGlobalSeasonsLotLoaded = 45, kGlobalSnowCoverSeed = 46,
    kGlobalLeafCoverSeed = 47,
    kGlobalSeasonsFirst = 38, kGlobalSeasonsLast = 47,  // trees may write 38..47
    kGlobalRainwear = 48, kGlobalRainwearArt = 49,      // mod-written, read-only
};
static const int kSeasonsHeartbeatMinutes = 10;
static const float kWinterwearOnCover = 0.25f;
static const float kWinterwearOffCover = 0.05f;

static const short kGlobalTreeDressInSpecifiedOutfit = 486;  // Global.iff "Dress in specified outfit"(outfit)
static const short kOutfitRainwear = 61;                // one past the last Maxis outfit (60)
static const short kPersonDataSeasonsRainCoat = 240;    // 10 * coat + colour, 0 = none (kept with the neighbour)
static const short kPersonDataSeasonsUmbrella = 241;    // design 1..kUmbrellaDesigns, 0 = none (kept with the neighbour)
static const int kRainColours = 6;
static const int kUmbrellaDesigns = 13;
static const int kClothingTypeRainwear = 6;             // Hot Date's clothing dialog, temp 0 = clothing type
static const int kClothingTypeUmbrella = 7;
static const int kOutfitUmbrellaPreview = 62;           // the vita button's own mode only, no Sim is put in it

// the season in effect (Seasons Controller or the day counter); the current lot's ground follows the seasons
int Seasons_GetSeason();
bool Seasons_LotFollowsSeasons();
struct SeasonsCalendar { int nDayInSeason, nLen, nSeason, bHoliday, nDaysToHoliday, nDaysToNextSeason, bForced, bController; };
bool Seasons_GetCalendar(SeasonsCalendar *pCal);

// ground
void Seasons_TerrainSetColorPrefix(cTerrain *pTerrain);
bool Seasons_GetPaletteRow(int iColor, int nColors, unsigned long *pLight, unsigned long *pDark);
int Seasons_AdjustColorIndex(signed char nTileX, signed char nTileY, int nIndex, int nColors);
void Seasons_TerrainCornerColors(const float *pPos, const unsigned char *pLightIdx, const unsigned char *pRowIdx,
                                 unsigned int *pColors);
void Seasons_TerrainDetailAlpha(const float *pPos, float *pAlpha);

// weather overlay, drawn into the current pass inside the view rectangle (also advances the season / weather state)
#if GAME_CC
// 2003: drawn in software into the lot view's 16-bit buffer (HouseViewer's view buffer and view rect when it has them,
// else pViewBuf / the given rect). Seasons_PrePaint runs before the scene is drawn and damages the whole view while the
// overlay shows (the 2003 view only redraws damaged rectangles).
void Seasons_PrePaint();
void Seasons_PaintOverlay(cTSBuffer *pViewBuf, int nLeft, int nTop, int nRight, int nBottom);
#else
void Seasons_PaintOverlay(int nLeft, int nTop, int nRight, int nBottom);
#endif

// find_location_for modes 7..10 (snow): true when the mode is one of them (*pnResult set)
bool Seasons_FindGoodLocation(cXObject *pThis, StackElem *pFrame, BehaviorNodeParam *pParams, int *pnResult);

// dressing at the door (cXObject::SetLocation reports a Sim's room change; cXPerson::TryGotoRoutingSlot takes the outfit)
void Seasons_PersonRoomChanged(cXObject *pObj, short nOldRoom, short nNewRoom);
bool Seasons_TakeDoorOutfit(cXObject *pObj, short *pOutfit);

// raincoats (outfit 61) and umbrellas, and their choice dialogs (clothing types 6 / 7)
bool Seasons_GetRainwearSuitTex(cXPerson *pPerson, StringBuffer &suitOut, StringBuffer &texOut);
void Seasons_RainwearDialogBegin(cXPerson *pPreview, cXPerson *pSource);
void Seasons_RainwearDialogStep(cXPerson *pPreview, int nDir);
int Seasons_RainwearDialogCode(cXPerson *pPreview);
void Seasons_UmbrellaDialogBegin(cXPerson *pPreview, cXPerson *pSource);
void Seasons_UmbrellaDialogStep(cXPerson *pPreview, int nDir);
int Seasons_UmbrellaDialogCode(cXPerson *pPreview);

// the season icon in the control panel (SeasonsPanelIcon.cpp)
void Seasons_PanelIconInit(cTSWin *pPanel, cTSWinBtn *pFunds, cTSWinBtn *pFriends);
void Seasons_PanelIconUpdate(cTSWin *pPanel, cTSWinBtn *pFunds, cTSWinBtn *pFriends);

// the mod's log (mod.log)
void Seasons_Log(const char *fmt, ...);
