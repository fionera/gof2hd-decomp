#include "class.h"

namespace FMOD { struct EventProject; struct EventGroup; }

// FModSound::freeAllEvents()
extern "C" int FMOD_EventSystem_getProject(void *system, const char *name, FMOD::EventProject **out);
extern "C" int FMOD_Event_getParentGroup(void *event, FMOD::EventGroup **out);
extern "C" int FMOD_Event_getState(void *event, unsigned *out);
__attribute__((visibility("hidden"))) static const char kProjName[8] = "GoF2";

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
 void freeAllEvents(); };
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
