// Galaxy.h defines the B/I/P offset-cast helpers unconditionally; include it first
// and mark them as already provided so ModMainMenu.h does not redefine them.
#include "gof2/Galaxy.h"
#include "gof2/SolarSystem.h"
#define GOF2_BIP_HELPERS
#include "gof2/ModMainMenu.h"
#include "gof2/CutScene.h"
#include "gof2/FModSound.h"
#include "gof2/Level.h"
#include "gof2/MenuTouchWindow.h"
#include "gof2/Status.h"
#include "gof2/ApplicationManager.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/RecordHandler.h"
#include "gof2/PaintCanvasClass.h"   // real PaintCanvas:: methods


extern "C" void *CutScene_dtor(void *self);
int GameText_getLanguage();
void Globals_loadFont(int obj, int language);
extern "C" void ModMainMenu_releaseTail(void *sound);
extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void ModMainMenu_resumeTail(int obj, int one, int arg);
extern "C" void ModMainMenu_r3dTail(void *canvas);
ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self);
extern "C" void ModMainMenu_deleteTail(ModMainMenu *self);
extern "C" void ModMainMenu_suspendTail(int obj);
extern "C" void ModMainMenu_touchEndTail(void *starSystem);
void _ZN11ModMainMenu9OnReleaseEv(ModMainMenu *self);
namespace AbyssEngine { namespace AEMath { float Sinf(float value); } }
extern "C" void ModMainMenu_r2dTail(int canvas);
void Globals_startNewSoundResourceList(void *soundRes);
namespace AbyssEngine { namespace AERandom {
void reset(void *random);
int nextInt(void *random, int limit);
} }
extern "C" void CutScene_ctor(void *self, int mode);
extern "C" void *GameRecord_dtor(void *record);
void Globals_playMusicAndFadeOutCurrent(int music);

// ---- ModMainMenu_1757a0.cpp ----
__attribute__((visibility("hidden"))) extern void *volatile g_ModMainMenu_vtable;

void _ZN11ModMainMenuC2Ev(ModMainMenu *self)
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


void _ZN11ModMainMenu9OnReleaseEv(ModMainMenu *self)
{
    void *cutscene = P(self, 0x1c);
    if (cutscene != 0)
        ::operator delete(CutScene_dtor(cutscene));

    void *touchWindow = P(self, 0x18);
    P(self, 0x1c) = 0;
    if (touchWindow != 0)
        ::operator delete(((MenuTouchWindow *)(touchWindow))->dtor());

    P(self, 0x18) = 0;
    ((PaintCanvas *)*g_ModMainMenu_releaseCanvas)->ReleaseAllResources();

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


void _ZN11ModMainMenu8OnResumeEv(ModMainMenu *self)
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


void _ZN11ModMainMenu10OnRender3DEv(ModMainMenu *self)
{
    void **canvas = g_ModMainMenu_r3d_canvas;
    ((PaintCanvas *)*canvas)->ClearBuffer(0);
    ((CutScene *)(P(self, 0x1c)))->renderBG();
    ((PaintCanvas *)*canvas)->Begin3d();
    ((CutScene *)(P(self, 0x1c)))->render3D();
    ModMainMenu_r3dTail(*canvas);
}

// ---- _ModMainMenu_1757e8.cpp ----
void _ZN11ModMainMenuD0Ev(ModMainMenu *self)
{
    ModMainMenu_deleteTail(_ZN11ModMainMenuD2Ev(self));
}

// ---- OnTouchMove_175a98.cpp ----
extern "C" __attribute__((disable_tail_calls)) void _ZN11ModMainMenu11OnTouchMoveEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) != 0)
        return;
    ((MenuTouchWindow *)(P(self, 0x18)))->OnTouchMove(x, y);
}

// ---- OnSuspend_17582c.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_suspendObj;


void _ZN11ModMainMenu9OnSuspendEv(ModMainMenu *self)
{
    int obj = *g_ModMainMenu_suspendObj;
    if (obj != 0)
        ModMainMenu_suspendTail(obj);
}

// ---- OnTouchEnd_175ab4.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_touchEndFlag;


void _ZN11ModMainMenu10OnTouchEndEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) == 0) {
        ((MenuTouchWindow *)(P(self, 0x18)))->OnTouchEnd(x, y);
        ModMainMenu_touchEndTail((void *)(intptr_t)((Level *)(*(void **)P(self, 0x1c)))->getStarSystem());
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
    ((MenuTouchWindow *)(P(self, 0x18)))->OnTouchBegin(x, y, (int)(intptr_t)touch);
}

// ---- _ModMainMenu_1757c4.cpp ----


ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self)
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


void _ZN11ModMainMenu10OnRender2DEv(ModMainMenu *self)
{
    ((PaintCanvas *)(long)I(self, 0x04))->Begin2d();
    ((PaintCanvas *)(long)I(self, 0x04))->SetColor((unsigned int)I(self, 0x04));
    ((CutScene *)(self->f_1c))->render2D();

    if (UC(self, 0x28) == 0) {
        ((MenuTouchWindow *)(self->f_18))->draw();
    } else {
        int color;
        int time = I(self, 0x24);
        if (time <= 0x0f3b)
            color = (int)(((float)time / 3900.0f) * 255.0f) - 0x100;
        else
            color = -1;

        (void)color;
        ((PaintCanvas *)(long)I(self, 0x04))->SetColor((unsigned int)I(self, 0x04));

        int *imageHolder = g_ModMainMenu_r2d_image;
        ((PaintCanvas *)(long)*imageHolder)->DrawImage2D((unsigned int)I(self, 0x20), 0, 0, (unsigned char)'D');

        if (I(self, 0x24) >= 0x0f3c) {
            int canvas = *imageHolder;
            void **timeHolder = g_ModMainMenu_r2d_time;
            float pulseA =
                AbyssEngine::AEMath::Sinf((float)((ApplicationManager *)(*timeHolder))->GetSystemTimeMillis() *
                            0.003f);
            float pulseB =
                AbyssEngine::AEMath::Sinf((float)((ApplicationManager *)(*timeHolder))->GetSystemTimeMillis() *
                            0.003f);
            float signedPulse = pulseA > 0.0f ? pulseB : -pulseB;
            int alpha = (unsigned int)(signedPulse * 255.0f);
            (void)alpha;
            ((PaintCanvas *)(long)canvas)->SetColor((unsigned char)canvas, 0xff, 0xff, 0xff);

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
            int textWidth = ((PaintCanvas *)(long)measureCanvas)->GetTextWidth((unsigned int)measureCanvas, measureStr);

            int image = I(self, 0x20);
            int screenH = *g_ModMainMenu_r2d_screenH;
            int imageHeight = ((PaintCanvas *)(long)*imageHolder)->GetImage2DHeight((unsigned int)*imageHolder);
            ((PaintCanvas *)(long)draw.canvas)->DrawString((unsigned int)draw.canvas, draw.str, text,
                                   (screenW >> 1) - (textWidth >> 1),
                                   (bool)((char)(screenH >> 1) + (char)(imageHeight >> 1) + '\n'));
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


void _ZN11ModMainMenu12OnInitializeEv(ModMainMenu *self)
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
        ((Status *)(*statusHolder))->resetGame();
        void **randomHolder = g_ModMainMenu_initRandom;
        AbyssEngine::AERandom::reset(*randomHolder);

        void *status = *statusHolder;
        void *galaxy = *g_ModMainMenu_initGalaxy;
        void *station = (void *)(intptr_t)((Galaxy *)(galaxy))->getStation(AbyssEngine::AERandom::nextInt(*randomHolder, 100));
        ((Status *)(status))->setStation((Station *)station);

        void *cutscene = ::operator new(0xa0);
        CutScene_ctor(cutscene, 2);
        self->f_1c = cutscene;
        ((CutScene *)(cutscene))->initialize();

        int canvas = I(self, 0x04);
        int texture;
        if (((Status *)(*statusHolder))->inAlienOrbit() != 0) {
            texture = 0x2f08;
        } else {
            texture = (((SolarSystem *)((void *)(intptr_t)((Status *)(*statusHolder))->getSystem()))->getTextureIndex() +
                       0x2efe) &
                      0xffff;
        }
        unsigned int texSlot = 0xffffffff;
        ((PaintCanvas *)(long)canvas)->TextureCreate((unsigned short)texture, &texSlot, true);
        ((PaintCanvas *)(long)I(self, 0x04))->ChangeCubeTexture((unsigned int)I(self, 0x04));
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
            ::operator delete(GameRecord_dtor(record));
        }
        UC(options, 0x48) = 1;
        ((RecordHandler *)(*recordHolder))->saveOptions();
    }

    ((Status *)(*g_ModMainMenu_initPlayingTime))->setPlayingTime(0);
    window = ::operator new(0x240);
    ((MenuTouchWindow *)(window))->ctor(0);
    self->f_18 = window;
    if (UC(self, 0x29) != 0) {
        ((MenuTouchWindow *)(window))->showSupernovaMessage();
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
    ((PaintCanvas *)*g_ModMainMenu_initImageFactory)->Image2DCreate(0x1b5a,
                              (unsigned int *)B(self, 0x20));
    UC(self, 0x28) = 1;
    I(self, 0x24) = 0;
    *g_ModMainMenu_initTouchFlag = 1;
    I(self, 0x0c) = 0x3c;
}

// ---- OnUpdate_175b78.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateLayout;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateListener;


void _ZN11ModMainMenu8OnUpdateEv(ModMainMenu *self)
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
    ((CutScene *)(self->f_1c))->update();
    if (UC(self, 0x28) == 0)
        ((MenuTouchWindow *)(self->f_18))->update(I(self, 0x14));
    ((Layout *)(*layout))->update(I(self, 0x14));

    void **listener = g_ModMainMenu_updateListener;
    I(self, 0x24) = I(self, 0x14) + I(self, 0x24);
    void *zero = 0;
    ((FModSound *)(*listener))->updateAll((Vector *)zero, (Vector *)zero, (Vector *)zero, (Vector *)zero);
}
