#include "ChoiceWindow.h"


extern "C" void TouchButton_OnTouchMove(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchMove(void *self, int x, int y);
extern "C" String *GameText_getText(void *self, int id);
extern "C" int TouchButton_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchWindow_update(void *self, int dt);
extern "C" void TouchButton_OnTouchBegin(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchBegin(void *self, int x, int y);
extern "C" void *TouchButton_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
extern "C" void String_dtor(void *self);
extern "C" void TouchButton_setPosition(void *self, int x, int y, int anchor);
extern "C" void String_assign(void *self, String *src);
extern "C" void String_ctor_cstr(void *self, char const *text, bool copy);
extern "C" void String_ctor_string(void *self, String *src, bool copy);
extern "C" void String_ctor_int(void *self, int value);
extern "C" int Achievements_getValue(void *self, int medal, int count);
extern "C" void Status_replaceHash(void *out, void *status, void *pattern, void *value);
extern "C" void String_plus(void *out, void *a, void *b);
extern "C" void ScrollTouchWindow_setTextCentered(void *self, bool centered);
extern "C" void PaintCanvas_Image2DCreate(void *self, unsigned short image, unsigned int *out);
extern "C" void String_ctor(void *self);
extern "C" int String_Compare(String *a, String const &b);
extern "C" void FModSound_play(void *self, int sound, void *pos, void *vel, float volume);
extern "C" void String_assign(void *self, String const &src);
extern "C" void Array_StringPtr_ctor(void *self);
extern "C" void Globals_getLineArray(void *self, void *font, String const &text, int width, void *array);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int width, int height, bool centered);
extern "C" void String_ctor_string(void *self, String const &src, bool copy);
extern "C" void ScrollTouchWindow_setText(void *self, void *title, void *message);
extern "C" void ArrayReleaseClasses_StringPtr(void *self);
extern "C" void *Array_StringPtr_dtor(void *self);
extern "C" int TouchButton_getWidth(void *self);
extern "C" void TouchButton_translate(void *self, int x, int y);
extern "C" void ScrollTouchWindow_setPosition(void *self, int y);
extern "C" void Layout_drawMask(void *self);
extern "C" void Layout_drawBox(void *self, int style, int x, int y, int width, int height, void *title);
extern "C" void PaintCanvas_SetColor(void *self, unsigned int color);
extern "C" void PaintCanvas_DrawImage2D(void *self, int image, int x, int y, int anchor);
extern "C" int Status_hardCoreMode(void *self);
extern "C" int Achievements_isEliteMedal(void *self, int medal);
extern "C" void Layout_formatCredits(void *out, void *layout, int value);
extern "C" int PaintCanvas_GetTextWidth(void *self, void *font, void *text);
extern "C" void PaintCanvas_DrawString(void *self, void *font, void *text, int x, int y, bool shadow);
extern "C" void ScrollTouchWindow_draw(void *self);
extern "C" void TouchButton_draw(void *self);
extern "C" void TouchButton_getPosition(void *out, void *self);

// ---- OnTouchMove_147518.cpp ----
int ChoiceWindow::OnTouchMove(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (F<void *>(this, 0x10) != 0)
            TouchButton_OnTouchMove(F<void *>(this, 0x10), x, y);
        if (F<void *>(this, 0x14) != 0)
            TouchButton_OnTouchMove(F<void *>(this, 0x14), x, y);
        if (F<void *>(this, 0x18) != 0)
            TouchButton_OnTouchMove(F<void *>(this, 0x18), x, y);
    }
    if (F<void *>(this, 0x1c) != 0)
        ScrollTouchWindow_OnTouchMove(F<void *>(this, 0x1c), x, y);
    return 0;
}

// ---- hasChoice_146d9c.cpp ----
uint8_t ChoiceWindow::hasChoice()
{
    return F<uint8_t>(this, 0x58);
}

// ---- setWidth_146da4.cpp ----
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth;

void ChoiceWindow::setWidth(int width)
{
    F<int>(this, 0x8) = width;
    F<int>(this, 0x0) = *g_ChoiceWindow_screenWidth / 2 - width / 2;
}

// ---- set_146954.cpp ----
typedef String *(*GetTextFn)(void *self, int id);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146954;
__attribute__((visibility("hidden"))) extern GetTextFn g_ChoiceWindow_getText_146954;

void ChoiceWindow::set(String const &a, String const &b)
{
    void **gameText = g_ChoiceWindow_gameText_146954;
    GetTextFn getText = g_ChoiceWindow_getText_146954;
    String *title = getText(*gameText, 0x186);
    String *left = getText(*gameText, 0x86);
    String *center = getText(*gameText, 0x87);
    set(*title, a, false, *left, *center, b, -1, -1);
}

// ---- set_146d5c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146d5c;

void ChoiceWindow::set(String const &text, bool flag)
{
    String *title = GameText_getText(*g_ChoiceWindow_gameText_146d5c, 0x186);
    set(*title, text, flag);
}

// ---- set_1468f4.cpp ----
typedef String *(*GetTextFn)(void *self, int id);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468f4;
__attribute__((visibility("hidden"))) extern GetTextFn g_ChoiceWindow_getText_1468f4;

void ChoiceWindow::set(String const &title, String const &message, bool flag)
{
    void **gameText = g_ChoiceWindow_gameText_1468f4;
    GetTextFn getText = g_ChoiceWindow_getText_1468f4;
    String *left = getText(*gameText, 0x86);
    String *center = getText(*gameText, 0x87);
    String *right = getText(*gameText, 0x20c);
    set(title, message, flag, *left, *center, *right, -1, -1);
}

// ---- OnTouchEnd_147564.cpp ----
int ChoiceWindow::OnTouchEnd(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (F<void *>(this, 0x10) != 0 && TouchButton_OnTouchEnd(F<void *>(this, 0x10), x, y) != 0)
            return 0;
        if (F<void *>(this, 0x14) != 0 && TouchButton_OnTouchEnd(F<void *>(this, 0x14), x, y) != 0)
            return 1;
        if (F<void *>(this, 0x18) != 0 && TouchButton_OnTouchEnd(F<void *>(this, 0x18), x, y) != 0)
            return 2;
    }
    if (F<void *>(this, 0x1c) != 0)
        ScrollTouchWindow_OnTouchEnd(F<void *>(this, 0x1c), x, y);
    return -1;
}

// ---- set_1468c8.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_1468c8;

void ChoiceWindow::set(String const &text)
{
    String *title = GameText_getText(*g_ChoiceWindow_gameText_1468c8, 0x186);
    set(*title, text, false);
}

// ---- update_1471b4.cpp ----
void ChoiceWindow::update(int dt)
{
    ScrollTouchWindow_update(F<void *>(this, 0x1c), dt);
}

// ---- removeButtons_146d94.cpp ----
void ChoiceWindow::removeButtons()
{
    F<uint8_t>(this, 0x59) = 0;
}

// ---- OnTouchBegin_1474cc.cpp ----
int ChoiceWindow::OnTouchBegin(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (F<void *>(this, 0x10) != 0)
            TouchButton_OnTouchBegin(F<void *>(this, 0x10), x, y);
        if (F<void *>(this, 0x14) != 0)
            TouchButton_OnTouchBegin(F<void *>(this, 0x14), x, y);
        if (F<void *>(this, 0x18) != 0)
            TouchButton_OnTouchBegin(F<void *>(this, 0x18), x, y);
    }
    if (F<void *>(this, 0x1c) != 0)
        ScrollTouchWindow_OnTouchBegin(F<void *>(this, 0x1c), x, y);
    return 0;
}

// ---- _ChoiceWindow_146870.cpp ----
void operator delete(void *ptr) noexcept;

ChoiceWindow::~ChoiceWindow()
{
    void *button = F<void *>(this, 0x10);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x10) = 0;

    button = F<void *>(this, 0x14);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x14) = 0;

    button = F<void *>(this, 0x18);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x18) = 0;

    void *scroll = F<void *>(this, 0x1c);
    if (scroll != 0)
        operator delete(ScrollTouchWindow_dtor(scroll));
    void *title = (char *)this + 0x3c;
    F<void *>(this, 0x1c) = 0;
    String_dtor(title);
    String_dtor((char *)this + 0x20);
}

// ---- setHeight_146dc4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_146dc4;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_146dc4;

void ChoiceWindow::setHeight(int height)
{
    void **layoutHolder = g_ChoiceWindow_layout_146dc4;
    void *layout = *layoutHolder;
    int top = F<int>(layout, 0x8);
    F<int>(this, 0xc) = top + height;

    void *button = F<void *>(this, 0x10);
    int yBase = *g_ChoiceWindow_screenHeight_146dc4 / 2 - height / 2;
    F<int>(this, 0x4) = yBase - top;

    if (F<uint8_t>(this, 0x58) == 0) {
        if (button == 0)
            return;
        TouchButton_setPosition(button,
                                F<int>(this, 0x0) + F<int>(this, 0x8) / 2,
                                height + yBase - F<int>(this, 0x48),
                                0x24);
        return;
    }

    if (button != 0) {
        TouchButton_setPosition(button,
                                F<int>(this, 0x0) + F<int>(this, 0x8) / 2 - F<int>(layout, 0x4c) / 2,
                                height + yBase - F<int>(this, 0x48),
                                0x22);
    }

    button = F<void *>(this, 0x14);
    if (button == 0)
        return;

    layout = *layoutHolder;
    TouchButton_setPosition(button,
                            F<int>(this, 0x0) + F<int>(this, 0x8) / 2 + F<int>(layout, 0x4c) / 2,
                            F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x48),
                            0x21);
}

// ---- setMedal_146e8c.cpp ----
typedef void (*StringDtorFn)(void *self);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_gameText_146e8c;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_replaceHash_146e8c;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_achievements_146e8c;
__attribute__((visibility("hidden"))) extern StringDtorFn g_ChoiceWindow_stringDtor_146e8c;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_canvas_146e8c;
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalImagesLow_146e8c[];
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalImagesHigh_146e8c[];
__attribute__((visibility("hidden"))) extern char g_ChoiceWindow_medalImages_146e8c[];

void ChoiceWindow::setMedal(int medal, int count)
{
    char numberText[12];
    char numberCopy[12];
    char pattern[12];
    char replaced[12];
    char prefix[12];
    char joined[12];
    char suffix[12];
    char finalText[12];

    void *gameText = *g_ChoiceWindow_gameText_146e8c;
    String_assign((char *)this + 0x3c, GameText_getText(gameText, medal + 0x5e3));

    String_ctor_cstr(prefix, "", false);
    void *replaceBase = *g_ChoiceWindow_replaceHash_146e8c;
    String_ctor_string(pattern, GameText_getText(gameText, medal + 0x610), false);
    int value = Achievements_getValue(*g_ChoiceWindow_achievements_146e8c, medal, count);
    String_ctor_int(numberText, value);
    String_ctor_string(numberCopy, (String *)numberText, false);
    Status_replaceHash(replaced, replaceBase, pattern, numberCopy);
    String_plus(joined, prefix, replaced);
    String_ctor_cstr(suffix, "", false);
    String_plus(finalText, joined, suffix);

    StringDtorFn dtor = g_ChoiceWindow_stringDtor_146e8c;
    dtor(suffix);
    dtor(joined);
    dtor(replaced);
    dtor(numberCopy);
    dtor(numberText);
    dtor(pattern);
    dtor(prefix);

    set(*GameText_getText(gameText, 0x161), *(String *)finalText, false);
    ScrollTouchWindow_setTextCentered(F<void *>(this, 0x1c), true);

    void *canvas = *g_ChoiceWindow_canvas_146e8c;
    if (count < 0x24) {
        PaintCanvas_Image2DCreate(canvas, F<unsigned short>(g_ChoiceWindow_medalImagesLow_146e8c, count * 4),
                                  (unsigned int *)((char *)this + 0x34));
    } else {
        PaintCanvas_Image2DCreate(canvas, F<unsigned short>(g_ChoiceWindow_medalImagesHigh_146e8c, count * 4),
                                  (unsigned int *)((char *)this + 0x34));
    }
    PaintCanvas_Image2DCreate(*g_ChoiceWindow_canvas_146e8c,
                              F<unsigned short>(g_ChoiceWindow_medalImages_146e8c, medal * 4),
                              (unsigned int *)((char *)this + 0x38));

    F<int>(this, 0x2c) = count;
    F<int>(this, 0x30) = medal;
    F<uint8_t>(this, 0x59) = 1;
    String_dtor(finalText);
}

// ---- ChoiceWindow_1467ec.cpp ----
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_config_1467ec;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenWidth_1467ec;

struct ChoiceWindow_Int4 {
    int a;
    int b;
    int c;
    int d;
};

ChoiceWindow::ChoiceWindow()
{
    void *primaryText = (char *)this + 0x20;
    String_ctor(primaryText);
    String_ctor((char *)this + 0x3c);

    void **configHolder = g_ChoiceWindow_config_1467ec;
    int *screenWidthPtr = g_ChoiceWindow_screenWidth_1467ec;
    void *config = *configHolder;
    int width = F<int>(config, 0x264);
    F<int>(this, 0x8) = width;
    F<ChoiceWindow_Int4>(this, 0x48) = F<ChoiceWindow_Int4>(config, 0x268);

    int screenWidth = *screenWidthPtr;
    F<ChoiceWindow_Int4>(this, 0x10) = ChoiceWindow_Int4();
    F<int>(this, 0x34) = -1;
    F<int>(this, 0x38) = -1;
    F<uint8_t>(this, 0x59) = 1;
    F<int>(this, 0x0) = screenWidth / 2 - width / 2;
}

// ---- set_1469b0.cpp ----
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" void TouchButton_ctor(void *self, String const &text, int value, int x, int y,
                                  int width, int anchor, int mode);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_canary_1469b0;
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
    char titleCopy[12];
    char messageCopy[12];

    F<uint8_t>(this, 0x58) = hasButtons;

    void *gameText = *g_ChoiceWindow_gameText_1469b0;
    bool defaultButtons = false;
    if (String_Compare(GameText_getText(gameText, 0x86), left) == 0)
        defaultButtons = String_Compare(GameText_getText(gameText, 0x87), right) == 0;

    if (width == -1)
        width = F<int>(*g_ChoiceWindow_defaultConfig_1469b0, 0x264);
    if (!defaultButtons)
        width = (int)((float)width * 0.85f);

    F<int>(this, 0x8) = width;
    F<int>(this, 0x0) = *g_ChoiceWindow_screenWidth_1469b0 / 2 - width / 2;
    FModSound_play(*g_ChoiceWindow_sound_1469b0, 0x7e, 0, 0, 0.0f);
    String_assign((char *)this + 0x20, title);

    void *scroll = F<void *>(this, 0x1c);
    if (scroll != 0)
        operator delete(ScrollTouchWindow_dtor(scroll));
    F<void *>(this, 0x1c) = 0;

    void *lines = operator new(0xc);
    Array_StringPtr_ctor(lines);

    void *layout = *g_ChoiceWindow_layout_1469b0;
    Globals_getLineArray(*g_ChoiceWindow_globals_1469b0, *g_ChoiceWindow_lineFont_1469b0, message,
                         (F<int>(this, 0x8) - F<int>(layout, 0x4c) * 2) -
                             F<int>(layout, 0x48),
                         lines);

    int contentHeight = F<int>(layout, 0x4) * F<int>(lines, 0x0) + F<int>(layout, 0x8) +
                        F<int>(layout, 0x4c) * 2 + F<int>(this, 0x48) * 2 +
                        F<int>(layout, 0x30);
    int maxHeight = F<int>(layout, 0x278);
    if ((unsigned)contentHeight < (unsigned)maxHeight)
        maxHeight = contentHeight;
    F<int>(this, 0xc) = maxHeight;

    if (y == -1)
        y = *g_ChoiceWindow_screenHeight_1469b0 / 2 - maxHeight / 2;
    F<int>(this, 0x4) = y;

    scroll = operator new(0x24);
    ScrollTouchWindow_ctor(scroll, F<int>(this, 0x0), F<int>(layout, 0x8) + y,
                           F<int>(this, 0x8),
                           (((maxHeight - F<int>(this, 0x48) * 2) - F<int>(layout, 0x8)) -
                            F<int>(layout, 0x30)) +
                               F<int>(layout, 0x2bc),
                           false);
    F<void *>(this, 0x1c) = scroll;

    String_ctor_string(titleCopy, title, false);
    String_ctor_string(messageCopy, message, false);
    ScrollTouchWindow_setText(scroll, titleCopy, messageCopy);
    String_dtor(messageCopy);
    String_dtor(titleCopy);

    ArrayReleaseClasses_StringPtr(lines);
    operator delete(Array_StringPtr_dtor(lines));

    void *button = F<void *>(this, 0x10);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x10) = 0;

    button = F<void *>(this, 0x14);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x14) = 0;

    if (hasButtons) {
        int baseWidth = F<int>(layout, 0x40);
        float scaled = (float)baseWidth * 1.5f;
        if (defaultButtons)
            scaled = (float)baseWidth;
        int buttonWidth = (int)scaled;

        button = operator new(0xc8);
        TouchButton_ctor(button, left, 0,
                         F<int>(this, 0x0) + F<int>(this, 0x8) / 2 - F<int>(layout, 0x4c) / 2,
                         F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x48),
                         buttonWidth, 0x22, 4);
        F<void *>(this, 0x10) = button;

        button = operator new(0xc8);
        TouchButton_ctor(button, right, 0,
                         F<int>(this, 0x0) + F<int>(this, 0x8) / 2 + F<int>(layout, 0x4c) / 2,
                         F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x48),
                         buttonWidth, 0x21, 4);
        F<void *>(this, 0x14) = button;
    } else {
        button = operator new(0xc8);
        TouchButton_ctor(button, single, 0,
                         F<int>(this, 0x0) + F<int>(this, 0x8) / 2,
                         F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x48),
                         F<int>(layout, 0x40), 0x24, 4);
        F<void *>(this, 0x14) = button;
    }

    F<uint8_t>(this, 0x59) = 1;
}

// ---- setMiscButton_147068.cpp ----
void *operator new(__SIZE_TYPE__ size);

extern "C" void TouchButton_ctor(void *self, String const &text, int value, int x, int y,
                                  int width, int anchor, int mode);

__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_a;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_b;
__attribute__((visibility("hidden"))) extern void **g_ChoiceWindow_layout_147068_c;
__attribute__((visibility("hidden"))) extern int *g_ChoiceWindow_screenHeight_147068;

void ChoiceWindow::setMiscButton(String const &text)
{
    void *layout;
    int padding = F<int>(this, 0x48);
    int buttonWidth;

    if (F<void *>(this, 0x10) == 0 || F<void *>(this, 0x14) == 0) {
        layout = *g_ChoiceWindow_layout_147068_b;
        buttonWidth = padding + F<int>(layout, 0x40) * 2;
    } else {
        int leftWidth = TouchButton_getWidth(F<void *>(this, 0x10));
        int rightWidth = TouchButton_getWidth(F<void *>(this, 0x14));
        layout = *g_ChoiceWindow_layout_147068_a;
        buttonWidth = leftWidth + rightWidth + padding;
    }

    void *button = operator new(0xc8);
    TouchButton_ctor(button, text, 0,
                     F<int>(this, 0x0) + F<int>(this, 0x8) / 2 - F<int>(layout, 0x4c) / 2,
                     F<int>(this, 0x4) + F<int>(this, 0xc) - padding,
                     buttonWidth, 0x22, 4);

    F<void *>(this, 0x18) = button;

    void **layoutHolder = g_ChoiceWindow_layout_147068_c;
    layout = *layoutHolder;
    setHeight((F<int>(this, 0xc) - F<int>(layout, 0x8)) + F<int>(this, 0x48) * 2 +
              F<int>(layout, 0x30));

    TouchButton_setPosition(F<void *>(this, 0x18),
                            F<int>(this, 0x0) + F<int>(this, 0x8) / 2,
                            F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x48),
                            0x24);

    int delta = -(F<int>(*layoutHolder, 0x30) + F<int>(this, 0x48));
    if (F<void *>(this, 0x10) != 0)
        TouchButton_translate(F<void *>(this, 0x10), 0, delta);
    if (F<void *>(this, 0x14) != 0)
        TouchButton_translate(F<void *>(this, 0x14), 0, delta);

    ScrollTouchWindow_setPosition(F<void *>(this, 0x1c),
                                  *g_ChoiceWindow_screenHeight_147068 / 2 -
                                      F<int>(this, 0xc) / 2 + F<int>(*layoutHolder, 0x8));
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
    char titleCopy[12];
    char prefix[12];
    char credits[12];
    char creditsText[12];
    float pos[2];

    void *layout = *g_ChoiceWindow_layout_1471bc;
    Layout_drawMask(layout);

    String_ctor_string(titleCopy, *(String *)((char *)this + 0x20), false);
    Layout_drawBox(layout, 7, F<int>(this, 0x0), F<int>(this, 0x4),
                   F<int>(this, 0x8), F<int>(this, 0xc), titleCopy);
    String_dtor(titleCopy);

    void *canvas = *g_ChoiceWindow_canvas_1471bc;
    PaintCanvas_SetColor(canvas, 0xffffffff);

    if (F<int>(this, 0x34) != -1) {
        PaintCanvas_DrawImage2D(canvas, F<int>(this, 0x34),
                                F<int>(this, 0x0) + (F<int>(this, 0x8) >> 1),
                                F<int>(this, 0x4) + F<int>(this, 0x4c) + 1, 0x11);
        int color;
        if (F<int>(this, 0x2c) < 0x24)
            color = F<int>(g_ChoiceWindow_medalColorsLow_1471bc, F<int>(this, 0x2c) * 4);
        else
            color = F<int>(g_ChoiceWindow_medalColorsHigh_1471bc, F<int>(this, 0x2c) * 4);
        PaintCanvas_SetColor(canvas, color);
        PaintCanvas_DrawImage2D(canvas, F<int>(this, 0x38),
                                F<int>(this, 0x0) + (F<int>(this, 0x8) >> 1),
                                F<int>(this, 0x4) + F<int>(this, 0x4c), 0x11);

        if (Status_hardCoreMode(*g_ChoiceWindow_status_1471bc) == 0 &&
            Achievements_isEliteMedal(*g_ChoiceWindow_achievements_1471bc, F<int>(this, 0x30)) == 0) {
            String_ctor_cstr(prefix, "", false);
            Layout_formatCredits(credits, layout,
                                 F<int>(g_ChoiceWindow_creditValues_1471bc, F<int>(this, 0x2c) * 4));
            String_plus(creditsText, prefix, credits);
            String_dtor(credits);
            String_dtor(prefix);

            void *font = *g_ChoiceWindow_font_1471bc_a;
            int textWidth = PaintCanvas_GetTextWidth(canvas, font, creditsText);
            PaintCanvas_DrawString(canvas, font, creditsText,
                                   F<int>(this, 0x0) + (F<int>(this, 0x8) >> 1) - textWidth / 2,
                                   F<int>(this, 0x4) + F<int>(this, 0xc) - F<int>(this, 0x50),
                                   false);
            String_dtor(creditsText);
        }

        PaintCanvas_SetColor(canvas, 0xffffffff);
        void *font = *g_ChoiceWindow_font_1471bc_b;
        int textWidth = PaintCanvas_GetTextWidth(canvas, font, (char *)this + 0x3c);
        PaintCanvas_DrawString(canvas, font, (char *)this + 0x3c,
                               F<int>(this, 0x0) + (F<int>(this, 0x8) >> 1) - textWidth / 2,
                               F<int>(this, 0x4) + F<int>(this, 0x54), false);
    }

    ScrollTouchWindow_draw(F<void *>(this, 0x1c));

    if (F<uint8_t>(this, 0x59) != 0) {
        if (F<void *>(this, 0x10) != 0)
            TouchButton_draw(F<void *>(this, 0x10));

        if (F<void *>(this, 0x14) != 0) {
            TouchButton_draw(F<void *>(this, 0x14));
            if (F<void *>(this, 0x14) != 0) {
                TouchButton_getPosition(pos, F<void *>(this, 0x14));
                F<int>(g_ChoiceWindow_posTargetA_1471bc, 0x8) = (int)pos[0];
                TouchButton_getPosition(pos, F<void *>(this, 0x14));
                F<int>(g_ChoiceWindow_posTargetB_1471bc, 0x8) = (int)pos[1];
            }
        }

        if (F<void *>(this, 0x10) != 0) {
            TouchButton_getPosition(pos, F<void *>(this, 0x10));
            F<int>(g_ChoiceWindow_posTargetC_1471bc, 0xc) = (int)pos[0];
            TouchButton_getPosition(pos, F<void *>(this, 0x10));
            F<int>(g_ChoiceWindow_posTargetD_1471bc, 0xc) = (int)pos[1];

            if (F<void *>(this, 0x14) == 0) {
                TouchButton_getPosition(pos, F<void *>(this, 0x10));
                F<int>(g_ChoiceWindow_posTargetC_1471bc, 0x8) = (int)pos[0];
                TouchButton_getPosition(pos, F<void *>(this, 0x10));
                F<int>(g_ChoiceWindow_posTargetD_1471bc, 0x8) = (int)pos[1];
            }
        }

        *g_ChoiceWindow_dirtyFlag_1471bc = 1;
        if (F<void *>(this, 0x18) != 0)
            TouchButton_draw(F<void *>(this, 0x18));
    }
}
