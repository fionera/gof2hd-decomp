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
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    uint8_t f_8; // 0x8
    unsigned char _pad_9[3];
    void* f_c; // 0xc
    uint8_t f_10; // 0x10
    unsigned char _pad_11[9199];
    int f_2400; // 0x2400
    int f_2404; // 0x2404
    int f_2408; // 0x2408
    int f_240c; // 0x240c
    unsigned char _pad_2410[20];
    void* f_2424; // 0x2424
    void* f_2428; // 0x2428
    void* f_242c; // 0x242c
    void* f_2430; // 0x2430
    void* f_2434; // 0x2434
    void* f_2438; // 0x2438

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
