#include "engine/audio/FModSound.h"
#include "engine/core/GameText.h"

#include <cstring>

static inline int FMOD_Event_stop(FMOD::Event *event, int immediate) { return event->stop(immediate); }

static inline int FMOD_Event_setPaused(FMOD::Event *event, int paused) { return event->setPaused(paused); }

static inline void FMOD_EventSystem_unload(FMOD::EventSystem *system) { system->unload(); }

static inline void FMOD_EventSystem_release(FMOD::EventSystem *system) { system->release(); }

class AEFile { public: static const char *GetAppRootDir(); };

static inline int FMOD_Event_setPitch(FMOD::Event *event, float pitch, int mode) { return event->setPitch(pitch, (FMOD_EVENT_PITCHUNITS)mode); }

static inline int FMOD_Event_setVolume(FMOD::Event *event, float vol) { return event->setVolume(vol); }

static inline int FMOD_Event_getProperty(FMOD::Event *event, unsigned char *prop, unsigned char *out, int b = 1) { return event->getProperty((const char*)prop, (void*)out, b); }

static inline int FMOD_EventSystem_getNumReverbPresets(FMOD::EventSystem *system, int *out) { return system->getNumReverbPresets(out); }

static inline int FMOD_EventSystem_getReverbPresetByIndex(FMOD::EventSystem *system, int idx, unsigned char *props, char **name) { return system->getReverbPresetByIndex(idx, (FMOD_REVERB_PROPERTIES *)props, name); }

static inline int FMOD_EventSystem_setReverbProperties(FMOD::EventSystem *system, unsigned char *props) { return system->setReverbProperties((const FMOD_REVERB_PROPERTIES *)props); }

static inline int FMOD_Event_getParameterByIndex(FMOD::Event *event, int idx, FMOD::EventParameter **out) { return event->getParameterByIndex(idx, out); }

static inline int FMOD_EventParameter_setValue(FMOD::EventParameter *p, float v) { return p->setValue(v); }

static inline int FMOD_EventSystem_init(FMOD::EventSystem *system, int maxch, unsigned char *extdriver, int flags) { return system->init(maxch, (FMOD_INITFLAGS)(uintptr_t)extdriver, (void*)(uintptr_t)flags, (FMOD_EVENT_INITFLAGS)0); }

static inline int FMOD_EventSystem_load(FMOD::EventSystem *system, const char *name, FMOD::EventProject *proj) { return system->load(name, nullptr, proj ? &proj : nullptr); }

static inline int FMOD_EventSystem_getCategory(FMOD::EventSystem *system, const char *name, FMOD::EventCategory **out) { return system->getCategory(name, out); }

static inline int FMOD_EventSystem_getProjectByIndex(FMOD::EventSystem *system, int index, FMOD::EventProject **out) { return system->getProjectByIndex(index, out); }

static inline int FMOD_Event_getState(FMOD::Event *event, unsigned *out) { return event->getState(out); }

static inline int FMOD_Event_getParameter(FMOD::Event *event, const char *name, FMOD::EventParameter **out) { return event->getParameter(name, out); }

static inline int FMOD_EventSystem_getProject(FMOD::EventSystem *system, const char *name, FMOD::EventProject **out) { return system->getProject(name, out); }

static inline int FMOD_Event_getParentGroup(FMOD::Event *event, FMOD::EventGroup * *out) { return event->getParentGroup(out); }
static inline int FMOD_Event_getCategory(FMOD::Event *event, FMOD::EventCategory * *out = 0) { return event->getCategory(out); }

static inline void FMOD_play(FModSound *self, int a, unsigned char * /*b*/, float v) { self->play(a, nullptr, nullptr, v); }

static inline int FMOD_Event_getInfo(FMOD::Event *event, int *index, char **name, FMOD_EVENT_INFO *info) { return event->getInfo(index, name, info); }

static inline int FMOD_Event_set3DAttributes(FMOD::Event *event, Vector *pos, Vector *vel) { return event->set3DAttributes((const FMOD_VECTOR *)pos, (const FMOD_VECTOR *)vel, nullptr); }

static inline int FMOD_Event_start(FMOD::Event *event) { return event->start(); }

static inline int FMOD_EventSystem_update(int system) { return ((FMOD::EventSystem*)(uintptr_t)system)->update(); }

float VectorSignedToFloat(int v, int mode);

static inline void FMOD_MusicSystem_promptCue(FMOD::MusicSystem *music, int cueId) { music->promptCue(cueId); }

static inline void FMOD_MusicSystem_setParameterValue(FMOD::MusicSystem *music, int paramId, float v) { music->setParameterValue(paramId, v); }

static inline void FMOD_EventCategory_setVolume(FMOD::EventCategory *category, float volume) { category->setVolume(volume); }

static inline void FMOD_EventCategory_stopAllEvents(FMOD::EventCategory *category) { category->stopAllEvents(); }

static inline int FMOD_EventCategory_getInfo(FMOD::EventCategory *category, int *index, char **name) { return category->getInfo(index, name); }

char *property_name_pause = (char *) "pause";
char *property_name_purge = (char *) "purge";
static float FModSound_defaultPitch = 0.0f;

void FModSound::setAudioLanguage(int p1) {
    static const char *const kLangs[2] = {"english", "deutsch"};
    if (!this->system)
        return;
    this->system->setLanguage(kLangs[p1 == 1]);
}

void FModSound::updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c) {
    this->events[idx] = updateEvent3DAttributes(this->events[idx], idx, a, b, c);
}

void FModSound::stopAll() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_stop(this->events[i], 1);
    }
}

void FModSound::resumeAll() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_setPaused(this->events[i], 0);
    }
}

void FModSound::promptMusicCue(int p1) {
    if (!this->music)
        return;
    FMOD_MusicSystem_promptCue(this->music, p1);
}

static const float kFade = 0.1f;

void FModSound::fadeOutNow() {
    int s = this->currentMusicEvent;
    if (s == -1)
        return;
    if (this->fadeTargetMusicEvent == s)
        return;
    this->setParamValue(0, s, kFade);
}

void FModSound::release() {
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

FModSound::FModSound() {
    this->system = 0;
    this->music = 0;
    this->initialized = 0;
    this->appRootDir = (char *)AEFile::GetAppRootDir();
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

void FModSound::stop(FMOD::Event *e) {
    if (e == 0)
        return;
    e->stop(false);
}

void FModSound::setVolume(int p1, float vol) {
    if (this->system == 0)
        return;
    FMOD::EventCategory *h = this->category[p1];
    if (!h)
        return;
    FMOD_EventCategory_setVolume(h, vol);
}

void FModSound::pauseAll() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            FMOD_Event_setPaused(this->events[i], 1);
    }
}

FModSound::~FModSound() {
    release();
}

void FModSound::setDownPitch(bool down) {
    static const float kUp = 0.0f;
    float pitch = down ? -1.0f : kUp;
    this->downPitch = down ? 1 : 0;
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i] && isPlaying(i))
            FMOD_Event_setPitch(this->events[i], pitch, 1);
    }
}

void FModSound::stop(int p1) {
    if (p1 < 0)
        return;
    FMOD::Event *h = this->events[p1];
    if (!h)
        return;
    FMOD_Event_stop(h, 0);
}

void FModSound::setMusicParamValue(int p1, float p2) {
    if (!this->music)
        return;
    FMOD_MusicSystem_setParameterValue(this->music, p1, p2);
}

void FModSound::setSoundVolume(int p1, float vol) {
    if (this->system != 0) {
        FMOD::Event *h = this->events[p1];
        if (h)
            FMOD_Event_setVolume(h, vol);
    }
}

void FModSound::pauseAllPlaying() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system) {
            if (this->events[i] && isPlaying(i))
                FMOD_Event_setPaused(this->events[i], 1);
        }
    }
}

void FModSound::resume(int p1) {
    if (this->system != 0) {
        FMOD::Event *h = this->events[p1];
        if (h)
            FMOD_Event_setPaused(h, 0);
    }
}

int FModSound::getEventPauseLength(int idx) {
    if (this->initialized == 0 || this->system == nullptr || this->categoryEnabled[0] == 0)
        return 0;
    FMOD::Event *event = this->events[idx];
    if (event == nullptr)
        return 0;
    int length = 0;
    event->getProperty(property_name_pause, &length, true);
    return length;
}

void FModSound::enableReverb(int p1) {
    if (!this->system)
        return;
    int presetCount;
    FMOD_REVERB_PROPERTIES properties;
    if (FMOD_EventSystem_getNumReverbPresets(this->system, &presetCount) != 0)
        return;
    if (presetCount <= p1)
        return;
    if (this->reverbPreset == p1)
        return;
    this->reverbPreset = p1;
    if (FMOD_EventSystem_getReverbPresetByIndex(
            this->system, p1, reinterpret_cast<unsigned char *>(&properties), 0) == 0)
        FMOD_EventSystem_setReverbProperties(
            this->system, reinterpret_cast<unsigned char *>(&properties));
}

float FModSound::getPlayingProgress(int idx) {
    if (this->initialized != 0 && this->system != 0 &&
        this->categoryEnabled[0] != 0 &&
        this->events[idx] != 0) {
        char *name;
        int index;
        FMOD_EVENT_INFO info;
        FMOD_Event_getInfo(this->events[idx], &index, &name, &info);
        return (float) info.positionms / (float) info.lengthms;
    }
    return 0.0f;
}

void FModSound::play(int idx, Vector *pos, Vector *vel, float pitch) {
    if (this->initialized == 0)
        return;
    if (static_cast<unsigned int>(idx) >= 0x8f5 || this->system == nullptr)
        return;

    FMOD::Event *event = this->events[idx];
    bool loaded = true;
    if (event == nullptr) {
        this->system->getEventBySystemID(idx, FMOD_EVENT_INFOONLY, &event);
        loaded = false;
    }
    if (event == nullptr)
        return;

    event->setPitch(this->downPitch ? -1.0f : 0.0f, FMOD_EVENT_PITCHUNITS_OCTAVES);
    if (pitch != 0.0f)
        event->setPitch(pitch, FMOD_EVENT_PITCHUNITS_RAW);

    FMOD::EventCategory *category;
    if (event->getCategory(&category) != FMOD_OK) {
        if (this->events[idx] != nullptr) {
            FMOD::EventGroup *group;
            if (this->events[idx]->getParentGroup(&group) == FMOD_OK) {
                group->freeEventData(this->events[idx], false);
                this->events[idx] = nullptr;
            }
        }
        return;
    }

    char *categoryName;
    int categoryIndex;
    category->getInfo(&categoryIndex, &categoryName);
    ++categoryIndex; // slot 0 of categoryEnabled is the master category
    if (categoryIndex == 1)
        this->currentMusicEvent = idx;
    else if (categoryIndex > 3)
        return;
    if (this->categoryEnabled[categoryIndex] == 0)
        return;

    bool hasPos = false;
    if (pos != nullptr) {
        if (this->eventPos == nullptr)
            this->eventPos = new Vector();
        this->eventPos->x = pos->x;
        this->eventPos->y = pos->y;
        this->eventPos->z = pos->z;
        hasPos = true;
    }
    bool hasVel = false;
    if (vel != nullptr) {
        if (this->eventVel == nullptr)
            this->eventVel = new Vector();
        this->eventVel->x = vel->x;
        this->eventVel->y = vel->y;
        this->eventVel->z = vel->z;
        hasVel = true;
    }

    if (!loaded) {
        if (this->system->getEventBySystemID(idx, FMOD_EVENT_DEFAULT, &event) != FMOD_OK)
            return;
        this->events[idx] = event;
    }

    if (hasPos || hasVel) {
        this->events[idx]->set3DAttributes(
            reinterpret_cast<const FMOD_VECTOR *>(hasPos ? this->eventPos : nullptr),
            reinterpret_cast<const FMOD_VECTOR *>(hasVel ? this->eventVel : nullptr), nullptr);
    }

    int purge = 0;
    if (this->events[idx]->getProperty(property_name_purge, &purge, true) == FMOD_OK && purge == 1) {
        for (unsigned int i = 0; i <= 4; ++i) {
            if (this->fxSlots[i] == -1) {
                this->fxSlots[i] = idx;
                break;
            }
        }
    }
    this->events[idx]->start();
}

bool FModSound::resume(FMOD::Event *e) {
    FMOD::EventSystem *sys = this->system;
    bool ok = false;
    if (e != 0 && sys != 0)
        ok = FMOD_Event_setPaused(e, 0) == 0;
    return ok;
}

void FModSound::setParamValue(FMOD::Event *e, int paramIdx, float val) {
    if (e != 0 && this->system != 0) {
        FMOD::EventParameter *p[1];
        FMOD_Event_getParameterByIndex(e, paramIdx, p);
        FMOD_EventParameter_setValue(p[0], val);
    }
}

void FModSound::stopAllSoundFXEvents() {
    if (this->system == 0)
        return;
    for (int i = 1; i < 4; ++i) {
        if (i == 1) // music keeps playing
            continue;
        this->category[i]->stopAllEvents();
    }
}

int FModSound::pause(int p1) {
    uintptr_t self = (uintptr_t) this;
    if (this->system) {
        FMOD::Event *ev = this->events[p1];
        self = (uintptr_t) ev;
        if (ev)
            return FMOD_Event_setPaused(ev, 1);
    }
    return (int) (long) self;
}

void FModSound::disableReverb() {
    struct ReverbProps {
        int instance;
        int environment;
        float envDiffusion;
        int room;
        int roomHF;
        int roomLF;
        float decayTime;
        float decayHFRatio;
        float decayLFRatio;
        int reflections;
        float reflectionsDelay;
        int reverb;
        float reverbDelay;
        float modulationTime;
        float modulationDepth;
        float hfReference;
        float lfReference;
        float diffusion;
        float density;
        unsigned int flags;
    };
    static const ReverbProps kOff = {0,       -1,     1.0f, -10000, -10000, 0,    1.0f,
                                     1.0f,    1.0f,   -2602, 0.007f, 200,   0.011f,
                                     0.25f,   0.0f,   5000.0f, 250.0f, 0.0f, 0.0f, 0x33f};
    if (this->system) {
        unsigned char buf[0x50];
        memcpy(buf, &kOff, 0x50);
        FMOD_EventSystem_setReverbProperties(this->system, buf);
    }
    this->reverbPreset = -1;
}

void FModSound::init() {
    static const char kSuffixA[16] = "FMOD_GOF2.fev";
    static const char kSuffixB[32] = "data/audio/FMOD_GOF2.fev";
    static const char *kCats[4] = {"master", "music", "sfx", "voice"};

    for (int i = 0; i != 5; i++)
        this->fxSlots[i] = -1;
    FMOD_EventSystem_Create((FMOD_EVENTSYSTEM **) &this->system);
    FMOD_EventSystem_init(this->system, 0x20, (unsigned char *) 0x82, 0);
    setAudioLanguage(GameText::getLanguage());

    char path[0x400];
    memset(path, 0, 0x400);
    strcpy(path, this->appRootDir);
    uint8_t lowFlag = this->lowMemory;
    char *end = path + strlen(path);
    if (lowFlag == 0)
        memcpy(end, kSuffixB, 25);
    else
        memcpy(end, kSuffixA, 14);
    FMOD_EventSystem_load(this->system, path, 0);
    for (int i = 0; i != 4; i++) {
        this->category[i] = 0;
        FMOD_EventSystem_getCategory(this->system, kCats[i], &this->category[i]);
    }
    FMOD_EventSystem_getProjectByIndex(this->system, 0, (FMOD::EventProject **) &this->initialized);
    this->propSlot = 0;
    this->reverbPreset = -1;
    this->currentMusicEvent = -1;
    this->fadeTargetMusicEvent = -1;
    this->downPitch = 0;
}

void FModSound::setParamValue(int paramIdx, int idx, float val) {
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p[1];
        FMOD_Event_getParameterByIndex(this->events[idx], paramIdx, p);
        FMOD_EventParameter_setValue(p[0], val);
    }
}

unsigned FModSound::isChannelActive(int p1) {
    if (this->system != 0) {
        FMOD::Event *h = this->events[p1];
        if (h != 0) {
            unsigned s[1];
            FMOD_Event_getState(h, s);
            return (s[0] >> 4) & 1;
        }
    }
    return 0;
}

void FModSound::playMusicFadeOutCurrent(int p1) {
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

FMOD::EventParameter *FModSound::getParam(const char *name, int idx) {
    // The shipped code leaves `param` unset when the system or event is missing.
    FMOD::EventParameter *param;
    if (this->system != nullptr && this->events[idx] != nullptr)
        this->events[idx]->getParameter(name, &param);
    return param;
}

void FModSound::updateAll(Vector *pos, Vector *vel, Vector *forward, Vector *up) {
    if (this->system == nullptr)
        return;

    bool hasPos = false;
    if (pos != nullptr) {
        if (this->listenerPos == nullptr)
            this->listenerPos = new Vector();
        this->listenerPos->x = pos->x;
        this->listenerPos->y = pos->y;
        this->listenerPos->z = pos->z;
        hasPos = true;
    }
    bool hasVel = false;
    if (vel != nullptr) {
        if (this->listenerVel == nullptr)
            this->listenerVel = new Vector();
        this->listenerVel->x = vel->x;
        this->listenerVel->y = vel->y;
        this->listenerVel->z = vel->z;
        hasVel = true;
    }
    bool hasForward = false;
    if (forward != nullptr) {
        if (this->listenerForward == nullptr)
            this->listenerForward = new Vector();
        this->listenerForward->x = forward->x;
        this->listenerForward->y = forward->y;
        this->listenerForward->z = forward->z;
        hasForward = true;
    }
    bool hasUp = false;
    if (up != nullptr) {
        if (this->listenerUp == nullptr)
            this->listenerUp = new Vector();
        this->listenerUp->x = up->x;
        this->listenerUp->y = up->y;
        this->listenerUp->z = up->z;
        hasUp = true;
    }

    // The shipped build hands the vectors over as (pos, up, vel, forward).
    if (hasPos || hasVel || hasForward || hasUp) {
        this->system->set3DListenerAttributes(
                0, reinterpret_cast<const FMOD_VECTOR *>(hasPos ? this->listenerPos : nullptr),
                reinterpret_cast<const FMOD_VECTOR *>(hasUp ? this->listenerUp : nullptr),
                reinterpret_cast<const FMOD_VECTOR *>(hasVel ? this->listenerVel : nullptr),
                reinterpret_cast<const FMOD_VECTOR *>(hasForward ? this->listenerForward : nullptr));
    }
    this->system->update();

    for (int i = 0; i < 5; ++i) {
        int idx = this->fxSlots[i];
        if (idx != -1 && this->events[idx] != nullptr && !isPlaying(idx)) {
            FMOD::EventGroup *group;
            if (this->events[idx]->getParentGroup(&group) == FMOD_OK) {
                group->freeEventData(this->events[idx], false);
                this->events[idx] = nullptr;
                this->fxSlots[i] = -1;
            }
        }
    }
}

unsigned FModSound::isPlaying(int p1) {
    if (this->system != 0) {
        FMOD::Event *h = this->events[p1];
        if (h != 0) {
            unsigned s[1];
            FMOD_Event_getState(h, s);
            return (s[0] >> 3) & 1;
        }
    }
    return 0;
}

void FModSound::freeAllEvents() {
    if (this->system != 0) {
        FMOD::EventProject *proj[1];
        proj[0] = 0;
        FMOD_EventSystem_getProject(this->system, "FMOD_GOF2", proj);
        if (proj[0] == 0)
            return;
        for (unsigned i = 0; i < 0x8f5u; ++i) {
            FMOD::Event *e = this->events[i];
            if (e) {
                FMOD::EventGroup *grp[1];
                if (FMOD_Event_getParentGroup(e, grp) == 0) {
                    unsigned st[1];
                    FMOD_Event_getState(this->events[i], st);
                    if ((st[0] & 0x0a) == 0) {
                        grp[0]->freeEventData(this->events[i], false);
                        this->events[i] = 0;
                    }
                }
            }
        }
    }
    delete this->listenerPos;
    this->listenerPos = 0;
    delete this->listenerVel;
    this->listenerVel = 0;
    delete this->listenerForward;
    this->listenerForward = 0;
    delete this->listenerUp;
    this->listenerUp = 0;
    delete this->eventPos;
    this->eventPos = 0;
    delete this->eventVel;
    this->eventVel = 0;
}

bool FModSound::IsCategoryEnabled(int p1) {
    bool r = false;
    if (p1 <= 3 && this->system != 0)
        r = this->categoryEnabled[p1] != 0;
    return r;
}

void FModSound::setParamValue(const char *name, int idx, float val) {
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p[1];
        FMOD_Event_getParameter(this->events[idx], name, p);
        FMOD_EventParameter_setValue(p[0], val);
    }
}

int FModSound::pause(FMOD::Event *e) {
    unsigned r = 0;
    if (e != 0 && this->system != 0) {
        unsigned state;
        if (FMOD_Event_getState(e, &state) == 0 && (int) (state << 0x1c) < 0)
            r = FMOD_Event_setPaused(e, 1) == 0;
    }
    return r;
}

// Moves a 3D event; an event without a live handle is (re)acquired from the event system.
FMOD::Event *FModSound::updateEvent3DAttributes(FMOD::Event *event, int idx, Vector *pos, Vector *vel, bool restart) {
    if (this->initialized == 0 || this->categoryEnabled[0] == 0)
        return event;

    if (event != nullptr) {
        FMOD::EventCategory *category;
        FMOD_RESULT result = event->getCategory(&category);
        if (result != FMOD_ERR_INVALID_HANDLE) {
            if (result != FMOD_OK)
                return nullptr;
            char *categoryName;
            int categoryIndex;
            if (category->getInfo(&categoryIndex, &categoryName) != FMOD_OK)
                return event;
            ++categoryIndex;
            if (this->categoryEnabled[categoryIndex] == 0)
                return event;

        bool hasPos = false;
        if (pos != nullptr) {
            if (this->eventPos == nullptr)
                this->eventPos = new Vector();
            this->eventPos->x = pos->x;
            this->eventPos->y = pos->y;
            this->eventPos->z = pos->z;
            hasPos = true;
        }
        bool hasVel = false;
        if (vel != nullptr) {
            if (this->eventVel == nullptr)
                this->eventVel = new Vector();
            this->eventVel->x = vel->x;
            this->eventVel->y = vel->y;
            this->eventVel->z = vel->z;
            hasVel = true;
        }
        if (!hasPos && !hasVel)
            return nullptr;
            if (event->set3DAttributes(reinterpret_cast<const FMOD_VECTOR *>(hasPos ? this->eventPos : nullptr),
                                       reinterpret_cast<const FMOD_VECTOR *>(hasVel ? this->eventVel : nullptr),
                                       nullptr) != FMOD_OK)
                return nullptr;
            if (restart) {
                FMOD_EVENT_STATE state;
                event->getState(&state);
                if (state & FMOD_EVENT_STATE_PLAYING)
                    event->stop(true);
                event->start();
            }
            return event;
        }
    }

    FMOD::Event *found;
    if (this->system->getEventBySystemID(idx, FMOD_EVENT_INFOONLY, &found) != FMOD_OK)
        return nullptr;
    FMOD::EventCategory *category;
    if (found->getCategory(&category) != FMOD_OK)
        return nullptr;
    char *categoryName;
    int categoryIndex;
    if (category->getInfo(&categoryIndex, &categoryName) != FMOD_OK)
        return nullptr;
    ++categoryIndex;
    if (this->categoryEnabled[categoryIndex] == 0)
        return nullptr;

    bool hasPos = false;
    if (pos != nullptr) {
        if (this->eventPos == nullptr)
            this->eventPos = new Vector();
        this->eventPos->x = pos->x;
        this->eventPos->y = pos->y;
        this->eventPos->z = pos->z;
        hasPos = true;
    }
    bool hasVel = false;
    if (vel != nullptr) {
        if (this->eventVel == nullptr)
            this->eventVel = new Vector();
        this->eventVel->x = vel->x;
        this->eventVel->y = vel->y;
        this->eventVel->z = vel->z;
        hasVel = true;
    }
    if (!hasPos && !hasVel)
        return nullptr;
    if (found->set3DAttributes(reinterpret_cast<const FMOD_VECTOR *>(hasPos ? this->eventPos : nullptr),
                               reinterpret_cast<const FMOD_VECTOR *>(hasVel ? this->eventVel : nullptr),
                               nullptr) != FMOD_OK)
        return nullptr;
    if (this->system->getEventBySystemID(idx, FMOD_EVENT_DEFAULT, &found) != FMOD_OK)
        return nullptr;
    found->start();
    return found;
}

void FModSound::pauseAllPlayingSoundFXEvents() {
    for (int i = 0; i < 0x8f5; ++i) {
        if (this->system != nullptr && this->events[i] != nullptr && isPlaying(i)) {
            FMOD::EventCategory *cat;
            this->events[i]->getCategory(&cat);
            char *categoryName;
            int categoryIndex;
            cat->getInfo(&categoryIndex, &categoryName);
            if (categoryIndex == 1)
                this->events[i]->setPaused(true);
        }
    }
}

void FModSound::enableCategory(int index, bool enable) {
    if (index > 3 || this->system == nullptr)
        return;
    FMOD::EventCategory *cat = this->category[index];
    if (cat == nullptr)
        return;

    this->categoryEnabled[index] = enable;
    if (!enable)
        cat->stopAllEvents();
    else if (index == 1 && this->currentMusicEvent >= 0)
        play(this->currentMusicEvent, nullptr, nullptr, 0.0f);

    // Slot 0 mirrors "any of music/sfx/voice enabled".
    bool any = false;
    for (int i = 1; i < 4; ++i) {
        any |= this->categoryEnabled[i];
        if (this->categoryEnabled[i])
            break;
    }
    this->categoryEnabled[0] = any;
}

int FModSound::tryToStopMusicForBGMusic() {
    return 0;
}

bool FModSound::isHeadsetPluggedIn() {
    return false;
}

void FModSound::ERRCHECK(FMOD_RESULT /*result*/) {
}
