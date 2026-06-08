#include "class.h"
typedef unsigned int uintptr_t;

namespace AbyssEngine { namespace AEMath { struct Vector; } }
using AbyssEngine::AEMath::Vector;

extern "C" {
void *operator_new(uint32_t size);
int FMOD_EventSystem_getEventBySystemID(unsigned int system, int id, void **out);
int FMOD_Event_setPitch(void *event, float pitch, int mode);
int FMOD_Event_getCategory(void *event);
int FMOD_Event_set3DAttributes(void *event, void *pos, void *vel);
int FMOD_Event_getProperty(void *event, void *name, void *value);
int FMOD_Event_start(void *event);
int FMOD_Event_getParentGroup(void *event, void **group);
}

__attribute__((visibility("hidden"))) extern void **g_fmodPropName; // property-name pointer
extern "C" extern float FModSound_defaultPitch;                     // DAT_000a3518

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

    void play(int idx, Vector *pos, Vector *vel, float pitch);
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
            void *group = 0;
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
