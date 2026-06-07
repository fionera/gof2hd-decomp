#include "class.h"

extern "C" void *operator_new_li(unsigned sz);                  // 0x6eb24
extern "C" void *ChoiceWindow_ctor(void *cw);                   // 0x74584
extern "C" void FModSound_play(int sound, int a, int b, int v, int pad); // 0x71548
struct GameText; struct GameTextHolder { GameText *obj; };
extern "C" void *GameText_getText(GameText *gt, int id);         // 0x72f70
extern "C" void ChoiceWindow_set(void *cw, void *title, void *body, bool); // 0x74ea8
__attribute__((visibility("hidden"))) extern int *gFmod;            // ldr [0xe4bb8]
__attribute__((visibility("hidden"))) extern GameTextHolder *gGameText; // ldr [0xe4bbc]

// Layout::initHelpWindow(String text)
extern "C" void Layout_initHelpWindow(Layout *self, void *text) {
    if (F<void *>(self, 0x3c4) == 0) {
        void *cw = operator_new_li(0x5c);
        ChoiceWindow_ctor(cw);
        F<void *>(self, 0x3c4) = cw;
    }
    FModSound_play(*gFmod, 0x7e, 0, 0, 0);
    void *cw = F<void *>(self, 0x3c4);
    void *title = GameText_getText(gGameText->obj, 0x187);
    ChoiceWindow_set(cw, title, text, false);
    F<uint8_t>(self, 0x0) = 1;
    F<uint8_t>(self, 0x3c0) = 0;
}
