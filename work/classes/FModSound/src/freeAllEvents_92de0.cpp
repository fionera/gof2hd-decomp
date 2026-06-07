#include "class.h"

namespace FMOD { struct EventProject; struct EventGroup; }

// FModSound::freeAllEvents()
extern "C" int FMOD_EventSystem_getProject(void *system, const char *name, FMOD::EventProject **out);
extern "C" int FMOD_Event_getParentGroup(void *event, FMOD::EventGroup **out);
extern "C" int FMOD_Event_getState(void *event, unsigned *out);
__attribute__((visibility("hidden"))) static const char kProjName[8] = "GoF2";

struct FModSound { void freeAllEvents(); };
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
    if (pp(this, 0x2424)) operator delete(pp(this, 0x2424));
    pp(this, 0x2424) = 0;
    if (pp(this, 0x2428)) operator delete(pp(this, 0x2428));
    pp(this, 0x2428) = 0;
    if (pp(this, 0x242c)) operator delete(pp(this, 0x242c));
    pp(this, 0x242c) = 0;
    if (pp(this, 0x2430)) operator delete(pp(this, 0x2430));
    pp(this, 0x2430) = 0;
    if (pp(this, 0x2434)) operator delete(pp(this, 0x2434));
    pp(this, 0x2434) = 0;
    if (pp(this, 0x2438)) operator delete(pp(this, 0x2438));
    pp(this, 0x2438) = 0;
}
