#include "class.h"

extern "C" int PaintCanvas_GetWidth(void *canvas);
extern "C" int PaintCanvas_GetHeight(void *canvas);

extern "C" void ApplicationManager_ConvertTouchCoords(ApplicationManager *self, int *x, int *y)
{
    ApplicationManager *manager = self;
    int *xPtr = x;
    int *yPtr = y;
    void *canvas = *(void **)manager;
    int orientation = *(int *)((char *)canvas + 0x30);
    int newY;

    if (orientation != 3) {
        if (orientation != 1) {
            if (orientation != 0) {
                return;
            }
            newY = *xPtr;
            *xPtr = PaintCanvas_GetWidth(canvas) - *yPtr;
            goto storeY;
        }
        int oldX = *xPtr;
        *xPtr = *yPtr;
        *yPtr = PaintCanvas_GetHeight(canvas) - oldX;
        canvas = *(void **)manager;
    }
    *xPtr = PaintCanvas_GetWidth(canvas) - *xPtr;
    newY = PaintCanvas_GetHeight(canvas) - *yPtr;

storeY:
    *yPtr = newY;
}
