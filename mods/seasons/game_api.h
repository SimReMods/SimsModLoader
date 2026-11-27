// game_api.h - the part of Sims.exe (Legacy Collection, Steam) the seasons mod touches: class layouts with explicit
// offsets (only the fields the mod uses) and the game's functions called through address thunks. Names are the game's
// (RTTI); addresses are in game_addr.h.
//
// Memory: the mod's operator new / delete are the exe's (seasons.cpp), so the std containers and strings of this DLL
// and of the exe share one heap, and the exe's std::vector / std::string / std::map (the same MSVC STL, release
// layout) are used here as the real types: the mod may push into a game vector and the game may free it later.
//
// GAME_CC=1 builds for the Complete Collection (2003, VC6 + SGI STL): game_addr_cc.h, and GA_O(legacy, cc) picks the
// 2003 offset of a field where the class layout differs (VC6 string 12 bytes, members only Legacy has). The 2003
// containers are not this DLL's: a vector is laid out the same (three pointers) but grows through the SGI pool
// allocator, a string is 12 bytes (start, finish, end of storage). So under GAME_CC the game's vectors are GVec (read
// only; the mod grows one only through the exe's own push_back), its strings GameString (built, assigned and freed by
// the exe's own string functions), and the objects the game deletes are allocated with the exe's operator new.
#pragma once
#include <stddef.h>
#include <string.h>
#include <string>
#include <vector>
#include <functional>
#include <cstddef>

#ifndef GAME_CC
#define GAME_CC 0
#endif
// GAME_EA=1: the Legacy Collection from the EA app - the Steam code relinked at other addresses (game_addr_ea.h is
// generated from game_addr.h by tools/port_addrs.py); class layouts are the same.
#ifndef GAME_EA
#define GAME_EA 0
#endif
#if GAME_CC
#define GA_ADDR_FILE "game_addr_cc.h"
#define GA_O(legacy, cc) (cc)
#elif GAME_EA
#define GA_ADDR_FILE "game_addr_ea.h"
#define GA_O(legacy, cc) (legacy)
#else
#define GA_ADDR_FILE "game_addr.h"
#define GA_O(legacy, cc) (legacy)
#endif

#pragma warning(push)
#pragma warning(disable : 4201)     // anonymous structs: the fields below are union members at explicit offsets

// a field `name` of `type` at byte offset `off`, placed inside a `union { ... };`
#define GA_F(off, type, name) struct { char _o_##name[off]; type name; };
#define GA_F0(type, name) struct { type name; };
#define GA_A(off, type, name, n) struct { char _o_##name[off]; type name[n]; };
#define GA_A0(type, name, n) struct { type name[n]; };
#define GA_SIZE(n) char _ga_size[n];

namespace ga
{
    // addresses (link-time VAs, see game_addr.h)
    namespace A
    {
        enum : unsigned
        {
#define GA_FN(n, va, ...) n = va,
#define GA_DATA(n, va) n = va,
#define GA_CALL(n, va, f, c) n = va,
#define GA_CODE(n, va, len, f) n = va,
#define GA_LABEL(n, va, f) n = va,
#define GA_CRT(n, va, site) n = va,
#define GA_RTTI(n, va, s) n = va,
#define GA_VTBL(n, va) n = va,
#define GA_VSLOT(n, vt, slot, fn) n = slot,
#include GA_ADDR_FILE
#undef GA_FN
#undef GA_DATA
#undef GA_CALL
#undef GA_CODE
#undef GA_LABEL
#undef GA_CRT
#undef GA_RTTI
#undef GA_VTBL
#undef GA_VSLOT
        };
    }

    extern unsigned g_exeBase;                          // actual base of Sims.exe (ASLR), set by SimsMod_Init
    inline void *Va(unsigned va) { return (void *)(va - 0x400000u + g_exeBase); }
    template <class T> inline T &Global(unsigned va) { return *(T *)Va(va); }

    // thunks: cdecl / thiscall / virtual call by slot. Reference parameters are passed as pointers (same ABI). A class
    // returned by value goes through a hidden pointer (CallRet / ThisCallRet: the caller's slot, as MSVC passes it).
    template <class R, class... T> inline R Call(unsigned va, T... a) { return ((R(__cdecl *)(T...))Va(va))(a...); }
    template <class R, class... T> inline R ThisCall(unsigned va, const void *self, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))Va(va))(self, a...);
    }
    template <class R, class... T> inline void CallRet(R *pRet, unsigned va, T... a)
    {
        ((R *(__cdecl *)(R *, T...))Va(va))(pRet, a...);
    }
    template <class R, class... T> inline void ThisCallRet(R *pRet, unsigned va, const void *self, T... a)
    {
        ((R *(__thiscall *)(const void *, R *, T...))Va(va))(self, pRet, a...);
    }
    template <class R, class... T> inline R VCall(const void *self, int slot, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))(*(void ***)self)[slot])(self, a...);
    }

    class cXObject;
    class cXPerson;
    class ObjectModule;
    class ObjSelector;
    class iResFile;
    class cCareer;
    class cTSWin;
    class cTSWinBtn;
    class RenderDevice;
    struct RenderTarget;
    struct HandleNode;
    struct StackElem;
    struct Skill;
    struct Skeleton;
    struct Suit;
    struct Practice;
    class SAnimator;

    struct cTSPoint { long x, y; };
    struct cTSRect { long left, top, right, bottom; };

    // ---- the game's containers ----------------------------------------------------------------------------------
#if GAME_CC
    // std::vector<T> as the 2003 exe lays it out (SGI: start, finish, end of storage). Read and shrunk in place only;
    // it grows through the exe's own push_back for that element type (PushBack), which allocates from the exe's pool.
    template <class T> struct GVec
    {
        T *_Myfirst, *_Mylast, *_Myend;
        size_t size() const { return (size_t)(_Mylast - _Myfirst); }
        bool empty() const { return _Myfirst == _Mylast; }
        T &operator[](size_t i) const { return _Myfirst[i]; }
        T *begin() const { return _Myfirst; }
        T *end() const { return _Mylast; }
        T &back() const { return _Mylast[-1]; }
        void PushBack(unsigned vaPushBack, const T &x) { ThisCall<void>(vaPushBack, this, &x); }
    };
    // std::string as the 2003 exe lays it out (SGI: start, finish, end of storage; NUL-terminated), built, assigned and
    // freed by the exe's own functions (its pool allocator owns the text)
    class GameString
    {
    public:
        char *_M_start, *_M_finish, *_M_end_of_storage;
        explicit GameString(const char *psz) { ThisCall<void>(A::GameString__GameString, this, psz); }
        ~GameString() { ThisCall<void>(A::GameString___GameString, this); }
        GameString(const GameString &) = delete;
        GameString &operator=(const GameString &) = delete;
        GameString &operator=(const char *psz) { ThisCall<void>(A::GameString__assign, this, psz); return *this; }
        const char *c_str() const { return _M_start ? _M_start : ""; }
        size_t size() const { return (size_t)(_M_finish - _M_start); }
        // std::string::compare(0, n, psz)
        int compare(size_t nPos, size_t n, const char *psz) const
        {
            size_t nLen = size() - nPos < n ? size() - nPos : n, nOther = strlen(psz);
            int r = memcmp(c_str() + nPos, psz, nLen < nOther ? nLen : nOther);
            return r ? r : (nLen < nOther ? -1 : nLen > nOther ? 1 : 0);
        }
    };
    static_assert(sizeof(GameString) == 12, "GameString");
#else
    template <class T> using GVec = std::vector<T>;
    typedef std::string GameString;
#endif

    // ---- strings ----------------------------------------------------------------------------------------------------

    // StringBuffer (vptr, buffer, length, capacity) and StackString<N>: the exe's ctor sets the vtable (the virtual
    // destructor is empty, so a stack copy needs no destructor call).
    class StringBuffer
    {
    public:
        void *m_vptr;
        char *m_pBuf;
        unsigned short m_nLen;
        unsigned short m_nCapacity;
        const char *c_str() const { return m_pBuf; }
        int length() const { return m_nLen; }
        void clear() { m_nLen = 0; *m_pBuf = 0; }
        void append(const char *psz, int nLen = -1) { ThisCall<void>(A::StringBuffer__append_sz, this, psz, nLen); }
        void append(const StringBuffer &s, int nLen) { ThisCall<void>(A::StringBuffer__append, this, &s, nLen); }
        void copy(const char *psz) { ThisCall<void>(A::StringBuffer__copy_sz, this, psz); }
        void copy(const StringBuffer &s) { ThisCall<void>(A::StringBuffer__copy, this, &s); }
        int compare(const StringBuffer &s) const { return ThisCall<int>(A::StringBuffer__compare, this, &s); }
        int compareNoCase(const StringBuffer &s) const { return ThisCall<int>(A::StringBuffer__compareNoCase, this, &s); }
    protected:
        StringBuffer() {}
    };
    static_assert(sizeof(StringBuffer) == 12, "StringBuffer");

    template <int N> class StackString : public StringBuffer
    {
    public:
        StackString() { Init(); }
        StackString(const char *psz) { Init(); append(psz, -1); }
        StackString(const StackString &s) { Init(); append(s, s.length()); }
        StackString &operator=(const StackString &s) { copy(s); return *this; }
        char m_Buf[N];
    private:
        void Init() { ThisCall<void>(A::StringBuffer__StringBuffer, this, m_Buf, 0u, (unsigned)N); }
    };
    static_assert(sizeof(StackString<256>) == 0x10C, "StackString<256>");

    // CTGString / cTSString: std::string with extra methods (24 bytes, the same MSVC layout as this DLL's std::string;
    // 2003: the exe's 12-byte string)
    class CTGString : public GameString
    {
    public:
        CTGString(const char *psz) : GameString(psz) {}
    };
    class cTSString : public GameString
    {
    public:
        cTSString(const char *psz) : GameString(psz) {}
    };
    static_assert(sizeof(CTGString) == GA_O(24, 12), "CTGString");

    // ---- render layer -----------------------------------------------------------------------------------------------
#if GAME_CC
    // the 2003 renderer: software drawing into 16-bit cTSBuffers (DirectDraw surfaces or system memory), each window
    // into its buffer, the lot view into HouseViewer's view buffer
    struct cTSPixelFormat { int m_nFormat, m_nBitsPerPixel; };                  // passed by value (two dwords)
    struct cTSBlitFlags { unsigned long m_nColorKey, m_nFlags; };              // the source colour key, mirror bits
    enum { kTSLockBlit = 0x10, kTSLockPixels = 0x40 };                        // cTSBuffer::Lock / Unlock modes
    class cTSBuffer                                     // 0x64 bytes, reference counted (+0x58), 58 virtual slots
    {
    public:
        enum { kSize = 0x64 };
        union
        {
            GA_F(0x08, cTSPixelFormat, m_PixelFormat) GA_F(0x14, cTSRect, m_rcBounds) GA_F(0x24, cTSBlitFlags, m_BlitFlags)
            GA_F(0x58, int, m_nRefCount) GA_F(0x5C, cTSBuffer *, m_pZBuffer) GA_SIZE(kSize)
        };
        cTSBuffer() = delete;
        bool Create(int nWidth, int nHeight, cTSPixelFormat fmt, void *pPalette, int bVideoMemory, int nMemType)
        {
            return VCall<bool>(this, 3, nWidth, nHeight, fmt, pPalette, bVideoMemory, nMemType);
        }
        bool Lock(int nMode) { return VCall<bool>(this, 8, nMode); }
        bool Unlock(int nMode) { return VCall<bool>(this, 9, nMode); }
        void ClearColorKey() { VCall<void>(this, 28); }
        void SetBlitFlags(const cTSBlitFlags *p) { VCall<void>(this, 29, p); }
        bool Blit(cTSBuffer *pSrc, const cTSRect *pSrcRect, const cTSRect *pDstRect, const cTSRect *pClip)
        {
            return VCall<bool>(this, 39, pSrc, pSrcRect, pDstRect, pClip);
        }
        void *GetBits() { return VCall<void *>(this, 51); }
        int GetPitch() { return VCall<int>(this, 52); }                          // bytes
        unsigned short MakeNativeColor(int r, int g, int b)
        {
            return ThisCall<unsigned short>(A::cTSBuffer__MakeNativeColor, this, r & 0xFF, g & 0xFF, b & 0xFF);
        }
        bool Is565() { return MakeNativeColor(0, 255, 0) == 0x07E0; }
        void AddRef() { ThisCall<int>(A::cTSBuffer__AddRef, this); }
        void Release() { ThisCall<int>(A::cTSBuffer__Release, this); }        // deletes the buffer at 0
        // a new empty buffer from the exe's heap and constructor (refcount 0)
        static cTSBuffer *New()
        {
            void *p = Call<void *>(A::operator_new, (size_t)kSize);
            return p ? ThisCall<cTSBuffer *>(A::cTSBuffer__cTSBuffer, p) : 0;
        }
    };
    static_assert(sizeof(cTSBuffer) == 0x64, "cTSBuffer");

    // cTSBufferRef (2003): a cTSBuffer * holding one reference (the exe's handle is out of line; same AddRef / Release)
    class cTSBufferRef
    {
    public:
        cTSBuffer *m_p;
        cTSBufferRef() : m_p(0) {}
        explicit cTSBufferRef(cTSBuffer *p) : m_p(p) { if (m_p) m_p->AddRef(); }
        cTSBufferRef(const cTSBufferRef &o) : m_p(o.m_p) { if (m_p) m_p->AddRef(); }
        cTSBufferRef(cTSBufferRef &&o) noexcept : m_p(o.m_p) { o.m_p = 0; }
        ~cTSBufferRef() { Reset(); }
        cTSBufferRef &operator=(const cTSBufferRef &o)
        {
            if (o.m_p)
                o.m_p->AddRef();
            Reset();
            m_p = o.m_p;
            return *this;
        }
        cTSBufferRef &operator=(cTSBufferRef &&o) noexcept
        {
            if (&o != this)
            {
                Reset();
                m_p = o.m_p;
                o.m_p = 0;
            }
            return *this;
        }
        void Reset()
        {
            cTSBuffer *p = m_p;
            m_p = 0;
            if (p)
                p->Release();
        }
        cTSBuffer *Get() const { return m_p; }
    };
    static_assert(sizeof(cTSBufferRef) == 4, "cTSBufferRef");
    inline cTSRect RectOfBuffer(const cTSBuffer *p) { return p ? p->m_rcBounds : cTSRect{ 0, 0, 0, 0 }; }
#else

    struct TextureRef                                   // 0x50 bytes
    {
        union { GA_F0(unsigned int, m_nWidth) GA_F(0x04, unsigned int, m_nHeight) GA_F(0x40, unsigned __int64, m_hSet) GA_SIZE(0x50) };
    };
    struct ImageRes
    {
        TextureRef m_Tex;
        int m_nRefCount;                                // +0x50
        RenderDevice *m_pDevice;                        // +0x54
    };
    static_assert(sizeof(ImageRes) == 0x58, "ImageRes");

    // RefHandle<ImageRes> (cTSBufferRef): intrusive reference count, the exe's destructor releases
    class cTSBufferRef
    {
    public:
        ImageRes *m_p;
        cTSBufferRef() : m_p(0) {}
        cTSBufferRef(const cTSBufferRef &o) : m_p(o.m_p) { if (m_p) ++m_p->m_nRefCount; }
        cTSBufferRef(cTSBufferRef &&o) noexcept : m_p(o.m_p) { o.m_p = 0; }
        ~cTSBufferRef() { ThisCall<void>(A::RefHandle_ImageRes___RefHandle, this); }
        cTSBufferRef &operator=(const cTSBufferRef &o)
        {
            if (o.m_p)
                ++o.m_p->m_nRefCount;
            Reset();
            m_p = o.m_p;
            return *this;
        }
        cTSBufferRef &operator=(cTSBufferRef &&o) noexcept
        {
            if (&o != this)
            {
                Reset();
                m_p = o.m_p;
                o.m_p = 0;
            }
            return *this;
        }
        void Reset() { ThisCall<void>(A::RefHandle_ImageRes___RefHandle, this); m_p = 0; }
        ImageRes *Get() const { return m_p; }
    };
    static_assert(sizeof(cTSBufferRef) == 4, "cTSBufferRef");

    struct RenderTarget
    {
        union { GA_F0(int, m_nWidth) GA_F(0x04, int, m_nHeight) GA_F(0xB0, int, m_nRefCount) GA_SIZE(0xB8) };
    };
    class cRenderTargetRef                              // RefHandle<RenderTarget>
    {
    public:
        RenderTarget *m_p;
        cRenderTargetRef() : m_p(0) {}
        cRenderTargetRef(const cRenderTargetRef &o) : m_p(o.m_p) { if (m_p) ++m_p->m_nRefCount; }
        ~cRenderTargetRef() { ThisCall<void>(A::RefHandle_RenderTarget___RefHandle, this); }
        cRenderTargetRef &operator=(const cRenderTargetRef &) = delete;
    };

    struct VkDrawBinding { unsigned __int64 hBuffer; unsigned int nFirstVertex, nVertexCount; };
    struct VertexRange                                  // 24 bytes
    {
        VkDrawBinding binding;
        void *ptr;
        int reserved;
        VertexRange() : binding(), ptr(0), reserved(0) {}
        VertexRange(const VertexRange &o) : binding(o.binding), ptr(o.ptr), reserved(o.reserved) {}
    };
    static_assert(sizeof(VertexRange) == 24, "VertexRange");
    struct VertPosColorUV { float x, y, z; unsigned int color; float u, v; };
    struct Float2 { float x, y; };
    struct GraphicsPipeline;

    struct RenderPassState                              // 392 bytes
    {
        union
        {
            GA_F(0x0C, GraphicsPipeline *, m_pPipelines) GA_A(0x10, float, projection, 32) GA_F(0xA0, char, state)
            GA_SIZE(392)
        };
    };

    class RenderDevice
    {
    public:
        union { GA_F(0x38, int, m_eColorFormat) GA_F(0x3C, int, m_eDepthFormat) };
        RenderPassState *GetCurrentState() { return ThisCall<RenderPassState *>(A::RenderDevice__GetCurrentState, this); }
    };

    // Canvas: a RenderPassState * (header-inline helpers; the exe keeps out-of-line copies of the ones used here)
    class Canvas
    {
    public:
        RenderPassState *m_pState;
        explicit Canvas(RenderPassState *p) : m_pState(p) {}
        VertexRange AllocVerticesPosColorUV(unsigned nCount)
        {
            VertexRange r;
            ThisCallRet(&r, A::Canvas__AllocVertices_VertPosColorUV, this, nCount);
            return r;
        }
        // pipeline 3 (alpha-blended, textured), the pass projection
        static void SubmitQuad3(RenderPassState *pState, const VertexRange &r, const unsigned __int64 &hSet)
        {
            Call<void>(A::Canvas__SubmitQuad_3_1, (void *)&pState->state, (const void *)pState->m_pPipelines, &r, &hSet,
                       (const void *)pState->projection);
        }
        void DrawTexturedQuad2(const unsigned __int64 &hSet, const Float2 &tl, const Float2 &br, float z, const Float2 &uvTL,
                               const Float2 &uvBR, unsigned int nColor)
        {
            ThisCall<void>(A::Canvas__DrawTexturedQuad_2, this, &hSet, &tl, &br, z, &uvTL, &uvBR, nColor, (const void *)0);
        }
        void DrawTexturedQuad3(const unsigned __int64 &hSet, const Float2 &tl, const Float2 &br, float z, const Float2 &uvTL,
                               const Float2 &uvBR, unsigned int nColor)
        {
            ThisCall<void>(A::Canvas__DrawTexturedQuad_3, this, &hSet, &tl, &br, z, &uvTL, &uvBR, nColor, (const void *)0);
        }
    };

    struct ColorBgra8888 { unsigned char b, g, r, a; };
    struct TexLoadOptions                               // 88 bytes
    {
        std::function<bool(ColorBgra8888)> m_fnIsTransparentColor;
        std::function<bool(unsigned char)> m_fnIsTransparentIndex;
        unsigned char m_nSamplerIndex = 0;
        unsigned char m_bAntialiasAlphaEdge = 0;
    };
    static_assert(sizeof(TexLoadOptions) == 88, "TexLoadOptions");
    static_assert(offsetof(TexLoadOptions, m_nSamplerIndex) == 0x50, "TexLoadOptions::m_nSamplerIndex");
    struct ImageDataSrc                                 // 56 bytes
    {
        unsigned short m_nWidth;
        unsigned short m_nHeight;
        unsigned char m_nKind;
        int m_nRowPitch;
        int m_nFormat;                                  // VkFormat
        std::function<void(const ImageDataSrc &, std::byte *, const TexLoadOptions &)> m_fnDecode;
    };
    static_assert(sizeof(ImageDataSrc) == 56, "ImageDataSrc");

    inline RenderDevice *Graphics_GetCurrentTarget() { return Call<RenderDevice *>(A::Graphics_GetCurrentTarget); }
    inline cTSBufferRef Texture_LoadWithDecoder(RenderDevice *pDevice, ImageDataSrc *pSrc, const TexLoadOptions *pOpts)
    {
        cTSBufferRef r;
        CallRet(&r, A::Texture_LoadWithDecoder, pDevice, pSrc, pOpts);
        return r;
    }
#endif

    // ---- lots, terrain ----------------------------------------------------------------------------------------------

    struct CTilePt { signed char nX, nY, nLevel; };
    struct FTilePt { int nPosY16, nPosX16; };           // the exe's order: y first
    struct ScreenPt16 { short nY, nX; };              // the exe's order: y first (Mac {v, h})
    struct TileRotateEntry { signed char x, y; };

    template <class T> class c2DArray                   // _c2DArray: rows at +0x0C
    {
    public:
        union { GA_F(0x0C, T **, m_pRows) GA_SIZE(44) };
        T *operator[](int x) const { return m_pRows[x]; }
    };

    class cFixedWorld
    {
    public:
        union
        {
            GA_F(0x10, int, m_nSize) GA_A(0x20, c2DArray<unsigned short> *, m_pFloorsByLevel, 3)
            GA_A(0x2C, c2DArray<unsigned short> *, m_pRooms, 2) GA_A(0x34, c2DArray<unsigned char> *, m_pFlags, 2)
            GA_F(0x70, int, m_nRotMinX) GA_F(0x74, int, m_nRotMaxX) GA_F(0x78, int, m_nRotMinY) GA_F(0x7C, int, m_nRotMaxY)
            GA_F(0x84, int, m_nRotation)                // cRotatableWorld only
        };
        c2DArray<unsigned short> *Floors(int i) const { return m_pFloorsByLevel[1 + i]; }   // m_pFloors[i]
    };

    struct TileWalls                                    // 44 bytes (returned through a hidden pointer)
    {
        unsigned int m_nWallMask;
        char m_Rest[40];
        TileWalls() { memset(this, 0, sizeof *this); }
        TileWalls(const TileWalls &o) { memcpy(this, &o, sizeof *this); }
    };
    static_assert(sizeof(TileWalls) == 44, "TileWalls");
    enum { kWallSeg_AnyDiagonal = 0x30 };

    // ---- objects ----------------------------------------------------------------------------------------------------

    struct OBJD
    {
        union { GA_F(0x12, short, type) GA_F(0xCC, short, VitaBoy_Flags) };
    };
    enum { kObjType_Person = 2 };
    enum { kObjData_Room = 29 };

    class iResFile
    {
    public:
        void GetFileName(StackString<260> &path) { VCall<void>(this, 9, &path); }
        short Count(long nType) { return VCall<short>(this, 13, nType); }
        HandleNode *GetByIndex(long nType, short nIndex, void *pfn) { return VCall<HandleNode *>(this, 16, nType, (int)nIndex, pfn); }
        void GetID(HandleNode *h, short *pnId) { VCall<void>(this, 20, h, pnId); }
        void Release(HandleNode *h) { ThisCall<void>(A::iResFile__Release, this, h); }
    };

    class ObjSelector
    {
    public:
        union { GA_F0(iResFile *, m_pResFile) GA_F(0xB4, unsigned int, m_nFlagsWord) };
        int Kind() const { return (m_nFlagsWord >> 2) & 3; }    // bitfield m_nKind (1 = character, 2 = ...)
        long GetGUID() { return ThisCall<long>(A::ObjSelector__GetGUID, this); }
        void GetShortFilename(StringBuffer *p) { ThisCall<void>(A::ObjSelector__GetShortFilename, this, p); }
    };

    struct StackElem                                    // a tree frame (TreeSim.h)
    {
        union { GA_F(0x04, short, m_nStackObjectId) GA_F(0x06, unsigned char, m_nNumLocals) GA_F(0x08, int, m_nYieldState)
                GA_SIZE(20) };
        short *GetLocals() { return ThisCall<short *>(A::StackElem__GetLocals, this); }
    };

    union BehaviorNodeParam
    {
        GA_A0(short, m_Param, 4) GA_A0(unsigned char, m_Op, 8)
    };

    class cXObject
    {
    public:
        enum { kSize = GA_O(0x190, 0x18C) };
        union
        {
            GA_F(0x02C, short *, m_pAttributes) GA_F(0x030, int, m_nNumAttributes) GA_A(0x03A, short, m_Temps, 8)
            GA_A(0x04A, short, m_aData, 73) GA_F(0x04C, short, m_nDirection) GA_F(0x084, short, m_nRoom)
            GA_F(0x0DC, ObjectModule *, m_pModule) GA_F(0x0F0, short, m_nObjectId) GA_F(0x0F4, int, m_nPosY16)
            GA_F(0x0F8, int, m_nPosX16) GA_F(0x0FC, CTilePt, m_Tile) GA_F(0x110, int, m_nLevel)
            GA_F(GA_O(0x11C, 0x118), OBJD *, m_pObjdData) GA_F(GA_O(0x120, 0x11C), ObjSelector *, m_pSelector)
            GA_F(GA_O(0x12C, 0x128), short **, m_pInterpDest)
            GA_SIZE(kSize)
        };
        bool CanPlace(const FTilePt &pos, int nLevel, cXObject *pParent, int nSlot)     // virtual
        {
            return VCall<bool>(this, A::Slot_CanPlace, &pos, nLevel, pParent, nSlot);
        }
        void Place(const FTilePt &pos, int nLevel, cXObject *pParent, int nSlot, bool b)   // virtual
        {
            VCall<void>(this, A::Slot_Place, &pos, nLevel, pParent, nSlot, b);
        }
        void ForceLocation() { VCall<void>(this, A::Slot_ForceLocation); }
    };

#define GA_FP(off, type, name) GA_F((off) - cXObject::kSize, type, name)   // cXPerson field at absolute offset `off`
#define GA_AP(off, type, name, n) GA_A((off) - cXObject::kSize, type, name, n)
    enum ePersonData
    {
        kPersonDataCurrentOutfit = 0x8, kPersonDataCostumeTrunkOffset = 0x15, kPersonDataPersonType = 0x20,
        kPersonDataJobType = 0x38, kPersonDataJobPromotionLevel = 0x39, kPersonDataJobPerformance = 0x3F,
        kPersonDataGender = 0x41, kPersonDataIsGhost = 0x44,
    };
    class cXPerson : public cXObject
    {
    public:
        union { GA_AP(GA_O(0x590, 0x58C), short, m_PersonData, 256) GA_FP(GA_O(0x604, 0x600), short, m_nAge) };
        bool IsChild() const { return m_nAge && m_nAge < 18; }          // header inline (0x4b92c0)
        SAnimator *GetSAnimator() { return ThisCall<SAnimator *>(A::cXPerson__GetSAnimator, this); }
        bool GosubObjectTree(cXObject *pObj, short *pParams, short nTree, bool b)
        {
            return ThisCall<bool>(A::cXPerson__GosubObjectTree, this, pObj, pParams, (int)nTree, b);
        }
    };

    // dynamic_cast through the exe's own RTTI and __RTDynamicCast
    inline void *DynCast(void *p, unsigned tdFrom, unsigned tdTo)
    {
        return p ? Call<void *>(A::RTDynamicCast, p, 0L, Va(tdFrom), Va(tdTo), 0) : 0;
    }
    inline cXPerson *AsPerson(cXObject *p) { return (cXPerson *)DynCast(p, A::TD_cXObject, A::TD_cXPerson); }

    class ObjectModule
    {
    public:
        union { GA_F(0x18, GVec<cXObject *>, m_Objects) };
        cXObject *GetObject(int nId)                    // header inline in the exe
        {
            return nId > 0 && (unsigned)nId < m_Objects.size() + 1 ? m_Objects[nId - 1] : 0;
        }
    };

    class Room { public: bool IsOutside() { return ThisCall<bool>(A::Room__IsOutside, this); } };
    class RoomManager { public: Room *GetRoom(unsigned short n) { return ThisCall<Room *>(A::RoomManager__GetRoom, this, (unsigned)n); } };

    class House
    {
    public:
        union { GA_F(0x24, RoomManager *, m_pRoomManager) };
    };

    struct ObjectIterator                               // (House *, const CTilePt *, int): m_pCur is the object
    {
        union { GA_F(0x04, cXObject *, m_pCur) GA_SIZE(12) };
        ObjectIterator(House *pHouse, const CTilePt *pTile, int n) { ThisCall<void>(A::ObjectIterator__ObjectIterator, this, pHouse, pTile, n); }
        void Next() { ThisCall<void>(A::ObjectIterator__operator_inc, this); }
    };

    // ---- animation ----------------------------------------------------------------------------------------------------

    struct VBAnimMgr
    {
        Suit *FindSuit(CTGString *p) { return ThisCall<Suit *>(A::VBAnimMgr__FindSuit, this, p); }
        Skill *FindSkill(CTGString *p) { return ThisCall<Skill *>(A::VBAnimMgr__FindSkill, this, p); }
    };
    struct Skeleton { void UpdatePriorities() { ThisCall<void>(A::Skeleton__UpdatePriorities, this); } };
    struct Practice
    {
        union { GA_F(0x40, int, m_nLoopMode) };
        void SetPriority(int n) { ThisCall<void>(A::Practice__SetPriority, this, n); }
    };
    enum { kLoop_Repeat = 1 };
    struct VitaBoy
    {
        union { GA_F0(VBAnimMgr *, m_pAnimManager) GA_F(0x04, Skeleton *, m_pSkeleton) };
        Practice *StartPractice(Skill *p, int nPri, const float *pSpeed, const float *pWeight, int nTime)
        {
            return ThisCall<Practice *>(A::VitaBoy__StartPractice, this, p, nPri, pSpeed, pWeight, nTime);
        }
    };
    struct DressingRecord                               // 52 bytes
    {
        union { GA_F0(void *, m_pBinding) GA_F(0x04, GameString, m_Suit) GA_F(GA_O(0x1C, 0x10), GameString, m_Skin) };
        bool Redress(VBAnimMgr *pMgr, Skeleton *pSkel, CTGString *pSuit, CTGString *pSkin)
        {
            return ThisCall<bool>(A::DressingRecord__Redress, this, pMgr, pSkel, pSuit, pSkin);
        }
    };
    class SAnimator
    {
    public:
        union
        {
            GA_F(0x008, VitaBoy *, m_pAnimObject) GA_F(GA_O(0x144, 0x138), cXPerson *, m_pPerson)
            GA_F(GA_O(0x1BC, 0x1B0), int, m_nAppliedOutfit)
            GA_F(GA_O(0x268, 0x25C), GVec<DressingRecord *>, m_HandSuits)
        };
        VitaBoy *GetAnimationObject() { return VCall<VitaBoy *>(this, 43); }
        void Dress(CTGString *pSuit, CTGString *pSkin) { ThisCall<void>(A::SAnimator__Dress, this, pSuit, pSkin); }
        void Undress(CTGString *pSuit) { ThisCall<void>(A::SAnimator__Undress, this, pSuit); }
        void UpdateOutfitAndHands(bool b) { ThisCall<void>(A::SAnimator__UpdateOutfitAndHands, this, b); }
    };

    class EditPerson                                    // 152 bytes, the exe's ctor / dtor
    {
    public:
        union { GA_SIZE(152) };
        EditPerson() { ThisCall<void>(A::EditPerson__EditPerson, this); }
        ~EditPerson() { ThisCall<void>(A::EditPerson___EditPerson, this); }
        EditPerson(const EditPerson &) = delete;
        bool Begin(ObjSelector *pSel, cXPerson *pPerson) { return ThisCall<bool>(A::EditPerson__Begin, this, pSel, pPerson); }
        void End(bool b) { ThisCall<void>(A::EditPerson__End, this, b); }
        void SelectEscapeCharSuit(StackString<64> &s) { ThisCall<void>(A::EditPerson__SelectEscapeCharSuit, this, &s); }
        void SelectEscapeCharTexture(StackString<64> &s) { ThisCall<void>(A::EditPerson__SelectEscapeCharTexture, this, &s); }
        StackString<64> *GetSuit(int i) { return ThisCall<StackString<64> *>(A::EditPerson__GetSuit, this, i); }
        int GetSuitCount()                              // inlined in the exe (2003: 0x409291)
        {
            GVec<StackString<64>> &names = Global<GVec<StackString<64>>>(A::g_SuitNames);
            if (names.begin() == names.end())
                GetSuit(0);
            return (int)names.size();
        }
    };

    // ---- the simulator, the app -------------------------------------------------------------------------------------

    struct SimGlobals                                   // 64 shorts
    {
        union
        {
            GA_F0(short, Hour) GA_F(0x08, short, TimeOfDay) GA_F(0x0A, short, Minute) GA_F(0x14, short, CurrentHouse)
            GA_F(0x28, short, GameEdition) GA_F(0x38, short, DaysRunning) GA_A0(short, a, 64)
        };
    };
    static_assert(offsetof(SimGlobals, DaysRunning) == 2 * 28, "DaysRunning is global 28");

    class cSimulator
    {
    public:
        union { GA_F(0x10, SimGlobals, m_Globals) GA_F(0xB0, ObjectModule *, m_pObjectModule) };
        void SetGlobal(short n, short v) { ThisCall<void>(A::cSimulator__SetGlobal, this, (int)n, (int)v); }
        bool IsPaused() { return ThisCall<bool>(A::cSimulator__IsPaused, this); }
#if GAME_CC
        // not in the 2003 exe: 42 / the tick period of the speed (cSimulator::SetSpeed's formula), as the Legacy one
        float GetSimRate()
        {
            int nSpeed = m_Globals.a[16];               // global Speed
            return 42.0f / (float)(unsigned long)(__int64)(172.0f - nSpeed / 1000.0f * 171.0f + 0.5);
        }
#else
        float GetSimRate() { return ThisCall<float>(A::cSimulator__GetSimRate, this); }
#endif
    };

    class cSoundPlayer
    {
    public:
        bool PlayBySource(const char *psz, short n) { return ThisCall<bool>(A::cSoundPlayer__PlayBySource, this, psz, (int)n); }
    };

    class cTerrain;
    class HouseViewer;
    class Neighborhood;
    class cSimsSettings                                 // Globs
    {
    public:
        union
        {
            GA_F0(House *, m_pGame) GA_F(0x04, HouseViewer *, m_pHouseViewer) GA_F(0x10, cSoundPlayer *, m_pSoundSys)
            GA_F(0x1C, cSimulator *, m_pSimulator) GA_F(0x20, cTerrain *, m_pTerrain) GA_F(0x24, Neighborhood *, m_pNeighborhood)
            GA_F(GA_O(0x68, 0x64), signed char, m_nCurrentLanguage) GA_F(GA_O(0x6A, 0x66), short, m_nExpansionMask)
        };
    };
    enum { kEP_Vacation = 0x8, kEP_MakinMagic = 0x80 };

    class cSimsApp
    {
    public:
        int MessageDialog(const char *pszText, const char *pszTitle, unsigned long nSet, bool b)
        {
            return ThisCall<int>(A::cSimsApp__MessageDialog, this, pszText, pszTitle, nSet, b);
        }
    };
    enum { kMsgSet_Ok = 0 };

    inline cSimsSettings *Settings() { return Global<cSimsSettings *>(A::g_pAppSettings); }
    inline cSimsApp *SimsApp() { return Global<cSimsApp *>(A::g_pSimsApp); }
    inline cFixedWorld *Lot() { return Global<cFixedWorld *>(A::g_pLot); }        // cRotatableWorld
    inline cFixedWorld *World() { return Global<cFixedWorld *>(A::g_pWorld); }
    inline TileRotateEntry (&TileRotateLUT())[3][64][64] { return Global<TileRotateEntry[3][64][64]>(A::g_TileRotateLUT); }
#if GAME_CC
    // inlined at its callers in the 2003 exe (Globs::m_nCurrentLanguage, as Legacy's GetLanguage reads it)
    inline char GetLanguage() { return Settings()->m_nCurrentLanguage; }
#else
    inline char GetLanguage() { return Call<char>(A::GetLanguage); }       // 1 English .. 20 (see visit_lots -lang)
#endif
    enum { kLang_French = 3, kLang_German = 4, kLang_Italian = 5, kLang_Spanish = 6, kLang_Dutch = 7, kLang_Polish = 16 };
    inline short GlobalDispatch(short nCmd, long nParam) { return Call<short>(A::GlobalDispatch, (int)nCmd, nParam); }
    inline unsigned SimRand() { return Call<unsigned>(A::SimRand); }

    // cRotatableWorld::GetFlags (header inline): the flag byte of a view tile
    inline unsigned char LotGetFlags(CTilePt pt)
    {
        cFixedWorld *pLot = Lot();
        int nRot = -pLot->m_nRotation & 3;
        if (nRot)
        {
            const TileRotateEntry &e = TileRotateLUT()[nRot - 1][pt.nX][pt.nY];
            pt.nX = e.x;
            pt.nY = e.y;
        }
        return (*pLot->m_pFlags[pt.nLevel - 1])[pt.nX][pt.nY];
    }
    // cRotatableWorld::Rotate (header inline): view tile -> world tile
    inline CTilePt LotRotate(CTilePt pt)
    {
        int nRot = -Lot()->m_nRotation & 3;
        if (nRot)
        {
            const TileRotateEntry &e = TileRotateLUT()[nRot - 1][pt.nX][pt.nY];
            pt.nX = e.x;
            pt.nY = e.y;
        }
        return pt;
    }
}

#pragma warning(pop)

#ifndef GA_NO_USING
using namespace ga;
#endif
