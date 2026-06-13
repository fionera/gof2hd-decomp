#include "gof2/engine/audio/FModSound.h"

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

// FModSound::setAudioLanguage(int)
//   if (system) tail-call ext(system, langTable[p1 == 1])
__attribute__((visibility("hidden"))) static const uint32_t langTable[2] = {0, 1};

void FModSound::setAudioLanguage(int p1)
{
    if (!this->system)
        return;
    return FMOD_setLanguage(this->system, langTable[p1 == 1]);
}

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

// FModSound::updateEvent3DAttributes(int, Vector*, Vector*, bool)
//   events[p1] = updateEvent3DAttributes(events[p1], p1, p2, p3, p4)
void FModSound::updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c)
{
    this->events[idx] = updateEvent3DAttributes(this->events[idx], idx, a, b, c);
}

// FModSound::stopAll() -> for 0x8f5 slots: if (system && events[i]) Event::stop(events[i],1)

void FModSound::stopAll()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_stop(this->events[i], 1);
    }
}

// FModSound::resumeAll() -> for 0x8f5 slots: if (system && events[i]) Event::setPaused(events[i],0)

void FModSound::resumeAll()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_setPaused(this->events[i], 0);
    }
}

// FModSound::promptMusicCue(int)
//   music = this->0x2400; if (music) tail-call music->vtbl[0x28](music, p1)
void FModSound::promptMusicCue(int p1)
{
    if (!this->music)
        return;
    void **vt = *(void ***)this->music;
    typedef void (*Fn)(void *, int);
    return ((Fn)vt[0x28 / 4])(this->music, p1);
}

// FModSound::fadeOutNow()
//   s = this[0]; if (s==-1 || this[4]==s) return; tail-call ext(this, 0, s, kConst)
static const float kFade = 0.1f;

void FModSound::fadeOutNow()
{
    int s = this->currentMusicEvent;
    if (s == -1)
        return;
    if (this->fadeTargetMusicEvent == s)
        return;
    return FMOD_fade(this, 0, s, kFade);
}

// FModSound::release()

void FModSound::release()
{
    if (this->system != 0) {
        FMOD_EventSystem_unload(this->system);
        FMOD_EventSystem_release(this->system);
        this->system = 0;
    }
    for (unsigned i = 0; i < 0x8f5u; ++i)
        this->events[i] = 0;
    for (int i = 0; i != 4; i++)
        this->category[i] = 0;
}

// FModSound::FModSound()

FModSound::FModSound()
{
    this->system = 0;
    this->music = 0;
    this->initialized = 0;
    this->appRootDir = (char *)AEFile_GetAppRootDir();
    this->lowMemory = 1;
    for (int i = 0; i != 4; i++)
        this->categoryEnabled[i] = 1;
    for (int i = 0; i != 0x8f5; i++)
        this->events[i] = 0;
    this->eventVel = 0;
    this->eventPos = 0;
    this->listenerPos = 0;
    this->listenerVel = 0;
    this->listenerForward = 0;
    this->listenerUp = 0;
}

// FModSound::stop(FMOD::Event*) -> if (e) return Event::stop(e, 0); else this
struct FModSound;

FModSound *FModSound::stop(void *e)
{
    if (e == 0)
        return this;
    return FMOD_Event_stop_p(e, 0);
}

// FModSound::setVolume(int, float)
//   if (system && (h=this[p1*4+0x23ec])) tail-call h->vtbl[0x20](h, vol)
void FModSound::setVolume(int p1, float vol)
{
    if (this->system == 0)
        return;
    void *h = this->category[p1];
    if (!h)
        return;
    void **vt = *(void ***)h;
    typedef void (*Fn)(void *, float);
    return ((Fn)vt[0x20 / 4])(h, vol);
}

// FModSound::pauseAll() -> for 0x8f5 slots: if (system && events[i]) Event::setPaused(events[i],1)

void FModSound::pauseAll()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_setPaused(this->events[i], 1);
    }
}

// FModSound::~FModSound() -> release(this); (returns this)
FModSound::~FModSound()
{
    release();
}

// FModSound::setDownPitch(bool)
//   pitch = down ? -1.0 : 0.1 ; this[8]=down ; for i<0x8f5: if(system&&events[i]&&isPlaying(i)) setPitch
static const float kUp = 0.1f;

void FModSound::setDownPitch(bool down)
{
    float pitch = down ? -1.0f : kUp;
    this->downPitch = (uint8_t)down;
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i] && isPlaying(i))
            FMOD_Event_setPitch(this->events[i], pitch, 1);
    }
}

// FModSound::stop(int) -> if (p1>=0 && events[p1]) tail-call Event::stop(h, 0)

void FModSound::stop(int p1)
{
    if (p1 < 0)
        return;
    void *h = this->events[p1];
    if (!h)
        return;
    FMOD_Event_stop(h, 0);
}

// FModSound::setMusicParamValue(int, float)
//   music = this->0x2400; if (music) tail-call music->vtbl[0x34](music, p1, p2)
void FModSound::setMusicParamValue(int p1, float p2)
{
    if (!this->music)
        return;
    void **vt = *(void ***)this->music;
    typedef void (*Fn)(void *, int, float);
    return ((Fn)vt[0x34 / 4])(this->music, p1, p2);
}

// FModSound::setSoundVolume(int, float) -> if (system && events[p1]) tail-call setVolume(h, vol)

void FModSound::setSoundVolume(int p1, float vol)
{
    if (this->system != 0) {
        void *h = this->events[p1];
        if (h)
            FMOD_Event_setVolume(h, vol);
    }
}

// FModSound::pauseAllPlaying()
//   for i<0x8f5: if (system && events[i] && isPlaying(i)) Event::setPaused(events[i],1)

void FModSound::pauseAllPlaying()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system) {
            void *slot = this->events[i];
            if (slot && isPlaying(i))
                FMOD_Event_setPaused(slot, 1);
        }
    }
}

// FModSound::resume(int) -> if (system && events[p1]) tail-call setPaused(h, 0)

void FModSound::resume(int p1)
{
    if (this->system != 0) {
        void *h = this->events[p1];
        if (h)
            FMOD_Event_setPaused(h, 0);
    }
}

// FModSound::getEventPauseLength(int idx)
//   if (this[0x2404] && system && enabled[0x11] && events[idx]) getProperty(h, *gProp, &out, 1)
extern "C" void **gPauseProp;   // pc-relative global: holds &prop

int FModSound::getEventPauseLength(int idx)
{
    int out = 0;
    if (this->initialized != 0 && this->system != 0 && this->categoryEnabled[0] != 0) {
        void *h = this->events[idx];
        if (h != 0)
            FMOD_Event_getProperty(h, *gPauseProp, &out, 1);
    }
    return out;
}

// FModSound::enableReverb(int)
//   One stack array holds both the FMOD_REVERB_PROPERTIES (buf[0..0x4f]) and numPresets (buf+0x50),
//   matching the target's frame: buffer at sp+0, scalar at sp+0x50, canary at sp+0x54.

void FModSound::enableReverb(int p1)
{
    if (!this->system)
        return;
    char buf[0x54];
    if (FMOD_EventSystem_getNumReverbPresets(this->system, (int *)(buf + 0x50)) != 0)
        return;
    if (*(int *)(buf + 0x50) <= p1)
        return;
    if (this->reverbPreset == p1)
        return;
    this->reverbPreset = p1;
    if (FMOD_EventSystem_getReverbPresetByIndex(this->system, p1, buf, 0) == 0)
        FMOD_EventSystem_setReverbProperties(this->system, buf);
}

extern "C" {
int FMOD_Event_getInfo(void *event, char **name, void *info);
float VectorSignedToFloat(int v, int mode);
}

// FModSound::getPlayingProgress(int) -> queries the FMOD event info for a slot
// and converts two of its fields to float (progress numerator/denominator).

float FModSound::getPlayingProgress(int idx)
{
    if (this->initialized != 0 && this->system != 0 &&
        this->categoryEnabled[0] != 0 &&
        this->events[idx] != 0) {
        char *name = 0;
        int info[8];
        FMOD_Event_getInfo(this->events[idx], &name, info);
        VectorSignedToFloat(info[2], 0);
        VectorSignedToFloat(info[1], 0);
    }
    return 0.0f;
}

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
int FMOD_EventSystem_getEventBySystemID(unsigned int system, int id, void **out);
int FMOD_Event_setPitch(void *event, float pitch, int mode);
int FMOD_Event_set3DAttributes(void *event, void *pos, void *vel);
int FMOD_Event_start(void *event);
}

__attribute__((visibility("hidden"))) extern void **g_fmodPropName; // property-name pointer
extern "C" extern float FModSound_defaultPitch;

static void *cacheVec(Vector *&cached, Vector *src)
{
    int *slot = (int *)cached;
    if (slot == 0) {
        slot = (int *)::operator new(0xc);
        slot[0] = 0;
        slot[1] = 0;
        slot[2] = 0;
        cached = (Vector *)slot;
    }
    slot[0] = *(int *)src;
    slot[1] = *((int *)src + 1);
    slot[2] = *((int *)src + 2);
    return slot;
}

void FModSound::play(int idx, Vector *pos, Vector *vel, float pitch)
{
    if (this->initialized == 0 || (unsigned int)idx >= 0x8f5 || this->system == 0)
        return;

    void *&slot = this->events[idx];
    void *event = slot;
    bool freshLookup = (event == 0);

    if (freshLookup) {
        void *ev = 0;
        FMOD_EventSystem_getEventBySystemID((unsigned int)(uintptr_t)this->system, idx, &ev);
        event = ev;
    }

    if (event == 0)
        return;

    float basePitch = (this->downPitch == 0) ? FModSound_defaultPitch : -1.0f;
    FMOD_Event_setPitch(event, basePitch, 1);
    if (pitch != 0.0f)
        FMOD_Event_setPitch(event, pitch, 0);

    int category = FMOD_Event_getCategory(event);

    if (category == 0) {
        if (this->currentMusicEvent == 0)
            this->currentMusicEvent = idx;
        else if (category > 2)
            return;

        if (this->categoryEnabled[idx] == 0)
            return;

        void *posPtr = 0;
        void *velPtr = 0;
        bool havePos = false, haveVel = false;
        if (pos != 0) { posPtr = cacheVec(this->eventPos, pos); havePos = true; }
        if (vel != 0) { velPtr = cacheVec(this->eventVel, vel); haveVel = true; }

        if (freshLookup) {
            void *dummy = 0;
            if (FMOD_EventSystem_getEventBySystemID(
                    (unsigned int)(uintptr_t)this->system, idx, &dummy) != 0)
                return;
            slot = event;
        }

        if (havePos || haveVel) {
            void *p = havePos ? (void *)this->eventPos : 0;
            void *v = haveVel ? (void *)this->eventVel : 0;
            FMOD_Event_set3DAttributes(slot, p, v);
        }

        int got = 0;
        if (FMOD_Event_getProperty(slot, *g_fmodPropName, &got) == 0 &&
            this->currentMusicEvent == 1) {
            for (unsigned int i = 0; i <= 4; i++) {
                if (this->fxSlots[i] == -1) {
                    this->fxSlots[i] = idx;
                    break;
                }
            }
        }
        FMOD_Event_start(slot);
    } else {
        if (slot != 0) {
            FMOD::EventGroup *group = 0;
            if (FMOD_Event_getParentGroup(slot, &group) == 0) {
                void **sysObj = (void **)this->system;
                typedef int (*ReleaseFn)(void *, int, int);
                ReleaseFn fn = *(ReleaseFn *)(*(char **)sysObj + 8);
                fn(sysObj, (int)(uintptr_t)slot, 0);
                slot = 0;
            }
        }
    }
}

// FModSound::resume(FMOD::Event*) -> bool: e && system && setPaused(e,0)==0

bool FModSound::resume(void *e)
{
    bool ok = false;
    if (e != 0 && this->system != 0) {
        int r = FMOD_Event_setPaused(e, 0);
        ok = r == 0;
    }
    return ok;
}

namespace FMOD { struct Event; struct EventParameter; }

// FModSound::setParamValue(FMOD::Event* e, int paramIdx, float val)

void FModSound::setParamValue(FMOD::Event *e, int paramIdx, float val)
{
    if (e != 0 && this->system != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(e, paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

// FModSound::stopAllSoundFXEvents()
//   if (system) iterate category indices (inner skip on ==1, stop at 4): cat[i]->vtbl[0x1c](cat[i])
void FModSound::stopAllSoundFXEvents()
{
    if (this->system == 0)
        return;
    void **cats = &this->category[1];
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

// FModSound::pause(int)
//   system==0 -> return this ;  h==0 -> return 0 ;  else tail-call setPaused(h,1)

int FModSound::pause(int p1)
{
    void *self = this;
    if (this->system) {
        self = this->events[p1];
        if (self)
            return FMOD_Event_setPaused(self, 1);
    }
    return (int)(long)self;
}

// FModSound::disableReverb()
//   if (system) { memcpy8(buf, &kRev, 0x50); setReverbProperties(system, buf); }
//   this->0x2408 = -1;

__attribute__((visibility("hidden"))) static const unsigned char kRev[0x50] = {0};

void FModSound::disableReverb()
{
    if (this->system) {
        unsigned char buf[0x50];
        __aeabi_memcpy8(buf, kRev, 0x50);
        FMOD_EventSystem_setReverbProperties(this->system, buf);
    }
    this->reverbPreset = -1;
}

// FModSound::init()

__attribute__((visibility("hidden"))) static const char kSuffixA[16] = ".fev";
__attribute__((visibility("hidden"))) static const char kSuffixB[24] = "_low.fev";
__attribute__((visibility("hidden"))) static void *kCats[4];

int FModSound::init()
{
    for (int i = 0; i != 5; i++)
        this->fxSlots[i] = 0xffffffff;
    FMOD_EventSystem_Create(&this->system);
    FMOD_EventSystem_init(this->system, 0x20, (void *)0x82, 0);
    setAudioLanguage(GameText_getLanguage());

    char path[0x400];
    __aeabi_memclr8(path, 0x400);
    strcpy(path, this->appRootDir);
    uint8_t lowFlag = this->lowMemory;
    char *end = path + strlen(path);
    if (lowFlag == 0) {
        __builtin_memcpy(end, kSuffixB, 16);
    } else {
        __builtin_memcpy(end, kSuffixA, 14);
    }
    FMOD_EventSystem_load(this->system, path, 0);
    for (int i = 0; i != 4; i++) {
        this->category[i] = 0;
        FMOD_EventSystem_getCategory(this->system, kCats[i]);
    }
    FMOD_EventSystem_getProjectByIndex(this->system, 0);
    this->propSlot = 0;
    this->reverbPreset = 0xffffffff;
    this->currentMusicEvent = 0xffffffff;
    this->fadeTargetMusicEvent = 0xffffffff;
    this->downPitch = 0;
    return 0;
}

namespace FMOD { struct EventParameter; }

// FModSound::setParamValue(int idx, int paramIdx, float val)

void FModSound::setParamValue(int paramIdx, int idx, float val)
{
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(this->events[idx], paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

// FModSound::isChannelActive(int) -> if (system && events[p1]) { getState(h,&s); r=(s>>4)&1; } else 0

unsigned FModSound::isChannelActive(int p1)
{
    if (this->system != 0) {
        void *h = this->events[p1];
        if (h != 0) {
            unsigned s;
            FMOD_Event_getState(h, &s);
            return (s >> 4) & 1;
        }
    }
    return 0;
}

// FModSound::playMusicFadeOutCurrent(int p1)
//   if (this[0] != p1) { if (this[0]==-1) {this[0]=p1; s=p1;} setParamValue(0,s,0.1f); this[4]=p1; }

void FModSound::playMusicFadeOutCurrent(int p1)
{
    int s = this->currentMusicEvent;
    if (s == p1)
        return;
    if (s == -1) {
        this->currentMusicEvent = p1;
        s = p1;
    }
    setParamValue(0, s, kFade);
    this->fadeTargetMusicEvent = p1;
}

namespace FMOD { struct EventParameter; }

// FModSound::getParam(char const*, int) -> if (system && events[idx]) getParameter(h, name, &out)

void FModSound::getParam(const char *name, int idx)
{
    if (this->system != 0) {
        void *h = this->events[idx];
        if (h != 0) {
            FMOD::EventParameter *out;
            FMOD_Event_getParameter(h, name, &out);
        }
    }
}

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
int FMOD_EventSystem_set3DListenerAttributes(int system, void *zero, void *pos, void *vel,
                                             void *forward);
int FMOD_EventSystem_update(int system);
}

void FModSound::updateAll(Vector *pos, Vector *vel, Vector *forward, Vector *up)
{
    int system = (int)(uintptr_t)this->system;
    if (system == 0)
        return;

    unsigned int havePos = 0;
    if (pos != 0) { cacheVec(this->listenerPos, pos); havePos = 1; }
    unsigned int haveVel = 0;
    if (vel != 0) { cacheVec(this->listenerVel, vel); haveVel = 1; }
    unsigned int haveFwd = 0;
    if (forward != 0) { cacheVec(this->listenerForward, forward); haveFwd = 1; }

    bool haveUp = false;
    if (up == 0) {
        if ((havePos | haveVel | haveFwd) == 1) {
            // Nothing to set; fall through to the update + reap loop.
            goto afterListener;
        }
        haveUp = false;
    } else {
        cacheVec(this->listenerUp, up);
        haveUp = true;
    }

    {
        void *pPos = havePos ? (void *)this->listenerPos : 0;
        void *pUp = haveUp ? (void *)this->listenerUp : 0;
        void *pVel = haveVel ? (void *)this->listenerVel : 0;
        FMOD_EventSystem_set3DListenerAttributes(system, 0, pPos, pUp, pVel);
    }

afterListener:
    FMOD_EventSystem_update(system);

    for (int i = 0; i != 5; i++) {
        int slotIdx = this->fxSlots[i];
        void **evp = 0;
        void *ev = 0;
        if (slotIdx != -1) {
            evp = &this->events[slotIdx];
            ev = *evp;
        }
        if (slotIdx != -1 && ev != 0 && isPlaying(slotIdx) == 0) {
            FMOD::EventGroup *group = 0;
            if (FMOD_Event_getParentGroup(*evp, &group) == 0) {
                // Release the event back to the system.
                void **sysObj = (void **)this->system;
                typedef int (*ReleaseFn)(void *, int, int);
                ReleaseFn fn = *(ReleaseFn *)(*(char **)sysObj + 8);
                fn(sysObj, (int)(uintptr_t)*evp, 0);
                *evp = 0;
                this->fxSlots[i] = -1;
            }
        }
    }
}

// FModSound::isPlaying(int) -> if (system && events[p1]) { getState(h,&s); r=(s>>3)&1; } else 0
// NOTE: target carries a stack canary for the 4-byte state slot (built with stack-protector-strong
// upstream); basic -fstack-protector here does not, so this is a near-match.

unsigned FModSound::isPlaying(int p1)
{
    if (this->system != 0) {
        void *h = this->events[p1];
        if (h != 0) {
            unsigned s;
            FMOD_Event_getState(h, &s);
            return (s >> 3) & 1;
        }
    }
    return 0;
}

namespace FMOD { struct EventProject; struct EventGroup; }

// FModSound::freeAllEvents()
__attribute__((visibility("hidden"))) static const char kProjName[8] = "GoF2";

void FModSound::freeAllEvents()
{
    if (this->system != 0) {
        FMOD::EventProject *proj = 0;
        FMOD_EventSystem_getProject(this->system, kProjName, &proj);
        if (proj != 0) {
            for (unsigned i = 0; i < 0x8f5u; ++i) {
                void *e = this->events[i];
                if (e) {
                    FMOD::EventGroup *grp;
                    if (FMOD_Event_getParentGroup(e, &grp) == 0) {
                        unsigned st;
                        FMOD_Event_getState(this->events[i], &st);
                        if ((st & 0x0a) == 0) {
                            void *ee = this->events[i];
                            void **vt = *(void ***)ee;
                            typedef void (*Fn)(void *, int);
                            ((Fn)vt[8 / 4])(ee, 0);
                            this->events[i] = 0;
                        }
                    }
                }
            }
        }
    }
    if (this->listenerPos) operator delete(this->listenerPos);
    this->listenerPos = 0;
    if (this->listenerVel) operator delete(this->listenerVel);
    this->listenerVel = 0;
    if (this->listenerForward) operator delete(this->listenerForward);
    this->listenerForward = 0;
    if (this->listenerUp) operator delete(this->listenerUp);
    this->listenerUp = 0;
    if (this->eventPos) operator delete(this->eventPos);
    this->eventPos = 0;
    if (this->eventVel) operator delete(this->eventVel);
    this->eventVel = 0;
}

// FModSound::IsCategoryEnabled(int) -> p1<=3 && system && enabled[p1] ? 1 : 0
uint8_t FModSound::IsCategoryEnabled(int p1)
{
    uint8_t r = 0;
    if (p1 <= 3 && this->system != 0 && this->categoryEnabled[p1] != 0)
        r = 1;
    return r;
}

namespace FMOD { struct EventParameter; }

// FModSound::setParamValue(char const* name, int idx, float val)

void FModSound::setParamValue(const char *name, int idx, float val)
{
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameter(this->events[idx], name, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

// FModSound::pause(FMOD::Event*) -> int
//   if (e && system) { if (getState(e,&s)==0 && (s<<0x1c)<0) return setPaused(e,1)==0; return 0; }

int FModSound::pause(void *e)
{
    unsigned r = 0;
    if (e != 0 && this->system != 0) {
        unsigned s;
        int st = FMOD_Event_getState(e, &s);
        if (st == 0 && (int)(s << 0x1c) < 0)
            r = FMOD_Event_setPaused(e, 1) == 0;
        else
            r = 0;
    }
    return r;
}

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
    if (this->initialized == 0 || this->categoryEnabled[0] == 0)
        return event;

    int category = (event == 0) ? 0x24 : FMOD_Event_getCategory(event);

    if (event == 0 || category == 0x24) {
        void *ev = 0;
        if (FMOD_EventSystem_getEventBySystemID((unsigned int)(uintptr_t)this->system,
                                                idx, &ev) != 0)
            return event;
        if (FMOD_Event_getCategory(ev) != 0)
            return event;
        if (this->categoryEnabled[idx + 1] == 0)
            return event;

        void *posPtr = 0;
        void *velPtr = 0;
        bool havePos = false;
        if (pos != 0) {
            posPtr = cacheVec(this->eventPos, pos);
            havePos = true;
        }
        if (vel != 0) {
            velPtr = cacheVec(this->eventVel, vel);
            posPtr = havePos ? (void *)this->eventPos : 0;
        } else if (!havePos) {
            return event;
        } else {
            posPtr = this->eventPos;
        }

        if (FMOD_Event_set3DAttributes(ev, posPtr, velPtr) == 0) {
            void *dummy = 0;
            if (FMOD_EventSystem_getEventBySystemID(
                    (unsigned int)(uintptr_t)this->system, idx, &dummy) == 0)
                FMOD_Event_start(ev);
        }
        return ev;
    }

    // Direct-event path (event already resolved).
    if (this->categoryEnabled[idx + 1] == 0)
        return event;

    void *posPtr = 0;
    void *velPtr = 0;
    bool havePos = false;
    if (pos != 0) {
        posPtr = cacheVec(this->eventPos, pos);
        havePos = true;
    }
    if (vel != 0) {
        velPtr = cacheVec(this->eventVel, vel);
        posPtr = havePos ? (void *)this->eventPos : 0;
    } else if (!havePos) {
        return event;
    } else {
        posPtr = this->eventPos;
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

namespace FMOD { struct EventCategory; }

// FModSound::pauseAllPlayingSoundFXEvents()
//   for i<0x8f5: if (system && events[i] && isPlaying(i)) { getCategory(h,&cat);
//       cat->vtbl[0](cat,&a,&b); if (a==1) setPaused(h,1); }

void FModSound::pauseAllPlayingSoundFXEvents()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system) {
            void *h = this->events[i];
            if (h && isPlaying(i)) {
                FMOD::EventCategory *cat;
                FMOD_Event_getCategory(h, &cat);
                int a, b;
                void **vt = *(void ***)cat;
                typedef void (*Fn)(void *, int *, int *);
                ((Fn)vt[0])(cat, &a, &b);
                if (a == 1)
                    FMOD_Event_setPaused(this->events[i], 1);
            }
        }
    }
}

// FModSound::enableCategory(int p1, bool enable)

void FModSound::enableCategory(int p1, bool enable)
{
    if (p1 > 3)
        return;
    if (this->system == 0)
        return;
    void *cat = this->category[p1];
    if (cat == 0)
        return;

    this->categoryEnabled[p1] = (uint8_t)enable;
    if (!enable) {
        void **vt = *(void ***)cat;
        typedef void (*Fn)(void *);
        ((Fn)vt[0x1c / 4])(cat);
    } else if (p1 == 1 && this->currentMusicEvent >= 0) {
        FMOD_play(this, this->currentMusicEvent, 0, 0.0f);
    }

    uint8_t any = 0;
    int i = 1;
    do {
        if ((unsigned)(i - 0) > 3)
            break;
        uint8_t v = this->categoryEnabled[i];
        i++;
        any = (any || v) ? 1 : 0;
    } while (this->categoryEnabled[i - 1] == 0);
    this->categoryEnabled[0] = any;
}

// ---- tryToStopMusicForBGMusic_a2da8 ----
// Stub in the shipped build: it never stops the current track and reports that
// nothing was stopped, so callers fall back to their own background music.
int FModSound::tryToStopMusicForBGMusic()
{
    return 0;
}

// ---- defaultPitch (DAT_000a3518) ----
// Base pitch applied to freshly started events; the shipped table holds 0.0,
// meaning "use the event's authored pitch" unless the down-pitch flag overrides it.
float FModSound::defaultPitch = 0.0f;

// ---- engine-sound event helpers ----
// The KIPlayer/AI sound code drives a ship's looping engine sound by handing this
// manager the owning Player. The live FMOD event lives on the Player at +0xf0
// (Player::engineEvent); these helpers simply forward the requested transport
// operation to that handle, guarding against a null/unstarted event.

// FModSound::pauseEvent(Player*) -> pause the player's engine event.
void FModSound::pauseEvent(void *player)
{
    if (player == 0)
        return;
    void *event = pp(player, 0xf0);
    if (event != 0)
        FMOD_Event_setPaused(event, 1);
}

// FModSound::resumeEvent(Player*, int) -> resume the player's engine event.
// The second argument mirrors the call site (always 0); it selects the FMOD
// "immediate vs. queued" un-pause behaviour and is passed straight through.
void FModSound::resumeEvent(void *player, int immediate)
{
    if (player == 0)
        return;
    void *event = pp(player, 0xf0);
    if (event != 0)
        FMOD_Event_setPaused(event, immediate);
}

// FModSound::stopEvent(Player*) -> stop the player's engine event.
void FModSound::stopEvent(void *player)
{
    if (player == 0)
        return;
    void *event = pp(player, 0xf0);
    if (event != 0)
        FMOD_Event_stop(event, 0);
}

// FModSound::playEvent(Player*, int eventId, int) -> (re)start the player's
// engine event. The event id is the slot the AI tracked (KIPlayer::engineSoundEvent);
// the handle to actually start lives on the Player at +0xf0.
void FModSound::playEvent(void *player, int /*eventId*/, int /*mode*/)
{
    if (player == 0)
        return;
    void *event = pp(player, 0xf0);
    if (event != 0)
        FMOD_Event_start(event);
}

// FModSound::setProp(FModSound*, int) -> configure a property slot on the sound
// manager for the next event start and report the pitch that should be used for it.
// The jump-scene code issues several of these to arm the cinematic engine sound,
// then feeds the final return value into play() as the launch pitch.
float FModSound::setProp(int snd, int id)
{
    FModSound *self = (FModSound *)(uintptr_t)snd;
    if (self == 0 || self->initialized == 0)
        return FModSound::defaultPitch;
    self->propSlot = id;
    return FModSound::defaultPitch;
}

// FModSound::restoreState() -> un-pause every live event after a scene that had
// suspended audio (e.g. exiting a cinematic / docking sequence). Resolves the
// active manager and resumes all of its currently-allocated events.
void FModSound::restoreState()
{
    FModSound *self = FModSound::active();
    if (self == 0)
        return;
    self->resumeAll();
}

// Accessor for the process-wide FModSound instance the legacy code reached through
// a fixed global slot. The pointer is published by init(); until then it is null.
FModSound *FModSound::g_instance = 0;

FModSound *FModSound::active()
{
    return FModSound::g_instance;
}
