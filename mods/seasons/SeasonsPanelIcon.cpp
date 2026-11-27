// SeasonsPanelIcon.cpp - the season icon in the control panel puck, at the right end of the funds / friend count slot
// (cWinViewControl).
//
// Layout: the icon (11x11, the size of the friend-count smiley in the panel art) takes the right end of the funds slot,
// and the funds and friend count buttons move left by the icon's width plus a 3 px gap. Both are right-aligned captions,
// so the shift is measured from the real button rects and the captions' real widths (every language font). If the
// friend count would run into the smiley, the icon is not shown at all. Shown on lots whose ground follows the seasons,
// while the funds are shown. Hidden, the funds and friend count are back at their original places.
// Art: drawn procedurally (sun, leaf, snowflake, blossom; no Maxis pixels), premultiplied alpha. Tooltip: the season's
// name (EN, PL, DE, FR, ES, IT, NL; English otherwise). A click shows the calendar.
//
// The icon window is a cTSWin built by the exe's own constructor; its vtable is a copy of cTSWin's with TSPaint and
// TSOnMouseDownL replaced, and the icon's state follows the cTSWin part in the same block.
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Seasons.h"
#include "SeasonsHooks.h"

// ---- the game's window classes (cTSWin 296 bytes, its vtable 98 slots) -----------------------------------------------

namespace ga
{
    class cITSFont
    {
    public:
        long TSCharHeight() { return VCall<long>(this, 22); }
        long GetStringWidth(const char *p, unsigned long n) { return VCall<long>(this, 26, p, n); }
        void GetCharDims(unsigned long ch, cTSRect *pOut) { VCall<void>(this, 29, ch, pOut); }
    };
    class cTSWin
    {
    public:
        enum { kSize = 0x128, kSlotCount = 98, kSlotChildAdd = 8, kSlotSetArea = 24, kSlotGetFlag = 35, kSlotShowWindow = 37,
               kSlotHideWindow = 38, kSlotTSOnMouseDownL = 55, kSlotTSPaint = 87, kSlotInvalidateSelf = 88 };
        enum { kWinFlag_Visible = 0x1 };
        union
        {
            GA_F0(void **, m_vptr) GA_F(0x08, unsigned long, m_nWinId) GA_F(0x1C, cTSRect, m_rcDraw)
            GA_F(0x50, std::string, m_strCaption) GA_F(0xD4, cTSRect, m_rcArea)
        };
        cTSWin() = delete;
        cTSWin *GetChildWindowFromID(unsigned long n) { return ThisCall<cTSWin *>(A::cTSWin__GetChildWindowFromID, this, n); }
        bool ChildAdd(cTSWin *p) { return VCall<bool>(this, kSlotChildAdd, p); }
        void SetArea(long l, long t, long r, long b) { VCall<void>(this, kSlotSetArea, l, t, r, b); }
        bool GetFlag(int n) { return VCall<bool>(this, kSlotGetFlag, n); }
        void ShowWindow() { VCall<void>(this, kSlotShowWindow); }
        void HideWindow() { VCall<void>(this, kSlotHideWindow); }
        void InvalidateSelf() { VCall<void>(this, kSlotInvalidateSelf); }
        void SetToolTipsText(const cTSString &s) { ThisCall<void>(A::cTSWin__SetToolTipsText, this, &s); }
        void TSWinMoveTo(const cTSPoint &pt) { ThisCall<void>(A::cTSWin__TSWinMoveTo, this, &pt); }
    };
    class cTSWinBtn : public cTSWin
    {
    public:
        cITSFont *GetFont() { return ThisCall<cITSFont *>(A::cTSWinBtn__GetFont, this); }
    };
}

// ---- the four icons, drawn procedurally: 11x11 each, B8G8R8A8 -------------------------------------------------------
static const int kSeasonIconSize = 11;

static float IconClamp01(float f) { return f < 0.0f ? 0.0f : (f > 1.0f ? 1.0f : f); }

// distance from p to the segment a-b
static float IconSegDist(float px, float py, float ax, float ay, float bx, float by)
{
    float vx = bx - ax, vy = by - ay, wx = px - ax, wy = py - ay;
    float t = IconClamp01((wx * vx + wy * vy) / (vx * vx + vy * vy));
    float dx = wx - vx * t, dy = wy - vy * t;
    return sqrtf(dx * dx + dy * dy);
}

// each icon is a silhouette in the panel's text green (cWinViewControl's funds / friend digits, RGB 41, 230, 94), with
// inner details cut out as holes (leaf vein, blossom heart) so the shapes still read at 11 px. Coverage 0/1 per sample.
static const float kIconTextR = 41.0f / 255.0f, kIconTextG = 230.0f / 255.0f, kIconTextB = 94.0f / 255.0f;

static bool SeasonIconMask(int nSeason, float x, float y)
{
    const float cx = 5.5f, cy = 5.5f;
    float dx = x - cx, dy = y - cy;
    float d = sqrtf(dx * dx + dy * dy);
    switch (nSeason)
    {
    case 0:     // summer: a sun, a disc and eight rays with a clear gap between them
    {
        if (d < 2.7f)
            return true;
        for (int i = 0; i < 8; i++)
        {
            float a = (float)i * 0.785398f;
            float ux = cosf(a), uy = sinf(a);
            float fLen = (i & 1) ? 4.7f : 5.4f;
            if (IconSegDist(x, y, cx + ux * 3.8f, cy + uy * 3.8f, cx + ux * fLen, cy + uy * fLen) < 0.6f)
                return true;
        }
        return false;
    }
    case 1:     // autumn: a leaf with its vein cut out, and the stem
    {
        const float ax = 2.6f, ay = 8.4f, bx = 9.4f, by = 1.6f;
        if (IconSegDist(x, y, ax, ay, 1.0f, 10.2f) < 0.6f)
            return true;
        float mx = (ax + bx) * 0.5f, my = (ay + by) * 0.5f;
        float lx = bx - ax, ly = by - ay;
        float fL = sqrtf(lx * lx + ly * ly);
        float ux = lx / fL, uy = ly / fL, nx = -uy, ny = ux;
        float h = fL * 0.5f, w = 3.0f;
        float r0 = (h * h - w * w) / (2.0f * w), R = r0 + w;
        float px = x - mx, py = y - my;
        float sl = px * ux + py * uy, t = px * nx + py * ny;
        float d1 = sqrtf(sl * sl + (t - r0) * (t - r0)), d2 = sqrtf(sl * sl + (t + r0) * (t + r0));
        if (!(d1 < R && d2 < R))
            return false;
        if (fabsf(t) < 0.4f && sl > -h + 1.0f && sl < h - 1.6f)
            return false;                                                   // the vein
        return true;
    }
    case 2:     // winter: a six-armed snowflake
    {
        for (int i = 0; i < 6; i++)
        {
            float a = -1.570796f + (float)i * 1.047198f;
            float ux = cosf(a), uy = sinf(a);
            if (IconSegDist(x, y, cx, cy, cx + ux * 5.1f, cy + uy * 5.1f) < 0.55f)
                return true;
            float bx0 = cx + ux * 3.0f, by0 = cy + uy * 3.0f;
            for (int sgn = -1; sgn <= 1; sgn += 2)
            {
                float b = a + (float)sgn * 0.85f;
                if (IconSegDist(x, y, bx0, by0, bx0 + cosf(b) * 1.7f, by0 + sinf(b) * 1.7f) < 0.5f)
                    return true;
            }
        }
        return false;
    }
    case 3:     // spring: a five-petal blossom with its heart cut out
    {
        if (d < 1.0f)
            return false;
        float fAng = atan2f(dy, dx);
        for (int i = 0; i < 5; i++)                                         // thin notches between the petals
        {
            float m = -1.570796f + ((float)i + 0.5f) * 1.256637f;
            float dd = fabsf(fmodf(fAng - m + 3.141593f + 6.283185f, 6.283185f) - 3.141593f);
            if (dd * d < 0.45f && d > 1.5f)
                return false;
        }
        for (int i = 0; i < 5; i++)
        {
            float a = -1.570796f + (float)i * 1.256637f;
            float px = cx + cosf(a) * 3.1f, py = cy + sinf(a) * 3.1f;
            if (sqrtf((x - px) * (x - px) + (y - py) * (y - py)) < 1.95f)
                return true;
        }
        return false;
    }
    }
    return false;
}

// the icon of nSeason, 4x4 supersampled, into pOut (rows nStride pixels apart), B8G8R8A8 with premultiplied colour
static void PaintSeasonIcon(int nSeason, unsigned int *pOut, int nStride)
{
    const int kSub = 4;
    const float kInv = 1.0f / (float)(kSub * kSub);
    for (int y = 0; y < kSeasonIconSize; y++)
        for (int x = 0; x < kSeasonIconSize; x++)
        {
            float a = 0;
            for (int sy = 0; sy < kSub; sy++)
                for (int sx = 0; sx < kSub; sx++)
                    if (SeasonIconMask(nSeason, (float)x + ((float)sx + 0.5f) / kSub, (float)y + ((float)sy + 0.5f) / kSub))
                        a += 1.0f;
            a = IconClamp01(a * kInv);
            unsigned int nA = (unsigned int)(a * 255.0f + 0.5f);
            unsigned int nR = (unsigned int)(kIconTextR * a * 255.0f + 0.5f);
            unsigned int nG = (unsigned int)(kIconTextG * a * 255.0f + 0.5f);
            unsigned int nB = (unsigned int)(kIconTextB * a * 255.0f + 0.5f);
            pOut[y * nStride + x] = (nA << 24) | (nR << 16) | (nG << 8) | nB;
        }
}

// ---- the strip texture: the four icons side by side, in eSeason order ------------------------------------------------

static cTSBufferRef *g_phSeasonIcons;
static bool g_bSeasonIconsTried;
static unsigned int s_aIconPixels[kSeasonIconSize * kSeasonIconSize * kSeasonCount];

static ImageRes *SeasonIconStrip()
{
    if (!g_bSeasonIconsTried)
    {
        g_bSeasonIconsTried = true;
        for (int n = 0; n < kSeasonCount; n++)
            PaintSeasonIcon(n, s_aIconPixels + n * kSeasonIconSize, kSeasonIconSize * kSeasonCount);
        ImageDataSrc src = {};
        src.m_nKind = 1;
        src.m_nWidth = kSeasonIconSize * kSeasonCount;
        src.m_nHeight = kSeasonIconSize;
        src.m_nRowPitch = kSeasonIconSize * kSeasonCount * 4;
        src.m_nFormat = 44;                     // VK_FORMAT_B8G8R8A8_UNORM
        src.m_fnDecode = [](const ImageDataSrc &, std::byte *pDst, const TexLoadOptions &)
        {
            memcpy(pDst, s_aIconPixels, sizeof(s_aIconPixels));
        };
        TexLoadOptions opts;
        opts.m_nSamplerIndex = 0;               // nearest: drawn 1:1
        g_phSeasonIcons = new cTSBufferRef(Texture_LoadWithDecoder(Graphics_GetCurrentTarget(), &src, &opts));
    }
    return g_phSeasonIcons ? g_phSeasonIcons->Get() : 0;
}

// ---- the localised season names (tooltip), in each language's code page ---------------------------------------------

static const char *SeasonDisplayName(int nSeason)
{
    // eSeason order: summer, autumn, winter, spring
    static const char *s_aEN[4] = { "Summer", "Autumn", "Winter", "Spring" };
    static const char *s_aPL[4] = { "Lato", "Jesie\xF1", "Zima", "Wiosna" };                        // windows-1250
    static const char *s_aDE[4] = { "Sommer", "Herbst", "Winter", "Fr\xFChling" };                  // windows-1252
    static const char *s_aFR[4] = { "\xC9t\xE9", "Automne", "Hiver", "Printemps" };
    static const char *s_aES[4] = { "Verano", "Oto\xF1o", "Invierno", "Primavera" };
    static const char *s_aIT[4] = { "Estate", "Autunno", "Inverno", "Primavera" };
    static const char *s_aNL[4] = { "Zomer", "Herfst", "Winter", "Lente" };
    if (nSeason < 0 || nSeason >= kSeasonCount)
        return "";
    const char **ppNames = s_aEN;
    switch (GetLanguage())
    {
    case kLang_Polish:  ppNames = s_aPL; break;
    case kLang_German:  ppNames = s_aDE; break;
    case kLang_French:  ppNames = s_aFR; break;
    case kLang_Spanish: ppNames = s_aES; break;
    case kLang_Italian: ppNames = s_aIT; break;
    case kLang_Dutch:   ppNames = s_aNL; break;
    default: break;
    }
    return ppNames[nSeason];
}

// ---- the icon window -------------------------------------------------------------------------------------------------

static const unsigned long kSeasonIconWinId = 0x5EA50001;
static const int kSeasonIconGap = 3;            // px between the funds text and the icon
static const int kSmileyRight = 79;             // the friend-count smiley of the panel art ends at x 77 (+ 2 px air)

struct SeasonIconState
{
    int m_nSeason;                              // shown season, -1 = hidden
    cTSPoint m_ptFunds, m_ptFriends;            // the buttons' original places (parent coordinates)
    long m_nShift;                              // how far they move left while the icon shows
};
static SeasonIconState &IconState(cTSWin *pWin) { return *(SeasonIconState *)((char *)pWin + cTSWin::kSize); }

static void ShowSeasonCalendar();

static bool __fastcall Icon_TSOnMouseDownL(cTSWin *pWin, void *, long, long, unsigned long)
{
    if (IconState(pWin).m_nSeason >= 0)         // a click shows the calendar
        ShowSeasonCalendar();
    return true;
}

static bool __fastcall Icon_TSPaint(cTSWin *pWin, void *, bool)
{
    int nSeason = IconState(pWin).m_nSeason;
    if (nSeason < 0)
        return true;
    ImageRes *pStrip = SeasonIconStrip();
    if (!pStrip || !pStrip->m_Tex.m_hSet)
        return true;
    Canvas canvas(Graphics_GetCurrentTarget()->GetCurrentState());
    float fW = (float)pStrip->m_Tex.m_nWidth;
    Float2 tl = { (float)pWin->m_rcDraw.left, (float)pWin->m_rcDraw.top };
    Float2 br = { tl.x + (float)kSeasonIconSize, tl.y + (float)kSeasonIconSize };
    Float2 uvTL = { (float)(nSeason * kSeasonIconSize) / fW, 0.0f };
    Float2 uvBR = { (float)((nSeason + 1) * kSeasonIconSize) / fW, 1.0f };
    unsigned __int64 hSet = pStrip->m_Tex.m_hSet;
    canvas.DrawTexturedQuad2(hSet, tl, br, 0.0f, uvTL, uvBR, 0xFFFFFFFFu);
    return true;
}

// a new icon window: cTSWin's constructor, then our vtable (cTSWin's, with TSPaint and TSOnMouseDownL replaced; the
// RTTI pointer before it copied too)
static void *s_aIconVtbl[1 + cTSWin::kSlotCount];
static cTSWin *NewIconWin()
{
    void *p = operator new(cTSWin::kSize + sizeof(SeasonIconState));
    ThisCall<void>(A::cTSWin__cTSWin, p);
    cTSWin *pWin = (cTSWin *)p;
    if (!s_aIconVtbl[1])
    {
        memcpy(s_aIconVtbl, pWin->m_vptr - 1, sizeof s_aIconVtbl);
        s_aIconVtbl[1 + cTSWin::kSlotTSOnMouseDownL] = (void *)&Icon_TSOnMouseDownL;
        s_aIconVtbl[1 + cTSWin::kSlotTSPaint] = (void *)&Icon_TSPaint;
    }
    pWin->m_vptr = s_aIconVtbl + 1;
    SeasonIconState &s = IconState(pWin);
    s.m_nSeason = -1;
    s.m_ptFunds = s.m_ptFriends = cTSPoint{ 0, 0 };
    s.m_nShift = 0;
    return pWin;
}

void Seasons_PanelIconInit(cTSWin *pPanel, cTSWinBtn *pFunds, cTSWinBtn *pFriends)
{
    if (!pPanel || !pFunds || !pFriends || pPanel->GetChildWindowFromID(kSeasonIconWinId))
        return;
    cTSWin *pIcon = NewIconWin();
    SeasonIconState &s = IconState(pIcon);
    pIcon->m_nWinId = kSeasonIconWinId;
    s.m_ptFunds = cTSPoint{ pFunds->m_rcArea.left, pFunds->m_rcArea.top };
    s.m_ptFriends = cTSPoint{ pFriends->m_rcArea.left, pFriends->m_rcArea.top };
    s.m_nShift = kSeasonIconSize + kSeasonIconGap;
    // vertically: centred on the digits ('0' glyph box of the caption font; the cell when the font gives no offset)
    long nTop = pFunds->m_rcArea.top, nBottom = pFunds->m_rcArea.bottom;
    if (cITSFont *pFont = pFunds->GetFont())
    {
        cTSRect rc = {};
        pFont->GetCharDims('0', &rc);
        if (rc.bottom > rc.top && rc.bottom <= pFont->TSCharHeight() + 4)
        {
            nTop = pFunds->m_rcArea.top + rc.top;
            nBottom = pFunds->m_rcArea.top + rc.bottom;
        }
    }
    long nIconTop = (nTop + nBottom - kSeasonIconSize) / 2;
    if (nIconTop < pFunds->m_rcArea.top)
        nIconTop = pFunds->m_rcArea.top;
    long nIconLeft = pFunds->m_rcArea.right - kSeasonIconSize;
    pPanel->ChildAdd(pIcon);
    pIcon->SetArea(nIconLeft, nIconTop, nIconLeft + kSeasonIconSize, nIconTop + kSeasonIconSize);
    pIcon->HideWindow();
}

// The icon sits at the right end of the funds slot; the funds text (right-aligned) moves left by m_nShift. The friend
// count (also right-aligned, its box sized for "999") moves left only as far as the funds text needs, and the icon shows
// only when the friend count's actual digits then still clear the smiley. Decided on the captions' real widths, at
// every update.
static long CaptionWidth(cTSWinBtn *pBtn)
{
    cITSFont *pFont = pBtn->GetFont();
    if (!pFont || pBtn->m_strCaption.size() == 0)
        return 0;
    return pFont->GetStringWidth(pBtn->m_strCaption.c_str(), (unsigned long)pBtn->m_strCaption.size());
}

void Seasons_PanelIconUpdate(cTSWin *pPanel, cTSWinBtn *pFunds, cTSWinBtn *pFriends)
{
    if (!pPanel || !pFunds || !pFriends)
        return;
    cTSWin *pIcon = pPanel->GetChildWindowFromID(kSeasonIconWinId);
    if (!pIcon || pIcon->m_vptr != s_aIconVtbl + 1)
        return;
    SeasonIconState &s = IconState(pIcon);
    int nSeason = -1;
    long nFriendsX = s.m_ptFriends.x;               // where the friend count goes while the icon shows
    long nFundsW = pFunds->m_rcArea.right - pFunds->m_rcArea.left;
    long nFriendsW = pFriends->m_rcArea.right - pFriends->m_rcArea.left;
    if (pFunds->GetFlag(cTSWin::kWinFlag_Visible) && pFunds->m_strCaption.size() > 0 && Seasons_LotFollowsSeasons())
    {
        long nFundsTextLeft = s.m_ptFunds.x + nFundsW - s.m_nShift - CaptionWidth(pFunds);
        long nFriendsRight = s.m_ptFriends.x + nFriendsW;
        if (nFriendsRight > nFundsTextLeft - kSeasonIconGap)
            nFriendsRight = nFundsTextLeft - kSeasonIconGap;
        if (nFriendsRight - CaptionWidth(pFriends) >= kSmileyRight)
        {
            nFriendsX = nFriendsRight - nFriendsW;
            nSeason = Seasons_GetSeason();
            if (nSeason < 0 || nSeason >= kSeasonCount)
                nSeason = -1;
        }
    }
    cTSPoint ptFunds = s.m_ptFunds;
    cTSPoint ptFriends = s.m_ptFriends;
    if (nSeason >= 0)
    {
        ptFunds.x -= s.m_nShift;
        ptFriends.x = nFriendsX;
    }
    bool bMoved = false;
    if (pFunds->m_rcArea.left != ptFunds.x || pFunds->m_rcArea.top != ptFunds.y)
    {
        pFunds->TSWinMoveTo(ptFunds);
        bMoved = true;
    }
    if (pFriends->m_rcArea.left != ptFriends.x || pFriends->m_rcArea.top != ptFriends.y)
    {
        pFriends->TSWinMoveTo(ptFriends);
        bMoved = true;
    }
    if (nSeason == s.m_nSeason && !bMoved)
        return;
    bool bWasShown = s.m_nSeason >= 0;
    s.m_nSeason = nSeason;
    if (nSeason >= 0)
    {
        if (!bWasShown)
            pIcon->ShowWindow();
        pIcon->SetToolTipsText(cTSString(SeasonDisplayName(nSeason)));
        pIcon->InvalidateSelf();
    }
    else if (bWasShown)
        pIcon->HideWindow();
    pPanel->InvalidateSelf();
}

// ---- the calendar box (a click on the icon) ------------------------------------------------------------------------
// EN + PL (windows-1250 for PL, as the tooltip names); other languages read English.
static void ShowSeasonCalendar()
{
    SeasonsCalendar cal;
    bool bCtl = Seasons_GetCalendar(&cal);
    bool bPL = GetLanguage() == kLang_Polish;
    int nNext = (cal.nSeason + 1) % kSeasonCount;
    static const char *s_aHolEN[4] = { "summer holidays", "autumn holidays", "winter holidays", "spring holidays" };
    static const char *s_aHolPL[4] = { "wakacje", "przerwa jesienna", "przerwa \x9Cwi\xB9teczna", "przerwa wiosenna" };
    static const char *s_aHolPLcap[4] = { "Wakacje", "Przerwa jesienna", "Przerwa \x9Cwi\xB9teczna", "Przerwa wiosenna" };
    static const char *s_aSeasonPLlow[4] = { "lato", "jesie\xF1", "zima", "wiosna" };
    char szLine1[160], szLine2[200], szLine3[160];
    szLine2[0] = 0;
    if (bPL)
    {
        sprintf_s(szLine1, "%s, dzie\xF1 %d z %d.%s", SeasonDisplayName(cal.nSeason), cal.nDayInSeason, cal.nLen,
                  cal.bForced ? " (pora roku ustawiona na termostacie)" : "");
        if (!bCtl)
            sprintf_s(szLine2, "Bez sterownika p\xF3r roku nie ma przerw w szkole i pracy.");
        else if (cal.bHoliday)
            sprintf_s(szLine2, "Dzi\x9C %s: nie ma szko\xB3y ani pracy.", s_aHolPL[cal.nSeason]);
        else if (cal.nDaysToHoliday == 1)
            sprintf_s(szLine2, "Jutro %s: nie b\xEA" "dzie szko\xB3y ani pracy.", s_aHolPL[cal.nSeason]);
        else if (cal.nDaysToHoliday > 1)
            sprintf_s(szLine2, "%s za %d dni.", s_aHolPLcap[cal.nSeason], cal.nDaysToHoliday);
        else
            sprintf_s(szLine2, "W tej porze roku nie ma ju\xBF przerwy.");
        if (cal.bForced)
            szLine3[0] = 0;
        else if (cal.nDaysToNextSeason == 1)
            sprintf_s(szLine3, "Jutro zaczyna si\xEA %s.", s_aSeasonPLlow[nNext]);
        else
            sprintf_s(szLine3, "Do ko\xF1" "ca pory roku: %d dni (potem %s).", cal.nDaysToNextSeason, s_aSeasonPLlow[nNext]);
    }
    else
    {
        static const char *s_aEN[4] = { "Summer", "Autumn", "Winter", "Spring" };
        sprintf_s(szLine1, "%s, day %d of %d.%s", s_aEN[cal.nSeason], cal.nDayInSeason, cal.nLen,
                  cal.bForced ? " (season set on the thermostat)" : "");
        if (!bCtl)
            sprintf_s(szLine2, "Without a Seasons Controller there are no school or work holidays.");
        else if (cal.bHoliday)
            sprintf_s(szLine2, "It's the %s today: no school or work.", s_aHolEN[cal.nSeason]);
        else if (cal.nDaysToHoliday == 1)
            sprintf_s(szLine2, "The %s start tomorrow: no school or work.", s_aHolEN[cal.nSeason]);
        else if (cal.nDaysToHoliday > 1)
            sprintf_s(szLine2, "The %s start in %d days.", s_aHolEN[cal.nSeason], cal.nDaysToHoliday);
        else
            sprintf_s(szLine2, "No more holidays this season.");
        if (cal.bForced)
            szLine3[0] = 0;
        else if (cal.nDaysToNextSeason == 1)
            sprintf_s(szLine3, "%s begins tomorrow.", s_aEN[nNext]);
        else
            sprintf_s(szLine3, "%s begins in %d days.", s_aEN[nNext], cal.nDaysToNextSeason);
    }
    char szText[600];
    sprintf_s(szText, "%s\r\n%s%s%s", szLine1, szLine2, szLine3[0] ? "\r\n" : "", szLine3);
    SimsApp()->MessageDialog(szText, bPL ? "Kalendarz" : "Calendar", kMsgSet_Ok, false);
}

// ---- hooks -------------------------------------------------------------------------------------------------------

class cWinViewControl
{
public:
    union { GA_F(0x168, cTSWinBtn *, m_pFundsBtn) GA_F(0x25C, cTSWinBtn *, m_pFriendCountBtn) };
    cWinViewControl() = delete;
};

// cWinViewControl::Init (every return): the season icon right of the funds, hidden until UpdateViewFromCPState shows
// it (nothing without the funds / friend-count buttons, or when the icon exists already)
ORIG(ViewControlInit, bool(__thiscall *)(cWinViewControl *));
static bool __fastcall W_ViewControlInit(cWinViewControl *p, void *)
{
    bool r = o_ViewControlInit(p);
    Seasons_PanelIconInit((cTSWin *)p, p->m_pFundsBtn, p->m_pFriendCountBtn);
    return r;
}

// cWinViewControl::UpdateViewFromCPState (at its end): show / hide / change the season icon
ORIG(UpdateViewFromCPState, void(__thiscall *)(cWinViewControl *, void *));
static void __fastcall W_UpdateViewFromCPState(cWinViewControl *p, void *, void *pState)
{
    o_UpdateViewFromCPState(p, pState);
    Seasons_PanelIconUpdate((cTSWin *)p, p->m_pFundsBtn, p->m_pFriendCountBtn);
}

const Hook g_PanelHooks[] = {
    WRAP(cWinViewControl__Init,                     W_ViewControlInit, o_ViewControlInit),
    WRAP(cWinViewControl__UpdateViewFromCPState,    W_UpdateViewFromCPState, o_UpdateViewFromCPState),
};
const size_t g_nPanelHooks = sizeof g_PanelHooks / sizeof g_PanelHooks[0];
