#include "gof2/Globals.h"
#include "gof2/PaintCanvasClass.h"
#include "gof2/AEGeometry.h"
#include "gof2/FModSound.h"
// gof2/FileRead.h intentionally NOT included: its stub `struct Station`/`struct Agent`
// collide with the canonical Station.h/Agent.h definitions this file relies on. FileRead
// is only used here as an opaque handle via local extern "C" declarations.
#include "gof2/Status.h"
#include "gof2/Agent.h"          // defines the canonical global `struct RetStr` (12-byte String sret)
#include "gof2/ApplicationManager.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
// Mission.h transitively pulls Station.h, which redefines the identical (token-for-token,
// 12-byte aligned aggregate) global `struct RetStr` already provided by Agent.h. Rename the
// duplicate so it doesn't collide. Station::getName()'s return type becomes RetStr,
// which is layout-identical and only ever discarded here. (Other headers are out of edit scope.)
#define RetStr RetStr
#include "gof2/Mission.h"
#include "gof2/Station.h"
#undef RetStr
#include "gof2/RecordHandler.h"
#include "gof2/SolarSystem.h"
#include "gof2/String.h"

// Status / AEGeometry / String full layouts are not needed here: Globals only takes
// Status* as an opaque handle and reaches the engine via extern "C" free functions.
struct Status;


extern "C" void ArrayInt_release(Array<int> *a);
extern "C" void ArrayInt_add(int val, Array<int> *a);
extern "C" void Globals_startNewSoundResourceList_tail(int val, Array<int> *a);
extern "C" void AEString_ctor_copy(void *dst, void *src, bool flag);
extern "C" void AEString_ctor_default(void *out);
extern "C" float Globals_sqrt_impl(float x);
extern "C" void Globals_getRandomSystemForDrinks_tail(int a, int b);
extern "C" void Globals_addSoundResource_tail(int val, Array<int> *a);
extern "C" void AEString_default_ctor(void *dst);
extern "C" void AEString_copy_ctor(void *dst, void *src, int c);
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);
extern "C" void AEString_substr(void *dst, void *s, unsigned from, unsigned to);
extern "C" void AEString_assign(void *dst, void *src);
extern "C" void AEString_dtor(void *s);
extern "C" short *AEString_index(void *s, int i);
extern "C" void ArraySetLength_Str(unsigned n, void *a);
extern "C" long long Globals_lts_divmod(long long num, int den, int *rem);
extern "C" void AEString_int_ctor(void *dst, int v);
extern "C" void AEString_concat(void *dst, void *a);
void Globals_getLine(void *retSlot, unsigned font, void *text, int maxWidth, void *lineArr);
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" float VectorUnsignedToFloat(unsigned v, int mode);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_cstr_ctor(void *s, const char *str, int copy);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" int Ship_getPrice(int ship);
extern "C" int Ship_hasModInstalled(int ship, int modIndex);
// AERandom::nextInt — the real defs are free functions in AERandom.cpp:
//   nextInt_71aa4(self)        -> unbounded 32-bit draw (1-arg call form)
//   nextInt_71ad0(self, bound) -> bounded draw            (2-arg call form)
// The decompiler folded both into one variadic AERandom_nextInt thunk; call the real symbols.
namespace AbyssEngine { class AERandom; }
uint32_t nextInt_71aa4(AbyssEngine::AERandom *self);
int      nextInt_71ad0(AbyssEngine::AERandom *self, int bound);
extern "C" int idiv(int a, int b);
extern "C" void Globals_buildAgentMissionText(void *out, void *agent, int offer);
extern "C" int AEString_compare(void *a, void *b);
void MatrixSetTranslation(void *m, float x, float y, float z);
extern "C" void Globals_buildShipGroup0f(void *self, int param_2, void *canvas);
extern "C" int AERandom_nextIntB(int rng, int bound);
extern "C" void Mesh_setFace(void *canvas, int mesh, int face, int i0, int i1, int i2);
extern "C" void Mesh_setUV(void *canvas, int mesh, int vert, float u, float v);
extern "C" void Mesh_setVertex(void *canvas, int mesh, int vert, float x, float y, float z);
extern "C" void FileRead_ctor(void *self);
extern "C" void *FileRead_dtor(void *self);
// FileRead is used as an opaque handle here (gof2/FileRead.h not included; see top of file).
// loadStation / loadWreckCollision are instance methods on the local FileRead view above.
extern "C" void ArrayBV_ctor(void *a);
extern "C" void ArraySetLength_BV(unsigned n, void *a);
extern "C" void ArrayRelease_int(void *a);
extern "C" void *ArrayInt_dtor(void *a);
extern "C" float VectorScale(void *vec, float scalar);
extern "C" void BoundingSphere_ctor(void *self, float cx, float cy, float cz, float r);
extern "C" void *Galaxy_dtor(void *g);
extern "C" void *Status_dtor(void *s);
extern "C" void *GameText_dtor(void *g);
extern "C" void *AERandom_dtor(void *r);
extern "C" void *Layout_dtor(void *l);
extern "C" void *Generator_dtor(void *g);
extern "C" void *FModSound_dtor(void *s);
extern "C" void *Achievements_dtor(void *a);
extern "C" void *ImageFactory_dtor(void *f);
extern "C" void ArrayReleaseClasses_Item(void *a);
extern "C" void *ArrayItem_dtor(void *a);
extern "C" void ArrayReleaseClasses_Ship(void *a);
extern "C" void *ArrayShip_dtor(void *a);
extern "C" void Globals_releaseResources_tail(void *arg);
extern "C" void Mission_ctor(void *m);
extern "C" void Galaxy_ctor(void *g);
extern "C" void Achievements_ctor(void *a);
extern "C" void Status_ctor(void *s);
extern "C" void FileRead_ctor(void *f);
extern "C" void *FileRead_dtor(void *f);
extern "C" void AERandom_ctor(void *r);
extern "C" void Generator_ctor(void *g);
extern "C" void FModSound_ctor(void *s);
extern "C" int FModSound_tryToStopMusicForBGMusic();
void ParticleSettingsRef_initialize();
extern "C" void ArrayInt_ctor(void *a);
extern "C" int Station_getIndex(int station);
// The dropped-self Status singleton. The original loaded the global Status* implicitly for
// each of these 0-arg calls; the singleton lives at the hidden global g_status (same one
// MGame/Mission/PlayerTurret reach). Recover the receiver as (*g_status)->method().
extern "C" __attribute__((visibility("hidden"))) Status **g_status;
extern "C" void Status_resetGame();
extern "C" int Globals_dialogueDispatch(int category, int code);
extern "C" void ArrayReleaseClasses_Str(void *a);
extern "C" void *ArrayStr_dtor(void *a);
int GameText_getLanguage();
extern "C" void AEString_cstr_ctor(void *dst, const char *str, int c);

// ---- reportLeaderboards_e4b90.cpp ----
struct Status;
// Two hidden PC-relative globals, each a pointer-to-pointer (deref'd twice).
extern void *const gLB_status __attribute__((visibility("hidden")));
extern void *const gLB_dest __attribute__((visibility("hidden")));

void Globals_reportLeaderboards()
{
    int kills = ((Status *)(*(Status **)gLB_status))->getKills();
    *(int *)gLB_dest = kills;
}

// ---- resetHints_e3938.cpp ----
// Globals::resetHints() zeroes four 16-byte blocks of the hint struct (at offsets 0, 0x2b,
// 0x20, 0x10) using a zeroed NEON quad. Offset 0x2b is unaligned, so use packed 16-byte
// vector stores.
typedef int v4si __attribute__((vector_size(16)));
struct __attribute__((packed)) Q16 { v4si v; };

extern void *const gHints __attribute__((visibility("hidden")));

void Globals_resetHints()
{
    char *p = (char *)gHints;
    const v4si z = {0, 0, 0, 0};
    ((Q16 *)(p + 0x00))->v = z;
    ((Q16 *)(p + 0x2b))->v = z;
    ((Q16 *)(p + 0x20))->v = z;
    ((Q16 *)(p + 0x10))->v = z;
}

// ---- startNewSoundResourceList_e5554.cpp ----
void Globals_startNewSoundResourceList(void *self)
{
    if (((Globals*)self)->field_0x4 != 0) {
        ArrayInt_release(((Globals*)self)->field_0x4);
        if (((Globals*)self)->field_0x4 != 0) {
            ::operator delete(ArrayInt_dtor(((Globals*)self)->field_0x4));
        }
    }
    ((Globals*)self)->field_0x4 = 0;
    Array<int> *a = new Array<int>();
    ((Globals*)self)->field_0x4 = a;
    ArrayInt_add(0x7c, a);
    return Globals_startNewSoundResourceList_tail(0x7b, ((Globals*)self)->field_0x4);
}

// ---- getItemName_e4b68.cpp ----
// hidden PC-relative pointer-to-pointer global (deref'd twice).
extern void *const gItemNameGameText __attribute__((visibility("hidden")));

// RetStr (12-byte String sret aggregate) is provided by gof2/Agent.h above.

// r0=sret, r1=unused, r2=item id.
RetStr Globals_getItemName(void *unused, int item)
{
    (void)unused;
    String *src = (String *)((GameText *)(*(void **)gItemNameGameText))->getText(item + 0x4fa);
    RetStr r;
    return r;
}

// ---- getKeyActionName_e8c68.cpp ----
// Globals::getKeyActionName(int) returns a String by value. The default ctor returns void,
// so the compiler keeps a frame and restores the sret pointer (no tail call). The int arg
// is unused.

RetStr Globals_getKeyActionName(int action)
{
    (void)action;
    RetStr r;
    AEString_ctor_default(&r);
    return r;
}

// ---- sqrt_e8c80.cpp ----
// Globals::sqrt(float) - the target moves the float arg (r1) to r0 and tail-jumps to the
// runtime sqrtf. The instance pointer (r0) is unused; the float arrives in r1.

float Globals::sqrt(float x) {
    Globals *self = this;
    (void)self;
    return Globals_sqrt_impl(x);
}

// ---- getRandomSystemForDrinks_e4b38.cpp ----
extern void *const gDrinks_a __attribute__((visibility("hidden")));
extern void *const gDrinks_rng __attribute__((visibility("hidden")));

void Globals_getRandomSystemForDrinks()
{
    int a = *(int *)gDrinks_a;
    int r = nextInt_71ad0((AbyssEngine::AERandom *)*(int *)gDrinks_rng, 0x16);
    return Globals_getRandomSystemForDrinks_tail(a, r);
}

// ---- addSoundResourceToList_e55a6.cpp ----
// r0=container, r1=value. Linear search; on no-match (index reaches length) tail-call to add.
void Globals_addSoundResourceToList(void *self, int val)
{
    Array<int> *a = ((Globals*)self)->field_0x4;
    if (a != 0) {
        unsigned i = 0;
        for (;;) {
            if (i >= a->size()) {
                return Globals_addSoundResource_tail(val, a);
            }
            int e = (*a)[i];
            i = i + 1;
            if (e == val) {
                break;
            }
        }
    }
}

// ---- replaceKeyBindingTokens_e8c72.cpp ----
// Globals::replaceKeyBindingTokens(String const&) returns a String by value (sret in r0).
// Body copy-constructs the return slot from the source string (in r2), flag=false.
// The copy-ctor returns void -> frame kept, sret pointer restored (no tail call).

RetStr Globals_replaceKeyBindingTokens(void *unused, void *src)
{
    (void)unused;
    RetStr r;
    return r;
}

// ---- getRandomStation_e4af0.cpp ----
// Local minimal FileRead view (gof2/FileRead.h conflicts with Station.h/Agent.h here,
// see top of file). Declaring the instance methods we call lets us link straight to the
// real FileRead:: symbols without the opaque extern "C" thunks.
struct FileRead {
    int32_t loadStation(int32_t id);
    Array<int32_t> *loadWreckCollision(int32_t id);
    Array<Item *> *loadItemsBinary();
    Array<Ship *> *loadShipsBinary();
    Array<String *> *loadNamesBinary(int32_t type, bool first, bool second);
};
extern void *const gStationRng __attribute__((visibility("hidden")));

int Globals_getRandomStation()
{
    FileRead *f = (FileRead *)::operator new(1);
    FileRead_ctor(f);
    int which = nextInt_71ad0((AbyssEngine::AERandom *)*(int *)gStationRng, 0x87);
    int r = (int)(long)f->loadStation(which);
    ::operator delete(FileRead_dtor(f));
    return r;
}

// ---- drawLines_e4688.cpp ----
// 4-arg Globals::drawLines(uint, Array<String*>*, int, int) forwards to the 6-value
// drawLines variant with the first arg forced to 0, the incoming 5th stack value as the
// "startY", and a trailing 0.
void Globals_drawLines5(unsigned p1, void *font, Array<int> *lines, int baseX,
                                   int startY, int centered);

void Globals_drawLines4(unsigned p1, void *font, Array<int> *lines, int baseX,
                                   int startY)
{
    (void)p1;
    Globals_drawLines5(0, font, lines, baseX, startY, 0);
}

// ---- getLineArray_e4384.cpp ----
extern "C" void AEString_append(void *dst, void *src);                // operator+=
extern "C" void AEString_substrTrailing(void *dst, void *s);          // SubString(this) one-arg form

extern void *const gGLA_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4584
extern const char gGLA_newline[] __attribute__((visibility("hidden")));     // DAT_000f4588

// Globals::getLineArray(uint font, AbyssEngine::String const& text, int maxWidth,
//                        Array<String*>* out)
// font in r1, text in r2, maxWidth in r3, out at [r7+0x8].
void Globals_getLineArray(unsigned font, void *text, int maxWidth, void *arg3,
                                     Array<void *> *out)
{
    (void)arg3;
    int *guardP = *(int **)gGLA_guardHolder;
    volatile int saved = *guardP;

    void *line = ::operator new(0xc);
    AEString_default_ctor(line);

    char work[12];
    AEString_copy_ctor(work, text, 0);   // work = copy of text (Ghidra param_3 here is the text)
    char nl[12];
    AEString_cstr_ctor(nl, gGLA_newline, 0);
    AEString_append(work, nl);

    unsigned count = 0;
    int consumed = 0;
    int total = *(int *)((char *)work + 0x14);   // local_2c-style length
    while (consumed < total) {
        char sub[12];
        AEString_substr(sub, work, consumed, total);
        char ssub[12];
        AEString_copy_ctor(ssub, sub, 0);
        Globals_getLine(line, font, ssub, maxWidth, out);
        int adv = *(int *)((char *)line + 8);
        count++;
        consumed += adv;
    }
    {
        void (**vt)(void *) = *(void (***)(void *))line;
        vt[1](line);
    }

    ArraySetLength_Str(count, out);
    for (unsigned i = 0; i < count; i++) {
        void *s = ::operator new(0xc);
        AEString_default_ctor(s);
        (*out)[i] = s;
    }

    for (unsigned i = 0; i < count; i++) {
        char sub[12];
        AEString_substr(sub, work, consumed, total);
        char ssub[12];
        AEString_copy_ctor(ssub, sub, 0);
        void *slot = (*out)[i];
        Globals_getLine(slot, font, ssub, maxWidth, slot);

        int li = 0;
        void *s = (*out)[i];
        int hi = (int)((String *)s)->size();
        while (*AEString_index(s, li) == 0x20) {
            li++;
            s = (*out)[i];
        }
        hi++;
        do {
            void *cur = (*out)[i];
            short ch = *AEString_index(cur, hi - 2);
            hi--;
            if (ch != 0x20) break;
        } while (true);

        char trimmed[12];
        void *cur = (*out)[i];
        AEString_substr(trimmed, cur, li, hi);
    }


    return;
}

// ---- longToTimeString_e3994.cpp ----
extern void *const gLTS2_guardHolder __attribute__((visibility("hidden")));  // DAT_000f3c38
extern const char gLTS2_secTens[] __attribute__((visibility("hidden")));     // DAT_000f3c3c
extern const char gLTS2_secEmpty[] __attribute__((visibility("hidden")));    // DAT_000f3c40
extern const char gLTS2_minTens[] __attribute__((visibility("hidden")));     // DAT_000f3c44
extern const char gLTS2_minEmpty[] __attribute__((visibility("hidden")));    // DAT_000f3c48
extern const char gLTS2_zeroPrefix[] __attribute__((visibility("hidden")));  // DAT_000f3c4c
extern const char gLTS2_hrTens[] __attribute__((visibility("hidden")));      // DAT_000f3c54
extern const char gLTS2_hrEmpty[] __attribute__((visibility("hidden")));     // DAT_000f3c58
extern const char gLTS2_sep1[] __attribute__((visibility("hidden")));        // DAT_000f3c5c
extern const char gLTS2_sep2[] __attribute__((visibility("hidden")));        // DAT_000f3c60

// Globals::longToTimeString(long long ms, AbyssEngine::String& out)
// out at [r7+0x8]; ms in r2:r3.
void Globals_longToTimeString(void *retSlot, void *unused, long long ms)
{
    (void)unused;
    (void)retSlot;
    int *guardP = *(int **)gLTS2_guardHolder;
    volatile int saved = *guardP;
    void *out = retSlot;   // sret slot lives at [r7+8] in target; reuse param

    int rem = 0;
    long long secQ = Globals_lts_divmod(ms, 1000, &rem);
    int seconds = 0;
    Globals_lts_divmod(secQ, 0x3c, &seconds);

    long long minQ = Globals_lts_divmod(ms, 0xea60, &rem);
    int minute = 0;
    Globals_lts_divmod(minQ, 0x3c, &minute);

    long long hrQ = Globals_lts_divmod(ms, 0xea60, &rem);
    int hours = 0;
    Globals_lts_divmod(hrQ, 0x18, &hours);

    char secPart[12], secNum[12], secStr[12];
    AEString_cstr_ctor(secPart, seconds < 10 ? gLTS2_secTens : gLTS2_secEmpty, 0);
    AEString_int_ctor(secNum, seconds);
    AEString_concat(secStr, secPart);

    char minPart[12], minNum[12], minStr[12];
    AEString_cstr_ctor(minPart, minute < 10 ? gLTS2_minTens : gLTS2_minEmpty, 0);
    AEString_int_ctor(minNum, minute);
    AEString_concat(minStr, minPart);

    if (hours == 0) {
        char prefix[12], left[12], full[12];
        AEString_cstr_ctor(prefix, gLTS2_zeroPrefix, 0);
        AEString_concat(left, minStr);
        AEString_concat(full, left);
    } else {
        int rem4 = 0;
        long long h = Globals_lts_divmod(ms, 0xea60, &rem4);
        int hv = (int)h * 0x18 + hours;

        char hrPart[12], hrNum[12], hrStr[12];
        AEString_cstr_ctor(hrPart, hv < 10 ? gLTS2_hrTens : gLTS2_hrEmpty, 0);
        AEString_int_ctor(hrNum, hv);
        AEString_concat(hrStr, hrPart);

        char s1[12], a[12], b[12], s2[12], c[12], full[12];
        AEString_cstr_ctor(s1, gLTS2_sep1, 0);
        AEString_concat(a, hrStr);
        AEString_concat(b, a);
        AEString_cstr_ctor(s2, gLTS2_sep2, 0);
        AEString_concat(c, b);
        AEString_concat(full, c);
    }


    return;
}

// ---- getBoundedString_e469c.cpp ----
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);   // String(String const&, bool)
extern "C" void AEString_default_ctor(void *dst);                     // String()
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);  // String(char const*, bool)
extern "C" void AEString_assign(void *dst, void *src);               // operator=
extern "C" void AEString_concat(void *dst, void *line);              // operator+

extern void *const gGBS_guardHolder __attribute__((visibility("hidden")));  // DAT_000f478c
extern void *const gGBS_strPtr __attribute__((visibility("hidden")));       // DAT_000f4790
extern void *const gGBS_canvas __attribute__((visibility("hidden")));       // DAT_000f4794
extern const char gGBS_prefix[] __attribute__((visibility("hidden")));      // DAT_000f4798

// Globals::getBoundedString(AbyssEngine::String const&, int) — String returned via sret (param_1).
// text is in r2 (the const String&), width in r3.
void Globals_getBoundedString(void *retSlot, void *unused, void *text, int width)
{
    (void)unused;
    int *guardP = *(int **)gGBS_guardHolder;
    volatile int saved = *guardP;

    AEString_copy_ctor(retSlot, text, 0);

    void **strPtr = *(void ***)gGBS_strPtr;
    int **canvas = *(int ***)gGBS_canvas;
    int w = ((PaintCanvas *)(long)**canvas)->GetTextWidth(0, *strPtr);
    if (width < w) {
        void *line = ::operator new(0xc);
        AEString_default_ctor(line);

        int font = (int)(long)*strPtr;
        char tmpText[12];
        AEString_copy_ctor(tmpText, text, 0);
        Globals_getLine(font ? (void *)(long)font : line, font, tmpText, width - 3, line);

        char prefix[12];
        AEString_cstr_ctor(prefix, gGBS_prefix, 0);
        char concat[12];
        AEString_concat(concat, line);

        void (**vt)(void *) = *(void (***)(void *))line;
        vt[1](line);
    }

    return;
}

// ---- setCoordsSteer_e8404.cpp ----
// Hidden float constants and locale/flag globals (PC-relative).
extern const float gSCS_f86f8 __attribute__((visibility("hidden")));
extern const float gSCS_f86fc __attribute__((visibility("hidden")));
extern const float gSCS_f8700 __attribute__((visibility("hidden")));
extern const float gSCS_f8704 __attribute__((visibility("hidden")));
extern const float gSCS_f8708 __attribute__((visibility("hidden")));
extern const float gSCS_f870c __attribute__((visibility("hidden")));
extern const float gSCS_f8710 __attribute__((visibility("hidden")));
extern const float gSCS_f8714 __attribute__((visibility("hidden")));
extern const float gSCS_f8718 __attribute__((visibility("hidden")));
extern const float gSCS_f871c __attribute__((visibility("hidden")));
extern const float gSCS_f8720 __attribute__((visibility("hidden")));
extern const float gSCS_f8724 __attribute__((visibility("hidden")));
extern const float gSCS_f8728 __attribute__((visibility("hidden")));
extern const float gSCS_f872c __attribute__((visibility("hidden")));
extern const float gSCS_f8730 __attribute__((visibility("hidden")));

extern void *const gSCS_screenH __attribute__((visibility("hidden")));   // DAT_000f8734 -> int*
extern void *const gSCS_isPhone __attribute__((visibility("hidden")));   // DAT_000f8738 -> char*
extern void *const gSCS_flagB __attribute__((visibility("hidden")));     // DAT_000f873c
extern void *const gSCS_objA __attribute__((visibility("hidden")));      // DAT_000f8740 -> obj (field 0x54)
extern void *const gSCS_objB __attribute__((visibility("hidden")));      // DAT_000f8744
extern void *const gSCS_flagC __attribute__((visibility("hidden")));     // DAT_000f8748
extern void *const gSCS_flagD __attribute__((visibility("hidden")));     // DAT_000f874c
extern void *const gSCS_objC __attribute__((visibility("hidden")));      // DAT_000f8750
extern void *const gSCS_flagE __attribute__((visibility("hidden")));     // DAT_000f8754
extern void *const gSCS_flagF __attribute__((visibility("hidden")));     // DAT_000f8758
extern void *const gSCS_flagG __attribute__((visibility("hidden")));     // DAT_000f875c
extern void *const gSCS_flagH __attribute__((visibility("hidden")));     // DAT_000f8760

static inline char rdflag(void *const g) { return **(char **)&g; }
static inline int *rdobj(void *const g) { return *(int **)&g; }

// Globals::setCoordsSteer(int p1, int p2, int p3, int p4, ushort& a..ushort& j)
void Globals_setCoordsSteer(void *self, int p1, int p2, int p3, int p4,
                                       unsigned short *o5, unsigned short *o6, unsigned short *o7,
                                       unsigned short *o8, unsigned short *o9, unsigned short *o10,
                                       unsigned short *o11, unsigned short *o12, unsigned short *o13,
                                       unsigned short *o14)
{
    (void)self;
    int screenH = **(int **)gSCS_screenH;
    char isPhone = rdflag(gSCS_isPhone);
    int bottom = ((-0x19 - p2) - p3) + screenH;

    float thresh = gSCS_f86f8;
    if (isPhone == 0) {
        thresh = (rdflag(gSCS_flagB) == 0) ? gSCS_f8700 : gSCS_f86fc;
    }
    if (bottom < p1) {
        p1 = bottom;
    }
    float fp1 = VectorSignedToFloat(p1, 0);

    unsigned short uv;
    int iv;
    if (thresh < fp1) {
        iv = (int)fp1;
        uv = (unsigned short)iv;
        rdobj(gSCS_objB)[0x54 / 4] = (int)fp1;
        char flag8;
        if (isPhone == 0) {
            flag8 = rdflag(gSCS_flagC);
            goto label8508;
        }
        goto common;
    label8508:
        uv = (flag8 == 0) ? 0x14 : 0x28;
        *o5 = uv;
        *o6 = (unsigned short)iv;
        *o13 = uv;
        {
            float sub = (flag8 == 0) ? gSCS_f8710 : gSCS_f870c;
            float add14 = (flag8 == 0) ? gSCS_f8718 : gSCS_f8714;
            float val = VectorUnsignedToFloat(*o6, 0);
            float r = val - sub;
            *o14 = (unsigned short)((0.0f < r) ? (short)(int)r : 0);
            *o7 = 0x14;
            (void)add14;
            goto label8556;
        }
    } else {
        if (isPhone == 0) {
            char flag = rdflag(gSCS_flagD);
            iv = (flag == 0) ? 0x96 : 0x12c;
            rdobj(gSCS_objC)[0x54 / 4] = iv;
            char flag8 = (flag == 0) ? 0 : 1;
            (void)flag8;
            goto label8508;
        }
        uv = 0xd2;
        rdobj(gSCS_objA)[0x54 / 4] = 0xd2;
    }

common: {
    float fv = gSCS_f8708;
    *o5 = 0x1c;
    *o6 = uv;
    *o13 = 0x1c;
    float val = VectorUnsignedToFloat(*o6, 0);
    float r = val + gSCS_f8704;
    *o14 = (unsigned short)((0.0f < r) ? (short)(int)r : 0);
    *o7 = 0x14;
    (void)fv;
}

label8556: {
    unsigned absp2 = (unsigned)(p2 - (p2 >> 31));
    float fv = gSCS_f8708;
    float val6 = VectorUnsignedToFloat(*o6, 0);
    float r8 = fv + val6;
    *o8 = (unsigned short)((0.0f < r8) ? (short)(int)r8 : 0);
    *o9 = (unsigned short)(*o7 + (short)(absp2 >> 1));
    *o10 = (unsigned short)(*o8 + (short)(absp2 >> 1));

    float val6b = VectorUnsignedToFloat(*o6, 0);
    float fv2;
    float acc;
    if (isPhone == 0) {
        bool b = (rdflag(gSCS_flagE) == 0);
        acc = (b ? gSCS_f8728 : gSCS_f8724) + val6b;
        fv2 = b ? 2.0f : 4.0f;
    } else {
        acc = val6b + gSCS_f871c;
        fv2 = gSCS_f8720;
    }
    int i12 = (0.0f < acc) ? (int)acc : 0;
    *o12 = (unsigned short)i12;

    float top = VectorSignedToFloat(screenH - p4, 0);
    float f12 = VectorUnsignedToFloat((unsigned)i12, 0);
    if (f12 <= top - fv2) {
        unsigned short u11;
        if (isPhone == 0) {
            u11 = (rdflag(gSCS_flagH) == 0) ? 0x14 : 0x28;
        } else {
            u11 = 0x1c;
        }
        *o11 = u11;
    } else {
        int a, b;
        if (isPhone == 0) {
            float fv14 = 4.0f;
            a = 0x15a;
            char flag8 = rdflag(gSCS_flagF);
            if (flag8 == 0) fv14 = 2.0f;
            b = 0x10a;
            float d = f12 - (top - fv14);
            int i16 = (0.0f < d) ? (int)d : 0;
            *o12 = (unsigned short)i16;
            if (flag8 == 0) { a = 0xad; b = 0x85; }
            float fd = VectorUnsignedToFloat((unsigned)i16, 0);
            float fr = VectorSignedToFloat(a - b, 0);
            float scale = 1.0f;
            if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
            float base = VectorSignedToFloat(b, 0);
            base = base + scale * fr;
            *o11 = (unsigned short)((0.0f < base) ? (short)(int)base : 0);
            float fv15 = gSCS_f8720;
            if (rdflag(gSCS_flagG) == 0) fv15 = 2.0f;
            *o12 = (unsigned short)((0.0f < top - fv15) ? (short)(int)(top - fv15) : 0);
        } else {
            a = 0xf3; b = 0xbb;
            float d = f12 - (top + gSCS_f872c);
            int i16 = (0.0f < d) ? (int)d : 0;
            *o12 = (unsigned short)i16;
            float fd = VectorUnsignedToFloat((unsigned)i16, 0);
            float fr = VectorSignedToFloat(a - b, 0);
            float scale = 1.0f;
            if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
            float base = VectorSignedToFloat(b, 0);
            base = base + scale * fr;
            *o11 = (unsigned short)((0.0f < base) ? (short)(int)base : 0);
            *o12 = (unsigned short)((0.0f < top - gSCS_f8720) ? (short)(int)(top - gSCS_f8720) : 0);
        }
    }
}
}

// ---- getAgentMissionText_e5ed4.cpp ----
// The full briefing-text assembly (large data-driven template/concat sequence). Driven by the
// agent's offer/event context; writes the completed String into `out` (sret slot). Kept as an
// extern helper because the original is a ~7KB straight-line concat over ~100 String temporaries.

extern void *const gGAMT_guard __attribute__((visibility("hidden")));    // DAT_000f6154
extern const char gGAMT_noAgent[] __attribute__((visibility("hidden"))); // DAT_000f616c
extern void *const gGAMT_busyObj __attribute__((visibility("hidden")));  // DAT_000f6178 (field 0xd0 counter)
extern void *const gGAMT_modText __attribute__((visibility("hidden")));  // DAT_000f617c

// Globals::getAgentMissionText(Agent*) -> String via sret (param_1). agent in r2.
void Globals_getAgentMissionText(void *out, void *unused, void *agent)
{
    (void)unused;
    int *guardP = *(int **)gGAMT_guard;
    volatile int saved = *guardP;

    if (agent == 0) {
        AEString_cstr_ctor(out, gGAMT_noAgent, 0);
        goto epilogue;
    }

    {
        char acc[12];
        AEString_default_ctor(acc);

        if (((Agent *)(agent))->isGenericAgent() == 0) {
            char scratch[12];
            AEString_default_ctor(scratch);

            int event = ((Agent *)(agent))->getEvent();
            if (event < 1 && ((Agent *)(agent))->hasAcceptedOffer() == 0) {
                int *busy = *(int **)gGAMT_busyObj;
                *(int *)(*busy + 0xd0) += 1;       // mark "assembling text" re-entrancy guard
                int offer = ((Agent *)(agent))->getOffer();

                if (offer == 8) {
                    int ship = (int)(long)(*g_status)->getShip();
                    int price = Ship_getPrice(ship);
                    int pct = ((Agent *)(agent))->getModPricePercentage();
                    ((Agent *)(agent))->setSellItemPrice(idiv(price * pct, 100));
                    ship = (int)(long)(*g_status)->getShip();
                    int modIdx = ((Agent *)(agent))->getSellModIndex();
                    if (Ship_hasModInstalled(ship, modIdx) != 0) {
                        void *t = ((GameText *)((void *)(long)**(int **)gGAMT_modText))->getText(modIdx);
                        *(int *)(*busy + 0xd0) -= 1;
                        AEString_copy_ctor(out, acc, 0);
                        goto epilogue;
                    }
                }

                // General offer/event briefing text: data-driven assembly into `acc`.
                Globals_buildAgentMissionText(acc, agent, offer);
                *(int *)(*busy + 0xd0) -= 1;
            } else {
                Globals_buildAgentMissionText(acc, agent, -1);
            }
        } else {
            Globals_buildAgentMissionText(acc, agent, -1);
        }

        AEString_copy_ctor(out, acc, 0);
    }

epilogue:
    return;
}

// ---- getInAppPurchaseArrayIndex_e8c88.cpp ----
extern void *const gIAP_guardHolder __attribute__((visibility("hidden")));  // DAT_000f8f20
extern const char gIAP_prefixA[] __attribute__((visibility("hidden")));     // DAT_000f8f24
extern const char gIAP_prefixB[] __attribute__((visibility("hidden")));     // DAT_000f8f28
extern const char gIAP_id50[] __attribute__((visibility("hidden")));        // DAT_000f8f2c
extern const char gIAP_id0[] __attribute__((visibility("hidden")));         // DAT_000f8f30
extern const char gIAP_id1[] __attribute__((visibility("hidden")));         // DAT_000f8f34
extern const char gIAP_id2[] __attribute__((visibility("hidden")));         // DAT_000f8f38
extern const char gIAP_id3[] __attribute__((visibility("hidden")));         // DAT_000f8f3c
extern const char gIAP_id4[] __attribute__((visibility("hidden")));         // DAT_000f8f40
extern const char gIAP_id51[] __attribute__((visibility("hidden")));        // DAT_000f8f44
extern const char gIAP_id52[] __attribute__((visibility("hidden")));        // DAT_000f8f48
extern const char gIAP_id53[] __attribute__((visibility("hidden")));        // DAT_000f8f4c
extern const char gIAP_id54[] __attribute__((visibility("hidden")));        // DAT_000f8f50

// Globals::getInAppPurchaseArrayIndex(int productCode, Array<String*>* list)
// productCode in r1, list in r2; returns index or -1.
int Globals_getInAppPurchaseArrayIndex(void *self, int productCode, void *list)
{
    (void)self;
    int *guardP = *(int **)gIAP_guardHolder;
    volatile int saved = *guardP;
    int result = -1;

    if (list != 0) {
        unsigned len = *(unsigned *)list;
        unsigned i = 0;
        bool keepGoing = true;
        while (i < len && keepGoing) {
            void *entry = *(void **)(*(char **)((char *)list + 4) + i * 4);
            char base[12], pb[12], prefix[12];
            AEString_cstr_ctor(base, gIAP_prefixA, 0);
            AEString_cstr_ctor(pb, gIAP_prefixB, 0);
            AEString_concat(prefix, base);

            if (entry == 0) {
                result = -1;
                keepGoing = false;
            } else {
                const char *suffix = 0;
                bool known = true;
                switch (productCode) {
                case 0: suffix = gIAP_id0; break;
                case 1: suffix = gIAP_id1; break;
                case 2: suffix = gIAP_id2; break;
                case 3: suffix = gIAP_id3; break;
                case 4: suffix = gIAP_id4; break;
                case 0x32: suffix = gIAP_id50; break;
                case 0x33: suffix = gIAP_id51; break;
                case 0x34: suffix = gIAP_id52; break;
                case 0x35: suffix = gIAP_id53; break;
                case 0x36: suffix = gIAP_id54; break;
                default: known = false; break;
                }
                if (!known) {
                    keepGoing = true;   // treat unknown like "no match, continue"
                } else {
                    char suf[12], full[12];
                    AEString_cstr_ctor(suf, suffix, 0);
                    AEString_concat(full, prefix);
                    int cmp = AEString_compare(entry, full);
                    if (cmp == 0) {
                        result = (int)i;
                        keepGoing = false;
                    } else {
                        keepGoing = true;
                    }
                }
            }
            i++;
        }
    }

    return result;
}

// ---- getKeyBindingReplaceString_e8c1c.cpp ----
// Globals::getKeyBindingReplaceString(int) — returns a String by value.
// Default-construct a stack String, upper-case it (returns a String& copy-constructed into the
// sret slot), then the stack String is destroyed. The on-stack String is guarded by an
// explicit stack canary, mirroring the matched ConfigReader::GetNewLine / AEFile idiom.


// Default-construct a temp String, upper-case it, copy-construct the result into the sret
// blob, then destroy the temp.
RetStr Globals_getKeyBindingReplaceString(Globals *, int key)
{
    (void)key;

    RetStr tmp;
    AEString_ctor_default(&tmp);
    ((String *)((String *)&tmp))->ToUpperCase();   // in-place; returns void
    RetStr result;
    return result;
}

// ---- longToTimeStringNoSeconds_e3c68.cpp ----
// 64-bit divide helper: returns quotient (r0/r1), remainder lands in r2 (extraout_r2).
// Modeled as lldiv computing both; we expose remainder via out-param.

extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);  // String(char const*, bool)
extern "C" void AEString_int_ctor(void *dst, int v);                  // String(int)
extern "C" void AEString_concat(void *dst, void *a);                 // operator+
extern "C" void AEString_assign(void *dst, void *src);               // operator=

extern void *const gLTS_guardHolder __attribute__((visibility("hidden")));  // DAT_000f3dec
extern const char gLTS_minTens[] __attribute__((visibility("hidden")));     // DAT_000f3df0 "0"
extern const char gLTS_minEmpty[] __attribute__((visibility("hidden")));    // DAT_000f3df4 ""
extern const char gLTS_hrTens[] __attribute__((visibility("hidden")));      // DAT_000f3df8 "0"
extern const char gLTS_hrEmpty[] __attribute__((visibility("hidden")));     // DAT_000f3dfc ""
extern const char gLTS_sep[] __attribute__((visibility("hidden")));         // DAT_000f3e00 ":"

// Globals::longToTimeStringNoSeconds(long long ms, AbyssEngine::String& out)
// out is the sret String at [r7+0x8]; param_1 (ms) in r2:r3.
void Globals_longToTimeStringNoSeconds(void *retSlot, void *unused, long long ms)
{
    (void)unused;
    int *guardP = *(int **)gLTS_guardHolder;
    volatile int saved = *guardP;

    int rem = 0;
    // ms / 60000 -> total minutes (quotient), then minute-of-hour via %60.
    long long q = Globals_lts_divmod(ms, 0xea60, &rem);
    int minute = 0;
    Globals_lts_divmod(q, 0x3c, &minute);   // minute = q % 60 (remainder)

    long long q2 = Globals_lts_divmod(ms, 0xea60, &rem);
    int hours = 0;
    Globals_lts_divmod(q2, 0x18, &hours);   // hours = q2 % 24

    char mPart[12], mNum[12], minStr[12];
    AEString_cstr_ctor(mPart, minute < 10 ? gLTS_minTens : gLTS_minEmpty, 0);
    AEString_int_ctor(mNum, minute);
    AEString_concat(minStr, mPart);

    int rem3 = 0;
    long long h = Globals_lts_divmod(ms, 0xea60, &rem3);
    int hv = (int)h * 0x18 + hours;

    char hPart[12], hNum[12], hrStr[12];
    AEString_cstr_ctor(hPart, hv < 10 ? gLTS_hrTens : gLTS_hrEmpty, 0);
    AEString_int_ctor(hNum, hv);
    AEString_concat(hrStr, hPart);

    char sep[12], left[12], full[12];
    AEString_cstr_ctor(sep, gLTS_sep, 0);
    AEString_concat(left, hrStr);
    AEString_concat(full, left);


    return;
}

// ---- getShipGroup_e4e14.cpp ----


// Per-group builders for the special param_1 == 0xf sub-cases that build articulated
// transform chains (capital ships). Each delegates to the engine the same way the target
// does; kept as helpers to preserve the recoverable dispatch without inlining ~1.7KB.

extern void *const gGSG_guard __attribute__((visibility("hidden")));     // DAT_000f51dc
extern void *const gGSG_canvas __attribute__((visibility("hidden")));    // shared canvas global
extern const unsigned short gGSG_resTable[] __attribute__((visibility("hidden")));   // DAT_000f520c
extern const unsigned short gGSG_meshTable[] __attribute__((visibility("hidden")));  // DAT_000f5210
extern const short gGSG_extraTable[] __attribute__((visibility("hidden")));          // DAT_000f5544
extern const unsigned gGSG_lodTable[] __attribute__((visibility("hidden")));         // DAT_000f5548 (3/group)
extern const unsigned gGSG_childTable[] __attribute__((visibility("hidden")));       // DAT_000f5550 (3/group)

// Globals::getShipGroup(int kind, int variant, bool wireframe) -> AEGeometry* via sret (this).
void Globals_getShipGroup(void *self, int kind, int variant, int wireframe)
{
    int *guardP = *(int **)gGSG_guard;
    volatile int saved = *guardP;
    void **canvasP = *(void ***)gGSG_canvas;

    if (kind == 0xf) {
        Globals_buildShipGroup0f(self, variant, *canvasP);
        goto done;
    }
    if (kind == 0xe || kind == 0xd) {
        void *geom = ::operator new(0xc0);
        int resId = (kind == 0xe) ? 0x37e7 : 0x4275;
        new ((void *)geom) AEGeometry((uint16_t)resId, (PaintCanvas *)*canvasP, false);
        unsigned t0 = 0xffffffff;
        ((PaintCanvas *)*canvasP)->TransformCreate((unsigned int *)&t0);
        ((PaintCanvas *)*canvasP)->TransformAddMesh((unsigned)t0, 0, (bool)(1));
        ((AEGeometry *)((unsigned)(long)geom))->addChild(t0);
        unsigned t1 = 0xffffffff;
        ((PaintCanvas *)*canvasP)->TransformCreate((unsigned int *)&t1);
        ((PaintCanvas *)*canvasP)->TransformAddMesh((unsigned)t1, 0, (bool)(1));
        ((AEGeometry *)((unsigned)(long)geom))->addChild(t1);
        unsigned short lodMeshes[2] = {0, 0};
        int dist[2];
        dist[0] = (kind == 0xe) ? 35000 : 35000;
        dist[1] = (kind == 0xe) ? 60000 : 45000;
        ((AEGeometry *)(geom))->setLodMeshes(lodMeshes, dist, 2);
        unsigned short childMeshes[1] = {0};
        ((AEGeometry *)(geom))->setLodChildMeshes(childMeshes);
        if (kind == 0xe) {
            ((AEGeometry *)(geom))->setScaling(1.0f);
        }
        goto done;
    }

    // Generic path: per-ship table-driven build indexed by `kind`.
    {
        void *geom = ::operator new(0xc0);
        new ((void *)geom) AEGeometry((uint16_t)gGSG_resTable[kind], (PaintCanvas *)*canvasP, true);
        unsigned short mesh = gGSG_meshTable[kind];
        unsigned mainT = 0xffffffff;
        unsigned mainMesh = 0xffffffff;
        if (mesh != 0xffff) {
            ((PaintCanvas *)*canvasP)->MeshCreate(mesh, &mainMesh, true);
            ((PaintCanvas *)*canvasP)->TransformCreate((unsigned int *)&mainT);
            ((PaintCanvas *)*canvasP)->TransformAddMeshId(mainT, mainMesh);
            ((AEGeometry *)((unsigned)(long)geom))->addChild(mainT);
            *(unsigned *)((char *)geom + 0x20) = mainMesh;
        }
        if (!wireframe) {
            unsigned short mat = (unsigned short)((short)kind + 0x7dc8);
            unsigned matH = 0xffffffff;
            ((PaintCanvas *)*canvasP)->MaterialCreate(mat, &matH);
            ((PaintCanvas *)*canvasP)->MeshChangeResourceMaterial(*(unsigned *)((char *)geom + 0x1c),
                                                   mat);
        }
        short extra = gGSG_extraTable[kind];
        if (extra != -1) {
            unsigned t = 0xffffffff;
            ((PaintCanvas *)*canvasP)->TransformCreate((unsigned int *)&t);
            ((PaintCanvas *)*canvasP)->TransformAddMesh((unsigned)t, 0, (bool)((int)(unsigned char)(char)extra));
            ((AEGeometry *)((unsigned)(long)geom))->addChild(t);
        }
        if (wireframe) {
            if (kind != 0x27 && kind != 0x29) {
                unsigned t = 0xffffffff;
                ((PaintCanvas *)*canvasP)->TransformCreate((unsigned int *)&t);
                ((PaintCanvas *)*canvasP)->TransformAddMesh((unsigned)t, 0, (bool)((int)(char)(-0x14 + (char)kind)));
                ((AEGeometry *)((unsigned)(long)geom))->addChild(t);
            }
        } else {
            unsigned t = 0xffffffff;
            ((PaintCanvas *)*canvasP)->TransformCreate((unsigned int *)&t);
            ((PaintCanvas *)*canvasP)->TransformAddMesh((unsigned)t, 0, (bool)((int)(char)(0x50 + (char)kind)));
            ((AEGeometry *)((unsigned)(long)geom))->addChild(t);
        }

        // Count up to 2 LOD meshes for this ship.
        unsigned lastVisibleDist = 5000;   // last accumulated LOD distance (see loop below)
        const unsigned *lod = &gGSG_lodTable[kind * 3];
        unsigned count = 0;
        for (int i = 0; i != 2; i++) {
            if (lod[i] != 0xffff) count++;
        }
        if (count != 0) {
            unsigned short *meshes = (unsigned short *)::operator new[](count << 1);
            unsigned *ids = (unsigned *)::operator new[](count * 4);
            int *dist = (int *)::operator new[](count * 4);
            int d = 5000;
            unsigned *idp = ids;
            const unsigned *src = lod;
            for (unsigned i = 0; i < count; i++) {
                unsigned m = *src;
                dist[i] = d;
                meshes[i] = (unsigned short)m;
                if (!wireframe) {
                    ((PaintCanvas *)*canvasP)->MeshCreate((unsigned short)m, idp, true);
                    ((PaintCanvas *)*canvasP)->MeshChangeResourceMaterial(*idp,
                                                           (unsigned short)((short)kind + 0x7dc8));
                }
                d += 8000;
                idp++;
                src++;
            }
            lastVisibleDist = (unsigned)d;
            if (wireframe) {
                ((AEGeometry *)(geom))->setLodMeshes(meshes, dist, count);
            } else {
                ((AEGeometry *)(geom))->setLodMeshesWithMeshIds(meshes, ids, dist, count);
            }

            const unsigned *childSrc = &gGSG_childTable[kind * 3];
            int childCount = 0;
            for (int i = 0; i != 2; i++) {
                if (childSrc[i] != 0xffff) childCount++;
            }
            if (childCount != 0) {
                unsigned short *childMeshes = (unsigned short *)::operator new[](childCount << 1);
                for (int i = 0; i != childCount; i++) {
                    childMeshes[i] = (unsigned short)childSrc[i];
                }
                ((AEGeometry *)(geom))->setLodChildMeshes(childMeshes);
                ::operator delete[](childMeshes);
            }
            ::operator delete[](meshes);
            ::operator delete[](dist);
        }
        ((AEGeometry *)(geom))->setLodLastVisibleDistance(lastVisibleDist);
    }

done:
    return;
}

// ---- getRandomEnemyFighter_e49dc.cpp ----
extern void *const gREF_dlc __attribute__((visibility("hidden")));
extern void *const gREF_rng1 __attribute__((visibility("hidden")));
extern void *const gREF_rng2 __attribute__((visibility("hidden")));
extern const int gREF_table __attribute__((visibility("hidden")));

unsigned Globals::getRandomEnemyFighter(int kind) {
    Globals *self = this;
    (void)self;
    int t = kind;
    if ((unsigned)(kind - 9) > 1) {
        t = 8;
    }
    if (kind <= 3) {
        t = kind;
    }
    unsigned r;
    if (t == 1) {
        if (((Status *)(*(int *)gREF_dlc))->dlc1Won() == 0) {
            r = 9;
        } else {
            int n = AERandom_nextIntB(*(int *)gREF_rng1, 0x64);
            r = 0x27;
            if (n < 0x55) {
                r = 0x29;
            }
            if (n < 0x3c) {
                r = 9;
            }
        }
    } else if (t == 9) {
        r = 8;
    } else if (t == 0xa) {
        r = 0x2c;
    } else {
        const int *table = &gREF_table;
        int *rng = (int *)gREF_rng2;
        do {
            do {
                r = AERandom_nextIntB(*rng, 0x25);
            } while ((unsigned)((r & ~4u) - 9) < 2);
        } while (((r < 0x10) && ((1u << r & 0x8101u) != 0)) ||
                 (table[r] != t));
    }
    return r;
}

// ---- drawLines_e4608.cpp ----
extern void *const gDL2_canvas __attribute__((visibility("hidden")));     // DAT_000f4680
extern void *const gDL2_lineHeight __attribute__((visibility("hidden"))); // DAT_000f4684

// Globals::drawLines(uint, Array<String*>*, int, int, uint, bool) — 7-decl form.
// param_1(font), lines(r2), baseX(r3), startY(stack r7+0xc), rightX(param_5 r7+...), centered(stack).
// When NOT centered: dx = rightX - GetTextWidth(); when centered: dx stays 0.
void Globals_drawLines7(unsigned font, Array<int> *lines, int baseX, int startY,
                                   unsigned rightX, int centered)
{
    int *cv = (int *)gDL2_canvas;          // global value (pointer); deref'd each iteration
    int **lh = (int **)gDL2_lineHeight;    // global value (pointer)
    int yacc = startY;
    int dx = 0;
    for (unsigned i = 0; i < lines->size(); i++) {
        if (centered == 0) {
            int w = ((PaintCanvas *)(long)*cv)->GetTextWidth(font, (void *)(uintptr_t)(*lines)[i]);
            dx = (int)rightX - w;
        }
        ((PaintCanvas *)(long)*cv)->DrawString(font, (void *)(uintptr_t)(*lines)[i], dx + baseX, yacc, false);
        yacc += *(int *)((char *)*lh + 4);
    }
}

// ---- createBillBoard_e3e08.cpp ----
// Distinct PaintCanvas::MeshCreate overload (explicit vertex/face/uv counts).
// per-face index triple setter
// per-vertex UV/attr setter (variadic-ish; extra floats are uv coords)
// per-vertex position setter (x, y, z)

extern void *const gCBB_counter __attribute__((visibility("hidden")));   // DAT_000f40a0
extern void *const gCBB_canvas __attribute__((visibility("hidden")));    // DAT_000f40a4

// Globals::createBillBoard(int p1, int height, float u0, float v0, float u1, float v1, int width)
// p1 ignored layout-wise; height in r1, width passed on stack(param_7).
void Globals_createBillBoard(int p1, int height, float u0, float v0, float u1, float v1,
                                        int width)
{
    (void)p1;
    unsigned *counter = *(unsigned **)gCBB_counter;
    int *canvasP = *(int **)gCBB_canvas;
    int snapshot = *counter;

    long long mesh64 = 0;
    ((PaintCanvas *)(long)*canvasP)->MeshCreate((unsigned short)0xc, (unsigned short)6, (signed char)0x13, (unsigned short)0, (unsigned int *)&mesh64);
    int mesh = (int)mesh64;
    int cv = *canvasP;

    Mesh_setFace((void *)(long)cv, mesh, 0, 0, 1, 2);
    Mesh_setFace((void *)(long)cv, mesh, 1, 2, 1, 3);
    Mesh_setFace((void *)(long)cv, mesh, 2, 4, 5, 6);
    Mesh_setFace((void *)(long)cv, mesh, 3, 6, 5, 7);
    Mesh_setFace((void *)(long)cv, mesh, 4, 8, 9, 10);
    Mesh_setFace((void *)(long)cv, mesh, 5, 10, 9, 0xb);

    // UV coords: pairs (u0,v0) and (u0,v1) drive the texture mapping of each face.
    Mesh_setUV((void *)(long)cv, mesh, 0, u0, 0);
    Mesh_setUV((void *)(long)cv, mesh, 1, u1, 0);
    Mesh_setUV((void *)(long)cv, mesh, 2, v1, u0);
    Mesh_setUV((void *)(long)cv, mesh, 3, v1, v0);
    Mesh_setUV((void *)(long)cv, mesh, 4, u0, 0);
    Mesh_setUV((void *)(long)cv, mesh, 5, u1, 0);
    Mesh_setUV((void *)(long)cv, mesh, 6, v1, u0);
    Mesh_setUV((void *)(long)cv, mesh, 7, v1, v0);
    Mesh_setUV((void *)(long)cv, mesh, 8, u0, 0);
    Mesh_setUV((void *)(long)cv, mesh, 9, u1, 0);
    Mesh_setUV((void *)(long)cv, mesh, 10, v1, u0);
    Mesh_setUV((void *)(long)cv, mesh, 0xb, v1, v0);

    float pw = VectorSignedToFloat(width, 0);
    float nh = VectorSignedToFloat(-height, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 0, nh, 0, pw);
    float nw = VectorSignedToFloat(-width, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 1, nh, 0, nw);
    float ph = VectorSignedToFloat(height, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 2, ph, 0, pw);
    Mesh_setVertex((void *)(long)cv, mesh, 3, ph, 0, nw);
    Mesh_setVertex((void *)(long)cv, mesh, 4, 0, nh, pw);
    Mesh_setVertex((void *)(long)cv, mesh, 5, 0, nh, nw);
    Mesh_setVertex((void *)(long)cv, mesh, 6, 0, ph, pw);
    Mesh_setVertex((void *)(long)cv, mesh, 7, 0, ph, nw);
    Mesh_setVertex((void *)(long)cv, mesh, 8, nh, ph, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 9, nh, nh, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 10, ph, ph, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 0xb, ph, nh, 0);

    return;
}

// ---- getWreckCollision_e4bb8.cpp ----
// Vector scale: in-place multiply of a Vector by a scalar, returns resulting magnitude/component.
// BoundingSphere(center.x, center.y, center.z, radius)  (extra fp regs are scratch).
// BoundingAAB(maxX, maxY, maxZ, minX, minY, minZ) — 6 floats from the doubled extents.
extern "C" void BoundingAAB_ctor(void *self, float x0, float y0, float z0, float x1, float y1,
                                 float z1);

extern void *const gGWC_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4e10

// Globals::getWreckCollision(int kind, AEGeometry* geom) — kind in r1.
void Globals_getWreckCollision(void *retSlot, int kind, void *geom)
{
    (void)retSlot;
    int *guardP = *(int **)gGWC_guardHolder;
    volatile int saved = *guardP;

    void *fr = ::operator new(1);
    FileRead_ctor(fr);
    int *data = (int *)((FileRead *)fr)->loadWreckCollision(kind);
    ::operator delete(FileRead_dtor(fr));

    void *outArr = 0;
    if (data != 0) {
        int count = **(int **)((char *)data + 4);

        float v[3] = {0, 0, 0};       // local_40..local_38 vector
        float c[3] = {0, 0, 0};       // local_34/2c/30 scalar parts

        outArr = ::operator new(0xc);
        ArrayBV_ctor(outArr);
        ArraySetLength_BV((unsigned)count, outArr);

        int pos = 1;
        for (int i = 0; i < count; i++) {
            int *base = *(int **)((char *)data + 4);
            int kindWord = base[pos];
            void *bound = 0;
            if (kindWord == 1) {
                int *rec = base + pos;
                c[2] = (float)(-base[pos + 1]);
                c[0] = (float)rec[2];
                c[1] = (float)rec[3];
                v[0] = (float)rec[4];
                v[1] = (float)rec[5];
                v[2] = (float)rec[6];
                float mag = VectorScale(v, c[1]);
                if (geom == 0) {
                    mag = VectorScale(c, mag);
                    VectorScale(v, mag);
                }
                bound = ::operator new(0x2c);
                BoundingAAB_ctor(bound, v[2] + v[2], c[1] + c[1], v[1] + v[1], c[2], c[0], v[0]);
                pos += 7;
            } else if (kindWord == 0) {
                int *rec = base + pos;
                c[2] = (float)(-base[pos + 1]);
                c[0] = (float)rec[2];
                c[1] = (float)rec[3];
                v[0] = (float)rec[4];
                VectorScale(v, v[0]);
                float mag = v[0];
                if (v[0] < 0.0f) {
                    mag = VectorScale(v, v[0]);
                }
                if (geom == 0) {
                    mag = VectorScale(c, mag);
                    VectorScale(v, mag);
                }
                bound = ::operator new(0x48);
                BoundingSphere_ctor(bound, c[2], c[1], c[0], v[0]);
                pos += 5;
            } else {
                pos += 1;
                continue;
            }
            *(void **)(*(char **)((char *)outArr + 4) + i * 4) = bound;
        }

        ArrayRelease_int(data);
        ::operator delete(ArrayInt_dtor(data));
    }

    return;
}

// ---- Globals_e31e0.cpp ----
// Singleton-pointer globals zeroed/initialized by the constructor (PC-relative).
extern int **const gGC_p_f31f6 __attribute__((visibility("hidden")));   // DAT_000f33c8
extern void **const gGC_p_f31fc __attribute__((visibility("hidden")));  // DAT_000f33cc (puVar11)
extern void **const gGC_p_f3200 __attribute__((visibility("hidden")));  // DAT_000f33d0 (puVar7)
extern void **const gGC_p_f3204 __attribute__((visibility("hidden")));  // DAT_000f33d4 (puVar12)
extern void **const gGC_p_f3208 __attribute__((visibility("hidden")));  // DAT_000f33d8 (puVar9)
extern void **const gGC_p_f320e __attribute__((visibility("hidden")));  // DAT_000f33dc (puVar8 - settings obj)
extern void **const gGC_p_f3216 __attribute__((visibility("hidden")));  // DAT_000f33e0 (puVar5)
extern int **const gGC_p_f322a __attribute__((visibility("hidden")));   // DAT_000f33e4
extern int **const gGC_p_f3242 __attribute__((visibility("hidden")));   // DAT_000f33e8
extern int **const gGC_p_f324a __attribute__((visibility("hidden")));   // DAT_000f33ec
extern int **const gGC_p_f3252 __attribute__((visibility("hidden")));   // DAT_000f33f0
extern int **const gGC_p_f325a __attribute__((visibility("hidden")));   // DAT_000f33f4
extern int **const gGC_p_f3262 __attribute__((visibility("hidden")));   // DAT_000f33f8
extern int **const gGC_p_f326a __attribute__((visibility("hidden")));   // DAT_000f33fc
extern int **const gGC_p_f3272 __attribute__((visibility("hidden")));   // DAT_000f3400
extern int **const gGC_p_f327a __attribute__((visibility("hidden")));   // DAT_000f3404
extern int **const gGC_p_f3288 __attribute__((visibility("hidden")));   // DAT_000f3408
extern void **const gGC_p_f32bc __attribute__((visibility("hidden")));  // DAT_000f340c (puVar5 secondary)
extern char **const gGC_p_f32ec __attribute__((visibility("hidden")));  // DAT_000f3410
extern char **const gGC_p_f32f4 __attribute__((visibility("hidden")));  // DAT_000f3414
extern char **const gGC_p_f3300 __attribute__((visibility("hidden")));  // DAT_000f3418
extern void **const gGC_p_f330c __attribute__((visibility("hidden")));  // DAT_000f341c (puVar7 secondary)
extern void **const gGC_p_f3316 __attribute__((visibility("hidden")));  // DAT_000f3420 (puVar5)
extern void **const gGC_p_f3366 __attribute__((visibility("hidden")));  // DAT_000f3424 (puVar11)
extern int **const gGC_p_f3368 __attribute__((visibility("hidden")));   // DAT_000f3428
extern int **const gGC_p_f3372 __attribute__((visibility("hidden")));   // DAT_000f342c
extern void **const gGC_p_f337c __attribute__((visibility("hidden")));  // DAT_000f3430 (puVar9)
extern int **const gGC_p_f337e __attribute__((visibility("hidden")));   // DAT_000f3434
extern int **const gGC_p_f3388 __attribute__((visibility("hidden")));   // DAT_000f3438
extern int **const gGC_p_f3390 __attribute__((visibility("hidden")));   // DAT_000f343c
extern void **const gGC_p_f339e __attribute__((visibility("hidden")));  // DAT_000f3440 (puVar8 secondary)

// Globals::Globals()
Globals * Globals::ctor() {
    Globals *self = this;
    void *settings = *gGC_p_f320e;     // puVar8 — the main settings/state sub-object
    int *secondary = (int *)*gGC_p_f32bc;
    void *p5 = *gGC_p_f3216;
    void *p7 = *gGC_p_f3200;
    void *p9 = *gGC_p_f3208;
    int *p11 = (int *)*gGC_p_f31fc;
    int *p12 = (int *)*gGC_p_f3204;

    **gGC_p_f31f6 = 0;
    **gGC_p_f322a = 0;
    **gGC_p_f3242 = 0;
    **gGC_p_f324a = 0;
    **gGC_p_f3252 = 0;
    **gGC_p_f325a = 0;
    **gGC_p_f3262 = 0;
    **gGC_p_f326a = 0;
    **gGC_p_f3272 = 0;
    **gGC_p_f327a = 0;
    *(int *)p5 = 0;
    **gGC_p_f3288 = 0;

    *(int *)self = 0;

    char *s = (char *)settings;
    // Vector pairs (DAT_000f33b8 / DAT_000f33c0) at +0x14 .. via vst1; model as zero/identity blocks.
    *(float *)(s + 0x00) = 0.5f;
    *(float *)(s + 0x04) = 0.5f;
    *(float *)(s + 0x08) = 0.5f;
    *(short *)(s + 0x0c) = 0x101;
    *(short *)(s + 0x10) = 0;
    *(float *)(s + 0x24) = 0.5f;
    *(float *)(s + 0x28) = 1.0f;
    *(float *)(s + 0x2c) = 0.5f;
    *(unsigned char *)(s + 0x0e) = 1;
    *(unsigned char *)(s + 0x30) = 1;
    *(short *)(s + 0x40) = 0;
    *(int *)(s + 0x50) = 0;
    *(unsigned char *)(s + 0x39) = 0;
    *(int *)(s + 0x35) = 0;
    *(int *)(s + 0x31) = 0;
    secondary[1] = 0;
    *(unsigned char *)((char *)secondary + 0x13) = 0;
    *(unsigned char *)(s + 0x3f) = 0;
    *(int *)(s + 0x3b) = 0;
    *(unsigned char *)(s + 0x4e) = 0;
    *(short *)(s + 0x4c) = 0;
    *(int *)(s + 0x48) = 0;
    *secondary = 0;

    **gGC_p_f32ec = 0;
    **gGC_p_f32f4 = 0;
    *p11 = -1;
    **gGC_p_f3300 = 0;
    *(int *)p7 = -1;
    *(int *)p9 = 0;
    *p12 = -1;

    void *p7b = *gGC_p_f330c;
    void *p5b = *gGC_p_f3316;

    float fv = *(float *)(s + 0x44);
    int v54 = 0x247;
    if (1.0f <= fv) v54 = 0x33e;
    if (fv <= 0.0f) v54 = 0x19f;
    *(int *)(s + 0x54) = v54;

    int v58 = 0x201;
    if (1.0f <= fv) v58 = 0x2da;
    if (fv <= 0.0f) v58 = 0x16d;
    *(int *)(s + 0x58) = v58;

    int *p11b = (int *)*gGC_p_f3366;
    **gGC_p_f3368 = 0;
    **gGC_p_f3372 = 0;
    void *p9b = *gGC_p_f337c;
    **gGC_p_f337e = -1;
    **gGC_p_f3388 = 0;
    **gGC_p_f3390 = 0;
    *(int *)p9b = 0;
    *(short *)(s + 0x60) = 0x100;
    *p11b = 0;
    void *p8b = *gGC_p_f339e;
    *(int *)p5b = 0;
    *(int *)p7b = 0;
    *(int *)p8b = 0;
    self->field_0x4 = 0;
    return self;
}

// ---- _Globals_e3444.cpp ----
// Each singleton lives behind a hidden PC-relative pointer-to-pointer global.
extern void **const gG_recordHandler __attribute__((visibility("hidden")));  // DAT_000f35e0
extern void **const gG_galaxy __attribute__((visibility("hidden")));         // DAT_000f35e4
extern void **const gG_status __attribute__((visibility("hidden")));         // DAT_000f35e8
extern void **const gG_gameText __attribute__((visibility("hidden")));       // DAT_000f35ec
extern void **const gG_random __attribute__((visibility("hidden")));         // DAT_000f35f0
extern void **const gG_layout __attribute__((visibility("hidden")));         // DAT_000f35f4
extern void **const gG_generator __attribute__((visibility("hidden")));      // DAT_000f35f8
extern void **const gG_polyObj __attribute__((visibility("hidden")));        // DAT_000f35fc (vtable dtor)
extern void **const gG_fmod __attribute__((visibility("hidden")));           // DAT_000f3600
extern void **const gG_items __attribute__((visibility("hidden")));          // DAT_000f3604
extern void **const gG_ships __attribute__((visibility("hidden")));          // DAT_000f3608
extern void **const gG_achievements __attribute__((visibility("hidden")));   // DAT_000f360c
extern void **const gG_imageFactory __attribute__((visibility("hidden")));   // DAT_000f3610
extern int **const gG_tail __attribute__((visibility("hidden")));            // DAT_000f3614

// Globals::~Globals()
void * Globals::dtor() {
    Globals *self = this;
    void **rhSlot = gG_recordHandler;
    if (*rhSlot != 0) {
        ((RecordHandler *)(*rhSlot))->saveOptions();
    }
    void **galSlot = gG_galaxy;
    if (*galSlot != 0) {
        ::operator delete(Galaxy_dtor(*galSlot));
    }
    *galSlot = 0;
    void **statSlot = gG_status;
    if (*statSlot != 0) {
        ::operator delete(Status_dtor(*statSlot));
    }
    *statSlot = 0;
    void **gtSlot = gG_gameText;
    if (*gtSlot != 0) {
        ::operator delete(GameText_dtor(*gtSlot));
    }
    *gtSlot = 0;
    void **rngSlot = gG_random;
    if (*rngSlot != 0) {
        ::operator delete(AERandom_dtor(*rngSlot));
    }
    *rngSlot = 0;
    void **laySlot = gG_layout;
    if (*laySlot != 0) {
        ::operator delete(Layout_dtor(*laySlot));
    }
    *laySlot = 0;
    void **genSlot = gG_generator;
    if (*genSlot != 0) {
        ::operator delete(Generator_dtor(*genSlot));
    }
    *genSlot = 0;
    if (*rhSlot != 0) {
        ::operator delete(((RecordHandler *)(*rhSlot))->dtor());
    }
    *rhSlot = 0;
    void **polySlot = gG_polyObj;
    void *poly = *polySlot;
    if (poly != 0) {
        void (**vt)(void *) = *(void (***)(void *))poly;
        vt[1](poly);
    }
    *polySlot = 0;
    void **fmodSlot = gG_fmod;
    if (*fmodSlot != 0) {
        ::operator delete(FModSound_dtor(*fmodSlot));
    }
    *fmodSlot = 0;
    void **itemSlot = gG_items;
    if (*itemSlot != 0) {
        ArrayReleaseClasses_Item(*itemSlot);
        if (*itemSlot != 0) {
            ::operator delete(ArrayItem_dtor(*itemSlot));
        }
    }
    *itemSlot = 0;
    void **shipSlot = gG_ships;
    if (*shipSlot != 0) {
        ArrayReleaseClasses_Ship(*shipSlot);
        if (*shipSlot != 0) {
            ::operator delete(ArrayShip_dtor(*shipSlot));
        }
    }
    *shipSlot = 0;
    if (*galSlot != 0) {
        ::operator delete(Galaxy_dtor(*galSlot));
    }
    *galSlot = 0;
    void **achSlot = gG_achievements;
    if (*achSlot != 0) {
        ::operator delete(Achievements_dtor(*achSlot));
    }
    *achSlot = 0;
    if (*statSlot != 0) {
        ::operator delete(Status_dtor(*statSlot));
    }
    *statSlot = 0;
    void **ifSlot = gG_imageFactory;
    if (*ifSlot != 0) {
        ::operator delete(ImageFactory_dtor(*ifSlot));
    }
    *ifSlot = 0;

    void *selfArr = self->field_0x4;
    if (selfArr != 0) {
        ArrayRelease_int(selfArr);
        if (self->field_0x4 != 0) {
            ::operator delete(ArrayInt_dtor(self->field_0x4));
        }
    }
    self->field_0x4 = 0;
    **gG_tail = 0;
    return self;
}

// ---- drawLines_e458c.cpp ----
extern void *const gDL_canvas __attribute__((visibility("hidden")));
extern void *const gDL_lineHeight __attribute__((visibility("hidden")));

// Globals::drawLines(uint, Array<String*>*, int, int, bool) — 5-decl form; the body
// consumes 6 incoming values: p1(unused), font(r1), lines(r2), baseX(r3), startY(stack0),
// centered(stack1).
void Globals_drawLines5(unsigned p1, void *font, Array<int> *lines, int baseX,
                                   int startY, int centered)
{
    (void)p1;
    int *cv = (int *)gDL_canvas;          // global value (pointer); deref'd each iteration
    int **lh = (int **)gDL_lineHeight;    // global value (pointer)
    int yacc = startY;
    int dx = 0;
    for (unsigned i = 0; i < lines->size(); i++) {
        if (centered != 0) {
            int w = ((PaintCanvas *)(long)*cv)->GetTextWidth((unsigned)(uintptr_t)font, (void *)(uintptr_t)(*lines)[i]);
            dx = -(w >> 1);
        }
        ((PaintCanvas *)(long)*cv)->DrawString((unsigned)(uintptr_t)font, (void *)(uintptr_t)(*lines)[i], dx + baseX, yacc, false);
        yacc += *(int *)((char *)*lh + 4);
    }
}

// ---- setCoordsFire_e8764.cpp ----
// Hidden float constants.
extern const float gSCF_b90 __attribute__((visibility("hidden")));
extern const float gSCF_b10 __attribute__((visibility("hidden")));
extern const float gSCF_b14 __attribute__((visibility("hidden")));
extern const float gSCF_b7c __attribute__((visibility("hidden")));
extern const float gSCF_b80 __attribute__((visibility("hidden")));
extern const float gSCF_b84 __attribute__((visibility("hidden")));
extern const float gSCF_b88 __attribute__((visibility("hidden")));
extern const float gSCF_b8c __attribute__((visibility("hidden")));
extern const float gSCF_b94 __attribute__((visibility("hidden")));
extern const float gSCF_b98 __attribute__((visibility("hidden")));
extern const float gSCF_b9c __attribute__((visibility("hidden")));
extern const float gSCF_ba0 __attribute__((visibility("hidden")));
extern const float gSCF_ba4 __attribute__((visibility("hidden")));
extern const float gSCF_ba8 __attribute__((visibility("hidden")));
extern const float gSCF_bac __attribute__((visibility("hidden")));
extern const float gSCF_bb0 __attribute__((visibility("hidden")));
extern const float gSCF_bb4 __attribute__((visibility("hidden")));
extern const float gSCF_bb8 __attribute__((visibility("hidden")));
extern const float gSCF_bbc __attribute__((visibility("hidden")));
extern const float gSCF_bc0 __attribute__((visibility("hidden")));
extern const float gSCF_bc4 __attribute__((visibility("hidden")));
extern const float gSCF_bc8 __attribute__((visibility("hidden")));
extern const float gSCF_bcc __attribute__((visibility("hidden")));
extern const float gSCF_bd0 __attribute__((visibility("hidden")));
extern const float gSCF_bd4 __attribute__((visibility("hidden")));
extern const float gSCF_bd8 __attribute__((visibility("hidden")));
extern const float gSCF_bdc __attribute__((visibility("hidden")));
extern const float gSCF_be0 __attribute__((visibility("hidden")));
extern const float gSCF_be4 __attribute__((visibility("hidden")));
extern const float gSCF_be8 __attribute__((visibility("hidden")));
extern const float gSCF_bec __attribute__((visibility("hidden")));
extern const float gSCF_bf0 __attribute__((visibility("hidden")));

extern void *const gSCF_isPhone __attribute__((visibility("hidden")));   // DAT_000f8b0c
extern void *const gSCF_flagB __attribute__((visibility("hidden")));     // DAT_000f8b18
extern void *const gSCF_screenW __attribute__((visibility("hidden")));   // DAT_000f8b1c
extern void *const gSCF_flagC __attribute__((visibility("hidden")));     // DAT_000f8b70
extern void *const gSCF_screenW2 __attribute__((visibility("hidden")));  // DAT_000f8b74
extern void *const gSCF_objA __attribute__((visibility("hidden")));      // DAT_000f8b78
extern void *const gSCF_flagD __attribute__((visibility("hidden")));     // DAT_000f8bf4
extern void *const gSCF_objB __attribute__((visibility("hidden")));      // DAT_000f8bf8
extern void *const gSCF_flagE __attribute__((visibility("hidden")));     // DAT_000f8bfc
extern void *const gSCF_flagF __attribute__((visibility("hidden")));     // DAT_000f8c00
extern void *const gSCF_screenW3 __attribute__((visibility("hidden")));  // DAT_000f8c04
extern void *const gSCF_objC __attribute__((visibility("hidden")));      // DAT_000f8c08
extern void *const gSCF_flagG __attribute__((visibility("hidden")));     // DAT_000f8c0c
extern void *const gSCF_flagH __attribute__((visibility("hidden")));     // DAT_000f8c10
extern void *const gSCF_flagI __attribute__((visibility("hidden")));     // DAT_000f8c14
extern void *const gSCF_flagJ __attribute__((visibility("hidden")));     // DAT_000f8c18

static inline char rf(void *const g) { return **(char **)&g; }
static inline int rint(void *const g) { return **(int **)&g; }
static inline int *robj(void *const g) { return *(int **)&g; }

static inline unsigned short clampU(float v)
{
    return (unsigned short)((0.0f < v) ? (short)(int)v : 0);
}

// Globals::setCoordsFire(int p1, int p2, uint p3, uint p4, uint& o5, ushort& o6..ushort& o17)
void Globals_setCoordsFire(void *self, int p1, int p2, unsigned p3, unsigned p4,
                                      unsigned *o5, unsigned short *o6, unsigned short *o7,
                                      unsigned short *o8, unsigned short *o9, unsigned short *o10,
                                      unsigned short *o11, unsigned short *o12, unsigned short *o13,
                                      unsigned short *o14, unsigned short *o15, unsigned short *o16,
                                      unsigned short *o17)
{
    (void)self;
    char isPhone = rf(gSCF_isPhone);
    float fbase = gSCF_b14;
    float fwid = gSCF_b10;
    if (isPhone == 0) {
        bool b = (rf(gSCF_flagB) == 0);
        fwid = b ? gSCF_b84 : gSCF_b80;
        fbase = b ? gSCF_b8c : gSCF_b88;
    }

    int colDelta = rint(gSCF_screenW) - p2;
    float fcol = VectorSignedToFloat(colDelta, 0);
    float fp1 = VectorSignedToFloat(p1, 0);
    float fmax = fp1;
    if (fwid + fcol < fp1) {
        fmax = gSCF_b7c;
        if (isPhone == 0) {
            fmax = (rf(gSCF_flagC) == 0) ? gSCF_b84 : gSCF_b80;
        }
        fmax = fmax + fcol;
    }

    int iv;
    float adj13 = gSCF_b90;
    float wField;
    if (fbase < fmax) {
        float t = gSCF_b7c;
        if (isPhone == 0) {
            t = (rf(gSCF_flagE) == 0) ? gSCF_b84 : gSCF_b80;
        }
        float fp1b = fp1;
        if (t + fcol < fp1) {
            fp1b = gSCF_b7c;
            if (isPhone == 0) {
                fp1b = (rf(gSCF_flagF) == 0) ? gSCF_b84 : gSCF_b80;
            }
            fp1b = fp1b + fcol;
        }
        wField = VectorSignedToFloat(rint(gSCF_screenW3) - p2, 0);
        iv = (int)fp1b;
        robj(gSCF_objC)[0x58 / 4] = (int)fp1b;
        adj13 = gSCF_b90;
        if (isPhone == 0) {
            char flag6 = rf(gSCF_flagG);
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        }
    } else {
        if (isPhone == 0) {
            wField = VectorSignedToFloat(rint(gSCF_screenW2) - p2, 0);
            char flag6 = rf(gSCF_flagD);
            iv = (flag6 == 0) ? 0x96 : 0x12c;
            robj(gSCF_objB)[0x58 / 4] = iv;
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        } else {
            iv = 0xd2;
            wField = VectorSignedToFloat(rint(gSCF_screenW2) - p2, 0);
            robj(gSCF_objA)[0x58 / 4] = 0xd2;
        }
    }

    *o6 = clampU(wField + adj13);
    *o7 = (unsigned short)iv;
    *o8 = (unsigned short)(*o6 + (short)(p2 >> 1));
    *o9 = (unsigned short)(*o7 + (short)(p2 >> 1));

    float tail;
    unsigned short u16;
    if (isPhone == 0) {
        char flag6 = rf(gSCF_flagH);
        float a10 = (flag6 == 0) ? gSCF_bb8 : gSCF_bb4;
        *o10 = clampU(a10 + VectorUnsignedToFloat(*o6, 0));
        float a11 = (flag6 == 0) ? 4.0f : 8.0f;
        *o11 = clampU(a11 + VectorUnsignedToFloat(*o7, 0));
        float a12 = (flag6 == 0) ? 15.0f : 28.0f;
        *o12 = clampU(a12 + VectorUnsignedToFloat(*o6, 0));
        float a13 = (flag6 == 0) ? 13.0f : 27.0f;
        *o13 = clampU(a13 + VectorUnsignedToFloat(*o7, 0));
        float a16 = (flag6 == 0) ? gSCF_bc0 : gSCF_bbc;
        u16 = clampU(a16 + VectorUnsignedToFloat(*o6, 0));
        tail = (flag6 == 0) ? gSCF_bc8 : gSCF_bc4;
    } else {
        *o10 = clampU(VectorUnsignedToFloat(*o6, 0) + gSCF_b9c);
        *o11 = clampU(VectorUnsignedToFloat(*o7, 0) + gSCF_ba0);
        *o12 = clampU(VectorUnsignedToFloat(*o6, 0) + gSCF_ba4);
        *o13 = clampU(VectorUnsignedToFloat(*o7, 0) + gSCF_ba8);
        u16 = clampU(VectorUnsignedToFloat(*o6, 0) + gSCF_bac);
        tail = gSCF_bb0;
    }

    *o16 = u16;
    *o17 = clampU(VectorUnsignedToFloat(*o7, 0) - tail);

    unsigned short u14;
    float t15;
    if (colDelta < iv) {
        *o5 = p3;
        float w = VectorUnsignedToFloat(*o6, 0);
        if (isPhone != 0) {
            u14 = clampU(w + -2.0f);
            t15 = gSCF_be4;
        } else {
            char flag = rf(gSCF_flagI);
            float a = (flag == 0) ? gSCF_be8 : -2.0f;
            u14 = clampU(a + gSCF_be4);
            t15 = (flag == 0) ? gSCF_bf0 : gSCF_bec;
        }
    } else {
        *o5 = p4;
        float w = VectorUnsignedToFloat(*o6, 0);
        if (isPhone != 0) {
            u14 = clampU(w + gSCF_bcc);
            t15 = gSCF_bd0;
        } else {
            char flag = rf(gSCF_flagJ);
            float a = (flag == 0) ? gSCF_bd8 : gSCF_bd4;
            u14 = clampU(a + gSCF_bd0);
            t15 = (flag == 0) ? gSCF_be0 : gSCF_bdc;
        }
    }

    *o14 = u14;
    *o15 = clampU(t15 + VectorUnsignedToFloat(*o7, 0));
}

// ---- releaseResources_e479c.cpp ----
// Two hidden PC-relative pointer-to-pointer globals (deref'd twice).
extern void *const gRR_canvas __attribute__((visibility("hidden")));
extern void *const gRR_arg __attribute__((visibility("hidden")));

void Globals_releaseResources()
{
    ((PaintCanvas *)*(void **)gRR_canvas)->ReleaseAllResources();
    return Globals_releaseResources_tail(*(void **)gRR_arg);
}

// ---- loadFont_e47c4.cpp ----
extern "C" void Globals_loadFont_tail(int canvas, int font, int zero);  // veneer at 0x1ac2c8

// Per-font canvas+font globals (PC-relative). canvasP/fontP each *(void**) -> object pointer.
extern void *const gLF_canvas9 __attribute__((visibility("hidden")));   // DAT_000f4980
extern void *const gLF_font9 __attribute__((visibility("hidden")));     // DAT_000f4984
extern void *const gLF_canvas10 __attribute__((visibility("hidden")));  // DAT_000f4990
extern void *const gLF_font10 __attribute__((visibility("hidden")));    // DAT_000f4994
extern void *const gLF_canvas11 __attribute__((visibility("hidden")));  // DAT_000f499c
extern void *const gLF_font11 __attribute__((visibility("hidden")));    // DAT_000f49a0
extern void *const gLF_canvas14 __attribute__((visibility("hidden")));  // DAT_000f49a4
extern void *const gLF_font14 __attribute__((visibility("hidden")));    // DAT_000f49a8
extern void *const gLF_canvasD __attribute__((visibility("hidden")));   // DAT_000f49b0 default
extern void *const gLF_font15 __attribute__((visibility("hidden")));    // DAT_000f49b4
extern void *const gLF_fontDef __attribute__((visibility("hidden")));   // DAT_000f49c0
extern void *const gLF_canvasMain __attribute__((visibility("hidden"))); // DAT_000f49d0
extern void *const gLF_fontMain __attribute__((visibility("hidden")));  // DAT_000f49d4
extern void *const gLF_fontExtra __attribute__((visibility("hidden"))); // DAT_000f49d8

// Locale-related boolean flag globals: each *(char**) -> a byte.
extern void *const gLF_flagA __attribute__((visibility("hidden")));     // DAT_000f4988 / shared
extern void *const gLF_flagB __attribute__((visibility("hidden")));     // DAT_000f498c
extern void *const gLF_flagC __attribute__((visibility("hidden")));     // DAT_000f49b8
extern void *const gLF_flagD __attribute__((visibility("hidden")));     // DAT_000f49bc
extern void *const gLF_flagE __attribute__((visibility("hidden")));     // DAT_000f49c4
extern void *const gLF_flagF __attribute__((visibility("hidden")));     // DAT_000f49c8
extern void *const gLF_flagG __attribute__((visibility("hidden")));     // DAT_000f49cc

static inline char flag(void *const g) { return **(char **)&g; }

// Globals::loadFont(int kind) — kind in r1.
void Globals_loadFont(void *self, int kind)
{
    (void)self;
    void **canvasP;
    void **fontP;
    unsigned glyph;
    short spacing;
    unsigned char isMainFontPersian = 1;

    switch (kind) {
    case 9: {
        canvasP = *(void ***)gLF_canvas9;
        fontP = *(void ***)gLF_font9;
        ((PaintCanvas *)*canvasP)->FontCreate((unsigned short)0x2d74, (unsigned int *)(long)*fontP, false);
        if (flag(gLF_flagA) != 0) {
            spacing = -6;
        } else {
            spacing = flag(gLF_flagB) != 0 ? -8 : -4;
        }
        ((PaintCanvas *)*canvasP)->FontSetSpacing(*(unsigned *)fontP, spacing);
        isMainFontPersian = 0;
        goto epilogue;
    }
    case 10:
        canvasP = *(void ***)gLF_canvas10;
        fontP = *(void ***)gLF_font10;
        glyph = 0x2d78;
        break;
    case 0xb:
        canvasP = *(void ***)gLF_canvas11;
        fontP = *(void ***)gLF_font11;
        glyph = 0x2d76;
        break;
    case 0xe:
        canvasP = *(void ***)gLF_canvas14;
        fontP = *(void ***)gLF_font14;
        glyph = 0x2d7c;
        break;
    default: {
        canvasP = *(void ***)gLF_canvasD;
        if (kind == 0xf) {
            fontP = *(void ***)gLF_font15;
            ((PaintCanvas *)*canvasP)->FontCreate((unsigned short)0x2d7e, (unsigned int *)(long)*fontP, false);
            if (flag(gLF_flagC) != 0) {
                spacing = -7;
            } else {
                spacing = flag(gLF_flagD) != 0 ? -10 : -5;
            }
        } else {
            fontP = *(void ***)gLF_fontDef;
            ((PaintCanvas *)*canvasP)->FontCreate((unsigned short)0x457, (unsigned int *)(long)*fontP, false);
            if (flag(gLF_flagE) != 0) {
                spacing = -5;
            } else if (flag(gLF_flagF) != 0) {
                spacing = -4;
            } else if (flag(gLF_flagG) != 0) {
                spacing = -4;
            } else {
                spacing = -2;
            }
        }
        ((PaintCanvas *)*canvasP)->FontSetSpacing(*(unsigned *)fontP, spacing);
        goto setMain;
    }
    }

    // cases 10/11/14: common creation tail
    ((PaintCanvas *)*canvasP)->FontCreate((unsigned short)glyph, (unsigned int *)(long)*fontP, false);
    if (flag(gLF_flagA) != 0) {
        spacing = -6;
    } else {
        spacing = flag(gLF_flagB) != 0 ? -8 : -4;
    }
    ((PaintCanvas *)*canvasP)->FontSetSpacing(*(unsigned *)fontP, spacing);

setMain:
    isMainFontPersian = 1;
epilogue: {
    int *mainCanvas = *(int **)gLF_canvasMain;
    unsigned *mainFont = *(unsigned **)gLF_fontMain;
    int cv = *mainCanvas;
    *(unsigned char *)(cv + 0x1c) = isMainFontPersian;
    ((PaintCanvas *)(long)cv)->FontCreate((unsigned short)0x51e, (unsigned int *)mainFont, false);
    ((PaintCanvas *)(long)*mainCanvas)->FontSetSpacing(*mainFont, 0);
    unsigned *extra = *(unsigned **)gLF_fontExtra;
    ((PaintCanvas *)(long)*mainCanvas)->FontCreate((unsigned short)0x2d7a, (unsigned int *)extra, false);
    Globals_loadFont_tail(*mainCanvas, (int)*extra, 0);
}
}

// ---- init_e3618.cpp ----
// Hidden PC-relative globals.
extern int **const gI_mission __attribute__((visibility("hidden")));     // DAT_000f38c8
extern void **const gI_settings __attribute__((visibility("hidden")));   // DAT_000f38cc (puVar14)
extern int **const gI_flagFFFF __attribute__((visibility("hidden")));    // DAT_000f38d0 (puVar6)
extern int **const gI_uVar7slot __attribute__((visibility("hidden")));   // DAT_000f38d4 (puVar10)
extern char **const gI_langFlag __attribute__((visibility("hidden")));   // DAT_000f38d8 (pcVar11)
extern char **const gI_zeroByte __attribute__((visibility("hidden")));   // DAT_000f38dc (puVar12)
extern void ***const gI_galaxy __attribute__((visibility("hidden")));    // DAT_000f38e0
extern void ***const gI_achieve __attribute__((visibility("hidden")));   // DAT_000f38e4
extern void ***const gI_status __attribute__((visibility("hidden")));    // DAT_000f38e8
extern void ***const gI_imgFac __attribute__((visibility("hidden")));    // DAT_000f38ec
extern int ***const gI_items __attribute__((visibility("hidden")));      // DAT_000f38f0
extern int ***const gI_ships __attribute__((visibility("hidden")));      // DAT_000f38f4
extern int **const gI_engineSlot __attribute__((visibility("hidden")));  // DAT_000f38f8
extern void ***const gI_appMgr __attribute__((visibility("hidden")));    // DAT_000f38fc
extern void ***const gI_ctxSlot __attribute__((visibility("hidden")));   // DAT_000f3900
extern void ***const gI_random __attribute__((visibility("hidden")));    // DAT_000f3904
extern void ***const gI_generator __attribute__((visibility("hidden"))); // DAT_000f3908
extern void ***const gI_recHandler __attribute__((visibility("hidden")));// DAT_000f390c
extern void ***const gI_fmod __attribute__((visibility("hidden")));      // DAT_000f3910
extern void **const gI_setMusVol __attribute__((visibility("hidden")));  // DAT_000f3914 (fn ptr)
extern void **const gI_setSfxVol __attribute__((visibility("hidden")));  // DAT_000f3918 (fn ptr)
extern int ***const gI_g381c __attribute__((visibility("hidden")));      // DAT_000f391c
extern char **const gI_g381a __attribute__((visibility("hidden")));      // DAT_000f3920 (puVar12)
extern int ***const gI_g381e __attribute__((visibility("hidden")));      // DAT_000f3924 (puVar6 second)
extern int **const gI_g3822 __attribute__((visibility("hidden")));       // DAT_000f3928 (puVar10)
extern char ***const gI_g3824 __attribute__((visibility("hidden")));     // DAT_000f392c (puVar17)
extern char **const gI_g383a __attribute__((visibility("hidden")));      // DAT_000f3930 (puVar16)
extern void ***const gI_layout __attribute__((visibility("hidden")));    // DAT_000f3934

typedef void (*VolFn)(void *snd, int channel, int value);

// Globals::init(ApplicationManager* app, Engine* engine)
// self in r0, app in r1.
int Globals::init(void *app) {
    Globals *self = this;
    int *missionSlot = *gI_mission;
    if (*missionSlot == 0) {
        void *m = ::operator new(0x78);
        Mission_ctor(m);
        *missionSlot = (int)(long)m;
    }

    int *settings = (int *)*gI_settings;
    int *flagFFFF = (int *)*gI_flagFFFF;
    int *uVar7slot = (int *)*gI_uVar7slot;
    char *langFlag = *gI_langFlag;
    char *zeroByte = *gI_zeroByte;

    char *s = (char *)settings;
    *(unsigned char *)(s + 0x11) = 1;
    *(unsigned char *)(s + 0x30) = 1;
    *(float *)(s + 0x00) = 0.5f;
    *(float *)(s + 0x04) = 0.5f;
    *(float *)(s + 0x08) = 0.5f;
    *(float *)(s + 0x24) = 0.5f;
    *(float *)(s + 0x28) = 1.0f;
    *zeroByte = 0;
    char lang = *langFlag;
    *(short *)(s + 0x0f) = 0x101;
    *flagFFFF = -1;
    *uVar7slot = (lang == 0) ? 6 : 0xc;

    void *galaxy = ::operator new(8);
    Galaxy_ctor(galaxy);
    **gI_galaxy = galaxy;
    void *ach = ::operator new(0x28);
    Achievements_ctor(ach);
    **gI_achieve = ach;
    void *status = ::operator new(0x1f0);
    Status_ctor(status);
    **gI_status = status;
    void *imgFac = ::operator new(0xc);
    ((ImageFactory *)(imgFac))->ctor();
    **gI_imgFac = imgFac;

    void *fr = ::operator new(1);
    FileRead_ctor(fr);
    **gI_items = (int *)(long)((FileRead *)fr)->loadItemsBinary();
    **gI_ships = (int *)(long)((FileRead *)fr)->loadShipsBinary();
    ::operator delete(FileRead_dtor(fr));

    int *engineSlot = *gI_engineSlot;
    if (*engineSlot == 0) {
        *engineSlot = *(int *)app;
    }
    **gI_appMgr = app;
    ((ApplicationManager *)(app))->VibrateEnable(0);

    void *rng = ::operator new(8);
    AERandom_ctor(rng);
    **gI_ctxSlot = self;
    **gI_random = (void *)rng;

    void *gen = ::operator new(1);
    Generator_ctor(gen);
    **gI_generator = gen;

    void *rh = ::operator new(0x2c);
    ((RecordHandler *)(rh))->ctor();
    void **rhSlotP = *gI_recHandler;
    *rhSlotP = rh;
    Status_resetGame();
    ((RecordHandler *)(*rhSlotP))->loadOptions();

    void *fmod = ::operator new(0x243c);
    FModSound_ctor(fmod);
    void **fmodSlotP = *gI_fmod;
    *fmodSlotP = fmod;
    ((FModSound *)(fmod))->init();

    VolFn setMus = (VolFn)*gI_setMusVol;
    setMus(*fmodSlotP, 1, *(unsigned char *)(s + 0xd));
    setMus(*fmodSlotP, 2, *(unsigned char *)(s + 0xc));
    setMus(*fmodSlotP, 3, *(unsigned char *)(s + 0xe));
    VolFn setSfx = (VolFn)*gI_setSfxVol;
    setSfx(*fmodSlotP, 1, settings[0]);
    setSfx(*fmodSlotP, 2, settings[1]);
    setSfx(*fmodSlotP, 3, settings[2]);

    if (FModSound_tryToStopMusicForBGMusic() != 0) {
        *(unsigned char *)(s + 0xd) = 0;
    }

    // Camera/transform-style zeroing of a vector-triple struct (puVar6 second object).
    **gI_g381c = 0;
    **gI_g381a = 1;
    int *obj = (int *)*gI_g381e;
    obj[0] = 0; obj[1] = 0; obj[2] = 0; obj[3] = 0;
    obj[4] = 0; obj[5] = 0; obj[6] = 0; obj[7] = 0;
    obj[8] = 0; obj[9] = 0; obj[10] = 0; obj[11] = 0;
    *(int *)((char *)obj + 0x2b) = 0;
    *(int *)((char *)obj + 0x2f) = 0;
    *(int *)((char *)obj + 0x33) = 0;
    *(int *)((char *)obj + 0x37) = 0;
    *(char *)*(int **)gI_g3822 = 0;
    **gI_g3824 = 0;
    **gI_g383a = 0;

    void *layout = ::operator new(0x414);
    ((Layout *)(layout))->ctor();
    **gI_layout = layout;
    ((Layout *)(layout))->reload();
    ParticleSettingsRef_initialize();

    Array<int> *arr = new Array<int>();
    self->field_0x4 = arr;
    return (int)(long)arr;
}

// ---- playMusicAndFadeOutCurrent_e55cc.cpp ----
extern void *const gPM_snd0 __attribute__((visibility("hidden")));   // DAT_000f57fc
extern void *const gPM_snd1 __attribute__((visibility("hidden")));   // DAT_000f5808 / 0x580c
extern void *const gPM_snd2 __attribute__((visibility("hidden")));   // DAT_000f5814
extern void *const gPM_sndStatus __attribute__((visibility("hidden"))); // DAT_000f5804
extern const int gPM_table0 __attribute__((visibility("hidden")));   // DAT_000f5800
extern const int gPM_table1 __attribute__((visibility("hidden")));   // DAT_000f5810

// Globals::playMusicAndFadeOutCurrent(int mode) — mode in r1.
int Globals_playMusicAndFadeOutCurrent(int prev, int mode)
{
    int snd;
    int track;
    int vol = 0;

    if (mode == 2) {
        int *sndP = *(int **)gPM_snd2;
        ((FModSound *)(*sndP))->stop(0);
        snd = *sndP;
        track = 0x91;
        ((FModSound *)(snd))->play(track, 0, 0, (float)vol);
        return 0;
    }
    if (mode == 1) {
        int *statSnd = *(int **)gPM_sndStatus;
        if ((*g_status)->inAlienOrbit() != 0) {
            int *sndP = *(int **)gPM_snd1;
            ((FModSound *)(*sndP))->stop(0);
            snd = *sndP;
            track = 0x88;
            int m = (*g_status)->getCurrentCampaignMission();
            if (m > 0x92 && (*g_status)->getCurrentCampaignMission() < 0x9a) {
                track = 0x91;
            }
            ((FModSound *)(snd))->play(track, 0, 0, (float)vol);
            return 0;
        }
        ((SolarSystem *)(long)(*g_status)->getSystem())->getRace();
        int *sndP = *(int **)gPM_snd1;
        ((FModSound *)(*sndP))->stop(0);
        if (Station_getIndex((int)(long)(*g_status)->getStation()) == 0x6c) {
            ((FModSound *)(*sndP))->play(0x92, 0, 0, (float)vol);
            return 0;
        }
        if (Station_getIndex((int)(long)(*g_status)->getStation()) == 0x65) {
            ((FModSound *)(*sndP))->play(0x93, 0, 0, (float)vol);
            return 0;
        }
        if ((*g_status)->inSupernovaSystem() != 0) {
            if ((*g_status)->getCurrentCampaignMission() == 0x59) {
                ((FModSound *)(*sndP))->play(0x8be, 0, 0, (float)vol);
                return 0;
            }
            if ((*g_status)->getMission() != 0 && ((Mission *)(long)(*g_status)->getMission())->isEmpty() == 0) {
                int tgt = ((Mission *)(long)(*g_status)->getMission())->getTargetStation();
                if (tgt == Station_getIndex((int)(long)(*g_status)->getStation())) {
                    int cm = (*g_status)->getCurrentCampaignMission();
                    track = cm < 0x6a ? 0x8c1 : 0x8c2;
                    ((FModSound *)(*sndP))->play(track, 0, 0, (float)vol);
                    return 0;
                }
            }
            ((FModSound *)(*sndP))->play(0x94, 0, 0, (float)vol);
            return 0;
        }
        if ((*g_status)->inDeepScienceOrbit() != 0) {
            ((FModSound *)(*sndP))->play(0x98, 0, 0, (float)vol);
            return 0;
        }
        if (Station_getIndex((int)(long)(*g_status)->getStation()) == 0x78 &&
            ((*g_status)->getCurrentCampaignMission() == 0x7e ||
             (*g_status)->getCurrentCampaignMission() == 0x85)) {
            ((FModSound *)(*sndP))->play(0x8bf, 0, 0, (float)vol);
            return 0;
        }
        const int *table = &gPM_table1;
        track = table[((SolarSystem *)(long)(*g_status)->getSystem())->getRace()];
        ((FModSound *)(*sndP))->play(track, 0, 0, (float)vol);
        return 0;
    }
    if (mode != 0) {
        return prev;
    }

    int race = ((SolarSystem *)(long)(*g_status)->getSystem())->getRace();
    int *sndP = *(int **)gPM_snd0;
    ((FModSound *)(*sndP))->stop(0);
    if (Station_getIndex((int)(long)(*g_status)->getStation()) == 0x6c) {
        ((FModSound *)(*sndP))->play(0x84, 0, 0, (float)vol);
        return 0;
    }
    if (Station_getIndex((int)(long)(*g_status)->getStation()) == 0x65) {
        ((FModSound *)(*sndP))->play(0x83, 0, 0, (float)vol);
        return 0;
    }
    int idx = Station_getIndex((int)(long)(*g_status)->getStation());
    if (idx == 10 || Station_getIndex((int)(long)(*g_status)->getStation()) == 100) {
        if (Station_getIndex((int)(long)(*g_status)->getStation()) == 10 &&
            (*g_status)->getCurrentCampaignMission() == 0x9f) {
            ((FModSound *)(*sndP))->play(0x90, 0, 0, (float)vol);
            return 0;
        }
        ((FModSound *)(*sndP))->play(0x85, 0, 0, (float)vol);
        return 0;
    }
    const int *table = &gPM_table0;
    track = table[race];
    ((FModSound *)(*sndP))->play(track, 0, 0, (float)vol);
    return 0;
}

// ---- getDialogueSoundId_e5818.cpp ----
// Static (dialogueCode, soundId) pair table, 47 entries; linear-searched first.
extern const int gGDS_pairTable[] __attribute__((visibility("hidden")));  // DAT_000f5a6c base
// Per-(category) dialogue-code -> sound-id dispatch. category encodes race/gender bucket
// (the disasm's iVar2 switch value 0..5). Returns -1 when unmapped.

// Globals::getDialogueSoundId(int code, Agent* agent)
int Globals_getDialogueSoundId(void *self, int code, void *agent)
{
    (void)self;
    // Phase 1: linear search of the static pair table (47 pairs).
    const int *t = gGDS_pairTable;
    for (unsigned i = 0; (i >> 6) < 0x2f; i += 2) {
        if (t[i * 2] == code) {
            return t[i * 2 + 1];
        }
    }

    if (agent == 0) {
        return -1;
    }

    int race = ((Agent *)(agent))->getRace();
    int male = ((Agent *)(agent))->isMale();

    int category;
    if (race == 3) {
        int *parts = ((Agent *)(agent))->getImageParts();
        if (parts != 0) {
            int *p = ((Agent *)(agent))->getImageParts();
            category = (*p == 2) ? 3 : 0;
            // category 0 dispatch differs by gender; fold gender into the bucket id.
            return Globals_dialogueDispatch(male != 0 ? (category + 10) : category, code);
        }
        // No image parts: race-3 fallback uses the "case 2/3" generic dialogue table.
        return Globals_dialogueDispatch(2, code);
    }

    // Non-Klingon races: bucket by race index, gendered.
    category = race;
    return Globals_dialogueDispatch(male != 0 ? (category + 10) : category, code);
}

// ---- getRandomPlanetName_e4a8c.cpp ----
struct FileRead;
struct Station;
extern void *const gPlanetRng __attribute__((visibility("hidden")));

// r0 = sret buffer for the returned name String; it is returned (r0 preserved -> the final
// delete is a regular call, not a tail call).
String *Globals_getRandomPlanetName(String *ret)
{
    FileRead *f = (FileRead *)::operator new(1);
    FileRead_ctor(f);
    int which = nextInt_71ad0((AbyssEngine::AERandom *)*(int *)gPlanetRng, 0x64);
    Station *st = (Station *)(long)f->loadStation(which);
    ((Station *)(ret))->getName();
    if (st != 0) {
        ((Station *)(st))->dtor();   // dtor() returns void
        ::operator delete(st);
    }
    ::operator delete(FileRead_dtor(f));
    return ret;
}

// ---- getRandomName_e40b4.cpp ----
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int c);  // String(char const*, bool)
extern "C" void AEString_copy_ctor(void *dst, void *src, int c);     // String(String const&, bool)
extern "C" void AEString_concat(void *dst, void *a);                 // operator+

extern void *const gGRN_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4218
extern const char gGRN_noFirst[] __attribute__((visibility("hidden")));     // DAT_000f421c
extern void *const gGRN_rng1 __attribute__((visibility("hidden")));         // DAT_000f4220
extern const char gGRN_noLast[] __attribute__((visibility("hidden")));      // DAT_000f4224
extern void *const gGRN_rng2 __attribute__((visibility("hidden")));         // DAT_000f4228
extern const char gGRN_space[] __attribute__((visibility("hidden")));       // DAT_000f422c (" ")

// Globals::getRandomName(int kind, bool both) -> String via sret (r0).
// kind in r2, both in r3.
void Globals_getRandomName(void *retSlot, void *unused, int kind, int both)
{
    (void)unused;
    int *guardP = *(int **)gGRN_guardHolder;
    volatile int saved = *guardP;

    void *fr = ::operator new(1);
    FileRead_ctor(fr);
    Array<void *> *first = (Array<void *> *)((FileRead *)fr)->loadNamesBinary(kind, both, 1);
    Array<void *> *last = (Array<void *> *)((FileRead *)fr)->loadNamesBinary(kind, both, 0);

    char firstStr[12], lastStr[12];
    if (first == 0) {
        AEString_cstr_ctor(firstStr, gGRN_noFirst, 0);
    } else {
        int idx = nextInt_71aa4((AbyssEngine::AERandom *)**(int **)gGRN_rng1);
        AEString_copy_ctor(firstStr, (*first)[idx], 0);
    }
    if (last == 0) {
        AEString_cstr_ctor(lastStr, gGRN_noLast, 0);
    } else {
        int idx = nextInt_71aa4((AbyssEngine::AERandom *)**(int **)gGRN_rng2);
        AEString_copy_ctor(lastStr, (*last)[idx], 0);
    }

    if (first != 0) {
        ArrayReleaseClasses_Str(first);
        ::operator delete(ArrayStr_dtor(first));
    }
    if (last != 0) {
        ArrayReleaseClasses_Str(last);
        ::operator delete(ArrayStr_dtor(last));
    }
    ::operator delete(FileRead_dtor(fr));

    // local_34 holds the high field of firstStr (size); when 0 (empty), just copy firstStr.
    if (*((int *)firstStr + 2) == 0) {
        AEString_copy_ctor(retSlot, firstStr, 0);
    } else {
        char space[12], mid[12];
        AEString_cstr_ctor(space, gGRN_space, 0);
        AEString_concat(mid, firstStr);
        AEString_concat(retSlot, mid);
    }

    return;
}

// ---- getLine_e4230.cpp ----
extern void *const gGL_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4378
extern void *const gGL_canvas __attribute__((visibility("hidden")));       // DAT_000f437c
extern const char gGL_empty[] __attribute__((visibility("hidden")));       // DAT_000f4380

// Globals::getLine(uint font, AbyssEngine::String text, int maxWidth, AbyssEngine::String* out)
// font in r1, text(String*) in r2, maxWidth in r3, out at [r7+0x8].
void Globals_getLine(void *retSlot, unsigned font, void *text, int maxWidth,
                                void *out)
{
    (void)retSlot;
    int *guardP = *(int **)gGL_guardHolder;
    volatile int saved = *guardP;

    int lang = GameText_getLanguage();
    int width = 5;
    if (((unsigned)(lang | 1)) == 0xb) width = 0xf;
    if ((unsigned)lang == 0xf) width = 0xf;

    int *canvas = *(int **)gGL_canvas;
    unsigned lastSpace = 0;
    unsigned i = 0;
    char tmp[12];
    unsigned len = *(unsigned *)((char *)text + 8);

    while (i < len) {
        short ch = *AEString_index(text, i);
        width += ((PaintCanvas *)(long)*canvas)->GetTextWidth(font, text, i, i + 1);
        if (ch == 0x20) {
            lastSpace = i;
        }
        if (maxWidth <= width) {
            short c0 = *AEString_index(text, i);
            short c1 = *AEString_index(text, i);
            unsigned end = i + 1;
            if (c0 == 0x0a || c1 == 0x0d) {
                AEString_substr(tmp, text, 0, end);
            } else if ((int)lastSpace < 1) {
                AEString_substr(tmp, text, 0, end);
            } else {
                AEString_substr(tmp, text, 0, lastSpace + 1);
            }
            goto done;
        }
        i++;
        len = *(unsigned *)((char *)text + 8);
    }

    if ((int)len < 2) {
        AEString_cstr_ctor(tmp, gGL_empty, 0);
    } else {
        AEString_substr(tmp, text, 0, len);
    }

done:
    AEString_dtor(tmp);
    return;
}
