#include "class.h"

extern "C" String *GameText_getText(void *self, int id);
extern "C" void String_assign(void *self, String *src);
extern "C" void String_ctor_cstr(void *self, char const *text, bool copy);
extern "C" void String_ctor_string(void *self, String *src, bool copy);
extern "C" void String_ctor_int(void *self, int value);
extern "C" void String_dtor(void *self);
extern "C" int Achievements_getValue(void *self, int medal, int count);
extern "C" void Status_replaceHash(void *out, void *status, void *pattern, void *value);
extern "C" void String_plus(void *out, void *a, void *b);
extern "C" void ScrollTouchWindow_setTextCentered(void *self, bool centered);
extern "C" void PaintCanvas_Image2DCreate(void *self, unsigned short image, unsigned int *out);

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
    ScrollTouchWindow_setTextCentered(this->f_1c, true);

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

    this->f_2c = count;
    this->f_30 = medal;
    F<uint8_t>(this, 0x59) = 1;
    String_dtor(finalText);
}
