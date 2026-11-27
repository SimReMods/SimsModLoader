// SeasonsCareers.cpp - the extra careers of <install>/ExpansionPack8/SeasonsCareers.iff (ids 22+), their job panel art
// and at-work portraits, and the fallback for a Sim saved in a career that no longer exists.
#include <windows.h>
#include <string.h>
#include <vector>
#include "Seasons.h"
#include "SeasonsHooks.h"

// ---- game parts only this file uses ----------------------------------------------------------------------------------

struct cJobRaw { char m_Bytes[972]; };                  // cJob (built by the exe; destroyed with its ~cJob)
class cJob
{
public:
    union
    {
        GA_F(0x054, StackString<128>, m_Name) GA_F(0x0E0, StackString<128>, m_NameAlt) GA_F(0x16C, StackString<128>, m_NameBase)
        GA_F(0x1F8, StackString<128>, m_NameBaseAlt) GA_F(0x3B4, std::string, m_Description) GA_SIZE(972)
    };
    cJob() = delete;
    ~cJob() = delete;
};
namespace ga
{
    class cCareer                                       // 140 bytes
    {
    public:
        long m_nID;                                     // +0x00
        std::vector<cJobRaw> m_Jobs;                    // +0x04
        StackString<64> m_Name;                         // +0x10
        std::string m_OfferDialogText[2];               // +0x5C (CTGString)
        cJob &Job(int j) { return *(cJob *)&m_Jobs[j]; }
        cCareer() = delete;
    };
}
static_assert(sizeof(cCareer) == 140, "cCareer");
static_assert(offsetof(cCareer, m_OfferDialogText) == 0x5C, "cCareer::m_OfferDialogText");

static std::vector<cCareer *> &Careers() { return Global<std::vector<cCareer *>>(A::g_Careers); }
static cCareer *GetCareerByID(int nId) { return Call<cCareer *>(A::GetCareerByID, nId); }

// a new cCareer from the exe's ctor (exe heap: LoadCareers / TearDownCareers delete it), and its destructor
static cCareer *NewCareer()
{
    void *p = operator new(sizeof(cCareer));
    ThisCall<void>(A::cCareer__cCareer, p);
    return (cCareer *)p;
}
static void DeleteCareer(cCareer *p)
{
    for (size_t j = 0; j < p->m_Jobs.size(); j++)
        ThisCall<void>(A::cJob___cJob, &p->m_Jobs[j]);
    p->m_Jobs.~vector();
    p->m_OfferDialogText[1].~basic_string();
    p->m_OfferDialogText[0].~basic_string();
    operator delete(p, sizeof(cCareer));
}

class StdResFile                                        // 0x244 bytes, the exe's ctor / dtor
{
public:
    union { GA_SIZE(0x244) };
    StdResFile() { ThisCall<void>(A::StdResFile__StdResFile, this); }
    ~StdResFile() { ThisCall<void>(A::StdResFile___StdResFile, this); }
    StdResFile(const StdResFile &) = delete;
    int Open(const StackString<260> &path, bool bWrite) { return ThisCall<int>(A::StdResFile__Open, this, &path, bWrite); }
    iResFile *Res() { return (iResFile *)this; }
};

class StringSet                                         // 0x6C bytes, the exe's ctor / dtor
{
public:
    union { GA_SIZE(0x6C) };
    StringSet() { ThisCall<void>(A::StringSet__StringSet, this); }
    ~StringSet() { ThisCall<void>(A::StringSet___StringSet, this); }
    StringSet(const StringSet &) = delete;
    int Load(iResFile *pRes, short nId, bool b) { return ThisCall<int>(A::StringSetBase__Load, this, pRes, (int)nId, b); }
    const char *GetString(int n, char nLang = -1) { return ThisCall<const char *>(A::StringSetBase__GetString, this, n, (int)nLang); }
};

static void BuildInstallPath(const char *pszDir, const char *pszLeaf, StringBuffer *pOut)
{
    Call<void>(A::Path_BuildInstallPath, pszDir, pszLeaf, pOut);
}

// ---- E1: the extra careers ----------------------------------------------------------------------------------------
// Every CARR chunk of ExpansionPack8/SeasonsCareers.iff whose id is > 0, not yet loaded and has exactly 10 levels
// becomes a track, appended after the Maxis ones (Work.iff / Careers.txt load stays as it was). Its texts come from the
// same file with the layout of Work.iff: STR# <id> (1 name, 2/3 offer text, 4+2j short title, 5+2j description, 24+j
// female short title) and optional STR# 1000+<id> (4+2j / 24+j full titles). The original string loop only covers the
// first 11/16/21 vector entries, which is why extra careers need this. Newspaper / computer offers, carpool, pay,
// promotion and firing are all by id and need no change.
static std::vector<long> *s_pExtraCareerIds;            // ids appended from SeasonsCareers.iff (SaveCareers skips them)

static void LoadCareerStrings(StdResFile &resFile, cCareer *pCareer)
{
    StringSet strings;
    StringSet altStrings;
    if (strings.Load(resFile.Res(), (short)pCareer->m_nID, false) != 0)
        return;
    int nAltError = altStrings.Load(resFile.Res(), (short)(pCareer->m_nID + 1000), false);
    pCareer->m_Name.copy(strings.GetString(1));
    pCareer->m_OfferDialogText[0] = strings.GetString(2);
    pCareer->m_OfferDialogText[1] = strings.GetString(3);
    for (int j = 0; j < 10; j++)
    {
        cJob &job = pCareer->Job(j);
        int nBaseId = 4 + 2 * j;
        int nAltId = 24 + j;
        job.m_NameBase.copy(strings.GetString(nBaseId));
        job.m_NameBaseAlt.copy(strings.GetString(nAltId));
        if (nAltError)
        {
            job.m_Name.copy(strings.GetString(nBaseId + 1000));
            job.m_NameAlt.copy(strings.GetString(nAltId + 1000));
        }
        else
        {
            job.m_Name.copy(altStrings.GetString(nBaseId));
            job.m_NameAlt.copy(altStrings.GetString(nAltId));
        }
        job.m_Description = strings.GetString(nBaseId + 1);
    }
}

static void LoadExtraCareers()
{
    if (!s_pExtraCareerIds)
        s_pExtraCareerIds = new std::vector<long>;
    s_pExtraCareerIds->clear();
    StackString<260> path;
    BuildInstallPath("ExpansionPack8/", "SeasonsCareers", &path);
    StdResFile resFile;
    if (resFile.Open(path, false) != 0)
        return;                                         // not installed: the Maxis careers only
    const long kCARR = 'CARR';
    int nCount = resFile.Res()->Count(kCARR);
    for (int i = 1; i <= nCount; i++)
    {
        HandleNode *h = resFile.Res()->GetByIndex(kCARR, (short)i, Va(A::ReconStream_InitFlags));
        if (!h)
            continue;
        short nId;
        resFile.Res()->GetID(h, &nId);
        resFile.Res()->Release(h);
        if (nId <= 0 || GetCareerByID(nId))
            continue;
        cCareer *pCareer = NewCareer();
        if (Call<int>(A::ReconLoadObject_cCareer, pCareer, resFile.Res(), kCARR, (int)nId, (long *)0) != 0 || pCareer->m_Jobs.size() != 10)
        {
            DeleteCareer(pCareer);
            continue;
        }
        pCareer->m_nID = nId;
        LoadCareerStrings(resFile, pCareer);
        Careers().push_back(pCareer);
        s_pExtraCareerIds->push_back(nId);
    }
    if (!s_pExtraCareerIds->empty())
        Seasons_Log("%u careers from ExpansionPack8\\SeasonsCareers.iff", (unsigned)s_pExtraCareerIds->size());
}

// LoadCareers: the extra careers after Work.iff or Careers.txt (on its success paths)
ORIG(LoadCareers, bool(__cdecl *)());
static bool __cdecl W_LoadCareers()
{
    bool r = o_LoadCareers();
    if (r)
        LoadExtraCareers();
    return r;
}

// ---- E3: SaveCareers writes the Maxis tracks only ---------------------------------------------------------------------
// While the original runs, the extra careers are out of g_Careers (they live in SeasonsCareers.iff). LoadExtraCareers
// appends them after every Maxis track, so they are the tail of the vector; they go back in the same order.
ORIG(SaveCareers, bool(__cdecl *)());
static bool __cdecl W_SaveCareers()
{
    struct Hide
    {
        std::vector<cCareer *> m_Hidden;
        Hide()
        {
            std::vector<cCareer *> &careers = Careers();
            size_t nKeep = careers.size();
            while (nKeep > 0 && s_pExtraCareerIds)
            {
                long nId = careers[nKeep - 1]->m_nID;
                bool bExtra = false;
                for (size_t i = 0; i < s_pExtraCareerIds->size(); i++)
                    bExtra |= (*s_pExtraCareerIds)[i] == nId;
                if (!bExtra)
                    break;
                nKeep--;
            }
            m_Hidden.assign(careers.begin() + nKeep, careers.end());
            careers.resize(nKeep);
        }
        ~Hide() { Careers().insert(Careers().end(), m_Hidden.begin(), m_Hidden.end()); }
    } hide;
    return o_SaveCareers();
}

// ---- E4: a Sim saved in a career that no longer exists --------------------------------------------------------------
// An id above Maxis's 1..21 (e.g. the Seasons careers after SeasonsCareers.iff was removed) would make every jobdata
// read throw SAE_CareerIdNotFound. Such a Sim becomes unemployed instead (career, level and performance 0, on the person
// and the neighbour so it sticks). Ids <= 21 keep the original behaviour.
namespace ga
{
    class Neighbor { public: union { GA_A(0x74, short, m_PersonData, 256) }; };
    class Neighborhood
    {
    public:
        Neighbor *FindNeighborBySelector(ObjSelector *p) { return ThisCall<Neighbor *>(A::Neighborhood__FindNeighborBySelector, this, p); }
    };
}
ORIG(LoadPersistentData, void(__thiscall *)(Neighborhood *, cXPerson *, const void *));
static void __fastcall W_LoadPersistentData(Neighborhood *n, void *, cXPerson *pPerson, const void *pPersons)
{
    o_LoadPersistentData(n, pPerson, pPersons);
    short nJobType = pPerson->m_PersonData[kPersonDataJobType];
    if (nJobType > 21 && !GetCareerByID(nJobType))
    {
        pPerson->m_PersonData[kPersonDataJobType] = 0;
        pPerson->m_PersonData[kPersonDataJobPromotionLevel] = 0;
        pPerson->m_PersonData[kPersonDataJobPerformance] = 0;
        if (Neighbor *pOwn = n->FindNeighborBySelector(pPerson->m_pSelector))
        {
            pOwn->m_PersonData[kPersonDataJobType] = 0;
            pOwn->m_PersonData[kPersonDataJobPromotionLevel] = 0;
            pOwn->m_PersonData[kPersonDataJobPerformance] = 0;
        }
    }
}

// ---- E2: job panel art of the extra careers ---------------------------------------------------------------------------
// The Maxis strips have exactly 22 rows (careers 0..21), so careers 22+ get no button icon and no popup picture. Two
// loose files in <install>/ExpansionPack8/, read once in cWinSubpanelJob::Init next to the Maxis images:
//   seasons-career-icons.bmp  job button: same cell as the Maxis job icon strip (4 state columns), row = career - 22
//   seasons-career-popup.bmp  popup picture: same slice as the Maxis popup composite, slice = career - 22
// A missing file, or one whose width / cell height differs from the Maxis image, is not used: those careers then look
// exactly as before (no icon). Careers 0..21 always use the Maxis images. The original TSPaint's SetRow(career id)
// works on the stacked strip as it is.

static cTSRect RectFromImageSize(const cTSBufferRef &h)
{
    cTSRect rc;
    CallRet(&rc, A::Rect_FromImageSize, &h);
    return rc;
}
static RenderDevice *RenderDeviceOfFramework()
{
    void *pGraphics = Call<void *>(A::Framework_GetGraphicSystem);
    return pGraphics ? ThisCall<RenderDevice *>(A::cTSGraphicSystemW95__GetRenderDevice, pGraphics) : 0;
}
static cRenderTargetRef CreateTarget(RenderDevice *pDevice, unsigned w, unsigned h)
{
    cRenderTargetRef r;
    CallRet(&r, A::RenderTarget_CreateShared, pDevice, w, h, pDevice->m_eColorFormat, pDevice->m_eDepthFormat, 0, 1, 0);
    return r;
}
// renders pDraw(canvas) into a new texture of w x h (cleared to transparent)
template <class F> static cTSBufferRef RenderToTexture(unsigned w, unsigned h, F pDraw)
{
    RenderDevice *pDevice = RenderDeviceOfFramework();
    if (!pDevice)
        return cTSBufferRef();
    cRenderTargetRef hTarget = CreateTarget(pDevice, w, h);
    if (!hTarget.m_p)
        return cTSBufferRef();
    float aClear[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    Call<bool>(A::RenderPass_BeginOrDefault, pDevice, &hTarget, (const void *)aClear, 1.0f);
    {
        Canvas canvas(pDevice->GetCurrentState());
        pDraw(canvas, hTarget.m_p);
    }
    Call<void>(A::RenderPass_EndOrRestoreDefault, pDevice, &hTarget, Call<int>(A::Vk_GetShaderReadOnlyLayout));
    cTSBufferRef r;
    CallRet(&r, A::RenderTarget_TakeTextureRef, pDevice, &hTarget);
    return r;
}

static cTSBufferRef *s_phCareerIcons, *s_phJobBtnStrip, *s_phCareerPopup;   // kept for the session (no exit-time release)
static int s_nCareerIconRows = 0;
static int s_nCareerPopupRows = 0;

static cTSBufferRef LoadCareerStrip(const char *pszLeaf, const cTSBufferRef &hMaxis, int nMaxisRows, int *pnRows)
{
    *pnRows = 0;
    if (!hMaxis.m_p)
        return cTSBufferRef();
    StackString<260> path;
    BuildInstallPath("ExpansionPack8/", pszLeaf, &path);
    cTSBufferRef hStrip;
    CallRet(&hStrip, A::Texture_LoadFromFileLocked, path, (const void *)Va(A::g_ImageResDescriptor));
    if (!hStrip.m_p)
        return cTSBufferRef();
    unsigned int nWidth = hMaxis.m_p->m_Tex.m_nWidth;
    unsigned int nCellH = hMaxis.m_p->m_Tex.m_nHeight / nMaxisRows;
    unsigned int nStripW = hStrip.m_p->m_Tex.m_nWidth;
    unsigned int nStripH = hStrip.m_p->m_Tex.m_nHeight;
    if (nCellH == 0 || nStripW != nWidth || nStripH == 0 || nStripH % nCellH != 0)
    {
        Seasons_Log("ExpansionPack8\\%s: %ux%u does not fit the Maxis cells (%u wide, %u high): not used", pszLeaf, nStripW,
                    nStripH, nWidth, nCellH);
        return cTSBufferRef();
    }
    *pnRows = (int)(nStripH / nCellH);
    return hStrip;
}

// hTop with hBottom below it, copied 1:1 into one texture (same width; the caller checked it). Empty on failure.
static cTSBufferRef StackStrips(const cTSBufferRef &hTop, const cTSBufferRef &hBottom)
{
    const ImageRes *pTop = hTop.m_p, *pBottom = hBottom.m_p;
    unsigned int nWidth = pTop->m_Tex.m_nWidth, nTopH = pTop->m_Tex.m_nHeight, nBottomH = pBottom->m_Tex.m_nHeight;
    return RenderToTexture(nWidth, nTopH + nBottomH, [&](Canvas &canvas, RenderTarget *) {
        Float2 uvTL = { 0.0f, 0.0f }, uvBR = { 1.0f, 1.0f };
        Float2 tl = { 0.0f, 0.0f }, br = { (float)nWidth, (float)nTopH };
        unsigned __int64 hTopSet = pTop->m_Tex.m_hSet;
        canvas.DrawTexturedQuad3(hTopSet, tl, br, 0.0f, uvTL, uvBR, 0xFFFFFFFFu);
        Float2 tl2 = { 0.0f, (float)nTopH }, br2 = { (float)nWidth, (float)(nTopH + nBottomH) };
        unsigned __int64 hBottomSet = pBottom->m_Tex.m_hSet;
        canvas.DrawTexturedQuad3(hBottomSet, tl2, br2, 0.0f, uvTL, uvBR, 0xFFFFFFFFu);
    });
}

class cFlashyBtn                                        // a cTSWinBtn
{
public:
    union { GA_F(0x128, cTSBufferRef, m_hImage) GA_F(0x13C, int, m_nRowCount) GA_F(0x140, int, m_nFrameCount) GA_F(0x184, cTSRect, m_rcFgSrc) };
    cFlashyBtn() = delete;
    void InvalidateSelf() { ThisCall<void>(A::cTSWin__InvalidateSelf, this); }
    void SetForeground(cTSBufferRef h, cTSPoint ofs) { ThisCall<void>(A::cTSWinBtn__SetForeground, this, h, ofs); }
};
class cWinSubpanelJob
{
public:
    union
    {
        GA_F(0x12C, cTSBufferRef, m_hImgSkillBarBg) GA_F(0x13C, std::vector<cFlashyBtn *>, m_vecJobBtns)
        GA_F(0x330, cTSBufferRef, m_hImgJobPopupComposite) GA_F(0x334, cTSBufferRef, m_hImgCareerIcons)
    };
    cWinSubpanelJob() = delete;
};

static void Keep(cTSBufferRef *&p, cTSBufferRef h)
{
    if (!p)
        p = new cTSBufferRef;
    *p = std::move(h);
}

// cWinSubpanelJob::Init (after the original body): our strips, checked against the Maxis images the original just
// loaded; the career button (job button 0, set up with the Maxis strip and 22 rows) gets the stacked strip with 22 + n
// rows. Same cell size, so its area is unchanged.
static void JobPanelInit(cWinSubpanelJob *pPanel)
{
    if ((s_phJobBtnStrip && s_phJobBtnStrip->m_p) || pPanel->m_vecJobBtns.empty())
        return;                                         // done already (Init runs its body once), or no buttons
    Keep(s_phCareerIcons, LoadCareerStrip("seasons-career-icons.bmp", pPanel->m_hImgCareerIcons, 22, &s_nCareerIconRows));
    Keep(s_phCareerPopup, LoadCareerStrip("seasons-career-popup.bmp", pPanel->m_hImgJobPopupComposite, 22, &s_nCareerPopupRows));
    if (!s_phCareerIcons->m_p)
        return;
    Keep(s_phJobBtnStrip, StackStrips(pPanel->m_hImgCareerIcons, *s_phCareerIcons));
    cFlashyBtn *pCareerBtn = pPanel->m_vecJobBtns[0];
    if (!s_phJobBtnStrip->m_p || !pCareerBtn || pCareerBtn->m_hImage.m_p != pPanel->m_hImgCareerIcons.m_p)
        return;
    pCareerBtn->m_hImage = *s_phJobBtnStrip;            // not SetImage: that resizes the button
    pCareerBtn->m_nRowCount = 22 + s_nCareerIconRows;
    pCareerBtn->InvalidateSelf();
}

ORIG(JobPanelInit, bool(__thiscall *)(cWinSubpanelJob *));
static bool __fastcall W_JobPanelInit(cWinSubpanelJob *p, void *)
{
    bool r = o_JobPanelInit(p);
    JobPanelInit(p);
    return r;
}

ORIG(JobPanelShutdown, bool(__thiscall *)(cWinSubpanelJob *));
static bool __fastcall W_JobPanelShutdown(cWinSubpanelJob *p, void *)
{
    for (cTSBufferRef *ph : { s_phJobBtnStrip, s_phCareerIcons, s_phCareerPopup })
        if (ph)
            ph->Reset();
    s_nCareerIconRows = 0;
    s_nCareerPopupRows = 0;
    return o_JobPanelShutdown(p);
}

// GetJobPopupComposite: the original answers careers 0..21 (and -1) and returns false for the rest; careers 22+ take
// their slice of seasons-career-popup.bmp (same slice size)
ORIG(GetJobPopupComposite, bool(__thiscall *)(cWinSubpanelJob *, unsigned, cTSRect *, cTSBufferRef *));
static bool __fastcall W_GetJobPopupComposite(cWinSubpanelJob *p, void *, unsigned nCareerID, cTSRect *pRect, cTSBufferRef *phImage)
{
    if (nCareerID == (unsigned)-1 || nCareerID <= 21)
        return o_GetJobPopupComposite(p, nCareerID, pRect, phImage);
    if (nCareerID - 22 < (unsigned)s_nCareerPopupRows && s_phCareerPopup && s_phCareerPopup->m_p)
    {
        *pRect = RectFromImageSize(*s_phCareerPopup);
        long nSliceHeight = (pRect->bottom - pRect->top) / s_nCareerPopupRows;
        pRect->top += (long)(nCareerID - 22) * nSliceHeight;
        pRect->bottom = pRect->top + nSliceHeight;
        *phImage = *s_phCareerPopup;
        return p->m_hImgSkillBarBg.m_p != 0;
    }
    return false;
}

// ---- E2: at-work portrait overlays of the extra careers -------------------------------------------------------------
// Same recipe as the Maxis ones in cWinPeople::UpdateAtWorkStatus (column 2 of the career's row, drawn at 3/4 size into
// its own texture), from seasons-career-icons.bmp; index = career id - 22. Built once the strip is loaded; stays empty
// without it (those careers then get no overlay, as before). Cleared in cWinPeople::Shutdown.
static std::vector<cTSBufferRef> *s_pAtWorkIcons;

static void BuildAtWorkCareerIcons()
{
    if (!s_pAtWorkIcons)
        s_pAtWorkIcons = new std::vector<cTSBufferRef>;
    if (!s_pAtWorkIcons->empty())
        return;
    int nRows = s_phCareerIcons && s_phCareerIcons->m_p ? s_nCareerIconRows : 0;
    if (nRows <= 0)
        return;
    cTSBufferRef hSheet = *s_phCareerIcons;
    s_pAtWorkIcons->resize(nRows);
    for (int iRow = 0; iRow < nRows; iRow++)
    {
        cTSRect rcSrc = RectFromImageSize(hSheet);
        rcSrc.right /= 4;
        rcSrc.bottom /= nRows;
        int nColumnOfsX = 2 * (rcSrc.right - rcSrc.left);
        rcSrc.left += nColumnOfsX;
        rcSrc.right += nColumnOfsX;
        int nRowOfsY = iRow * (rcSrc.bottom - rcSrc.top);
        rcSrc.top += nRowOfsY;
        rcSrc.bottom += nRowOfsY;
        const ImageRes *pSheet = hSheet.m_p;
        (*s_pAtWorkIcons)[iRow] = RenderToTexture(3 * (rcSrc.right - rcSrc.left) / 4, 3 * (rcSrc.bottom - rcSrc.top) / 4,
                                                 [&](Canvas &canvas, RenderTarget *pTarget) {
            Float2 uvBR = { (float)rcSrc.right / (float)pSheet->m_Tex.m_nWidth, (float)rcSrc.bottom / (float)pSheet->m_Tex.m_nHeight };
            Float2 uvTL = { (float)rcSrc.left / (float)pSheet->m_Tex.m_nWidth, (float)rcSrc.top / (float)pSheet->m_Tex.m_nHeight };
            Float2 br = { (float)(unsigned int)pTarget->m_nWidth, (float)(unsigned int)pTarget->m_nHeight };
            Float2 tl = { 0.0f, 0.0f };
            unsigned __int64 hSet = pSheet->m_Tex.m_hSet;
            canvas.DrawTexturedQuad3(hSet, tl, br, 0.0f, uvTL, uvBR, 0xFFFFFFFFu);
        });
    }
}

class cWinPeople
{
public:
    union { GA_F(0x148, std::vector<cFlashyBtn *>, m_SimBtns) GA_F(0x614, std::vector<cTSBufferRef>, m_CareerIcons) };
    cWinPeople() = delete;
};

// cWinPeople::UpdateAtWorkStatus (every return path): the original sets no overlay for a Sim at work in a career it has
// no icon for (id >= 22). Here those Sims get theirs, placed exactly as the original places the Maxis ones; the walk
// stops at the first empty button like the original's. Every other Sim is left as the original set it.
ORIG(UpdateAtWorkStatus, void(__thiscall *)(cWinPeople *));
static void __fastcall W_UpdateAtWorkStatus(cWinPeople *pPeople, void *)
{
    o_UpdateAtWorkStatus(pPeople);
    BuildAtWorkCareerIcons();
    if (s_pAtWorkIcons->empty() || pPeople->m_CareerIcons.empty())
        return;
    for (unsigned int iBtn = 0; iBtn < pPeople->m_SimBtns.size(); iBtn++)
    {
        int hSim = Call<int>(A::PersonFinder__GetPersonInHouseIndex, (int)iBtn);
        if (hSim == -1)
            return;
        if (!Call<bool>(A::PersonFinder__AtWork, (unsigned)hSim) || Call<int>(A::PersonFinder__GetAge, hSim) == 1)
            continue;
        cCareer *pCareer = Call<cCareer *>(A::PersonFinder__GetCareer, hSim);
        if (!pCareer || pCareer->m_nID < 22 || pCareer->m_nID - 22 >= (long)s_pAtWorkIcons->size())
            continue;
        cTSBufferRef hOverlay = (*s_pAtWorkIcons)[pCareer->m_nID - 22];
        if (!hOverlay.m_p)
            continue;
        cFlashyBtn *pBtn = pPeople->m_SimBtns[iBtn];
        cTSRect rcIcon = RectFromImageSize(pPeople->m_CareerIcons[0]);
        cTSRect rcFrame = RectFromImageSize(pBtn->m_hImage);
        rcFrame.right /= pBtn->m_nFrameCount;
        rcFrame.bottom /= pBtn->m_nRowCount;
        cTSPoint ofs = { rcFrame.right - rcIcon.right - rcFrame.left + rcIcon.left - 4,
                         rcFrame.bottom + rcIcon.top - rcFrame.top - rcIcon.bottom - 4 };
        pBtn->SetForeground(hOverlay, ofs);
        pBtn->m_rcFgSrc = rcIcon;
    }
}

ORIG(PeopleShutdown, bool(__thiscall *)(cWinPeople *));
static bool __fastcall W_PeopleShutdown(cWinPeople *p, void *)
{
    if (s_pAtWorkIcons)
        s_pAtWorkIcons->clear();
    return o_PeopleShutdown(p);
}

const Hook g_CareerHooks[] = {
    WRAP(LoadCareers,                               W_LoadCareers, o_LoadCareers),
    WRAP(SaveCareers,                               W_SaveCareers, o_SaveCareers),
    WRAP(Neighborhood__LoadPersistentData,          W_LoadPersistentData, o_LoadPersistentData),
    WRAP(cWinSubpanelJob__Init,                     W_JobPanelInit, o_JobPanelInit),
    WRAP(cWinSubpanelJob__Shutdown,                 W_JobPanelShutdown, o_JobPanelShutdown),
    WRAP(cWinSubpanelJob__GetJobPopupComposite,     W_GetJobPopupComposite, o_GetJobPopupComposite),
    WRAP(cWinPeople__UpdateAtWorkStatus,            W_UpdateAtWorkStatus, o_UpdateAtWorkStatus),
    WRAP(cWinPeople__Shutdown,                      W_PeopleShutdown, o_PeopleShutdown),
};
const size_t g_nCareerHooks = sizeof g_CareerHooks / sizeof g_CareerHooks[0];
