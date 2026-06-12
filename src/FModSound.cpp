#include "gof2/FModSound.h"


extern "C" void FMOD_setLanguage(void *system, uint32_t lang);
extern "C" int FMOD_Event_stop(void *event, int immediate);
extern "C" int FMOD_Event_setPaused(void *event, int paused);
extern "C" void FMOD_fade(void *self, int a, int s, float v);
extern "C" void FMOD_EventSystem_unload(void *system);
extern "C" void FMOD_EventSystem_release(void *system);
extern "C" void *AEFile_GetAppRootDir();
extern "C" FModSound *FMOD_Event_stop_p(void *event, int immediate);
extern "C" int FMOD_Event_setPitch(void *event, float pitch, int mode);
extern "C" int FMOD_Event_setVolume(void *event, float vol);
extern "C" int FMOD_Event_getProperty(void *event, void *prop, void *out, int b = 1);
extern "C" int FMOD_EventSystem_getNumReverbPresets(void *system, int *out);
extern "C" int FMOD_EventSystem_getReverbPresetByIndex(void *system, int idx, void *props, char **name);
extern "C" int FMOD_EventSystem_setReverbProperties(void *system, void *props);
extern "C" int FMOD_Event_getParameterByIndex(void *event, int idx, FMOD::EventParameter **out);
extern "C" int FMOD_EventParameter_setValue(FMOD::EventParameter *p, float v);
extern "C" void *__aeabi_memcpy8(void *dst, const void *src, uint32_t n);
extern "C" void FMOD_EventSystem_Create(void *out);
extern "C" int FMOD_EventSystem_init(void *system, int maxch, void *extdriver, int flags);
int GameText_getLanguage();
extern "C" void *__aeabi_memclr8(void *d, uint32_t n);
extern "C" char *strcpy(char *d, const char *s);
extern "C" __SIZE_TYPE__ strlen(const char *s);
extern "C" int FMOD_EventSystem_load(void *system, const char *name, void *proj);
extern "C" void FMOD_EventSystem_getCategory(void *system, void *out);
extern "C" void FMOD_EventSystem_getProjectByIndex(void *system, void *out);
extern "C" int FMOD_Event_getState(void *event, unsigned *out);
extern "C" int FMOD_Event_getParameter(void *event, const char *name, FMOD::EventParameter **out);
extern "C" int FMOD_EventSystem_getProject(void *system, const char *name, FMOD::EventProject **out);
extern "C" int FMOD_Event_getParentGroup(void *event, FMOD::EventGroup **out);
extern "C" int FMOD_Event_getCategory(void *event, FMOD::EventCategory **out = 0);
extern "C" void FMOD_play(void *self, int a, void *b, float v);

// ---- setAudioLanguage_92d80.cpp ----
// FModSound::setAudioLanguage(int)
//   if (system) tail-call ext(system, langTable[p1 == 1])
__attribute__((visibility("hidden"))) static const uint32_t langTable[2] = {0, 1};

void FModSound::setAudioLanguage(int p1)
{
    void *system = pp(this, OFF_SYSTEM);
    if (!system)
        return;
    return FMOD_setLanguage(system, langTable[p1 == 1]);
}

// ---- updateEvent3DAttributes_93588.cpp ----
namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

// FModSound::updateEvent3DAttributes(int, Vector*, Vector*, bool)
//   events[p1] = updateEvent3DAttributes(events[p1], p1, p2, p3, p4)
void FModSound::updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c)
{
    void **slot = (void **)((char *)this + idx * 4 + OFF_EVENTS);
    *slot = updateEvent3DAttributes(*slot, idx, a, b, c);
}

// ---- stopAll_93b4c.cpp ----
// FModSound::stopAll() -> for 0x8f5 slots: if (system && events[i]) Event::stop(events[i],1)

void FModSound::stopAll()
{
    int *sys = &i32(this, OFF_SYSTEM);
    void **base = (void **)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (*sys && base[i])
            FMOD_Event_stop(base[i], 1);
    }
}

// ---- resumeAll_93b80.cpp ----
// FModSound::resumeAll() -> for 0x8f5 slots: if (system && events[i]) Event::setPaused(events[i],0)

void FModSound::resumeAll()
{
    int *sys = &i32(this, OFF_SYSTEM);
    void **base = (void **)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (*sys && base[i])
            FMOD_Event_setPaused(base[i], 0);
    }
}

// ---- promptMusicCue_93ad8.cpp ----
// FModSound::promptMusicCue(int)
//   music = this->0x2400; if (music) tail-call music->vtbl[0x28](music, p1)
void FModSound::promptMusicCue(int p1)
{
    void *music = pp(this, OFF_MUSIC);
    if (!music)
        return;
    void **vt = *(void ***)music;
    typedef void (*Fn)(void *, int);
    return ((Fn)vt[0x28 / 4])(music, p1);
}

// ---- fadeOutNow_93550.cpp ----
// FModSound::fadeOutNow()
//   s = this[0]; if (s==-1 || this[4]==s) return; tail-call ext(this, 0, s, kConst)
static const float kFade = 0.1f;

void FModSound::fadeOutNow()
{
    int s = i32(this, 0);
    if (s == -1)
        return;
    if (i32(this, 4) == s)
        return;
    return FMOD_fade(this, 0, s, kFade);
}

// ---- release_92be6.cpp ----
// FModSound::release()

void FModSound::release()
{
    void *system = pp(this, OFF_SYSTEM);
    if (system != 0) {
        FMOD_EventSystem_unload(system);
        FMOD_EventSystem_release(pp(this, OFF_SYSTEM));
        u32(this, OFF_SYSTEM) = 0;
    }
    uint32_t *p = (uint32_t *)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i)
        *p++ = 0;
    for (int i = 0; i != 4; i++)
        u32(this, i * 4 + OFF_CATEGORY) = 0;
}

// ---- FModSound_92b70.cpp ----
// FModSound::FModSound()

FModSound::FModSound()
{
    u32(this, OFF_SYSTEM) = 0;
    u32(this, 0x2400) = 0;
    u32(this, 0x2404) = 0;
    pp(this, 0xc) = AEFile_GetAppRootDir();
    u8(this, 0x10) = 1;
    for (int i = 0x11; i != 0x15; i++)
        u8(this, i) = 1;
    for (int i = 0; i != 0x23d4; i += 4)
        u32(this, i + OFF_EVENTS) = 0;
    u32(this, 0x2438) = 0;
    u32(this, 0x2434) = 0;
    u32(this, 0x2424) = 0;
    u32(this, 0x2428) = 0;
    u32(this, 0x242c) = 0;
    u32(this, 0x2430) = 0;
}

// ---- stop_9357c.cpp ----
// FModSound::stop(FMOD::Event*) -> if (e) return Event::stop(e, 0); else this
struct FModSound;

FModSound *FModSound::stop(void *e)
{
    if (e == 0)
        return this;
    return FMOD_Event_stop_p(e, 0);
}

// ---- setVolume_92fb8.cpp ----
// FModSound::setVolume(int, float)
//   if (system && (h=this[p1*4+0x23ec])) tail-call h->vtbl[0x20](h, vol)
void FModSound::setVolume(int p1, float vol)
{
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void *h = pp(this, p1 * 4 + OFF_CATEGORY);
    if (!h)
        return;
    void **vt = *(void ***)h;
    typedef void (*Fn)(void *, float);
    return ((Fn)vt[0x20 / 4])(h, vol);
}

// ---- pauseAll_93b18.cpp ----
// FModSound::pauseAll() -> for 0x8f5 slots: if (system && events[i]) Event::setPaused(events[i],1)

void FModSound::pauseAll()
{
    int *sys = &i32(this, OFF_SYSTEM);
    void **base = (void **)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (*sys && base[i])
            FMOD_Event_setPaused(base[i], 1);
    }
}

// ---- _FModSound_92bd4.cpp ----
// FModSound::~FModSound() -> release(this); (returns this)
FModSound::~FModSound()
{
    release();
}

// ---- setDownPitch_93a74.cpp ----
// FModSound::setDownPitch(bool)
//   pitch = down ? -1.0 : 0.1 ; this[8]=down ; for i<0x8f5: if(system&&events[i]&&isPlaying(i)) setPitch
static const float kUp = 0.1f;

void FModSound::setDownPitch(bool down)
{
    float pitch = down ? -1.0f : kUp;
    u8(this, 8) = (uint8_t)down;
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (u32(this, OFF_SYSTEM) && pp(this, i * 4 + OFF_EVENTS) && isPlaying(i))
            FMOD_Event_setPitch(pp(this, i * 4 + OFF_EVENTS), pitch, 1);
    }
}

// ---- stop_93568.cpp ----
// FModSound::stop(int) -> if (p1>=0 && events[p1]) tail-call Event::stop(h, 0)

void FModSound::stop(int p1)
{
    if (p1 < 0)
        return;
    void *h = pp(this, p1 * 4 + OFF_EVENTS);
    if (!h)
        return;
    FMOD_Event_stop(h, 0);
}

// ---- setMusicParamValue_92fa8.cpp ----
// FModSound::setMusicParamValue(int, float)
//   music = this->0x2400; if (music) tail-call music->vtbl[0x34](music, p1, p2)
void FModSound::setMusicParamValue(int p1, float p2)
{
    void *music = pp(this, OFF_MUSIC);
    if (!music)
        return;
    void **vt = *(void ***)music;
    typedef void (*Fn)(void *, int, float);
    return ((Fn)vt[0x34 / 4])(music, p1, p2);
}

// ---- setSoundVolume_92fd6.cpp ----
// FModSound::setSoundVolume(int, float) -> if (system && events[p1]) tail-call setVolume(h, vol)

void FModSound::setSoundVolume(int p1, float vol)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h)
            FMOD_Event_setVolume(h, vol);
    }
}

// ---- pauseAllPlaying_93bb4.cpp ----
// FModSound::pauseAllPlaying()
//   for i<0x8f5: if (system && events[i] && isPlaying(i)) Event::setPaused(events[i],1)

void FModSound::pauseAllPlaying()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (u32(this, OFF_SYSTEM)) {
            void **slot = (void **)((char *)this + i * 4 + OFF_EVENTS);
            if (*slot && isPlaying(i))
                FMOD_Event_setPaused(*slot, 1);
        }
    }
}

// ---- resume_93b00.cpp ----
// FModSound::resume(int) -> if (system && events[p1]) tail-call setPaused(h, 0)

void FModSound::resume(int p1)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h)
            FMOD_Event_setPaused(h, 0);
    }
}

// ---- getEventPauseLength_93a14.cpp ----
// FModSound::getEventPauseLength(int idx)
//   if (this[0x2404] && system && enabled[0x11] && events[idx]) getProperty(h, *gProp, &out, 1)
extern "C" void **gPauseProp;   // pc-relative global: holds &prop

int FModSound::getEventPauseLength(int idx)
{
    int out = 0;
    if (i32(this, OFF_FLAG2404) != 0 && u32(this, OFF_SYSTEM) != 0 && u8(this, OFF_ENABLED) != 0) {
        void *h = pp(this, idx * 4 + OFF_EVENTS);
        if (h != 0)
            FMOD_Event_getProperty(h, *gPauseProp, &out, 1);
    }
    return out;
}

// ---- enableReverb_938b8.cpp ----
// FModSound::enableReverb(int)
//   One stack array holds both the FMOD_REVERB_PROPERTIES (buf[0..0x4f]) and numPresets (buf+0x50),
//   matching the target's frame: buffer at sp+0, scalar at sp+0x50, canary at sp+0x54.

void FModSound::enableReverb(int p1)
{
    void *system = pp(this, OFF_SYSTEM);
    if (!system)
        return;
    char buf[0x54];
    if (FMOD_EventSystem_getNumReverbPresets(system, (int *)(buf + 0x50)) != 0)
        return;
    if (*(int *)(buf + 0x50) <= p1)
        return;
    if (this->f_2408 == p1)
        return;
    this->f_2408 = p1;
    if (FMOD_EventSystem_getReverbPresetByIndex(pp(this, OFF_SYSTEM), p1, buf, 0) == 0)
        FMOD_EventSystem_setReverbProperties(pp(this, OFF_SYSTEM), buf);
}

// ---- getPlayingProgress_93988.cpp ----
extern "C" {
int FMOD_Event_getInfo(void *event, char **name, void *info);
float VectorSignedToFloat(int v, int mode);
}

// FModSound::getPlayingProgress(int) -> queries the FMOD event info for a slot
// and converts two of its fields to float (progress numerator/denominator).

float FModSound::getPlayingProgress(int idx)
{
    if (pp(this, OFF_FLAG2404) != 0 && pp(this, OFF_SYSTEM) != 0 &&
        u8(this, OFF_ENABLED) != 0 &&
        pp(this, idx * 4 + OFF_EVENTS) != 0) {
        char *name = 0;
        int info[8];
        FMOD_Event_getInfo(pp(this, idx * 4 + OFF_EVENTS), &name, info);
        VectorSignedToFloat(info[2], 0);
        VectorSignedToFloat(info[1], 0);
    }
    return 0.0f;
}

// ---- play_932f4.cpp ----

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
int FMOD_EventSystem_getEventBySystemID(unsigned int system, int id, void **out);
int FMOD_Event_setPitch(void *event, float pitch, int mode);
int FMOD_Event_set3DAttributes(void *event, void *pos, void *vel);
int FMOD_Event_start(void *event);
}

__attribute__((visibility("hidden"))) extern void **g_fmodPropName; // property-name pointer
extern "C" extern float FModSound_defaultPitch;                     // DAT_000a3518


static void *cacheVec(void *self, uint32_t off, Vector *src)
{
    int *slot = (int *)pp(self, off);
    if (slot == 0) {
        slot = (int *)::operator new(0xc);
        slot[0] = 0;
        slot[1] = 0;
        slot[2] = 0;
        pp(self, off) = slot;
    }
    slot[0] = *(int *)src;
    slot[1] = *((int *)src + 1);
    slot[2] = *((int *)src + 2);
    return slot;
}

void FModSound::play(int idx, Vector *pos, Vector *vel, float pitch)
{
    if (pp(this, OFF_FLAG2404) == 0 || (unsigned int)idx >= 0x8f5 || pp(this, OFF_SYSTEM) == 0)
        return;

    int *slot = (int *)((char *)this + idx * 4 + OFF_EVENTS);
    void *event = (void *)(uintptr_t)*slot;
    bool freshLookup = (event == 0);

    if (freshLookup) {
        void *ev = 0;
        FMOD_EventSystem_getEventBySystemID((unsigned int)(uintptr_t)pp(this, OFF_SYSTEM), idx, &ev);
        event = ev;
    }

    if (event == 0)
        return;

    float basePitch = (this->f_8 == 0) ? FModSound_defaultPitch : -1.0f;
    FMOD_Event_setPitch(event, basePitch, 1);
    if (pitch != 0.0f)
        FMOD_Event_setPitch(event, pitch, 0);

    int category = FMOD_Event_getCategory(event);

    if (category == 0) {
        if (this->f_0 == 0)
            this->f_0 = idx;
        else if (category > 2)
            return;

        if (u8(this, idx + OFF_ENABLED) == 0)
            return;

        void *posPtr = 0;
        void *velPtr = 0;
        bool havePos = false, haveVel = false;
        if (pos != 0) { posPtr = cacheVec(this, 0x2434, pos); havePos = true; }
        if (vel != 0) { velPtr = cacheVec(this, 0x2438, vel); haveVel = true; }

        if (freshLookup) {
            void *dummy = 0;
            if (FMOD_EventSystem_getEventBySystemID(
                    (unsigned int)(uintptr_t)pp(this, OFF_SYSTEM), idx, &dummy) != 0)
                return;
            *slot = (int)(uintptr_t)event;
        }

        if (havePos || haveVel) {
            void *p = havePos ? this->f_2434 : 0;
            void *v = haveVel ? this->f_2438 : 0;
            FMOD_Event_set3DAttributes((void *)(uintptr_t)*slot, p, v);
        }

        int got = 0;
        if (FMOD_Event_getProperty((void *)(uintptr_t)*slot, *g_fmodPropName, &got) == 0 &&
            this->f_0 == 1) {
            int *p = (int *)((char *)this + 0x2410);
            for (unsigned int i = 0; i <= 4; i++) {
                if (p[i] == -1) {
                    p[i] = idx;
                    break;
                }
            }
        }
        FMOD_Event_start((void *)(uintptr_t)*slot);
    } else {
        if (*slot != 0) {
            FMOD::EventGroup *group = 0;
            if (FMOD_Event_getParentGroup((void *)(uintptr_t)*slot, &group) == 0) {
                void **sysObj = (void **)pp(this, OFF_SYSTEM);
                typedef int (*ReleaseFn)(void *, int, int);
                ReleaseFn fn = *(ReleaseFn *)(*(char **)sysObj + 8);
                fn(sysObj, *slot, 0);
                *slot = 0;
            }
        }
    }
}

// ---- resume_9384c.cpp ----
// FModSound::resume(FMOD::Event*) -> bool: e && system && setPaused(e,0)==0

bool FModSound::resume(void *e)
{
    bool ok = false;
    if (e != 0 && u32(this, OFF_SYSTEM) != 0) {
        int r = FMOD_Event_setPaused(e, 0);
        ok = r == 0;
    }
    return ok;
}

// ---- setParamValue_93874.cpp ----
namespace FMOD { struct Event; struct EventParameter; }

// FModSound::setParamValue(FMOD::Event* e, int paramIdx, float val)

void FModSound::setParamValue(FMOD::Event *e, int paramIdx, float val)
{
    if (e != 0 && u32(this, OFF_SYSTEM) != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(e, paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

// ---- stopAllSoundFXEvents_92dac.cpp ----
// FModSound::stopAllSoundFXEvents()
//   if (system) iterate category indices (inner skip on ==1, stop at 4): cat[i]->vtbl[0x1c](cat[i])
void FModSound::stopAllSoundFXEvents()
{
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void **cats = (void **)((char *)this + 0x23f0);
    unsigned j = 0;
    while (true) {
        unsigned i;
        do {
            i = j;
            j = i + 1;
        } while ((j & 0x7fffffff) == 1);
        if ((j & 0x7fffffff) == 4)
            break;
        void *c = cats[i];
        void **vt = *(void ***)c;
        typedef void (*Fn)(void *);
        ((Fn)vt[0x1c / 4])(c);
    }
}

// ---- pause_93ae8.cpp ----
// FModSound::pause(int)
//   system==0 -> return this ;  h==0 -> return 0 ;  else tail-call setPaused(h,1)

int FModSound::pause(int p1)
{
    void *self = this;
    if (u32(this, OFF_SYSTEM)) {
        self = pp(this, p1 * 4 + OFF_EVENTS);
        if (self)
            return FMOD_Event_setPaused(self, 1);
    }
    return (int)(long)self;
}

// ---- disableReverb_9392c.cpp ----
// FModSound::disableReverb()
//   if (system) { memcpy8(buf, &kRev, 0x50); setReverbProperties(system, buf); }
//   this->0x2408 = -1;

__attribute__((visibility("hidden"))) static const unsigned char kRev[0x50] = {0};

void FModSound::disableReverb()
{
    void *system = pp(this, OFF_SYSTEM);
    if (system) {
        unsigned char buf[0x50];
        __aeabi_memcpy8(buf, kRev, 0x50);
        FMOD_EventSystem_setReverbProperties(system, buf);
    }
    this->f_2408 = -1;
}

// ---- init_92c30.cpp ----
// FModSound::init()

__attribute__((visibility("hidden"))) static const char kSuffixA[16] = ".fev";
__attribute__((visibility("hidden"))) static const char kSuffixB[24] = "_low.fev";
__attribute__((visibility("hidden"))) static void *kCats[4];

int FModSound::init()
{
    for (int i = 0; i != 5; i++)
        u32(this, i * 4 + 0x2410) = 0xffffffff;
    FMOD_EventSystem_Create((char *)this + OFF_SYSTEM);
    FMOD_EventSystem_init(pp(this, OFF_SYSTEM), 0x20, (void *)0x82, 0);
    setAudioLanguage(GameText_getLanguage());

    char path[0x400];
    __aeabi_memclr8(path, 0x400);
    strcpy(path, (const char *)this->f_c);
    uint8_t lowFlag = this->f_10;
    char *end = path + strlen(path);
    if (lowFlag == 0) {
        __builtin_memcpy(end, kSuffixB, 16);
    } else {
        __builtin_memcpy(end, kSuffixA, 14);
    }
    FMOD_EventSystem_load(pp(this, OFF_SYSTEM), path, 0);
    for (int i = 0; i != 0x10; i += 4) {
        u32(this, i + OFF_CATEGORY) = 0;
        FMOD_EventSystem_getCategory(pp(this, OFF_SYSTEM), kCats[i / 4]);
    }
    FMOD_EventSystem_getProjectByIndex(pp(this, OFF_SYSTEM), 0);
    this->f_240c = 0;
    this->f_2408 = 0xffffffff;
    this->f_0 = 0xffffffff;
    this->f_4 = 0xffffffff;
    this->f_8 = 0;
    return 0;
}

// ---- setParamValue_92f20.cpp ----
namespace FMOD { struct EventParameter; }

// FModSound::setParamValue(int idx, int paramIdx, float val)

void FModSound::setParamValue(int paramIdx, int idx, float val)
{
    if (idx >= 0 && u32(this, OFF_SYSTEM) != 0 && pp(this, idx * 4 + OFF_EVENTS) != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(pp(this, idx * 4 + OFF_EVENTS), paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

// ---- isChannelActive_93228.cpp ----
// FModSound::isChannelActive(int) -> if (system && events[p1]) { getState(h,&s); r=(s>>4)&1; } else 0

unsigned FModSound::isChannelActive(int p1)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h != 0) {
            unsigned s;
            FMOD_Event_getState(h, &s);
            return (s >> 4) & 1;
        }
    }
    return 0;
}

// ---- playMusicFadeOutCurrent_93524.cpp ----
// FModSound::playMusicFadeOutCurrent(int p1)
//   if (this[0] != p1) { if (this[0]==-1) {this[0]=p1; s=p1;} setParamValue(0,s,0.1f); this[4]=p1; }

void FModSound::playMusicFadeOutCurrent(int p1)
{
    int s = this->f_0;
    if (s == p1)
        return;
    if (s == -1) {
        this->f_0 = p1;
        s = p1;
    }
    setParamValue(0, s, kFade);
    this->f_4 = p1;
}

// ---- getParam_92ee4.cpp ----
namespace FMOD { struct EventParameter; }

// FModSound::getParam(char const*, int) -> if (system && events[idx]) getParameter(h, name, &out)

void FModSound::getParam(const char *name, int idx)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, idx * 4 + OFF_EVENTS);
        if (h != 0) {
            FMOD::EventParameter *out;
            FMOD_Event_getParameter(h, name, &out);
        }
    }
}

// ---- updateAll_92ff0.cpp ----

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
int FMOD_EventSystem_set3DListenerAttributes(int system, void *zero, void *pos, void *vel,
                                             void *forward);
int FMOD_EventSystem_update(int system);
}


void FModSound::updateAll(Vector *pos, Vector *vel, Vector *forward, Vector *up)
{
    int system = i32(this, OFF_SYSTEM);
    if (system == 0)
        return;

    unsigned int havePos = 0;
    if (pos != 0) { cacheVec(this, 0x2424, pos); havePos = 1; }
    unsigned int haveVel = 0;
    if (vel != 0) { cacheVec(this, 0x2428, vel); haveVel = 1; }
    unsigned int haveFwd = 0;
    if (forward != 0) { cacheVec(this, 0x242c, forward); haveFwd = 1; }

    bool haveUp = false;
    if (up == 0) {
        if ((havePos | haveVel | haveFwd) == 1) {
            // Nothing to set; fall through to the update + reap loop.
            goto afterListener;
        }
        haveUp = false;
    } else {
        cacheVec(this, 0x2430, up);
        haveUp = true;
    }

    {
        void *pPos = havePos ? this->f_2424 : 0;
        void *pUp = haveUp ? this->f_2430 : 0;
        void *pVel = haveVel ? this->f_2428 : 0;
        FMOD_EventSystem_set3DListenerAttributes(system, 0, pPos, pUp, pVel);
    }

afterListener:
    FMOD_EventSystem_update(system);

    for (int i = 0; i != 5; i++) {
        int slotIdx = i32(this, i * 4 + 0x2410);
        int *evp = 0;
        int ev = 0;
        if (slotIdx != -1) {
            evp = (int *)((char *)this + slotIdx * 4 + OFF_EVENTS);
            ev = *evp;
        }
        if (slotIdx != -1 && ev != 0 && isPlaying(slotIdx) == 0) {
            FMOD::EventGroup *group = 0;
            if (FMOD_Event_getParentGroup((void *)(uintptr_t)*evp, &group) == 0) {
                // Release the event back to the system.
                void **sysObj = (void **)pp(this, OFF_SYSTEM);
                typedef int (*ReleaseFn)(void *, int, int);
                ReleaseFn fn = *(ReleaseFn *)(*(char **)sysObj + 8);
                fn(sysObj, *evp, 0);
                *evp = 0;
                i32(this, i * 4 + 0x2410) = -1;
            }
        }
    }
}

// ---- isPlaying_931e4.cpp ----
// FModSound::isPlaying(int) -> if (system && events[p1]) { getState(h,&s); r=(s>>3)&1; } else 0
// NOTE: target carries a stack canary for the 4-byte state slot (built with stack-protector-strong
// upstream); basic -fstack-protector here does not, so this is a near-match.

unsigned FModSound::isPlaying(int p1)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h != 0) {
            unsigned s;
            FMOD_Event_getState(h, &s);
            return (s >> 3) & 1;
        }
    }
    return 0;
}

// ---- freeAllEvents_92de0.cpp ----
namespace FMOD { struct EventProject; struct EventGroup; }

// FModSound::freeAllEvents()
__attribute__((visibility("hidden"))) static const char kProjName[8] = "GoF2";

void FModSound::freeAllEvents()
{
    if (u32(this, OFF_SYSTEM) != 0) {
        FMOD::EventProject *proj = 0;
        FMOD_EventSystem_getProject(pp(this, OFF_SYSTEM), kProjName, &proj);
        if (proj != 0) {
            void **ev = (void **)((char *)this + OFF_EVENTS);
            for (unsigned i = 0; i < 0x8f5u; ++i) {
                void *e = ev[i];
                if (e) {
                    FMOD::EventGroup *grp;
                    if (FMOD_Event_getParentGroup(e, &grp) == 0) {
                        unsigned st;
                        FMOD_Event_getState(ev[i], &st);
                        if ((st & 0x0a) == 0) {
                            void *ee = ev[i];
                            void **vt = *(void ***)ee;
                            typedef void (*Fn)(void *, int);
                            ((Fn)vt[8 / 4])(ee, 0);
                            ev[i] = 0;
                        }
                    }
                }
            }
        }
    }
    if (this->f_2424) operator delete(this->f_2424);
    this->f_2424 = 0;
    if (this->f_2428) operator delete(this->f_2428);
    this->f_2428 = 0;
    if (this->f_242c) operator delete(this->f_242c);
    this->f_242c = 0;
    if (this->f_2430) operator delete(this->f_2430);
    this->f_2430 = 0;
    if (this->f_2434) operator delete(this->f_2434);
    this->f_2434 = 0;
    if (this->f_2438) operator delete(this->f_2438);
    this->f_2438 = 0;
}

// ---- IsCategoryEnabled_9326c.cpp ----
// FModSound::IsCategoryEnabled(int) -> p1<=3 && system && enabled[p1] ? 1 : 0
uint8_t FModSound::IsCategoryEnabled(int p1)
{
    uint8_t r = 0;
    if (p1 <= 3 && u32(this, OFF_SYSTEM) != 0 && u8(this, p1 + OFF_ENABLED) != 0)
        r = 1;
    return r;
}

// ---- setParamValue_92f64.cpp ----
namespace FMOD { struct EventParameter; }

// FModSound::setParamValue(char const* name, int idx, float val)

void FModSound::setParamValue(const char *name, int idx, float val)
{
    if (idx >= 0 && u32(this, OFF_SYSTEM) != 0 && pp(this, idx * 4 + OFF_EVENTS) != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameter(pp(this, idx * 4 + OFF_EVENTS), name, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

// ---- pause_937f0.cpp ----
// FModSound::pause(FMOD::Event*) -> int
//   if (e && system) { if (getState(e,&s)==0 && (s<<0x1c)<0) return setPaused(e,1)==0; return 0; }

int FModSound::pause(void *e)
{
    unsigned r = 0;
    if (e != 0 && u32(this, OFF_SYSTEM) != 0) {
        unsigned s;
        int st = FMOD_Event_getState(e, &s);
        if (st == 0 && (int)(s << 0x1c) < 0)
            r = FMOD_Event_setPaused(e, 1) == 0;
        else
            r = 0;
    }
    return r;
}

// ---- updateEvent3DAttributes_935a8.cpp ----

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
int FMOD_EventSystem_getEventBySystemID(unsigned int system, int id, void **out);
int FMOD_Event_set3DAttributes(void *event, void *pos, void *vel);
int FMOD_Event_start(void *event);
int FMOD_Event_getState(void *event, unsigned int *state);
}

// FModSound::updateEvent3DAttributes(Event*, int, Vector*, Vector*, bool)

// Copy a Vector (3 floats) into a cached FMOD_VECTOR slot, allocating if needed.
void *FModSound::updateEvent3DAttributes(void *event, int idx, Vector *pos, Vector *vel, bool restart)
{
    if (pp(this, OFF_FLAG2404) == 0 || u8(this, OFF_ENABLED) == 0)
        return event;

    int category = (event == 0) ? 0x24 : FMOD_Event_getCategory(event);

    if (event == 0 || category == 0x24) {
        void *ev = 0;
        if (FMOD_EventSystem_getEventBySystemID((unsigned int)(uintptr_t)pp(this, OFF_SYSTEM),
                                                idx, &ev) != 0)
            return event;
        if (FMOD_Event_getCategory(ev) != 0)
            return event;
        if (u8(this, idx + 0x12) == 0)
            return event;

        void *posPtr = 0;
        void *velPtr = 0;
        bool havePos = false;
        if (pos != 0) {
            posPtr = cacheVec(this, 0x2434, pos);
            havePos = true;
        }
        if (vel != 0) {
            velPtr = cacheVec(this, 0x2438, vel);
            posPtr = havePos ? this->f_2434 : 0;
        } else if (!havePos) {
            return event;
        } else {
            posPtr = this->f_2434;
        }

        if (FMOD_Event_set3DAttributes(ev, posPtr, velPtr) == 0) {
            void *dummy = 0;
            if (FMOD_EventSystem_getEventBySystemID(
                    (unsigned int)(uintptr_t)pp(this, OFF_SYSTEM), idx, &dummy) == 0)
                FMOD_Event_start(ev);
        }
        return ev;
    }

    // Direct-event path (event already resolved).
    if (u8(this, idx + 0x12) == 0)
        return event;

    void *posPtr = 0;
    void *velPtr = 0;
    bool havePos = false;
    if (pos != 0) {
        posPtr = cacheVec(this, 0x2434, pos);
        havePos = true;
    }
    if (vel != 0) {
        velPtr = cacheVec(this, 0x2438, vel);
        posPtr = havePos ? this->f_2434 : 0;
    } else if (!havePos) {
        return event;
    } else {
        posPtr = this->f_2434;
    }

    if (FMOD_Event_set3DAttributes(event, posPtr, velPtr) == 0 && restart) {
        unsigned int state = 0;
        FMOD_Event_getState(event, &state);
        if ((int)(state << 0x1c) < 0)
            FMOD_Event_stop(event, false);
        FMOD_Event_start(event);
    }
    return event;
}

// ---- pauseAllPlayingSoundFXEvents_93bfc.cpp ----
namespace FMOD { struct EventCategory; }

// FModSound::pauseAllPlayingSoundFXEvents()
//   for i<0x8f5: if (system && events[i] && isPlaying(i)) { getCategory(h,&cat);
//       cat->vtbl[0](cat,&a,&b); if (a==1) setPaused(h,1); }

void FModSound::pauseAllPlayingSoundFXEvents()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (u32(this, OFF_SYSTEM)) {
            void *h = pp(this, i * 4 + OFF_EVENTS);
            if (h && isPlaying(i)) {
                FMOD::EventCategory *cat;
                FMOD_Event_getCategory(h, &cat);
                int a, b;
                void **vt = *(void ***)cat;
                typedef void (*Fn)(void *, int *, int *);
                ((Fn)vt[0])(cat, &a, &b);
                if (a == 1)
                    FMOD_Event_setPaused(pp(this, i * 4 + OFF_EVENTS), 1);
            }
        }
    }
}

// ---- enableCategory_93288.cpp ----
// FModSound::enableCategory(int p1, bool enable)

void FModSound::enableCategory(int p1, bool enable)
{
    if (p1 > 3)
        return;
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void *cat = pp(this, p1 * 4 + OFF_CATEGORY);
    if (cat == 0)
        return;

    u8(this, p1 + OFF_ENABLED) = (uint8_t)enable;
    if (!enable) {
        void **vt = *(void ***)cat;
        typedef void (*Fn)(void *);
        ((Fn)vt[0x1c / 4])(cat);
    } else if (p1 == 1 && this->f_0 >= 0) {
        FMOD_play(this, this->f_0, 0, 0.0f);
    }

    uint8_t any = 0;
    int i = 0x12;
    do {
        if ((unsigned)(i - 0x11) > 3)
            break;
        uint8_t v = u8(this, i);
        i++;
        any = (any || v) ? 1 : 0;
    } while (u8(this, i - 1) == 0);
    u8(this, OFF_ENABLED) = any;
}

// ---- tryToStopMusicForBGMusic_a2da8 ----
// Stub in the shipped build: it never stops the current track and reports that
// nothing was stopped, so callers fall back to their own background music.
int FModSound::tryToStopMusicForBGMusic()
{
    return 0;
}
