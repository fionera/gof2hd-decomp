#include "gof2/game/menu/ModMainMenu.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/world/StarSystem.h"
#include "gof2/game/core/CutScene.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/ui/MenuTouchWindow.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/mission/RecordHandler.h"
#include "gof2/game/mission/GameRecord.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/engine/render/ImageFactory.h"

// ---- engine glue ---------------------------------------------------------------
// These resolve to other game singletons/helpers in the full link; they are kept as
// externs because the menu does not own the objects they refer to.
int  GameText_getLanguage();
void Globals_loadFont(int obj, int language);
void Globals_startNewSoundResourceList(void *soundRes);
void Globals_playMusicAndFadeOutCurrent(int music);
int  FModSound_tryToStopMusicForBGMusic();

namespace AbyssEngine {
namespace AEMath { float Sinf(float value); }
namespace AERandom {
void reset(void *random);
int  nextInt(void *random, int limit);
}
}

extern void *volatile g_ModMainMenu_vtable;

void **g_ModMainMenu_releaseCanvas;
int  *g_ModMainMenu_releaseFontObj;
void **g_ModMainMenu_releaseReload;
void **g_ModMainMenu_releaseImageFactory;
void **g_ModMainMenu_releaseSound;

int  *g_ModMainMenu_resumeObj;
int  *g_ModMainMenu_resumeArg;

void **g_ModMainMenu_r3d_canvas;

int  *g_ModMainMenu_suspendObj;

int  *g_ModMainMenu_touchEndFlag;

int  *g_ModMainMenu_r2d_image;
void **g_ModMainMenu_r2d_time;
void **g_ModMainMenu_r2d_string;
int  *g_ModMainMenu_r2d_textId;
int  *g_ModMainMenu_r2d_screenW;
int  *g_ModMainMenu_r2d_screenH;

void **g_ModMainMenu_initSoundRes;
void (*g_ModMainMenu_initAddSound)(void *, int);
void **g_ModMainMenu_initStatus;
void **g_ModMainMenu_initRandom;
void **g_ModMainMenu_initGalaxy;
void *g_ModMainMenu_initOptions;
void **g_ModMainMenu_initRecord;
int  *g_ModMainMenu_initPlayingTime;
void **g_ModMainMenu_initImageFactory;
int  *g_ModMainMenu_initTouchFlag;
int  *g_ModMainMenu_initMusicSlot;
int  *g_ModMainMenu_initMusic;

void **g_ModMainMenu_updateLayout;
void **g_ModMainMenu_updateListener;

// --------------------------------------------------------------------------------

ModMainMenu::ModMainMenu()
{
    this->initialized = 0;
    this->hasSavedGame = 0;
    this->state = 100;
    this->frameTime = 0;
    this->touchWindow = nullptr;
    this->cutScene = nullptr;
    this->vtable = (char *)g_ModMainMenu_vtable + 8;
}

ModMainMenu::~ModMainMenu()
{
    this->vtable = (char *)g_ModMainMenu_vtable + 8;
    OnRelease();
}

void ModMainMenu::OnRelease()
{
    if (this->cutScene != nullptr)
        delete this->cutScene;
    this->cutScene = nullptr;

    if (this->touchWindow != nullptr)
        delete this->touchWindow;
    this->touchWindow = nullptr;

    ((PaintCanvas *)*g_ModMainMenu_releaseCanvas)->ReleaseAllResources();

    int fontObj = *g_ModMainMenu_releaseFontObj;
    Globals_loadFont(fontObj, GameText_getLanguage());

    void **reload = g_ModMainMenu_releaseReload;
    if (*reload != nullptr) {
        ((Layout *)(*reload))->reload();
        ((ImageFactory *)(*g_ModMainMenu_releaseImageFactory))->reload();
        ((Layout *)(*reload))->initTip();
    }

    void *sound = *g_ModMainMenu_releaseSound;
    if (sound != nullptr)
        ((FModSound *)sound)->freeAllEvents();
}

void ModMainMenu::OnResume()
{
    int *holder = g_ModMainMenu_resumeObj;
    if (*holder == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    int arg = *g_ModMainMenu_resumeArg;
    ((FModSound *)(intptr_t)*holder)->setVolume(1, (float)arg);
}

void ModMainMenu::OnRender3D()
{
    void **canvas = g_ModMainMenu_r3d_canvas;
    ((PaintCanvas *)*canvas)->ClearBuffer(0);
    this->cutScene->renderBG();
    ((PaintCanvas *)*canvas)->Begin3d();
    this->cutScene->render3D();
    ((PaintCanvas *)*canvas)->End3d();
}

void ModMainMenu::OnTouchMove(int x, int y, void *touch)
{
    (void)touch;
    if (this->logoActive != 0)
        return;
    this->touchWindow->OnTouchMove(x, y);
}

void ModMainMenu::OnSuspend()
{
    int obj = *g_ModMainMenu_suspendObj;
    if (obj != 0)
        ((RecordHandler *)(intptr_t)obj)->saveOptions();
}

void ModMainMenu::OnTouchEnd(int x, int y, void *touch)
{
    (void)touch;
    if (this->logoActive == 0) {
        this->touchWindow->OnTouchEnd(x, y);
        Level *level = *(Level **)this->cutScene;
        ((StarSystem *)(intptr_t)level->getStarSystem())->initLight();
        return;
    }

    this->logoActive = 0;
    *g_ModMainMenu_touchEndFlag = 0;
}

void ModMainMenu::OnTouchBegin(int x, int y, void *touch)
{
    if (this->logoActive != 0)
        return;
    this->touchWindow->OnTouchBegin(x, y, (int)(intptr_t)touch);
}

void ModMainMenu::OnRender2D()
{
    ((PaintCanvas *)(long)this->paintCanvas)->Begin2d();
    ((PaintCanvas *)(long)this->paintCanvas)->SetColor((unsigned int)this->paintCanvas);
    this->cutScene->render2D();

    if (this->logoActive == 0) {
        this->touchWindow->draw();
    } else {
        int time = this->fadeTimer;
        int color = time <= 0x0f3b
                        ? (int)(((float)time / 3900.0f) * 255.0f) - 0x100
                        : -1;
        (void)color;
        ((PaintCanvas *)(long)this->paintCanvas)->SetColor((unsigned int)this->paintCanvas);

        int *imageHolder = g_ModMainMenu_r2d_image;
        ((PaintCanvas *)(long)*imageHolder)->DrawImage2D((unsigned int)this->logoImage, 0, 0, (unsigned char)'D');

        if (this->fadeTimer >= 0x0f3c) {
            int canvas = *imageHolder;
            void **timeHolder = g_ModMainMenu_r2d_time;
            float pulse = AbyssEngine::AEMath::Sinf(
                (float)((ApplicationManager *)(*timeHolder))->GetSystemTimeMillis() * 0.003f);
            float signedPulse = pulse > 0.0f ? pulse : -pulse;
            int alpha = (unsigned int)(signedPulse * 255.0f);
            (void)alpha;
            ((PaintCanvas *)(long)canvas)->SetColor((unsigned char)canvas, 0xff, 0xff, 0xff);

            void **stringHolder = g_ModMainMenu_r2d_string;
            int *textIdHolder = g_ModMainMenu_r2d_textId;
            int drawCanvas = *imageHolder;
            void *drawStr = *stringHolder;
            int text = (int)(long)((GameText *)(*textIdHolder))->getText(0xc7);

            int screenW = *g_ModMainMenu_r2d_screenW;
            int textWidth = ((PaintCanvas *)(long)drawCanvas)->GetTextWidth((unsigned int)drawCanvas, drawStr);

            int screenH = *g_ModMainMenu_r2d_screenH;
            int imageHeight = ((PaintCanvas *)(long)*imageHolder)->GetImage2DHeight((unsigned int)*imageHolder);
            ((PaintCanvas *)(long)drawCanvas)->DrawString((unsigned int)drawCanvas, drawStr, text,
                                   (screenW >> 1) - (textWidth >> 1),
                                   (bool)((char)(screenH >> 1) + (char)(imageHeight >> 1) + '\n'));
        }
    }

    ((PaintCanvas *)(intptr_t)this->paintCanvas)->End2d();
}

void ModMainMenu::OnInitialize()
{
    if (this->cutScene == nullptr) {
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
        int station = ((Galaxy *)galaxy)->getStation(AbyssEngine::AERandom::nextInt(*randomHolder, 100));
        ((Status *)status)->setStation((Station *)(intptr_t)station);

        CutScene *cutscene = new CutScene(2);
        this->cutScene = cutscene;
        cutscene->initialize();

        int canvas = this->paintCanvas;
        int texture;
        if (((Status *)(*statusHolder))->inAlienOrbit() != 0) {
            texture = 0x2f08;
        } else {
            texture = (((SolarSystem *)(intptr_t)((Status *)(*statusHolder))->getSystem())->getTextureIndex() +
                       0x2efe) &
                      0xffff;
        }
        unsigned int texSlot = 0xffffffff;
        ((PaintCanvas *)(long)canvas)->TextureCreate((unsigned short)texture, &texSlot, true);
        ((PaintCanvas *)(long)this->paintCanvas)->ChangeCubeTexture((unsigned int)this->paintCanvas);
        return;
    }

    int state = this->state;
    if (state == 0x1e)
        goto state30;
    if (state == 0x3c)
        goto state60;
    if (state == 0x50)
        goto state80;
    if (state != 100)
        goto music;

    {
        void *options = g_ModMainMenu_initOptions;
        if (UC(options, 0x48) == 0) {
            void **recordHolder = g_ModMainMenu_initRecord;
            void *record = ((RecordHandler *)(*recordHolder))->recordStoreReadPreview(0);
            if (record != nullptr) {
                this->hasSavedGame = 1;
                delete (GameRecord *)record;
            }
            UC(options, 0x48) = 1;
            ((RecordHandler *)(*recordHolder))->saveOptions();
        }

        ((Status *)(*g_ModMainMenu_initPlayingTime))->setPlayingTime(0);
        MenuTouchWindow *window = new MenuTouchWindow(0);
        this->touchWindow = window;
        if (this->hasSavedGame != 0) {
            window->showSupernovaMessage();
            this->hasSavedGame = 0;
        }
        this->state = 0x50;
    }
    return;

state60:
    this->state = 0x1e;
    return;

state30:
    this->state = 1;

music:
    {
        int *musicSlot = g_ModMainMenu_initMusicSlot;
        if (*musicSlot != -1)
            Globals_playMusicAndFadeOutCurrent(*g_ModMainMenu_initMusic);
        *musicSlot = -1;
    }
    this->initialized = 1;
    this->state = 100;
    return;

state80:
    ((PaintCanvas *)*g_ModMainMenu_initImageFactory)->Image2DCreate(0x1b5a,
                              (unsigned int *)&this->logoImage);
    this->logoActive = 1;
    this->fadeTimer = 0;
    *g_ModMainMenu_initTouchFlag = 1;
    this->state = 0x3c;
}

void ModMainMenu::OnUpdate()
{
    ApplicationManager *app = (ApplicationManager *)this->appManager;
    int elapsed = (int)app->GetElapsedTimeMillis();
    int frameTime;
    if (elapsed < 0x97 && (elapsed = (int)app->GetElapsedTimeMillis()) < 0) {
        frameTime = 0;
    } else {
        elapsed = (int)app->GetElapsedTimeMillis();
        if (elapsed > 0x96)
            frameTime = 0x96;
        else
            frameTime = (int)app->GetElapsedTimeMillis();
    }

    this->frameTime = frameTime;

    void **layout = g_ModMainMenu_updateLayout;
    ((Layout *)(*layout))->update(frameTime);
    this->cutScene->update();
    if (this->logoActive == 0)
        this->touchWindow->update(this->frameTime);
    ((Layout *)(*layout))->update(this->frameTime);

    void **listener = g_ModMainMenu_updateListener;
    this->fadeTimer = this->frameTime + this->fadeTimer;
    ((FModSound *)(*listener))->updateAll(nullptr, nullptr, nullptr, nullptr);
}
