#include "engine/core/GameData.h"

#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameText.h"
#include "engine/core/AERandom.h"
#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"
#include "game/core/Globals.h"
#include "game/core/CheatHandler.h"
#include "game/ui/Layout.h"
#include "game/menu/MGame.h"
#include "game/menu/ModMainMenu.h"
#include "game/menu/ModStation.h"
#include "game/menu/MTitle.h"
#include "platform/android/NdkHooks.h"

// Classes/Game/GameInit.cpp of the original: application bootstrap (GameData/Globals creation,
// module registration), teardown and the loading-screen callback. GameData's implicit ctor/dtor
// are emitted here as well (.text 0xd03e4..0xd0d14).

void loadingScreen(AbyssEngine::PaintCanvas *canvas, int progress, void *resourceHolder) { // lint: void_ptr (exported free function; mangled signature is fixed)
    // lint: void_ptr (exported free function; mangled signature is fixed)
    canvas->ClearBuffer(0xff);
    canvas->Begin2d();
    canvas->SetColor(0xffffffffu);

    if (resourceHolder == nullptr) {
        canvas->End2d();
        canvas->SwapBuffer();
        return;
    }

    unsigned int resId = *static_cast<unsigned int *>(resourceHolder);
    if (GameText::getLanguage() == 9 ||
        !canvas->ResourceLoaded(resId, static_cast<AbyssEngine::ResourceType>(1))) {
        Globals::globals->loadFont(GameText::getLanguage());
        *static_cast<unsigned int *>(resourceHolder) = resId;
    }

    unsigned int spinner = 0xffffffffu;
    unsigned int barFill = 0xffffffffu;
    Globals::Canvas->Image2DCreate(0x504, spinner);
    Globals::Canvas->Image2DCreate(0x505, barFill);

    int barFillW = Globals::Canvas->GetImage2DWidth(barFill);
    int spinnerW = Globals::Canvas->GetImage2DWidth(spinner);
    int barFillH = Globals::Canvas->GetImage2DHeight(barFill);

    int topMargin = *reinterpret_cast<int *>((unsigned char *) Globals::globals + 0x27c);

    Globals::layout->drawBG();
    Globals::layout->drawHeader();
    Globals::layout->drawEmptyFooter(false);

    int captionId = Globals::gameSaving ? 0x18a : 0x189;
    String caption(*Globals::gameText->getText(captionId), false);

    if (progress < 0) progress = 100;
    int barShortfall = 100;
    if (progress > 0) barShortfall = 100 - progress;

    int *layoutFields = *reinterpret_cast<int **>(Globals::layout);
    int globalsCenterX = *reinterpret_cast<int *>((unsigned char *) Globals::globals);
    int barBaseline = *reinterpret_cast<int *>((unsigned char *) Globals::globals + 0x290);

    int captionW = canvas->GetTextWidth(resId, caption);
    int captionX = (Globals::w / 2) - (captionW / 2);
    int captionY = (Globals::h / 2) - topMargin - layoutFields[1] - layoutFields[0x280 / 4];
    canvas->DrawString(resId, caption, captionX, captionY, false);

    int spinnerX = (globalsCenterX / 2) - (spinnerW / 2);
    int spinnerY = (Globals::h / 2) - topMargin;
    canvas->DrawImage2D(spinner, spinnerX, spinnerY);

    float fillW = static_cast<float>(barShortfall < 0 ? 0 : barShortfall) * 0.01f *
                  static_cast<float>(barFillW);
    int barTransY = barBaseline + ((Globals::h / 2) - topMargin);
    int barCenterX = globalsCenterX / 2;

    if (GameText::getLanguage() == 9) {
        float pivot = static_cast<float>(barCenterX + (barFillW / 2)) - fillW;
        Globals::Canvas->DrawRegion2D(barFill,
                                  static_cast<int>(static_cast<float>(barFillW) - fillW), 0,
                                  static_cast<int>(fillW), barFillH,
                                  0.0f, 0, 0,
                                  static_cast<int>(pivot), barTransY);
    } else {
        Globals::Canvas->DrawRegion2D(barFill,
                                  0, 0,
                                  static_cast<int>(fillW), barFillH,
                                  0.0f, 0, 0,
                                  barCenterX - (barFillW / 2), barTransY);
    }

    canvas->End2d();
    canvas->SwapBuffer();
}

void OnDestroyApplication(AbyssEngine::Engine *engine) {
    AbyssEngine::ApplicationManager *app =
            *reinterpret_cast<AbyssEngine::ApplicationManager **>(
                reinterpret_cast<char *>(engine) + 0x30);

    GameData *data = static_cast<GameData *>(app->GetApplicationData());

    if (data->globals != nullptr) {
        delete data->globals;
    }
    data->globals = nullptr;

    if (data != nullptr) {
        delete data;
    }
}

void OnCreateApplication(AbyssEngine::Engine *engine) {
    using AbyssEngine::Engine;

    GameData *data = new GameData();
    data->field_0x04 = 0;
    data->field_0x0c = 0;
    data->field_0x0e = 0;
    data->field_0x14 = 0;
    data->field_0x10 = -1;
    data->field_0x44 = 0;
    data->field_0x3c = 0;
    data->field_0x3d = 0;
    data->field_0x3e = 0;
    data->field_0x3f = 0;
    data->field_0x40 = 0;
    data->field_0x41 = 0;
    data->field_0x42 = 0;
    data->field_0x43 = 0;
    data->field_0x48 = -1;
    data->field_0x4c = 1;
    data->field_0x08 = 0;
    data->field_0x6d = 0;
    data->field_0x6e = 0;
    data->field_0x6f = 0;
    data->field_0x70 = 0;
    data->field_0x71 = 0;
    data->field_0x72 = 0;
    data->field_0x73 = 0;
    data->field_0x74 = 0;
    data->field_0x75 = 0;
    data->field_0x76 = 0;
    data->field_0x77 = 0;
    data->field_0x50 = 0;
    data->field_0x54 = 0;
    data->field_0x58 = 0;
    data->field_0x5c = 0;
    data->field_0x60 = 0;
    data->field_0x64 = 0;
    data->field_0x68 = 0;

    data->globals = new Globals();

    const int width = static_cast<int>(engine->GetDeviceInfo().width);
    const int height = static_cast<int>(engine->GetDeviceInfo().height);
    const bool pad = engine->GetDeviceInfo().isPad != 0;

    if (height < 640 || pad)
        Globals::retinaDisplay = engine->GetDeviceInfo().isPad != 0 && height > 800;
    else
        Globals::retinaDisplay = true;

    data->field_0xa8 = AbyssEngine::String("", false);
    data->field_0xa5 = 0;
    data->field_0xb8 = AbyssEngine::String("", false);
    data->field_0xc4 = 0;
    data->field_0xb4 = 0;
    data->field_0x78 = 0;
    data->field_0x7a = 0;
    data->field_0x7c = AbyssEngine::String("", false);
    data->field_0x88 = AbyssEngine::String("", false);
    data->field_0x94 = AbyssEngine::String("", false);
    data->field_0xa4 = 0;
    data->field_0xa0 = 0;
    data->field_0xa1 = 0;
    data->field_0xa2 = 0;
    data->field_0xa3 = 0;

    Globals::n9 = height == 480 && width == 864;

    const unsigned char isPad = engine->GetDeviceInfo().isPad;
    Globals::iPadHD = 0;
    Globals::iPad = isPad;
    Globals::iPadLargePossible = 0;
    Globals::iPadLarge = isPad != 0 ? Globals::retinaDisplay : 0;
    Globals::iPadAssetsWithLowerRes = isPad != 0 ? (height < 768 && width < 1024) : 0;
    Globals::switch_to_target_setting = -1;
    Globals::enterSpaceLounge = 0;

    BuildResourceList(engine);

    Globals::appManager = engine->appManager;
    Globals::Canvas = engine->appManager->paintCanvas;
    Globals::w = Globals::Canvas->GetWidth();
    Globals::h = Globals::Canvas->GetHeight();

    GameText *text = new GameText();
    Globals::gameText = text;
    text->setLanguage(static_cast<short>(AbyssEngine::Engine::countryCode), 3401);
    Globals::globals->loadFont(GameText::getLanguage());
    data->globals->init(engine->appManager, engine);

    engine->appManager->SetApplicationData(data);
    engine->appManager->SetLoadingCallback(&loadingScreen, Globals::font);

    engine->appManager->RegisterApplicationModule(
        2, reinterpret_cast<AbyssEngine::IApplicationModule *>(new MGame()));
    engine->appManager->RegisterApplicationModule(
        1, reinterpret_cast<AbyssEngine::IApplicationModule *>(new ModMainMenu()));
    engine->appManager->RegisterApplicationModule(
        5, reinterpret_cast<AbyssEngine::IApplicationModule *>(new ModStation()));
    engine->appManager->RegisterApplicationModule(
        0, reinterpret_cast<AbyssEngine::IApplicationModule *>(new MTitle()));

    engine->appManager->CheatSetCallback(&OnCheatActivated, nullptr);
    engine->appManager->CheatAddCode(AbyssEngine::String("754753835", false), 0);
    engine->appManager->CheatAddCode(AbyssEngine::String("448366639", false), 1);
    engine->appManager->CheatAddCode(AbyssEngine::String("373352623", false), 2);

    Globals::layout->initTip();
    Globals::status->resetGame();
    AbyssEngine::Engine::vfc = true;
    Globals::rnd->reset();
    engine->appManager->SetCurrentApplicationModule(0);
    AbyssEngine::Engine::clampTextures = false;

    engine->appManager->paintCanvas->TextureCreateGlobal(
        AbyssEngine::String("data/textures/pow_texture.aei", false), 2);

    AbyssEngine::Engine::vboSupported = true;
    AbyssEngine::Engine::clampTextures = false;
    engine->field_0x74 = false;
    AbyssEngine::Engine::lodBiasNormal = -0.5f;
    AbyssEngine::Engine::lodBiasDiffuse = -1.3f;
}
