#include "class.h"

// MenuTouchWindow::draw(). Caches the current button Y positions, publishes the active state
// to a global, optionally paints the background layout, then renders the widget set for the
// current menu state (this->0x16c). Each state's drawing is large (dozens of localized
// strings, boxes and thumbnails) and is delegated to its extern renderer doing the genuine
// work. r0=this.

extern "C" void _mtw_TouchButton_getPosition(void *out, void *btn);
extern "C" void _mtw_Layout_drawBG();

// Per-state renderers.
extern "C" void _mtw_draw_mainMenu(void *self);        // state 0
extern "C" void _mtw_draw_loadSaveList(void *self);    // states 1,2 (-> drawLoadSaveMenu)
extern "C" void _mtw_draw_optionsTabs(void *self);     // state 3
extern "C" void _mtw_draw_scrollA(void *self);         // state 4
extern "C" void _mtw_draw_audioOptions(void *self);    // state 7
extern "C" void _mtw_draw_keyConfig(void *self);       // state 8
extern "C" void _mtw_draw_missions(void *self);        // state 9
extern "C" void _mtw_draw_cinematic(void *self);       // state 0xb
extern "C" void _mtw_draw_buttonsB4(void *self);       // state 0xc
extern "C" void _mtw_draw_store(void *self);           // state 0xd
extern "C" void _mtw_draw_buttonsB0(void *self);       // state 0xe
extern "C" void _mtw_draw_scrollB(void *self);         // states 0xf,0x10
extern "C" void _mtw_draw_buttonsB8(void *self);       // state 0x11

extern void *const gDrawDrawingFlag __attribute__((visibility("hidden"))); // *holder -> int := 1
extern void *const gDrawStatePub    __attribute__((visibility("hidden"))); // *holder -> int := state
extern void *const gDrawPosX        __attribute__((visibility("hidden")));
extern void *const gDrawPosY        __attribute__((visibility("hidden")));
extern void *const gDrawCountObj    __attribute__((visibility("hidden")));
extern void *const gDrawColorSrc    __attribute__((visibility("hidden"))); // *holder -> color obj +0x118.. used as 0x168 gate

struct MenuTouchWindow { void draw(); };
void MenuTouchWindow::draw()
{
    void *self = this;

    *(int *)*(void **)gDrawDrawingFlag = 1;
    int state = i32(self, 0x16c);
    *(int *)*(void **)gDrawStatePub = state;

    // pick which button array supplies the cached positions for this state
    int fieldOff = 0x4;
    if (state == 0xc) fieldOff = 0xb4;
    if (state == 0xe) fieldOff = 0xb0;
    void **arr = (void **)pp(self, fieldOff);

    int *posX = (int *)*(void **)gDrawPosX;
    int *posY = (int *)*(void **)gDrawPosY;
    for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
        if (i < 10) {
            char buf[12];
            _mtw_TouchButton_getPosition(buf, ((void **)arr[1])[i]);
            posX[i] = (int)*(float *)(buf + 0);
            _mtw_TouchButton_getPosition(buf, ((void **)arr[1])[i]);
            posY[i] = (int)*(float *)(buf + 4);
        }
    }
    *(unsigned int *)*(void **)gDrawCountObj = *(unsigned int *)arr;

    // optionally paint the background layout (skipped for the in-game states 9 and 0xb)
    if (i32(self, 0x168) != 0) {
        unsigned int s = u32(self, 0x16c);
        if ((s | 4) != 0xd) {
            _mtw_Layout_drawBG();
        }
    }

    switch (u32(self, 0x16c)) {
    case 0:  _mtw_draw_mainMenu(self);     break;
    case 1:
    case 2:  _mtw_draw_loadSaveList(self); break;
    case 3:  _mtw_draw_optionsTabs(self);  break;
    case 4:  _mtw_draw_scrollA(self);      break;
    case 7:  _mtw_draw_audioOptions(self); break;
    case 8:  _mtw_draw_keyConfig(self);    break;
    case 9:  _mtw_draw_missions(self);     break;
    case 0xb: _mtw_draw_cinematic(self);   break;
    case 0xc: _mtw_draw_buttonsB4(self);   break;
    case 0xd: _mtw_draw_store(self);       break;
    case 0xe: _mtw_draw_buttonsB0(self);   break;
    case 0xf:
    case 0x10: _mtw_draw_scrollB(self);    break;
    case 0x11: _mtw_draw_buttonsB8(self);  break;
    default: break;
    }
}
