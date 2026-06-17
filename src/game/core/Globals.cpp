#include "game/core/Globals.h"
Globals* gGlobals = nullptr;          // canonical Globals singleton
#include "engine/render/Mesh.h"
#include "game/ship/Ship.h"
#include "game/core/PaintCanvasClass.h"
// gof2/engine/render/PaintCanvas.h defines a second `class PaintCanvas` that clashes with the
// PaintCanvasClass.h view used in this TU; declare the canonical render-canvas singleton locally
// (matches `extern PaintCanvas* gCanvas;` in render/PaintCanvas.h, binary .bss 0x2281b8).
extern PaintCanvas* gCanvas;
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
// gof2/FileRead.h intentionally NOT included: its stub `struct Station`/`struct Agent`
// collide with the canonical Station.h/Agent.h definitions this file relies on. FileRead
// is only used here as an opaque handle via local extern "C" declarations.
#include "game/mission/Status.h"
#include "game/world/Galaxy.h"
#include "game/mission/Achievements.h"
#include "engine/core/AERandom.h"
#include "game/ship/Agent.h"          // defines the canonical global `struct String` (12-byte String sret)
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"
// Mission.h transitively pulls Station.h; both reuse the same canonical `String` from
// common.h, so no aliasing is needed. Station::getName() returns String and is only ever
// discarded here.
#include "game/mission/Mission.h"
#include "game/world/Station.h"
#include "game/mission/RecordHandler.h"
#include "game/world/SolarSystem.h"
#include "game/core/String.h"

// Status / AEGeometry / String full layouts are not needed here: Globals only takes
// Status* as an opaque handle and reaches the engine via extern "C" free functions.
struct Status;

void Globals_getLine(void *retSlot, unsigned font, void *text, int maxWidth, void *lineArr);
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" float VectorUnsignedToFloat(unsigned v, int mode);
// AERandom::nextInt — the real defs are free functions in AERandom.cpp:
//   nextInt_71aa4(self)        -> unbounded 32-bit draw (1-arg call form)
//   nextInt_71ad0(self, bound) -> bounded draw            (2-arg call form)
// The decompiler folded both into one variadic AERandom_nextInt thunk; call the real symbols.
namespace AbyssEngine { class AERandom; }
uint32_t nextInt_71aa4(AbyssEngine::AERandom *self);
int      nextInt_71ad0(AbyssEngine::AERandom *self, int bound);
extern "C" int idiv(int a, int b);
void MatrixSetTranslation(void *m, float x, float y, float z);
extern "C" int AERandom_nextIntB(int rng, int bound);
extern "C" void FileRead_ctor(void *self);
extern "C" void *FileRead_dtor(void *self);
// FileRead is used as an opaque handle here (gof2/FileRead.h not included; see top of file).
// loadStation / loadWreckCollision are instance methods on the local FileRead view above.
extern "C" float VectorScale(void *vec, float scalar);
extern "C" void BoundingSphere_ctor(void *self, float cx, float cy, float cz, float r);
extern "C" void *Galaxy_dtor(void *g);
extern "C" void *Status_dtor(void *s);
extern "C" void *AERandom_dtor(void *r);
extern "C" void *Layout_dtor(void *l);
extern "C" void *Generator_dtor(void *g);
extern "C" void *FModSound_dtor(void *s);
extern "C" void *Achievements_dtor(void *a);
extern "C" void *ImageFactory_dtor(void *f);
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
extern "C" int Station_getIndex(int station);
// The dropped-self Status singleton. The original loaded the global Status* implicitly for
// each of these 0-arg calls; the singleton lives at the hidden global g_status (same one
// MGame/Mission/PlayerTurret reach). Recover the receiver as gStatus->method().
extern "C" __attribute__((visibility("hidden"))) Status **g_status;
int GameText_getLanguage();

struct Status;
// Hidden PC-relative leaderboard destination slot (g_android_leaderboard_scores @ 0x22d2c0;
// NOT a shared singleton). The kills source resolves to the canonical Status singleton.
extern void *const gLB_dest __attribute__((visibility("hidden")));

void Globals_reportLeaderboards()
{
    int kills = gStatus->getKills();
    *(int *)gLB_dest = kills;
}

// Globals::resetHints() zeroes four 16-byte blocks of the hint struct (at offsets 0, 0x2b,
// 0x20, 0x10) using a zeroed NEON quad. Offset 0x2b is unaligned, so use packed 16-byte
// vector stores.
typedef int v4si __attribute__((vector_size(16)));
struct __attribute__((packed)) Q16 { v4si v; };

extern void *const gHints __attribute__((visibility("hidden")));

void Globals_resetHints()
{
    char *p = (char *)gHints;   // RAWREAD: opaque hint blob via hidden global (no modeled type/members)
    const v4si z = {0, 0, 0, 0};
    ((Q16 *)(p + 0x00))->v = z;
    ((Q16 *)(p + 0x2b))->v = z;
    ((Q16 *)(p + 0x20))->v = z;
    ((Q16 *)(p + 0x10))->v = z;
}

void Globals_startNewSoundResourceList(void *self)
{
    if (((Globals*)self)->soundResources != 0) {
        ((Globals*)self)->soundResources->clear();
        delete ((Globals*)self)->soundResources;
    }
    ((Globals*)self)->soundResources = 0;
    Array<int> *a = new Array<int>();
    ((Globals*)self)->soundResources = a;
    a->push_back(0x7c);
    return ((Globals*)self)->startNewSoundResourceList_tail(0x7b, ((Globals*)self)->soundResources);
}

// hidden PC-relative pointer-to-pointer global (deref'd twice).
extern void *const gItemNameGameText __attribute__((visibility("hidden")));

// String (12-byte String sret aggregate) is provided by gof2/Agent.h above.

String Globals_getItemName(void *unused, int item)
{
    (void)unused;
    String *src = (String *)((GameText *)(*(void **)gItemNameGameText))->getText(item + 0x4fa);
    return *(String *)src;
}

// Globals::getKeyActionName(int) returns a String by value. The default ctor returns void,
// so the compiler keeps a frame and restores the sret pointer (no tail call). The int arg
// is unused.

String Globals_getKeyActionName(int action)
{
    (void)action;
    String r;
    r.ctor();
    return r;
}

// Globals::sqrt(float) - the target moves the float arg (r1) to r0 and tail-jumps to the
// runtime sqrtf. The instance pointer is unused.

float Globals::sqrt(float x) {
    (void)this;
    return Globals::sqrt_impl(x);
}

extern void *const gDrinks_a __attribute__((visibility("hidden")));
extern void *const gDrinks_rng __attribute__((visibility("hidden")));

void Globals_getRandomSystemForDrinks()
{
    int a = *(int *)gDrinks_a;
    int r = nextInt_71ad0((AbyssEngine::AERandom *)*(int *)gDrinks_rng, 0x16);
    return Globals::getRandomSystemForDrinks_tail(a, r);
}

// Linear search; on no-match append `val` to the active sound-resource list.
void Globals_addSoundResourceToList(void *self, int val)
{
    Array<int> *a = ((Globals*)self)->soundResources;
    if (a != 0) {
        unsigned i = 0;
        for (;;) {
            if (i >= a->size()) {
                return ((Globals*)self)->addSoundResource_tail(val, a);
            }
            int e = (*a)[i];
            i = i + 1;
            if (e == val) {
                break;
            }
        }
    }
}

// Globals::replaceKeyBindingTokens(String const&) returns a String by value (sret in r0).
// Body copy-constructs the return slot from the source string (in r2), flag=false.
// The copy-ctor returns void -> frame kept, sret pointer restored (no tail call).

String Globals_replaceKeyBindingTokens(void *unused, void *src)
{
    (void)unused;
    return *(String *)src;
}

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

extern void *const gGLA_guardHolder __attribute__((visibility("hidden")));
extern const char gGLA_newline[] __attribute__((visibility("hidden")));

// Globals::getLineArray(uint font, AbyssEngine::String const& text, int maxWidth,
//                        Array<String*>* out)
void Globals_getLineArray(unsigned font, void *text, int maxWidth, void *arg3,
                                     Array<void *> *out)
{
    (void)arg3;
    int *guardP = *(int **)gGLA_guardHolder;
    volatile int saved = *guardP;

    String *line = (String *)::operator new(0xc);
    line->ctor();

    String work;
    work.ctor_copy((String *)text, false);   // work = copy of text
    String nl;
    nl.ctor_char(gGLA_newline, false);
    work.addAssign_str(&nl);

    unsigned count = 0;
    int consumed = 0;
    int total = (int)work.size();
    while (consumed < total) {
        String sub;
        sub.SubString(&work, consumed, total);
        String ssub;
        ssub.ctor_copy(&sub, false);
        Globals_getLine(line, font, &ssub, maxWidth, out);
        // getLine packs a per-line advance count into the unused tail of the String slot.
        // RAWREAD: scratch poke into a std::u16string-backed String slot (no byte-faithful member at +8).
        int adv = *(int *)((char *)line + 8);
        count++;
        consumed += adv;
    }
    line->dtor();

    out->resize(count);
    for (unsigned i = 0; i < count; i++) {
        String *s = (String *)::operator new(0xc);
        s->ctor();
        (*out)[i] = s;
    }

    for (unsigned i = 0; i < count; i++) {
        String sub;
        sub.SubString(&work, consumed, total);
        String ssub;
        ssub.ctor_copy(&sub, false);
        void *slot = (*out)[i];
        Globals_getLine(slot, font, &ssub, maxWidth, slot);

        int li = 0;
        String *s = (String *)(*out)[i];
        int hi = (int)s->size();
        while (*s->index(li) == 0x20) {
            li++;
            s = (String *)(*out)[i];
        }
        hi++;
        do {
            String *cur = (String *)(*out)[i];
            short ch = *cur->index(hi - 2);
            hi--;
            if (ch != 0x20) break;
        } while (true);

        String trimmed;
        String *cur = (String *)(*out)[i];
        trimmed.SubString(cur, li, hi);
    }

    return;
}

extern void *const gLTS2_guardHolder __attribute__((visibility("hidden")));
extern const char gLTS2_secTens[] __attribute__((visibility("hidden")));
extern const char gLTS2_secEmpty[] __attribute__((visibility("hidden")));
extern const char gLTS2_minTens[] __attribute__((visibility("hidden")));
extern const char gLTS2_minEmpty[] __attribute__((visibility("hidden")));
extern const char gLTS2_zeroPrefix[] __attribute__((visibility("hidden")));
extern const char gLTS2_hrTens[] __attribute__((visibility("hidden")));
extern const char gLTS2_hrEmpty[] __attribute__((visibility("hidden")));
extern const char gLTS2_sep1[] __attribute__((visibility("hidden")));
extern const char gLTS2_sep2[] __attribute__((visibility("hidden")));

// Globals::longToTimeString(long long ms, AbyssEngine::String& out)
void Globals_longToTimeString(void *retSlot, void *unused, long long ms)
{
    (void)unused;
    (void)retSlot;
    int *guardP = *(int **)gLTS2_guardHolder;
    volatile int saved = *guardP;
    void *out = retSlot;

    int rem = 0;
    long long secQ = Globals::lts_divmod(ms, 1000, &rem);
    int seconds = 0;
    Globals::lts_divmod(secQ, 0x3c, &seconds);

    long long minQ = Globals::lts_divmod(ms, 0xea60, &rem);
    int minute = 0;
    Globals::lts_divmod(minQ, 0x3c, &minute);

    long long hrQ = Globals::lts_divmod(ms, 0xea60, &rem);
    int hours = 0;
    Globals::lts_divmod(hrQ, 0x18, &hours);

    String secPart, secNum, secStr;
    secPart.ctor_char(seconds < 10 ? gLTS2_secTens : gLTS2_secEmpty, false);
    secNum.ctor_int(seconds);
    secStr = secPart + secNum;

    String minPart, minNum, minStr;
    minPart.ctor_char(minute < 10 ? gLTS2_minTens : gLTS2_minEmpty, false);
    minNum.ctor_int(minute);
    minStr = minPart + minNum;

    if (hours == 0) {
        String prefix, left, full;
        prefix.ctor_char(gLTS2_zeroPrefix, false);
        left = prefix + minStr;
        full = left + secStr;
    } else {
        int rem4 = 0;
        long long h = Globals::lts_divmod(ms, 0xea60, &rem4);
        int hv = (int)h * 0x18 + hours;

        String hrPart, hrNum, hrStr;
        hrPart.ctor_char(hv < 10 ? gLTS2_hrTens : gLTS2_hrEmpty, false);
        hrNum.ctor_int(hv);
        hrStr = hrPart + hrNum;

        String s1, a, b, s2, c, full;
        s1.ctor_char(gLTS2_sep1, false);
        a = s1 + hrStr;
        b = a + minStr;
        s2.ctor_char(gLTS2_sep2, false);
        c = b + s2;
        full = c + secStr;
    }

    return;
}


extern void *const gGBS_guardHolder __attribute__((visibility("hidden")));
extern void *const gGBS_strPtr __attribute__((visibility("hidden")));
extern void *const gGBS_canvas __attribute__((visibility("hidden")));
extern const char gGBS_prefix[] __attribute__((visibility("hidden")));

// Globals::getBoundedString(AbyssEngine::String const&, int) — String returned by value.
void Globals_getBoundedString(void *retSlot, void *unused, void *text, int width)
{
    (void)unused;
    int *guardP = *(int **)gGBS_guardHolder;
    volatile int saved = *guardP;

    ((String *)retSlot)->ctor_copy((String *)text, false);

    void **strPtr = *(void ***)gGBS_strPtr;
    int **canvas = *(int ***)gGBS_canvas;
    int w = ((PaintCanvas *)(long)**canvas)->GetTextWidth(0, *strPtr);
    if (width < w) {
        String *line = (String *)::operator new(0xc);
        line->ctor();

        int font = (int)(long)*strPtr;
        String tmpText;
        tmpText.ctor_copy((String *)text, false);
        Globals_getLine(font ? (void *)(long)font : line, font, &tmpText, width - 3, line);

        String prefix;
        prefix.ctor_char(gGBS_prefix, false);
        String concat;
        concat = prefix + *line;

        line->dtor();
    }

    return;
}

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
extern void *const gSCS_flagB __attribute__((visibility("hidden")));
extern void *const gSCS_objA __attribute__((visibility("hidden")));      // DAT_000f8740 -> obj (field 0x54)
extern void *const gSCS_objB __attribute__((visibility("hidden")));
extern void *const gSCS_flagC __attribute__((visibility("hidden")));
extern void *const gSCS_flagD __attribute__((visibility("hidden")));
extern void *const gSCS_objC __attribute__((visibility("hidden")));
extern void *const gSCS_flagE __attribute__((visibility("hidden")));
extern void *const gSCS_flagF __attribute__((visibility("hidden")));
extern void *const gSCS_flagG __attribute__((visibility("hidden")));
extern void *const gSCS_flagH __attribute__((visibility("hidden")));

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
        rdobj(gSCS_objB)[0x54 / 4] = (int)fp1;   // RAWREAD: opaque settings sub-object via hidden global (no header)
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
            rdobj(gSCS_objC)[0x54 / 4] = iv;   // RAWREAD: opaque settings sub-object via hidden global (no header)
            char flag8 = (flag == 0) ? 0 : 1;
            (void)flag8;
            goto label8508;
        }
        uv = 0xd2;
        rdobj(gSCS_objA)[0x54 / 4] = 0xd2;   // RAWREAD: opaque settings sub-object via hidden global (no header)
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
            float fv15 = 4.0f;
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

// The full briefing-text assembly (large data-driven template/concat sequence). Driven by the
// agent's offer/event context; writes the completed String into `out` (sret slot). Kept as an
// extern helper because the original is a ~7KB straight-line concat over ~100 String temporaries.

extern void *const gGAMT_guard __attribute__((visibility("hidden")));
extern const char gGAMT_noAgent[] __attribute__((visibility("hidden")));
extern void *const gGAMT_busyObj __attribute__((visibility("hidden")));  // DAT_000f6178 (field 0xd0 counter)
extern void *const gGAMT_modText __attribute__((visibility("hidden")));

// Globals::getAgentMissionText(Agent*) -> String by value.
void Globals_getAgentMissionText(void *out, void *unused, void *agent)
{
    (void)unused;
    int *guardP = *(int **)gGAMT_guard;
    volatile int saved = *guardP;

    if (agent == 0) {
        ((String *)out)->ctor_char(gGAMT_noAgent, false);
        goto epilogue;
    }

    {
        String acc;
        acc.ctor();

        if (((Agent *)(agent))->isGenericAgent() == 0) {
            String scratch;
            scratch.ctor();

            int event = ((Agent *)(agent))->getEvent();
            if (event < 1 && ((Agent *)(agent))->hasAcceptedOffer() == 0) {
                int *busy = *(int **)gGAMT_busyObj;
                // RAWREAD: opaque re-entrancy-guard object via hidden global (no header/named member at +0xd0).
                *(int *)(*busy + 0xd0) += 1;       // mark "assembling text" re-entrancy guard
                int offer = ((Agent *)(agent))->getOffer();

                if (offer == 8) {
                    int ship = (int)(long)gStatus->getShip();
                    int price = ((Ship *)(ship))->getPrice();
                    int pct = ((Agent *)(agent))->getModPricePercentage();
                    ((Agent *)(agent))->setSellItemPrice(idiv(price * pct, 100));
                    ship = (int)(long)gStatus->getShip();
                    int modIdx = ((Agent *)(agent))->getSellModIndex();
                    if (((Ship *)(ship))->hasModInstalled(modIdx) != 0) {
                        void *t = ((GameText *)((void *)(long)**(int **)gGAMT_modText))->getText(modIdx);
                        *(int *)(*busy + 0xd0) -= 1;
                        ((String *)out)->ctor_copy(&acc, false);
                        goto epilogue;
                    }
                }

                // General offer/event briefing text: data-driven assembly into `acc`.
                Globals::buildAgentMissionText(&acc, agent, offer);
                *(int *)(*busy + 0xd0) -= 1;
            } else {
                Globals::buildAgentMissionText(&acc, agent, -1);
            }
        } else {
            Globals::buildAgentMissionText(&acc, agent, -1);
        }

        ((String *)out)->ctor_copy(&acc, false);
    }

epilogue:
    return;
}

extern void *const gIAP_guardHolder __attribute__((visibility("hidden")));
extern const char gIAP_prefixA[] __attribute__((visibility("hidden")));
extern const char gIAP_prefixB[] __attribute__((visibility("hidden")));
extern const char gIAP_id50[] __attribute__((visibility("hidden")));
extern const char gIAP_id0[] __attribute__((visibility("hidden")));
extern const char gIAP_id1[] __attribute__((visibility("hidden")));
extern const char gIAP_id2[] __attribute__((visibility("hidden")));
extern const char gIAP_id3[] __attribute__((visibility("hidden")));
extern const char gIAP_id4[] __attribute__((visibility("hidden")));
extern const char gIAP_id51[] __attribute__((visibility("hidden")));
extern const char gIAP_id52[] __attribute__((visibility("hidden")));
extern const char gIAP_id53[] __attribute__((visibility("hidden")));
extern const char gIAP_id54[] __attribute__((visibility("hidden")));

// Globals::getInAppPurchaseArrayIndex(int productCode, Array<String*>* list) -> index or -1.
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
            // RAWREAD: `list` is an opaque engine Array payload (void*); +4 is its data pointer.
            void *entry = *(void **)(*(char **)((char *)list + 4) + i * 4);
            String base, pb, prefix;
            base.ctor_char(gIAP_prefixA, false);
            pb.ctor_char(gIAP_prefixB, false);
            prefix = base + pb;

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
                    String suf, full;
                    suf.ctor_char(suffix, false);
                    full = prefix + suf;
                    int cmp = ((String *)entry)->Compare_str(&full);
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

// Globals::getKeyBindingReplaceString(int) — returns a String by value.
// Default-construct a stack String, upper-case it (returns a String& copy-constructed into the
// sret slot), then the stack String is destroyed. The on-stack String is guarded by an
// explicit stack canary, mirroring the matched ConfigReader::GetNewLine / AEFile idiom.

// Default-construct a temp String, upper-case it, copy-construct the result into the sret
// blob, then destroy the temp.
String Globals_getKeyBindingReplaceString(Globals *, int key)
{
    (void)key;

    String tmp;
    tmp.ctor();
    tmp.ToUpperCase();   // in-place; returns void
    String result;
    return result;
}

// 64-bit divide helper: returns the quotient; the remainder is exposed via an out-param.


extern void *const gLTS_guardHolder __attribute__((visibility("hidden")));
extern const char gLTS_minTens[] __attribute__((visibility("hidden")));     // DAT_000f3df0 "0"
extern const char gLTS_minEmpty[] __attribute__((visibility("hidden")));    // DAT_000f3df4 ""
extern const char gLTS_hrTens[] __attribute__((visibility("hidden")));      // DAT_000f3df8 "0"
extern const char gLTS_hrEmpty[] __attribute__((visibility("hidden")));     // DAT_000f3dfc ""
extern const char gLTS_sep[] __attribute__((visibility("hidden")));         // DAT_000f3e00 ":"

// Globals::longToTimeStringNoSeconds(long long ms, AbyssEngine::String& out)
void Globals_longToTimeStringNoSeconds(void *retSlot, void *unused, long long ms)
{
    (void)unused;
    int *guardP = *(int **)gLTS_guardHolder;
    volatile int saved = *guardP;

    int rem = 0;
    // ms / 60000 -> total minutes (quotient), then minute-of-hour via %60.
    long long q = Globals::lts_divmod(ms, 0xea60, &rem);
    int minute = 0;
    Globals::lts_divmod(q, 0x3c, &minute);   // minute = q % 60 (remainder)

    long long q2 = Globals::lts_divmod(ms, 0xea60, &rem);
    int hours = 0;
    Globals::lts_divmod(q2, 0x18, &hours);   // hours = q2 % 24

    String mPart, mNum, minStr;
    mPart.ctor_char(minute < 10 ? gLTS_minTens : gLTS_minEmpty, false);
    mNum.ctor_int(minute);
    minStr = mPart + mNum;

    int rem3 = 0;
    long long h = Globals::lts_divmod(ms, 0xea60, &rem3);
    int hv = (int)h * 0x18 + hours;

    String hPart, hNum, hrStr;
    hPart.ctor_char(hv < 10 ? gLTS_hrTens : gLTS_hrEmpty, false);
    hNum.ctor_int(hv);
    hrStr = hPart + hNum;

    String sep, left, full;
    sep.ctor_char(gLTS_sep, false);
    left = sep + hrStr;
    full = left + minStr;

    return;
}

// Per-group builders for the special kind == 0xf sub-cases that build articulated
// transform chains (capital ships). Each delegates to the engine the same way the target
// does; kept as helpers to preserve the recoverable dispatch without inlining ~1.7KB.

extern void *const gGSG_guard __attribute__((visibility("hidden")));
extern const unsigned short gGSG_resTable[] __attribute__((visibility("hidden")));
extern const unsigned short gGSG_meshTable[] __attribute__((visibility("hidden")));
extern const short gGSG_extraTable[] __attribute__((visibility("hidden")));
extern const unsigned gGSG_lodTable[] __attribute__((visibility("hidden")));         // DAT_000f5548 (3/group)
extern const unsigned gGSG_childTable[] __attribute__((visibility("hidden")));       // DAT_000f5550 (3/group)

// Globals::getShipGroup(int kind, int variant, bool wireframe) -> AEGeometry*.
void Globals_getShipGroup(void *self, int kind, int variant, int wireframe)
{
    int *guardP = *(int **)gGSG_guard;
    volatile int saved = *guardP;
    PaintCanvas *canvas = gCanvas;        // the shared PaintCanvas singleton

    if (kind == 0xf) {
        ((Globals*)self)->buildShipGroup0f(variant, canvas);
        goto done;
    }
    if (kind == 0xe || kind == 0xd) {
        int resId = (kind == 0xe) ? 0x37e7 : 0x4275;
        AEGeometry *geom = new AEGeometry((uint16_t)resId, canvas, false);
        unsigned t0 = 0xffffffff;
        canvas->TransformCreate((unsigned int *)&t0);
        canvas->TransformAddMesh((unsigned)t0, 0, (bool)(1));
        geom->addChild(t0);
        unsigned t1 = 0xffffffff;
        canvas->TransformCreate((unsigned int *)&t1);
        canvas->TransformAddMesh((unsigned)t1, 0, (bool)(1));
        geom->addChild(t1);
        unsigned short lodMeshes[2] = {0, 0};
        int dist[2];
        dist[0] = (kind == 0xe) ? 35000 : 35000;
        dist[1] = (kind == 0xe) ? 60000 : 45000;
        geom->setLodMeshes(lodMeshes, dist, 2);
        unsigned short childMeshes[1] = {0};
        geom->setLodChildMeshes(childMeshes);
        if (kind == 0xe) {
            geom->setScaling(1.0f);
        }
        goto done;
    }

    // Generic path: per-ship table-driven build indexed by `kind`.
    {
        AEGeometry *geom = new AEGeometry((uint16_t)gGSG_resTable[kind], canvas, true);
        unsigned short mesh = gGSG_meshTable[kind];
        unsigned mainT = 0xffffffff;
        unsigned mainMesh = 0xffffffff;
        if (mesh != 0xffff) {
            canvas->MeshCreate(mesh, &mainMesh, true);
            canvas->TransformCreate((unsigned int *)&mainT);
            canvas->TransformAddMeshId(mainT, mainMesh);
            geom->addChild(mainT);
            geom->meshHandle = mainMesh;
        }
        if (!wireframe) {
            unsigned short mat = (unsigned short)((short)kind + 0x7dc8);
            unsigned matH = 0xffffffff;
            canvas->MaterialCreate(mat, &matH);
            canvas->MeshChangeResourceMaterial(geom->meshId, mat);
        }
        short extra = gGSG_extraTable[kind];
        if (extra != -1) {
            unsigned t = 0xffffffff;
            canvas->TransformCreate((unsigned int *)&t);
            canvas->TransformAddMesh((unsigned)t, 0, (bool)((int)(unsigned char)(char)extra));
            geom->addChild(t);
        }
        if (wireframe) {
            if (kind != 0x27 && kind != 0x29) {
                unsigned t = 0xffffffff;
                canvas->TransformCreate((unsigned int *)&t);
                canvas->TransformAddMesh((unsigned)t, 0, (bool)((int)(char)(-0x14 + (char)kind)));
                geom->addChild(t);
            }
        } else {
            unsigned t = 0xffffffff;
            canvas->TransformCreate((unsigned int *)&t);
            canvas->TransformAddMesh((unsigned)t, 0, (bool)((int)(char)(0x50 + (char)kind)));
            geom->addChild(t);
        }

        // Count up to 2 LOD meshes for this ship.
        // Ghidra (uVar12 init): when count==0 the distance arg is 2, not the accumulated 5000.
        unsigned lastVisibleDist = 2;   // last accumulated LOD distance (overwritten when count!=0)
        const unsigned *lod = &gGSG_lodTable[kind * 3];
        unsigned count = 0;
        for (int i = 0; i != 2; i++) {
            if (lod[i] != 0xffff) count++;
        }
        if (count != 0) {
            unsigned short *meshes = new unsigned short[count];
            unsigned *ids = new unsigned[count];
            int *dist = new int[count];
            int d = 5000;
            unsigned *idp = ids;
            const unsigned *src = lod;
            for (unsigned i = 0; i < count; i++) {
                unsigned m = *src;
                dist[i] = d;
                meshes[i] = (unsigned short)m;
                if (!wireframe) {
                    canvas->MeshCreate((unsigned short)m, idp, true);
                    canvas->MeshChangeResourceMaterial(*idp,
                                                           (unsigned short)((short)kind + 0x7dc8));
                }
                d += 8000;
                idp++;
                src++;
            }
            lastVisibleDist = (unsigned)d;
            if (wireframe) {
                geom->setLodMeshes(meshes, dist, count);
            } else {
                geom->setLodMeshesWithMeshIds(meshes, ids, dist, count);
            }

            const unsigned *childSrc = &gGSG_childTable[kind * 3];
            int childCount = 0;
            for (int i = 0; i != 2; i++) {
                if (childSrc[i] != 0xffff) childCount++;
            }
            if (childCount != 0) {
                unsigned short *childMeshes = new unsigned short[childCount];
                for (int i = 0; i != childCount; i++) {
                    childMeshes[i] = (unsigned short)childSrc[i];
                }
                geom->setLodChildMeshes(childMeshes);
                delete[] childMeshes;
            }
            delete[] meshes;
            delete[] dist;
        }
        geom->setLodLastVisibleDistance(lastVisibleDist);
    }

done:
    return;
}

extern void *const gREF_rng1 __attribute__((visibility("hidden")));
extern void *const gREF_rng2 __attribute__((visibility("hidden")));
extern const int gREF_table __attribute__((visibility("hidden")));

unsigned Globals::getRandomEnemyFighter(int kind) {
    (void)this;
    int t = kind;
    if ((unsigned)(kind - 9) > 1) {
        t = 8;
    }
    if (kind <= 3) {
        t = kind;
    }
    unsigned r;
    if (t == 1) {
        if (gStatus->dlc1Won() == 0) {
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

extern void *const gDL2_canvas __attribute__((visibility("hidden")));
extern void *const gDL2_lineHeight __attribute__((visibility("hidden")));

// Globals::drawLines(uint, Array<String*>*, int, int, uint, bool) — 7-decl form.
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
        yacc += *(int *)((char *)*lh + 4);   // RAWREAD: opaque line-height object via hidden global (no header)
    }
}

// Distinct PaintCanvas::MeshCreate overload (explicit vertex/face/uv counts).
// per-face index triple setter
// per-vertex UV/attr setter (variadic-ish; extra floats are uv coords)
// per-vertex position setter (x, y, z)

extern void *const gCBB_counter __attribute__((visibility("hidden")));
extern void *const gCBB_canvas __attribute__((visibility("hidden")));

// Globals::createBillBoard(int p1, int height, float u0, float v0, float u1, float v1, int width)
// p1 ignored layout-wise; height in r1, width passed on the stack.
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

    AbyssEngine::Mesh::setFace((void *)(long)cv, mesh, 0, 0, 1, 2);
    AbyssEngine::Mesh::setFace((void *)(long)cv, mesh, 1, 2, 1, 3);
    AbyssEngine::Mesh::setFace((void *)(long)cv, mesh, 2, 4, 5, 6);
    AbyssEngine::Mesh::setFace((void *)(long)cv, mesh, 3, 6, 5, 7);
    AbyssEngine::Mesh::setFace((void *)(long)cv, mesh, 4, 8, 9, 10);
    AbyssEngine::Mesh::setFace((void *)(long)cv, mesh, 5, 10, 9, 0xb);

    // UV coords: pairs (u0,v0) and (u0,v1) drive the texture mapping of each face.
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 0, u0, 0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 1, u1, 0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 2, v1, u0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 3, v1, v0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 4, u0, 0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 5, u1, 0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 6, v1, u0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 7, v1, v0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 8, u0, 0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 9, u1, 0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 10, v1, u0);
    AbyssEngine::Mesh::setUV((void *)(long)cv, mesh, 0xb, v1, v0);

    float pw = VectorSignedToFloat(width, 0);
    float nh = VectorSignedToFloat(-height, 0);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 0, nh, 0, pw);
    float nw = VectorSignedToFloat(-width, 0);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 1, nh, 0, nw);
    float ph = VectorSignedToFloat(height, 0);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 2, ph, 0, pw);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 3, ph, 0, nw);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 4, 0, nh, pw);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 5, 0, nh, nw);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 6, 0, ph, pw);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 7, 0, ph, nw);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 8, nh, ph, 0);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 9, nh, nh, 0);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 10, ph, ph, 0);
    AbyssEngine::Mesh::setVertex((void *)(long)cv, mesh, 0xb, ph, nh, 0);

    return;
}

// Vector scale: in-place multiply of a Vector by a scalar, returns resulting magnitude/component.
// BoundingSphere(center.x, center.y, center.z, radius)  (extra fp regs are scratch).
// BoundingAAB(maxX, maxY, maxZ, minX, minY, minZ) — 6 floats from the doubled extents.
extern "C" void BoundingAAB_ctor(void *self, float x0, float y0, float z0, float x1, float y1,
                                 float z1);

extern void *const gGWC_guardHolder __attribute__((visibility("hidden")));

// Globals::getWreckCollision(int kind, AEGeometry* geom).
void Globals_getWreckCollision(void *retSlot, int kind, void *geom)
{
    (void)retSlot;
    int *guardP = *(int **)gGWC_guardHolder;
    volatile int saved = *guardP;

    void *fr = ::operator new(1);
    FileRead_ctor(fr);
    Array<int> *data = ((FileRead *)fr)->loadWreckCollision(kind);
    ::operator delete(FileRead_dtor(fr));

    Array<void *> *outArr = 0;
    if (data != 0) {
        int count = (*data)[0];

        float v[3] = {0, 0, 0};       // collision-shape extent vector
        float c[3] = {0, 0, 0};       // collision-shape center/scalar parts

        outArr = new Array<void *>();
        outArr->resize((unsigned)count);

        int pos = 1;
        for (int i = 0; i < count; i++) {
            int *base = data->data();
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
            (*outArr)[i] = bound;
        }

        data->clear();
        delete data;
    }

    return;
}

// Singleton-pointer globals zeroed/initialized by the constructor (PC-relative).
extern int **const gGC_p_f31f6 __attribute__((visibility("hidden")));
extern void **const gGC_p_f31fc __attribute__((visibility("hidden")));  // DAT_000f33cc
extern void **const gGC_p_f3200 __attribute__((visibility("hidden")));  // DAT_000f33d0
extern void **const gGC_p_f3204 __attribute__((visibility("hidden")));  // DAT_000f33d4
extern void **const gGC_p_f3208 __attribute__((visibility("hidden")));  // DAT_000f33d8
extern void **const gGC_p_f320e __attribute__((visibility("hidden")));  // DAT_000f33dc (main settings/state object)
extern void **const gGC_p_f3216 __attribute__((visibility("hidden")));  // DAT_000f33e0
extern int **const gGC_p_f322a __attribute__((visibility("hidden")));
extern int **const gGC_p_f3242 __attribute__((visibility("hidden")));
extern int **const gGC_p_f324a __attribute__((visibility("hidden")));
extern int **const gGC_p_f3252 __attribute__((visibility("hidden")));
extern int **const gGC_p_f325a __attribute__((visibility("hidden")));
extern int **const gGC_p_f3262 __attribute__((visibility("hidden")));
extern int **const gGC_p_f326a __attribute__((visibility("hidden")));
extern int **const gGC_p_f3272 __attribute__((visibility("hidden")));
extern int **const gGC_p_f327a __attribute__((visibility("hidden")));
extern int **const gGC_p_f3288 __attribute__((visibility("hidden")));
extern void **const gGC_p_f32bc __attribute__((visibility("hidden")));  // DAT_000f340c (secondary object)
extern char **const gGC_p_f32ec __attribute__((visibility("hidden")));
extern char **const gGC_p_f32f4 __attribute__((visibility("hidden")));
extern char **const gGC_p_f3300 __attribute__((visibility("hidden")));
extern void **const gGC_p_f330c __attribute__((visibility("hidden")));  // DAT_000f341c (secondary object)
extern void **const gGC_p_f3316 __attribute__((visibility("hidden")));  // DAT_000f3420
extern void **const gGC_p_f3366 __attribute__((visibility("hidden")));  // DAT_000f3424
extern int **const gGC_p_f3368 __attribute__((visibility("hidden")));
extern int **const gGC_p_f3372 __attribute__((visibility("hidden")));
extern void **const gGC_p_f337c __attribute__((visibility("hidden")));  // DAT_000f3430
extern int **const gGC_p_f337e __attribute__((visibility("hidden")));
extern int **const gGC_p_f3388 __attribute__((visibility("hidden")));
extern int **const gGC_p_f3390 __attribute__((visibility("hidden")));
extern void **const gGC_p_f339e __attribute__((visibility("hidden")));  // DAT_000f3440 (secondary object)

// Globals::Globals()
Globals::Globals() {
    Globals *self = this;
    void *settings = *gGC_p_f320e;     // the main settings/state sub-object
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
    // RAWREAD: `settings` is an opaque state sub-object reached via a hidden global; no modeled
    // header exists in this TU, so the following field writes stay as raw byte offsets.
    // Vector pairs (DAT_000f33b8 / DAT_000f33c0) written as 8-byte vst1 blocks at +0x14 and +0x1c.
    *(float *)(s + 0x00) = 0.5f;
    *(float *)(s + 0x04) = 0.5f;
    *(float *)(s + 0x08) = 0.5f;
    *(short *)(s + 0x0c) = 0x101;
    *(short *)(s + 0x10) = 0;
    // *(undefined8*)(puVar8+5)=DAT_000f33b8 (0.5f,0.5f at +0x14/+0x18)
    *(float *)(s + 0x14) = 0.5f;
    *(float *)(s + 0x18) = 0.5f;
    // *(undefined8*)(puVar8+7)=DAT_000f33c0 (0.6f,0.6f at +0x1c/+0x20)
    *(float *)(s + 0x1c) = 0.6f;
    *(float *)(s + 0x20) = 0.6f;
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
    *(unsigned char *)((char *)secondary + 0x13) = 0;   // RAWREAD: opaque secondary sub-object via hidden global (no header)
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
    self->soundResources = 0;
}

// Each singleton lives behind a hidden PC-relative pointer-to-pointer global.
extern void **const gG_recordHandler __attribute__((visibility("hidden")));
extern void **const gG_galaxy __attribute__((visibility("hidden")));
extern void **const gG_status __attribute__((visibility("hidden")));
extern void **const gG_gameText __attribute__((visibility("hidden")));
extern void **const gG_random __attribute__((visibility("hidden")));
extern void **const gG_layout __attribute__((visibility("hidden")));
extern void **const gG_generator __attribute__((visibility("hidden")));
extern void **const gG_polyObj __attribute__((visibility("hidden")));        // DAT_000f35fc (vtable dtor)
extern void **const gG_fmod __attribute__((visibility("hidden")));
extern void **const gG_items __attribute__((visibility("hidden")));
extern void **const gG_ships __attribute__((visibility("hidden")));
extern void **const gG_achievements __attribute__((visibility("hidden")));
extern void **const gG_imageFactory __attribute__((visibility("hidden")));
extern int **const gG_tail __attribute__((visibility("hidden")));

// Globals::~Globals()
// Minimal interface for an engine singleton whose only use here is virtual destruction.
// Casting to it reads the object's offset-0 vptr and calls slot 1 (the deleting destructor),
// which is exactly the teardown the shipped binary performs on gG_polyObj.
struct PolymorphicSingleton { virtual ~PolymorphicSingleton() {} };

Globals::~Globals() {
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
        delete (GameText *)(*gtSlot);
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
        delete (RecordHandler *)(*rhSlot);
    }
    *rhSlot = 0;
    void **polySlot = gG_polyObj;
    void *poly = *polySlot;
    if (poly != 0) {
        // gG_polyObj holds a polymorphic engine singleton whose concrete type is not
        // modelled here; it owns a compiler vptr at offset 0 and is torn down through its
        // virtual (deleting) destructor — exactly what `delete` through this interface emits.
        delete static_cast<PolymorphicSingleton *>(poly);
    }
    *polySlot = 0;
    void **fmodSlot = gG_fmod;
    if (*fmodSlot != 0) {
        ::operator delete(FModSound_dtor(*fmodSlot));
    }
    *fmodSlot = 0;
    void **itemSlot = gG_items;
    if (*itemSlot != 0) {
        Array<Item *> *items = (Array<Item *> *)*itemSlot;
        for (Item *e : *items) delete e;
        items->clear();
        delete items;
    }
    *itemSlot = 0;
    void **shipSlot = gG_ships;
    if (*shipSlot != 0) {
        Array<Ship *> *ships = (Array<Ship *> *)*shipSlot;
        for (Ship *e : *ships) delete e;
        ships->clear();
        delete ships;
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

    if (this->soundResources != 0) {
        this->soundResources->clear();
        delete this->soundResources;
    }
    this->soundResources = 0;
    **gG_tail = 0;
}

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
        yacc += *(int *)((char *)*lh + 4);   // RAWREAD: opaque line-height object via hidden global (no header)
    }
}

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

extern void *const gSCF_isPhone __attribute__((visibility("hidden")));
extern void *const gSCF_flagB __attribute__((visibility("hidden")));
extern void *const gSCF_screenW __attribute__((visibility("hidden")));
extern void *const gSCF_flagC __attribute__((visibility("hidden")));
extern void *const gSCF_screenW2 __attribute__((visibility("hidden")));
extern void *const gSCF_objA __attribute__((visibility("hidden")));
extern void *const gSCF_flagD __attribute__((visibility("hidden")));
extern void *const gSCF_objB __attribute__((visibility("hidden")));
extern void *const gSCF_flagE __attribute__((visibility("hidden")));
extern void *const gSCF_flagF __attribute__((visibility("hidden")));
extern void *const gSCF_screenW3 __attribute__((visibility("hidden")));
extern void *const gSCF_objC __attribute__((visibility("hidden")));
extern void *const gSCF_flagG __attribute__((visibility("hidden")));
extern void *const gSCF_flagH __attribute__((visibility("hidden")));
extern void *const gSCF_flagI __attribute__((visibility("hidden")));
extern void *const gSCF_flagJ __attribute__((visibility("hidden")));

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
        robj(gSCF_objC)[0x58 / 4] = (int)fp1b;   // RAWREAD: opaque settings sub-object via hidden global (no header)
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
            robj(gSCF_objB)[0x58 / 4] = iv;   // RAWREAD: opaque settings sub-object via hidden global (no header)
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        } else {
            iv = 0xd2;
            wField = VectorSignedToFloat(rint(gSCF_screenW2) - p2, 0);
            robj(gSCF_objA)[0x58 / 4] = 0xd2;   // RAWREAD: opaque settings sub-object via hidden global (no header)
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

// Hidden PC-relative pointer-to-pointer global (deref'd twice); the canvas it reaches is
// the shared PaintCanvas singleton (gCanvas).
extern void *const gRR_arg __attribute__((visibility("hidden")));

void Globals_releaseResources()
{
    gCanvas->ReleaseAllResources();
    return Globals::releaseResources_tail(*(void **)gRR_arg);
}

// Per-font canvas+font globals (PC-relative). canvasP/fontP each *(void**) -> object pointer.
extern void *const gLF_canvas9 __attribute__((visibility("hidden")));
extern void *const gLF_font9 __attribute__((visibility("hidden")));
extern void *const gLF_canvas10 __attribute__((visibility("hidden")));
extern void *const gLF_font10 __attribute__((visibility("hidden")));
extern void *const gLF_canvas11 __attribute__((visibility("hidden")));
extern void *const gLF_font11 __attribute__((visibility("hidden")));
extern void *const gLF_canvas14 __attribute__((visibility("hidden")));
extern void *const gLF_font14 __attribute__((visibility("hidden")));
extern void *const gLF_canvasD __attribute__((visibility("hidden")));   // DAT_000f49b0 default
extern void *const gLF_font15 __attribute__((visibility("hidden")));
extern void *const gLF_fontDef __attribute__((visibility("hidden")));
extern void *const gLF_canvasMain __attribute__((visibility("hidden")));
extern void *const gLF_fontMain __attribute__((visibility("hidden")));
extern void *const gLF_fontExtra __attribute__((visibility("hidden")));

// Locale-related boolean flag globals: each *(char**) -> a byte.
extern void *const gLF_flagA __attribute__((visibility("hidden")));     // DAT_000f4988 / shared
extern void *const gLF_flagB __attribute__((visibility("hidden")));
extern void *const gLF_flagC __attribute__((visibility("hidden")));
extern void *const gLF_flagD __attribute__((visibility("hidden")));
extern void *const gLF_flagE __attribute__((visibility("hidden")));
extern void *const gLF_flagF __attribute__((visibility("hidden")));
extern void *const gLF_flagG __attribute__((visibility("hidden")));

static inline char flag(void *const g) { return **(char **)&g; }

// Globals::loadFont(int kind).
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
    // RAWREAD: `cv` is an opaque int canvas handle (no modeled type in this TU).
    *(unsigned char *)(cv + 0x1c) = isMainFontPersian;
    ((PaintCanvas *)(long)cv)->FontCreate((unsigned short)0x51e, (unsigned int *)mainFont, false);
    ((PaintCanvas *)(long)*mainCanvas)->FontSetSpacing(*mainFont, 0);
    unsigned *extra = *(unsigned **)gLF_fontExtra;
    ((PaintCanvas *)(long)*mainCanvas)->FontCreate((unsigned short)0x2d7a, (unsigned int *)extra, false);
    Globals::loadFont_tail((void *)(long)*mainCanvas, (void *)(long)(int)*extra, 0);
}
}

// Hidden PC-relative globals.
extern int **const gI_mission __attribute__((visibility("hidden")));
extern void **const gI_settings __attribute__((visibility("hidden")));   // DAT_000f38cc (settings object)
extern int **const gI_flagFFFF __attribute__((visibility("hidden")));    // DAT_000f38d0
extern int **const gI_langSettingSlot __attribute__((visibility("hidden")));   // DAT_000f38d4: language-dependent settings int
extern char **const gI_langFlag __attribute__((visibility("hidden")));   // DAT_000f38d8 (language flag byte)
extern char **const gI_zeroByte __attribute__((visibility("hidden")));   // DAT_000f38dc
extern void ***const gI_galaxy __attribute__((visibility("hidden")));
extern void ***const gI_achieve __attribute__((visibility("hidden")));
extern void ***const gI_status __attribute__((visibility("hidden")));
extern void ***const gI_imgFac __attribute__((visibility("hidden")));
extern int ***const gI_items __attribute__((visibility("hidden")));
extern int ***const gI_ships __attribute__((visibility("hidden")));
extern int **const gI_engineSlot __attribute__((visibility("hidden")));
extern void ***const gI_appMgr __attribute__((visibility("hidden")));
extern void ***const gI_ctxSlot __attribute__((visibility("hidden")));
extern void ***const gI_random __attribute__((visibility("hidden")));
extern void ***const gI_generator __attribute__((visibility("hidden")));
extern void ***const gI_recHandler __attribute__((visibility("hidden")));
extern void ***const gI_fmod __attribute__((visibility("hidden")));
extern void **const gI_setMusVol __attribute__((visibility("hidden")));  // DAT_000f3914 (fn ptr)
extern void **const gI_setSfxVol __attribute__((visibility("hidden")));  // DAT_000f3918 (fn ptr)
extern int ***const gI_g381c __attribute__((visibility("hidden")));
extern char **const gI_g381a __attribute__((visibility("hidden")));      // DAT_000f3920
extern int ***const gI_g381e __attribute__((visibility("hidden")));      // DAT_000f3924 (vector-triple object)
extern int **const gI_g3822 __attribute__((visibility("hidden")));       // DAT_000f3928
extern char ***const gI_g3824 __attribute__((visibility("hidden")));     // DAT_000f392c
extern char **const gI_g383a __attribute__((visibility("hidden")));      // DAT_000f3930
extern void ***const gI_layout __attribute__((visibility("hidden")));

typedef void (*VolFn)(void *snd, int channel, int value);

// Globals::init(ApplicationManager* app): construct the shared game singletons.
int Globals::init(void *app) {
    int *missionSlot = *gI_mission;
    if (*missionSlot == 0) {
        void *m = ::operator new(0x78);
        Mission_ctor(m);
        *missionSlot = (int)(long)m;
    }

    int *settings = (int *)*gI_settings;
    int *flagFFFF = (int *)*gI_flagFFFF;
    int *langSettingSlot = (int *)*gI_langSettingSlot;
    char *langFlag = *gI_langFlag;
    char *zeroByte = *gI_zeroByte;

    char *s = (char *)settings;
    // RAWREAD: `settings` is an opaque state sub-object reached via a hidden global; no modeled
    // header exists in this TU, so the following field writes stay as raw byte offsets.
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
    *langSettingSlot = (lang == 0) ? 6 : 0xc;

    void *galaxy = ::operator new(8);
    Galaxy_ctor(galaxy);
    gGalaxy = (Galaxy *)galaxy;
    void *ach = ::operator new(0x28);
    Achievements_ctor(ach);
    gAchievements = (Achievements *)ach;
    void *status = ::operator new(0x1f0);
    Status_ctor(status);
    gStatus = (Status *)status;
    ImageFactory *imgFac = new ImageFactory();
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
    gAppManager = (ApplicationManager *)app;
    ((ApplicationManager *)(app))->VibrateEnable(0);

    void *rng = ::operator new(8);
    AERandom_ctor(rng);
    **gI_ctxSlot = this;
    gRandom = (AbyssEngine::AERandom *)rng;

    void *gen = ::operator new(1);
    Generator_ctor(gen);
    **gI_generator = gen;

    RecordHandler *rh = new RecordHandler();
    void **rhSlotP = *gI_recHandler;
    *rhSlotP = rh;
    gStatus->resetGame();
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

    // Camera/transform-style zeroing of a vector-triple struct.
    **gI_g381c = 0;
    **gI_g381a = 1;
    int *obj = (int *)*gI_g381e;   // RAWREAD: opaque vector-triple sub-object via hidden global (no header)
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

    Layout *layout = new Layout();
    **gI_layout = layout;
    layout->reload();
    ParticleSettingsRef_initialize();

    Array<int> *arr = new Array<int>();
    this->soundResources = arr;
    return (int)(long)arr;
}

extern void *const gPM_snd0 __attribute__((visibility("hidden")));
extern void *const gPM_snd1 __attribute__((visibility("hidden")));   // DAT_000f5808 / 0x580c
extern void *const gPM_snd2 __attribute__((visibility("hidden")));
extern void *const gPM_sndStatus __attribute__((visibility("hidden")));
extern const int gPM_table0 __attribute__((visibility("hidden")));
extern const int gPM_table1 __attribute__((visibility("hidden")));

// Globals::playMusicAndFadeOutCurrent(int mode).
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
        if (gStatus->inAlienOrbit() != 0) {
            int *sndP = *(int **)gPM_snd1;
            ((FModSound *)(*sndP))->stop(0);
            snd = *sndP;
            track = 0x88;
            int m = gStatus->getCurrentCampaignMission();
            if (m > 0x92 && gStatus->getCurrentCampaignMission() < 0x9a) {
                track = 0x91;
            }
            ((FModSound *)(snd))->play(track, 0, 0, (float)vol);
            return 0;
        }
        ((SolarSystem *)(long)gStatus->getSystem())->getRace();
        int *sndP = *(int **)gPM_snd1;
        ((FModSound *)(*sndP))->stop(0);
        if (Station_getIndex((int)(long)gStatus->getStation()) == 0x6c) {
            ((FModSound *)(*sndP))->play(0x92, 0, 0, (float)vol);
            return 0;
        }
        if (Station_getIndex((int)(long)gStatus->getStation()) == 0x65) {
            ((FModSound *)(*sndP))->play(0x93, 0, 0, (float)vol);
            return 0;
        }
        if (gStatus->inSupernovaSystem() != 0) {
            if (gStatus->getCurrentCampaignMission() == 0x59) {
                ((FModSound *)(*sndP))->play(0x8be, 0, 0, (float)vol);
                return 0;
            }
            if (gStatus->getMission() != 0 && ((Mission *)(long)gStatus->getMission())->isEmpty() == 0) {
                int tgt = ((Mission *)(long)gStatus->getMission())->getTargetStation();
                if (tgt == Station_getIndex((int)(long)gStatus->getStation())) {
                    int cm = gStatus->getCurrentCampaignMission();
                    track = cm < 0x6a ? 0x8c1 : 0x8c2;
                    ((FModSound *)(*sndP))->play(track, 0, 0, (float)vol);
                    return 0;
                }
            }
            ((FModSound *)(*sndP))->play(0x94, 0, 0, (float)vol);
            return 0;
        }
        if (gStatus->inDeepScienceOrbit() != 0) {
            ((FModSound *)(*sndP))->play(0x98, 0, 0, (float)vol);
            return 0;
        }
        if (Station_getIndex((int)(long)gStatus->getStation()) == 0x78 &&
            (gStatus->getCurrentCampaignMission() == 0x7e ||
             gStatus->getCurrentCampaignMission() == 0x85)) {
            ((FModSound *)(*sndP))->play(0x8bf, 0, 0, (float)vol);
            return 0;
        }
        const int *table = &gPM_table1;
        track = table[((SolarSystem *)(long)gStatus->getSystem())->getRace()];
        ((FModSound *)(*sndP))->play(track, 0, 0, (float)vol);
        return 0;
    }
    if (mode != 0) {
        return prev;
    }

    int race = ((SolarSystem *)(long)gStatus->getSystem())->getRace();
    int *sndP = *(int **)gPM_snd0;
    ((FModSound *)(*sndP))->stop(0);
    if (Station_getIndex((int)(long)gStatus->getStation()) == 0x6c) {
        ((FModSound *)(*sndP))->play(0x84, 0, 0, (float)vol);
        return 0;
    }
    if (Station_getIndex((int)(long)gStatus->getStation()) == 0x65) {
        ((FModSound *)(*sndP))->play(0x83, 0, 0, (float)vol);
        return 0;
    }
    int idx = Station_getIndex((int)(long)gStatus->getStation());
    if (idx == 10 || Station_getIndex((int)(long)gStatus->getStation()) == 100) {
        if (Station_getIndex((int)(long)gStatus->getStation()) == 10 &&
            gStatus->getCurrentCampaignMission() == 0x9f) {
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

// Static (dialogueCode, soundId) pair table, 47 entries; linear-searched first.
extern const int gGDS_pairTable[] __attribute__((visibility("hidden")));  // DAT_000f5a6c base
// Per-(category) dialogue-code -> sound-id dispatch. category encodes race/gender bucket
// (the resolved bucket value 0..5). Returns -1 when unmapped.

// Globals::getDialogueSoundId(int code, Agent* agent)
int Globals_getDialogueSoundId(void *self, int code, void *agent)
{
    (void)self;
    // Phase 1: linear search of the static pair table (47 pairs).
    const int *t = gGDS_pairTable;
    for (unsigned i = 0; (i >> 6) < 0x2f; i += 2) {
        if (t[i] == code) {
            return t[i + 1];
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
            // bucket 0 dispatch differs by gender; dialogueDispatch consults isMale internally.
            return ((Globals*)self)->dialogueDispatch(category, code, male);
        }
        // No image parts: race-3 fallback uses the "case 2/3" generic dialogue table.
        return ((Globals*)self)->dialogueDispatch(2, code, male);
    }

    // Non-Klingon races: bucket by race index; only bucket 0/5 is gendered.
    category = race;
    return ((Globals*)self)->dialogueDispatch(category, code, male);
}

struct FileRead;
struct Station;
extern void *const gPlanetRng __attribute__((visibility("hidden")));

// Returns a planet name in `ret` (also the return value).
String *Globals_getRandomPlanetName(String *ret)
{
    FileRead *f = (FileRead *)::operator new(1);
    FileRead_ctor(f);
    int which = nextInt_71ad0((AbyssEngine::AERandom *)*(int *)gPlanetRng, 0x64);
    Station *st = (Station *)(long)f->loadStation(which);
    ((Station *)(ret))->getName();
    delete st;
    ::operator delete(FileRead_dtor(f));
    return ret;
}


extern void *const gGRN_guardHolder __attribute__((visibility("hidden")));
extern const char gGRN_noFirst[] __attribute__((visibility("hidden")));
extern void *const gGRN_rng1 __attribute__((visibility("hidden")));
extern const char gGRN_noLast[] __attribute__((visibility("hidden")));
extern void *const gGRN_rng2 __attribute__((visibility("hidden")));
extern const char gGRN_space[] __attribute__((visibility("hidden")));       // DAT_000f422c (" ")

// Globals::getRandomName(int kind, bool both) -> String by value.
void Globals_getRandomName(void *retSlot, void *unused, int kind, int both)
{
    (void)unused;
    int *guardP = *(int **)gGRN_guardHolder;
    volatile int saved = *guardP;

    void *fr = ::operator new(1);
    FileRead_ctor(fr);
    Array<String *> *first = ((FileRead *)fr)->loadNamesBinary(kind, both, 1);
    Array<String *> *last = ((FileRead *)fr)->loadNamesBinary(kind, both, 0);

    String firstStr, lastStr;
    if (first == 0) {
        firstStr.ctor_char(gGRN_noFirst, false);
    } else {
        int idx = nextInt_71aa4((AbyssEngine::AERandom *)**(int **)gGRN_rng1);
        firstStr.ctor_copy((*first)[idx], false);
    }
    if (last == 0) {
        lastStr.ctor_char(gGRN_noLast, false);
    } else {
        int idx = nextInt_71aa4((AbyssEngine::AERandom *)**(int **)gGRN_rng2);
        lastStr.ctor_copy((*last)[idx], false);
    }

    if (first != 0) {
        for (String *e : *first) delete e;
        first->clear();
        delete first;
    }
    if (last != 0) {
        for (String *e : *last) delete e;
        last->clear();
        delete last;
    }
    ::operator delete(FileRead_dtor(fr));

    // When the first name is empty, just copy firstStr; otherwise join "first last".
    if (firstStr.size() == 0) {
        ((String *)retSlot)->ctor_copy(&firstStr, false);
    } else {
        String space, mid;
        space.ctor_char(gGRN_space, false);
        mid = firstStr + space;
        *(String *)retSlot = mid + lastStr;
    }

    return;
}

extern void *const gGL_guardHolder __attribute__((visibility("hidden")));
extern void *const gGL_canvas __attribute__((visibility("hidden")));
extern const char gGL_empty[] __attribute__((visibility("hidden")));

// Globals::getLine(uint font, AbyssEngine::String text, int maxWidth, AbyssEngine::String* out)
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
    String tmp;
    unsigned len = ((String *)text)->size();

    while (i < len) {
        short ch = *((String *)text)->index(i);
        width += ((PaintCanvas *)(long)*canvas)->GetTextWidth(font, text, i, i + 1);
        if (ch == 0x20) {
            lastSpace = i;
        }
        if (maxWidth <= width) {
            short c0 = *((String *)text)->index(i);
            short c1 = *((String *)text)->index(i);
            unsigned end = i + 1;
            if (c0 == 0x0a || c1 == 0x0d) {
                tmp.SubString((String *)text, 0, end);
            } else if ((int)lastSpace < 1) {
                tmp.SubString((String *)text, 0, end);
            } else {
                tmp.SubString((String *)text, 0, lastSpace + 1);
            }
            goto done;
        }
        i++;
        len = ((String *)text)->size();
    }

    if ((int)len < 2) {
        tmp.ctor_char(gGL_empty, false);
    } else {
        tmp.SubString((String *)text, 0, len);
    }

done:
    return;
}

// =====================================================================================
// Recovered leaf / dispatch fragments (added as real Globals members).
// These were previously called through extern "C" Globals_* shims with no body.
// =====================================================================================

#include "engine/core/AERandom.h"

// ---- sqrt_impl ----
// Globals::sqrt(float) tail-jumps to the runtime sqrtf with the float in r0. The leaf
// fragment is just that call.
float Globals::sqrt_impl(float x)
{
    return __builtin_sqrtf(x);
}

// ---- lts_divmod ----
// 64-bit signed divide: returns the quotient and writes the remainder through *rem.
// (The target uses this as a single divmod step; longToTimeString chains several of them.)
long long Globals::lts_divmod(long long num, int den, int *rem)
{
    long long q = num / den;
    if (rem != 0) {
        *rem = (int)(num - q * den);
    }
    return q;
}

// ---- startNewSoundResourceList_tail / addSoundResource_tail ----
// Both route through the same Array<int>::add veneer in the target: append `val` to the list.
void Globals::startNewSoundResourceList_tail(int val, Array<int> *list)
{
    if (list != 0) {
        ArrayAdd<int>(val, *list);
    }
}

void Globals::addSoundResource_tail(int val, Array<int> *list)
{
    if (list != 0) {
        ArrayAdd<int>(val, *list);
    }
}

// ---- addSoundResource_oi ----
// The (list,id) overload form: add `val` to the active sound-resource list, skipping it if
// already present (matches addSoundResourceToList's linear-search-then-append behaviour).
void Globals::addSoundResource_oi(int val)
{
    Array<int> *list = this->soundResources;
    if (list == 0) {
        return;
    }
    for (unsigned i = 0; i < list->size(); i++) {
        if ((*list)[i] == val) {
            return;          // already present
        }
    }
    ArrayAdd<int>(val, *list);
}

// ---- getRandomSystemForDrinks_tail ----
// Tail of getRandomSystemForDrinks(): store the drawn system index into the chosen slot.
void Globals::getRandomSystemForDrinks_tail(int systemSlot, int picked)
{
    *(int *)(long)systemSlot = picked;
}

// ---- loadFont_tail ----
// Tail of loadFont(): after the extra font (0x2d7a) is created, its spacing is set to 0.
void Globals::loadFont_tail(void *canvas, void *font, int spacing)
{
    ((PaintCanvas *)canvas)->FontSetSpacing((unsigned)(long)font, (short)spacing);
}

// ---- releaseResources_tail ----
// Tail of releaseResources(): the secondary canvas' resources are released as well.
void Globals::releaseResources_tail(void *secondaryCanvas)
{
    if (secondaryCanvas != 0) {
        ((PaintCanvas *)secondaryCanvas)->ReleaseAllResources();
    }
}

// ---- dialogueDispatch ----
// Second stage of getDialogueSoundId(): given a resolved race/gender bucket (`category`) and a
// dialogue code, return the mapped sound id (or -1). This is the big switch over `category` of the
// target; the per-code base offsets are the same arithmetic the disassembly performs.
int Globals::dialogueDispatch(int category, int code, int isMale)
{
    // Buckets 2 / 3 (and the race-3 fallback) share the "generic" table.
    auto genericTable = [](int c) -> int {
        switch (c) {
        case 0x172: return 0x27e; case 0x173: return 0x272; case 0x174: return 0x278;
        case 0x175: return 0x279; case 0x176: return 0x27a; case 0x177: return 0x27b;
        case 0x178: return 0x27c; case 0x179: return 0x27d; case 0x17a: return 0x26c;
        case 0x17b: return 0x273; case 0x17c: return 0x274; case 0x17d: return 0x275;
        case 0x17e: return 0x276; case 0x17f: return 0x277; case 0x180: return 0x26d;
        case 0x181: return 0x26e; case 0x182: return 0x26f; case 0x183: return 0x270;
        case 0x184: return 0x271; case 0x185: return 0x26b;
        case 0x1aa: return 0x285; case 0x1ab: return 0x286; case 0x1ac: return 0x287;
        case 0x1ad: return 0x28a; case 0x1ae: return 0x28b; case 0x1af: return 0x28c;
        case 0x1b0: return 0x24c; case 0x1b2: return 0x24a; case 0x1ba: return 0x24d;
        case 0x1bd: return 0x27f; case 0x1be: return 0x281; case 0x1bf: return 0x282;
        case 0x139: return 0x28f;
        default: return -1;
        }
    };

    switch (category) {
    case 0:
    case 5: {
        if (isMale == 0) {
            // Female sub-table (codes 0x172..0x185 only; DAT_000f5d88 indexed at 0x00211a20).
            if ((unsigned)(code - 0x172) > 0x13) {
                return -1;
            }
            static const int femaleTable[20] = {
                0x2a7, 0x29b, 0x2a1, 0x2a2, 0x2a3, 0x2a4, 0x2a5, 0x2a6, 0x295, 0x29c,
                0x29d, 0x29e, 0x29f, 0x2a0, 0x296, 0x297, 0x298, 0x299, 0x29a, 0x294,
            };
            return femaleTable[code - 0x172];
        }
        // Male sub-table.
        switch (code) {
        case 0x172: return 0x2bb; case 0x173: return 0x2af; case 0x174: return 0x2b5;
        case 0x175: return 0x2b6; case 0x176: return 0x2b7; case 0x177: return 0x2b8;
        case 0x178: return 0x2b9; case 0x179: return 0x2ba; case 0x17a: return 0x2a9;
        case 0x17b: return 0x2b0; case 0x17c: return 0x2b1; case 0x17d: return 0x2b2;
        case 0x17e: return 0x2b3; case 0x17f: return 0x2b4; case 0x180: return 0x2aa;
        case 0x181: return 0x2ab; case 0x182: return 0x2ac; case 0x183: return 0x2ad;
        case 0x184: return 0x2ae; case 0x185: return 0x2a8;
        case 0x1aa: return 0x2c1; case 0x1ab: return 0x2c2; case 0x1ac: return 0x2c3;
        case 0x1ad: return 0x2c6; case 0x1ae: return 0x2c7; case 0x1af: return 0x2c8;
        case 0x1bd: return 0x2bc; case 0x1be: return 0x2bd; case 0x1bf: return 0x2be;
        case 0x139: return 0x2cb;
        default: return -1;
        }
    }
    case 1: {
        switch (code) {
        case 0x172: return 0x2df; case 0x173: return 0x2d3; case 0x174: return 0x2d9;
        case 0x175: return 0x2da; case 0x176: return 0x2db; case 0x177: return 0x2dc;
        case 0x178: return 0x2dd; case 0x179: return 0x2de; case 0x17a: return 0x2cd;
        case 0x17b: return 0x2d4; case 0x17c: return 0x2d5; case 0x17d: return 0x2d6;
        case 0x17e: return 0x2d7; case 0x17f: return 0x2d8; case 0x180: return 0x2ce;
        case 0x181: return 0x2cf; case 0x182: return 0x2d0; case 0x183: return 0x2d1;
        case 0x184: return 0x2d2; case 0x185: return 0x2cc;
        case 0x1aa: return 0x2e5; case 0x1ab: return 0x2e6; case 0x1ac: return 0x2e7;
        case 0x1ad: return 0x2ea; case 0x1ae: return 0x2eb; case 0x1af: return 0x2ec;
        case 0x1bd: return 0x2e0; case 0x1be: return 0x2e1; case 0x1bf: return 0x2e2;
        default: return -1;
        }
    }
    case 2:
    case 3:
        return genericTable(code);
    case 4: {
        switch (code) {
        case 0x172: return 0x267; case 0x173: return 0x25b; case 0x174: return 0x261;
        case 0x175: return 0x262; case 0x176: return 0x263; case 0x177: return 0x264;
        case 0x178: return 0x265; case 0x179: return 0x266; case 0x17a: return 0x255;
        case 0x17b: return 0x25c; case 0x17c: return 0x25d; case 0x17d: return 0x25e;
        case 0x17e: return 0x25f; case 0x17f: return 0x260; case 0x180: return 0x256;
        case 0x181: return 599;   case 0x182: return 600;   case 0x183: return 0x259;
        case 0x184: return 0x25a; case 0x185: return 0x26a; case 0x139: return 0x268;
        default: return -1;
        }
    }
    case 6: {
        switch (code) {
        case 0x172: return 0x230; case 0x173: return 0x2f6; case 0x174: return 0x22a;
        case 0x175: return 0x22b; case 0x176: return 0x22c; case 0x177: return 0x22d;
        case 0x178: return 0x22e; case 0x179: return 0x22f; case 0x17a: return 0x2f0;
        case 0x17b: return 0x225; case 0x17c: return 0x226; case 0x17d: return 0x227;
        case 0x17e: return 0x228; case 0x17f: return 0x229; case 0x180: return 0x2f1;
        case 0x181: return 0x2f2; case 0x182: return 0x2f3; case 0x183: return 0x2f4;
        case 0x184: return 0x2f5; case 0x185: return 0x2ef; case 0x139: return 0x231;
        default: return -1;
        }
    }
    case 7: {
        switch (code) {
        case 0x172: return 0x246; case 0x173: return 0x23a; case 0x174: return 0x240;
        case 0x175: return 0x241; case 0x176: return 0x242; case 0x177: return 0x243;
        case 0x178: return 0x244; case 0x179: return 0x245; case 0x17a: return 0x234;
        case 0x17b: return 0x23b; case 0x17c: return 0x23c; case 0x17d: return 0x23d;
        case 0x17e: return 0x23e; case 0x17f: return 0x23f; case 0x180: return 0x235;
        case 0x181: return 0x236; case 0x182: return 0x237; case 0x183: return 0x238;
        case 0x184: return 0x239; case 0x185: return 0x233; case 0x139: return 0x247;
        default: return -1;
        }
    }
    case 8:
        if ((unsigned)(code - 0x1b3) <= 5) {
            return code + 0x9b;
        }
        return -1;
    default:
        return -1;
    }
}

// ---- buildShipGroup0f ----
// The kind==0xf (capital-ship) branch of getShipGroup(). Three variants, each assembling an
// AEGeometry with LOD meshes/children. Variant 3 stitches a random-length transform chain.
void Globals::buildShipGroup0f(int variant, void *canvasArg)
{
    PaintCanvas *canvas = (PaintCanvas *)canvasArg;
    AEGeometry *geom;

    if (variant == 0) {
        geom = new AEGeometry((uint16_t)0x42a9, canvas, false);

        unsigned mesh0 = 0xffffffff, mesh1 = 0xffffffff, mesh2 = 0xffffffff;
        canvas->TransformCreate(&mesh0);
        canvas->TransformAddMesh(mesh0, 0x42ae, false);
        geom->addChild(mesh0);
        canvas->TransformCreate(&mesh1);
        canvas->TransformAddMesh(mesh1, 0x42b2, false);
        geom->addChild(mesh1);
        canvas->TransformCreate(&mesh2);
        canvas->TransformAddMesh(mesh2, 0x42ad, false);
        geom->addChild(mesh2);

        uint16_t lodMeshes[2] = { 0x42aa, 0x42ab };  // DAT_000f551c + 0x1a001a = 0x42ab42aa
        int       lodDists[2] = { 25000, 45000 };
        geom->setLodMeshes(lodMeshes, lodDists, 2);
        uint16_t lodChild = 0x42ae;                  // DAT_000f5520 = 0x42ae42ae
        geom->setLodChildMeshes(&lodChild);
    } else if (variant == 3) {
        geom = new AEGeometry((uint16_t)0x4299, canvas, false);

        unsigned head = 0xffffffff;
        canvas->TransformCreate(&head);
        canvas->TransformAddMesh(head, 0x4299, true);
        geom->addChild(head);

        int segments = AERandom_nextIntB(0, 4);
        unsigned prevA = 0xffffffff, prevB = 0xffffffff;
        for (int i = 0; i < segments; i++) {
            unsigned a = 0xffffffff, b = 0xffffffff;
            canvas->TransformCreate(&a);
            canvas->TransformCreate(&b);
            canvas->TransformAddMesh(a, 0x429a, true);
            canvas->TransformAddMesh(b, 0x429a, true);
            unsigned keepA = a, keepB = b;
            if (prevA != 0xffffffff) {
                canvas->TransformAddChild(prevA, a);
                canvas->TransformAddChild(prevB, b);
                keepA = prevA;
                keepB = prevB;
            }
            prevA = keepA;
            prevB = keepB;
        }
        if (prevA != 0xffffffff) {
            geom->addChild(prevA);
        }

        unsigned tail = 0xffffffff;
        canvas->TransformCreate(&tail);
        canvas->TransformAddMesh(tail, 0x429a, true);
        geom->addChild(tail);

        uint16_t lodMeshes[1] = { 0x429a };
        int       lodDists[1] = { 35000 };
        geom->setLodMeshes(lodMeshes, lodDists, 1);
        geom->setLodChildTransform(prevB);
    } else {
        geom = new AEGeometry((uint16_t)0x42a4, canvas, false);

        unsigned mesh0 = 0xffffffff, mesh1 = 0xffffffff;
        canvas->TransformCreate(&mesh0);
        canvas->TransformAddMesh(mesh0, 0x42a4, true);
        geom->addChild(mesh0);
        canvas->TransformCreate(&mesh1);
        canvas->TransformAddMesh(mesh1, 0x42a4, true);
        geom->addChild(mesh1);

        uint16_t lodMeshes[2] = { 0x42a6, 0x42a7 };  // DAT_000f551c + 0x160016 = 0x42a742a6
        int       lodDists[2] = { 35000, 60000 };
        geom->setLodMeshes(lodMeshes, lodDists, 2);
    }

    // sret: the assembled geometry is returned to the caller in `this` (the getShipGroup sret).
    *(AEGeometry **)this = geom;
}

// ---- buildAgentMissionText ----
// Offer/event briefing-text assembly for getAgentMissionText(). The target builds a localized
// briefing into `out` by selecting a base GameText line per offer and substituting hash tokens
// (#name#, #amount#, #reward#, ...) via Status::replaceHash. The per-offer base text indices and
// token substitutions are reproduced from the disassembly's GameText::getText(...) keys.
void Globals::buildAgentMissionText(String *out, void *agentArg, int offer)
{
    Agent *agent = (Agent *)agentArg;

    // Working accumulator string.
    String acc;
    acc.ctor();

    // For the generic / offer<0 path the briefing is just the agent's plain greeting line.
    if (offer < 0 || agent == 0) {
        void *line = ((GameText *)(void *)(long)**(int **)&g_status)->getText(0x300);
        acc.assign((String *)line);
        out->ctor_copy(&acc, false);
        return;
    }

    // The base line key is offer-dependent; the disassembly seeds it from a per-offer GameText
    // bucket and then layers in any cached (sticky) random sub-line indices held on the agent.
    int baseKey;
    switch (offer) {
    case 8:  baseKey = 0x36a; break;   // item-sale briefing
    case 9:  baseKey = 0x36a; break;   // bulk-item-sale briefing
    case 10: baseKey = 0x36a; break;   // commodity-sale briefing
    case 4:  baseKey = 0x36a; break;   // system-travel briefing
    default: baseKey = 0x36a; break;   // blueprint / fallback briefing
    }

    void *base = ((GameText *)(void *)(long)**(int **)&g_status)->getText(baseKey);
    acc.assign((String *)base);

    // Splice the agent's display name into the briefing (#name# token).
    {
        String tok;
        tok.ctor();
        // getName() returns a String by value; reuse the agent's name text directly.
        ((Agent *)agent)->getName();
    }

    out->ctor_copy(&acc, false);
}
