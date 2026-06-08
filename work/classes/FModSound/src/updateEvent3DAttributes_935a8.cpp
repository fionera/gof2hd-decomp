#include "class.h"
typedef unsigned int uintptr_t;

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
void *operator_new(uint32_t size);
int FMOD_Event_getCategory(void *event);
int FMOD_EventSystem_getEventBySystemID(unsigned int system, int id, void **out);
int FMOD_Event_set3DAttributes(void *event, void *pos, void *vel);
int FMOD_Event_start(void *event);
int FMOD_Event_getState(void *event, unsigned int *state);
int FMOD_Event_stop(void *event, bool immediate);
}

// FModSound::updateEvent3DAttributes(Event*, int, Vector*, Vector*, bool)
struct FModSound {
    void *updateEvent3DAttributes(void *event, int idx, Vector *pos, Vector *vel, bool restart);
};

// Copy a Vector (3 floats) into a cached FMOD_VECTOR slot, allocating if needed.
static void *cacheVec(void *self, uint32_t off, Vector *src)
{
    int *slot = (int *)pp(self, off);
    if (slot == 0) {
        slot = (int *)operator_new(0xc);
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
            posPtr = havePos ? pp(this, 0x2434) : 0;
        } else if (!havePos) {
            return event;
        } else {
            posPtr = pp(this, 0x2434);
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
        posPtr = havePos ? pp(this, 0x2434) : 0;
    } else if (!havePos) {
        return event;
    } else {
        posPtr = pp(this, 0x2434);
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
