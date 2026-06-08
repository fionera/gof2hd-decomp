#include "class.h"

void *operator new(unsigned int size);
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

    this->f_58 = hasButtons;

    void *gameText = *g_ChoiceWindow_gameText_1469b0;
    bool defaultButtons = false;
    if (String_Compare(GameText_getText(gameText, 0x86), left) == 0)
        defaultButtons = String_Compare(GameText_getText(gameText, 0x87), right) == 0;

    if (width == -1)
        width = F<int>(*g_ChoiceWindow_defaultConfig_1469b0, 0x264);
    if (!defaultButtons)
        width = (int)((float)width * 0.85f);

    this->f_8 = width;
    this->f_0 = *g_ChoiceWindow_screenWidth_1469b0 / 2 - width / 2;
    FModSound_play(*g_ChoiceWindow_sound_1469b0, 0x7e, 0, 0, 0.0f);
    String_assign((char *)this + 0x20, title);

    void *scroll = this->f_1c;
    if (scroll != 0)
        operator delete(ScrollTouchWindow_dtor(scroll));
    this->f_1c = 0;

    void *lines = operator new(0xc);
    Array_StringPtr_ctor(lines);

    void *layout = *g_ChoiceWindow_layout_1469b0;
    Globals_getLineArray(*g_ChoiceWindow_globals_1469b0, *g_ChoiceWindow_lineFont_1469b0, message,
                         (this->f_8 - layout->f_4c * 2) -
                             layout->f_48,
                         lines);

    int contentHeight = layout->f_4 * lines->f_0 + layout->f_8 +
                        layout->f_4c * 2 + this->f_48 * 2 +
                        layout->f_30;
    int maxHeight = layout->f_278;
    if ((unsigned)contentHeight < (unsigned)maxHeight)
        maxHeight = contentHeight;
    this->f_c = maxHeight;

    if (y == -1)
        y = *g_ChoiceWindow_screenHeight_1469b0 / 2 - maxHeight / 2;
    this->f_4 = y;

    scroll = operator new(0x24);
    ScrollTouchWindow_ctor(scroll, this->f_0, layout->f_8 + y,
                           this->f_8,
                           (((maxHeight - this->f_48 * 2) - layout->f_8) -
                            layout->f_30) +
                               layout->f_2bc,
                           false);
    this->f_1c = scroll;

    String_ctor_string(titleCopy, title, false);
    String_ctor_string(messageCopy, message, false);
    ScrollTouchWindow_setText(scroll, titleCopy, messageCopy);
    String_dtor(messageCopy);
    String_dtor(titleCopy);

    ArrayReleaseClasses_StringPtr(lines);
    operator delete(Array_StringPtr_dtor(lines));

    void *button = this->f_10;
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    this->f_10 = 0;

    button = this->f_14;
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    this->f_14 = 0;

    if (hasButtons) {
        int baseWidth = layout->f_40;
        float scaled = (float)baseWidth * 1.5f;
        if (defaultButtons)
            scaled = (float)baseWidth;
        int buttonWidth = (int)scaled;

        button = operator new(0xc8);
        TouchButton_ctor(button, left, 0,
                         this->f_0 + this->f_8 / 2 - layout->f_4c / 2,
                         this->f_4 + this->f_c - this->f_48,
                         buttonWidth, 0x22, 4);
        this->f_10 = button;

        button = operator new(0xc8);
        TouchButton_ctor(button, right, 0,
                         this->f_0 + this->f_8 / 2 + layout->f_4c / 2,
                         this->f_4 + this->f_c - this->f_48,
                         buttonWidth, 0x21, 4);
        this->f_14 = button;
    } else {
        button = operator new(0xc8);
        TouchButton_ctor(button, single, 0,
                         this->f_0 + this->f_8 / 2,
                         this->f_4 + this->f_c - this->f_48,
                         layout->f_40, 0x24, 4);
        this->f_14 = button;
    }

    F<uint8_t>(this, 0x59) = 1;
}
