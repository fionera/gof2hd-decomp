#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/ui/ScrollTouchWindow.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/mission/Achievements.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/ui/TouchButton.h"
#include "gof2/engine/render/PaintCanvas.h"
// NOTE: the foreign Layout object is read by byte offset via F<int>(layout, ...) -- its
// header (gof2/Layout.h) is not included here because Layout is not part of this batch and
// its full layout is not modelled.

// ---- foreign helpers (defined in the engine; linked elsewhere) -----------------------
extern "C" {
void *ScrollTouchWindow_dtor(void *self);
void Array_StringPtr_ctor(void *self);
void Globals_getLineArray(void *self, void *font, String const &text, int width, void *array);
void ScrollTouchWindow_ctor(void *self, int x, int y, int width, int height, bool centered);
void ArrayReleaseClasses_StringPtr(void *self);
void *Array_StringPtr_dtor(void *self);
void ScrollTouchWindow_setPosition(void *self, int y);
void Layout_formatCredits(String *out, void *layout, int value);
void TouchButton_getPosition(float *out, void *self);
// Status::replaceHash(String pattern, String value) -> String
void Status_replaceHash(String *out, void *status, String const &pattern, String const &value);
}

int ChoiceWindow::OnTouchMove(int x, int y)
{
    if (this->buttonsVisible != 0) {
        if (this->leftButton != 0) ((TouchButton *)(this->leftButton))->OnTouchMove(x, y);
        if (this->rightButton != 0) ((TouchButton *)(this->rightButton))->OnTouchMove(x, y);
        if (this->miscButton != 0) ((TouchButton *)(this->miscButton))->OnTouchMove(x, y);
    }
    if (this->scrollWindow != 0) ((ScrollTouchWindow *)(this->scrollWindow))->OnTouchMove(x, y);
    return 0;
}

uint8_t ChoiceWindow::hasChoice()
{
    return this->hasButtons;
}

__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth;

void ChoiceWindow::setWidth(int width)
{
    this->width = width;
    this->x = *g_ChoiceWindow_screenWidth / 2 - width / 2;
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146954;

void ChoiceWindow::set(String const &a, String const &b)
{
    void *gameText = *g_ChoiceWindow_gameText_146954;
    String *title = (String *)((GameText *)(gameText))->getText(0x186);
    String *left = (String *)((GameText *)(gameText))->getText(0x86);
    String *center = (String *)((GameText *)(gameText))->getText(0x87);
    set(*title, a, false, *left, *center, b, -1, -1);
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146d5c;

void ChoiceWindow::set(String const &text, bool flag)
{
    String *title = (String *)((GameText *)(*g_ChoiceWindow_gameText_146d5c))->getText(0x186);
    set(*title, text, flag);
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468f4;

void ChoiceWindow::set(String const &title, String const &message, bool flag)
{
    void *gameText = *g_ChoiceWindow_gameText_1468f4;
    String *left = (String *)((GameText *)(gameText))->getText(0x86);
    String *center = (String *)((GameText *)(gameText))->getText(0x87);
    String *right = (String *)((GameText *)(gameText))->getText(0x20c);
    set(title, message, flag, *left, *center, *right, -1, -1);
}

int ChoiceWindow::OnTouchEnd(int x, int y)
{
    if (this->buttonsVisible != 0) {
        if (this->leftButton != 0 && ((TouchButton *)(this->leftButton))->OnTouchEnd(x, y) != 0)
            return 0;
        if (this->rightButton != 0 && ((TouchButton *)(this->rightButton))->OnTouchEnd(x, y) != 0)
            return 1;
        if (this->miscButton != 0 && ((TouchButton *)(this->miscButton))->OnTouchEnd(x, y) != 0)
            return 2;
    }
    if (this->scrollWindow != 0) ((ScrollTouchWindow *)(this->scrollWindow))->OnTouchEnd(x, y);
    return -1;
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468c8;

void ChoiceWindow::set(String const &text)
{
    String *title = (String *)((GameText *)(*g_ChoiceWindow_gameText_1468c8))->getText(0x186);
    set(*title, text, false);
}

void ChoiceWindow::update(int dt)
{
    ((ScrollTouchWindow *)(this->scrollWindow))->update(dt);
}

void ChoiceWindow::removeButtons()
{
    this->buttonsVisible = 0;
}

int ChoiceWindow::OnTouchBegin(int x, int y)
{
    if (this->buttonsVisible != 0) {
        if (this->leftButton != 0) ((TouchButton *)(this->leftButton))->OnTouchBegin(x, y);
        if (this->rightButton != 0) ((TouchButton *)(this->rightButton))->OnTouchBegin(x, y);
        if (this->miscButton != 0) ((TouchButton *)(this->miscButton))->OnTouchBegin(x, y);
    }
    if (this->scrollWindow != 0) ((ScrollTouchWindow *)(this->scrollWindow))->OnTouchBegin(x, y);
    return 0;
}

ChoiceWindow::~ChoiceWindow()
{
    if (this->leftButton != 0) { ((TouchButton *)(this->leftButton))->dtor(); operator delete(this->leftButton); }
    this->leftButton = 0;

    if (this->rightButton != 0) { ((TouchButton *)(this->rightButton))->dtor(); operator delete(this->rightButton); }
    this->rightButton = 0;

    if (this->miscButton != 0) { ((TouchButton *)(this->miscButton))->dtor(); operator delete(this->miscButton); }
    this->miscButton = 0;

    if (this->scrollWindow != 0) operator delete(ScrollTouchWindow_dtor(this->scrollWindow));
    this->scrollWindow = 0;

    this->medalText.~String();
    this->title.~String();
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_146dc4;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_146dc4;

void ChoiceWindow::setHeight(int height)
{
    void *layout = *g_ChoiceWindow_layout_146dc4;
    int top = F<int>(layout, 0x8);
    this->height = top + height;

    void *button = this->leftButton;
    int yBase = *g_ChoiceWindow_screenHeight_146dc4 / 2 - height / 2;
    this->y = yBase - top;

    if (this->hasButtons == 0) {
        if (button == 0) return;
        ((TouchButton *)(button))->setPosition(this->x + this->width / 2, height + yBase - this->padding, 0x24);
        return;
    }

    if (button != 0) {
        ((TouchButton *)(button))->setPosition(this->x + this->width / 2 - F<int>(layout, 0x4c) / 2, height + yBase - this->padding, 0x22);
    }

    button = this->rightButton;
    if (button == 0) return;

    layout = *g_ChoiceWindow_layout_146dc4;
    ((TouchButton *)(button))->setPosition(this->x + this->width / 2 + F<int>(layout, 0x4c) / 2, this->y + this->height - this->padding, 0x21);
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146e8c;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_replaceHash_146e8c;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_achievements_146e8c;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_canvas_146e8c;
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalImagesLow_146e8c[];
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalImagesHigh_146e8c[];
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalImages_146e8c[];

void ChoiceWindow::setMedal(int medal, int count)
{
    void *gameText = *g_ChoiceWindow_gameText_146e8c;
    this->medalText = *(String *)((GameText *)(gameText))->getText(medal + 0x5e3);

    String pattern = *(String *)((GameText *)(gameText))->getText(medal + 0x610);
    int value = ((Achievements *)(*g_ChoiceWindow_achievements_146e8c))->getValue(medal, count);
    String number;
    {
        // String(int) -> decimal text; the engine's int ctor is not modelled, so
        // build the decimal text directly into the value String.
        int v = value;
        char buf[16];
        int n = 0;
        bool neg = v < 0;
        unsigned uv = neg ? (unsigned)(-v) : (unsigned)v;
        do { buf[n++] = char('0' + uv % 10); uv /= 10; } while (uv);
        if (neg) buf[n++] = '-';
        while (n--) number.s.push_back((char16_t)(unsigned char)buf[n]);
    }

    String replaced;
    Status_replaceHash(&replaced, *g_ChoiceWindow_replaceHash_146e8c, pattern, number);
    String finalText = replaced;  // prefix "" + replaced + suffix "" == replaced

    set(*(String *)((GameText *)(gameText))->getText(0x161), finalText, false);
    ((ScrollTouchWindow *)(this->scrollWindow))->setTextCentered(true);

    void *canvas = *g_ChoiceWindow_canvas_146e8c;
    if (count < 0x24) {
        ((PaintCanvas *)canvas)->Image2DCreate(F<unsigned short>(g_ChoiceWindow_medalImagesLow_146e8c, count * 4),
                                  (unsigned int *)&this->medalImage);
    } else {
        ((PaintCanvas *)canvas)->Image2DCreate(F<unsigned short>(g_ChoiceWindow_medalImagesHigh_146e8c, count * 4),
                                  (unsigned int *)&this->medalImage);
    }
    ((PaintCanvas *)*g_ChoiceWindow_canvas_146e8c)->Image2DCreate(
                              F<unsigned short>(g_ChoiceWindow_medalImages_146e8c, medal * 4),
                              (unsigned int *)&this->medalBgImage);

    this->count = count;
    this->medal = medal;
    this->buttonsVisible = 1;
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_config_1467ec;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth_1467ec;

ChoiceWindow::ChoiceWindow()
{
    new (&this->title) String();
    new (&this->medalText) String();

    void *config = *g_ChoiceWindow_config_1467ec;
    int width = F<int>(config, 0x264);
    this->width = width;
    this->padding = F<int>(config, 0x268);
    this->field_0x4c = F<int>(config, 0x26c);
    this->field_0x50 = F<int>(config, 0x270);
    this->field_0x54 = F<int>(config, 0x274);

    int screenWidth = *g_ChoiceWindow_screenWidth_1467ec;
    this->leftButton = 0;
    this->rightButton = 0;
    this->miscButton = 0;
    this->scrollWindow = 0;
    this->medalImage = -1;
    this->medalBgImage = -1;
    this->buttonsVisible = 1;
    this->x = screenWidth / 2 - width / 2;
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1469b0;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_defaultConfig_1469b0;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth_1469b0;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_sound_1469b0;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_1469b0;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_lineFont_1469b0;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_globals_1469b0;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_1469b0;

void ChoiceWindow::set(String const &title, String const &message, bool hasButtons,
                       String const &left, String const &right, String const &single,
                       int y, int width)
{
    this->hasButtons = hasButtons;

    void *gameText = *g_ChoiceWindow_gameText_1469b0;
    bool defaultButtons = false;
    if (((String *)((GameText *)(gameText))->getText(0x86))->Compare_str((String *)&(left)) == 0)
        defaultButtons = ((String *)((GameText *)(gameText))->getText(0x87))->Compare_str((String *)&(right)) == 0;

    if (width == -1)
        width = F<int>(*g_ChoiceWindow_defaultConfig_1469b0, 0x264);
    if (!defaultButtons)
        width = (int)((float)width * 0.85f);

    this->width = width;
    this->x = *g_ChoiceWindow_screenWidth_1469b0 / 2 - width / 2;
    ((FModSound *)(*g_ChoiceWindow_sound_1469b0))->play(0x7e, 0, 0, 0.0f);
    this->title = title;

    if (this->scrollWindow != 0) operator delete(ScrollTouchWindow_dtor(this->scrollWindow));
    this->scrollWindow = 0;

    Array<String *> *lines = new Array<String *>();

    void *layout = *g_ChoiceWindow_layout_1469b0;
    Globals_getLineArray(*g_ChoiceWindow_globals_1469b0, *g_ChoiceWindow_lineFont_1469b0, message,
                         (this->width - F<int>(layout, 0x4c) * 2) - F<int>(layout, 0x48),
                         lines);

    int contentHeight = F<int>(layout, 0x4) * (int)lines->size() + F<int>(layout, 0x8) +
                        F<int>(layout, 0x4c) * 2 + this->padding * 2 +
                        F<int>(layout, 0x30);
    int maxHeight = F<int>(layout, 0x278);
    if ((unsigned)contentHeight < (unsigned)maxHeight)
        maxHeight = contentHeight;
    this->height = maxHeight;

    if (y == -1)
        y = *g_ChoiceWindow_screenHeight_1469b0 / 2 - maxHeight / 2;
    this->y = y;

    void *scroll = operator new(0x24);
    ScrollTouchWindow_ctor(scroll, this->x, F<int>(layout, 0x8) + y,
                           this->width,
                           (((maxHeight - this->padding * 2) - F<int>(layout, 0x8)) -
                            F<int>(layout, 0x30)) + F<int>(layout, 0x2bc),
                           false);
    this->scrollWindow = scroll;

    ((ScrollTouchWindow *)(scroll))->setText(title, message);

    ArrayReleaseClasses_StringPtr(lines);
    operator delete(Array_StringPtr_dtor(lines));

    if (this->leftButton != 0) { ((TouchButton *)(this->leftButton))->dtor(); operator delete(this->leftButton); }
    this->leftButton = 0;

    if (this->rightButton != 0) { ((TouchButton *)(this->rightButton))->dtor(); operator delete(this->rightButton); }
    this->rightButton = 0;

    if (hasButtons) {
        int baseWidth = F<int>(layout, 0x40);
        float scaled = (float)baseWidth * 1.5f;
        if (defaultButtons) scaled = (float)baseWidth;
        int buttonWidth = (int)scaled;

        void *button = operator new(0xc8);
        ((TouchButton *)(button))->ctor((String *)&left, 0, this->x + this->width / 2 - F<int>(layout, 0x4c) / 2, this->y + this->height - this->padding, buttonWidth, 0x22, 4);
        this->leftButton = button;

        button = operator new(0xc8);
        ((TouchButton *)(button))->ctor((String *)&right, 0, this->x + this->width / 2 + F<int>(layout, 0x4c) / 2, this->y + this->height - this->padding, buttonWidth, 0x21, 4);
        this->rightButton = button;
    } else {
        void *button = operator new(0xc8);
        ((TouchButton *)(button))->ctor((String *)&single, 0, this->x + this->width / 2, this->y + this->height - this->padding, F<int>(layout, 0x40), 0x24, 4);
        this->rightButton = button;
    }

    this->buttonsVisible = 1;
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_a;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_b;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_c;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_147068;

void ChoiceWindow::setMiscButton(String const &text)
{
    void *layout;
    int padding = this->padding;
    int buttonWidth;

    if (this->leftButton == 0 || this->rightButton == 0) {
        layout = *g_ChoiceWindow_layout_147068_b;
        buttonWidth = padding + F<int>(layout, 0x40) * 2;
    } else {
        int leftWidth = ((TouchButton *)(this->leftButton))->getWidth();
        int rightWidth = ((TouchButton *)(this->rightButton))->getWidth();
        layout = *g_ChoiceWindow_layout_147068_a;
        buttonWidth = leftWidth + rightWidth + padding;
    }

    void *button = operator new(0xc8);
    ((TouchButton *)(button))->ctor((String *)&text, 0, this->x + this->width / 2 - F<int>(layout, 0x4c) / 2, this->y + this->height - padding, buttonWidth, 0x22, 4);

    this->miscButton = button;

    layout = *g_ChoiceWindow_layout_147068_c;
    setHeight((this->height - F<int>(layout, 0x8)) + this->padding * 2 + F<int>(layout, 0x30));

    ((TouchButton *)(this->miscButton))->setPosition(this->x + this->width / 2, this->y + this->height - this->padding, 0x24);

    layout = *g_ChoiceWindow_layout_147068_c;
    int delta = -(F<int>(layout, 0x30) + this->padding);
    if (this->leftButton != 0) ((TouchButton *)(this->leftButton))->translate(0, delta);
    if (this->rightButton != 0) ((TouchButton *)(this->rightButton))->translate(0, delta);

    ScrollTouchWindow_setPosition(this->scrollWindow,
                                  *g_ChoiceWindow_screenHeight_147068 / 2 -
                                      this->height / 2 + F<int>(layout, 0x8));
}

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_1471bc;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_canvas_1471bc;
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalColorsLow_1471bc[];
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalColorsHigh_1471bc[];
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_status_1471bc;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_achievements_1471bc;
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_creditValues_1471bc[];
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_font_1471bc_a;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_font_1471bc_b;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetA_1471bc;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetB_1471bc;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetC_1471bc;
__attribute__((visibility("hidden"))) extern void *g_ChoiceWindow_posTargetD_1471bc;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_dirtyFlag_1471bc;

void ChoiceWindow::draw()
{
    float pos[2];

    void *layout = *g_ChoiceWindow_layout_1471bc;
    ((Layout *)(layout))->drawMask();

    ((Layout *)(layout))->drawBox6(7, this->x, this->y, this->width, this->height, &this->title);

    void *canvas = *g_ChoiceWindow_canvas_1471bc;
    ((PaintCanvas *)canvas)->SetColor(0xffffffff);

    if (this->medalImage != -1) {
        ((PaintCanvas *)canvas)->DrawImage2D(this->medalImage,
                                this->x + (this->width >> 1),
                                this->y + this->field_0x4c + 1, (unsigned char)0x11);
        int color;
        if (this->count < 0x24)
            color = F<int>(g_ChoiceWindow_medalColorsLow_1471bc, this->count * 4);
        else
            color = F<int>(g_ChoiceWindow_medalColorsHigh_1471bc, this->count * 4);
        ((PaintCanvas *)canvas)->SetColor(color);
        ((PaintCanvas *)canvas)->DrawImage2D(this->medalBgImage,
                                this->x + (this->width >> 1),
                                this->y + this->field_0x4c, (unsigned char)0x11);

        if (((Status *)(*g_ChoiceWindow_status_1471bc))->hardCoreMode() == 0 &&
            ((Achievements *)(*g_ChoiceWindow_achievements_1471bc))->isEliteMedal(this->medal) == 0) {
            String credits;
            Layout_formatCredits(&credits, layout,
                                 F<int>(g_ChoiceWindow_creditValues_1471bc, this->count * 4));
            String creditsText = credits;  // prefix "" + credits

            void *font = *g_ChoiceWindow_font_1471bc_a;
            int textWidth = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(unsigned long)font, (void *)&creditsText);
            ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&creditsText,
                                   this->x + (this->width >> 1) - textWidth / 2,
                                   this->y + this->height - this->field_0x50,
                                   false);
        }

        ((PaintCanvas *)canvas)->SetColor(0xffffffff);
        void *font = *g_ChoiceWindow_font_1471bc_b;
        int textWidth = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(unsigned long)font, (void *)&this->medalText);
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&this->medalText,
                               this->x + (this->width >> 1) - textWidth / 2,
                               this->y + this->field_0x54, false);
    }

    ((ScrollTouchWindow *)(this->scrollWindow))->draw();

    if (this->buttonsVisible != 0) {
        if (this->leftButton != 0) ((TouchButton *)(this->leftButton))->draw();

        if (this->rightButton != 0) {
            ((TouchButton *)(this->rightButton))->draw();
            if (this->rightButton != 0) {
                TouchButton_getPosition(pos, this->rightButton);
                F<int>(g_ChoiceWindow_posTargetA_1471bc, 0x8) = (int)pos[0];
                TouchButton_getPosition(pos, this->rightButton);
                F<int>(g_ChoiceWindow_posTargetB_1471bc, 0x8) = (int)pos[1];
            }
        }

        if (this->leftButton != 0) {
            TouchButton_getPosition(pos, this->leftButton);
            F<int>(g_ChoiceWindow_posTargetC_1471bc, 0xc) = (int)pos[0];
            TouchButton_getPosition(pos, this->leftButton);
            F<int>(g_ChoiceWindow_posTargetD_1471bc, 0xc) = (int)pos[1];

            if (this->rightButton == 0) {
                TouchButton_getPosition(pos, this->leftButton);
                F<int>(g_ChoiceWindow_posTargetC_1471bc, 0x8) = (int)pos[0];
                TouchButton_getPosition(pos, this->leftButton);
                F<int>(g_ChoiceWindow_posTargetD_1471bc, 0x8) = (int)pos[1];
            }
        }

        *g_ChoiceWindow_dirtyFlag_1471bc = 1;
        if (this->miscButton != 0) ((TouchButton *)(this->miscButton))->draw();
    }
}

// Invoked when the left choice button is pressed. The decision result is read
// back through hasChoice(); this callback itself performs no work in the
// shipped build (the dialog merely records the press via the button state).
void ChoiceWindow::left() {
}

// Invoked when the right choice button is pressed; an intentional no-op, the
// caller queries hasChoice() / the button state for the actual decision.
void ChoiceWindow::right() {
}

// ---- engine-name convenience setters / forwarders -----------------------------------
// These mirror the way the shipped code drives a ChoiceWindow from the hangar / lounge
// screens: each delegates to one of the recovered set() overloads (or, for the button
// labels, retitles the existing TouchButtons) so the window keeps a single layout path.

// setMsg(text, hasButtons): show a default-titled message; equivalent to set(text, flag).
void ChoiceWindow::setMsg(String const &text, bool hasButtons)
{
    set(text, hasButtons);
}

// setText(title, body): show an explicit title + body with the standard two buttons.
void ChoiceWindow::setText(String const &title, String const &body)
{
    set(title, body, true);
}

// setButtonText(left, right): override the labels on the two choice buttons in place,
// without rebuilding the window (the buttons are created by set()).
void ChoiceWindow::setButtonText(String const &left, String const &right)
{
    if (this->leftButton != 0)
        ((TouchButton *)(this->leftButton))->setText((String *)&left);
    if (this->rightButton != 0)
        ((TouchButton *)(this->rightButton))->setText((String *)&right);
}

// touch_end(x, y): engine-name alias for the touch-release handler.
int ChoiceWindow::touch_end(int x, int y)
{
    return this->OnTouchEnd(x, y);
}

// ===========================================================================
// extern "C" interop shims.
//
// Other modules (MenuTouchWindow / DialogueWindow / SpaceLounge / StarMap /
// MissionsWindow / HangarWindow) reach into ChoiceWindow through `extern "C"`
// veneers the decompiler peeled out of their bodies (placement-new'd dialogs,
// deleting-destructor chains, and the touch/update dispatch). Each forwards to
// the real ChoiceWindow member. Construct/destruct shims return `this` so the
// callers' `operator delete(ChoiceWindow_dtor(p))` chains stay byte-faithful.
// ===========================================================================

// In-place construction of a ChoiceWindow at an already-allocated slot.
extern "C" void *ChoiceWindow_ctor(void *self) {
    new (self) ChoiceWindow();
    return self;
}

// Base destructor (returns `this`); the caller tail-calls operator delete.
extern "C" void *ChoiceWindow_dtor(void *p) {
    ((ChoiceWindow *)p)->~ChoiceWindow();
    return p;
}
extern "C" void *_mtw_ChoiceWindow_dtor(void *p) {
    ((ChoiceWindow *)p)->~ChoiceWindow();
    return p;
}
extern "C" void *_mw_ChoiceWindow_dtor(void *w) {
    ((ChoiceWindow *)w)->~ChoiceWindow();
    return w;
}

// Per-frame update of the embedded scroll window.
extern "C" void _mtw_ChoiceWindow_update(void *cw) {
    ((ChoiceWindow *)cw)->update(0);
}

// Touch dispatch from the menu's master handler. The veneers carry a single
// coordinate (the menu's primary touch axis, which lands in the handler's first
// argument); the orthogonal coordinate is not threaded through the veneer.
extern "C" void _mtw_ChoiceWindow_OnTouchBegin(void *cw, int y) {
    ((ChoiceWindow *)cw)->OnTouchBegin(y, 0);
}
extern "C" int _mtw_ChoiceWindow_OnTouchEnd(void *cw, int y) {
    return ((ChoiceWindow *)cw)->OnTouchEnd(y, 0);
}
extern "C" void _mtw_ChoiceWindow_OnTouchMove(void *cw, int y) {
    ((ChoiceWindow *)cw)->OnTouchMove(y, 0);
}
