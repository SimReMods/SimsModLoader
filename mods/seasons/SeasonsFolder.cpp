// SeasonsFolder.cpp - Seasons' objects: <install>/ExpansionPack8/ (loose .iff or .far, any depth) is scanned after the
// official packs with its own source category, and its objects count as downloads for the sound / skin / texture /
// animation search paths.
#include <windows.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include "Seasons.h"
#include "SeasonsHooks.h"

enum { kSourceSeasonsEp8 = 4096 };
enum { kCmdObjectFolderChanged = 223 };

class ObjectFolder
{
public:
    union { GA_F(0x14, bool, m_bFlag14) GA_F(0x20, std::vector<ObjSelector *>, m_Selectors) };
    ObjectFolder() = delete;
    void ScanDirectory(StringBuffer *pPath, int nSourceCategory) { ThisCall<void>(A::ObjectFolder__ScanDirectory, this, pPath, nSourceCategory); }
    void AddAllSelectors(const StackString<260> &path, int nSourceCategory) { ThisCall<void>(A::ObjectFolder__AddAllSelectors, this, &path, nSourceCategory); }
};

static void ConvertSlashes(StringBuffer *p) { Call<void>(A::ConvertSlashes, p); }
static void ExtractDirectory(const StackString<260> &path, StackString<260> &dir) { Call<void>(A::ExtractDirectory, &path, &dir); }

static StackString<260> *s_pEp8Dir;             // empty when the folder does not exist
static StackString<260> *s_pBaseDir;            // ObjectFolder::Init's pszBaseDir (as given: not ConvertSlashes'd)
static bool s_bEp8Resolved = false;             // s_pEp8Dir looked up for s_pBaseDir

// the full source category of every selector (AddSelector keeps only its low two bits)
static std::unordered_map<const ObjSelector *, int> &SelectorSourceMap()
{
    static std::unordered_map<const ObjSelector *, int> *s_pMap = new std::unordered_map<const ObjSelector *, int>;
    return *s_pMap;
}
static int SelectorSourceCategory(const ObjSelector *pSelector)
{
    std::unordered_map<const ObjSelector *, int>::iterator it = SelectorSourceMap().find(pSelector);
    return it == SelectorSourceMap().end() ? -1 : it->second;
}

// ObjectFolder::Init: remember the base directory; the folder itself is looked up when it is first scanned
ORIG(FolderInit, void(__thiscall *)(ObjectFolder *, const char *, bool));
static void __fastcall W_FolderInit(ObjectFolder *f, void *, const char *pszBaseDir, bool bSkipDisabled)
{
    if (!s_pBaseDir)
    {
        s_pBaseDir = new StackString<260>;
        s_pEp8Dir = new StackString<260>;
    }
    s_pBaseDir->clear();
    s_pBaseDir->append(pszBaseDir, -1);
    s_pEp8Dir->clear();
    s_bEp8Resolved = false;
    o_FolderInit(f, pszBaseDir, bSkipDisabled);
}

// <base>/ExpansionPack8/ when it is a directory: mapped for the file manager (sounds, skins, ...) and kept, slashes
// converted, in s_pEp8Dir; else s_pEp8Dir stays empty. Once per Init.
static void ResolveEp8Dir()
{
    if (s_bEp8Resolved || !s_pBaseDir)
        return;
    s_bEp8Resolved = true;
    s_pEp8Dir->clear();
    StackString<260> ep8;
    ep8.append(s_pBaseDir->c_str(), -1);        // not m_BaseDir: ConvertSlashes already made that backslashed
    ep8.append("ExpansionPack8/", -1);
    // GetFileAttributesA, not stat: MSVC's stat fails on a path with a trailing slash
    DWORD nAttr = GetFileAttributesA(ep8.c_str());
    if (nAttr == INVALID_FILE_ATTRIBUTES || !(nAttr & FILE_ATTRIBUTE_DIRECTORY))
        return;
    void *pMgr = Call<void *>(A::CTGFileManager__Get);
    if (!ThisCall<bool>(A::CTGFileManager__IsDirMapped, pMgr, ep8.c_str()))
        ThisCall<bool>(A::CTGFileManager__AddDirectoryImage, pMgr, ep8.c_str(), true, true);
    ConvertSlashes(&ep8);
    s_pEp8Dir->copy(ep8);
    Seasons_Log("objects folder %s", ep8.c_str());
}

// ObjectFolder::ScanForNewFiles: the Seasons objects after every official pack, with their own source category.
// Objects added and the folder already announced (m_bFlag14): kCmdObjectFolderChanged once more (the original sent its
// own already when it found something).
ORIG(ScanForNewFiles, void(__thiscall *)(ObjectFolder *));
static void __fastcall W_ScanForNewFiles(ObjectFolder *f, void *)
{
    o_ScanForNewFiles(f);
    ResolveEp8Dir();
    if (!s_pEp8Dir || !s_pEp8Dir->length())
        return;
    Call<void>(A::SetSplashMessage, (int)'aaaa', "Scanning Seasons Objects");
    int nBefore = (int)f->m_Selectors.size();
    f->ScanDirectory(s_pEp8Dir, kSourceSeasonsEp8);
    int nAdded = (int)f->m_Selectors.size() - nBefore;
    if (nAdded)
        Seasons_Log("%d objects from ExpansionPack8", nAdded);
    if (nAdded && f->m_bFlag14)
        GlobalDispatch(kCmdObjectFolderChanged, 0);
}

// ObjectFolder::NewFileAdded: a file in the ExpansionPack8 folder (which the original does not know and ignores) is
// added with its source category, as the original adds one of a pack folder
ORIG(NewFileAdded, void(__thiscall *)(ObjectFolder *, StackString<260> *));
static void __fastcall W_NewFileAdded(ObjectFolder *f, void *, StackString<260> *pFilePath)
{
    if (s_pEp8Dir && s_pEp8Dir->length())
    {
        StackString<260> baseDir;
        ExtractDirectory(*pFilePath, baseDir);
        ConvertSlashes(&baseDir);
        if (baseDir.compareNoCase(*s_pEp8Dir) == 0)
        {
            int nBefore = (int)f->m_Selectors.size();
            StackString<260> normalized(*pFilePath);
            ConvertSlashes(&normalized);
            f->AddAllSelectors(normalized, kSourceSeasonsEp8);
            if (nBefore != (int)f->m_Selectors.size() && f->m_bFlag14)
                GlobalDispatch(kCmdObjectFolderChanged, 0);
            return;
        }
    }
    o_NewFileAdded(f, pFilePath);
}

// ObjectFolder::GetDownloadDirectories: Seasons objects count as downloads here, so their folders join the sound,
// skin, texture and animation search paths like Downloads (same walk as the original, for our objects)
ORIG(GetDownloadDirectories, void(__thiscall *)(ObjectFolder *, std::vector<StackString<260>> *));
static void __fastcall W_GetDownloadDirectories(ObjectFolder *f, void *, std::vector<StackString<260>> *pDirs)
{
    o_GetDownloadDirectories(f, pDirs);
    std::vector<ObjSelector *> &sel = f->m_Selectors;
    std::vector<StackString<260>> &dirs = *pDirs;
    for (std::vector<ObjSelector *>::iterator it = sel.begin(); it != sel.end(); ++it)
    {
        std::vector<ObjSelector *>::iterator itNext = it + 1;
        if (SelectorSourceCategory(*it) != kSourceSeasonsEp8 || (*it)->Kind() == 2)
            continue;                                   // not ours, or the original took it already
        if (itNext != sel.end() && (*itNext)->m_pResFile == (*it)->m_pResFile)
            continue;
        StackString<260> filePath;
        StackString<260> dir;
        (*it)->m_pResFile->GetFileName(filePath);
        ExtractDirectory(filePath, dir);
        std::vector<StackString<260>>::iterator itDir;
        for (itDir = dirs.begin(); itDir != dirs.end(); ++itDir)
            if (!itDir->compareNoCase(dir))
                break;
        if (itDir == dirs.end())
            dirs.push_back(dir);
    }
}

// ObjectFolder::AddSelector: the full source category of a selector it added (it keeps the low bits only)
ORIG(AddSelector, void(__thiscall *)(ObjectFolder *, iResFile *, short, int));
static void __fastcall W_AddSelector(ObjectFolder *f, void *, iResFile *pResFile, short nObjdResId, int nSourceCategory)
{
    size_t nBefore = f->m_Selectors.size();
    o_AddSelector(f, pResFile, nObjdResId, nSourceCategory);
    if (f->m_Selectors.size() > nBefore)
        SelectorSourceMap()[f->m_Selectors.back()] = nSourceCategory;
}

const Hook g_FolderHooks[] = {
    WRAP(ObjectFolder__Init,                        W_FolderInit, o_FolderInit),
    WRAP(ObjectFolder__ScanForNewFiles,             W_ScanForNewFiles, o_ScanForNewFiles),
    WRAP(ObjectFolder__NewFileAdded,                W_NewFileAdded, o_NewFileAdded),
    WRAP(ObjectFolder__GetDownloadDirectories,      W_GetDownloadDirectories, o_GetDownloadDirectories),
    WRAP(ObjectFolder__AddSelector,                 W_AddSelector, o_AddSelector),
};
const size_t g_nFolderHooks = sizeof g_FolderHooks / sizeof g_FolderHooks[0];
