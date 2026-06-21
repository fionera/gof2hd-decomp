#include "game/ui/ChoiceWindow.h"
#include "engine/audio/FModSound.h"
#include "game/ui/ScrollTouchWindow.h"
#include "game/mission/Status.h"
#include "game/mission/Achievements.h"
#include "game/core/Globals.h"
#include "engine/core/GameText.h"
#include "game/ui/Layout.h"
#include "game/ui/TouchButton.h"
#include "engine/render/PaintCanvas.h"


int ChoiceWindow::OnTouchMove(int x, int y)
{
    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->OnTouchMove(x, y);
        if (this->rightButton != nullptr) this->rightButton->OnTouchMove(x, y);
        if (this->miscButton != nullptr) this->miscButton->OnTouchMove(x, y);
    }
    if (this->scrollWindow != nullptr) this->scrollWindow->OnTouchMove(x, y);
    return 0;
}

uint8_t ChoiceWindow::hasChoice()
{
    return this->hasButtons;
}

extern int *g_ChoiceWindow_screenWidth;

void ChoiceWindow::setWidth(int width)
{
    this->width = width;
    this->x = *g_ChoiceWindow_screenWidth / 2 - width / 2;
}

extern GameText **g_ChoiceWindow_gameText_146954;

void ChoiceWindow::set(String const &a, String const &b)
{
    GameText *gameText = *g_ChoiceWindow_gameText_146954;
    String *title = gameText->getText(0x186);
    String *left = gameText->getText(0x86);
    String *center = gameText->getText(0x87);
    set(*title, a, false, *left, *center, b, -1, -1);
}

extern GameText **g_ChoiceWindow_gameText_146d5c;

void ChoiceWindow::set(String const &text, bool flag)
{
    String *title = (*g_ChoiceWindow_gameText_146d5c)->getText(0x186);
    set(*title, text, flag);
}

extern GameText **g_ChoiceWindow_gameText_1468f4;

void ChoiceWindow::set(String const &title, String const &message, bool flag)
{
    GameText *gameText = *g_ChoiceWindow_gameText_1468f4;
    String *left = gameText->getText(0x86);
    String *center = gameText->getText(0x87);
    String *right = gameText->getText(0x20c);
    set(title, message, flag, *left, *center, *right, -1, -1);
}

int ChoiceWindow::OnTouchEnd(int x, int y)
{
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

extern GameText **g_ChoiceWindow_gameText_1468c8;

void ChoiceWindow::set(String const &text)
{
    String *title = (*g_ChoiceWindow_gameText_1468c8)->getText(0x186);
    set(*title, text, false);
}

void ChoiceWindow::update(int dt)
{
    this->scrollWindow->update(dt);
}

void ChoiceWindow::removeButtons()
{
    this->buttonsVisible = 0;
}

int ChoiceWindow::OnTouchBegin(int x, int y)
{
    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->OnTouchBegin(x, y);
        if (this->rightButton != nullptr) this->rightButton->OnTouchBegin(x, y);
        if (this->miscButton != nullptr) this->miscButton->OnTouchBegin(x, y);
    }
    if (this->scrollWindow != nullptr) this->scrollWindow->OnTouchBegin(x, y);
    return 0;
}

ChoiceWindow::~ChoiceWindow()
{
    delete this->leftButton;
    this->leftButton = nullptr;

    delete this->rightButton;
    this->rightButton = nullptr;

    delete this->miscButton;
    this->miscButton = nullptr;

    delete this->scrollWindow;
    this->scrollWindow = nullptr;
}

extern Layout **g_ChoiceWindow_layout_146dc4;
extern int *g_ChoiceWindow_screenHeight_146dc4;

void ChoiceWindow::setHeight(int height)
{
    Layout *layout = *g_ChoiceWindow_layout_146dc4;
    int top = layout->field_0x8;
    this->height = top + height;

    TouchButton *button = this->leftButton;
    int yBase = *g_ChoiceWindow_screenHeight_146dc4 / 2 - height / 2;
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

    layout = *g_ChoiceWindow_layout_146dc4;
    button->setPosition(this->x + this->width / 2 + layout->field_0x4c / 2,
                        this->y + this->height - this->padding, 0x21);
}

extern GameText **g_ChoiceWindow_gameText_146e8c;
extern char g_ChoiceWindow_medalImagesLow_146e8c[];
extern char g_ChoiceWindow_medalImagesHigh_146e8c[];
extern char g_ChoiceWindow_medalImages_146e8c[];

void ChoiceWindow::setMedal(int medal, int count)
{
    GameText *gameText = *g_ChoiceWindow_gameText_146e8c;
    this->medalText = *gameText->getText(medal + 0x5e3);

    String pattern = *gameText->getText(medal + 0x610);
    int value = gAchievements->getValue(medal, count);
    String number;
    {
        // Build the decimal text of `value` directly into the String.
        int v = value;
        char buf[16];
        int n = 0;
        bool neg = v < 0;
        unsigned uv = neg ? (unsigned)(-v) : (unsigned)v;
        do { buf[n++] = char('0' + uv % 10); uv /= 10; } while (uv);
        if (neg) buf[n++] = '-';
        while (n--) number.s.push_back((char16_t)(unsigned char)buf[n]);
    }

    String finalText = gStatus->replaceHash(pattern, number);

    set(*gameText->getText(0x161), finalText, false);
    this->scrollWindow->setTextCentered(true);

    PaintCanvas *canvas = gCanvas;
    unsigned int medalImageHandle;
    if (count < 0x24) {
        canvas->Image2DCreate(F<unsigned short>(g_ChoiceWindow_medalImagesLow_146e8c, count * 4),
                              medalImageHandle);
    } else {
        canvas->Image2DCreate(F<unsigned short>(g_ChoiceWindow_medalImagesHigh_146e8c, count * 4),
                              medalImageHandle);
    }
    this->medalImage = medalImageHandle;
    unsigned int medalBgImageHandle;
    canvas->Image2DCreate(F<unsigned short>(g_ChoiceWindow_medalImages_146e8c, medal * 4),
                          medalBgImageHandle);
    this->medalBgImage = medalBgImageHandle;

    this->count = count;
    this->medal = medal;
    this->buttonsVisible = 1;
}

extern void **g_ChoiceWindow_config_1467ec;
extern int *g_ChoiceWindow_screenWidth_1467ec;

ChoiceWindow::ChoiceWindow()
{
    void *config = *g_ChoiceWindow_config_1467ec;
    int width = F<int>(config, 0x264);
    this->width = width;
    this->padding = F<int>(config, 0x268);
    this->padding2 = F<int>(config, 0x26c);
    this->padding3 = F<int>(config, 0x270);
    this->padding4 = F<int>(config, 0x274);

    int screenWidth = *g_ChoiceWindow_screenWidth_1467ec;
    this->leftButton = nullptr;
    this->rightButton = nullptr;
    this->miscButton = nullptr;
    this->scrollWindow = nullptr;
    this->medalImage = -1;
    this->medalBgImage = -1;
    this->buttonsVisible = 1;
    this->x = screenWidth / 2 - width / 2;
}

extern GameText **g_ChoiceWindow_gameText_1469b0;
extern void **g_ChoiceWindow_defaultConfig_1469b0;
extern int *g_ChoiceWindow_screenWidth_1469b0;
extern FModSound **g_ChoiceWindow_sound_1469b0;
extern Layout **g_ChoiceWindow_layout_1469b0;
extern void **g_ChoiceWindow_lineFont_1469b0;
extern int *g_ChoiceWindow_screenHeight_1469b0;

void ChoiceWindow::set(String const &title, String const &message, bool hasButtons,
                       String const &left, String const &right, String const &single,
                       int y, int width)
{
    this->hasButtons = hasButtons;

    GameText *gameText = *g_ChoiceWindow_gameText_1469b0;
    bool defaultButtons = false;
    if (gameText->getText(0x86)->Compare_str((String *)&left) == 0)
        defaultButtons = gameText->getText(0x87)->Compare_str((String *)&right) == 0;

    if (width == -1)
        width = F<int>(*g_ChoiceWindow_defaultConfig_1469b0, 0x264);
    if (!defaultButtons)
        width = (int)((float)width * 0.85f);

    this->width = width;
    this->x = *g_ChoiceWindow_screenWidth_1469b0 / 2 - width / 2;
    (*g_ChoiceWindow_sound_1469b0)->play(0x7e, nullptr, nullptr, 0.0f);
    this->title = title;

    delete this->scrollWindow;
    this->scrollWindow = nullptr;

    Array<String *> *lines = new Array<String *>();

    Layout *layout = *g_ChoiceWindow_layout_1469b0;
    gGlobals->getLineArray(
        static_cast<unsigned int>(reinterpret_cast<std::size_t>(*g_ChoiceWindow_lineFont_1469b0)),
        message,
        (this->width - layout->field_0x4c * 2) - layout->field_0x48,
        lines);

    int contentHeight = layout->field_0x4 * (int)lines->size() + layout->field_0x8 +
                        layout->field_0x4c * 2 + this->padding * 2 +
                        layout->field_0x30;
    int maxHeight = layout->field_0x278;
    if ((unsigned)contentHeight < (unsigned)maxHeight)
        maxHeight = contentHeight;
    this->height = maxHeight;

    if (y == -1)
        y = *g_ChoiceWindow_screenHeight_1469b0 / 2 - maxHeight / 2;
    this->y = y;

    this->scrollWindow = new ScrollTouchWindow(
        this->x, layout->field_0x8 + y, this->width,
        (((maxHeight - this->padding * 2) - layout->field_0x8) - layout->field_0x30) +
            layout->field_0x2bc,
        false);

    this->scrollWindow->setText(title, message);

    for (String *line : *lines) delete line;
    lines->clear();
    delete lines;

    delete this->leftButton;
    this->leftButton = nullptr;

    delete this->rightButton;
    this->rightButton = nullptr;

    if (hasButtons) {
        int baseWidth = layout->field_0x40;
        float scaled = (float)baseWidth * 1.5f;
        if (defaultButtons) scaled = (float)baseWidth;
        int buttonWidth = (int)scaled;

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

extern Layout **g_ChoiceWindow_layout_147068_a;
extern Layout **g_ChoiceWindow_layout_147068_b;
extern Layout **g_ChoiceWindow_layout_147068_c;
extern int *g_ChoiceWindow_screenHeight_147068;

void ChoiceWindow::setMiscButton(String const &text)
{
    Layout *layout;
    int padding = this->padding;
    int buttonWidth;

    if (this->leftButton == nullptr || this->rightButton == nullptr) {
        layout = *g_ChoiceWindow_layout_147068_b;
        buttonWidth = padding + layout->field_0x40 * 2;
    } else {
        int leftWidth = this->leftButton->getWidth();
        int rightWidth = this->rightButton->getWidth();
        layout = *g_ChoiceWindow_layout_147068_a;
        buttonWidth = leftWidth + rightWidth + padding;
    }

    this->miscButton = new TouchButton(text, 0,
                                       this->x + this->width / 2 - layout->field_0x4c / 2,
                                       this->y + this->height - padding, buttonWidth, 0x22, 4);

    layout = *g_ChoiceWindow_layout_147068_c;
    setHeight((this->height - layout->field_0x8) + this->padding * 2 + layout->field_0x30);

    this->miscButton->setPosition(this->x + this->width / 2,
                                  this->y + this->height - this->padding, 0x24);

    layout = *g_ChoiceWindow_layout_147068_c;
    int delta = -(layout->field_0x30 + this->padding);
    if (this->leftButton != nullptr) this->leftButton->translate(0, delta);
    if (this->rightButton != nullptr) this->rightButton->translate(0, delta);

    this->scrollWindow->setYPosition(
        *g_ChoiceWindow_screenHeight_147068 / 2 - this->height / 2 + layout->field_0x8);
}

extern Layout **g_ChoiceWindow_layout_1471bc;
extern char g_ChoiceWindow_medalColorsLow_1471bc[];
extern char g_ChoiceWindow_medalColorsHigh_1471bc[];
extern char g_ChoiceWindow_creditValues_1471bc[];
extern void **g_ChoiceWindow_font_1471bc_a;
extern void **g_ChoiceWindow_font_1471bc_b;
extern void *g_ChoiceWindow_posTargetA_1471bc;
extern void *g_ChoiceWindow_posTargetB_1471bc;
extern void *g_ChoiceWindow_posTargetC_1471bc;
extern void *g_ChoiceWindow_posTargetD_1471bc;
extern int *g_ChoiceWindow_dirtyFlag_1471bc;

void ChoiceWindow::draw()
{
    Vector pos;

    Layout *layout = *g_ChoiceWindow_layout_1471bc;
    layout->drawMask();

    layout->drawBox(7, this->x, this->y, this->width, this->height, this->title, 1u);

    PaintCanvas *canvas = gCanvas;
    canvas->SetColor(0xffffffff);

    if (this->medalImage != -1) {
        canvas->DrawImage2D(this->medalImage,
                            this->x + (this->width >> 1),
                            this->y + this->padding2 + 1, (unsigned char)0x11);
        int color;
        if (this->count < 0x24)
            color = F<int>(g_ChoiceWindow_medalColorsLow_1471bc, this->count * 4);
        else
            color = F<int>(g_ChoiceWindow_medalColorsHigh_1471bc, this->count * 4);
        canvas->SetColor(color);
        canvas->DrawImage2D(this->medalBgImage,
                            this->x + (this->width >> 1),
                            this->y + this->padding2, (unsigned char)0x11);

        if (gStatus->hardCoreMode() == 0 &&
            gAchievements->isEliteMedal(this->medal) == 0) {
            String creditsText = Layout::formatCredits(
                F<int>(g_ChoiceWindow_creditValues_1471bc, this->count * 4));

            void *font = *g_ChoiceWindow_font_1471bc_a;
            int textWidth = canvas->GetTextWidth((unsigned int)(unsigned long)font, creditsText);
            canvas->DrawString((unsigned int)(unsigned long)font, creditsText,
                               this->x + (this->width >> 1) - textWidth / 2,
                               this->y + this->height - this->padding3,
                               false);
        }

        canvas->SetColor(0xffffffff);
        void *font = *g_ChoiceWindow_font_1471bc_b;
        int textWidth = canvas->GetTextWidth((unsigned int)(unsigned long)font, this->medalText);
        canvas->DrawString((unsigned int)(unsigned long)font, this->medalText,
                           this->x + (this->width >> 1) - textWidth / 2,
                           this->y + this->padding4, false);
    }

    this->scrollWindow->draw();

    if (this->buttonsVisible != 0) {
        if (this->leftButton != nullptr) this->leftButton->draw();

        if (this->rightButton != nullptr) {
            this->rightButton->draw();
            pos = this->rightButton->getPosition();
            F<int>(g_ChoiceWindow_posTargetA_1471bc, 0x8) = (int)pos[0];
            pos = this->rightButton->getPosition();
            F<int>(g_ChoiceWindow_posTargetB_1471bc, 0x8) = (int)pos[1];
        }

        if (this->leftButton != nullptr) {
            pos = this->leftButton->getPosition();
            F<int>(g_ChoiceWindow_posTargetC_1471bc, 0xc) = (int)pos[0];
            pos = this->leftButton->getPosition();
            F<int>(g_ChoiceWindow_posTargetD_1471bc, 0xc) = (int)pos[1];

            if (this->rightButton == nullptr) {
                pos = this->leftButton->getPosition();
                F<int>(g_ChoiceWindow_posTargetC_1471bc, 0x8) = (int)pos[0];
                pos = this->leftButton->getPosition();
                F<int>(g_ChoiceWindow_posTargetD_1471bc, 0x8) = (int)pos[1];
            }
        }

        *g_ChoiceWindow_dirtyFlag_1471bc = 1;
        if (this->miscButton != nullptr) this->miscButton->draw();
    }
}

// Invoked when the left choice button is pressed. The decision result is read
// back through hasChoice(); this callback itself performs no work in the shipped
// build (the dialog records the press via the button state).
void ChoiceWindow::left() {
}

// Invoked when the right choice button is pressed; an intentional no-op, the
// caller queries hasChoice() / the button state for the actual decision.
void ChoiceWindow::right() {
}

// Fire callback for the left choice button: an intentional no-op in the shipped
// build (returns 0). The caller reads the decision back via hasChoice() / the
// recorded button state.
int ChoiceWindow::fire()
{
    return 0;
}
