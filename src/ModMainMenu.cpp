#include "gof2/ModMainMenu.h"
#include "gof2/ApplicationManager.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/RecordHandler.h"


extern "C" void *CutScene_dtor(void *self);
extern "C" void *MenuTouchWindow_dtor(void *self);
extern "C" void operator_delete(void *ptr);
extern "C" void PaintCanvas_ReleaseAllResources(void *canvas);
extern "C" int GameText_getLanguage();
extern "C" void Globals_loadFont(int obj, int language);
extern "C" void ModMainMenu_releaseTail(void *sound);
extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void ModMainMenu_resumeTail(int obj, int one, int arg);
extern "C" void PaintCanvas_ClearBuffer(void *canvas, int value);
extern "C" void PaintCanvas_Begin3d(void *canvas);
extern "C" void CutScene_renderBG(void *scene);
extern "C" void CutScene_render3D(void *scene);
extern "C" void ModMainMenu_r3dTail(void *canvas);
extern "C" ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self);
extern "C" void ModMainMenu_deleteTail(ModMainMenu *self);
extern "C" void MenuTouchWindow_OnTouchMove(void *self, int x, int y, void *touch);
extern "C" void ModMainMenu_suspendTail(int obj);
extern "C" void MenuTouchWindow_OnTouchEnd(void *self, int x, int y, void *touch);
extern "C" void *Level_getStarSystem(void *level);
extern "C" void ModMainMenu_touchEndTail(void *starSystem);
extern "C" void MenuTouchWindow_OnTouchBegin(void *self, int x, int y, void *touch);
extern "C" void _ZN11ModMainMenu9OnReleaseEv(ModMainMenu *self);
extern "C" void PaintCanvas_Begin2d(int canvas);
extern "C" void PaintCanvas_SetColor(int canvas, int color);
extern "C" void PaintCanvas_SetColor4(int canvas, int r, int g, int b, int a);
extern "C" int PaintCanvas_GetTextWidth(int canvas, void *str, int text);
extern "C" int PaintCanvas_GetImage2DHeight(int canvas, int image);
extern "C" void CutScene_render2D(void *scene);
extern "C" void MenuTouchWindow_draw(void *window);
extern "C" float AEMath_Sinf(float value);
extern "C" void ModMainMenu_r2dTail(int canvas);
extern "C" void Globals_startNewSoundResourceList(void *soundRes);
extern "C" void Status_resetGame(void *status);
extern "C" void AERandom_reset(void *random);
extern "C" int AERandom_nextInt(void *random, int limit);
extern "C" void *Galaxy_getStation(void *galaxy, int index);
extern "C" void Status_setStation(void *status, void *station);
extern "C" void *operator_new(unsigned int size);
extern "C" void CutScene_ctor(void *self, int mode);
extern "C" void CutScene_initialize(void *self);
extern "C" int Status_inAlienOrbit(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int SolarSystem_getTextureIndex(void *system);
extern "C" void PaintCanvas_TextureCreate(int canvas, int texture, int *out, int flags);
extern "C" void PaintCanvas_ChangeCubeTexture(int canvas, int texture);
extern "C" void *GameRecord_dtor(void *record);
extern "C" void Status_setPlayingTime(int time, long long zero);
extern "C" void MenuTouchWindow_ctor(void *self, int mode);
extern "C" void MenuTouchWindow_showSupernovaMessage(void *self);
extern "C" void PaintCanvas_Image2DCreate(void *factory, int image, int *out);
extern "C" void Globals_playMusicAndFadeOutCurrent(int music);
extern "C" void CutScene_update(void *scene, int elapsed);
extern "C" void MenuTouchWindow_update(void *window, int elapsed);

// ---- ModMainMenu_1757a0.cpp ----
__attribute__((visibility("hidden"))) extern void *volatile g_ModMainMenu_vtable;

extern "C" void _ZN11ModMainMenuC2Ev(ModMainMenu *self)
{
    *(volatile uint8_t *)B(self, 0x10) = 0;
    *(volatile uint8_t *)B(self, 0x29) = 0;
    *(volatile int *)B(self, 0x0c) = 100;
    void *vtable = g_ModMainMenu_vtable;
    I(self, 0x14) = 0;
    I(self, 0x18) = 0;
    I(self, 0x1c) = 0;
    P(self, 0) = (char *)vtable + 8;
}

// ---- OnRelease_175aec.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseCanvas;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_releaseFontObj;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseReload;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseImageFactory;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseSound;


extern "C" void _ZN11ModMainMenu9OnReleaseEv(ModMainMenu *self)
{
    void *cutscene = P(self, 0x1c);
    if (cutscene != 0)
        operator_delete(CutScene_dtor(cutscene));

    void *touchWindow = P(self, 0x18);
    P(self, 0x1c) = 0;
    if (touchWindow != 0)
        operator_delete(MenuTouchWindow_dtor(touchWindow));

    P(self, 0x18) = 0;
    PaintCanvas_ReleaseAllResources(*g_ModMainMenu_releaseCanvas);

    int fontObj = *g_ModMainMenu_releaseFontObj;
    Globals_loadFont(fontObj, GameText_getLanguage());

    void **reload = g_ModMainMenu_releaseReload;
    if (*reload != 0) {
        ((Layout *)(*reload))->reload();
        ((ImageFactory *)(*g_ModMainMenu_releaseImageFactory))->reload();
        ((Layout *)(*reload))->initTip();
    }

    void *sound = *g_ModMainMenu_releaseSound;
    if (sound != 0)
        ModMainMenu_releaseTail(sound);
}

// ---- OnResume_1757f8.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_resumeObj;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_resumeArg;


extern "C" void _ZN11ModMainMenu8OnResumeEv(ModMainMenu *self)
{
    int *holder = g_ModMainMenu_resumeObj;
    int obj = *holder;
    if (obj == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    int arg = *g_ModMainMenu_resumeArg;
    ModMainMenu_resumeTail(*holder, 1, arg);
}

// ---- OnRender3D_175dc4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r3d_canvas;


extern "C" void _ZN11ModMainMenu10OnRender3DEv(ModMainMenu *self)
{
    void **canvas = g_ModMainMenu_r3d_canvas;
    PaintCanvas_ClearBuffer(*canvas, 0);
    CutScene_renderBG(P(self, 0x1c));
    PaintCanvas_Begin3d(*canvas);
    CutScene_render3D(P(self, 0x1c));
    ModMainMenu_r3dTail(*canvas);
}

// ---- _ModMainMenu_1757e8.cpp ----
extern "C" void _ZN11ModMainMenuD0Ev(ModMainMenu *self)
{
    ModMainMenu_deleteTail(_ZN11ModMainMenuD2Ev(self));
}

// ---- OnTouchMove_175a98.cpp ----
extern "C" __attribute__((disable_tail_calls)) void _ZN11ModMainMenu11OnTouchMoveEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) != 0)
        return;
    MenuTouchWindow_OnTouchMove(P(self, 0x18), x, y, touch);
}

// ---- OnSuspend_17582c.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_suspendObj;


extern "C" void _ZN11ModMainMenu9OnSuspendEv(ModMainMenu *self)
{
    int obj = *g_ModMainMenu_suspendObj;
    if (obj != 0)
        ModMainMenu_suspendTail(obj);
}

// ---- OnTouchEnd_175ab4.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_touchEndFlag;


extern "C" void _ZN11ModMainMenu10OnTouchEndEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) == 0) {
        MenuTouchWindow_OnTouchEnd(P(self, 0x18), x, y, touch);
        ModMainMenu_touchEndTail(Level_getStarSystem(*(void **)P(self, 0x1c)));
        return;
    }

    UC(self, 0x28) = 0;
    *g_ModMainMenu_touchEndFlag = 0;
}

// ---- OnTouchBegin_175a7c.cpp ----
extern "C" __attribute__((disable_tail_calls)) void _ZN11ModMainMenu12OnTouchBeginEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) != 0)
        return;
    MenuTouchWindow_OnTouchBegin(P(self, 0x18), x, y, touch);
}

// ---- _ModMainMenu_1757c4.cpp ----


extern "C" ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self)
{
    P(self, 0) = (char *)g_ModMainMenu_vtable + 8;
    _ZN11ModMainMenu9OnReleaseEv(self);
    return self;
}

// ---- OnRender2D_175c00.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_image;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r2d_time;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r2d_string;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_textId;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_screenW;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_screenH;

extern "C" void PaintCanvas_DrawImage2D(
    int canvas, int image, int x, int y, int alignX, int alignY);
extern "C" void PaintCanvas_DrawString(
    int canvas, void *str, int text, int x, int y, bool shadow);

extern "C" void _ZN11ModMainMenu10OnRender2DEv(ModMainMenu *self)
{
    PaintCanvas_Begin2d(I(self, 0x04));
    PaintCanvas_SetColor(I(self, 0x04), -1);
    CutScene_render2D(self->f_1c);

    if (UC(self, 0x28) == 0) {
        MenuTouchWindow_draw(self->f_18);
    } else {
        int color;
        int time = I(self, 0x24);
        if (time <= 0x0f3b)
            color = (int)(((float)time / 3900.0f) * 255.0f) - 0x100;
        else
            color = -1;

        PaintCanvas_SetColor(I(self, 0x04), color);

        int *imageHolder = g_ModMainMenu_r2d_image;
        PaintCanvas_DrawImage2D(*imageHolder, I(self, 0x20), 0, 0, 'D', 'D');

        if (I(self, 0x24) >= 0x0f3c) {
            int canvas = *imageHolder;
            void **timeHolder = g_ModMainMenu_r2d_time;
            float pulseA =
                AEMath_Sinf((float)((ApplicationManager *)(*timeHolder))->GetSystemTimeMillis() *
                            0.003f);
            float pulseB =
                AEMath_Sinf((float)((ApplicationManager *)(*timeHolder))->GetSystemTimeMillis() *
                            0.003f);
            float signedPulse = pulseA > 0.0f ? pulseB : -pulseB;
            int alpha = (unsigned int)(signedPulse * 255.0f);
            PaintCanvas_SetColor4(canvas, 0xff, 0xff, 0xff, alpha);

            void **stringHolder = g_ModMainMenu_r2d_string;
            int *textIdHolder = g_ModMainMenu_r2d_textId;
            struct DrawTarget {
                int canvas;
                void *str;
            };
            DrawTarget draw = {*imageHolder, *stringHolder};
            int text = (int)(long)((GameText *)(*textIdHolder))->getText(0xc7);

            int screenW = *g_ModMainMenu_r2d_screenW;
            void *measureStr = *stringHolder;
            int measureCanvas = *imageHolder;
            int measureText = (int)(long)((GameText *)(*textIdHolder))->getText(0xc7);
            int textWidth = PaintCanvas_GetTextWidth(measureCanvas, measureStr, measureText);

            int image = I(self, 0x20);
            int screenH = *g_ModMainMenu_r2d_screenH;
            int imageHeight = PaintCanvas_GetImage2DHeight(*imageHolder, image);
            PaintCanvas_DrawString(draw.canvas, draw.str, text,
                                   (screenW >> 1) - (textWidth >> 1),
                                   (screenH >> 1) + (imageHeight >> 1) + 10, false);
        }
    }

    ModMainMenu_r2dTail(I(self, 0x04));
}

// ---- OnInitialize_175844.cpp ----
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
        record = ((RecordHandler *)(*recordHolder))->recordStoreReadPreview(0);
        if (record != 0) {
            UC(self, 0x29) = 1;
            operator_delete(GameRecord_dtor(record));
        }
        UC(options, 0x48) = 1;
        ((RecordHandler *)(*recordHolder))->saveOptions();
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

// ---- OnUpdate_175b78.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateLayout;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateListener;

extern "C" void FModSound_updateAll(
    void *listener, void *position, void *velocity, void *forward, void *up);

extern "C" void _ZN11ModMainMenu8OnUpdateEv(ModMainMenu *self)
{
    int elapsed = ((ApplicationManager *)(self->f_8))->GetElapsedTimeMillis();
    int frameTime;
    if (elapsed < 0x97 &&
        (elapsed = ((ApplicationManager *)(self->f_8))->GetElapsedTimeMillis()) < 0) {
        frameTime = 0;
    } else {
        elapsed = ((ApplicationManager *)(self->f_8))->GetElapsedTimeMillis();
        if (elapsed > 0x96)
            frameTime = 0x96;
        else
            frameTime = ((ApplicationManager *)(self->f_8))->GetElapsedTimeMillis();
    }

    I(self, 0x14) = frameTime;

    void **layout = g_ModMainMenu_updateLayout;
    ((Layout *)(*layout))->update(frameTime);
    CutScene_update(self->f_1c, I(self, 0x14));
    if (UC(self, 0x28) == 0)
        MenuTouchWindow_update(self->f_18, I(self, 0x14));
    ((Layout *)(*layout))->update(I(self, 0x14));

    void **listener = g_ModMainMenu_updateListener;
    I(self, 0x24) = I(self, 0x14) + I(self, 0x24);
    void *zero = 0;
    FModSound_updateAll(*listener, zero, zero, zero, zero);
}
