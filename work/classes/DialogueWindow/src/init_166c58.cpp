#include "class.h"

extern "C" void *operator_new(unsigned size);
extern "C" void *operator_new_arr(unsigned size);
extern "C" void String_ctor_literal(StringSlot *self, const char *text, bool copy);
extern "C" void String_assign(String *self, StringSlot *other);
extern "C" void String_dtor(StringSlot *self);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);
extern "C" void ChoiceWindow_ctor(void *self);
extern "C" String *GameText_getText(void *self, int id);
extern "C" void TouchButton_ctor(void *self, String *text, int type, int x, int y, int width, int icon, int style);

__attribute__((visibility("hidden"))) extern int g_dw_briefingPartCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successPartCounts[];
__attribute__((visibility("hidden"))) extern const char g_dw_defaultAgentName[];
__attribute__((visibility("hidden"))) extern int *g_dw_screenWidth;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutInit;
__attribute__((visibility("hidden"))) extern int *g_dw_screenHeight;
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextInit;

static inline int half_round_to_zero(int v)
{
    return (v + ((unsigned)v >> 31)) >> 1;
}

extern "C" int DialogueWindow_init(DialogueWindow *self)
{
    StringSlot name;

    int *briefingOffsets = (int *)operator_new_arr(0x288);
    F<int *>(self, 0x5c) = briefingOffsets;
    int *successOffsets = (int *)operator_new_arr(0x288);
    F<int *>(self, 0x60) = successOffsets;

    int briefingSum = 0;
    int successSum = 0;
    for (int i = 0; i != 0xa2; ++i) {
        int briefingCount = g_dw_briefingPartCounts[i];
        int successCount = g_dw_successPartCounts[i];
        briefingOffsets[i] = briefingSum;
        successOffsets[i] = successSum;
        successSum += briefingCount;
        briefingSum += successCount;
    }

    String_ctor_literal(&name, g_dw_defaultAgentName, false);
    String_assign((String *)((char *)self + 0x34), &name);
    String_dtor(&name);

    F<void *>(self, 0x4c) = 0;
    F<void *>(self, 0x50) = 0;
    F<void *>(self, 0x58) = 0;
    F<void *>(self, 0x24) = 0;
    F<void *>(self, 0x0c) = 0;
    F<int>(self, 0x64) = -1;
    F<int>(self, 0x68) = 0;
    F<uint8_t>(self, 0x70) = 0;

    void *layout = *g_dw_layoutInit;
    int frameW = F<int>(layout, 0x54);
    int frameH = F<int>(layout, 0x58);
    F<int>(self, 0x1c) = frameW;
    F<int>(self, 0x20) = frameH;
    int frameX = half_round_to_zero(*g_dw_screenWidth) - half_round_to_zero(frameW);
    int frameY = half_round_to_zero(*g_dw_screenHeight) - half_round_to_zero(frameH);
    F<int>(self, 0x14) = frameX;
    F<int>(self, 0x18) = frameY;

    void *scroll = operator_new(0x24);
    int margin = F<int>(layout, 0x4c);
    ScrollTouchWindow_ctor(scroll,
                           frameX + margin * 2 + F<int>(layout, 0x2d4),
                           F<int>(layout, 0x08) + frameY,
                           frameW - margin * 2 - F<int>(layout, 0x2d4),
                           frameH - margin * 2 - F<int>(layout, 0x08) - F<int>(layout, 0x30),
                           false);
    F<void *>(self, 0x40) = scroll;

    void *choice = operator_new(0x5c);
    ChoiceWindow_ctor(choice);
    F<void *>(self, 0x50) = choice;

    void **gameText = g_dw_gameTextInit;
    void *button = operator_new(0xc8);
    String *label = GameText_getText(*gameText, 0xb3);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 5,
                     F<int>(self, 0x14) + margin,
                     F<int>(self, 0x18) - margin + F<int>(self, 0x20),
                     F<int>(layout, 0x50), 0x21, 4);
    F<void *>(self, 0x00) = button;

    button = operator_new(0xc8);
    label = GameText_getText(*gameText, 0xb4);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 6,
                     F<int>(self, 0x14) + F<int>(self, 0x1c) - margin,
                     F<int>(self, 0x18) - margin + F<int>(self, 0x20),
                     F<int>(layout, 0x50), 0x22, 4);
    F<void *>(self, 0x04) = button;

    button = operator_new(0xc8);
    label = GameText_getText(*gameText, 0x18b);
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    TouchButton_ctor(button, label, 0,
                     F<int>(self, 0x14) + half_round_to_zero(F<int>(self, 0x1c)),
                     F<int>(self, 0x18) + F<int>(self, 0x20) - margin,
                     F<int>(self, 0x1c) - margin * 4 - F<int>(layout, 0x50) * 2,
                     0x24, 4);
    F<uint8_t>(self, 0x54) = 0;
    F<void *>(self, 0x08) = button;
    return 0;
}
