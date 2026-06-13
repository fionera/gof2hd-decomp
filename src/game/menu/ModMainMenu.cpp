// Galaxy.h defines the B/I/P offset-cast helpers unconditionally; include it first
// and mark them as already provided so ModMainMenu.h does not redefine them.
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/world/SolarSystem.h"
#define GOF2_BIP_HELPERS
#include "gof2/game/menu/ModMainMenu.h"
#include "gof2/game/core/CutScene.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/ui/MenuTouchWindow.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/mission/RecordHandler.h"
#include "gof2/game/core/PaintCanvasClass.h"   // real PaintCanvas:: methods

extern "C" void *CutScene_dtor(void *self);
int GameText_getLanguage();
void Globals_loadFont(int obj, int language);
extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void ModMainMenu_r3dTail(void *canvas);
ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self);
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

// ---- recovered tail-call fragments ----------------------------------------
// Each OnXxx() above peels its final tail-call out into a small *_Tail helper
// (the decompiler split the basic block at the tail branch). The body of each
// helper forwards into the engine/runtime method the original code branched to;
// the targets were resolved from the PLT veneers in the binary.
extern "C" void _ZN10StarSystem9initLightEv(void *self);   // StarSystem::initLight()

// OnRender3D(): finish the 3D pass that drew the rotating cut-scene backdrop.
extern "C" void ModMainMenu_r3dTail(void *canvas)
{
    ((PaintCanvas *)canvas)->End3d();
}

// OnRender2D(): close the 2D overlay pass (HUD/loading text) for this frame.
extern "C" void ModMainMenu_r2dTail(int canvas)
{
    ((PaintCanvas *)(intptr_t)canvas)->End2d();
}

// ---- the recovered tail fragments, as real ModMainMenu members ----------------
// Each OnXxx() peels its final tail-call out into one of these helpers; the body
// forwards into the engine/runtime method the original code branched to (the
// menu owns none of those objects directly, so each is passed in).

// OnRelease(): once the menu's sound resource list is torn down, free every
// remaining FMOD event handle owned by the sound manager.
void ModMainMenu::releaseTail(void *sound)
{
    ((FModSound *)sound)->freeAllEvents();
}

void ModMainMenu::resumeTail(int sound, int channel, int arg)
{
    ((FModSound *)(intptr_t)sound)->setVolume(channel, (float)arg);
}

void ModMainMenu::renderBackdropEnd(void *canvas)
{
    ((PaintCanvas *)canvas)->End3d();
}

void ModMainMenu::deleteTail(ModMainMenu *self)
{
    ::operator delete(self);
}

void ModMainMenu::suspendTail(int recordHandler)
{
    ((RecordHandler *)(intptr_t)recordHandler)->saveOptions();
}

void ModMainMenu::touchEndTail(void *starSystem)
{
    _ZN10StarSystem9initLightEv(starSystem);
}

void ModMainMenu::renderOverlayEnd(int canvas)
{
    ((PaintCanvas *)(intptr_t)canvas)->End2d();
}

__attribute__((visibility("hidden"))) extern void *volatile g_ModMainMenu_vtable;

void _ZN11ModMainMenuC2Ev(ModMainMenu *self)
{
    self->initialized = 0;
    self->hasSavedGame = 0;
    self->state = 100;
    void *vtable = g_ModMainMenu_vtable;
    self->frameTime = 0;
    self->touchWindow = 0;
    self->cutScene = 0;
    self->field_0x00 = (char *)vtable + 8;
}

__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseCanvas;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_releaseFontObj;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseReload;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseImageFactory;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseSound;

void _ZN11ModMainMenu9OnReleaseEv(ModMainMenu *self)
{
    void *cutscene = self->cutScene;
    if (cutscene != 0)
        ::operator delete(CutScene_dtor(cutscene));

    void *touchWindow = self->touchWindow;
    self->cutScene = 0;
    if (touchWindow != 0)
        ::operator delete(((MenuTouchWindow *)(touchWindow))->dtor());

    self->touchWindow = 0;
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
        ModMainMenu::releaseTail(sound);
}

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
    ModMainMenu::resumeTail(*holder, 1, arg);
}

__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r3d_canvas;

void _ZN11ModMainMenu10OnRender3DEv(ModMainMenu *self)
{
    void **canvas = g_ModMainMenu_r3d_canvas;
    ((PaintCanvas *)*canvas)->ClearBuffer(0);
    ((CutScene *)(self->cutScene))->renderBG();
    ((PaintCanvas *)*canvas)->Begin3d();
    ((CutScene *)(self->cutScene))->render3D();
    ModMainMenu_r3dTail(*canvas);
}

void _ZN11ModMainMenuD0Ev(ModMainMenu *self)
{
    ModMainMenu::deleteTail(_ZN11ModMainMenuD2Ev(self));
}

extern "C" __attribute__((disable_tail_calls)) void _ZN11ModMainMenu11OnTouchMoveEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (self->logoActive != 0)
        return;
    ((MenuTouchWindow *)(self->touchWindow))->OnTouchMove(x, y);
}

__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_suspendObj;

void _ZN11ModMainMenu9OnSuspendEv(ModMainMenu *self)
{
    int obj = *g_ModMainMenu_suspendObj;
    if (obj != 0)
        ModMainMenu::suspendTail(obj);
}

__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_touchEndFlag;

void _ZN11ModMainMenu10OnTouchEndEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (self->logoActive == 0) {
        ((MenuTouchWindow *)(self->touchWindow))->OnTouchEnd(x, y);
        ModMainMenu::touchEndTail((void *)(intptr_t)((Level *)(*(void **)(self->cutScene)))->getStarSystem());
        return;
    }

    self->logoActive = 0;
    *g_ModMainMenu_touchEndFlag = 0;
}

extern "C" __attribute__((disable_tail_calls)) void _ZN11ModMainMenu12OnTouchBeginEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (self->logoActive != 0)
        return;
    ((MenuTouchWindow *)(self->touchWindow))->OnTouchBegin(x, y, (int)(intptr_t)touch);
}

ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self)
{
    self->field_0x00 = (char *)g_ModMainMenu_vtable + 8;
    _ZN11ModMainMenu9OnReleaseEv(self);
    return self;
}

__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_image;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r2d_time;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_r2d_string;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_textId;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_screenW;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_r2d_screenH;

void _ZN11ModMainMenu10OnRender2DEv(ModMainMenu *self)
{
    ((PaintCanvas *)(long)self->paintCanvas)->Begin2d();
    ((PaintCanvas *)(long)self->paintCanvas)->SetColor((unsigned int)self->paintCanvas);
    ((CutScene *)(self->cutScene))->render2D();

    if (self->logoActive == 0) {
        ((MenuTouchWindow *)(self->touchWindow))->draw();
    } else {
        int color;
        int time = self->fadeTimer;
        if (time <= 0x0f3b)
            color = (int)(((float)time / 3900.0f) * 255.0f) - 0x100;
        else
            color = -1;

        (void)color;
        ((PaintCanvas *)(long)self->paintCanvas)->SetColor((unsigned int)self->paintCanvas);

        int *imageHolder = g_ModMainMenu_r2d_image;
        ((PaintCanvas *)(long)*imageHolder)->DrawImage2D((unsigned int)self->logoImage, 0, 0, (unsigned char)'D');

        if (self->fadeTimer >= 0x0f3c) {
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

            int image = self->logoImage;
            int screenH = *g_ModMainMenu_r2d_screenH;
            int imageHeight = ((PaintCanvas *)(long)*imageHolder)->GetImage2DHeight((unsigned int)*imageHolder);
            ((PaintCanvas *)(long)draw.canvas)->DrawString((unsigned int)draw.canvas, draw.str, text,
                                   (screenW >> 1) - (textWidth >> 1),
                                   (bool)((char)(screenH >> 1) + (char)(imageHeight >> 1) + '\n'));
        }
    }

    ModMainMenu_r2dTail(self->paintCanvas);
}

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

    if (self->cutScene == 0) {
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
        self->cutScene = cutscene;
        ((CutScene *)(cutscene))->initialize();

        int canvas = self->paintCanvas;
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
        ((PaintCanvas *)(long)self->paintCanvas)->ChangeCubeTexture((unsigned int)self->paintCanvas);
        return;
    }

    int state = self->state;
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
            self->hasSavedGame = 1;
            ::operator delete(GameRecord_dtor(record));
        }
        UC(options, 0x48) = 1;
        ((RecordHandler *)(*recordHolder))->saveOptions();
    }

    ((Status *)(*g_ModMainMenu_initPlayingTime))->setPlayingTime(0);
    window = ::operator new(0x240);
    ((MenuTouchWindow *)(window))->ctor(0);
    self->touchWindow = window;
    if (self->hasSavedGame != 0) {
        ((MenuTouchWindow *)(window))->showSupernovaMessage();
        self->hasSavedGame = 0;
    }
    self->state = 0x50;
    return;

state60:
    self->state = 0x1e;
    return;

state30:
    self->state = 1;

music:
    musicSlot = g_ModMainMenu_initMusicSlot;
    if (*musicSlot != -1)
        Globals_playMusicAndFadeOutCurrent(*g_ModMainMenu_initMusic);
    *musicSlot = -1;
    self->initialized = 1;
    self->state = 100;
    return;

state80:
    ((PaintCanvas *)*g_ModMainMenu_initImageFactory)->Image2DCreate(0x1b5a,
                              (unsigned int *)&self->logoImage);
    self->logoActive = 1;
    self->fadeTimer = 0;
    *g_ModMainMenu_initTouchFlag = 1;
    self->state = 0x3c;
}

__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateLayout;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateListener;

void _ZN11ModMainMenu8OnUpdateEv(ModMainMenu *self)
{
    int elapsed = ((ApplicationManager *)(self->appManager))->GetElapsedTimeMillis();
    int frameTime;
    if (elapsed < 0x97 &&
        (elapsed = ((ApplicationManager *)(self->appManager))->GetElapsedTimeMillis()) < 0) {
        frameTime = 0;
    } else {
        elapsed = ((ApplicationManager *)(self->appManager))->GetElapsedTimeMillis();
        if (elapsed > 0x96)
            frameTime = 0x96;
        else
            frameTime = ((ApplicationManager *)(self->appManager))->GetElapsedTimeMillis();
    }

    self->frameTime = frameTime;

    void **layout = g_ModMainMenu_updateLayout;
    ((Layout *)(*layout))->update(frameTime);
    ((CutScene *)(self->cutScene))->update();
    if (self->logoActive == 0)
        ((MenuTouchWindow *)(self->touchWindow))->update(self->frameTime);
    ((Layout *)(*layout))->update(self->frameTime);

    void **listener = g_ModMainMenu_updateListener;
    self->fadeTimer = self->frameTime + self->fadeTimer;
    void *zero = 0;
    ((FModSound *)(*listener))->updateAll((Vector *)zero, (Vector *)zero, (Vector *)zero, (Vector *)zero);
}
