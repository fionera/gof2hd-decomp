#include "gof2/ApplicationManager.h"
#include "gof2/Engine.h"


extern "C" void ext_001ab578(void *sound, int volume);
extern "C" void ext_001ab528(void *sound);
extern "C" int ext_001ab5a8(void *sound);
extern "C" void ext_001ab4f8(void *sound, int soundId);
extern "C" void ext_001ab488(void *sound, int soundId, float volume);
extern "C" void ext_001ab5e8(void *cheats, unsigned short key);
extern "C" void ext_001ab5d8(void *cheats);
extern "C" void ext_001ab548(void *sound);
extern "C" void Engine_Resume(void *engine);
extern "C" void ext_001ab518(void *sound);
extern "C" void ext_001ab508(void *sound, int soundId);
extern "C" void ext_001ab4e8(void *sound, int soundId);
extern "C" void ext_001ab4d8(void *sound, int soundId);
extern "C" void ext_001ab598(void *sound);
extern "C" void ext_001ab558(void *sound, int soundId);
extern "C" void ext_001ab4c8(void *sound);
extern "C" void Engine_Suspend(void *engine);
extern "C" void ext_001ab568(void *sound);
extern "C" void ext_001ab328(void *engine, unsigned short duration);
extern "C" void ext_001ab588(void *sound);
extern "C" void ext_001ab5c8(void *cheats);
extern "C" void ext_001ab538(void *sound);
extern "C" void ConfigReader_ParseFile(ConfigReader *reader, String *name);
extern "C" char g_touchDown;
extern "C" void ApplicationManager_ConvertTouchCoords(ApplicationManager *self, int *x, int *y);
extern "C" uint64_t g_perfElapsed;
extern "C" uint64_t g_perfActionCount;
extern "C" uint64_t g_perfLimitValue;
extern "C" uint64_t g_perfTotal;
extern "C" uint64_t g_perfFrame;
extern "C" char g_perfEnabled;
extern "C" char g_perfPendingFlag;
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
extern "C" void ArrayCtor_modules(void *array);
extern "C" void ArrayCtor_uint(void *array);
extern "C" void ArrayCtor_long_long(void *array);
extern "C" void *operator_new(unsigned int size);
extern "C" void PaintCanvas_ctor(void *canvas, void *engine);
extern "C" void AESoundRessource_ctor(void *sound);
extern "C" void ConfigReader_ctor(void *reader, void *engine);
extern "C" void CheatHandler_ctor(void *cheats, void *keys);
extern "C" void AESoundRessource_SetSound(void *sound, void *info, int count);
extern "C" void AESoundRessource_init(void *sound, int index);
extern "C" void IApplicationModule_SetApplicationManager(void *module, ApplicationManager *manager);
extern "C" void ArrayAdd_IApplicationModule(void *module, void *array);
extern "C" void ext_001ab5b8(unsigned int id, void *array);
extern "C" char g_perfPending;
extern "C" uint64_t g_perfCounter;
extern "C" uint64_t g_perfLimit;
extern "C" char g_perfExpired;
extern "C" int PaintCanvas_GetWidth(void *canvas);
extern "C" int PaintCanvas_GetHeight(void *canvas);
extern "C" void ext_001ab318(void *engine);
extern "C" int g_touchMode;
extern "C" char g_touchToggle;
extern "C" int g_touchValue;
extern "C" float g_touchFloat;
extern "C" void ArrayAdd_long_long(long long value, void *array);
extern "C" void ext_001ab5f8(long long key, void *array);
extern "C" void Array_StringPtr_ctor(void *array);
extern "C" void ArrayAdd_StringPtr(String *string, void *array);
extern "C" String *String_assign(String *self, String *other);
extern "C" int g_orientationLeft;
extern "C" int g_orientationFlat;
extern "C" int g_orientationRight;
extern "C" int g_orientationUpsideDown;
extern "C" int g_orientationInactive;
extern "C" void PaintCanvas_SetGameOrientation(void *canvas, int orientation);
extern "C" void ArrayRelease_modules(void *array);
extern "C" void ArrayRelease_uint(void *array);
extern "C" void ArrayDtor_long_long(void *array);
extern "C" void ArrayDtor_uint(void *array);
extern "C" void ArrayDtor_modules(void *array);
extern "C" void PaintCanvas_dtor(void *canvas);
extern "C" void AESoundRessource_dtor(void *sound);
extern "C" void CheatHandler_dtor(void *cheats);
extern "C" void ConfigReader_dtor(void *reader);
extern "C" void operator_delete(void *ptr);
extern "C" void operator_delete_array(void *ptr);

// ---- SetLoadingCallback_82506.cpp ----
typedef void LoadingCallback_t(PaintCanvas *, int, void *);

extern "C" void ApplicationManager_SetLoadingCallback(ApplicationManager *self, LoadingCallback_t *callback, void *data)
{
    self->field_0x20 = (void *)callback;
    self->field_0x24 = data;
}

// ---- SoundSetVolume_82270.cpp ----
extern "C" void ApplicationManager_SoundSetVolume(ApplicationManager *self, int volume)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab578(sound, volume);
    }
}

// ---- SoundStopSounds_821fc.cpp ----
extern "C" void ApplicationManager_SoundStopSounds(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab528(sound);
    }
}

// ---- SoundIsPlaying_8229a.cpp ----
extern "C" int ApplicationManager_SoundIsPlaying(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if (sound == 0) {
        return 0;
    }
    return ext_001ab5a8(sound);
}

// ---- GetSystemTimeMillis_83e9e.cpp ----
extern "C" uint64_t ApplicationManager_GetSystemTimeMillis(ApplicationManager *self)
{
    return self->field_0x68;
}

// ---- LoadingCallbackShow_8250c.cpp ----
typedef void LoadingShowCallback(PaintCanvas *, int, void *);

extern "C" void ApplicationManager_LoadingCallbackShow(ApplicationManager *self, int mode, void *data)
{
    LoadingShowCallback *callback = (LoadingShowCallback *)self->field_0x20;
    if (callback != 0) {
        callback(*(PaintCanvas **)self, mode, data);
    }
}

// ---- SoundPlay_821b2.cpp ----
extern "C" void ApplicationManager_SoundPlay(ApplicationManager *self, int soundId)
{
    void *sound = self->field_0xac;
    if (sound != 0 && self->field_0xb0) {
        ext_001ab4f8(sound, soundId);
    }
}

// ---- SoundPlay_821c6.cpp ----
extern "C" void ApplicationManager_SoundPlay_vol(ApplicationManager *self, int soundId, float volume)
{
    void *sound = self->field_0xac;
    if (sound == 0) {
        return;
    }
    if (self->field_0xb0 == 0) {
        return;
    }
    ext_001ab488(sound, soundId, volume);
}

// ---- CheatUpdate_8253a.cpp ----
extern "C" void ApplicationManager_CheatUpdate(ApplicationManager *self, unsigned short key)
{
    if (self->field_0x34) {
        void *cheats = self->field_0x30;
        if (cheats != 0) {
            ext_001ab5e8(cheats, key);
        }
    }
}

// ---- CheatAddCode_82528.cpp ----
extern "C" void ApplicationManager_CheatAddCode(ApplicationManager *self, void *code, int value)
{
    void *cheats = self->field_0x30;
    if (cheats != 0) {
        ext_001ab5d8(cheats);
    }
}

// ---- SoundPause_82218.cpp ----
extern "C" void ApplicationManager_SoundPause(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab548(sound);
    }
}

// ---- GetActionState_83e8a.cpp ----
extern "C" uint64_t ApplicationManager_GetActionState(ApplicationManager *self)
{
    return self->field_0xa0;
}

// ---- Resume_824d0.cpp ----
typedef void ModuleCallback(void *);


extern "C" void ApplicationManager_Resume(ApplicationManager *self)
{
    if (self->field_0x3c != 3) {
        return;
    }

    void *module = self->field_0x18;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x40 / 4](module);
        void *engine = self->field_0xa8;
        if (engine != 0) {
            Engine_Resume(engine);
        }
        self->field_0xa0 = 0;
        self->field_0xa4 = 0;
        self->field_0x80 = 0;
        self->field_0x84 = 0;
        self->field_0x3c = 4;
    }
}

// ---- SoundStop_821ee.cpp ----
extern "C" void ApplicationManager_SoundStop(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab518(sound);
    }
}

// ---- SoundPlayLoop_821da.cpp ----
extern "C" void ApplicationManager_SoundPlayLoop(ApplicationManager *self, int soundId)
{
    void *sound = self->field_0xac;
    if (sound != 0 && self->field_0xb0) {
        ext_001ab508(sound, soundId);
    }
}

// ---- SoundPlayMusicLoop_8219e.cpp ----
extern "C" void ApplicationManager_SoundPlayMusicLoop(ApplicationManager *self, int soundId)
{
    void *sound = self->field_0xac;
    if (sound != 0 && self->field_0xb1) {
        ext_001ab4e8(sound, soundId);
    }
}

// ---- ResetKeyState_83e90.cpp ----
extern "C" void ApplicationManager_ResetKeyState(ApplicationManager *self)
{
    self->field_0x80 = 0;
}

// ---- GetElapsedTimeMillis_827dc.cpp ----
extern "C" uint64_t ApplicationManager_GetElapsedTimeMillis(ApplicationManager *self)
{
    return self->field_0x70 - self->field_0x78;
}

// ---- SoundMusicEnable_8226a.cpp ----
extern "C" void ApplicationManager_SoundMusicEnable(ApplicationManager *self, bool enable)
{
    self->field_0xb1 = enable;
}

// ---- CheatEnable_82534.cpp ----
extern "C" void ApplicationManager_CheatEnable(ApplicationManager *self, bool enable)
{
    self->field_0x34 = enable;
}

// ---- GetCurrentTimeMillis_83e98.cpp ----
extern "C" uint64_t ApplicationManager_GetCurrentTimeMillis(ApplicationManager *self)
{
    return self->field_0x68;
}

// ---- SoundPlayMusic_8218a.cpp ----
extern "C" void ApplicationManager_SoundPlayMusic(ApplicationManager *self, int soundId)
{
    void *sound = self->field_0xac;
    if (sound != 0 && self->field_0xb1) {
        ext_001ab4d8(sound, soundId);
    }
}

// ---- OnTouchEnd_83c90.cpp ----
extern "C" void ApplicationManager_OnTouchEndSimple(ApplicationManager *self)
{
    self->field_0x8 = 0;
    self->field_0xc = 0;
    self->field_0x80 = 0;
    self->field_0x84 = 0;
    // Original wrote a 16-byte vector zero across +0x98..+0xa7 (action mask + action state).
    self->field_0x98 = 0;
    self->field_0x9c = 0;
    self->field_0xa0 = 0;
    self->field_0xa4 = 0;
}

// ---- SoundEnable_8225a.cpp ----
extern "C" void ApplicationManager_SoundEnable(ApplicationManager *self, bool enable)
{
    self->field_0xb0 = enable;
    self->field_0xb1 = enable;
}

// ---- SoundSetMusicVolume_8228c.cpp ----
extern "C" void ApplicationManager_SoundSetMusicVolume(ApplicationManager *self, int volume)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab598(sound);
    }
}

// ---- SetResumeCallback_82516.cpp ----
typedef bool ResumeCallback_t(PaintCanvas *, void *);

extern "C" void ApplicationManager_SetResumeCallback(ApplicationManager *self, ResumeCallback_t *callback, void *data)
{
    self->field_0x28 = (void *)callback;
    self->field_0x2c = data;
}

// ---- GetApplicationVersionString_82480.cpp ----
extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_GetApplicationVersionString(String *out)
{
    new (out) String();
    out->s = u"2.0.16";
}

// ---- VibrateEnable_822b6.cpp ----
extern "C" void ApplicationManager_VibrateEnable(ApplicationManager *self, bool enable)
{
    self->field_0xb2 = enable;
}

// ---- SoundResume_82226.cpp ----
extern "C" void ApplicationManager_SoundResume(ApplicationManager *self, int soundId)
{
    void *sound = self->field_0xac;
    if ((sound != 0 && self->field_0xb0) || self->field_0xb1) {
        ext_001ab558(sound, soundId);
    }
}

// ---- SoundRelease_8217c.cpp ----
extern "C" void ApplicationManager_SoundRelease(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab4c8(sound);
    }
}

// ---- Quit_82476.cpp ----
typedef void QuitCallback_t();

extern "C" void ApplicationManager_Quit(ApplicationManager *self)
{
    QuitCallback_t *callback = (QuitCallback_t *)self->field_0x1c;
    if (callback != 0) {
        callback();
    }
}

// ---- GetEngine_8249c.cpp ----
extern "C" void *ApplicationManager_GetEngine(ApplicationManager *self)
{
    return self->field_0xa8;
}

// ---- Suspend_824a2.cpp ----
typedef void ModuleCallback(void *);


extern "C" void ApplicationManager_Suspend(ApplicationManager *self)
{
    unsigned int state = self->field_0x3c;
    if (state - 3 < 2) {
        return;
    }

    void *module = self->field_0x18;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x3c / 4](module);
        void *engine = self->field_0xa8;
        if (engine != 0) {
            Engine_Suspend(engine);
        }
        int oldState = self->field_0x3c;
        self->field_0x3c = 3;
        self->field_0x40 = oldState;
    }
}

// ---- SoundFxEnable_82264.cpp ----
extern "C" void ApplicationManager_SoundFxEnable(ApplicationManager *self, bool enable)
{
    self->field_0xb0 = enable;
}

// ---- SoundResume_822a8.cpp ----
extern "C" void ApplicationManager_SoundResumeSelf(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab568(sound);
    }
}

// ---- Vibrate_822c4.cpp ----
extern "C" void ApplicationManager_Vibrate(ApplicationManager *self, unsigned short duration)
{
    if (self->field_0xb2) {
        ext_001ab328(self->field_0xa8, duration);
    }
}

// ---- SoundSetFXVolume_8227e.cpp ----
extern "C" void ApplicationManager_SoundSetFXVolume(ApplicationManager *self, int volume)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab588(sound);
    }
}

// ---- CheatSetCallback_8251c.cpp ----
extern "C" void ApplicationManager_CheatSetCallback(ApplicationManager *self, void *callback, void *data)
{
    void *cheats = self->field_0x30;
    if (cheats != 0) {
        ext_001ab5c8(cheats);
    }
}

// ---- SoundResumeSounds_82240.cpp ----
extern "C" void ApplicationManager_SoundResumeSounds(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if ((sound != 0 && self->field_0xb0) || self->field_0xb1) {
        ext_001ab568(sound);
    }
}

// ---- SoundPauseSounds_8220a.cpp ----
extern "C" void ApplicationManager_SoundPauseSounds(ApplicationManager *self)
{
    void *sound = self->field_0xac;
    if (sound != 0) {
        ext_001ab538(sound);
    }
}

// ---- GetKeyState_83e84.cpp ----
extern "C" uint64_t ApplicationManager_GetKeyState(ApplicationManager *self)
{
    return self->field_0x80;
}

// ---- SetApplicationModule_82440.cpp ----
extern "C" void ApplicationManager_SetApplicationModule(ApplicationManager *self, void *module)
{
    void *current = self->field_0x18;
    self->field_0x60 = module;
    self->field_0x3c = current != 0;
}

// ---- ConfigReadFile_8254c.cpp ----
extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_ConfigReadFile(ApplicationManager *self, String *name)
{
    void * volatile cookie = __stack_chk_guard;
    unsigned char storage[sizeof(String)] __attribute__((aligned(4)));
    ConfigReader *reader = (ConfigReader *)self->field_0x38;
    if (reader != 0) {
        String *copy = (String *)storage;
        new (copy) String(*name);
        ConfigReader_ParseFile(reader, copy);
        ((String *)storage)->~String();
    }
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}

// ---- OnTouchEnd_83c18.cpp ----
typedef void TouchEndCallback(void *, int, int, void *);
typedef void TouchReleaseCallback(void *, int, int);


extern "C" void ApplicationManager_OnTouchEnd(ApplicationManager *self, int xArg, int yArg, void *touch)
{
    g_touchDown = 0;
    void * volatile cookie = __stack_chk_guard;
    int x = xArg;
    int y = yArg;

    void *module = self->field_0x18;
    if (module != 0 && self->field_0x3c == 5) {
        ApplicationManager_ConvertTouchCoords(self, &x, &y);
        module = self->field_0x18;
        void **vtable = *(void ***)module;
        ((TouchEndCallback *)vtable[0x2c / 4])(module, x, y, touch);
        module = self->field_0x18;
        vtable = *(void ***)module;
        ((TouchReleaseCallback *)vtable[0x20 / 4])(module, x, y);
        self->field_0xb4 = x;
        self->field_0xb8 = y;
    }

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}

// ---- EnablePerformanceTest_82334.cpp ----
extern "C" void ApplicationManager_EnablePerformanceTest(int count)
{
    g_perfElapsed = 0;
    g_perfActionCount = (long long)count;
    g_perfLimitValue = 0;
    g_perfTotal = 0;
    g_perfFrame = 0;
    g_perfEnabled = 1;
    g_perfPendingFlag = 0;
}

// ---- OnUpdate_827f0.cpp ----
typedef int ModuleIntCallback(void *);
typedef void ModuleVoidCallback(void *);
typedef bool LoadingCallbackU(PaintCanvas *, int, void *);
typedef bool ResumeCallbackU(PaintCanvas *, void *);


extern "C" void ApplicationManager_OnUpdate(ApplicationManager *self, long long now)
{
    void * volatile cookie = __stack_chk_guard;
    void *engine = self->field_0xa8;
    Engine_PreUpdate(engine);

    if (self->field_0x14) {
        ApplicationManager_CheckForOrientationChange(self);
    }
    void *sound = self->field_0xac;
    if (sound != 0 && self->field_0xb0) {
        AESoundRessource_checkLooping(sound);
    }

    switch (self->field_0x3c) {
    case 0: {
        void *next = self->field_0x60;
        void *module = next != 0 ? next : self->field_0x18;
        if (next != 0) {
            self->field_0x60 = 0;
            self->field_0x18 = next;
        }
        if (module != 0) {
            void **vtable = *(void ***)module;
            int loading = ((ModuleIntCallback *)vtable[2])(module);
            LoadingCallbackU *callback = (LoadingCallbackU *)self->field_0x20;
            if (callback != 0) {
                callback(*(PaintCanvas **)self, loading, self->field_0x24);
            }
            if (loading == 0) {
                self->field_0xa0 = 0;
                self->field_0x3c = 5;
                self->field_0x68 = 0;
                self->field_0x70 = now;
                self->field_0x78 = now - 1;
                self->field_0x80 = 0;
            } else {
                uint64_t previous = self->field_0x70;
                self->field_0x68 += (uint64_t)now - previous;
                self->field_0x78 = previous;
                self->field_0x70 = now;
            }
        }
        break;
    }
    case 1: {
        void *module = self->field_0x18;
        if (module != 0) {
            void **vtable = *(void ***)module;
            ((ModuleVoidCallback *)vtable[3])(module);
            Engine_ResetLightParam(engine);
            self->field_0x3c = 0;
            self->field_0x18 = 0;
        }
        break;
    }
    case 4:
        self->field_0xa0 = 0;
        self->field_0x3c = self->field_0x40;
        self->field_0x68 += 1;
        self->field_0x70 = now;
        self->field_0x78 = now - 1;
        self->field_0x80 = 0;
        break;
    case 5: {
        void *module = self->field_0x18;
        if (module != 0) {
            void **vtable = *(void ***)module;
            ((ModuleVoidCallback *)vtable[0x30 / 4])(module);
            *(uint64_t *)((char *)engine + 0x68) = 0;
            *(uint64_t *)((char *)engine + 0x58) = 0;
            *(int *)((char *)*(void **)self + 4) = 0;
            ((ModuleVoidCallback *)vtable[0x34 / 4])(module);
            ResumeCallbackU *resume = (ResumeCallbackU *)self->field_0x28;
            if (resume == 0 || !resume(*(PaintCanvas **)self, self->field_0x2c)) {
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

// ---- ApplicationManager_8193c.cpp ----
extern "C" ApplicationManager *ApplicationManager_ctor(ApplicationManager *self, void *engine)
{
    void * volatile cookie = __stack_chk_guard;

    ArrayCtor_modules((char *)self + 0x44);
    ArrayCtor_uint((char *)self + 0x50);
    ArrayCtor_long_long((char *)self + 0x88);

    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x8 = 0;
    self->field_0x3c = 5;
    self->field_0x18 = 0;
    self->field_0x80 = 0;
    self->field_0xa8 = engine;

    void *canvas = operator_new(0x20c);
    PaintCanvas_ctor(canvas, engine);
    *(void **)self = canvas;

    void *sound = operator_new(0x14);
    AESoundRessource_ctor(sound);
    self->field_0x34 = false;
    self->field_0xac = sound;

    void *reader = operator_new(0x14);
    ConfigReader_ctor(reader, engine);
    self->field_0x20 = 0;
    self->field_0x38 = reader;
    self->field_0x28 = 0;
    self->field_0xb0 = 0x101;
    self->field_0xb2 = true;
    self->field_0x14 = false;

    char *storage = (char *)operator_new(0x408);
    *(uint32_t *)storage = 0x10;
    *(uint32_t *)(storage + 4) = 0x40;
    char *keys = storage + 8;
    for (int offset = 0; offset != 0x400; offset += 0x10) {
        new ((String *)(keys + offset + 4)) String();
        *(uint32_t *)(keys + offset) = 0;
    }
    self->field_0x10 = keys;

    void *cheats = operator_new(0x10);
    CheatHandler_ctor(cheats, keys);
    self->field_0x30 = cheats;
    self->field_0xb4 = -1;
    self->field_0xb8 = -1;

    if (cookie == __stack_chk_guard) {
        return self;
    }
    __stack_chk_fail();
}

// ---- OnTouchMove_83bb0.cpp ----
typedef void TouchMoveCallback(void *, int, int, void *);
typedef void TouchDragCallback(void *, int, int);


extern "C" void ApplicationManager_OnTouchMove(ApplicationManager *self, int xArg, int yArg, void *touch)
{
    void * volatile cookie = __stack_chk_guard;
    int x = xArg;
    int y = yArg;

    void *module = self->field_0x18;
    if (module != 0 && self->field_0x3c == 5) {
        ApplicationManager_ConvertTouchCoords(self, &x, &y);
        module = self->field_0x18;
        void **vtable = *(void ***)module;
        ((TouchMoveCallback *)vtable[0x28 / 4])(module, x, y, touch);
        module = self->field_0x18;
        vtable = *(void ***)module;
        ((TouchDragCallback *)vtable[0x1c / 4])(module, x, y);
        self->field_0xb4 = x;
        self->field_0xb8 = y;
    }

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}

// ---- OnKeyPress_83ca6.cpp ----
typedef void KeyCallback(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int);

extern "C" void ApplicationManager_OnKeyPress(ApplicationManager *self, int key)
{
    self->field_0x98 = 0;
    self->field_0x8 = key;
    self->field_0xc = key >> 31;
    self->field_0x9c = 0;

    unsigned int keyLow = 0;
    unsigned int keyHigh = 0;
    unsigned int actionLow = 0;
    unsigned int actionHigh = 0;
    unsigned int keyIndex = 0;
    int *mapping = (int *)self->field_0x10;
    while (keyIndex <= 0x3f) {
        if (*mapping == key) {
            int highIndex = (int)keyIndex - 0x20;
            keyLow = 1u << keyIndex;
            if (highIndex >= 0) {
                keyLow = 0;
            }
            keyHigh = 1u >> (0x20 - keyIndex);
            if (highIndex >= 0) {
                keyHigh = 1u << highIndex;
            }
            self->field_0x80 =
                self->field_0x80 | (((uint64_t)keyHigh << 32) | keyLow);

            unsigned int offset = 0;
            for (unsigned int i = 0; i < self->field_0x88; i += 2) {
                char *entry = self->field_0x8c + offset;
                if (*(unsigned int *)(entry + 8) == keyIndex && *(int *)(entry + 0x0c) == 0) {
                    actionLow |= *(uint32_t *)entry;
                    actionHigh |= *(uint32_t *)(entry + 4);
                    self->field_0x98 = actionLow;
                    self->field_0x9c = actionHigh;
                    self->field_0xa0 |= actionLow;
                    self->field_0xa4 |= actionHigh;
                }
                offset += 0x10;
            }
            break;
        }
        mapping += 4;
        ++keyIndex;
    }

    void *module = self->field_0x18;
    if (module != 0 && self->field_0x3c == 5) {
        void **vtable = *(void ***)module;
        ((KeyCallback *)vtable[0x10 / 4])(module, module, keyLow, keyHigh, actionLow, actionHigh);
    }
}

// ---- SoundSet_82148.cpp ----
__attribute__((minsize)) extern "C" void ApplicationManager_SoundSet(ApplicationManager *self, void *info, int count)
{
    if (info != 0) {
        void *sound = self->field_0xac;
        if (sound != 0) {
            AESoundRessource_SetSound(sound, info, count);
            for (int i = 0; i < count; ++i) {
                AESoundRessource_init(self->field_0xac, i);
            }
        }
    }
}

// ---- RegisterApplicationModule_822d4.cpp ----
__attribute__((minsize)) extern "C" void ApplicationManager_RegisterApplicationModule(ApplicationManager *self, unsigned int id, void *module)
{
    if (module != 0) {
        IApplicationModule_SetApplicationManager(module, self);
        ArrayAdd_IApplicationModule(module, (char *)self + 0x44);
        ext_001ab5b8(id, (char *)self + 0x50);
    }
}

// ---- SetCurrentApplicationModule_823b0.cpp ----
extern "C" void ApplicationManager_SetCurrentApplicationModule(ApplicationManager *self, unsigned int id)
{
    char *pending = &g_perfPending;
    if (*pending != 0) {
        uint64_t *counter = &g_perfCounter;
        uint64_t limit = g_perfLimit;
        uint64_t value = *counter + 1;
        *counter = value;
        if ((long long)(value - limit) >= 0) {
            *(volatile char *)&g_perfExpired = 1;
            *(volatile char *)pending = 0;
        }
    }

    unsigned int count = self->field_0x50;
    unsigned int index = 0;
    while (index < count) {
        if (*(unsigned int *)(self->field_0x54 + index * 4) == id) {
            void *module = *(void **)(self->field_0x48 + index * 4);
            void *current = self->field_0x18;
            self->field_0x3c = current != 0;
            self->field_0x5c = id;
            self->field_0x60 = module;
            return;
        }
        ++index;
    }
}

// ---- ConvertTouchCoords_839de.cpp ----
extern "C" void ApplicationManager_ConvertTouchCoords(ApplicationManager *self, int *x, int *y)
{
    ApplicationManager *manager = self;
    int *xPtr = x;
    int *yPtr = y;
    void *canvas = *(void **)manager;
    int orientation = *(int *)((char *)canvas + 0x30);
    int newY;

    if (orientation != 3) {
        if (orientation != 1) {
            if (orientation != 0) {
                return;
            }
            newY = *xPtr;
            *xPtr = PaintCanvas_GetWidth(canvas) - *yPtr;
            goto storeY;
        }
        int oldX = *xPtr;
        *xPtr = *yPtr;
        *yPtr = PaintCanvas_GetHeight(canvas) - oldX;
        canvas = *(void **)manager;
    }
    *xPtr = PaintCanvas_GetWidth(canvas) - *xPtr;
    newY = PaintCanvas_GetHeight(canvas) - *yPtr;

storeY:
    *yPtr = newY;
}

// ---- VibrateSupported_822bc.cpp ----
extern "C" void ApplicationManager_VibrateSupported(ApplicationManager *self)
{
    ext_001ab318(self->field_0xa8);
}

// ---- OnTouchBegin_83a38.cpp ----
typedef void TouchBeginCallback(void *, int, int, void *);
typedef void TouchPressCallback(void *, int, int);


extern "C" void ApplicationManager_OnTouchBegin(ApplicationManager *self, int xArg, int yArg, void *touch)
{
    void * volatile cookie = __stack_chk_guard;
    int x = xArg;
    int y = yArg;

    void *module = self->field_0x18;
    if (module != 0 && self->field_0x3c == 5) {
        ApplicationManager_ConvertTouchCoords(self, &x, &y);
        module = self->field_0x18;
        void **vtable = *(void ***)module;
        ((TouchBeginCallback *)vtable[0x24 / 4])(module, x, y, touch);
        module = self->field_0x18;
        vtable = *(void ***)module;
        ((TouchPressCallback *)vtable[0x18 / 4])(module, x, y);
        self->field_0xb4 = x;
        self->field_0xb8 = y;

        void *engine = self->field_0xa8;
        int mode = g_touchMode;
        if (mode <= 3) {
            void *canvas = *(void **)self;
            if (mode == 0 && x <= 0x31 && y <= 0x31) {
                g_touchMode = 1;
            } else if (mode == 1 && x > PaintCanvas_GetWidth(canvas) - 0x32 &&
                       y > PaintCanvas_GetHeight(canvas) - 0x32) {
                g_touchMode = 2;
            } else if (mode == 2 && x <= 0x31 && y > PaintCanvas_GetHeight(canvas) - 0x32) {
                uint8_t *flag = (uint8_t *)((char *)engine + 0x74);
                *flag = !*flag;
            } else if (mode == 3 && y <= 0x31 && x > PaintCanvas_GetWidth(canvas) - 0x32) {
                g_touchMode = 4;
            }
        } else if (*(uint8_t *)((char *)engine + 0x74)) {
            if (y < 100) {
                g_touchToggle ^= 1;
            } else {
                int height = PaintCanvas_GetHeight(*(void **)self);
                int width = PaintCanvas_GetWidth(*(void **)self);
                int half = width / 2;
                if (height - 100 < y) {
                    g_touchValue = x < half ? 0 : 1;
                } else {
                    g_touchFloat += x < half ? -1.0f : 1.0f;
                }
            }
        }
    }

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}

// ---- ConfigRegisterAction_82600.cpp ----
extern "C" void ApplicationManager_ConfigRegisterAction(ApplicationManager *self, long long value, long long key)
{
    void *array = (char *)self + 0x88;
    ArrayAdd_long_long(value, array);
    ext_001ab5f8(key, array);
}

// ---- GetApplicationModule_82452.cpp ----
extern "C" void *ApplicationManager_GetApplicationModule(ApplicationManager *self, unsigned int id)
{
    unsigned int count = self->field_0x50;
    unsigned int index = 0;
    goto check;

advance:
    ++index;
check:
    if (index >= count) {
        return 0;
    }
    if (*(int *)(self->field_0x54 + index * 4) != (int)id) {
        goto advance;
    }
    return *(void **)(self->field_0x48 + index * 4);
}

// ---- ConfigGetKeysForAction_82654.cpp ----
extern "C" void *ApplicationManager_ConfigGetKeysForAction(ApplicationManager *self, long long action)
{
    int low = (int)action;
    int high = (int)(action >> 32);
    unsigned int byteOffset = 0;
    void *result = 0;
    unsigned int index = 0;
    goto check;

loop:
    {
        char *actions = self->field_0x8c;
        int actionLow = *(int *)(actions + byteOffset);
        int actionHigh = *(int *)(actions + byteOffset + 4);
        int mismatch = (actionLow ^ low) | (actionHigh ^ high);
        if (mismatch == 0) {
            if (result == 0) {
                result = operator new(0xc);
                Array_StringPtr_ctor(result);
            }
            String *string = (String *)operator new(0xc);
            actions = self->field_0x8c;
            char *keys = self->field_0x10;
            unsigned int keyIndex = *(unsigned int *)(actions + byteOffset + 8);
            new (string) String(*(String *)(keys + keyIndex * 0x10 + 4));
            ArrayAdd_StringPtr(string, result);
        }
        byteOffset += 0x10;
        index += 2;
    }
check:
    if (index < self->field_0x88) {
        goto loop;
    }
    return result;
}

// ---- OnKeyRelease_83d98.cpp ----
typedef void KeyCallback(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int);

extern "C" void ApplicationManager_OnKeyRelease(ApplicationManager *self, int key)
{
    self->field_0x98 = 0;
    self->field_0x9c = 0;

    unsigned int keyLow = 0;
    unsigned int keyHigh = 0;
    unsigned int actionLow = 0;
    unsigned int actionHigh = 0;
    unsigned int keyIndex = 0;
    int *mapping = (int *)self->field_0x10;
    while (keyIndex <= 0x3f) {
        if (*mapping == key) {
            int highIndex = (int)keyIndex - 0x20;
            keyLow = 1u << keyIndex;
            if (highIndex >= 0) {
                keyLow = 0;
            }
            keyHigh = 1u >> (0x20 - keyIndex);
            if (highIndex >= 0) {
                keyHigh = 1u << highIndex;
            }
            self->field_0x80 &= ~keyLow;
            self->field_0x84 &= ~keyHigh;

            unsigned int offset = 0;
            for (unsigned int i = 0; i < self->field_0x88; i += 2) {
                char *entry = self->field_0x8c + offset;
                if (*(unsigned int *)(entry + 8) == keyIndex && *(int *)(entry + 0x0c) == 0) {
                    actionLow |= *(uint32_t *)entry;
                    actionHigh |= *(uint32_t *)(entry + 4);
                    self->field_0x98 = actionLow;
                    self->field_0x9c = actionHigh;
                    self->field_0xa0 &= ~actionLow;
                    self->field_0xa4 &= ~actionHigh;
                }
                offset += 0x10;
            }
            break;
        }
        mapping += 4;
        ++keyIndex;
    }

    void *module = self->field_0x18;
    if (module != 0 && self->field_0x3c == 5) {
        void **vtable = *(void ***)module;
        ((KeyCallback *)vtable[0x14 / 4])(module, module, keyLow, keyHigh, actionLow, actionHigh);
    }
}

// ---- KeyCodeSetMapping_83ea4.cpp ----
extern "C" void ApplicationManager_KeyCodeSetMapping(ApplicationManager *self, void *array)
{
    unsigned int count = *(unsigned int *)array;
    if (count == 0x40) {
        unsigned int index = 0;
        int offset = 0;
        while (index < count) {
            uint32_t *mapping = *(uint32_t **)(*(char **)((char *)array + 4) + index * 4);
            char *dst = self->field_0x10 + offset;
            *(uint32_t *)dst = *mapping;
            String_assign((String *)(dst + 4), (String *)(mapping + 1));
            offset += 0x10;
            count = *(unsigned int *)array;
            ++index;
        }
    }
}

// ---- CheckForOrientationChange_826e8.cpp ----
static bool update_orientation_timer(ApplicationManager *self, int *timer)
{
    int elapsed = self->field_0x70 - self->field_0x78;
    int value = *timer + elapsed;
    *timer = value;
    return value >= 0xfb;
}

extern "C" void ApplicationManager_CheckForOrientationChange(ApplicationManager *self)
{
    void *engine = self->field_0xa8;
    double tilt = *(double *)((char *)engine + 0x4b0);
    void *canvas;
    int orientation;
    int *timer;
    int target;

    if (tilt < -0.5) {
        canvas = *(void **)self;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 0) {
            timer = &g_orientationLeft;
            target = 1;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
    }

    if (tilt > 0.5) {
        canvas = *(void **)self;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 3) {
            timer = &g_orientationRight;
            target = 2;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
        if (orientation == 1) {
            timer = &g_orientationFlat;
            if (update_orientation_timer(self, timer)) {
                PaintCanvas_SetGameOrientation(canvas, 0);
                *timer = 0;
            }
            return;
        }
    }

    if (tilt < -0.5) {
        canvas = *(void **)self;
        if (*(int *)((char *)canvas + 0x30) == 2) {
            timer = &g_orientationUpsideDown;
            target = 3;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
    }

    g_orientationInactive = 0;
    return;

setOrientation:
    PaintCanvas_SetGameOrientation(canvas, target);
    *timer = 0;
}

// ---- _ApplicationManager_82038.cpp ----
typedef void ModuleCallback(void *);

__attribute__((minsize)) ApplicationManager::~ApplicationManager()
{
    ApplicationManager *self = this;
    void *module = self->field_0x18;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x0c / 4](module);
    }

    void *modules = (char *)self + 0x44;
    unsigned int offset = 0;
    for (unsigned int i = 0; i < *(unsigned int *)modules; ++i) {
        void **slot = (void **)(self->field_0x48 + offset);
        void *entry = *slot;
        if (entry != 0) {
            ModuleCallback **vtable = *(ModuleCallback ***)entry;
            vtable[1](entry);
            slot = (void **)(self->field_0x48 + offset);
        }
        *slot = 0;
        offset += 4;
    }
    ArrayRelease_modules(modules);
    ArrayRelease_uint((char *)self + 0x50);

    void *canvas = *(void **)self;
    if (canvas != 0) {
        PaintCanvas_dtor(canvas);
        operator_delete(canvas);
    }
    *(void **)self = 0;

    void *sound = self->field_0xac;
    if (sound != 0) {
        AESoundRessource_dtor(sound);
        operator_delete(sound);
    }
    self->field_0xac = 0;

    void *cheats = self->field_0x30;
    if (cheats != 0) {
        CheatHandler_dtor(cheats);
        operator_delete(cheats);
    }
    self->field_0x30 = 0;

    void *reader = self->field_0x38;
    if (reader != 0) {
        ConfigReader_dtor(reader);
        operator_delete(reader);
    }
    self->field_0x38 = 0;

    char *keys = self->field_0x10;
    if (keys != 0) {
        unsigned int count = *(unsigned int *)(keys - 4);
        for (unsigned int i = count; i != 0; --i) {
            ((String *)(keys - 12 + i * 0x10))->~String();
        }
        operator_delete_array(keys - 8);
    }
    self->field_0x10 = 0;

    ArrayDtor_long_long((char *)self + 0x88);
    ArrayDtor_uint((char *)self + 0x50);
    ArrayDtor_modules(modules);
}

// ---- ConfigRegisterTokenReadFunction_825a4.cpp ----
typedef void ConfigTokenReadFunction(ConfigReader *, void *);

extern "C" void ConfigReader_RegisterTokenReadFunction(
    ConfigReader *reader, String *name, ConfigTokenReadFunction *read, void *context);

extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_ConfigRegisterTokenReadFunction(
    ApplicationManager *self, String *name, ConfigTokenReadFunction *read, void *context)
{
    void * volatile cookie = __stack_chk_guard;
    unsigned char storage[sizeof(String)] __attribute__((aligned(4)));
    ConfigReader *reader = (ConfigReader *)self->field_0x38;
    if (reader != 0) {
        String *copy = (String *)storage;
        new (copy) String(*name);
        ConfigReader_RegisterTokenReadFunction(reader, copy, read, context);
        ((String *)storage)->~String();
    }
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
