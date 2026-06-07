#include "class.h"

typedef void TouchMoveCallback(void *, int, int, void *);
typedef void TouchDragCallback(void *, int, int);

extern "C" void ApplicationManager_ConvertTouchCoords(ApplicationManager *self, int *x, int *y);

extern "C" void ApplicationManager_OnTouchMove(ApplicationManager *self, int xArg, int yArg, void *touch)
{
    void * volatile cookie = __stack_chk_guard;
    int x = xArg;
    int y = yArg;

    void *module = *(void **)((char *)self + 0x18);
    if (module != 0 && *(int *)((char *)self + 0x3c) == 5) {
        ApplicationManager_ConvertTouchCoords(self, &x, &y);
        module = *(void **)((char *)self + 0x18);
        void **vtable = *(void ***)module;
        ((TouchMoveCallback *)vtable[0x28 / 4])(module, x, y, touch);
        module = *(void **)((char *)self + 0x18);
        vtable = *(void ***)module;
        ((TouchDragCallback *)vtable[0x1c / 4])(module, x, y);
        *(int *)((char *)self + 0xb4) = x;
        *(int *)((char *)self + 0xb8) = y;
    }

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
