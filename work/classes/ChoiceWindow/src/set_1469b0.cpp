#include "class.h"

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" String *GameText_getText(void *self, int id);
extern "C" int String_Compare(String *a, String const &b);
extern "C" void FModSound_play(void *self, int sound, void *pos, void *vel, float volume);
extern "C" void String_assign(void *self, String const &src);
extern "C" void String_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
extern "C" void Array_StringPtr_ctor(void *self);
extern "C" void Globals_getLineArray(void *self, void *font, String const &text, int width, void *array);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int width, int height, bool centered);
extern "C" void String_ctor_string(void *self, String const &src, bool copy);
extern "C" void ScrollTouchWindow_setText(void *self, void *title, void *message);
extern "C" void ArrayReleaseClasses_StringPtr(void *self);
extern "C" void *Array_StringPtr_dtor(void *self);
extern "C" void *TouchButton_dtor(void *self);
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
