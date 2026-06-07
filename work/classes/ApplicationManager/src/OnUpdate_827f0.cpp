#include "class.h"

typedef int ModuleIntCallback(void *);
typedef void ModuleVoidCallback(void *);
typedef bool LoadingCallback(PaintCanvas *, int, void *);
typedef bool ResumeCallback(PaintCanvas *, void *);

extern "C" void Engine_PreUpdate(void *engine);
extern "C" void Engine_ClearBuffer(void *engine, int color);
extern "C" void Engine_ResetLightParam(void *engine);
extern "C" int Engine_IsPostEffectActivated(void *engine);
extern "C" void AESoundRessource_checkLooping(void *sound);
extern "C" void ApplicationManager_CheckForOrientationChange(ApplicationManager *self);
extern "C" void PaintCanvas_StartDraw2FBO(void *canvas);
extern "C" void PaintCanvas_StopDraw2FBO(void *canvas);
extern "C" void PaintCanvas_CheckNUseRefractFBO(void *canvas);
extern "C" void ext_001ab610(void);

extern "C" void ApplicationManager_OnUpdate(ApplicationManager *self, long long now)
{
    void * volatile cookie = __stack_chk_guard;
    void *engine = *(void **)((char *)self + 0xa8);
    Engine_PreUpdate(engine);

    if (*(bool *)((char *)self + 0x14)) {
        ApplicationManager_CheckForOrientationChange(self);
    }
    void *sound = *(void **)((char *)self + 0xac);
    if (sound != 0 && *(bool *)((char *)self + 0xb0)) {
        AESoundRessource_checkLooping(sound);
    }

    switch (*(int *)((char *)self + 0x3c)) {
    case 0: {
        void *next = *(void **)((char *)self + 0x60);
        void *module = next != 0 ? next : *(void **)((char *)self + 0x18);
        if (next != 0) {
            *(void **)((char *)self + 0x60) = 0;
            *(void **)((char *)self + 0x18) = next;
        }
        if (module != 0) {
            void **vtable = *(void ***)module;
            int loading = ((ModuleIntCallback *)vtable[2])(module);
            LoadingCallback *callback = *(LoadingCallback **)((char *)self + 0x20);
            if (callback != 0) {
                callback(*(PaintCanvas **)self, loading, *(void **)((char *)self + 0x24));
            }
            if (loading == 0) {
                *(uint64_t *)((char *)self + 0xa0) = 0;
                *(int *)((char *)self + 0x3c) = 5;
                *(uint64_t *)((char *)self + 0x68) = 0;
                *(uint64_t *)((char *)self + 0x70) = now;
                *(uint64_t *)((char *)self + 0x78) = now - 1;
                *(uint64_t *)((char *)self + 0x80) = 0;
            } else {
                uint64_t previous = *(uint64_t *)((char *)self + 0x70);
                *(uint64_t *)((char *)self + 0x68) += (uint64_t)now - previous;
                *(uint64_t *)((char *)self + 0x78) = previous;
                *(uint64_t *)((char *)self + 0x70) = now;
            }
        }
        break;
    }
    case 1: {
        void *module = *(void **)((char *)self + 0x18);
        if (module != 0) {
            void **vtable = *(void ***)module;
            ((ModuleVoidCallback *)vtable[3])(module);
            Engine_ResetLightParam(engine);
            *(int *)((char *)self + 0x3c) = 0;
            *(void **)((char *)self + 0x18) = 0;
        }
        break;
    }
    case 4:
        *(uint64_t *)((char *)self + 0xa0) = 0;
        *(int *)((char *)self + 0x3c) = *(int *)((char *)self + 0x40);
        *(uint64_t *)((char *)self + 0x68) += 1;
        *(uint64_t *)((char *)self + 0x70) = now;
        *(uint64_t *)((char *)self + 0x78) = now - 1;
        *(uint64_t *)((char *)self + 0x80) = 0;
        break;
    case 5: {
        void *module = *(void **)((char *)self + 0x18);
        if (module != 0) {
            void **vtable = *(void ***)module;
            ((ModuleVoidCallback *)vtable[0x30 / 4])(module);
            *(uint64_t *)((char *)engine + 0x68) = 0;
            *(uint64_t *)((char *)engine + 0x58) = 0;
            *(int *)((char *)*(void **)self + 4) = 0;
            ((ModuleVoidCallback *)vtable[0x34 / 4])(module);
            ResumeCallback *resume = *(ResumeCallback **)((char *)self + 0x28);
            if (resume == 0 || !resume(*(PaintCanvas **)self, *(void **)((char *)self + 0x2c))) {
                ((ModuleVoidCallback *)vtable[0x38 / 4])(module);
            }
        }
        break;
    }
    default:
        break;
    }

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
    ext_001ab610();
}
