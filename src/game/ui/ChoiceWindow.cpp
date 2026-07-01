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

int ChoiceWindow::OnTouchMove(int x, int y) {
    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->OnTouchMove(x, y);
        if (this->rightButton != nullptr) this->rightButton->OnTouchMove(x, y);
        if (this->miscButton != nullptr) this->miscButton->OnTouchMove(x, y);
    }
    if (this->scrollWindow != nullptr) this->scrollWindow->OnTouchMove(x, y);
    return 0;
}

uint8_t ChoiceWindow::hasChoice() {
    return this->hasButtons;
}


void ChoiceWindow::setWidth(int width) {
    this->width = width;
    this->x = Globals::w / 2 - width / 2;
}


void ChoiceWindow::set(String const &a, String const &b) {
    GameText *gameText = Globals::gameText;
    String *title = gameText->getText(0x186);
    String *left = gameText->getText(0x86);
    String *center = gameText->getText(0x87);
    set(*title, a, false, *left, *center, b, -1, -1);
}


void ChoiceWindow::set(String const &text, bool flag) {
    String *title = (Globals::gameText)->getText(0x186);
    set(*title, text, flag);
}


void ChoiceWindow::set(String const &title, String const &message, bool flag) {
    GameText *gameText = Globals::gameText;
    String *left = gameText->getText(0x86);
    String *center = gameText->getText(0x87);
    String *right = gameText->getText(0x20c);
    set(title, message, flag, *left, *center, *right, -1, -1);
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

int ChoiceWindow::OnTouchBegin(int x, int y) {
    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->OnTouchBegin(x, y);
        if (this->rightButton != nullptr) this->rightButton->OnTouchBegin(x, y);
        if (this->miscButton != nullptr) this->miscButton->OnTouchBegin(x, y);
    }
    if (this->scrollWindow != nullptr) this->scrollWindow->OnTouchBegin(x, y);
    return 0;
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

static uint32_t g_ChoiceWindow_medalImagesLow_146e8c[64];
static uint32_t g_ChoiceWindow_medalImagesHigh_146e8c[64];
static uint32_t g_ChoiceWindow_medalImages_146e8c[64];

void ChoiceWindow::setMedal(int medal, int count) {
    GameText *gameText = Globals::gameText;
    this->medalText = *gameText->getText(medal + 0x5e3);

    String pattern = *gameText->getText(medal + 0x610);
    int value = Globals::achievements->getValue(medal, count);
    String number;
    {
        int v = value;
        char buf[16];
        int n = 0;
        bool neg = v < 0;
        unsigned uv = neg ? (unsigned) (-v) : (unsigned) v;
        do {
            buf[n++] = char('0' + uv % 10);
            uv /= 10;
        } while (uv);
        if (neg) buf[n++] = '-';
        while (n--) {
            int _nl = number.length + 1;
            unsigned short *_nd = new unsigned short[_nl + 1];
            for (int _i = 0; _i < number.length; _i++) _nd[_i] = number.data[_i];
            _nd[number.length] = (unsigned short) ((char16_t) (unsigned char) buf[n]);
            _nd[_nl] = 0;
            if (number.data) delete[] number.data;
            number.data = _nd;
            number.length = _nl;
        }
    }

    String finalText = Globals::status->replaceHash(pattern, number);

    set(*gameText->getText(0x161), finalText, false);
    this->scrollWindow->setTextCentered(true);

    PaintCanvas *canvas = Globals::Canvas;
    unsigned int medalImageHandle;
    if (count < 0x24) {
        canvas->Image2DCreate((unsigned short) g_ChoiceWindow_medalImagesLow_146e8c[count],
                              medalImageHandle);
    } else {
        canvas->Image2DCreate((unsigned short) g_ChoiceWindow_medalImagesHigh_146e8c[count],
                              medalImageHandle);
    }
    this->medalImage = medalImageHandle;
    unsigned int medalBgImageHandle;
    canvas->Image2DCreate((unsigned short) g_ChoiceWindow_medalImages_146e8c[medal],
                          medalBgImageHandle);
    this->medalBgImage = medalBgImageHandle;

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

    GameText *gameText = Globals::gameText;
    bool defaultButtons = false;
    if (gameText->getText(0x86)->Compare_str((String *) &left) == 0)
        defaultButtons = gameText->getText(0x87)->Compare_str((String *) &right) == 0;

    if (width == -1)
        width = (Globals::layout)->field_0x264;
    if (!defaultButtons)
        width = (int) ((float) width * 0.85f);

    this->width = width;
    this->x = Globals::w / 2 - width / 2;
    (Globals::sound)->play(0x7e, nullptr, nullptr, 0.0f);
    this->title = title;

    delete this->scrollWindow;
    this->scrollWindow = nullptr;

    Array<String *> *lines = new Array<String *>();

    Layout *layout = Globals::layout;
    Globals::globals->getLineArray(
        (unsigned int) (uintptr_t) Globals::font,
        message,
        (this->width - layout->field_0x4c * 2) - layout->field_0x48,
        lines);

    int contentHeight = layout->field_0x4 * (int) lines->size() + layout->windowTopInset +
                        layout->field_0x4c * 2 + this->padding * 2 +
                        layout->field_0x30_rowHeight;
    int maxHeight = layout->field_0x278;
    if ((unsigned) contentHeight < (unsigned) maxHeight)
        maxHeight = contentHeight;
    this->height = maxHeight;

    if (y == -1)
        y = Globals::h / 2 - maxHeight / 2;
    this->y = y;

    this->scrollWindow = new ScrollTouchWindow(
        this->x, layout->windowTopInset + y, this->width,
        (((maxHeight - this->padding * 2) - layout->windowTopInset) - layout->field_0x30_rowHeight) +
        layout->field_0x2bc,
        false);

    this->scrollWindow->setText(title, message);

    ArrayReleaseClasses(*lines);
    delete lines;

    delete this->leftButton;
    this->leftButton = nullptr;

    delete this->rightButton;
    this->rightButton = nullptr;

    if (hasButtons) {
        int baseWidth = layout->field_0x40;
        float scaled = (float) baseWidth * 1.5f;
        if (defaultButtons) scaled = (float) baseWidth;
        int buttonWidth = (int) scaled;

        this->leftButton = new TouchButton(left, 0,
                                           this->x + this->width / 2 - layout->field_0x4c / 2,
                                           this->y + this->height - this->padding,
                                           buttonWidth, 0x22, 4);

        this->rightButton = new TouchButton(right, 0,
                                            this->x + this->width / 2 + layout->field_0x4c / 2,
                                            this->y + this->height - this->padding,
                                            buttonWidth, 0x21, 4);
    } else {
        this->rightButton = new TouchButton(single, 0,
                                            this->x + this->width / 2,
                                            this->y + this->height - this->padding,
                                            layout->field_0x40, 0x24, 4);
    }

    this->buttonsVisible = 1;
}


void ChoiceWindow::setMiscButton(String const &text) {
    Layout *layout;
    int padding = this->padding;
    int buttonWidth;

    if (this->leftButton == nullptr || this->rightButton == nullptr) {
        layout = Globals::layout;
        buttonWidth = padding + layout->field_0x40 * 2;
    } else {
        int leftWidth = this->leftButton->getWidth();
        int rightWidth = this->rightButton->getWidth();
        layout = Globals::layout;
        buttonWidth = leftWidth + rightWidth + padding;
    }

    this->miscButton = new TouchButton(text, 0,
                                       this->x + this->width / 2 - layout->field_0x4c / 2,
                                       this->y + this->height - padding, buttonWidth, 0x22, 4);

    layout = Globals::layout;
    setHeight((this->height - layout->windowTopInset) + this->padding * 2 + layout->field_0x30_rowHeight);

    this->miscButton->setPosition(this->x + this->width / 2,
                                  this->y + this->height - this->padding, 0x24);

    layout = Globals::layout;
    int delta = -(layout->field_0x30_rowHeight + this->padding);
    if (this->leftButton != nullptr) this->leftButton->translate(0, delta);
    if (this->rightButton != nullptr) this->rightButton->translate(0, delta);

    this->scrollWindow->setYPosition(
        Globals::h / 2 - this->height / 2 + layout->windowTopInset);
}

static int g_ChoiceWindow_medalColorsLow_1471bc[64];
static int g_ChoiceWindow_medalColorsHigh_1471bc[64];
static int g_ChoiceWindow_creditValues_1471bc[64];

void ChoiceWindow::draw() {
    Vector pos;

    Layout *layout = Globals::layout;
    layout->drawMask();

    layout->drawBox(7, this->x, this->y, this->width, this->height, this->title, 1u);

    PaintCanvas *canvas = Globals::Canvas;
    canvas->SetColor(0xffffffff);

    if (this->medalImage != -1) {
        canvas->DrawImage2D(this->medalImage,
                            this->x + (this->width >> 1),
                            this->y + this->padding2 + 1, (unsigned char) 0x11);
        int color;
        if (this->count < 0x24)
            color = g_ChoiceWindow_medalColorsLow_1471bc[this->count];
        else
            color = g_ChoiceWindow_medalColorsHigh_1471bc[this->count];
        canvas->SetColor(color);
        canvas->DrawImage2D(this->medalBgImage,
                            this->x + (this->width >> 1),
                            this->y + this->padding2, (unsigned char) 0x11);

        if (Globals::status->hardCoreMode() == 0 &&
            Globals::achievements->isEliteMedal(this->medal) == 0) {
            String creditsText = Layout::formatCredits(
                g_ChoiceWindow_creditValues_1471bc[this->count]);

            unsigned int font = (unsigned int) (uintptr_t) Globals::font;
            int textWidth = canvas->GetTextWidth(font, creditsText);
            canvas->DrawString(font, creditsText,
                               this->x + (this->width >> 1) - textWidth / 2,
                               this->y + this->height - this->padding3,
                               false);
        }

        canvas->SetColor(0xffffffff);
        unsigned int font = (unsigned int) (uintptr_t) Globals::font;
        int textWidth = canvas->GetTextWidth(font, this->medalText);
        canvas->DrawString(font, this->medalText,
                           this->x + (this->width >> 1) - textWidth / 2,
                           this->y + this->padding4, false);
    }

    this->scrollWindow->draw();

    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->draw();

        if (this->rightButton != nullptr) {
            this->rightButton->draw();
            pos = this->rightButton->getPosition();
            ((ChoiceWindowButtonPosCache *) Globals::other_buttons_x)->rightOrSingleButtonPos = (int) pos[0];
            pos = this->rightButton->getPosition();
            ((ChoiceWindowButtonPosCache *) Globals::other_buttons_y)->rightOrSingleButtonPos = (int) pos[1];
        }

        if (this->leftButton != nullptr) {
            pos = this->leftButton->getPosition();
            ((ChoiceWindowButtonPosCache *) Globals::other_buttons_x)->leftButtonPos = (int) pos[0];
            pos = this->leftButton->getPosition();
            ((ChoiceWindowButtonPosCache *) Globals::other_buttons_y)->leftButtonPos = (int) pos[1];

            if (this->rightButton == nullptr) {
                pos = this->leftButton->getPosition();
                ((ChoiceWindowButtonPosCache *) Globals::other_buttons_x)->rightOrSingleButtonPos = (int) pos[0];
                pos = this->leftButton->getPosition();
                ((ChoiceWindowButtonPosCache *) Globals::other_buttons_y)->rightOrSingleButtonPos = (int) pos[1];
            }
        }

        Globals::is_choice_window_visible = 1;
        if (this->miscButton != nullptr) this->miscButton->draw();
    }
}

void ChoiceWindow::left() {
}

void ChoiceWindow::right() {
}

int ChoiceWindow::fire() {
    return 0;
}
