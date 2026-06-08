#include "class.h"
typedef unsigned int uintptr_t;

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
void *operator_new(uint32_t size);
int FMOD_EventSystem_set3DListenerAttributes(int system, void *zero, void *pos, void *vel,
                                             void *forward);
int FMOD_EventSystem_update(int system);
int FMOD_Event_getParentGroup(void *event, void **group);
}

struct FModSound {
    int isPlaying(int idx);
    void updateAll(Vector *pos, Vector *vel, Vector *forward, Vector *up);
};

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
        void *pPos = havePos ? pp(this, 0x2424) : 0;
        void *pUp = haveUp ? pp(this, 0x2430) : 0;
        void *pVel = haveVel ? pp(this, 0x2428) : 0;
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
            void *group = 0;
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
