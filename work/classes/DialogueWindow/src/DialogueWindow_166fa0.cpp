#include "class.h"

extern "C" void String_ctor(String *self);
extern "C" void String_ctor_literal(StringSlot *self, const char *text, bool copy);
extern "C" void String_ctor_copy(StringSlot *self, String *text, bool copy);
extern "C" void String_dtor(StringSlot *self);
extern "C" void String_assign(String *self, String *other);
extern "C" int DialogueWindow_init(DialogueWindow *self);
extern "C" void ScrollTouchWindow_setText(void *self, StringSlot *style, StringSlot *text);
extern "C" void TouchButton_setVisible(void *self, bool visible);
extern "C" void *ImageFactory_loadChar(void *self, int *parts);
extern "C" void *TouchButton_dtor(void *self);
extern "C" void operator_delete(void *self);
extern "C" void *operator_new(unsigned size);
extern "C" String *GameText_getText(void *self, int id);
extern "C" void TouchButton_ctor(void *self, String *text, int type, int x, int y, int width, int icon, int style);

__attribute__((visibility("hidden"))) extern const char g_dw_emptyString[];
__attribute__((visibility("hidden"))) extern void **g_dw_imageFactoryCtor;
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextCtor;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutCtor;

extern "C" DialogueWindow *DialogueWindow_ctor_text(DialogueWindow *self, String *text, String *agentName, int *parts)
{
    StringSlot blank;
    StringSlot copy;

    String_ctor((String *)((char *)self + 0x28));
    String_ctor((String *)((char *)self + 0x34));
    DialogueWindow_init(self);

    void *scroll = F<void *>(self, 0x40);
    String_ctor_literal(&blank, g_dw_emptyString, false);
    String_ctor_copy(&copy, text, false);
    ScrollTouchWindow_setText(scroll, &blank, &copy);
    String_dtor(&copy);
    String_dtor(&blank);

    TouchButton_setVisible(F<void *>(self, 0x08), false);
    TouchButton_setVisible(F<void *>(self, 0x00), false);

    F<void *>(self, 0x0c) = ImageFactory_loadChar(*g_dw_imageFactoryCtor, parts);
    void *old = F<void *>(self, 0x04);
    if (old != 0) {
        operator_delete(TouchButton_dtor(old));
    }
    F<void *>(self, 0x04) = 0;

    void *button = operator_new(0xc8);
    String *buttonText = GameText_getText(*g_dw_gameTextCtor, 0x20c);
    void *layout = *g_dw_layoutCtor;
    int margin = F<int>(layout, 0x4c);
    int x = F<int>(self, 0x14) + F<int>(self, 0x1c) / 2;
    int y = F<int>(self, 0x18) + F<int>(self, 0x20) - margin;
    int width = F<int>(self, 0x1c) - margin * 4 - F<int>(layout, 0x50) * 2;
    TouchButton_ctor(button, buttonText, 0, x, y, width, 0x24, 4);
    F<void *>(self, 0x04) = button;

    String_assign((String *)((char *)self + 0x34), agentName);
    F<int>(self, 0x64) = -1;
    F<int>(self, 0x48) = 0;
    F<int>(self, 0x6c) = 0;
    return self;
}
