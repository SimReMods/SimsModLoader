// SeasonsCore.cpp - the seasons: the season clock, autumn leaves under trees, snow on the ground, the rain / snow /
// thunderstorm overlay, snow locations, dressing at the door, raincoats and umbrellas. The game is reached through
// game_api.h.
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <map>
#include <vector>
#include <algorithm>
#include "Seasons.h"

// ---- game parts only this file uses ----------------------------------------------------------------------------------

namespace ga
{
class cTerrain
{
public:
    union { GA_F(0x18, void *, m_pColorSwatch) };      // a cTerrainRenderer
    void SetColor() { ThisCall<void>(A::cTerrain__SetColor, this); }
};

#if GAME_CC
// the 2003 lot view: drawn in software into its view buffer, redrawing only the damaged rectangles of the view rect
struct DDDDamageList
{
    bool Add(const cTSRect &rc) { return ThisCall<bool>(A::DDDDamageList__Add, this, &rc); }
};
class HouseViewer
{
public:
    union { GA_F(0x28, cTSRect, m_ViewRect) GA_F(0x70, cTSBuffer *, m_pViewBuf) GA_F(0x88, DDDDamageList, m_DamageRects0) };
};
#else
struct PickQuadVert { float m_fX, m_fY, m_fZ, m_Attrib[4], m_Reserved[8]; };
static_assert(sizeof(PickQuadVert) == 60, "PickQuadVert");
struct PickQuadRecord
{
    int m_nObjectId;
    char m_Header[0x1C];
    PickQuadVert m_Verts[6];
};
static_assert(sizeof(PickQuadRecord) == 392, "PickQuadRecord");
struct PickRecord { char m_Data[204]; };
struct PickHitResult { float m_fDepth; PickRecord *m_pRecord; };
struct RenderBuckets                                    // 3084 bytes
{
    std::vector<int> m_Lists[256];                      // BucketArray: 16 x 16 screen cells of 32 pixels
    std::vector<PickQuadRecord> m_Records;
};
static_assert(sizeof(RenderBuckets) == 3084, "RenderBuckets");
struct RenderContext
{
    union
    {
        GA_F(0x0160, RenderBuckets, m_SpriteQuads) GA_F(0x0D6C, RenderBuckets, m_BlendQuads)
        GA_F(0x1978, RenderBuckets, m_PairQuads) GA_F(0x2584, RenderBuckets, m_WallQuads)
        GA_F(0x3DAC, std::vector<PickRecord>, m_PickRecords)
    };
    RenderContext() = delete;
    bool HitTestScreenPoint(int x, int y, PickHitResult *p) { return ThisCall<bool>(A::RenderContext__HitTestScreenPoint, this, x, y, p); }
};
class HouseViewer
{
public:
    RenderContext &Context() { return *(RenderContext *)((char *)this + 0x5C4); }
};
#endif
}

static FTilePt PointToTile(ScreenPt16 pt)
{
    FTilePt out = {};
    Call<FTilePt *>(A::GViewer_PointToTile, &out, &pt, true);
    return out;
}

static TileWalls GetWall(CTilePt pt)
{
    TileWalls w;
    ThisCallRet(&w, A::cFixedWorld__GetWall, World(), &pt);
    return w;
}

// ---- tunables --------------------------------------------------------------------------------------------------------

static const int kSeasonLength = 5;             // sim days per season without a Seasons Controller
static const float kLeafRadius = 2.6f;          // tiles of leaf litter around a tree trunk

// ---- small helpers ---------------------------------------------------------------------------------------------------

static unsigned int Hash32(unsigned int x)
{
    x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x846ca68b;
    x ^= x >> 16;
    return x;
}

static float Hash01(unsigned int x) { return (float)(Hash32(x) & 0xFFFF) / 65535.0f; }

static unsigned long LerpRGB(unsigned long a, unsigned long b, float t)
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    int ar = (a >> 16) & 0xFF, ag = (a >> 8) & 0xFF, ab = a & 0xFF;
    int br = (b >> 16) & 0xFF, bg = (b >> 8) & 0xFF, bb = b & 0xFF;
    int r = ar + (int)((br - ar) * t + 0.5f);
    int g = ag + (int)((bg - ag) * t + 0.5f);
    int bl = ab + (int)((bb - ab) * t + 0.5f);
    return ((unsigned long)r << 16) | ((unsigned long)g << 8) | (unsigned long)bl;
}

// vertex colour of the Canvas pipelines: R in the low byte
static unsigned int VtxColor(int r, int g, int b, float a)
{
    if (a < 0.0f) a = 0.0f;
    if (a > 1.0f) a = 1.0f;
    return (unsigned int)r | ((unsigned int)g << 8) | ((unsigned int)b << 16) | ((unsigned int)(a * 255.0f + 0.5f) << 24);
}

static cSimulator *Sim() { return Settings() ? Settings()->m_pSimulator : 0; }

// lots whose ground follows the season: the ones cTerrain::SetColor gives the plain dirt / grass swatch
static bool IsSeasonalHouse(int nHouse)
{
    if ((nHouse >= 28 && nHouse <= 29) || (nHouse >= 40 && nHouse <= 48))
        return false;                           // beach / vacation island
    if (nHouse >= 90)
        return false;                           // Magic Town
    return true;
}

// the snowy vacation lots: snow terrain all year, no seasons (cTerrain::SetColor gives them MakeSnow)
static bool IsSnowIsland(int nHouse) { return nHouse >= 40 && nHouse <= 42; }

// ---- the seasons contract with objects (Seasons.h, eSeasonsGlobal) ---------------------------------------------------
static short GlobalAt(cSimulator *pSim, int nIndex) { return pSim->m_Globals.a[nIndex]; }
static float Clamp01(float f) { return f < 0.0f ? 0.0f : (f > 1.0f ? 1.0f : f); }

// a Seasons Controller object is on the lot and has written its heartbeat in the last few sim minutes
static bool ControllerActive(cSimulator *pSim)
{
    if (!pSim)
        return false;
    int nBeat = GlobalAt(pSim, kGlobalSeasonsHeartbeat);
    if (nBeat <= 0 || nBeat > 1440)
        return false;
    int nNow = pSim->m_Globals.Hour * 60 + pSim->m_Globals.Minute + 1;
    int nAge = (nNow - nBeat + 1440) % 1440;
    return nAge <= kSeasonsHeartbeatMinutes;
}

int Seasons_GetSeason()
{
    cSimulator *pSim = Sim();
    if (!pSim)
        return kSeasonSummer;
    if (ControllerActive(pSim))
    {
        int nSeason = GlobalAt(pSim, kGlobalSeason);
        return nSeason >= 0 && nSeason < kSeasonCount ? nSeason : kSeasonSummer;
    }
    int nDay = pSim->m_Globals.DaysRunning;
    if (nDay < 1)
        nDay = 1;
    return ((nDay - 1) / kSeasonLength) % kSeasonCount;
}

bool Seasons_LotFollowsSeasons()
{
    cSimulator *pSim = Sim();
    return pSim && pSim->m_Globals.CurrentHouse > 0 && IsSeasonalHouse(pSim->m_Globals.CurrentHouse);
}

// the controller's calendar (attribute numbers of SeasonsController/module.simc `attr` list: 0 c_season_mode,
// 3 c_season_len, 6 c_visit_day, 7 c_visit_base, 8 c_visit_len, 9 c_visit_forced, 31 c_visit_season, 41 c_day_offset,
// 43 c_holiday_force; holidays are days 7 .. 8 of a season)
static const unsigned int kSeasonsControllerGUID = 0x80AD9D75;
static bool CalIsHoliday(int nDay, int nLen, int nForceDay)
{
    int d = (nDay - 1) % nLen + 1;
    return (d >= 7 && d <= 8) || (nForceDay > 0 && nDay == nForceDay);
}

bool Seasons_GetCalendar(SeasonsCalendar *pCal)
{
    cSimulator *pSim = Sim();
    if (!pCal || !pSim)
        return false;
    memset(pCal, 0, sizeof(*pCal));
    cXObject *pCtl = 0;
    if (pSim->m_pObjectModule)
    {
        GVec<cXObject *> &objs = pSim->m_pObjectModule->m_Objects;
        for (size_t i = 0; i < objs.size(); i++)
        {
            cXObject *pObj = objs[i];
            if (!pObj || !pObj->m_pSelector || (unsigned int)pObj->m_pSelector->GetGUID() != kSeasonsControllerGUID)
                continue;
            if (pObj->m_nNumAttributes < 44 || !pObj->m_pAttributes)
                continue;
            if (!pCtl || pObj->m_nObjectId < pCtl->m_nObjectId)      // only the oldest controller works
                pCtl = pObj;
        }
    }
    int nDay = pSim->m_Globals.DaysRunning, nLen = kSeasonLength, nForced = -1, nForceDay = 0;
    if (pCtl)
    {
        const short *a = pCtl->m_pAttributes;
        nDay += a[41];
        nLen = a[3];
        nForceDay = a[43];
        if (a[0] >= 0 && a[0] < 4)
            nForced = a[0];
        else if (a[31] >= 0)                            // a public lot keeps the visitors' calendar
        {
            if (a[8] > 0)
            {
                int nExtra = pSim->m_Globals.DaysRunning - a[7];
                nDay = a[6] + (nExtra > 0 ? nExtra : 0);
                nLen = a[8];
                nForced = a[9];
            }
            else
                nForced = a[31];
        }
        pCal->bController = 1;
    }
    if (nLen < 1)
        nLen = 1;
    if (nDay < 1)
        nDay = 1;
    pCal->nLen = nLen;
    pCal->nDayInSeason = (nDay - 1) % nLen + 1;
    pCal->bForced = nForced >= 0;
    pCal->nSeason = nForced >= 0 ? nForced : ((nDay - 1) / nLen) % kSeasonCount;
    pCal->bHoliday = pCtl && CalIsHoliday(nDay, nLen, nForceDay);
    pCal->nDaysToHoliday = -1;
    if (pCtl)
        for (int i = 0; nDay + i <= nDay - pCal->nDayInSeason + nLen; i++)   // the rest of this season
            if (CalIsHoliday(nDay + i, nLen, nForceDay))
            {
                pCal->nDaysToHoliday = i;
                break;
            }
    pCal->nDaysToNextSeason = nLen - pCal->nDayInSeason + 1;
    return pCtl != 0;
}

// ---- autumn leaf map (world tile coordinates) ------------------------------------------------------------------------

static float g_aLeaf[64][64];                   // 0..1 leaf cover per world tile
static bool g_bLeafMapValid;
static unsigned int g_nLeafSignature;           // tree count / positions the map was built from
static std::map<ObjSelector *, bool> *g_pTreeCache;

static bool IsTreeSelector(ObjSelector *pSel)
{
    if (!g_pTreeCache)
        g_pTreeCache = new std::map<ObjSelector *, bool>;
    std::map<ObjSelector *, bool>::iterator it = g_pTreeCache->find(pSel);
    if (it != g_pTreeCache->end())
        return it->second;
    bool bTree = false;
    StackString<260> sName;
    pSel->GetShortFilename(&sName);
    char szLower[260];
    const char *psz = sName.c_str();
    int i = 0;
    for (; psz && psz[i] && i < 259; i++)
        szLower[i] = (char)tolower((unsigned char)psz[i]);
    szLower[i] = 0;
    // Trees.iff, TreesHD.iff, Trees4.iff, TreesUnleashed.iff, TreeCurb.iff, trees2.iff, ... but not the street lamps
    // (LampsStreet) or the Christmas tree
    if (strstr(szLower, "tree") && !strstr(szLower, "street") && !strstr(szLower, "christmas"))
        bTree = true;
    (*g_pTreeCache)[pSel] = bTree;
    return bTree;
}

template <class F> static void ForEachTree(F fn)
{
    cSimulator *pSim = Sim();
    if (!pSim || !pSim->m_pObjectModule)
        return;
    GVec<cXObject *> &objs = pSim->m_pObjectModule->m_Objects;
    for (size_t i = 0; i < objs.size(); i++)
    {
        cXObject *pObj = objs[i];
        if (!pObj || !pObj->m_pSelector || pObj->m_nLevel != 1)
            continue;
        if (pObj->m_Tile.nX < 0 || pObj->m_Tile.nY < 0)      // in inventory / not placed
            continue;
        if (IsTreeSelector(pObj->m_pSelector))
            fn(pObj->m_Tile.nX, pObj->m_Tile.nY);
    }
}

static unsigned int ComputeTreeSignature()
{
    unsigned int nSig = 0x9E3779B9;
    ForEachTree([&](int x, int y) { nSig = Hash32(nSig ^ (unsigned int)(x * 64 + y + 1)); });
    return nSig;
}

// value noise on a coarse grid (cell = 5 tiles), bilinear: the scattered leaf patches away from trees
static float LeafNoise(int x, int y)
{
    const float kCell = 5.0f;
    float fx = (float)x / kCell, fy = (float)y / kCell;
    int ix = (int)fx, iy = (int)fy;
    float tx = fx - (float)ix, ty = fy - (float)iy;
    tx = tx * tx * (3.0f - 2.0f * tx);
    ty = ty * ty * (3.0f - 2.0f * ty);
    float a = Hash01((unsigned int)(ix * 73856093) ^ (unsigned int)(iy * 19349663) ^ 0x51ED);
    float b = Hash01((unsigned int)((ix + 1) * 73856093) ^ (unsigned int)(iy * 19349663) ^ 0x51ED);
    float c = Hash01((unsigned int)(ix * 73856093) ^ (unsigned int)((iy + 1) * 19349663) ^ 0x51ED);
    float d = Hash01((unsigned int)((ix + 1) * 73856093) ^ (unsigned int)((iy + 1) * 19349663) ^ 0x51ED);
    float ab = a + (b - a) * tx, cd = c + (d - c) * tx;
    float v = ab + (cd - ab) * ty;
    return v * 0.8f + 0.2f * Hash01((unsigned int)(x * 131 + y * 7919));   // a little per-tile grain
}

static void BuildLeafMap()
{
    memset(g_aLeaf, 0, sizeof(g_aLeaf));
    // wind-blown leaves everywhere: soft patches where the noise is high
    for (int x = 0; x < 64; x++)
        for (int y = 0; y < 64; y++)
        {
            float n = LeafNoise(x, y);
            if (n > 0.55f)
            {
                float f = (n - 0.55f) / 0.45f * 1.6f;
                g_aLeaf[x][y] = f > 0.95f ? 0.95f : f;
            }
        }
    int nR = (int)ceilf(kLeafRadius);
    ForEachTree([&](int tx, int ty)
    {
        for (int dx = -nR; dx <= nR; dx++)
            for (int dy = -nR; dy <= nR; dy++)
            {
                int x = tx + dx, y = ty + dy;
                if (x < 0 || y < 0 || x >= 64 || y >= 64)
                    continue;
                // ragged edge: the radius wobbles per tile so the litter is not a perfect disc
                float fR = kLeafRadius * (0.75f + 0.5f * Hash01((unsigned int)(x * 131 + y * 7919 + tx * 17 + ty)));
                float d = sqrtf((float)(dx * dx + dy * dy));
                float f = 1.0f - d / fR;
                if (f <= 0.0f)
                    continue;
                f = f * 1.6f;
                if (g_aLeaf[x][y] < f)
                    g_aLeaf[x][y] = f > 1.0f ? 1.0f : f;
            }
    });
    g_nLeafSignature = ComputeTreeSignature();
    g_bLeafMapValid = true;
}

static float LeafAtWorld(int x, int y)
{
    if (x < 0 || y < 0 || x >= 64 || y >= 64)
        return 0.0f;
    return g_aLeaf[x][y];
}

// ---- seasonal ground state -------------------------------------------------------------------------------------------
//
// Two amounts drive the ground, both advanced in sim minutes (pause stops them, fast speed hurries them):
//   g_fLeaf  0..1  leaf litter + autumn tint; grows in autumn, rots away in the other seasons
//   g_fSnow  0..1  snow cover; grows only while it is snowing, melts outside winter
// The palette is rebuilt (PreCompute) and the terrain redrawn when an amount has moved far enough from what the
// terrain was last built with.

static bool g_bSeasonalPalette;                 // the current lot uses the seasonal layout (rows 0..9 leaves)
static bool g_bDaytime = true;
static float g_fLeaf, g_fSnow;                  // live amounts
static float g_fAppliedLeaf, g_fAppliedSnow;    // what the terrain was last built with
static int g_nLastSimMinute = -1;
static int g_nStateHouse = -1;
static DWORD g_nLastTerrainRefresh;
static bool g_bWinterwear;                      // enough snow on the ground for coats (global Winterwear)
static bool g_bRainwear;                        // raining, no winterwear: raincoat weather (global Rainwear)
static int Seasons_RainwearArt();
static float g_fSnowfall;                       // the snow intensity right now (0 when not snowing)

void Seasons_TerrainSetColorPrefix(cTerrain *pTerrain)
{
    // the original's own tests: only the branch that ends in MakeDirt (a cColorSwatch on any other lot than the sand,
    // snow, leaves, soil and cloud ones) gets the seasonal state; there MakeDirt's PreCompute picks the palette up
    if (!pTerrain->m_pColorSwatch)
        return;
    if (!DynCast(pTerrain->m_pColorSwatch, A::TD_cTerrainRenderer, A::TD_cColorSwatch))
        return;
    cSimulator *pSim = Settings()->m_pSimulator;
    bool bDaytime = pSim->m_Globals.TimeOfDay == 0;
    int nHouse = pSim->m_Globals.CurrentHouse;
    if ((nHouse >= 28 && nHouse <= 29) || (nHouse >= 46 && nHouse <= 48) || (nHouse >= 40 && nHouse <= 42) ||
        nHouse == 95 || nHouse == 96 || (nHouse >= 90 && nHouse <= 94) || nHouse == 99)
        return;
    g_bDaytime = bDaytime;
    g_bSeasonalPalette = IsSeasonalHouse(nHouse);
    if (g_bSeasonalPalette && !g_bLeafMapValid)
        BuildLeafMap();
    g_fAppliedLeaf = g_fLeaf;
    g_fAppliedSnow = g_fSnow;
}

static const int kLeafRowCount = 10;            // rows 0..9 leaves, 10..31 grass

// the seasonal light / dark colour of colour row iColor, without frost (the ground's own colour)
static void SeasonalRowColors(int iColor, int nColors, unsigned long *pLight, unsigned long *pDark)
{
    // stock grass ramp (MakeDirt): brown 0x9D7641 / 0x513C12 -> green 0x50743B / 0x0E3508
    static const unsigned long kBrownL = 0x9D7641, kBrownD = 0x513C12;
    static const unsigned long kGreenL = 0x50743B, kGreenD = 0x0E3508;
    static const unsigned long kOliveL = 0x5C7638, kOliveD = 0x173A0B;   // tired late-summer green
    static const unsigned long kDryL = 0x9A7A44, kDryD = 0x4E3A14;       // autumn dead grass
    // leaf litter: deep red -> orange -> ochre (the last row leans to brown so the rows blend continuously)
    static const unsigned long kLeafL[3] = { 0x9A3418, 0xB45E22, 0xA88038 };
    static const unsigned long kLeafD[3] = { 0x4A1408, 0x5E2C0C, 0x584012 };
    int nLeafRows = kLeafRowCount * nColors / 32;
    unsigned long nL, nD;
    if (iColor < nLeafRows)
    {
        float t = (float)iColor / (float)(nLeafRows - 1);           // 0 = red, 0.5 = orange, 1 = ochre
        float s = t * 2.0f;
        int k = s >= 1.0f ? 1 : 0;
        float f = s - (float)k;
        nL = LerpRGB(kLeafL[k], kLeafL[k + 1], f);
        nD = LerpRGB(kLeafD[k], kLeafD[k + 1], f);
        if (iColor == nLeafRows - 1)
        {
            nL = LerpRGB(nL, kBrownL, 0.5f);
            nD = LerpRGB(nD, kBrownD, 0.5f);
        }
    }
    else
    {
        float t = (float)(iColor - nLeafRows) / (float)(nColors - 1 - nLeafRows);
        float fTint = g_fAppliedLeaf * 0.8f;
        nL = LerpRGB(LerpRGB(kBrownL, kDryL, fTint), LerpRGB(kGreenL, kOliveL, fTint), t);
        nD = LerpRGB(LerpRGB(kBrownD, kDryD, fTint), LerpRGB(kGreenD, kOliveD, fTint), t);
    }
    *pLight = nL;
    *pDark = nD;
}

bool Seasons_GetPaletteRow(int iColor, int nColors, unsigned long *pLight, unsigned long *pDark)
{
    if (!g_bSeasonalPalette || nColors < 32)
        return false;
    unsigned long nL, nD;
    SeasonalRowColors(iColor, nColors, &nL, &nD);
    // The table feeds both the ground vertex colours and the grass blades. The ground gets its snow per corner
    // (Seasons_TerrainCornerColors recomputes the unfrosted colour), so here the table frosts over evenly: the blades
    // turn white instead of poking green / red through the snow.
    unsigned long nSnowL = g_bDaytime ? 0xE6F0FF : 0x828C9B;
    unsigned long nSnowD = g_bDaytime ? 0xA1B4D2 : 0x6E6E82;
    float fFrost = g_fAppliedSnow * 1.2f;
    fFrost = fFrost > 1.0f ? 1.0f : fFrost;
    *pLight = LerpRGB(nL, nSnowL, fFrost);
    *pDark = LerpRGB(nD, nSnowD, fFrost);
    return true;
}

int Seasons_AdjustColorIndex(signed char nTileX, signed char nTileY, int nIndex, int nColors)
{
    if (!g_bSeasonalPalette || nColors < 32 || !Lot())
        return nIndex;
    int nLeafRows = kLeafRowCount * nColors / 32;
    // grass rows are squeezed into the upper part of the table
    int nGrass = nLeafRows + nIndex * (nColors - 1 - nLeafRows) / (nColors - 1);
    if (g_fAppliedLeaf <= 0.01f)
        return nGrass;
    // the mesh corner (x, y) touches the four tiles (x-1..x, y-1..y), as ComputeMeshPointColorIndex samples them
    float fLeaf = 0.0f;
    int nRot = -Lot()->m_nRotation & 3;
    for (int dx = -1; dx <= 0; dx++)
        for (int dy = -1; dy <= 0; dy++)
        {
            int x = nTileX + dx, y = nTileY + dy;
            if (x < 0 || y < 0 || x >= 64 || y >= 64)
                continue;
            int wx = x, wy = y;
            if (nRot)
            {
                const TileRotateEntry &e = TileRotateLUT()[nRot - 1][x][y];
                wx = e.x;
                wy = e.y;
            }
            fLeaf += LeafAtWorld(wx, wy);
        }
    fLeaf *= 0.25f;
    // the litter builds up from the densest spots outwards
    float f = fLeaf * 1.3f - (1.0f - g_fAppliedLeaf) * 1.3f;
    if (f <= 0.01f)
        return nGrass;
    if (f > 1.0f)
        f = 1.0f;
    // which leaf colour: a per-corner hash (red .. ochre); corners share it across tiles, so it stays seamless
    unsigned int nKey = (unsigned int)((unsigned char)nTileX * 64 + (unsigned char)nTileY);
    if (nRot)
    {
        int cx = nTileX & 63, cy = nTileY & 63;
        const TileRotateEntry &e = TileRotateLUT()[nRot - 1][cx][cy];
        nKey = (unsigned int)(e.x * 64 + e.y);
    }
    int nLeafIdx = (int)(Hash01(nKey * 2654435761u) * (float)(nLeafRows - 2));
    int n = (int)((float)nGrass + ((float)nLeafIdx - (float)nGrass) * f + 0.5f);
    if (n < 0) n = 0;
    if (n >= nColors) n = nColors - 1;
    return n;
}

// ---- per-corner snow cover (terrain vertex colours) ------------------------------------------------------------------
//
// The terrain base pass is plain vertex colour, so snow is blended into each tile corner's colour right where the
// engine computes it. Cover varies per corner: a coarse noise decides where the first patches settle (drifts first,
// sheltered spots last), and corners under an upper-storey floor stay bare.

static float SnowNoise(int x, int y)
{
    const float kCell = 3.0f;
    float fx = (float)(x + 64) / kCell, fy = (float)(y + 64) / kCell;
    int ix = (int)fx, iy = (int)fy;
    float tx = fx - (float)ix, ty = fy - (float)iy;
    tx = tx * tx * (3.0f - 2.0f * tx);
    ty = ty * ty * (3.0f - 2.0f * ty);
    float a = Hash01((unsigned int)(ix * 83492791) ^ (unsigned int)(iy * 2971215073u) ^ 0x5A0F);
    float b = Hash01((unsigned int)((ix + 1) * 83492791) ^ (unsigned int)(iy * 2971215073u) ^ 0x5A0F);
    float c = Hash01((unsigned int)(ix * 83492791) ^ (unsigned int)((iy + 1) * 2971215073u) ^ 0x5A0F);
    float d = Hash01((unsigned int)((ix + 1) * 83492791) ^ (unsigned int)((iy + 1) * 2971215073u) ^ 0x5A0F);
    float ab = a + (b - a) * tx, cd = c + (d - c) * tx;
    float v = ab + (cd - ab) * ty;
    return v * 0.75f + 0.25f * Hash01((unsigned int)((x + 64) * 7907 + (y + 64) * 104723));
}

// view-rotated tile -> world tile (the 3D terrain is built in view-rotated tile space)
static bool ViewTileToWorld(int x, int y, int *pX, int *pY)
{
    if (x < 0 || y < 0 || x >= 64 || y >= 64)
        return false;
    int nRot = -Lot()->m_nRotation & 3;
    if (nRot)
    {
        const TileRotateEntry &e = TileRotateLUT()[nRot - 1][x][y];
        x = e.x;
        y = e.y;
    }
    *pX = x;
    *pY = y;
    return true;
}

// fraction of the four tiles around view corner (cx, cy) that have an upper-storey floor over them
static float SnowShelter(int cx, int cy)
{
    if (!Lot() || !Lot()->Floors(1))
        return 0.0f;
    int nCovered = 0;
    for (int dx = -1; dx <= 0; dx++)
        for (int dy = -1; dy <= 0; dy++)
        {
            int x, y;
            if (!ViewTileToWorld(cx + dx, cy + dy, &x, &y))
                continue;
            if ((*Lot()->Floors(1))[x][y])
                nCovered++;
        }
    return (float)nCovered * 0.25f;
}

// snow cover 0..1 at a terrain corner given by its 3D position (1 tile = 3 units)
static float SnowCoverAt(const float *pPos)
{
    if (!g_bSeasonalPalette || g_fAppliedSnow <= 0.0f || !Lot())
        return 0.0f;
    // view-rotated tile space: tile x from pos[0], tile y from pos[2]
    int cx = (int)floorf(pPos[0] / 3.0f + 0.5f);
    int cy = (int)floorf(pPos[2] / 3.0f + 0.5f);
    int wx, wy;                                 // noise keyed by the world tile, so it does not move when rotating
    if (!ViewTileToWorld(cx, cy, &wx, &wy))
    {
        wx = cx;
        wy = cy;
    }
    float n = SnowNoise(wx, wy);
    // a wide, soft band: the whole lawn whitens together, drifts (low noise) a little ahead of the rest
    float t = n * 0.45f;
    float c = (g_fAppliedSnow - t) / 0.55f;
    c = c < 0.0f ? 0.0f : (c > 1.0f ? 1.0f : c);
    c = c * c * (3.0f - 2.0f * c);
    return c * (1.0f - SnowShelter(cx, cy));
}

void Seasons_TerrainCornerColors(const float *pPos, const unsigned char *pLightIdx, const unsigned char *pRowIdx,
                                 unsigned int *pColors)
{
    if (!g_bSeasonalPalette || g_fAppliedSnow <= 0.0f)
        return;
    unsigned long nSnowL = g_bDaytime ? 0xE6F0FF : 0x828C9B;
    unsigned long nSnowD = g_bDaytime ? 0xA1B4D2 : 0x6E6E82;
    for (int i = 0; i < 4; i++)
    {
        // the engine's lighting ramp runs light -> dark over 128 shades; the vertex colour reads the entry 8 shades back
        float t = ((float)pLightIdx[i] - 8.0f) / 128.0f;
        t = t < 0.0f ? 0.0f : t;
        unsigned long nL, nD;
        SeasonalRowColors(pRowIdx[i] < 32 ? pRowIdx[i] : 31, 32, &nL, &nD);
        unsigned long nGround = LerpRGB(nL, nD, t);
        unsigned long nSnow = LerpRGB(nSnowL, nSnowD, t);
        unsigned long nRgb = LerpRGB(nGround, nSnow, SnowCoverAt(pPos + 3 * i));
        pColors[i] = (pColors[i] & 0xFF000000) | ((nRgb >> 16) & 0xFF) | (nRgb & 0xFF00) | ((nRgb & 0xFF) << 16);
    }
}

void Seasons_TerrainDetailAlpha(const float *pPos, float *pAlpha)
{
    if (!g_bSeasonalPalette || g_fAppliedSnow <= 0.0f)
        return;
    for (int i = 0; i < 4; i++)
    {
        float c = SnowCoverAt(pPos + 3 * i);
        pAlpha[i] *= 1.0f - c;
    }
}

static void RefreshTerrain()
{
    if (Settings() && Settings()->m_pTerrain)
    {
        Settings()->m_pTerrain->SetColor();
        GlobalDispatch(131, 0);                 // HouseViewer::DirtyAll: redraw every terrain tile
        g_nLastTerrainRefresh = GetTickCount();
    }
}

static float Approach(float v, float target, float step)
{
    if (v < target)
        return v + step > target ? target : v + step;
    return v - step < target ? target : v - step;
}

// advances the leaf / snow amounts by the sim time since the last frame and redraws the ground when they moved
static void UpdateGroundState()
{
    cSimulator *pSim = Sim();
    if (!pSim)
        return;
    int nHouse = pSim->m_Globals.CurrentHouse;
    int nSeason = IsSeasonalHouse(nHouse) ? Seasons_GetSeason() : kSeasonSummer;
    int nNow = pSim->m_Globals.DaysRunning * 1440 + pSim->m_Globals.Hour * 60 + pSim->m_Globals.Minute;
    int nDelta = nNow - g_nLastSimMinute;
    if (g_nLastSimMinute < 0 || nHouse != g_nStateHouse || nDelta < 0 || nDelta > 6 * 60)
    {
        // a lot load (also a reload of the same lot, which moves the clock back) or a time jump: the ground as it
        // was saved with the lot (globals SnowCover / LeafCover / Winterwear, published on every update below)
        bool bSeasonal = IsSeasonalHouse(nHouse);
        g_fSnow = bSeasonal ? Clamp01(GlobalAt(pSim, kGlobalSnowCover) / 100.0f) : 0.0f;
        g_fLeaf = bSeasonal ? Clamp01(GlobalAt(pSim, kGlobalLeafCover) / 100.0f) : 0.0f;
        g_bWinterwear = bSeasonal && GlobalAt(pSim, kGlobalWinterwear) == 1;
        pSim->SetGlobal(kGlobalSeasonsLotLoaded, 1);   // the controller looks for the arriving Sims' season
        g_nStateHouse = nHouse;
        g_nLastSimMinute = nNow;
        if (fabsf(g_fLeaf - g_fAppliedLeaf) > 0.001f || fabsf(g_fSnow - g_fAppliedSnow) > 0.001f)
            RefreshTerrain();
        return;
    }
    if (!IsSeasonalHouse(nHouse))
    {
        // no seasonal ground (vacation islands, Magic Town): nothing settles, nothing to dress for
        g_fSnow = g_fLeaf = 0.0f;
        g_bWinterwear = false;
        g_nLastSimMinute = nNow;
        return;
    }
    // a start value from the controller (a public lot takes the ground of the visitors' home)
    short nSnowSeed = GlobalAt(pSim, kGlobalSnowCoverSeed);
    short nLeafSeed = GlobalAt(pSim, kGlobalLeafCoverSeed);
    if (nSnowSeed > 0 || nLeafSeed > 0)
    {
        if (nSnowSeed > 0)
        {
            g_fSnow = Clamp01((nSnowSeed - 1) / 100.0f);
            g_bWinterwear = g_fSnow >= kWinterwearOnCover;
            pSim->SetGlobal(kGlobalSnowCoverSeed, 0);
        }
        if (nLeafSeed > 0)
        {
            g_fLeaf = Clamp01((nLeafSeed - 1) / 100.0f);
            pSim->SetGlobal(kGlobalLeafCoverSeed, 0);
        }
        RefreshTerrain();
    }
    float fMin = (float)nDelta;
    if (fMin <= 0.0f)
        return;
    g_nLastSimMinute = nNow;
    // leaves: fall over ~5 sim hours in autumn, stay through winter (under the snow), rot away over ~8 hours once the
    // ground is clear of snow
    if (nSeason == kSeasonAutumn)
        g_fLeaf = Approach(g_fLeaf, 1.0f, fMin / 300.0f);
    else if (nSeason != kSeasonWinter && g_fSnow < 0.05f)
        g_fLeaf = Approach(g_fLeaf, 0.0f, fMin / 480.0f);
    // snow: settles only while it snows (~3 sim hours to full cover in a heavy fall); stays in winter, melts outside it
    if (g_fSnowfall > 0.0f)
        g_fSnow = Approach(g_fSnow, 1.0f, fMin / 180.0f * g_fSnowfall);
    else if (nSeason != kSeasonWinter)
        g_fSnow = Approach(g_fSnow, 0.0f, fMin / (nSeason == kSeasonSummer ? 720.0f : 1440.0f));
    // coats: on once the snow has settled for a while, off only when it has nearly melted
    if (g_fSnow >= kWinterwearOnCover)
        g_bWinterwear = true;
    else if (g_fSnow <= kWinterwearOffCover)
        g_bWinterwear = false;
    // redraw when the ground moved a visible step (and not more than every 0.7 s of real time)
    bool bMoved = fabsf(g_fLeaf - g_fAppliedLeaf) >= 0.02f || fabsf(g_fSnow - g_fAppliedSnow) >= 0.012f ||
                  ((g_fLeaf == 0.0f || g_fLeaf == 1.0f) && g_fLeaf != g_fAppliedLeaf) ||
                  ((g_fSnow == 0.0f || g_fSnow == 1.0f) && g_fSnow != g_fAppliedSnow);
    if (bMoved && GetTickCount() - g_nLastTerrainRefresh > 700)
        RefreshTerrain();
}

// ---- weather ---------------------------------------------------------------------------------------------------------

struct Particle
{
    float x, y;             // screen position (view-relative)
    float vx, vy;           // pixels per second
    float fSize;            // flake diameter / drop length
    float fAlpha;
    float fPhase;           // snow sway phase
    float fGroundY;         // rain: where the drop splashes
    float fFollow;          // snow: how much of a scroll it follows (parallax)
};

struct Splash
{
    float x, y, t;          // t: seconds left
};

static const int kMaxParticles = 4000;
static const int kMaxSplashes = 600;
static Particle g_aParticles[kMaxParticles];
static int g_nParticles;
static Splash g_aSplashes[kMaxSplashes];
static int g_nSplashes;
static int g_nParticleKind = kWeatherClear;     // what g_aParticles currently holds
static float g_fIntensity;                      // eased 0..1
static unsigned int g_nRand = 12345;
static DWORD g_nLastTick;
static int g_nViewW, g_nViewH;
static float g_fPxScale = 1.0f;                 // particle size scale: view height / 900 (high-res windows)
#if !GAME_CC
static cTSBufferRef *g_phDot;                   // 32x32 white soft dot (alpha only), kept for the session
static bool g_bDotTried;
#endif
static float g_fStorm;                          // eased 0..1: how much of a thunderstorm the rain is
static float g_fStormPower;                     // 0..1: the storm's intensity (how often lightning strikes)

static float Rand01()
{
    g_nRand = g_nRand * 1664525u + 1013904223u;
    return (float)(g_nRand >> 8) / 16777216.0f;
}

// the weather without a Seasons Controller heartbeat: always clear (the weather plan lives in the controller object)
static void AutoWeather(int *pKind, float *pIntensity)
{
    *pKind = kWeatherClear;
    *pIntensity = 0.0f;
}

static float g_fAnimTime;                       // sim-scaled seconds (snow sway, rain gusts)

// the rain's slant (sideways speed per fall speed): 0.16 up to intensity 50, rising smoothly to 0.38 at 100
static float RainWind()
{
    float t = (g_fIntensity - 0.5f) / 0.5f;
    t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
    t = t * t * (3.0f - 2.0f * t);
    float fWind = (0.16f + 0.22f * t) * (1.0f + 0.10f * t * sinf(g_fAnimTime * 0.7f));
    // a thunderstorm: stronger wind (up to ~30 degrees) in squalls - two slow waves, so the gusts are irregular
    float fGust = 0.5f + 0.3f * sinf(g_fAnimTime * 0.9f) + 0.2f * sinf(g_fAnimTime * 2.3f + 1.0f);
    return fWind + g_fStorm * (0.12f + 0.16f * fGust);
}

// ---- lightning (thunderstorms) ---------------------------------------------------------------------------------------
// A strike is a flash of the whole view (two or three flickers), sometimes with a visible bolt from the top of the view,
// and a thunderclap after a delay. The clap is Makin' Magic's "spellcasting_lightning" sound event when that pack is
// installed; there is no other thunder in the game.

static const int kBoltMaxPts = 64;
static float g_fNextStrike = 4.0f;              // sim-scaled seconds to the next strike
static float g_fFlashAge = -1.0f;               // real seconds since the strike (< 0: none)
static float g_fFlashPeak;                      // 0..1
static float g_afFlicker[3][2];                 // the flickers of a strike: start (s), amplitude
static float g_fThunderIn = -1.0f;              // real seconds to the thunderclap (< 0: none pending)
static float g_afBoltX[kBoltMaxPts], g_afBoltY[kBoltMaxPts];
static int g_nBoltPts;                          // 0: no bolt (sheet lightning in the clouds)
static int g_nBoltBranchAt;                     // points before this index are the main channel, after it the branch

// midpoint displacement between two points (inclusive), 2^nDepth segments
static void BoltSegment(float x0, float y0, float x1, float y1, int nDepth, float fJag)
{
    if (nDepth == 0 || g_nBoltPts >= kBoltMaxPts - 1)
    {
        g_afBoltX[g_nBoltPts] = x1;
        g_afBoltY[g_nBoltPts] = y1;
        g_nBoltPts++;
        return;
    }
    float fLen = sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    float mx = (x0 + x1) * 0.5f + (Rand01() - 0.5f) * fLen * fJag;
    float my = (y0 + y1) * 0.5f + (Rand01() - 0.5f) * fLen * fJag * 0.3f;
    BoltSegment(x0, y0, mx, my, nDepth - 1, fJag);
    BoltSegment(mx, my, x1, y1, nDepth - 1, fJag);
}

static void BuildBolt()
{
    float fW = (float)g_nViewW, fH = (float)g_nViewH;
    float x0 = fW * (0.1f + 0.8f * Rand01()), y0 = -10.0f;
    float x1 = x0 + fW * (Rand01() - 0.5f) * 0.3f, y1 = fH * (0.35f + 0.45f * Rand01());
    g_nBoltPts = 0;
    g_afBoltX[g_nBoltPts] = x0;
    g_afBoltY[g_nBoltPts] = y0;
    g_nBoltPts++;
    BoltSegment(x0, y0, x1, y1, 5, 0.55f);      // 33 points
    g_nBoltBranchAt = g_nBoltPts;
    // one branch from 30..60% down the channel, sideways and down, a third to a half as long
    int nFrom = 10 + (int)(Rand01() * 10.0f);
    float bx = g_afBoltX[nFrom], by = g_afBoltY[nFrom];
    float fSide = Rand01() < 0.5f ? -1.0f : 1.0f;
    float fLen = (y1 - y0) * (0.3f + 0.2f * Rand01());
    g_afBoltX[g_nBoltPts] = bx;
    g_afBoltY[g_nBoltPts] = by;
    g_nBoltPts++;
    BoltSegment(bx, by, bx + fSide * fLen * 0.7f, by + fLen * 0.8f, 4, 0.6f);
}

static void PlayThunder()
{
    if (Settings() && Settings()->m_pSoundSys && (Settings()->m_nExpansionMask & kEP_MakinMagic))
        Settings()->m_pSoundSys->PlayBySource("spellcasting_lightning", -4);
}

// advances the strike timer (dt: sim-scaled, 0 while paused) and the flash / thunder (dtReal)
static void UpdateLightning(float dt, float dtReal)
{
    if (g_fFlashAge >= 0.0f)
    {
        g_fFlashAge += dtReal;
        if (g_fFlashAge > 1.0f)
        {
            g_fFlashAge = -1.0f;
            g_nBoltPts = 0;
        }
    }
    if (g_fThunderIn >= 0.0f)
    {
        g_fThunderIn -= dtReal;
        if (g_fThunderIn < 0.0f)
            PlayThunder();
    }
    if (g_fStorm < 0.5f)
        return;
    g_fNextStrike -= dt;
    if (g_fNextStrike > 0.0f)
        return;
    // the next one: 5..18 s apart in a mild storm, 2.5..9 s in a violent one (sim-scaled: fast speed hurries it)
    g_fNextStrike = (5.0f + 13.0f * Rand01()) * (1.0f - 0.5f * g_fStormPower);
    bool bBolt = Rand01() < 0.45f;
    g_fFlashAge = 0.0f;
    g_fFlashPeak = bBolt ? 1.0f : 0.45f + 0.4f * Rand01();
    int nFlickers = Rand01() < 0.6f ? 3 : 2;
    float fStart = 0.0f;
    for (int i = 0; i < 3; i++)
    {
        g_afFlicker[i][0] = fStart;
        g_afFlicker[i][1] = i < nFlickers ? (i == 0 ? 1.0f : 0.5f + 0.5f * Rand01()) : 0.0f;
        fStart += 0.07f + 0.12f * Rand01();
    }
    g_nBoltPts = 0;
    if (bBolt)
        BuildBolt();
    // sound travels ~1 km in 3 s: a visible bolt is close (0.1..0.6 s), sheet lightning further (0.8..3 s)
    g_fThunderIn = bBolt ? 0.1f + 0.5f * Rand01() : 0.8f + 2.2f * Rand01();
}

// 0..1: the flash brightness right now (the flickers decay fast, ~55 ms each)
static float LightningFlash()
{
    if (g_fFlashAge < 0.0f)
        return 0.0f;
    float f = 0.0f;
    for (int i = 0; i < 3; i++)
    {
        float t = g_fFlashAge - g_afFlicker[i][0];
        if (t >= 0.0f)
            f += g_afFlicker[i][1] * expf(-t * 18.0f);
    }
    f *= g_fFlashPeak;
    return f > 1.0f ? 1.0f : f;
}

// how far right of the view the rain starts: slanted drops drift left while they fall the view height
static float RainSpawnWidth()
{
    return (float)g_nViewW + 120.0f + (float)g_nViewH * RainWind();
}

static void SpawnParticle(Particle &p, int nKind, bool bAnywhere)
{
    float fDepth = Rand01();                    // 0 = far (small, slow, faint), 1 = near
    if (nKind == kWeatherRain)
    {
        p.fSize = (10.0f + 16.0f * fDepth) * g_fPxScale;
        p.vy = (650.0f + 550.0f * fDepth) * g_fPxScale;
        p.vx = -RainWind() * p.vy;
        p.fAlpha = 0.18f + 0.30f * fDepth + (g_fIntensity > 0.5f ? 0.2f * (g_fIntensity - 0.5f) : 0.0f);
        p.fGroundY = (float)g_nViewH * (0.15f + 0.85f * Rand01());
    }
    else
    {
        p.fSize = (1.6f + 3.2f * fDepth * fDepth) * g_fPxScale;
        p.vy = (28.0f + 55.0f * fDepth) * g_fPxScale;
        p.vx = -6.0f;
        p.fAlpha = 0.55f + 0.45f * fDepth;
        p.fGroundY = 0.0f;
    }
    p.fFollow = 0.15f + 0.30f * fDepth;
    p.fPhase = Rand01() * 6.2831853f;
    p.x = Rand01() * (nKind == kWeatherRain ? RainSpawnWidth() : (float)g_nViewW + 120.0f);
    p.y = bAnywhere ? Rand01() * (float)g_nViewH : -p.fSize - Rand01() * 40.0f;
}

#if !GAME_CC
static unsigned int s_aDotPixels[32 * 32];
static void EnsureDot()
{
    if (g_bDotTried)
        return;
    g_bDotTried = true;
    for (int y = 0; y < 32; y++)
        for (int x = 0; x < 32; x++)
        {
            float dx = ((float)x + 0.5f) / 16.0f - 1.0f, dy = ((float)y + 0.5f) / 16.0f - 1.0f;
            float d = sqrtf(dx * dx + dy * dy);
            float a = d < 0.45f ? 1.0f : (d > 1.0f ? 0.0f : 1.0f - (d - 0.45f) / 0.55f);
            a = a * a * (3.0f - 2.0f * a);
            unsigned int nA = (unsigned int)(a * 255.0f + 0.5f);
            s_aDotPixels[y * 32 + x] = (nA << 24) | 0xFFFFFF;  // B8G8R8A8: straight alpha, white
        }
    ImageDataSrc src = {};
    src.m_nKind = 1;
    src.m_nWidth = 32;
    src.m_nHeight = 32;
    src.m_nRowPitch = 32 * 4;
    src.m_nFormat = 44;                         // VK_FORMAT_B8G8R8A8_UNORM
    src.m_fnDecode = [](const ImageDataSrc &, std::byte *pDst, const TexLoadOptions &)
    {
        memcpy(pDst, s_aDotPixels, sizeof(s_aDotPixels));
    };
    TexLoadOptions opts;
    opts.m_nSamplerIndex = 0;
    g_phDot = new cTSBufferRef(Texture_LoadWithDecoder(Graphics_GetCurrentTarget(), &src, &opts));
}

// one quad of the dot texture: centre (cx, cy), half extents along the unit axis (ax, ay) and its normal
static void WriteQuad(VertPosColorUV *v, float cx, float cy, float ax, float ay, float fHalfLen, float fHalfWid,
                      unsigned int nColor, float u0, float v0, float u1, float v1)
{
    float lx = ax * fHalfLen, ly = ay * fHalfLen;           // along
    float wx = -ay * fHalfWid, wy = ax * fHalfWid;          // across
    float px[4] = { cx - lx - wx, cx - lx + wx, cx + lx + wx, cx + lx - wx };
    float py[4] = { cy - ly - wy, cy - ly + wy, cy + ly + wy, cy + ly - wy };
    float pu[4] = { u0, u1, u1, u0 };
    float pv[4] = { v0, v0, v1, v1 };
    static const int s_aIdx[6] = { 0, 1, 2, 2, 3, 0 };
    for (int i = 0; i < 6; i++)
    {
        int k = s_aIdx[i];
        v[i].x = px[k];
        v[i].y = py[k];
        v[i].z = 0.0f;
        v[i].color = nColor;
        v[i].u = pu[k];
        v[i].v = pv[k];
    }
}

#endif

static float g_fScrollVelX, g_fScrollVelY;      // how fast scrolling moves the particles on screen (px/s, smoothed)

#if !GAME_CC

// a point of the scene's quad list (RenderContext's pick geometry): inside one of the quad's two triangles?
static bool QuadCoversPoint(const PickQuadRecord &q, float fX, float fY)
{
    for (int nBase = 0; nBase <= 3; nBase += 3)
    {
        const PickQuadVert *v = &q.m_Verts[nBase];
        float fAx = v[0].m_fX - v[1].m_fX, fAy = v[0].m_fY - v[1].m_fY;
        float fCx = v[2].m_fX - v[1].m_fX, fCy = v[2].m_fY - v[1].m_fY;
        float fDet = fCy * fAx - fCx * fAy;
        if (fDet > 1e-6f || fDet < -1e-6f)
        {
            float fPx = fX - v[1].m_fX, fPy = fY - v[1].m_fY;
            float fU = (fCy * fPx - fCx * fPy) / fDet;
            float fV = (fAx * fPy - fAy * fPx) / fDet;
            if (fU >= 0.0f && fV >= 0.0f && fU + fV <= 1.0f)
                return true;
        }
    }
    return false;
}

// does a raindrop landing at screen point (x, y) hit open ground? Outdoor tiles inside the lot (terrain, patios, paths,
// ponds, pools) that nothing is drawn over: no splashes outside the lot, indoors, on walls / upper floors or on objects
// / Sims standing in front of the ground there.
#endif

static bool IsGroundVisibleAt(float x, float y)
{
    cFixedWorld *pLot = Lot();
    if (!pLot || !pLot->Floors(0) || !Settings()->m_pHouseViewer)
        return false;
    long nX = (long)x, nY = (long)y;
    ScreenPt16 pt;
    pt.nX = (short)nX;
    pt.nY = (short)nY;
    FTilePt hit = PointToTile(pt);
    int tx = hit.nPosX16 >> 4, ty = hit.nPosY16 >> 4;
    if (tx < pLot->m_nRotMinX || tx > pLot->m_nRotMaxX || ty < pLot->m_nRotMinY || ty > pLot->m_nRotMaxY)
        return false;        // outside the lot (the grey surroundings)
    CTilePt view = { (signed char)tx, (signed char)ty, 1 };
    CTilePt world = LotRotate(view);
    if (world.nX < 0 || world.nY < 0 || world.nX >= 64 || world.nY >= 64)
        return false;
    if (!(LotGetFlags(view) & 4))
        return false;        // indoors (tile flag 4 = outside: grass, patios, paths, ponds and pools all splash)
#if GAME_CC
    // 2003: the world is software sprites with no pick-quad lists. An upper-storey floor over the tile is in front of
    // the ground; a drop landing on an object / Sim standing outdoors still splashes (a short, faint ellipse).
    if (pLot->Floors(1) && (*pLot->Floors(1))[world.nX][world.nY])
        return false;
    return true;
#else
    RenderContext &rc = Settings()->m_pHouseViewer->Context();
    // objects and Sims: the game's own alpha-tested pick (its scratch record list is kept for the mouse pick)
    {
        std::vector<PickRecord> saved;
        saved.swap(rc.m_PickRecords);
        PickHitResult res = {};
        bool bHit = rc.HitTestScreenPoint((int)nX, (int)nY, &res);
        rc.m_PickRecords.swap(saved);
        // a hit without a record is the terrain itself (HitTestScreenPoint also tests the terrain outline): bare ground
        if (bHit && res.m_pRecord)
            return false;
    }
    // walls and floors have no owning object, so the pick above skips them: walls are pair / blend quads, floor and
    // water tiles tile quads. Any wall covering the point stands in front of the ground. A tile quad covering it is
    // either this tile's own floor / water (its centre maps back to this tile) or an upper-storey floor in front.
    if (nX < 0 || nY < 0)
        return true;
    int nCell = 16 * ((nY / 32) & 0xF) + ((nX / 32) & 0xF);
    const RenderBuckets *apLayers[4] = { &rc.m_SpriteQuads, &rc.m_BlendQuads, &rc.m_PairQuads, &rc.m_WallQuads };
    for (const RenderBuckets *pLayer : apLayers)
    {
        for (int nIndex : pLayer->m_Lists[nCell])
        {
            if (nIndex < 0 || (size_t)nIndex >= pLayer->m_Records.size())
                continue;
            const PickQuadRecord &q = pLayer->m_Records[nIndex];
            if (q.m_nObjectId > 0 || !QuadCoversPoint(q, (float)nX, (float)nY))
                continue;
            if (pLayer == &rc.m_WallQuads)
            {
                float fCx = 0.0f, fCy = 0.0f;
                for (int k = 0; k < 6; k++)
                {
                    fCx += q.m_Verts[k].m_fX;
                    fCy += q.m_Verts[k].m_fY;
                }
                ScreenPt16 ptCentre;
                ptCentre.nX = (short)(fCx / 6.0f);
                ptCentre.nY = (short)(fCy / 6.0f);
                FTilePt centreTile = PointToTile(ptCentre);
                if ((centreTile.nPosX16 >> 4) == tx && (centreTile.nPosY16 >> 4) == ty)
                    continue;           // this tile's own floor / water
            }
            return false;
        }
    }
    return true;
#endif
}

#if GAME_CC
// ---- 2003: software drawing into the lot view's 16-bit buffer ------------------------------------------------------
//
// The 2003 lot view keeps its buffer between frames and redraws only damaged rectangles, so an overlay painted into it
// would stay. While weather (or a lightning flash) shows, Seasons_PrePaint adds the whole view to the damage before the
// scene is drawn - what a lot with a pool already gets on every frame - and the overlay is drawn over the fresh scene
// afterwards. Blending is per pixel in the native 555 / 565 format: a veil over the whole view, streaks, flakes,
// splashes and the bolt as soft-edged spans.
static bool g_bOverlayShown;                    // the overlay was drawn on the last frame (its pixels must be redrawn)

struct OverlayTarget
{
    unsigned short *pBits;
    int nPitch;                                 // pixels
    cTSRect rcClip;                             // buffer coordinates, right / bottom exclusive
    bool b565;
};

static inline unsigned short BlendPixel(unsigned short p, int r, int g, int b, int a, bool b565)
{
    // a 0..256; r, g, b 0..255 (the target colour)
    int pr, pg, pb;
    if (b565)
    {
        pr = (p >> 11) & 31;
        pg = (p >> 5) & 63;
        pb = p & 31;
        pr += (((r >> 3) - pr) * a) >> 8;
        pg += (((g >> 2) - pg) * a) >> 8;
        pb += (((b >> 3) - pb) * a) >> 8;
        return (unsigned short)((pr << 11) | (pg << 5) | pb);
    }
    pr = (p >> 10) & 31;
    pg = (p >> 5) & 31;
    pb = p & 31;
    pr += (((r >> 3) - pr) * a) >> 8;
    pg += (((g >> 3) - pg) * a) >> 8;
    pb += (((b >> 3) - pb) * a) >> 8;
    return (unsigned short)((pr << 10) | (pg << 5) | pb);
}

static inline void PlotBlend(const OverlayTarget &t, int x, int y, int r, int g, int b, float fAlpha)
{
    if (x < t.rcClip.left || y < t.rcClip.top || x >= t.rcClip.right || y >= t.rcClip.bottom || fAlpha <= 0.004f)
        return;
    int a = (int)(fAlpha * 256.0f + 0.5f);
    if (a > 256)
        a = 256;
    unsigned short *p = t.pBits + y * t.nPitch + x;
    *p = BlendPixel(*p, r, g, b, a, t.b565);
}

static void FillBlend(const OverlayTarget &t, int r, int g, int b, float fAlpha)
{
    int a = (int)(fAlpha * 256.0f + 0.5f);
    if (a <= 0)
        return;
    if (a > 256)
        a = 256;
    for (int y = t.rcClip.top; y < t.rcClip.bottom; y++)
    {
        unsigned short *p = t.pBits + y * t.nPitch + t.rcClip.left;
        for (int x = t.rcClip.left; x < t.rcClip.right; x++, p++)
            *p = BlendPixel(*p, r, g, b, a, t.b565);
    }
}

// a soft ellipse: radii (rx, ry), full alpha inside 45% of the radius, fading to 0 at the edge (the dot's profile)
static void SoftEllipse(const OverlayTarget &t, float cx, float cy, float rx, float ry, int r, int g, int b, float fAlpha)
{
    if (rx < 0.5f)
        rx = 0.5f;
    if (ry < 0.5f)
        ry = 0.5f;
    int x0 = (int)floorf(cx - rx), x1 = (int)ceilf(cx + rx), y0 = (int)floorf(cy - ry), y1 = (int)ceilf(cy + ry);
    for (int y = y0; y <= y1; y++)
        for (int x = x0; x <= x1; x++)
        {
            float dx = ((float)x + 0.5f - cx) / rx, dy = ((float)y + 0.5f - cy) / ry;
            float d = sqrtf(dx * dx + dy * dy);
            if (d >= 1.0f)
                continue;
            float f = d < 0.45f ? 1.0f : 1.0f - (d - 0.45f) / 0.55f;
            f = f * f * (3.0f - 2.0f * f);
            PlotBlend(t, x, y, r, g, b, fAlpha * f);
        }
}

// a soft line from the centre (cx, cy) along the unit axis (ax, ay): half length fHalfLen, half width fHalfWid; the
// alpha fades out over the ends (bTaper, the rain streaks) or stays full along it (the bolt)
static void SoftLine(const OverlayTarget &t, float cx, float cy, float ax, float ay, float fHalfLen, float fHalfWid,
                     int r, int g, int b, float fAlpha, bool bTaper)
{
    float fExt = fHalfLen + fHalfWid + 1.0f;
    float fWx = fabsf(ax) * fExt + fabsf(ay) * (fHalfWid + 1.0f), fWy = fabsf(ay) * fExt + fabsf(ax) * (fHalfWid + 1.0f);
    int x0 = (int)floorf(cx - fWx), x1 = (int)ceilf(cx + fWx), y0 = (int)floorf(cy - fWy), y1 = (int)ceilf(cy + fWy);
    if (x1 < t.rcClip.left || y1 < t.rcClip.top || x0 >= t.rcClip.right || y0 >= t.rcClip.bottom)
        return;
    float fWid = fHalfWid < 0.5f ? 0.5f : fHalfWid;
    for (int y = y0; y <= y1; y++)
        for (int x = x0; x <= x1; x++)
        {
            float px = (float)x + 0.5f - cx, py = (float)y + 0.5f - cy;
            float u = px * ax + py * ay;                // along
            float v = -px * ay + py * ax;               // across
            float fAcross = 1.0f - (fabsf(v) - fWid + 0.5f);
            if (fAcross <= 0.0f)
                continue;
            if (fAcross > 1.0f)
                fAcross = 1.0f;
            float fAlong = fHalfLen - fabsf(u) + 0.5f;
            if (fAlong <= 0.0f)
                continue;
            if (fAlong > 1.0f)
                fAlong = 1.0f;
            if (bTaper)
            {
                float s = u / (fHalfLen > 0.5f ? fHalfLen : 0.5f);
                fAlong *= 1.0f - s * s;
            }
            PlotBlend(t, x, y, r, g, b, fAlpha * fAcross * fAlong);
        }
}

// cDDDSimsView::TSPaint, before the scene is drawn: while the overlay shows, or showed on the last frame (its pixels
// must go), the whole view is damaged so it is redrawn (with the per-rect cost at its default, as the game does when it
// damages the whole view for a pool)
void Seasons_PrePaint()
{
    HouseViewer *pViewer = Settings() ? Settings()->m_pHouseViewer : 0;
    if (!pViewer)
        return;
    bool bWeather = g_fIntensity > 0.001f && g_nParticleKind != kWeatherClear;
    if (!bWeather && !g_bOverlayShown)
        return;
    float &fRenderParam = Global<float>(A::g_fRenderParam);
    float fSaved = fRenderParam;
    fRenderParam = Global<float>(A::g_fRenderParamDefault);
    pViewer->m_DamageRects0.Add(pViewer->m_ViewRect);
    fRenderParam = fSaved;
}

void Seasons_PaintOverlay(cTSBuffer *pViewBuf, int nLeft, int nTop, int nRight, int nBottom)
#else
void Seasons_PaintOverlay(int nLeft, int nTop, int nRight, int nBottom)
#endif
{
    cSimulator *pSim = Sim();
#if GAME_CC
    // 2003: the scene is drawn into HouseViewer's view buffer over its view rect (the rect the damage above covers)
    HouseViewer *pViewer = Settings() ? Settings()->m_pHouseViewer : 0;
    if (pViewer && pViewer->m_pViewBuf)
    {
        pViewBuf = pViewer->m_pViewBuf;
        nLeft = pViewer->m_ViewRect.left;
        nTop = pViewer->m_ViewRect.top;
        nRight = pViewer->m_ViewRect.right;
        nBottom = pViewer->m_ViewRect.bottom;
    }
    g_bOverlayShown = false;
#endif
    if (!pSim || nRight <= nLeft || nBottom <= nTop)
        return;

    // ---- trees moved (autumn): rebuild the leaf map ----
    static int s_nFrame;
    s_nFrame++;
    if (g_bSeasonalPalette && g_fAppliedLeaf > 0.0f && (s_nFrame % 30) == 0 && ComputeTreeSignature() != g_nLeafSignature)
    {
        BuildLeafMap();
        GlobalDispatch(131, 0);
    }

    // ---- time: real seconds for fades, sim-scaled seconds for motion (pause freezes, fast speed hurries) ----
    DWORD nNow = GetTickCount();
    float dtReal = g_nLastTick ? (float)(nNow - g_nLastTick) / 1000.0f : 0.0f;
    g_nLastTick = nNow;
    if (dtReal > 0.1f)
        dtReal = 0.1f;                          // a hitch (or the first frame) must not teleport everything
    float fSpeed = 0.0f;
    if (!pSim->IsPaused())
    {
        float fRate = pSim->GetSimRate();       // 1.0 at normal speed, up to 42 at the fastest
        fSpeed = 1.0f + (fRate > 0.0f ? logf(fRate) / logf(2.0f) : 0.0f) * 0.35f;
        fSpeed = fSpeed < 0.3f ? 0.3f : (fSpeed > 2.5f ? 2.5f : fSpeed);
    }
    float dt = dtReal * fSpeed;
    g_fAnimTime += dt;

    // ---- weather state ----
    int nKind;
    float fTarget;
    bool bController = ControllerActive(pSim);
    if (bController)
    {
        // the Seasons Controller object decides (globals Weather / WeatherIntensity)
        nKind = GlobalAt(pSim, kGlobalWeather);
        if (nKind < kWeatherClear || nKind > kWeatherStorm)
            nKind = kWeatherClear;
        int nPercent = GlobalAt(pSim, kGlobalWeatherIntensity);
        nPercent = nPercent < 0 ? 0 : (nPercent > 100 ? 100 : nPercent);
        fTarget = nKind == kWeatherClear ? 0.0f : (float)nPercent / 100.0f;
    }
    else
        AutoWeather(&nKind, &fTarget);
    float fPublishIntensity = fTarget;          // the weather's own intensity, as objects see it
    // a thunderstorm is rain (the same particles: rain <-> storm needs no fade-out) that is always heavy, plus the storm
    // effects, eased in and out on their own
    int nParticleKind = nKind;
    float fStormTarget = 0.0f;
    if (nKind == kWeatherStorm)
    {
        nParticleKind = kWeatherRain;
        g_fStormPower = fTarget;
        fStormTarget = fTarget > 0.0f ? 1.0f : 0.0f;
        fTarget = fTarget > 0.0f ? 0.6f + 0.4f * fTarget : 0.0f;
    }
    if (nParticleKind != g_nParticleKind && g_fIntensity > 0.02f)
    {
        fTarget = 0.0f;                         // fade the old kind out before switching
        fStormTarget = 0.0f;
    }
    else if (nParticleKind != g_nParticleKind)
    {
        g_nParticleKind = nParticleKind;
        g_nParticles = 0;
        g_nSplashes = 0;
    }
    float fEase = dtReal * 0.6f;                // ~2 s to full strength (real time, so it also eases in while paused)
    if (g_fIntensity < fTarget)
        g_fIntensity = g_fIntensity + fEase > fTarget ? fTarget : g_fIntensity + fEase;
    else
        g_fIntensity = g_fIntensity - fEase < fTarget ? fTarget : g_fIntensity - fEase;
    float fStormEase = dtReal * 0.25f;          // ~4 s: the sky darkens and the wind rises a little after the rain
    g_fStorm = Approach(g_fStorm, g_nParticleKind == kWeatherRain ? fStormTarget : 0.0f, fStormEase);
    UpdateLightning(dt, dtReal);

    // ---- the ground follows the season (and settles snow while it snows) ----
    g_fSnowfall = g_nParticleKind == kWeatherSnow ? g_fIntensity : 0.0f;
    UpdateGroundState();

    // ---- publish to objects (Seasons.h, eSeasonsGlobal): the ground always, the decisions when the mod made them ----
    pSim->SetGlobal(kGlobalSnowCover, IsSnowIsland(pSim->m_Globals.CurrentHouse) ? 100 : (short)(g_fSnow * 100.0f + 0.5f));
    pSim->SetGlobal(kGlobalLeafCover, (short)(g_fLeaf * 100.0f + 0.5f));
    pSim->SetGlobal(kGlobalWinterwear, (short)(g_bWinterwear ? 1 : 0));
    // raincoat weather: rain or a thunderstorm on a lot whose ground follows the seasons, unless it is coat weather
    g_bRainwear = !g_bWinterwear && IsSeasonalHouse(pSim->m_Globals.CurrentHouse) &&
                  (nKind == kWeatherRain || nKind == kWeatherStorm) && fPublishIntensity > 0.0f;
    pSim->SetGlobal(kGlobalRainwear, (short)(g_bRainwear ? 1 : 0));
    pSim->SetGlobal(kGlobalRainwearArt, (short)Seasons_RainwearArt());
    if (!bController)
    {
        pSim->SetGlobal(kGlobalSeason, (short)Seasons_GetSeason());
        pSim->SetGlobal(kGlobalWeather, (short)nKind);
        pSim->SetGlobal(kGlobalWeatherIntensity, (short)(fPublishIntensity * 100.0f + 0.5f));
    }

    if (g_fIntensity <= 0.001f || g_nParticleKind == kWeatherClear)
        return;

    int nW = nRight - nLeft, nH = nBottom - nTop;
    bool bResized = nW != g_nViewW || nH != g_nViewH;
    g_nViewW = nW;
    g_nViewH = nH;
    g_fPxScale = nH > 900 ? (float)nH / 900.0f : 1.0f;

    // ---- particle count follows intensity and screen area ----
    float fArea = (float)nW * (float)nH / (1024.0f * 768.0f);
    // rain grows quadratically: intensity 50 is 900 drops per 1024x768, 100 is a downpour (2700)
    float fAmount = g_nParticleKind == kWeatherRain ? 900.0f * (g_fIntensity + 2.0f * g_fIntensity * g_fIntensity)
                                                    : 1100.0f * g_fIntensity;
    int nWant = (int)(fAmount * fArea);
    if (nWant > kMaxParticles)
        nWant = kMaxParticles;
    if (bResized)
        g_nParticles = 0;
    while (g_nParticles < nWant)
        SpawnParticle(g_aParticles[g_nParticles++], g_nParticleKind, true);
    if (g_nParticles > nWant)
        g_nParticles = nWant;

    // ---- scrolling: the particles live in screen space, so move them with the world (as if they were anchored near
    // the ground) and wrap the ones pushed off an edge; zoom / rotation changes jump instead. Rain follows horizontally
    // (its streaks lean) but not vertically; snow follows by depth in both directions (parallax, 15..45% per flake).
    {
        int nScrollX = Global<int>(A::g_nScrollOffsetX), nScrollY = Global<int>(A::g_nScrollOffsetY);
        int nScale = Global<int>(A::g_nViewScale), nRotation = Global<int>(A::g_nViewRotation);
        bool bSnow = g_nParticleKind == kWeatherSnow;
        const float kScrollFollowX = 1.0f;          // rain (and the streak lean)
        const float kScrollFollowY = 0.0f;
        static int s_nLastScrollX, s_nLastScrollY, s_nLastScale = -1, s_nLastRotation = -1;
        bool bSameView = !bResized && s_nLastScale == nScale && s_nLastRotation == nRotation;
        if (bSameView)
        {
            // the world moves -fDx, -fDy on screen: screen = world + scroll offset
            float fDx = (float)(s_nLastScrollX - nScrollX);
            float fDy = (float)(s_nLastScrollY - nScrollY);
            if ((fDx != 0.0f || fDy != 0.0f) && fabsf(fDx) < (float)nW && fabsf(fDy) < (float)nH)
            {
                float fWrapW = (g_nParticleKind == kWeatherRain ? RainSpawnWidth() : (float)nW + 120.0f), fWrapH = (float)nH + 60.0f;
                for (int i = 0; i < g_nParticles; i++)
                {
                    Particle &p = g_aParticles[i];
                    float fFollowX = bSnow ? 0.6f * p.fFollow : kScrollFollowX;   // snow sideways: 9..27% of the scroll
                    float fFollowY = bSnow ? p.fFollow : kScrollFollowY;
                    p.x -= fDx * fFollowX;
                    float fShiftY = fDy * fFollowY;
                    if (bSnow)
                    {
                        // a scroll must not make a flake rise: the push up may slow its fall to 30%
                        float fFall = p.vy * dtReal;
                        if (fShiftY > 0.7f * fFall)
                            fShiftY = 0.7f * fFall;
                    }
                    p.y -= fShiftY;
                    p.fGroundY -= fDy * fFollowY;
                    if (p.x < -60.0f)
                        p.x += fWrapW;
                    else if (p.x > fWrapW - 60.0f)
                        p.x -= fWrapW;
                    if (p.y < -60.0f)
                    {
                        p.y += fWrapH;
                        p.fGroundY += fWrapH;
                    }
                    else if (p.y > (float)nH + 8.0f && g_nParticleKind == kWeatherSnow)
                        p.y -= fWrapH;
                    if (g_nParticleKind == kWeatherRain && p.fGroundY < p.y)
                        p.fGroundY = p.y + (float)nH * 0.15f * Rand01();   // pushed below its ground: land a little further on
                }
                for (int i = 0; i < g_nSplashes; i++)
                {
                    g_aSplashes[i].x -= fDx;
                    g_aSplashes[i].y -= fDy;
                }
            }
        }
        // the on-screen velocity the shift gives the particles: the rain streaks lean along it (smoothed over ~0.1 s)
        {
            float fShiftX = 0.0f, fShiftY = 0.0f;
            if (bSameView && dtReal > 0.0f)
            {
                float fDx = (float)(s_nLastScrollX - nScrollX);
                float fDy = (float)(s_nLastScrollY - nScrollY);
                if (fabsf(fDx) < (float)nW && fabsf(fDy) < (float)nH)
                {
                    fShiftX = -fDx * kScrollFollowX / dtReal;
                    fShiftY = -fDy * kScrollFollowY / dtReal;
                }
            }
            float fBlend = dtReal > 0.0f ? 1.0f - expf(-dtReal * 10.0f) : 0.0f;
            g_fScrollVelX += (fShiftX - g_fScrollVelX) * fBlend;
            g_fScrollVelY += (fShiftY - g_fScrollVelY) * fBlend;
        }
        s_nLastScrollX = nScrollX;
        s_nLastScrollY = nScrollY;
        s_nLastScale = nScale;
        s_nLastRotation = nRotation;
    }

    // ---- simulate ----
    for (int i = 0; i < g_nParticles; i++)
    {
        Particle &p = g_aParticles[i];
        if (g_nParticleKind == kWeatherSnow)
        {
            p.x += (p.vx + 14.0f * g_fPxScale * sinf(g_fAnimTime * 1.3f + p.fPhase)) * dt;
            p.y += p.vy * dt;
            // off the bottom, or drifted past the left edge: a new flake at the top
            if (p.y > (float)nH + 8.0f || p.x < -60.0f)
                SpawnParticle(p, g_nParticleKind, false);
        }
        else
        {
            p.x += p.vx * dt;
            p.y += p.vy * dt;
            if (p.y >= p.fGroundY)
            {
                if (g_nSplashes < kMaxSplashes && p.fAlpha > 0.3f && IsGroundVisibleAt(p.x, p.fGroundY))
                {
                    Splash &s = g_aSplashes[g_nSplashes++];
                    s.x = p.x;
                    s.y = p.fGroundY;
                    s.t = 0.22f;
                }
                SpawnParticle(p, g_nParticleKind, false);
            }
        }
    }
    for (int i = 0; i < g_nSplashes;)
    {
        g_aSplashes[i].t -= dt;
        if (g_aSplashes[i].t <= 0.0f)
            g_aSplashes[i] = g_aSplashes[--g_nSplashes];
        else
            i++;
    }

#if GAME_CC
    // ---- draw (2003): straight into the view buffer's pixels ----
    if (!pViewBuf || !pViewBuf->Lock(kTSLockPixels))
        return;
    {
        OverlayTarget t;
        t.pBits = (unsigned short *)pViewBuf->GetBits();
        t.nPitch = pViewBuf->GetPitch() / 2;
        t.b565 = pViewBuf->Is565();
        t.rcClip.left = nLeft < 0 ? 0 : nLeft;
        t.rcClip.top = nTop < 0 ? 0 : nTop;
        t.rcClip.right = nRight > pViewBuf->m_rcBounds.right ? pViewBuf->m_rcBounds.right : nRight;
        t.rcClip.bottom = nBottom > pViewBuf->m_rcBounds.bottom ? pViewBuf->m_rcBounds.bottom : nBottom;
        if (t.pBits && t.nPitch > 0 && t.rcClip.right > t.rcClip.left && t.rcClip.bottom > t.rcClip.top)
        {
            float fFlash = g_nParticleKind == kWeatherRain ? LightningFlash() : 0.0f;
            float fL = (float)nLeft, fT = (float)nTop;
            // atmosphere: a grey-blue veil for rain (darker in a thunderstorm), a faint cold haze for snow
            if (g_nParticleKind == kWeatherRain)
            {
                float fRainVeil = 0.52f * g_fIntensity;
                if (fRainVeil > 0.45f)
                    fRainVeil = 0.45f;
                fRainVeil += (0.58f - fRainVeil) * g_fStorm;
                FillBlend(t, 40 - (int)(14.0f * g_fStorm), 48 - (int)(16.0f * g_fStorm), 62 - (int)(16.0f * g_fStorm), fRainVeil);
            }
            else
                FillBlend(t, 225, 232, 245, 0.10f * g_fIntensity);
            // lightning: the whole view lights up cold white (stronger at night)
            if (fFlash > 0.0f)
                FillBlend(t, 228, 234, 255, fFlash * (g_bDaytime ? 0.38f : 0.62f));
            for (int i = 0; i < g_nParticles; i++)
            {
                const Particle &p = g_aParticles[i];
                if (g_nParticleKind == kWeatherRain)
                {
                    float fVx = p.vx + g_fScrollVelX, fVy = p.vy + g_fScrollVelY;
                    float fFall = sqrtf(p.vx * p.vx + p.vy * p.vy);
                    float fLen = sqrtf(fVx * fVx + fVy * fVy);
                    float fStretch = fLen / fFall;
                    fStretch = fStretch < 1.0f ? 1.0f : (fStretch > 1.6f ? 1.6f : fStretch);
                    SoftLine(t, fL + p.x, fT + p.y, fVx / fLen, fVy / fLen, p.fSize * 0.5f * fStretch, 0.45f * g_fPxScale,
                             200, 210, 225, p.fAlpha, true);
                }
                else
                {
                    float h = p.fSize * 0.5f;
                    SoftEllipse(t, fL + p.x, fT + p.y, h, h, 255, 255, 255, p.fAlpha);
                }
            }
            for (int i = 0; i < g_nSplashes; i++)
            {
                const Splash &s = g_aSplashes[i];
                float f = 1.0f - s.t / 0.22f;
                float fRad = (2.0f + 6.0f * f) * g_fPxScale;
                SoftEllipse(t, fL + s.x, fT + s.y, fRad, fRad * 0.35f, 210, 220, 235, 0.45f * (1.0f - f));
            }
            // the bolt: a wide soft glow per segment, then a thin bright core
            int nBoltSegs = fFlash > 0.02f && g_nBoltPts > 1 ? g_nBoltPts - 2 : 0;
            for (int nPass = 0; nPass < 2 && nBoltSegs > 0; nPass++)
            {
                float fHalfWid = (nPass == 0 ? 3.5f : 0.8f) * g_fPxScale;
                for (int i = 0; i + 1 < g_nBoltPts; i++)
                {
                    if (i + 1 == g_nBoltBranchAt)
                        continue;
                    float x0 = g_afBoltX[i], y0 = g_afBoltY[i], x1 = g_afBoltX[i + 1], y1 = g_afBoltY[i + 1];
                    float dx = x1 - x0, dy = y1 - y0;
                    float fLen = sqrtf(dx * dx + dy * dy);
                    if (fLen < 0.01f)
                        fLen = 0.01f;
                    float fWid = i >= g_nBoltBranchAt ? fHalfWid * 0.6f : fHalfWid;
                    if (nPass == 0)
                        SoftLine(t, fL + (x0 + x1) * 0.5f, fT + (y0 + y1) * 0.5f, dx / fLen, dy / fLen, fLen * 0.5f + fWid * 0.5f,
                                 fWid, 170, 185, 255, 0.35f * fFlash, false);
                    else
                        SoftLine(t, fL + (x0 + x1) * 0.5f, fT + (y0 + y1) * 0.5f, dx / fLen, dy / fLen, fLen * 0.5f + fWid * 0.5f,
                                 fWid, 245, 248, 255, fFlash, false);
                }
            }
            g_bOverlayShown = true;
        }
    }
    pViewBuf->Unlock(kTSLockPixels);
#else
    // ---- draw: one alpha-blended batch (pipeline 3, the dot texture) ----
    EnsureDot();
    ImageRes *pDot = g_phDot ? g_phDot->Get() : 0;
    if (!pDot || !pDot->m_Tex.m_hSet)
        return;
    RenderPassState *pState = Graphics_GetCurrentTarget()->GetCurrentState();
    if (!pState || !pState->m_pPipelines)
        return;
    Canvas canvas(pState);
    float fFlash = g_nParticleKind == kWeatherRain ? LightningFlash() : 0.0f;
    int nBoltSegs = fFlash > 0.02f && g_nBoltPts > 1 ? g_nBoltPts - 2 : 0;   // the branch's first point starts no segment
    int nQuads = 1 + (fFlash > 0.0f ? 1 : 0) + g_nParticles + g_nSplashes + 2 * nBoltSegs;
    VertexRange r = canvas.AllocVerticesPosColorUV(nQuads * 6);
    if (!r.ptr)
        return;
    VertPosColorUV *v = (VertPosColorUV *)r.ptr;
    float fL = (float)nLeft, fT = (float)nTop;

    // atmosphere: a grey-blue veil for rain, a faint cold haze for snow (the dot's solid core: uv 0.47..0.53)
    float fCx = fL + nW * 0.5f, fCy = fT + nH * 0.5f;
    float fRainVeil = 0.52f * g_fIntensity;
    if (fRainVeil > 0.45f)
        fRainVeil = 0.45f;
    // a thunderstorm darkens it further, to a slate grey (up to 0.58)
    fRainVeil += (0.58f - fRainVeil) * g_fStorm;
    int nVeilR = 40 - (int)(14.0f * g_fStorm), nVeilG = 48 - (int)(16.0f * g_fStorm), nVeilB = 62 - (int)(16.0f * g_fStorm);
    unsigned int nVeil = g_nParticleKind == kWeatherRain ? VtxColor(nVeilR, nVeilG, nVeilB, fRainVeil)
                                                         : VtxColor(225, 232, 245, 0.10f * g_fIntensity);
    WriteQuad(v, fCx, fCy, 1.0f, 0.0f, nW * 0.5f, nH * 0.5f, nVeil, 0.47f, 0.47f, 0.53f, 0.53f);
    v += 6;
    // lightning: the whole view lights up cold white (stronger at night)
    if (fFlash > 0.0f)
    {
        float fA = fFlash * (g_bDaytime ? 0.38f : 0.62f);
        WriteQuad(v, fCx, fCy, 1.0f, 0.0f, nW * 0.5f, nH * 0.5f, VtxColor(228, 234, 255, fA), 0.47f, 0.47f, 0.53f, 0.53f);
        v += 6;
    }

    for (int i = 0; i < g_nParticles; i++)
    {
        const Particle &p = g_aParticles[i];
        if (g_nParticleKind == kWeatherRain)
        {
            // the streak follows the drop's motion on screen: its fall plus what scrolling adds (it leans while panning)
            float fVx = p.vx + g_fScrollVelX, fVy = p.vy + g_fScrollVelY;
            float fFall = sqrtf(p.vx * p.vx + p.vy * p.vy);
            float fLen = sqrtf(fVx * fVx + fVy * fVy);
            float ax = fVx / fLen, ay = fVy / fLen;
            float fStretch = fLen / fFall;
            fStretch = fStretch < 1.0f ? 1.0f : (fStretch > 1.6f ? 1.6f : fStretch);
            WriteQuad(v, fL + p.x, fT + p.y, ax, ay, p.fSize * 0.5f * fStretch, 0.9f * g_fPxScale, VtxColor(200, 210, 225, p.fAlpha),
                      0.0f, 0.0f, 1.0f, 1.0f);
        }
        else
        {
            float h = p.fSize * 0.5f;
            WriteQuad(v, fL + p.x, fT + p.y, 1.0f, 0.0f, h, h, VtxColor(255, 255, 255, p.fAlpha), 0.0f, 0.0f, 1.0f, 1.0f);
        }
        v += 6;
    }
    for (int i = 0; i < g_nSplashes; i++)
    {
        const Splash &s = g_aSplashes[i];
        float f = 1.0f - s.t / 0.22f;           // 0 -> 1 over the splash life
        float fRad = (2.0f + 6.0f * f) * g_fPxScale;
        WriteQuad(v, fL + s.x, fT + s.y, 1.0f, 0.0f, fRad, fRad * 0.35f, VtxColor(210, 220, 235, 0.45f * (1.0f - f)),
                  0.0f, 0.0f, 1.0f, 1.0f);
        v += 6;
    }
    // the bolt: per segment a wide soft glow, then a thin bright core (the dot's profile across, its middle row along)
    for (int nPass = 0; nPass < 2 && nBoltSegs > 0; nPass++)
    {
        float fHalfWid = (nPass == 0 ? 7.0f : 1.6f) * g_fPxScale;
        unsigned int nColor = nPass == 0 ? VtxColor(170, 185, 255, 0.35f * fFlash) : VtxColor(245, 248, 255, fFlash);
        for (int i = 0; i + 1 < g_nBoltPts; i++)
        {
            if (i + 1 == g_nBoltBranchAt)
                continue;                       // the main channel's end -> the branch's start is no segment
            float x0 = g_afBoltX[i], y0 = g_afBoltY[i], x1 = g_afBoltX[i + 1], y1 = g_afBoltY[i + 1];
            float dx = x1 - x0, dy = y1 - y0;
            float fLen = sqrtf(dx * dx + dy * dy);
            if (fLen < 0.01f)
                fLen = 0.01f;
            float fWid = i >= g_nBoltBranchAt ? fHalfWid * 0.6f : fHalfWid;     // the branch is thinner
            WriteQuad(v, fL + (x0 + x1) * 0.5f, fT + (y0 + y1) * 0.5f, dx / fLen, dy / fLen, fLen * 0.5f + fWid * 0.5f,
                      fWid, nColor, 0.0f, 0.5f, 1.0f, 0.5f);
            v += 6;
        }
    }

    unsigned __int64 hSet = pDot->m_Tex.m_hSet;
    Canvas::SubmitQuad3(pState, r, hSet);
#endif
}

// ---- find_location_for modes 7..10 (snow) ------------------------------------------------------------------------
// Not in the original (Seasons Controller; SimC FindLocationMode.OutdoorFloor / OutdoorOffRoad). All keep off the road:
// the asphalt of the street and the driveway (built-in floors 9..11 on a locked tile), where the car portal places cars.
//   7 OutdoorFloor:   a random free ground tile that has a floor (patio, path, sidewalk) and lies outdoors.
//   8 OutdoorOffRoad: a free outdoor ground tile, floor or not; the nearest one to the object in `from` when given
//                     (a patch grows), else a random one.
//   9 OutdoorGround:  a free outdoor tile of bare ground, with the whole adult snow-angel imprint footprint behind it
//                     bare outdoor ground too (own tile + the 3 x 2 block behind), no walls between those tiles, no
//                     other imprint's footprint on them. Nearest to `from`, else random.
//  10 OutdoorGroundSmall: the same for a child's imprint (own tile, the row behind, the middle of the second row).
// False when no such tile is free. The lot border is skipped.
static const int kFindLocationOutdoorFloor = 7;
static const int kFindLocationOutdoorOffRoad = 8;
static const int kFindLocationOutdoorGround = 9;
static const int kFindLocationOutdoorGroundSmall = 10;
enum { SAE_NoStackObject = 0x15, SAE_ObjectRefNotFound = 0x17, SAE_LocalOutOfRange = 0x33 };

static bool IsOutdoorRoom(unsigned short nRoom)
{
    if (nRoom == 0xFFFB)                                    // the "outside" room id
        return true;
    Room *pRoom = Settings()->m_pGame->m_pRoomManager->GetRoom(nRoom);
    return pRoom && pRoom->IsOutside();
}

static bool IsRoadTile(cFixedWorld *pWorld, int x, int y)
{
    unsigned short nFloor = (*pWorld->m_pFloorsByLevel[1])[x][y];
    bool bLocked = (((*pWorld->m_pFlags[0])[x][y] >> 5) & 1) != 0;
    return bLocked && nFloor >= 9 && nFloor <= 11;          // floors.iff: 9 asphalt, 10 / 11 asphalt with lane lines
}

static bool IsBareOutdoorGround(cFixedWorld *pWorld, int x, int y)
{
    const int nLevel = 1;
    if (x < 1 || y < 1 || x >= pWorld->m_nSize - 1 || y >= pWorld->m_nSize - 1)
        return false;
    if ((*pWorld->m_pFloorsByLevel[nLevel])[x][y])
        return false;
    if (IsRoadTile(pWorld, x, y))
        return false;
    return IsOutdoorRoom((*pWorld->m_pRooms[nLevel - 1])[x][y]);
}

// the cells a snow angel imprint covers, as (side, back) tile steps from its own tile
struct SnowCell { int nSide, nBack; };
static const SnowCell kSnowFootprintAdult[] = { {0, 0}, {-1, 1}, {0, 1}, {1, 1}, {-1, 2}, {0, 2}, {1, 2} };
static const SnowCell kSnowFootprintChild[] = { {0, 0}, {-1, 1}, {0, 1}, {1, 1}, {0, 2} };

static void SnowCellTile(int x, int y, int nDir, const SnowCell &c, int *pX, int *pY)
{
    const int *pDx = &Global<int>(A::g_DirTileOffsetX), *pDy = &Global<int>(A::g_DirTileOffsetY);
    const int nBack = (nDir + 4) & 7, nSide = (nDir + 2) & 7;
    *pX = x + c.nBack * pDx[nBack] + c.nSide * pDx[nSide];
    *pY = y + c.nBack * pDy[nBack] + c.nSide * pDy[nSide];
}

// a wall, fence or diagonal wall on the edge between two 4-neighbouring tiles (or across either tile)
static bool SnowWallBetween(int x, int y, int nx, int ny)
{
    CTilePt pt = { (signed char)x, (signed char)y, 1 };
    TileWalls walls = GetWall(pt);
    if (walls.m_nWallMask & kWallSeg_AnyDiagonal)
        return true;
    const CTilePt *pOffsets = &Global<CTilePt>(A::g_TileNeighborOffsets);
    for (int d = 0; d < 4; ++d)                             // N, S, W, E
        if (pOffsets[d].nX == nx - x && pOffsets[d].nY == ny - y)
            return (walls.m_nWallMask & (unsigned)Call<int>(A::TileWalls__GetWallBetweenDir, d)) != 0;
    return false;
}

// another imprint (same GUID as pObj) whose adult footprint covers tile (cx, cy)
static bool SnowCellUnderOtherImprint(cXObject *pObj, int cx, int cy)
{
    long nGuid = pObj->m_pSelector->GetGUID();
    for (int ox = cx - 2; ox <= cx + 2; ++ox)
        for (int oy = cy - 2; oy <= cy + 2; ++oy)
        {
            if (ox < 0 || oy < 0 || ox >= World()->m_nSize || oy >= World()->m_nSize)
                continue;
            CTilePt tile = { (signed char)ox, (signed char)oy, 1 };
            for (ObjectIterator it(Settings()->m_pGame, &tile, 0); it.m_pCur; it.Next())
            {
                cXObject *pOther = it.m_pCur;
                if (pOther == pObj || !pOther->m_pSelector || pOther->m_pSelector->GetGUID() != nGuid)
                    continue;
                for (const SnowCell &c : kSnowFootprintAdult)
                {
                    int tx, ty;
                    SnowCellTile(ox, oy, pOther->m_nDirection, c, &tx, &ty);
                    if (tx == cx && ty == cy)
                        return true;
                }
            }
        }
    return false;
}

// nFootprint: 0 none (modes 7 / 8), 1 adult imprint (mode 9), 2 child imprint (mode 10)
static int FindSnowLocation(cXObject *pObj, bool bFloorOnly, cXObject *pNear, int nFootprint = 0)
{
    cFixedWorld *pWorld = World();
    const int nLevel = 1;
    const int nDir = pObj->m_nDirection;
    const SnowCell *pCells = nFootprint == 2 ? kSnowFootprintChild : kSnowFootprintAdult;
    const int nCells = nFootprint == 0 ? 0
                     : nFootprint == 2 ? (int)(sizeof(kSnowFootprintChild) / sizeof(SnowCell))
                                       : (int)(sizeof(kSnowFootprintAdult) / sizeof(SnowCell));
    std::vector<int> candidates;                            // x | y << 8
    for (int x = 1; x < pWorld->m_nSize - 1; ++x)
        for (int y = 1; y < pWorld->m_nSize - 1; ++y)
        {
            if (bFloorOnly && !(*pWorld->m_pFloorsByLevel[nLevel])[x][y])
                continue;
            bool bBare = true;
            for (int i = 0; i < nCells && bBare; ++i)
            {
                int cx, cy;
                SnowCellTile(x, y, nDir, pCells[i], &cx, &cy);
                bBare = IsBareOutdoorGround(pWorld, cx, cy);
            }
            if (!bBare)
                continue;
            if (IsRoadTile(pWorld, x, y))
                continue;
            if (!IsOutdoorRoom((*pWorld->m_pRooms[nLevel - 1])[x][y]))
                continue;
            candidates.push_back(x | (y << 8));
        }
    if (candidates.empty())
        return 0;
    if (pNear)                                              // nearest first (a patch grows around pNear)
    {
        int nx = pNear->m_nPosX16 >> 4, ny = pNear->m_nPosY16 >> 4;
        std::stable_sort(candidates.begin(), candidates.end(), [nx, ny](int a, int b) {
            int dax = (a & 0xFF) - nx, day = (a >> 8) - ny, dbx = (b & 0xFF) - nx, dby = (b >> 8) - ny;
            return dax * dax + day * day < dbx * dbx + dby * dby;
        });
    }
    int nStart = pNear ? 0 : (int)(SimRand() % candidates.size());   // random: a random tile first, then the others
    for (size_t i = 0; i < candidates.size(); ++i)
    {
        int c = candidates[(nStart + i) % candidates.size()];
        int x = c & 0xFF, y = c >> 8;
        bool bOk = true;
        for (int k = 0; k < nCells && bOk; ++k)             // no walls inside, no other imprint, room for the object
        {
            int cx, cy;
            SnowCellTile(x, y, nDir, pCells[k], &cx, &cy);
            for (int m = 0; m < nCells && bOk; ++m)
            {
                int mx, my;
                SnowCellTile(x, y, nDir, pCells[m], &mx, &my);
                if (abs(mx - cx) + abs(my - cy) == 1 && SnowWallBetween(cx, cy, mx, my))
                    bOk = false;
            }
            if (bOk && SnowCellUnderOtherImprint(pObj, cx, cy))
                bOk = false;
            if (bOk && k > 0)
            {
                FTilePt cell;
                cell.nPosX16 = (16 * cx) | 8;
                cell.nPosY16 = (16 * cy) | 8;
                if (!pObj->CanPlace(cell, nLevel, 0, 0))
                    bOk = false;
            }
        }
        if (!bOk)
            continue;
        FTilePt pos;
        pos.nPosX16 = (16 * x) | 8;
        pos.nPosY16 = (16 * y) | 8;
        if (pObj->CanPlace(pos, nLevel, 0, 0))
        {
            pObj->Place(pos, nLevel, 0, 0, true);
            pObj->ForceLocation();
            return 1;
        }
    }
    return 0;
}

// For the modes above, the stack object and the optional reference object (local Op[1] when Op[2] bit 0) are resolved
// exactly as the original does, with the same errors, and FindSnowLocation answers. Any other mode: false.
bool Seasons_FindGoodLocation(cXObject *pThis, StackElem *pFrame, BehaviorNodeParam *pParams, int *pnResult)
{
    int nMode = pParams->m_Op[0];
    if (nMode != kFindLocationOutdoorFloor && nMode != kFindLocationOutdoorOffRoad && nMode != kFindLocationOutdoorGround &&
        nMode != kFindLocationOutdoorGroundSmall)
        return false;
    cXObject *pStackObj = pThis->m_pModule->GetObject(pFrame->m_nStackObjectId);
    if (!pStackObj)
        throw (short)SAE_NoStackObject;
    cXObject *pRefObj = 0;
    if (pParams->m_Op[2] & 1)
    {
        int nLocalIdx = pParams->m_Op[1];
        if (nLocalIdx >= pFrame->m_nNumLocals)
            throw (short)SAE_LocalOutOfRange;
        pRefObj = pThis->m_pModule->GetObject(pFrame->GetLocals()[nLocalIdx]);
        if (!pRefObj)
            throw (short)SAE_ObjectRefNotFound;
    }
    if (nMode == kFindLocationOutdoorFloor)
        *pnResult = FindSnowLocation(pStackObj, true, 0);
    else if (nMode == kFindLocationOutdoorOffRoad)
        *pnResult = FindSnowLocation(pStackObj, false, pRefObj);
    else if (nMode == kFindLocationOutdoorGround)
        *pnResult = FindSnowLocation(pStackObj, false, pRefObj, 1);
    else
        *pnResult = FindSnowLocation(pStackObj, false, pRefObj, 2);
    return true;
}

// ---- dressing at the door ---------------------------------------------------------------------------------------
// In winter a Sim (resident or visitor, not an NPC, pet or ghost) who steps from inside to outside changes into
// Vacation's winterwear (outfit 17) at the door, and back into the outfit worn before on stepping in again: with the
// game's own spin (global tree #486 "Dress in specified outfit", as the dressers use), played as part of the walk.
// cXObject::SetLocation reports the room change here, during the door's portal tree; the route
// (cXPerson::TryGotoRoutingSlot, state 2, where it resumes after a portal tree) then runs the dressing tree the same
// way it ran the door's: a gosub in the middle of the route, so the action the Sim is walking for is not cancelled.
// From everyday clothes, swimsuit, formal wear or PJs (0, 2, 4, 5); a work uniform (3) or no clothes (1) stay. The
// outfit before is remembered per object id for the session; after a reload a Sim coming in in winterwear gets everyday
// clothes. In the rain (Rainwear, and no winterwear) the same with the Sim's raincoat (outfit 61): from everyday
// clothes, formal wear or PJs (0, 4, 5), only when a raincoat for this Sim is installed. Stepping in, both come off.
static const short kOutfitEveryday = 0, kOutfitWinter = 17;
static std::map<short, short> *g_pOutfitBeforeWinter;   // object id -> outfit before winterwear or the raincoat
static std::map<short, short> *g_pDoorOutfit;           // object id -> outfit to change into once through the door

static bool Seasons_IsOutsideRoom(short nRoom)
{
    if ((unsigned short)nRoom == 0xFFFB)
        return true;
    if (!Settings() || !Settings()->m_pGame || !Settings()->m_pGame->m_pRoomManager)
        return false;
    Room *pRoom = Settings()->m_pGame->m_pRoomManager->GetRoom((unsigned short)nRoom);
    return pRoom && pRoom->IsOutside();
}

static bool Seasons_HasRainwear(cXPerson *pPerson);

void Seasons_PersonRoomChanged(cXObject *pObj, short nOldRoom, short nNewRoom)
{
    if (!pObj || pObj->m_pObjdData->type != kObjType_Person)
        return;
    if (!g_pOutfitBeforeWinter)
    {
        g_pOutfitBeforeWinter = new std::map<short, short>;
        g_pDoorOutfit = new std::map<short, short>;
    }
    bool bVacation = (Settings()->m_nExpansionMask & kEP_Vacation) != 0;   // winterwear is Vacation's outfit
    cXPerson *pPerson = AsPerson(pObj);
    if (!pPerson)
        return;
    short *pData = pPerson->m_PersonData;
    if (pData[kPersonDataGender] >= 2 || (pData[kPersonDataIsGhost] & 1) || pData[kPersonDataPersonType] > 1)
        return;
    bool bWasOut = Seasons_IsOutsideRoom(nOldRoom);
    bool bIsOut = Seasons_IsOutsideRoom(nNewRoom);
    if (bWasOut == bIsOut)
        return;
    short outfit = pData[kPersonDataCurrentOutfit];
    short nId = pObj->m_nObjectId;
    g_pDoorOutfit->erase(nId);                          // back through before the last change ran: start over
    if (bIsOut)
    {
        if (g_bWinterwear && bVacation)                 // snow on the ground, not the calendar
        {
            if (outfit == 0 || outfit == 2 || outfit == 4 || outfit == 5)
            {
                (*g_pOutfitBeforeWinter)[nId] = outfit;
                (*g_pDoorOutfit)[nId] = kOutfitWinter;
            }
        }
        else if (g_bRainwear && (outfit == 0 || outfit == 4 || outfit == 5) && Seasons_HasRainwear(pPerson))
        {
            (*g_pOutfitBeforeWinter)[nId] = outfit;
            (*g_pDoorOutfit)[nId] = kOutfitRainwear;
        }
        return;
    }
    // stepping in: the coat comes off
    if (outfit != kOutfitWinter && outfit != kOutfitRainwear)
    {
        g_pOutfitBeforeWinter->erase(nId);
        return;
    }
    std::map<short, short>::iterator it = g_pOutfitBeforeWinter->find(nId);
    (*g_pDoorOutfit)[nId] = it != g_pOutfitBeforeWinter->end() ? it->second : kOutfitEveryday;
    if (it != g_pOutfitBeforeWinter->end())
        g_pOutfitBeforeWinter->erase(it);
}

bool Seasons_TakeDoorOutfit(cXObject *pObj, short *pOutfit)
{
    if (!g_pDoorOutfit)
        return false;
    std::map<short, short>::iterator it = g_pDoorOutfit->find(pObj->m_nObjectId);
    if (it == g_pDoorOutfit->end())
        return false;
    *pOutfit = it->second;
    g_pDoorOutfit->erase(it);
    return true;
}

// ---- raincoats (outfit 61) -------------------------------------------------------------------------------------------
// Four raincoats, real body outfits: coat 1 belted trench (adults), 2 hooded anorak (adults and children), 3 rain cape
// (adults), 4 slicker (children). Files (loose in ExpansionPack8, the skin search path):
//   suit     seasons-rain<coat>-<g><a><body>             g m / f / u (unisex), a a / c, body fit / fat / skn / chd
//   texture  <suit><tone>_<colour> or <suit>_<colour>    tone lgt / med / drk (when skin shows), colour 1..6
//            or shared by the bodies of one gender and age: seasons-rain<coat>-<g><a><tone>_<colour> / ..._<colour>
// The suit and texture are picked with the game's own escape selectors (EditPerson::SelectEscapeCharSuit /
// SelectEscapeCharTexture, as the costume trunk does): $g the gender or "u", $b the body of the Sim's everyday suit,
// $c the skin tone. Which coat: the Sim's own from a hash of the GUID, or the one it bought (person data 240), or a
// try-on over both (CostumeTrunkOffset 100 + 10 * coat + colour; TryChangeSuit keeps it when the Sim changes into 61).
// A coat that is not installed for this Sim falls back to the anorak, then any coat that fits the age; a missing colour
// to (colour - 1) % 3 + 1, then colour 1; a missing body to fit; with none at all the animator dresses everyday clothes
// and puts the Sim back in 0.
static const int kRainCoatTrench = 1, kRainCoatAnorak = 2, kRainCoatCape = 3, kRainCoatSlicker = 4;

static bool RainCoatFits(int nCoat, bool bChild)
{
    return bChild ? (nCoat == kRainCoatAnorak || nCoat == kRainCoatSlicker) : (nCoat >= kRainCoatTrench && nCoat <= kRainCoatCape);
}

// the suit for coat nCoat with this pattern tail ("$b" or a fixed body), "" when none is installed
static void RainSuit(EditPerson &edit, int nCoat, bool bChild, const char *pszBody, StackString<64> &suit)
{
    char sz[64];
    sprintf_s(sz, "seasons-rain%d-$g%s%s", nCoat, bChild ? "c" : "a", pszBody);
    suit.copy(sz);
    edit.SelectEscapeCharSuit(suit);
}

// the texture for a resolved suit: its own (with the skin tone, then without), then one shared by all bodies of that
// gender and age ("seasons-rain<coat>-<g><a>", the suit name without the body)
static bool RainTexture(EditPerson &edit, const StackString<64> &suit, int nColour, StackString<64> &tex)
{
    char szStem[2][64];
    sprintf_s(szStem[0], "%s", suit.c_str());
    sprintf_s(szStem[1], "%.16s", suit.c_str());      // "seasons-rain2-ma"
    for (int nStem = 0; nStem < 2; nStem++)
        for (int nTone = 0; nTone < 2; nTone++)
        {
            char sz[96];
            sprintf_s(sz, nTone == 0 ? "%s$c_%d" : "%s_%d", szStem[nStem], nColour);
            tex.copy(sz);
            edit.SelectEscapeCharTexture(tex);
            if (tex.length())
                return true;
        }
    return false;
}

// the coat and colour this Sim would wear before the fallbacks
static void RainwearChoice(cXPerson *pPerson, bool bChild, int &nCoat, int &nColour)
{
    short *pData = pPerson->m_PersonData;
    unsigned int h = Hash32((unsigned int)pPerson->m_pSelector->GetGUID());
    nCoat = bChild ? ((h & 1) ? kRainCoatSlicker : kRainCoatAnorak) : 1 + (int)(h % 3);
    nColour = 1 + (int)((h >> 8) % kRainColours);
    int aCode[2] = { pData[kPersonDataSeasonsRainCoat], pData[kPersonDataCostumeTrunkOffset] - 100 };
    for (int i = 0; i < 2; i++)
    {
        int c = aCode[i];
        if (c >= 10 && c <= 49 && RainCoatFits(c / 10, bChild))
        {
            nCoat = c / 10;
            if (c % 10 >= 1 && c % 10 <= kRainColours)
                nColour = c % 10;
        }
    }
}

// this coat exactly (a missing body falls back to fit / chd) in this colour exactly
static bool RainCoatSuitTex(EditPerson &edit, int nCoat, bool bChild, int nColour, StackString<64> &suit, StackString<64> &tex)
{
    RainSuit(edit, nCoat, bChild, "$b", suit);
    if (!suit.length())
        RainSuit(edit, nCoat, bChild, bChild ? "chd" : "fit", suit);
    if (!suit.length())
        return false;
    return RainTexture(edit, suit, nColour, tex);
}

// the suit and texture the Sim wears, and the coat / colour they are (after the fallbacks)
static bool RainwearResolve(cXPerson *pPerson, StringBuffer *pSuitOut, StringBuffer *pTexOut, int *pCoat, int *pColour)
{
    if (!pPerson || !pPerson->m_pSelector)
        return false;
    if (pPerson->m_PersonData[kPersonDataGender] >= 2)
        return false;                                   // pets
    bool bChild = pPerson->IsChild();
    int nCoat, nColour;
    RainwearChoice(pPerson, bChild, nCoat, nColour);
    int aOrder[5] = { nCoat, kRainCoatAnorak, kRainCoatTrench, kRainCoatCape, kRainCoatSlicker };
    int aColour[3] = { nColour, (nColour - 1) % 3 + 1, 1 };
    EditPerson edit;
    edit.Begin(pPerson->m_pSelector, 0);
    bool bFound = false;
    for (int i = 0; i < 5 && !bFound; i++)
    {
        int c = aOrder[i];
        if (!RainCoatFits(c, bChild) || (i > 0 && c == nCoat))
            continue;
        for (int k = 0; k < 3 && !bFound; k++)
        {
            StackString<64> suit;
            StackString<64> tex;
            if (!RainCoatSuitTex(edit, c, bChild, aColour[k], suit, tex))
                continue;
            if (pSuitOut)
                pSuitOut->copy(suit);
            if (pTexOut)
                pTexOut->copy(tex);
            if (pCoat)
                *pCoat = c;
            if (pColour)
                *pColour = aColour[k];
            bFound = true;
        }
    }
    edit.End(false);
    return bFound;
}

bool Seasons_GetRainwearSuitTex(cXPerson *pPerson, StringBuffer &suitOut, StringBuffer &texOut)
{
    return RainwearResolve(pPerson, &suitOut, &texOut, 0, 0);
}

// ---- the raincoat choice dialog (Hot Date's clothing dialog, clothing type 6) ----
// The preview Sim is the dialog's out-of-world copy (cWinVitaBtnSolo::SetPerson); its CostumeTrunkOffset holds the
// pair shown (100 + 10 * coat + colour), which the animator dresses like a try-on.

// every installed coat x colour pair for this Sim's age, in coat then colour order
static int RainwearPairs(cXPerson *pPerson, int *aCodes)
{
    int n = 0;
    if (!pPerson || !pPerson->m_pSelector)
        return 0;
    bool bChild = pPerson->IsChild();
    EditPerson edit;
    edit.Begin(pPerson->m_pSelector, 0);
    for (int c = kRainCoatTrench; c <= kRainCoatSlicker; c++)
    {
        if (!RainCoatFits(c, bChild))
            continue;
        for (int k = 1; k <= kRainColours; k++)
        {
            StackString<64> suit;
            StackString<64> tex;
            if (RainCoatSuitTex(edit, c, bChild, k, suit, tex))
                aCodes[n++] = 10 * c + k;
        }
    }
    edit.End(false);
    return n;
}

void Seasons_RainwearDialogBegin(cXPerson *pPreview, cXPerson *pSource)
{
    if (!pPreview)
        return;
    short *pData = pPreview->m_PersonData;
    if (pSource)
    {
        pData[kPersonDataSeasonsRainCoat] = pSource->m_PersonData[kPersonDataSeasonsRainCoat];
        pData[kPersonDataGender] = pSource->m_PersonData[kPersonDataGender];
    }
    pData[kPersonDataCostumeTrunkOffset] = 0;
    int nCoat = 0, nColour = 0;
    if (RainwearResolve(pPreview, 0, 0, &nCoat, &nColour))
        pData[kPersonDataCostumeTrunkOffset] = (short)(100 + 10 * nCoat + nColour);
}

int Seasons_RainwearDialogCode(cXPerson *pPreview)
{
    if (!pPreview)
        return 0;
    int c = pPreview->m_PersonData[kPersonDataCostumeTrunkOffset] - 100;
    return (c >= 11 && c <= 49) ? c : 0;
}

void Seasons_RainwearDialogStep(cXPerson *pPreview, int nDir)
{
    int aCodes[4 * 9];
    int n = RainwearPairs(pPreview, aCodes);
    if (n <= 0)
        return;
    int nCur = Seasons_RainwearDialogCode(pPreview);
    int i = 0;
    while (i < n && aCodes[i] != nCur)
        i++;
    if (i == n)                                         // not in the list: the first (or last) pair
        i = nDir > 0 ? n - 1 : 0;
    i = (i + (nDir > 0 ? 1 : n - 1)) % n;
    pPreview->m_PersonData[kPersonDataCostumeTrunkOffset] = (short)(100 + aCodes[i]);
}

// ---- umbrellas (person data 241; the umbrella choice dialog, clothing type 7) -----------------------------------------
// The preview Sim (the dialog's out-of-world copy) wears everyday clothes; the design shown is in its own person data
// 241, its accessory is dressed on its animator, and the right-arm carry overlay holds the umbrella up (a looping
// practice above the preview's idle motions). Design codes 1 yellow, 2 striped, 3 black, 4 navy, 5 red, 6 forest green,
// 7 white, 8 polka dot, 9 tartan, 10 floral, 11 rainbow, 12 clear bubble, 13 duck handle (the objects' STR# 304 order).
static const char *const s_apszUmbrellaSuits[kUmbrellaDesigns + 1] = {
    0,
    "suit-seasons-umbrella-yellow", "suit-seasons-umbrella-striped", "suit-seasons-umbrella",
    "suit-seasons-umbrella-navy", "suit-seasons-umbrella-red", "suit-seasons-umbrella-green", "suit-seasons-umbrella-white",
    "suit-seasons-umbrella-polka", "suit-seasons-umbrella-tartan", "suit-seasons-umbrella-floral",
    "suit-seasons-umbrella-rainbow", "suit-seasons-umbrella-bubble", "suit-seasons-umbrella-duck",
};

static bool UmbrellaInstalled(int nCode)
{
    VBAnimMgr *pMgr = Global<VBAnimMgr *>(A::g_pAnimationManager);
    if (nCode < 1 || nCode > kUmbrellaDesigns || !pMgr)
        return false;
    CTGString name(s_apszUmbrellaSuits[nCode]);
    return pMgr->FindSuit(&name) != 0;
}

static SAnimator *UmbrellaAnimator(cXPerson *pPreview) { return pPreview ? pPreview->GetSAnimator() : 0; }

// shows design nCode on the preview (0: none), taking the one shown before off
static void UmbrellaShow(cXPerson *pPreview, int nCode)
{
    SAnimator *pAnim = UmbrellaAnimator(pPreview);
    if (!pAnim)
        return;
    short *pData = pPreview->m_PersonData;
    int nOld = pData[kPersonDataSeasonsUmbrella];
    if (nOld >= 1 && nOld <= kUmbrellaDesigns)
    {
        CTGString old(s_apszUmbrellaSuits[nOld]);
        pAnim->Undress(&old);
    }
    pData[kPersonDataSeasonsUmbrella] = (short)nCode;
    if (nCode >= 1 && nCode <= kUmbrellaDesigns)
    {
        CTGString suit(s_apszUmbrellaSuits[nCode]);
        CTGString skin("");
        pAnim->Dress(&suit, &skin);
    }
}

void Seasons_UmbrellaDialogBegin(cXPerson *pPreview, cXPerson *pSource)
{
    SAnimator *pAnim = UmbrellaAnimator(pPreview);
    if (!pAnim)
        return;
    short *pData = pPreview->m_PersonData;
    int nOwned = pSource ? pSource->m_PersonData[kPersonDataSeasonsUmbrella] : 0;
    pData[kPersonDataSeasonsUmbrella] = 0;
    int nStart = 0;
    if (UmbrellaInstalled(nOwned))
        nStart = nOwned;
    for (int c = 1; c <= kUmbrellaDesigns && !nStart; c++)
        if (UmbrellaInstalled(c))
            nStart = c;
    UmbrellaShow(pPreview, nStart);
    // the umbrella held up: the game's right-arm carry overlay (a2o / c2o-rarm-carry-loop), looping above the preview's
    // own motions (they run at priority 1); it goes with the preview object
    VitaBoy *pAnimObject = pAnim->GetAnimationObject();
    if (!pAnimObject || !pAnimObject->m_pSkeleton || !pAnimObject->m_pAnimManager)
        return;
    CTGString carry(pPreview->IsChild() ? "c2o-rarm-carry-loop" : "a2o-rarm-carry-loop");
    Skill *pSkill = pAnimObject->m_pAnimManager->FindSkill(&carry);
    if (!pSkill)
        return;
    float fSpeed = 1.0f;
    float fWeight = 1.0f;
    Practice *pCarry = pAnimObject->StartPractice(pSkill, 10, &fSpeed, &fWeight, (int)Global<unsigned int>(A::g_nTickCount));
    if (pCarry)
    {
        pCarry->m_nLoopMode = kLoop_Repeat;
        pCarry->SetPriority(10);
        pAnimObject->m_pSkeleton->UpdatePriorities();
    }
}

int Seasons_UmbrellaDialogCode(cXPerson *pPreview)
{
    if (!pPreview)
        return 0;
    int c = pPreview->m_PersonData[kPersonDataSeasonsUmbrella];
    return (c >= 1 && c <= kUmbrellaDesigns) ? c : 0;
}

void Seasons_UmbrellaDialogStep(cXPerson *pPreview, int nDir)
{
    if (!pPreview)
        return;
    int nCur = Seasons_UmbrellaDialogCode(pPreview);
    int c = nCur;
    for (int i = 0; i < kUmbrellaDesigns; i++)
    {
        c = nDir > 0 ? (c % kUmbrellaDesigns) + 1 : (c <= 1 ? kUmbrellaDesigns : c - 1);
        if (UmbrellaInstalled(c))
        {
            if (c != nCur)
                UmbrellaShow(pPreview, c);
            return;
        }
    }
}

static bool Seasons_HasRainwear(cXPerson *pPerson)
{
    StackString<256> suit;
    StackString<256> tex;
    return Seasons_GetRainwearSuitTex(pPerson, suit, tex);
}

// which raincoats are installed at all (global RainwearArt), by gender and age: bit 0 men, bit 1 women, bit 2 boys,
// bit 3 girls; a unisex suit ("u") counts for both. The suit list is read once (it is fixed after start-up).
static int Seasons_RainwearArt()
{
    static int s_nArt = -1;
    if (s_nArt >= 0)
        return s_nArt;
    if (!Settings() || !Settings()->m_pSimulator)
        return 0;
    int nArt = 0;
    EditPerson edit;
    int nCount = edit.GetSuitCount();
    for (int i = 0; i < nCount; i++)
    {
        const char *psz = edit.GetSuit(i)->c_str();     // seasons-rain<coat>-<g><a>...
        if (_strnicmp(psz, "seasons-rain", 12) || psz[12] < '1' || psz[12] > '4' || psz[13] != '-' || !psz[14])
            continue;
        char g = (char)tolower((unsigned char)psz[14]);
        char a = (char)tolower((unsigned char)psz[15]);
        int nAge = a == 'a' ? 0 : (a == 'c' ? 2 : -1);
        if (nAge < 0)
            continue;
        if (g == 'm' || g == 'u')
            nArt |= 1 << nAge;
        if (g == 'f' || g == 'u')
            nArt |= 2 << nAge;
    }
    s_nArt = nArt;
    return s_nArt;
}
