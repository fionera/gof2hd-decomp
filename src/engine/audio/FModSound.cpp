#include "gof2/engine/audio/FModSound.h"
#include "gof2/platform/libc.h"
#include <cstring>

// FMOD designer-API shims. The native build links these against the FMOD runtime;
// they are declared here as the C entry points the engine expects.
extern "C" {
void  FMOD_setLanguage(void *system, uint32_t lang);
int   FMOD_Event_stop(void *event, int immediate);
int   FMOD_Event_setPaused(void *event, int paused);
void  FMOD_fade(void *self, int a, int s, float v);
void  FMOD_EventSystem_unload(void *system);
void  FMOD_EventSystem_release(void *system);
void *AEFile_GetAppRootDir();
FModSound *FMOD_Event_stop_p(void *event, int immediate);
int   FMOD_Event_setPitch(void *event, float pitch, int mode);
int   FMOD_Event_setVolume(void *event, float vol);
int   FMOD_Event_getProperty(void *event, void *prop, void *out, int b = 1);
int   FMOD_EventSystem_getNumReverbPresets(void *system, int *out);
int   FMOD_EventSystem_getReverbPresetByIndex(void *system, int idx, void *props, char **name);
int   FMOD_EventSystem_setReverbProperties(void *system, void *props);
int   FMOD_Event_getParameterByIndex(void *event, int idx, FMOD::EventParameter **out);
int   FMOD_EventParameter_setValue(FMOD::EventParameter *p, float v);
void  FMOD_EventSystem_Create(void *out);
int   FMOD_EventSystem_init(void *system, int maxch, void *extdriver, int flags);
int   FMOD_EventSystem_load(void *system, const char *name, void *proj);
void  FMOD_EventSystem_getCategory(void *system, void *out);
void  FMOD_EventSystem_getProjectByIndex(void *system, void *out);
int   FMOD_Event_getState(void *event, unsigned *out);
int   FMOD_Event_getParameter(void *event, const char *name, FMOD::EventParameter **out);
int   FMOD_EventSystem_getProject(void *system, const char *name, FMOD::EventProject **out);
int   FMOD_Event_getParentGroup(void *event, FMOD::EventGroup **out);
int   FMOD_Event_getCategory(void *event, FMOD::EventCategory **out = 0);
void  FMOD_play(void *self, int a, void *b, float v);
int   FMOD_Event_getInfo(void *event, char **name, void *info);
int   FMOD_EventSystem_getEventBySystemID(unsigned int system, int id, void **out);
int   FMOD_Event_set3DAttributes(void *event, void *pos, void *vel);
int   FMOD_Event_start(void *event);
int   FMOD_EventSystem_set3DListenerAttributes(int system, void *zero, void *pos, void *vel,
                                               void *forward);
int   FMOD_EventSystem_update(int system);
float VectorSignedToFloat(int v, int mode);
}

int GameText_getLanguage();

// Globals the engine publishes for the reverb/property property-name handles.
extern "C" void **gPauseProp;
extern void **g_fmodPropName;
extern "C" float FModSound_defaultPitch;

void FModSound::setAudioLanguage(int p1)
{
    static const uint32_t langTable[2] = {0, 1};
    if (!this->system)
        return;
    FMOD_setLanguage(this->system, langTable[p1 == 1]);
}

void FModSound::updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c)
{
    this->events[idx] = updateEvent3DAttributes(this->events[idx], idx, a, b, c);
}

void FModSound::stopAll()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_stop(this->events[i], 1);
    }
}

void FModSound::resumeAll()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_setPaused(this->events[i], 0);
    }
}

void FModSound::promptMusicCue(int p1)
{
    if (!this->music)
        return;
    void **vt = *(void ***)this->music;
    typedef void (*Fn)(void *, int);
    ((Fn)vt[0x28 / 4])(this->music, p1);
}

static const float kFade = 0.1f;

void FModSound::fadeOutNow()
{
    int s = this->currentMusicEvent;
    if (s == -1)
        return;
    if (this->fadeTargetMusicEvent == s)
        return;
    FMOD_fade(this, 0, s, kFade);
}

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

FModSound *FModSound::stop(void *e)
{
    if (e == 0)
        return this;
    return FMOD_Event_stop_p(e, 0);
}

void FModSound::setVolume(int p1, float vol)
{
    if (this->system == 0)
        return;
    void *h = this->category[p1];
    if (!h)
        return;
    void **vt = *(void ***)h;
    typedef void (*Fn)(void *, float);
    ((Fn)vt[0x20 / 4])(h, vol);
}

void FModSound::pauseAll()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_setPaused(this->events[i], 1);
    }
}

FModSound::~FModSound()
{
    release();
}

void FModSound::setDownPitch(bool down)
{
    static const float kUp = 0.1f;
    float pitch = down ? -1.0f : kUp;
    this->downPitch = down ? 1 : 0;
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i] && isPlaying(i))
            FMOD_Event_setPitch(this->events[i], pitch, 1);
    }
}

void FModSound::stop(int p1)
{
    if (p1 < 0)
        return;
    void *h = this->events[p1];
    if (!h)
        return;
    FMOD_Event_stop(h, 0);
}

void FModSound::setMusicParamValue(int p1, float p2)
{
    if (!this->music)
        return;
    void **vt = *(void ***)this->music;
    typedef void (*Fn)(void *, int, float);
    ((Fn)vt[0x34 / 4])(this->music, p1, p2);
}

void FModSound::setSoundVolume(int p1, float vol)
{
    if (this->system != 0) {
        void *h = this->events[p1];
        if (h)
            FMOD_Event_setVolume(h, vol);
    }
}

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

void FModSound::resume(int p1)
{
    if (this->system != 0) {
        void *h = this->events[p1];
        if (h)
            FMOD_Event_setPaused(h, 0);
    }
}

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

void FModSound::enableReverb(int p1)
{
    // One stack array holds both the FMOD_REVERB_PROPERTIES (buf[0..0x4f]) and the
    // preset count (buf+0x50).
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

// Queries the FMOD event info for a slot and converts two of its fields to float
// (progress numerator/denominator).
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

// Copy a Vector (3 floats) into a cached FMOD_VECTOR slot, allocating if needed.
static void *cacheVec(Vector *&cached, Vector *src)
{
    if (cached == 0)
        cached = new Vector();
    *cached = *src;
    return cached;
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

        bool havePos = false, haveVel = false;
        if (pos != 0) { cacheVec(this->eventPos, pos); havePos = true; }
        if (vel != 0) { cacheVec(this->eventVel, vel); haveVel = true; }

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

bool FModSound::resume(void *e)
{
    bool ok = false;
    if (e != 0 && this->system != 0)
        ok = FMOD_Event_setPaused(e, 0) == 0;
    return ok;
}

void FModSound::setParamValue(FMOD::Event *e, int paramIdx, float val)
{
    if (e != 0 && this->system != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(e, paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

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

void FModSound::disableReverb()
{
    static const unsigned char kRev[0x50] = {0};
    if (this->system) {
        unsigned char buf[0x50];
        memcpy(buf, kRev, 0x50);
        FMOD_EventSystem_setReverbProperties(this->system, buf);
    }
    this->reverbPreset = -1;
}

int FModSound::init()
{
    static const char kSuffixA[16] = ".fev";
    static const char kSuffixB[24] = "_low.fev";
    static void *kCats[4];

    for (int i = 0; i != 5; i++)
        this->fxSlots[i] = -1;
    FMOD_EventSystem_Create(&this->system);
    FMOD_EventSystem_init(this->system, 0x20, (void *)0x82, 0);
    setAudioLanguage(GameText_getLanguage());

    char path[0x400];
    memset(path, 0, 0x400);
    strcpy(path, this->appRootDir);
    uint8_t lowFlag = this->lowMemory;
    char *end = path + strlen(path);
    if (lowFlag == 0)
        memcpy(end, kSuffixB, 16);
    else
        memcpy(end, kSuffixA, 14);
    FMOD_EventSystem_load(this->system, path, 0);
    for (int i = 0; i != 4; i++) {
        this->category[i] = 0;
        FMOD_EventSystem_getCategory(this->system, kCats[i]);
    }
    FMOD_EventSystem_getProjectByIndex(this->system, 0);
    this->propSlot = 0;
    this->reverbPreset = -1;
    this->currentMusicEvent = -1;
    this->fadeTargetMusicEvent = -1;
    this->downPitch = 0;
    return 0;
}

void FModSound::setParamValue(int paramIdx, int idx, float val)
{
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(this->events[idx], paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

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
        if ((havePos | haveVel | haveFwd) == 1)
            goto afterListener;
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

void FModSound::freeAllEvents()
{
    static const char kProjName[8] = "GoF2";
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
    delete this->listenerPos;     this->listenerPos = 0;
    delete this->listenerVel;     this->listenerVel = 0;
    delete this->listenerForward; this->listenerForward = 0;
    delete this->listenerUp;      this->listenerUp = 0;
    delete this->eventPos;        this->eventPos = 0;
    delete this->eventVel;        this->eventVel = 0;
}

uint8_t FModSound::IsCategoryEnabled(int p1)
{
    uint8_t r = 0;
    if (p1 <= 3 && this->system != 0 && this->categoryEnabled[p1] != 0)
        r = 1;
    return r;
}

void FModSound::setParamValue(const char *name, int idx, float val)
{
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameter(this->events[idx], name, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}

int FModSound::pause(void *e)
{
    unsigned r = 0;
    if (e != 0 && this->system != 0) {
        unsigned s;
        int st = FMOD_Event_getState(e, &s);
        if (st == 0 && (int)(s << 0x1c) < 0)
            r = FMOD_Event_setPaused(e, 1) == 0;
    }
    return r;
}

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

void FModSound::enableCategory(int p1, bool enable)
{
    if (p1 > 3)
        return;
    if (this->system == 0)
        return;
    void *cat = this->category[p1];
    if (cat == 0)
        return;

    this->categoryEnabled[p1] = enable ? 1 : 0;
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
        if ((unsigned)i > 3)
            break;
        uint8_t v = this->categoryEnabled[i];
        i++;
        any = (any || v) ? 1 : 0;
    } while (this->categoryEnabled[i - 1] == 0);
    this->categoryEnabled[0] = any;
}

// Stub in the shipped build: it never stops the current track and reports that
// nothing was stopped, so callers fall back to their own background music.
int FModSound::tryToStopMusicForBGMusic()
{
    return 0;
}

// Base pitch applied to freshly started events; 0.0 means "use the event's
// authored pitch" unless the down-pitch flag overrides it.
float FModSound::defaultPitch = 0.0f;

// ---- engine-sound event helpers ----
// The KIPlayer/AI sound code drives a ship's looping engine sound by handing this
// manager the owning Player. The live FMOD event lives on the Player at the
// Player::engineEvent slot; these helpers forward the requested transport
// operation to that handle, guarding against a null/unstarted event.

// Player::engineEvent lives at a fixed offset on the opaque Player object.
static const unsigned kPlayerEngineEvent = 0xf0;

static void *playerEngineEvent(void *player)
{
    return *(void **)((char *)player + kPlayerEngineEvent);
}

void FModSound::pauseEvent(void *player)
{
    if (player == 0)
        return;
    void *event = playerEngineEvent(player);
    if (event != 0)
        FMOD_Event_setPaused(event, 1);
}

// The second argument mirrors the call site (always 0); it selects the FMOD
// "immediate vs. queued" un-pause behaviour and is passed straight through.
void FModSound::resumeEvent(void *player, int immediate)
{
    if (player == 0)
        return;
    void *event = playerEngineEvent(player);
    if (event != 0)
        FMOD_Event_setPaused(event, immediate);
}

void FModSound::stopEvent(void *player)
{
    if (player == 0)
        return;
    void *event = playerEngineEvent(player);
    if (event != 0)
        FMOD_Event_stop(event, 0);
}

void FModSound::playEvent(void *player, int /*eventId*/, int /*mode*/)
{
    if (player == 0)
        return;
    void *event = playerEngineEvent(player);
    if (event != 0)
        FMOD_Event_start(event);
}

// Configure a property slot on the sound manager for the next event start and
// report the pitch that should be used for it. The jump-scene code issues several
// of these to arm the cinematic engine sound, then feeds the final return value
// into play() as the launch pitch.
float FModSound::setProp(int snd, int id)
{
    FModSound *self = (FModSound *)(uintptr_t)snd;
    if (self == 0 || self->initialized == 0)
        return FModSound::defaultPitch;
    self->propSlot = id;
    return FModSound::defaultPitch;
}

// Un-pause every live event after a scene that had suspended audio (e.g. exiting
// a cinematic / docking sequence).
void FModSound::restoreState()
{
    FModSound *self = FModSound::active();
    if (self == 0)
        return;
    self->resumeAll();
}

// Process-wide FModSound instance the legacy code reached through a fixed global
// slot. Published by init(); until then it is null.
FModSound *FModSound::g_instance = 0;

FModSound *FModSound::active()
{
    return FModSound::g_instance;
}
