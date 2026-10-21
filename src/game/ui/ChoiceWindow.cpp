#include "game/ui/ChoiceWindow.h"
#include "engine/audio/FModSound.h"
#include "game/ui/ScrollTouchWindow.h"
#include "game/mission/Status.h"
#include "game/mission/Achievements.h"
#include "game/core/Globals.h"
#include "game/ui/ChoiceWindowButtonPosCache.h"
#include "engine/core/GameText.h"
#include "game/ui/Layout.h"
#include "game/ui/TouchButton.h"
#include "engine/render/PaintCanvas.h"

bool ChoiceWindow::OnTouchMove(int x, int y) {
    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->OnTouchMove(x, y);
        if (this->rightButton != nullptr) this->rightButton->OnTouchMove(x, y);
        if (this->miscButton != nullptr) this->miscButton->OnTouchMove(x, y);
    }
    if (this->scrollWindow != nullptr) this->scrollWindow->OnTouchMove(x, y);
    return false;
}

uint8_t ChoiceWindow::hasChoice() {
    return this->hasButtons;
}


void ChoiceWindow::setWidth(int width) {
    this->width = width;
    this->x = Globals::w / 2 - width / 2;
}


void ChoiceWindow::set(String const &a, String const &b) {
    set(*Globals::gameText->getText(390), a, false, *Globals::gameText->getText(134),
        *Globals::gameText->getText(135), b, -1, -1);
}


void ChoiceWindow::set(String const &text, bool flag) {
    String *title = (Globals::gameText)->getText(0x186);
    set(*title, text, flag);
}


void ChoiceWindow::set(String const &title, String const &message, bool flag) {
    set(title, message, flag, *Globals::gameText->getText(134), *Globals::gameText->getText(135),
        *Globals::gameText->getText(524), -1, -1);
}

int ChoiceWindow::OnTouchEnd(int x, int y) {
    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr && this->leftButton->OnTouchEnd(x, y) != 0)
            return 0;
        if (this->rightButton != nullptr && this->rightButton->OnTouchEnd(x, y) != 0)
            return 1;
        if (this->miscButton != nullptr && this->miscButton->OnTouchEnd(x, y) != 0)
            return 2;
    }
    if (this->scrollWindow != nullptr) this->scrollWindow->OnTouchEnd(x, y);
    return -1;
}


void ChoiceWindow::set(String const &text) {
    String *title = (Globals::gameText)->getText(0x186);
    set(*title, text, false);
}

void ChoiceWindow::update(int dt) {
    this->scrollWindow->update(dt);
}

void ChoiceWindow::removeButtons() {
    this->buttonsVisible = 0;
}

bool ChoiceWindow::OnTouchBegin(int x, int y) {
    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->OnTouchBegin(x, y);
        if (this->rightButton != nullptr) this->rightButton->OnTouchBegin(x, y);
        if (this->miscButton != nullptr) this->miscButton->OnTouchBegin(x, y);
    }
    if (this->scrollWindow != nullptr) this->scrollWindow->OnTouchBegin(x, y);
    return false;
}

ChoiceWindow::~ChoiceWindow() {
    delete this->leftButton;
    this->leftButton = nullptr;

    delete this->rightButton;
    this->rightButton = nullptr;

    delete this->miscButton;
    this->miscButton = nullptr;

    delete this->scrollWindow;
    this->scrollWindow = nullptr;
}


void ChoiceWindow::setHeight(int height) {
    Layout *layout = Globals::layout;
    int top = layout->windowTopInset;
    this->height = top + height;

    TouchButton *button = this->leftButton;
    int yBase = Globals::h / 2 - height / 2;
    this->y = yBase - top;

    if (this->hasButtons == 0) {
        if (button == nullptr) return;
        button->setPosition(this->x + this->width / 2, height + yBase - this->padding, 0x24);
        return;
    }

    if (button != nullptr) {
        button->setPosition(this->x + this->width / 2 - layout->field_0x4c / 2,
                            height + yBase - this->padding, 0x22);
    }

    button = this->rightButton;
    if (button == nullptr) return;

    layout = Globals::layout;
    button->setPosition(this->x + this->width / 2 + layout->field_0x4c / 2,
                        this->y + this->height - this->padding, 0x21);
}


// Medal artwork (original .rodata @ 0x1fb888 and 0x202784).
static const int MEDAL_TIER_IMAGES_DLC[4] = {8045, 8035, 2415, 2413};
static const int MEDAL_TIER_IMAGES[4] = {2416, 2414, 2415, 2413};
static const int MEDAL_IMAGES[45] = {
    2376, 2377, 2378, 2379, 2380, 2381, 2382, 2383, 2384, 2385, 2386, 2387, 2388, 2389, 2390,
    2391, 2392, 2393, 2394, 2395, 2396, 2397, 2398, 2399, 2400, 2401, 2402, 2403, 2404, 2405,
    2406, 2407, 2408, 2409, 2410, 2411, 8036, 8037, 8038, 8039, 8040, 8041, 8042, 8043, 8044,
};

void ChoiceWindow::setMedal(int medal, int count) {
    this->medalText = *Globals::gameText->getText(medal + 1507);
    String text = String("\n\n\n\n\n\n") +
                  Globals::status->replaceHash(*Globals::gameText->getText(medal + 1552),
                                               String(String(Globals::achievements->getValue(medal, count)), false)) +
                  String("\n\n");
    set(*Globals::gameText->getText(353), text, false);
    this->scrollWindow->setTextCentered(true);

    // NOTE: the original tests the tier (not the medal index) against the first DLC medal
    // here, unlike draw(); the tier is always < 36 in practice.
    if (count >= 36)
        Globals::Canvas->Image2DCreate((unsigned short) MEDAL_TIER_IMAGES_DLC[count], this->medalImage);
    else
        Globals::Canvas->Image2DCreate((unsigned short) MEDAL_TIER_IMAGES[count], this->medalImage);
    Globals::Canvas->Image2DCreate((unsigned short) MEDAL_IMAGES[medal], this->medalBgImage);
    this->count = count;
    this->medal = medal;
    this->buttonsVisible = 1;
}


ChoiceWindow::ChoiceWindow() {
    Layout *config = Globals::layout;
    int width = config->field_0x264;
    this->width = width;
    this->padding = config->field_0x268;
    this->padding2 = config->field_0x26c;
    this->padding3 = config->field_0x270;
    this->padding4 = config->field_0x274;

    int screenWidth = Globals::w;
    this->leftButton = nullptr;
    this->rightButton = nullptr;
    this->miscButton = nullptr;
    this->scrollWindow = nullptr;
    this->medalImage = -1;
    this->medalBgImage = -1;
    this->buttonsVisible = 1;
    this->x = screenWidth / 2 - width / 2;
}


void ChoiceWindow::set(String const &title, String const &message, bool hasButtons,
                       String const &left, String const &right, String const &single,
                       int y, int width) {
    this->hasButtons = hasButtons;
    // The standard yes/no pair keeps the default width; custom buttons get a wider window.
    bool defaultButtons = Globals::gameText->getText(134)->Compare(left) == 0 &&
                          Globals::gameText->getText(135)->Compare(right) == 0;
    if (width == -1)
        width = Globals::layout->field_0x264;
    if (!defaultButtons)
        width = (int) ((float) width * 1.2f);
    this->width = width;
    this->x = Globals::w / 2 - width / 2;
    Globals::sound->play(126, nullptr, nullptr, 0.0f);
    this->title = title;

    delete this->scrollWindow;
    this->scrollWindow = nullptr;
    Array<String *> *lines = new Array<String *>();
    Globals::globals->getLineArray((unsigned int) (uintptr_t) Globals::font, message,
                                   this->width - Globals::layout->field_0x4c * 2 - Globals::layout->field_0x48,
                                   lines);
    int contentHeight = lines->size() * Globals::layout->field_0x4 + Globals::layout->windowTopInset +
                        Globals::layout->field_0x4c * 2 + this->padding * 2 +
                        Globals::layout->field_0x30_rowHeight;
    unsigned int height = Globals::layout->field_0x278;
    if (height > (unsigned int) contentHeight)
        height = contentHeight;
    this->height = height;
    if (y == -1)
        y = Globals::h / 2 - this->height / 2;
    this->y = y;

    this->scrollWindow = new ScrollTouchWindow(this->x, Globals::layout->windowTopInset + y, this->width,
                                               this->height - this->padding * 2 - Globals::layout->windowTopInset -
                                               Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x2bc,
                                               false);
    this->scrollWindow->setText(title, message);
    ArrayReleaseClasses(*lines);
    delete lines;

    delete this->leftButton;
    this->leftButton = nullptr;
    delete this->rightButton;
    this->rightButton = nullptr;
    if (hasButtons) {
        float buttonWidth = (float) Globals::layout->field_0x40 * 1.5f;
        if (defaultButtons)
            buttonWidth = (float) Globals::layout->field_0x40;
        this->leftButton = new TouchButton(left, 0, this->x + this->width / 2 - Globals::layout->field_0x4c / 2,
                                           this->y + this->height - this->padding, (int) buttonWidth, 34, 4);
        this->rightButton = new TouchButton(right, 0, this->x + this->width / 2 + Globals::layout->field_0x4c / 2,
                                            this->y + this->height - this->padding, (int) buttonWidth, 33, 4);
    } else {
        this->leftButton = new TouchButton(single, 0, this->x + this->width / 2,
                                           this->y + this->height - this->padding, Globals::layout->field_0x40,
                                           36, 4);
    }
    this->buttonsVisible = 1;
}


void ChoiceWindow::setMiscButton(String const &text) {
    int buttonWidth;
    if (this->leftButton != nullptr && this->rightButton != nullptr)
        buttonWidth = this->leftButton->getWidth() + this->rightButton->getWidth() + this->padding;
    else
        buttonWidth = this->padding + Globals::layout->field_0x40 * 2;

    this->miscButton = new TouchButton(text, 0, this->x + this->width / 2 - Globals::layout->field_0x4c / 2,
                                       this->y + this->height - this->padding, buttonWidth, 34, 4);
    setHeight(this->height - Globals::layout->windowTopInset + this->padding * 2 +
              Globals::layout->field_0x30_rowHeight);
    this->miscButton->setPosition(this->x + this->width / 2, this->y + this->height - this->padding, 36);
    if (this->leftButton != nullptr)
        this->leftButton->translate(0, -(Globals::layout->field_0x30_rowHeight + this->padding));
    if (this->rightButton != nullptr)
        this->rightButton->translate(0, -(Globals::layout->field_0x30_rowHeight + this->padding));
    this->scrollWindow->setYPosition(Globals::h / 2 - this->height / 2 + Globals::layout->windowTopInset);
}


// Medal tier colours and credit rewards (original .rodata @ 0x1fb8a8, 0x1fb8b8, 0x1fb848).
static const unsigned int MEDAL_TIER_COLORS_DLC[4] = {0xfa792160, 0xfa7921ff, 0xffffffff, 0xce8258ff};
static const unsigned int MEDAL_TIER_COLORS[4] = {0x2198ff2f, 0xfad10eff, 0xffffffff, 0xce8258ff};
static const int MEDAL_TIER_CREDITS[4] = {0, 5000, 2500, 1000};

void ChoiceWindow::draw() {
    Globals::layout->drawMask();
    Globals::layout->drawBox(7, this->x, this->y, this->width, this->height, String(this->title, false));
    Globals::Canvas->SetColor(0xffffffff);

    if (this->medalImage != (unsigned int) -1) {
        Globals::Canvas->DrawImage2D(this->medalImage, this->x + (this->width >> 1), this->y + this->padding2 + 1,
                                     17, 68);
        if (this->medal >= 36)
            Globals::Canvas->SetColor(MEDAL_TIER_COLORS_DLC[this->count]);
        else
            Globals::Canvas->SetColor(MEDAL_TIER_COLORS[this->count]);
        Globals::Canvas->DrawImage2D(this->medalBgImage, this->x + (this->width >> 1), this->y + this->padding2,
                                     17, 68);

        if (!Globals::status->hardCoreMode() && !Globals::achievements->isEliteMedal(this->medal)) {
            String credits = String("+ ") + Globals::layout->formatCredits(MEDAL_TIER_CREDITS[this->count]);
            PaintCanvas *canvas = Globals::Canvas;
            unsigned int font = (unsigned int) (uintptr_t) Globals::font;
            canvas->DrawString(font, credits,
                               this->x + (this->width >> 1) - canvas->GetTextWidth(font, credits) / 2,
                               this->y + this->height - this->padding3, false);
        }

        Globals::Canvas->SetColor(0xff8000ff);
        PaintCanvas *canvas = Globals::Canvas;
        unsigned int font = (unsigned int) (uintptr_t) Globals::font;
        canvas->DrawString(font, this->medalText,
                           this->x + (this->width >> 1) - canvas->GetTextWidth(font, this->medalText) / 2,
                           this->y + this->padding4, false);
    }

    this->scrollWindow->draw();

    if (this->buttonsVisible) {
        if (this->leftButton != nullptr)
            this->leftButton->draw();
        if (this->rightButton != nullptr)
            this->rightButton->draw();
        // Remember the button positions for the touch/keyboard navigation.
        if (this->rightButton != nullptr) {
            Globals::other_buttons_x[2] = (int) this->rightButton->getPosition().x;
            Globals::other_buttons_y[2] = (int) this->rightButton->getPosition().y;
        }
        if (this->leftButton != nullptr) {
            Globals::other_buttons_x[3] = (int) this->leftButton->getPosition().x;
            Globals::other_buttons_y[3] = (int) this->leftButton->getPosition().y;
            if (this->rightButton == nullptr) {
                Globals::other_buttons_x[2] = (int) this->leftButton->getPosition().x;
                Globals::other_buttons_y[2] = (int) this->leftButton->getPosition().y;
            }
        }
        Globals::is_choice_window_visible = 1;
        if (this->miscButton != nullptr)
            this->miscButton->draw();
    }
}

void ChoiceWindow::left() {
}

void ChoiceWindow::right() {
}

int ChoiceWindow::fire() {
    return 0;
}
