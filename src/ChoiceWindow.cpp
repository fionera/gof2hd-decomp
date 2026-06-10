#include "gof2/ChoiceWindow.h"
#include "gof2/Achievements.h"
#include "gof2/GameText.h"
#include "gof2/Layout.h"
#include "gof2/TouchButton.h"
// NOTE: the foreign Layout object is read by byte offset via F<int>(layout, ...) -- its
// header (gof2/Layout.h) is not included here because Layout is not part of this batch and
// its full layout is not modelled.

// ---- foreign helpers (defined in the engine; linked elsewhere) -----------------------
extern "C" {
void ScrollTouchWindow_OnTouchMove(void *self, int x, int y);
void ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);
void ScrollTouchWindow_update(void *self, int dt);
void ScrollTouchWindow_OnTouchBegin(void *self, int x, int y);
void *ScrollTouchWindow_dtor(void *self);
void ScrollTouchWindow_setTextCentered(void *self, bool centered);
void PaintCanvas_Image2DCreate(void *self, unsigned short image, unsigned int *out);
int String_Compare(String *a, String const &b);
void FModSound_play(void *self, int sound, void *pos, void *vel, float volume);
void Array_StringPtr_ctor(void *self);
void Globals_getLineArray(void *self, void *font, String const &text, int width, void *array);
void ScrollTouchWindow_ctor(void *self, int x, int y, int width, int height, bool centered);
void ScrollTouchWindow_setText(void *self, String const &title, String const &message);
void ArrayReleaseClasses_StringPtr(void *self);
void *Array_StringPtr_dtor(void *self);
void ScrollTouchWindow_setPosition(void *self, int y);
void Layout_drawMask(void *self);
void PaintCanvas_SetColor(void *self, unsigned int color);
void PaintCanvas_DrawImage2D(void *self, int image, int x, int y, int anchor);
int Status_hardCoreMode(void *self);
void Layout_formatCredits(String *out, void *layout, int value);
int PaintCanvas_GetTextWidth(void *self, void *font, String const &text);
void PaintCanvas_DrawString(void *self, void *font, String const &text, int x, int y, bool shadow);
void ScrollTouchWindow_draw(void *self);
void TouchButton_getPosition(float *out, void *self);
void TouchButton_ctor(void *self, String const &text, int value, int x, int y,
                      int width, int anchor, int mode);
// Status::replaceHash(String pattern, String value) -> String
void Status_replaceHash(String *out, void *status, String const &pattern, String const &value);
}


// ---- OnTouchMove_147518.cpp ----
int ChoiceWindow::OnTouchMove(int x, int y)
{
    if (this->field_0x59 != 0) {
        if (this->field_0x10 != 0) ((TouchButton *)(this->field_0x10))->OnTouchMove(x, y);
        if (this->field_0x14 != 0) ((TouchButton *)(this->field_0x14))->OnTouchMove(x, y);
        if (this->field_0x18 != 0) ((TouchButton *)(this->field_0x18))->OnTouchMove(x, y);
    }
    if (this->field_0x1c != 0) ScrollTouchWindow_OnTouchMove(this->field_0x1c, x, y);
    return 0;
}

// ---- hasChoice_146d9c.cpp ----
uint8_t ChoiceWindow::hasChoice()
{
    return this->field_0x58;
}

// ---- setWidth_146da4.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth;

void ChoiceWindow::setWidth(int width)
{
    this->field_0x8 = width;
    this->field_0x0 = *g_ChoiceWindow_screenWidth / 2 - width / 2;
}

// ---- set_146954.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146954;

void ChoiceWindow::set(String const &a, String const &b)
{
    void *gameText = *g_ChoiceWindow_gameText_146954;
    String *title = (String *)((GameText *)(gameText))->getText(0x186);
    String *left = (String *)((GameText *)(gameText))->getText(0x86);
    String *center = (String *)((GameText *)(gameText))->getText(0x87);
    set(*title, a, false, *left, *center, b, -1, -1);
}

// ---- set_146d5c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146d5c;

void ChoiceWindow::set(String const &text, bool flag)
{
    String *title = (String *)((GameText *)(*g_ChoiceWindow_gameText_146d5c))->getText(0x186);
    set(*title, text, flag);
}

// ---- set_1468f4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468f4;

void ChoiceWindow::set(String const &title, String const &message, bool flag)
{
    void *gameText = *g_ChoiceWindow_gameText_1468f4;
    String *left = (String *)((GameText *)(gameText))->getText(0x86);
    String *center = (String *)((GameText *)(gameText))->getText(0x87);
    String *right = (String *)((GameText *)(gameText))->getText(0x20c);
    set(title, message, flag, *left, *center, *right, -1, -1);
}

// ---- OnTouchEnd_147564.cpp ----
int ChoiceWindow::OnTouchEnd(int x, int y)
{
    if (this->field_0x59 != 0) {
        if (this->field_0x10 != 0 && ((TouchButton *)(this->field_0x10))->OnTouchEnd(x, y) != 0)
            return 0;
        if (this->field_0x14 != 0 && ((TouchButton *)(this->field_0x14))->OnTouchEnd(x, y) != 0)
            return 1;
        if (this->field_0x18 != 0 && ((TouchButton *)(this->field_0x18))->OnTouchEnd(x, y) != 0)
            return 2;
    }
    if (this->field_0x1c != 0) ScrollTouchWindow_OnTouchEnd(this->field_0x1c, x, y);
    return -1;
}

// ---- set_1468c8.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468c8;

void ChoiceWindow::set(String const &text)
{
    String *title = (String *)((GameText *)(*g_ChoiceWindow_gameText_1468c8))->getText(0x186);
    set(*title, text, false);
}

// ---- update_1471b4.cpp ----
void ChoiceWindow::update(int dt)
{
    ScrollTouchWindow_update(this->field_0x1c, dt);
}

// ---- removeButtons_146d94.cpp ----
void ChoiceWindow::removeButtons()
{
    this->field_0x59 = 0;
}

// ---- OnTouchBegin_1474cc.cpp ----
int ChoiceWindow::OnTouchBegin(int x, int y)
{
    if (this->field_0x59 != 0) {
        if (this->field_0x10 != 0) ((TouchButton *)(this->field_0x10))->OnTouchBegin(x, y);
        if (this->field_0x14 != 0) ((TouchButton *)(this->field_0x14))->OnTouchBegin(x, y);
        if (this->field_0x18 != 0) ((TouchButton *)(this->field_0x18))->OnTouchBegin(x, y);
    }
    if (this->field_0x1c != 0) ScrollTouchWindow_OnTouchBegin(this->field_0x1c, x, y);
    return 0;
}

// ---- _ChoiceWindow_146870.cpp ----
ChoiceWindow::~ChoiceWindow()
{
    if (this->field_0x10 != 0) { ((TouchButton *)(this->field_0x10))->dtor(); operator delete(this->field_0x10); }
    this->field_0x10 = 0;

    if (this->field_0x14 != 0) { ((TouchButton *)(this->field_0x14))->dtor(); operator delete(this->field_0x14); }
    this->field_0x14 = 0;

    if (this->field_0x18 != 0) { ((TouchButton *)(this->field_0x18))->dtor(); operator delete(this->field_0x18); }
    this->field_0x18 = 0;

    if (this->field_0x1c != 0) operator delete(ScrollTouchWindow_dtor(this->field_0x1c));
    this->field_0x1c = 0;

    this->field_0x3c.~String();
    this->field_0x20.~String();
}

// ---- setHeight_146dc4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_146dc4;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_146dc4;

void ChoiceWindow::setHeight(int height)
{
    void *layout = *g_ChoiceWindow_layout_146dc4;
    int top = F<int>(layout, 0x8);
    this->field_0xc = top + height;

    void *button = this->field_0x10;
    int yBase = *g_ChoiceWindow_screenHeight_146dc4 / 2 - height / 2;
    this->field_0x4 = yBase - top;

    if (this->field_0x58 == 0) {
        if (button == 0) return;
        ((TouchButton *)(button))->setPosition(this->field_0x0 + this->field_0x8 / 2, height + yBase - this->field_0x48, 0x24);
        return;
    }

    if (button != 0) {
        ((TouchButton *)(button))->setPosition(this->field_0x0 + this->field_0x8 / 2 - F<int>(layout, 0x4c) / 2, height + yBase - this->field_0x48, 0x22);
    }

    button = this->field_0x14;
    if (button == 0) return;

    layout = *g_ChoiceWindow_layout_146dc4;
    ((TouchButton *)(button))->setPosition(this->field_0x0 + this->field_0x8 / 2 + F<int>(layout, 0x4c) / 2, this->field_0x4 + this->field_0xc - this->field_0x48, 0x21);
}

// ---- setMedal_146e8c.cpp ----
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
    this->field_0x3c = *(String *)((GameText *)(gameText))->getText(medal + 0x5e3);

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
    ScrollTouchWindow_setTextCentered(this->field_0x1c, true);

    void *canvas = *g_ChoiceWindow_canvas_146e8c;
    if (count < 0x24) {
        PaintCanvas_Image2DCreate(canvas, F<unsigned short>(g_ChoiceWindow_medalImagesLow_146e8c, count * 4),
                                  (unsigned int *)&this->field_0x34);
    } else {
        PaintCanvas_Image2DCreate(canvas, F<unsigned short>(g_ChoiceWindow_medalImagesHigh_146e8c, count * 4),
                                  (unsigned int *)&this->field_0x34);
    }
    PaintCanvas_Image2DCreate(*g_ChoiceWindow_canvas_146e8c,
                              F<unsigned short>(g_ChoiceWindow_medalImages_146e8c, medal * 4),
                              (unsigned int *)&this->field_0x38);

    this->field_0x2c = count;
    this->field_0x30 = medal;
    this->field_0x59 = 1;
}

// ---- ChoiceWindow_1467ec.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_config_1467ec;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth_1467ec;

ChoiceWindow::ChoiceWindow()
{
    new (&this->field_0x20) String();
    new (&this->field_0x3c) String();

    void *config = *g_ChoiceWindow_config_1467ec;
    int width = F<int>(config, 0x264);
    this->field_0x8 = width;
    this->field_0x48 = F<int>(config, 0x268);
    this->field_0x4c = F<int>(config, 0x26c);
    this->field_0x50 = F<int>(config, 0x270);
    this->field_0x54 = F<int>(config, 0x274);

    int screenWidth = *g_ChoiceWindow_screenWidth_1467ec;
    this->field_0x10 = 0;
    this->field_0x14 = 0;
    this->field_0x18 = 0;
    this->field_0x1c = 0;
    this->field_0x34 = -1;
    this->field_0x38 = -1;
    this->field_0x59 = 1;
    this->field_0x0 = screenWidth / 2 - width / 2;
}

// ---- set_1469b0.cpp ----
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
    this->field_0x58 = hasButtons;

    void *gameText = *g_ChoiceWindow_gameText_1469b0;
    bool defaultButtons = false;
    if (String_Compare((String *)((GameText *)(gameText))->getText(0x86), left) == 0)
        defaultButtons = String_Compare((String *)((GameText *)(gameText))->getText(0x87), right) == 0;

    if (width == -1)
        width = F<int>(*g_ChoiceWindow_defaultConfig_1469b0, 0x264);
    if (!defaultButtons)
        width = (int)((float)width * 0.85f);

    this->field_0x8 = width;
    this->field_0x0 = *g_ChoiceWindow_screenWidth_1469b0 / 2 - width / 2;
    FModSound_play(*g_ChoiceWindow_sound_1469b0, 0x7e, 0, 0, 0.0f);
    this->field_0x20 = title;

    if (this->field_0x1c != 0) operator delete(ScrollTouchWindow_dtor(this->field_0x1c));
    this->field_0x1c = 0;

    Array<String *> *lines = new Array<String *>();

    void *layout = *g_ChoiceWindow_layout_1469b0;
    Globals_getLineArray(*g_ChoiceWindow_globals_1469b0, *g_ChoiceWindow_lineFont_1469b0, message,
                         (this->field_0x8 - F<int>(layout, 0x4c) * 2) - F<int>(layout, 0x48),
                         lines);

    int contentHeight = F<int>(layout, 0x4) * (int)lines->size() + F<int>(layout, 0x8) +
                        F<int>(layout, 0x4c) * 2 + this->field_0x48 * 2 +
                        F<int>(layout, 0x30);
    int maxHeight = F<int>(layout, 0x278);
    if ((unsigned)contentHeight < (unsigned)maxHeight)
        maxHeight = contentHeight;
    this->field_0xc = maxHeight;

    if (y == -1)
        y = *g_ChoiceWindow_screenHeight_1469b0 / 2 - maxHeight / 2;
    this->field_0x4 = y;

    void *scroll = operator new(0x24);
    ScrollTouchWindow_ctor(scroll, this->field_0x0, F<int>(layout, 0x8) + y,
                           this->field_0x8,
                           (((maxHeight - this->field_0x48 * 2) - F<int>(layout, 0x8)) -
                            F<int>(layout, 0x30)) + F<int>(layout, 0x2bc),
                           false);
    this->field_0x1c = scroll;

    ScrollTouchWindow_setText(scroll, title, message);

    ArrayReleaseClasses_StringPtr(lines);
    operator delete(Array_StringPtr_dtor(lines));

    if (this->field_0x10 != 0) { ((TouchButton *)(this->field_0x10))->dtor(); operator delete(this->field_0x10); }
    this->field_0x10 = 0;

    if (this->field_0x14 != 0) { ((TouchButton *)(this->field_0x14))->dtor(); operator delete(this->field_0x14); }
    this->field_0x14 = 0;

    if (hasButtons) {
        int baseWidth = F<int>(layout, 0x40);
        float scaled = (float)baseWidth * 1.5f;
        if (defaultButtons) scaled = (float)baseWidth;
        int buttonWidth = (int)scaled;

        void *button = operator new(0xc8);
        TouchButton_ctor(button, left, 0,
                         this->field_0x0 + this->field_0x8 / 2 - F<int>(layout, 0x4c) / 2,
                         this->field_0x4 + this->field_0xc - this->field_0x48,
                         buttonWidth, 0x22, 4);
        this->field_0x10 = button;

        button = operator new(0xc8);
        TouchButton_ctor(button, right, 0,
                         this->field_0x0 + this->field_0x8 / 2 + F<int>(layout, 0x4c) / 2,
                         this->field_0x4 + this->field_0xc - this->field_0x48,
                         buttonWidth, 0x21, 4);
        this->field_0x14 = button;
    } else {
        void *button = operator new(0xc8);
        TouchButton_ctor(button, single, 0,
                         this->field_0x0 + this->field_0x8 / 2,
                         this->field_0x4 + this->field_0xc - this->field_0x48,
                         F<int>(layout, 0x40), 0x24, 4);
        this->field_0x14 = button;
    }

    this->field_0x59 = 1;
}

// ---- setMiscButton_147068.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_a;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_b;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_c;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_147068;

void ChoiceWindow::setMiscButton(String const &text)
{
    void *layout;
    int padding = this->field_0x48;
    int buttonWidth;

    if (this->field_0x10 == 0 || this->field_0x14 == 0) {
        layout = *g_ChoiceWindow_layout_147068_b;
        buttonWidth = padding + F<int>(layout, 0x40) * 2;
    } else {
        int leftWidth = ((TouchButton *)(this->field_0x10))->getWidth();
        int rightWidth = ((TouchButton *)(this->field_0x14))->getWidth();
        layout = *g_ChoiceWindow_layout_147068_a;
        buttonWidth = leftWidth + rightWidth + padding;
    }

    void *button = operator new(0xc8);
    TouchButton_ctor(button, text, 0,
                     this->field_0x0 + this->field_0x8 / 2 - F<int>(layout, 0x4c) / 2,
                     this->field_0x4 + this->field_0xc - padding,
                     buttonWidth, 0x22, 4);

    this->field_0x18 = button;

    layout = *g_ChoiceWindow_layout_147068_c;
    setHeight((this->field_0xc - F<int>(layout, 0x8)) + this->field_0x48 * 2 + F<int>(layout, 0x30));

    ((TouchButton *)(this->field_0x18))->setPosition(this->field_0x0 + this->field_0x8 / 2, this->field_0x4 + this->field_0xc - this->field_0x48, 0x24);

    layout = *g_ChoiceWindow_layout_147068_c;
    int delta = -(F<int>(layout, 0x30) + this->field_0x48);
    if (this->field_0x10 != 0) ((TouchButton *)(this->field_0x10))->translate(0, delta);
    if (this->field_0x14 != 0) ((TouchButton *)(this->field_0x14))->translate(0, delta);

    ScrollTouchWindow_setPosition(this->field_0x1c,
                                  *g_ChoiceWindow_screenHeight_147068 / 2 -
                                      this->field_0xc / 2 + F<int>(layout, 0x8));
}

// ---- draw_1471bc.cpp ----
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
    Layout_drawMask(layout);

    ((Layout *)(layout))->drawBox6(7, this->field_0x0, this->field_0x4, this->field_0x8, this->field_0xc, &this->field_0x20);

    void *canvas = *g_ChoiceWindow_canvas_1471bc;
    PaintCanvas_SetColor(canvas, 0xffffffff);

    if (this->field_0x34 != -1) {
        PaintCanvas_DrawImage2D(canvas, this->field_0x34,
                                this->field_0x0 + (this->field_0x8 >> 1),
                                this->field_0x4 + this->field_0x4c + 1, 0x11);
        int color;
        if (this->field_0x2c < 0x24)
            color = F<int>(g_ChoiceWindow_medalColorsLow_1471bc, this->field_0x2c * 4);
        else
            color = F<int>(g_ChoiceWindow_medalColorsHigh_1471bc, this->field_0x2c * 4);
        PaintCanvas_SetColor(canvas, color);
        PaintCanvas_DrawImage2D(canvas, this->field_0x38,
                                this->field_0x0 + (this->field_0x8 >> 1),
                                this->field_0x4 + this->field_0x4c, 0x11);

        if (Status_hardCoreMode(*g_ChoiceWindow_status_1471bc) == 0 &&
            ((Achievements *)(*g_ChoiceWindow_achievements_1471bc))->isEliteMedal(this->field_0x30) == 0) {
            String credits;
            Layout_formatCredits(&credits, layout,
                                 F<int>(g_ChoiceWindow_creditValues_1471bc, this->field_0x2c * 4));
            String creditsText = credits;  // prefix "" + credits

            void *font = *g_ChoiceWindow_font_1471bc_a;
            int textWidth = PaintCanvas_GetTextWidth(canvas, font, creditsText);
            PaintCanvas_DrawString(canvas, font, creditsText,
                                   this->field_0x0 + (this->field_0x8 >> 1) - textWidth / 2,
                                   this->field_0x4 + this->field_0xc - this->field_0x50,
                                   false);
        }

        PaintCanvas_SetColor(canvas, 0xffffffff);
        void *font = *g_ChoiceWindow_font_1471bc_b;
        int textWidth = PaintCanvas_GetTextWidth(canvas, font, this->field_0x3c);
        PaintCanvas_DrawString(canvas, font, this->field_0x3c,
                               this->field_0x0 + (this->field_0x8 >> 1) - textWidth / 2,
                               this->field_0x4 + this->field_0x54, false);
    }

    ScrollTouchWindow_draw(this->field_0x1c);

    if (this->field_0x59 != 0) {
        if (this->field_0x10 != 0) ((TouchButton *)(this->field_0x10))->draw();

        if (this->field_0x14 != 0) {
            ((TouchButton *)(this->field_0x14))->draw();
            if (this->field_0x14 != 0) {
                TouchButton_getPosition(pos, this->field_0x14);
                F<int>(g_ChoiceWindow_posTargetA_1471bc, 0x8) = (int)pos[0];
                TouchButton_getPosition(pos, this->field_0x14);
                F<int>(g_ChoiceWindow_posTargetB_1471bc, 0x8) = (int)pos[1];
            }
        }

        if (this->field_0x10 != 0) {
            TouchButton_getPosition(pos, this->field_0x10);
            F<int>(g_ChoiceWindow_posTargetC_1471bc, 0xc) = (int)pos[0];
            TouchButton_getPosition(pos, this->field_0x10);
            F<int>(g_ChoiceWindow_posTargetD_1471bc, 0xc) = (int)pos[1];

            if (this->field_0x14 == 0) {
                TouchButton_getPosition(pos, this->field_0x10);
                F<int>(g_ChoiceWindow_posTargetC_1471bc, 0x8) = (int)pos[0];
                TouchButton_getPosition(pos, this->field_0x10);
                F<int>(g_ChoiceWindow_posTargetD_1471bc, 0x8) = (int)pos[1];
            }
        }

        *g_ChoiceWindow_dirtyFlag_1471bc = 1;
        if (this->field_0x18 != 0) ((TouchButton *)(this->field_0x18))->draw();
    }
}
