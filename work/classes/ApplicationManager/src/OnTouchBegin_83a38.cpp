#include "class.h"

typedef void TouchBeginCallback(void *, int, int, void *);
typedef void TouchPressCallback(void *, int, int);

extern "C" int g_touchMode;
extern "C" char g_touchToggle;
extern "C" int g_touchValue;
extern "C" float g_touchFloat;
extern "C" void ApplicationManager_ConvertTouchCoords(ApplicationManager *self, int *x, int *y);
extern "C" int PaintCanvas_GetWidth(void *canvas);
extern "C" int PaintCanvas_GetHeight(void *canvas);

extern "C" void ApplicationManager_OnTouchBegin(ApplicationManager *self, int xArg, int yArg, void *touch)
{
    void * volatile cookie = __stack_chk_guard;
    int x = xArg;
    int y = yArg;

    void *module = *(void **)((char *)self + 0x18);
    if (module != 0 && *(int *)((char *)self + 0x3c) == 5) {
        ApplicationManager_ConvertTouchCoords(self, &x, &y);
        module = *(void **)((char *)self + 0x18);
        void **vtable = *(void ***)module;
        ((TouchBeginCallback *)vtable[0x24 / 4])(module, x, y, touch);
        module = *(void **)((char *)self + 0x18);
        vtable = *(void ***)module;
        ((TouchPressCallback *)vtable[0x18 / 4])(module, x, y);
        *(int *)((char *)self + 0xb4) = x;
        *(int *)((char *)self + 0xb8) = y;

        void *engine = *(void **)((char *)self + 0xa8);
        int mode = g_touchMode;
        if (mode <= 3) {
            void *canvas = *(void **)self;
            if (mode == 0 && x <= 0x31 && y <= 0x31) {
                g_touchMode = 1;
            } else if (mode == 1 && x > PaintCanvas_GetWidth(canvas) - 0x32 &&
                       y > PaintCanvas_GetHeight(canvas) - 0x32) {
                g_touchMode = 2;
            } else if (mode == 2 && x <= 0x31 && y > PaintCanvas_GetHeight(canvas) - 0x32) {
                *(bool *)((char *)engine + 0x74) = !*(bool *)((char *)engine + 0x74);
            } else if (mode == 3 && y <= 0x31 && x > PaintCanvas_GetWidth(canvas) - 0x32) {
                g_touchMode = 4;
            }
        } else if (*(bool *)((char *)engine + 0x74)) {
            if (y < 100) {
                g_touchToggle ^= 1;
            } else {
                int height = PaintCanvas_GetHeight(*(void **)self);
                int width = PaintCanvas_GetWidth(*(void **)self);
                int half = width / 2;
                if (height - 100 < y) {
                    g_touchValue = x < half ? 0 : 1;
                } else {
                    g_touchFloat += x < half ? -1.0f : 1.0f;
                }
            }
        }
    }

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
