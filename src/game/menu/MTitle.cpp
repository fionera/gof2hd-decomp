#include "gof2/game/menu/MTitle.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/core/PaintCanvasClass.h"

// Canonical render canvas singleton. PaintCanvasClass.h defines its own
// PaintCanvas (to dodge Mesh/Transform clashes) and does not declare gCanvas,
// so mirror the canonical extern locally rather than pulling in PaintCanvas.h.
extern PaintCanvas* gCanvas;

// Engine singletons reached through fixed global slots in the shipped binary.
extern Layout *g_currentLayout;         // the active full-screen Layout (may be null)
extern ImageFactory *g_imageFactory;    // the shared UI image factory
extern void *g_globalFont;              // the global font object Globals::loadFont mutates
extern FModSound *g_sound;              // the FMOD sound manager

// Localized text helpers (defined in GameText.cpp / Globals.cpp).
int GameText_getLanguage();
void Globals_loadFont(void *font, int lang);

MTitle::MTitle()
{
    this->renderPriority = 100;
}

MTitle::~MTitle()
{
    OnRelease();
}

int MTitle::OnInitialize()
{
    PaintCanvas *canvas = gCanvas;
    canvas->Image2DCreate(7000, &this->logoImage);
    canvas->Image2DCreate(0x1b59, &this->logoImage2);

    g_sound->play(0x91, (Vector *)0, (Vector *)0, 0.0f);

    this->step = 0;
    this->timer = 0;
    this->renderPriority = 100;
    return 0;
}

void MTitle::OnRelease()
{
    gCanvas->ReleaseAllResources();

    Globals_loadFont(g_globalFont, GameText_getLanguage());

    if (g_currentLayout != 0) {
        g_currentLayout->reload();
        g_imageFactory->reload();
    }
}

void MTitle::OnRender2D()
{
    PaintCanvas *canvas = gCanvas;
    canvas->Begin2d();
    canvas->SetColor((unsigned int)-1);

    g_currentLayout->drawBG();
    g_currentLayout->drawHeader0();
    g_currentLayout->drawEmptyFooter(0);

    // Advance the step timer, clamping the per-frame delta to 50ms.
    int delta = (int)this->applicationManager->GetElapsedTimeMillis();
    if (delta > 50)
        delta = 50;
    this->timer += delta;
    int t = this->timer;

    int image;
    float fade;
    if (t > 4000) {
        // Current logo has fully faded out: advance to the next logo (or hand
        // control to the main menu once both logos have played).
        this->step += 1;
        this->timer = 0;
        if (this->step == 2) {
            this->applicationManager->SetCurrentApplicationModule(1);
            return;
        }
        image = (int)(this->step == 0 ? this->logoImage2 : this->logoImage);
        fade = 0.0f;
    } else {
        image = (int)(this->step == 0 ? this->logoImage2 : this->logoImage);
        if (t < 1000) {
            fade = (float)t / 1000.0f;          // fade in
        } else if (t < 3001) {
            fade = 1.0f;                          // hold
        } else {
            fade = (float)(t - 3000) / -1000.0f + 1.0f;  // fade out
        }
    }

    int color = (int)(fade * 255.0f) - 0x100;
    canvas->SetColor((unsigned int)color);
    canvas->DrawImage2D((unsigned int)image, 0, 0, (unsigned char)0, (unsigned char)0x44);
    canvas->End2d();
}

void MTitle::OnRender3D()
{
    PaintCanvas *canvas = gCanvas;
    canvas->ClearBuffer(0xff);
    canvas->Begin3d();
}

void MTitle::OnTouchEnd(int x, int y)
{
    // Tapping the title skips straight to the end of the logo sequence.
    this->timer = 5000;
}
