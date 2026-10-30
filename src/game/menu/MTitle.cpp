#include "game/menu/MTitle.h"
#include "engine/core/GameText.h"
#include "engine/audio/FModSound.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"

static Layout **g_currentLayout = &Globals::layout;
static ImageFactory **g_imageFactory = &Globals::imageFactory;
static FModSound **g_sound = &Globals::sound;

MTitle::MTitle() {
    this->renderPriority = 100;
}

MTitle::~MTitle() {
    OnRelease();
}

int MTitle::OnInitialize() {
    Globals::Canvas->Image2DCreate(7000, this->logoImage);
    Globals::Canvas->Image2DCreate(0x1b59, this->logoImage2);

    (*g_sound)->play(0x91, (Vector *) 0, (Vector *) 0, 0.0f);

    this->step = 0;
    this->timer = 0;
    this->renderPriority = 100;
    return 0;
}

void MTitle::OnRelease() {
    Globals::Canvas->ReleaseAllResources();

    Globals::globals->loadFont(GameText::getLanguage());

    if (*g_currentLayout != nullptr) {
        (*g_currentLayout)->reload();
        (*g_imageFactory)->reload();
        (*g_currentLayout)->initTip();
    }
}

void MTitle::OnKeyPress(long long key, long long mod) {
}

void MTitle::OnTouchBegin(int x, int y) {
}

void MTitle::OnTouchMove(int x, int y) {
}

void MTitle::OnUpdate() {
}

void MTitle::OnRender2D() {
    this->paintCanvas->Begin2d();
    Globals::Canvas->SetColor((unsigned int) -1);

    Globals::layout->drawBG();
    Globals::layout->drawHeader();
    Globals::layout->drawEmptyFooter(false);

    this->timer += (int) this->applicationManager->GetElapsedTimeMillis() > 50
                           ? 50
                           : (int) this->applicationManager->GetElapsedTimeMillis();
    if (this->timer > 4000) {
        this->timer = 0;
        this->step += 1;
        if (this->step == 2) {
            Globals::appManager->SetCurrentApplicationModule(1);
            return;
        }
    }
    unsigned int image = this->step == 0 ? this->logoImage2 : this->logoImage;
    float fade;
    if (this->timer < 1000) {
        fade = (float) this->timer / 1000.0f;
    } else if (this->timer > 3000) {
        fade = (float) (this->timer - 3000) / -1000.0f + 1.0f;
    } else {
        fade = 1.0f;
    }

    this->paintCanvas->SetColor((unsigned int) ((int) (fade * 255.0f) - 0x100));
    Globals::Canvas->DrawImage2D(image, 0, 0, (unsigned char) 0x44, (unsigned char) 0x44);
    this->paintCanvas->End2d();
    this->paintCanvas->SwapBuffer();
}

void MTitle::OnRender3D() {
    Globals::Canvas->ClearBuffer(0xff);
    this->paintCanvas->Begin3d();
    this->paintCanvas->End3d();
}

void MTitle::OnTouchEnd(int x, int y) {
    this->timer = 5000;
}
