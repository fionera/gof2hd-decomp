#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_initSoundRes;
__attribute__((visibility("hidden"))) extern void (*g_ModMainMenu_initAddSound)(void *, int);
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_initStatus;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_initRandom;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_initGalaxy;
__attribute__((visibility("hidden"))) extern void *g_ModMainMenu_initOptions;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_initRecord;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_initPlayingTime;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_initImageFactory;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_initTouchFlag;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_initMusicSlot;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_initMusic;

extern "C" void Globals_startNewSoundResourceList(void *soundRes);
extern "C" void Status_resetGame(void *status);
extern "C" void AERandom_reset(void *random);
extern "C" int AERandom_nextInt(void *random, int limit);
extern "C" void *Galaxy_getStation(void *galaxy, int index);
extern "C" void Status_setStation(void *status, void *station);
extern "C" void *operator_new(unsigned int size);
extern "C" void operator_delete(void *ptr);
extern "C" void CutScene_ctor(void *self, int mode);
extern "C" void CutScene_initialize(void *self);
extern "C" int Status_inAlienOrbit(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int SolarSystem_getTextureIndex(void *system);
extern "C" void PaintCanvas_TextureCreate(int canvas, int texture, int *out, int flags);
extern "C" void PaintCanvas_ChangeCubeTexture(int canvas, int texture);
extern "C" void *RecordHandler_recordStoreReadPreview(void *record, int slot);
extern "C" void *GameRecord_dtor(void *record);
extern "C" void RecordHandler_saveOptions(void *record);
extern "C" void Status_setPlayingTime(int time, long long zero);
extern "C" void MenuTouchWindow_ctor(void *self, int mode);
extern "C" void MenuTouchWindow_showSupernovaMessage(void *self);
extern "C" void PaintCanvas_Image2DCreate(void *factory, int image, int *out);
extern "C" void Globals_playMusicAndFadeOutCurrent(int music);

extern "C" void _ZN11ModMainMenu12OnInitializeEv(ModMainMenu *self)
{
    char textureOut[5];
    void *options;
    void **recordHolder;
    void *record;
    void *window;
    int *musicSlot;

    if (self->f_1c == 0) {
        void **soundRes = g_ModMainMenu_initSoundRes;
        Globals_startNewSoundResourceList(*soundRes);
        void (*addSound)(void *, int) = g_ModMainMenu_initAddSound;
        addSound(*soundRes, 0x7e);
        addSound(*soundRes, 0x15);
        addSound(*soundRes, 0x12);
        addSound(*soundRes, 0x13);
        addSound(*soundRes, 0x14);

        void **statusHolder = g_ModMainMenu_initStatus;
        Status_resetGame(*statusHolder);
        void **randomHolder = g_ModMainMenu_initRandom;
        AERandom_reset(*randomHolder);

        void *status = *statusHolder;
        void *galaxy = *g_ModMainMenu_initGalaxy;
        void *station = Galaxy_getStation(galaxy, AERandom_nextInt(*randomHolder, 100));
        Status_setStation(status, station);

        void *cutscene = operator_new(0xa0);
        CutScene_ctor(cutscene, 2);
        self->f_1c = cutscene;
        CutScene_initialize(cutscene);

        int canvas = I(self, 0x04);
        int texture;
        if (Status_inAlienOrbit(*statusHolder) != 0) {
            texture = 0x2f08;
        } else {
            texture = (SolarSystem_getTextureIndex(Status_getSystem(*statusHolder)) +
                       0x2efe) &
                      0xffff;
        }
        PaintCanvas_TextureCreate(canvas, texture, (int *)textureOut, 0);
        PaintCanvas_ChangeCubeTexture(I(self, 0x04), *(int *)textureOut);
        return;
    }

    int state = I(self, 0x0c);
    if (state == 0x1e)
        goto state30;
    if (state == 0x3c)
        goto state60;
    if (state == 0x50)
        goto state80;
    if (state != 100)
        goto music;

    options = g_ModMainMenu_initOptions;
    if (UC(options, 0x48) == 0) {
        recordHolder = g_ModMainMenu_initRecord;
        record = RecordHandler_recordStoreReadPreview(*recordHolder, 0);
        if (record != 0) {
            UC(self, 0x29) = 1;
            operator_delete(GameRecord_dtor(record));
        }
        UC(options, 0x48) = 1;
        RecordHandler_saveOptions(*recordHolder);
    }

    Status_setPlayingTime(*g_ModMainMenu_initPlayingTime, 0);
    window = operator_new(0x240);
    MenuTouchWindow_ctor(window, 0);
    self->f_18 = window;
    if (UC(self, 0x29) != 0) {
        MenuTouchWindow_showSupernovaMessage(window);
        UC(self, 0x29) = 0;
    }
    I(self, 0x0c) = 0x50;
    return;

state60:
    I(self, 0x0c) = 0x1e;
    return;

state30:
    I(self, 0x0c) = 1;

music:
    musicSlot = g_ModMainMenu_initMusicSlot;
    if (*musicSlot != -1)
        Globals_playMusicAndFadeOutCurrent(*g_ModMainMenu_initMusic);
    *musicSlot = -1;
    UC(self, 0x10) = 1;
    I(self, 0x0c) = 100;
    return;

state80:
    PaintCanvas_Image2DCreate(*g_ModMainMenu_initImageFactory, 0x1b5a,
                              (int *)B(self, 0x20));
    UC(self, 0x28) = 1;
    I(self, 0x24) = 0;
    *g_ModMainMenu_initTouchFlag = 1;
    I(self, 0x0c) = 0x3c;
}
