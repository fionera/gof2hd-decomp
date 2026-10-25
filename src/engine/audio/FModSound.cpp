#include "engine/audio/FModSound.h"
#include "engine/core/GameText.h"

#include <cstring>

class AEFile { public: static const char *GetAppRootDir(); };

char *property_name_pause = (char *) "pause";
char *property_name_purge = (char *) "purge";





static const float kFade = 0.1f;








































FModSound::FModSound() {
    this->system = 0;
    this->music = 0;
    this->project = 0;
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

FModSound::~FModSound() {
    release();
}

void FModSound::release() {
    if (this->system != 0) {
        this->system->unload();
        this->system->release();
        this->system = 0;
    }
    for (unsigned i = 0; i < 0x8f5u; ++i)
        this->events[i] = 0;
    for (int i = 0; i != 4; i++)
        this->category[i] = 0;
}

void FModSound::init() {
    static const char kSuffixA[16] = "FMOD_GOF2.fev";
    static const char kSuffixB[32] = "data/audio/FMOD_GOF2.fev";
    static const char *kCats[4] = {"master", "music", "sfx", "voice"};

    for (int i = 0; i != 5; i++)
        this->fxSlots[i] = -1;
    FMOD::EventSystem_Create(&this->system);
    this->system->init(0x20, static_cast<FMOD_INITFLAGS>(0x82), nullptr, FMOD_EVENT_INIT_NORMAL);
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
    this->system->load(path, nullptr, nullptr);
    for (int i = 0; i != 4; i++) {
        this->category[i] = 0;
        this->system->getCategory(kCats[i], &this->category[i]);
    }
    this->system->getProjectByIndex(0, &this->project);
    this->propSlot = 0;
    this->reverbPreset = -1;
    this->currentMusicEvent = -1;
    this->fadeTargetMusicEvent = -1;
    this->downPitch = 0;
}

void FModSound::ERRCHECK(FMOD_RESULT /*result*/) {
}

void FModSound::setAudioLanguage(int p1) {
    static const char *const kLangs[2] = {"english", "deutsch"};
    if (!this->system)
        return;
    this->system->setLanguage(kLangs[p1 == 1]);
}

bool FModSound::isHeadsetPluggedIn() {
    return false;
}

int FModSound::tryToStopMusicForBGMusic() {
    return 0;
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

void FModSound::freeAllEvents() {
    if (this->system != 0) {
        FMOD::EventProject *proj[1];
        proj[0] = 0;
        this->system->getProject("FMOD_GOF2", proj);
        if (proj[0] == 0)
            return;
        for (unsigned i = 0; i < 0x8f5u; ++i) {
            FMOD::Event *e = this->events[i];
            if (e) {
                FMOD::EventGroup *grp[1];
                if (e->getParentGroup(grp) == 0) {
                    FMOD_EVENT_STATE state;
                    this->events[i]->getState(&state);
                    if ((state & 0x0a) == 0) {
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

FMOD::EventParameter *FModSound::getParam(const char *name, int idx) {
    // The shipped code leaves `param` unset when the system or event is missing.
    FMOD::EventParameter *param;
    if (this->system != nullptr && this->events[idx] != nullptr)
        this->events[idx]->getParameter(name, &param);
    return param;
}

void FModSound::setParamValue(int paramIdx, int idx, float val) {
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p[1];
        this->events[idx]->getParameterByIndex(paramIdx, p);
        p[0]->setValue(val);
    }
}

void FModSound::setParamValue(const char *name, int idx, float val) {
    if (idx >= 0 && this->system != 0 && this->events[idx] != 0) {
        FMOD::EventParameter *p[1];
        this->events[idx]->getParameter(name, p);
        p[0]->setValue(val);
    }
}

void FModSound::setMusicParamValue(int p1, float p2) {
    if (!this->music)
        return;
    this->music->setParameterValue(p1, p2);
}

void FModSound::setVolume(int p1, float vol) {
    if (this->system == 0)
        return;
    FMOD::EventCategory *h = this->category[p1];
    if (!h)
        return;
    h->setVolume(vol);
}

void FModSound::setSoundVolume(int p1, float vol) {
    if (this->system != 0) {
        FMOD::Event *h = this->events[p1];
        if (h)
            h->setVolume(vol);
    }
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
            FMOD_EVENT_STATE state;
            h->getState(&state);
            return (state >> 3) & 1;
        }
    }
    return 0;
}

unsigned FModSound::isChannelActive(int p1) {
    if (this->system != 0) {
        FMOD::Event *h = this->events[p1];
        if (h != 0) {
            FMOD_EVENT_STATE state;
            h->getState(&state);
            return (state >> 4) & 1;
        }
    }
    return 0;
}

bool FModSound::IsCategoryEnabled(int index) {
    FMOD::EventSystem *sys = this->system;
    bool enabled = false;
    if (index <= 3 && sys != nullptr)
        enabled = this->categoryEnabled[index];
    return enabled;
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

void FModSound::play(int idx, Vector *pos, Vector *vel, float pitch) {
    if (this->project == 0)
        return;
    if (static_cast<unsigned int>(idx) >= 0x8f5 && this->system != nullptr)
        return;
    if (this->system == nullptr)
        return;

    FMOD::Event *event = this->events[idx];
    bool loaded;
    if (this->events[idx] != nullptr) {
        loaded = true;
    } else {
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

void FModSound::fadeOutNow() {
    int s = this->currentMusicEvent;
    if (s == -1)
        return;
    if (this->fadeTargetMusicEvent == s)
        return;
    this->setParamValue(0, s, kFade);
}

void FModSound::stop(int p1) {
    if (p1 < 0)
        return;
    FMOD::Event *h = this->events[p1];
    if (!h)
        return;
    h->stop(false);
}

void FModSound::stop(FMOD::Event *e) {
    if (e == 0)
        return;
    e->stop(false);
}

void FModSound::updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c) {
    this->events[idx] = updateEvent3DAttributes(this->events[idx], idx, a, b, c);
}

// Moves a 3D event; an event without a live handle is (re)acquired from the event system.
FMOD::Event *FModSound::updateEvent3DAttributes(FMOD::Event *event, int idx, Vector *pos, Vector *vel, bool restart) {
    if (this->project == 0 || this->categoryEnabled[0] == 0)
        return event;

    FMOD::EventCategory *category;
    if (event != nullptr) {
        FMOD_RESULT result = event->getCategory(&category);
        if (result != FMOD_ERR_INVALID_HANDLE) {
            // Live handle: move it (and optionally restart it).
            if (result != FMOD_OK)
                return nullptr;
            char *categoryName;
            int categoryIndex;
            if (category->getInfo(&categoryIndex, &categoryName) == FMOD_OK) {
                ++categoryIndex;
                if (this->categoryEnabled[categoryIndex] != 0) {
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
                }
            }
            return event;
        }
    }

    FMOD::Event *found;
    if (this->system->getEventBySystemID(idx, FMOD_EVENT_INFOONLY, &found) == FMOD_OK) {
        if (found->getCategory(&category) == FMOD_OK) {
            char *categoryName;
            int categoryIndex;
            if (category->getInfo(&categoryIndex, &categoryName) == FMOD_OK) {
                ++categoryIndex;
                if (this->categoryEnabled[categoryIndex] != 0) {
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
                    if (this->system->getEventBySystemID(idx, FMOD_EVENT_DEFAULT, &found) == FMOD_OK) {
                        found->start();
                        return found;
                    }
                }
            }
        }
    }
    return nullptr;
}

int FModSound::pause(FMOD::Event *e) {
    FMOD::EventSystem *sys = this->system;
    bool paused = false;
    if (e != nullptr && sys != nullptr) {
        FMOD_EVENT_STATE state;
        if (e->getState(&state) == FMOD_OK && (state & FMOD_EVENT_STATE_PLAYING) != 0)
            paused = e->setPaused(true) == FMOD_OK;
    }
    return paused;
}

bool FModSound::resume(FMOD::Event *e) {
    FMOD::EventSystem *sys = this->system;
    bool ok = false;
    if (e != 0 && sys != 0)
        ok = e->setPaused(false) == 0;
    return ok;
}

void FModSound::setParamValue(FMOD::Event *e, int paramIdx, float val) {
    if (e != 0 && this->system != 0) {
        FMOD::EventParameter *p[1];
        e->getParameterByIndex(paramIdx, p);
        p[0]->setValue(val);
    }
}

void FModSound::enableReverb(int p1) {
    if (!this->system)
        return;
    int presetCount;
    FMOD_REVERB_PROPERTIES properties;
    if (this->system->getNumReverbPresets(&presetCount) != 0)
        return;
    if (presetCount <= p1)
        return;
    if (this->reverbPreset == p1)
        return;
    this->reverbPreset = p1;
    if (this->system->getReverbPresetByIndex(p1, &properties, nullptr) == 0)
        this->system->setReverbProperties(&properties);
}

void FModSound::disableReverb() {
    static const FMOD_REVERB_PROPERTIES kOff = FMOD_PRESET_OFF;
    if (this->system) {
        FMOD_REVERB_PROPERTIES properties;
        memcpy(&properties, &kOff, sizeof(properties));
        this->system->setReverbProperties(&properties);
    }
    this->reverbPreset = -1;
}

float FModSound::getPlayingProgress(int idx) {
    if (this->project != 0 && this->system != 0 &&
        this->categoryEnabled[0] != 0 &&
        this->events[idx] != 0) {
        char *name;
        int index;
        FMOD_EVENT_INFO info;
        this->events[idx]->getInfo(&index, &name, &info);
        return (float) info.positionms / (float) info.lengthms;
    }
    return 0.0f;
}

int FModSound::getEventPauseLength(int idx) {
    if (this->project == 0 || this->system == nullptr || this->categoryEnabled[0] == 0)
        return 0;
    FMOD::Event *event = this->events[idx];
    if (event == nullptr)
        return 0;
    int length = 0;
    event->getProperty(property_name_pause, &length, true);
    return length;
}

void FModSound::setDownPitch(bool down) {
    static const float kUp = 0.0f;
    float pitch = down ? -1.0f : kUp;
    this->downPitch = down ? 1 : 0;
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i] && isPlaying(i))
            this->events[i]->setPitch(pitch, FMOD_EVENT_PITCHUNITS_OCTAVES);
    }
}

void FModSound::promptMusicCue(int p1) {
    if (!this->music)
        return;
    this->music->promptCue(p1);
}

int FModSound::pause(int p1) {
    uintptr_t self = (uintptr_t) this;
    if (this->system) {
        FMOD::Event *ev = this->events[p1];
        self = (uintptr_t) ev;
        if (ev)
            return ev->setPaused(true);
    }
    return (int) (long) self;
}

void FModSound::resume(int p1) {
    if (this->system != 0) {
        FMOD::Event *h = this->events[p1];
        if (h)
            h->setPaused(false);
    }
}

void FModSound::pauseAll() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            this->events[i]->setPaused(true);
    }
}

void FModSound::stopAll() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            this->events[i]->stop(true);
    }
}

void FModSound::resumeAll() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system && this->events[i])
            this->events[i]->setPaused(false);
    }
}

void FModSound::pauseAllPlaying() {
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (this->system) {
            if (this->events[i] && isPlaying(i))
                this->events[i]->setPaused(true);
        }
    }
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

