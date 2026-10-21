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

void ModMainMenu::OnResume() {
    FModSound **holder = &Globals::sound;
    FModSound *sound = *holder;
    if (sound == nullptr)
        return;
    if (sound->tryToStopMusicForBGMusic() != 0)
        return;
    (*holder)->setVolume(1, *reinterpret_cast<float *>(Globals::options));
}

void ModMainMenu::OnSuspend() {
    if (Globals::recordHandler != nullptr)
        Globals::recordHandler->saveOptions();
}

int ModMainMenu::OnInitialize() {
    if (this->cutScene == nullptr) {
        // First call: load the menu sounds and set up the background cut scene at a random station.
        Globals::globals->startNewSoundResourceList();
        Globals::globals->addSoundResourceToList(0x7e);
        Globals::globals->addSoundResourceToList(0x15);
        Globals::globals->addSoundResourceToList(0x12);
        Globals::globals->addSoundResourceToList(0x13);
        Globals::globals->addSoundResourceToList(0x14);
        Globals::status->resetGame();
        Globals::rnd->reset();
        Globals::status->setStation((Station *) (intptr_t) Globals::galaxy->getStation(Globals::rnd->nextInt(100)));
        this->cutScene = new CutScene(2);
        this->cutScene->initialize();
        unsigned int skybox;
        if (Globals::status->inAlienOrbit())
            this->paintCanvas->TextureCreate(0x2f08, skybox, false);
        else
            this->paintCanvas
                    ->TextureCreate((unsigned short) (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() + 0x2efe),
                                    skybox, false);
        this->paintCanvas->ChangeCubeTexture(skybox);
        return 100;
    }

    switch (this->state) {
        case 100: {
            OptionsRecord *options = reinterpret_cast<OptionsRecord *>(Globals::options);
            if (!options->firstRunPreviewChecked) {
                GameRecord *record = (GameRecord *) Globals::recordHandler->recordStoreReadPreview(0);
                if (record != nullptr) {
                    this->hasSavedGame = 1;
                    delete record;
                }
                options->firstRunPreviewChecked = 1;
                Globals::recordHandler->saveOptions();
            }
            Globals::status->setPlayingTime(0);
            this->touchWindow = new MenuTouchWindow(0);
            if (this->hasSavedGame) {
                this->touchWindow->showSupernovaMessage();
                this->hasSavedGame = 0;
            }
            return this->state = 80;
        }
        case 80:
            Globals::Canvas->Image2DCreate(0x1b5a, reinterpret_cast<unsigned int &>(this->logoImage));
            this->fadeTimer = 0;
            this->logoActive = 1;
            Globals::logoIsShown = 1;
            return this->state = 60;
        case 60:
            return this->state = 30;
        case 30:
            this->state = 1;
            // fall through
        default:
            if (Globals::switch_to_target_setting != -1)
                Globals::globals->playMusicAndFadeOutCurrent(Globals::switch_to_target_setting);
            Globals::switch_to_target_setting = -1;
            this->state = 100;
            this->initialized = 1;
            return 0;
    }
}

void ModMainMenu::OnTouchBegin(int x, int y, void *touch) { // lint: void_ptr (exported virtual signature)
    // lint: void_ptr (exported virtual signature)
    if (this->logoActive != 0)
        return;
    this->touchWindow->OnTouchBegin(x, y, touch);
}

void ModMainMenu::OnTouchMove(int x, int y, void *touch) { // lint: void_ptr (exported virtual signature)
    // lint: void_ptr (exported virtual signature)
    if (this->logoActive != 0)
        return;
    this->touchWindow->OnTouchMove(x, y, touch);
}

void ModMainMenu::OnTouchEnd(int x, int y, void *touch) { // lint: void_ptr (exported virtual signature)
    // lint: void_ptr (exported virtual signature)
    if (this->logoActive == 0) {
        this->touchWindow->OnTouchEnd(x, y, touch);
        Level *level = *(Level **) this->cutScene;
        ((StarSystem *) (intptr_t) level->getStarSystem())->initLight();
        return;
    }

    this->logoActive = 0;
    Globals::logoIsShown = 0;
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
    if (Globals::layout != nullptr) {
        Globals::layout->reload();
        Globals::imageFactory->reload();
        Globals::layout->initTip();
    }
    if (Globals::sound != nullptr)
        Globals::sound->freeAllEvents();
}

void ModMainMenu::OnUpdate() {
    // Frame time clamped to [0, 150] ms.
    int frameTime;
    if ((int) this->applicationManager->GetElapsedTimeMillis() <= 150 && (int) this->applicationManager->GetElapsedTimeMillis() < 0)
        frameTime = 0;
    else if ((int) this->applicationManager->GetElapsedTimeMillis() > 150)
        frameTime = 150;
    else
        frameTime = (int) this->applicationManager->GetElapsedTimeMillis();
    this->frameTime = frameTime;

    Globals::layout->update(this->frameTime);
    this->cutScene->update(this->frameTime);
    if (!this->logoActive)
        this->touchWindow->update(this->frameTime);
    Globals::layout->update(this->frameTime);
    this->fadeTimer += this->frameTime;
    Globals::sound->updateAll(nullptr, nullptr, nullptr, nullptr);
}

void ModMainMenu::OnRender2D() {
    this->paintCanvas->Begin2d();
    this->paintCanvas->SetColor(0xffffffffu);
    this->cutScene->render2D();

    if (!this->logoActive) {
        this->touchWindow->draw();
    } else {
        // Logo fades in over 3.9 s, then a pulsing "touch to start" text appears below it.
        unsigned int color = this->fadeTimer <= 3899
                             ? (unsigned int) ((int) ((float) this->fadeTimer / 3900.0f * 255.0f) - 256)
                             : 0xffffffffu;
        this->paintCanvas->SetColor(color);
        Globals::Canvas->DrawImage2D((unsigned int) this->logoImage, 0, 0, 0x44, 0x44);
        if (this->fadeTimer >= 3900) {
            PaintCanvas *screen = Globals::Canvas;
            float pulse = AbyssEngine::AEMath::Sinf((float) (long long) Globals::appManager->GetSystemTimeMillis() * 0.003f) > 0.0f
                          ? AbyssEngine::AEMath::Sinf((float) (long long) Globals::appManager->GetSystemTimeMillis() * 0.003f)
                          : -AbyssEngine::AEMath::Sinf((float) (long long) Globals::appManager->GetSystemTimeMillis() * 0.003f);
            screen->SetColor(0xff, 0xff, 0xff, (unsigned char) (pulse * 255.0f));
            Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(199),
                    (Globals::w >> 1) -
                    (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                   *Globals::gameText->getText(199)) >> 1),
                    (Globals::h >> 1) + (Globals::Canvas->GetImage2DHeight((unsigned int) this->logoImage) >> 1) + 10,
                    false);
        }
    }
    this->paintCanvas->End2d();
}

void ModMainMenu::OnRender3D() {
    Globals::Canvas->ClearBuffer(0);
    this->cutScene->renderBG();
    Globals::Canvas->Begin3d();
    this->cutScene->render3D();
    Globals::Canvas->End3d();
}

