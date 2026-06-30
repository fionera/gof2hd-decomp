#include "game/menu/ModMainMenu.h"
#include "game/world/Galaxy.h"
#include "game/world/SolarSystem.h"
#include "game/world/Level.h"
#include "game/world/StarSystem.h"
#include "game/core/CutScene.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"
#include "engine/audio/FModSound.h"
#include "game/ui/MenuTouchWindow.h"
#include "game/ui/Layout.h"
#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"
#include "game/mission/GameRecord.h"
#include "game/mission/OptionsRecord.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "engine/core/AERandom.h"

static inline int FModSound_tryToStopMusicForBGMusic() { return (int)(Globals::sound->tryToStopMusicForBGMusic()); }

namespace AbyssEngine {
    namespace AEMath {
        float Sinf(float value);
    }
}

static Layout **g_ModMainMenu_releaseReload;
static ImageFactory **g_ModMainMenu_releaseImageFactory;
static FModSound **g_ModMainMenu_releaseSound;

static int *g_ModMainMenu_resumeObj;
static int *g_ModMainMenu_resumeArg;

static int *g_ModMainMenu_suspendObj;

static int *g_ModMainMenu_touchEndFlag;

static AbyssEngine::String **g_ModMainMenu_r2d_string;
static int *g_ModMainMenu_r2d_textId;
static int *g_ModMainMenu_r2d_screenW;
static int *g_ModMainMenu_r2d_screenH;

static Globals **g_ModMainMenu_initSoundRes;

static void (*g_ModMainMenu_initAddSound)(Globals *, int);

static OptionsRecord *g_ModMainMenu_initOptions;
static RecordHandler **g_ModMainMenu_initRecord;
static int *g_ModMainMenu_initTouchFlag;
static int *g_ModMainMenu_initMusicSlot;
static int *g_ModMainMenu_initMusic;

static Layout **g_ModMainMenu_updateLayout;
static FModSound **g_ModMainMenu_updateListener;

ModMainMenu::ModMainMenu() {
    this->initialized = 0;
    this->hasSavedGame = 0;
    this->state = 100;
    this->frameTime = 0;
    this->touchWindow = nullptr;
    this->cutScene = nullptr;
}

ModMainMenu::~ModMainMenu() {
    OnRelease();
}

long long ModMainMenu::OnKeyPress(long long key, long long mod) {
    (void) mod;
    return key;
}

long long ModMainMenu::OnKeyRelease(long long key, long long mod) {
    (void) mod;
    return key;
}

int ModMainMenu::ShowLoadingScreen() {
    return 1;
}

void ModMainMenu::OnRelease() {
    if (this->cutScene != nullptr)
        delete this->cutScene;
    this->cutScene = nullptr;

    if (this->touchWindow != nullptr)
        delete this->touchWindow;
    this->touchWindow = nullptr;

    Globals::Canvas->ReleaseAllResources();

    Globals::globals->loadFont(GameText::getLanguage());

    Layout **reload = g_ModMainMenu_releaseReload;
    if (*reload != nullptr) {
        (*reload)->reload();
        (*g_ModMainMenu_releaseImageFactory)->reload();
        (*reload)->initTip();
    }

    FModSound *sound = *g_ModMainMenu_releaseSound;
    if (sound != nullptr)
        sound->freeAllEvents();
}

void ModMainMenu::OnResume() {
    int *holder = g_ModMainMenu_resumeObj;
    if (*holder == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    int arg = *g_ModMainMenu_resumeArg;
    ((FModSound *) (intptr_t) * holder)->setVolume(1, (float) arg);
}

void ModMainMenu::OnRender3D() {
    Globals::Canvas->ClearBuffer(0);
    this->cutScene->renderBG();
    Globals::Canvas->Begin3d();
    this->cutScene->render3D();
    Globals::Canvas->End3d();
}

void ModMainMenu::OnTouchMove(int x, int y, void *touch) {
    // lint: void_ptr (exported virtual signature)
    (void) touch;
    if (this->logoActive != 0)
        return;
    this->touchWindow->OnTouchMove(x, y, nullptr);
}

void ModMainMenu::OnTouchMove(int x, int y) {
    (void) x;
    (void) y;
}

void ModMainMenu::OnSuspend() {
    int obj = *g_ModMainMenu_suspendObj;
    if (obj != 0)
        ((RecordHandler *) (intptr_t) obj)->saveOptions();
}

void ModMainMenu::OnTouchEnd(int x, int y, void *touch) {
    // lint: void_ptr (exported virtual signature)
    (void) touch;
    if (this->logoActive == 0) {
        this->touchWindow->OnTouchEnd(x, y, nullptr);
        Level *level = *(Level **) this->cutScene;
        ((StarSystem *) (intptr_t) level->getStarSystem())->initLight();
        return;
    }

    this->logoActive = 0;
    *g_ModMainMenu_touchEndFlag = 0;
}

void ModMainMenu::OnTouchEnd(int x, int y) {
    (void) x;
    (void) y;
}

void ModMainMenu::OnTouchBegin(int x, int y, void *touch) {
    // lint: void_ptr (exported virtual signature)
    if (this->logoActive != 0)
        return;
    this->touchWindow->OnTouchBegin(x, y, touch);
}

void ModMainMenu::OnTouchBegin(int x, int y) {
    (void) x;
    (void) y;
}

void ModMainMenu::OnRender2D() {
    ((PaintCanvas *) (long) this->paintCanvas)->Begin2d();
    ((PaintCanvas *) (long) this->paintCanvas)->SetColor((unsigned int) this->paintCanvas);
    this->cutScene->render2D();

    if (this->logoActive == 0) {
        this->touchWindow->draw();
    } else {
        int time = this->fadeTimer;
        int color = time <= 0x0f3b
                        ? (int) (((float) time / 3900.0f) * 255.0f) - 0x100
                        : -1;
        (void) color;
        ((PaintCanvas *) (long) this->paintCanvas)->SetColor((unsigned int) this->paintCanvas);

        Globals::Canvas->DrawImage2D((unsigned int) this->logoImage, 0, 0, (unsigned char) 'D');

        if (this->fadeTimer >= 0x0f3c) {
            int canvas = (int) (intptr_t) Globals::Canvas;
            float pulse = AbyssEngine::AEMath::Sinf(
                (float) Globals::appManager->GetSystemTimeMillis() * 0.003f);
            float signedPulse = pulse > 0.0f ? pulse : -pulse;
            int alpha = (unsigned int) (signedPulse * 255.0f);
            (void) alpha;
            Globals::Canvas->SetColor((unsigned char) canvas, 0xff, 0xff, 0xff);

            AbyssEngine::String **stringHolder = g_ModMainMenu_r2d_string;
            int *textIdHolder = g_ModMainMenu_r2d_textId;
            int drawCanvas = (int) (intptr_t) Globals::Canvas;
            AbyssEngine::String *drawStr = *stringHolder;
            int text = (int) (long) ((GameText *) (*textIdHolder))->getText(0xc7);

            int screenW = *g_ModMainMenu_r2d_screenW;
            int textWidth = Globals::Canvas->GetTextWidth((unsigned int) drawCanvas, *drawStr);

            int screenH = *g_ModMainMenu_r2d_screenH;
            int imageHeight = Globals::Canvas->GetImage2DHeight((unsigned int) (intptr_t) Globals::Canvas);
            Globals::Canvas->DrawString((unsigned int) drawCanvas, *drawStr, text,
                                        (screenW >> 1) - (textWidth >> 1),
                                        (bool) ((char) (screenH >> 1) + (char) (imageHeight >> 1) + '\n'));
        }
    }

    ((PaintCanvas *) (intptr_t) this->paintCanvas)->End2d();
}

void ModMainMenu::OnInitialize() {
    if (this->cutScene == nullptr) {
        Globals **soundRes = g_ModMainMenu_initSoundRes;
        (*soundRes)->startNewSoundResourceList();
        void(*addSound)(Globals *, int) = g_ModMainMenu_initAddSound;
        addSound(*soundRes, 0x7e);
        addSound(*soundRes, 0x15);
        addSound(*soundRes, 0x12);
        addSound(*soundRes, 0x13);
        addSound(*soundRes, 0x14);

        Globals::status->resetGame();
        Globals::rnd->reset();

        int station = Globals::galaxy->getStation(Globals::rnd->nextInt(100));
        Globals::status->setStation((Station *) (intptr_t) station);

        CutScene *cutscene = new CutScene(2);
        this->cutScene = cutscene;
        cutscene->initialize();

        int canvas = this->paintCanvas;
        int texture;
        if (Globals::status->inAlienOrbit() != 0) {
            texture = 0x2f08;
        } else {
            texture = (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() +
                       0x2efe) &
                      0xffff;
        }
        unsigned int texSlot = 0xffffffff;
        ((PaintCanvas *) (long) canvas)->TextureCreate((unsigned short) texture, texSlot, true);
        ((PaintCanvas *) (long) this->paintCanvas)->ChangeCubeTexture((unsigned int) this->paintCanvas);
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
        OptionsRecord *options = g_ModMainMenu_initOptions;
        if (options->firstRunPreviewChecked == 0) {
            RecordHandler **recordHolder = g_ModMainMenu_initRecord;
            GameRecord *record = (GameRecord *) (*recordHolder)->recordStoreReadPreview(0);
            if (record != nullptr) {
                this->hasSavedGame = 1;
                delete record;
            }
            options->firstRunPreviewChecked = 1;
            (*recordHolder)->saveOptions();
        }

        Globals::status->setPlayingTime(0);
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

music: {
        int *musicSlot = g_ModMainMenu_initMusicSlot;
        if (*musicSlot != -1)
            Globals::globals->playMusicAndFadeOutCurrent(*g_ModMainMenu_initMusic);
        *musicSlot = -1;
    }
    this->initialized = 1;
    this->state = 100;
    return;

state80:
    unsigned int logoImageHandle;
    Globals::Canvas->Image2DCreate(0x1b5a, logoImageHandle);
    this->logoImage = logoImageHandle;
    this->logoActive = 1;
    this->fadeTimer = 0;
    *g_ModMainMenu_initTouchFlag = 1;
    this->state = 0x3c;
}

void ModMainMenu::OnUpdate() {
    ApplicationManager *app = this->appManager;
    int elapsed = (int) app->GetElapsedTimeMillis();
    int frameTime;
    if (elapsed < 0x97 && (elapsed = (int) app->GetElapsedTimeMillis()) < 0) {
        frameTime = 0;
    } else {
        elapsed = (int) app->GetElapsedTimeMillis();
        if (elapsed > 0x96)
            frameTime = 0x96;
        else
            frameTime = (int) app->GetElapsedTimeMillis();
    }

    this->frameTime = frameTime;

    Layout **layout = g_ModMainMenu_updateLayout;
    (*layout)->update(frameTime);
    this->cutScene->update();
    if (this->logoActive == 0)
        this->touchWindow->update(this->frameTime);
    (*layout)->update(this->frameTime);

    FModSound **listener = g_ModMainMenu_updateListener;
    this->fadeTimer = this->frameTime + this->fadeTimer;
    (*listener)->updateAll(nullptr, nullptr, nullptr, nullptr);
}
