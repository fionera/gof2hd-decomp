#include "class.h"

// PC-relative hidden global: pointer to the PaintCanvas singleton holder.
__attribute__((visibility("hidden"))) extern void **gPaintCanvasHolder; // ldr [0xe3c38]
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, int imageId); // blx 0x72d84

// Layout::getFooterTransitionWidth()
extern "C" int Layout_getFooterTransitionWidth(Layout *self) {
    void **holder = gPaintCanvasHolder;
    int w1 = PaintCanvas_GetImage2DWidth(*holder, F<int>(self, 0x33c));
    int w2 = PaintCanvas_GetImage2DWidth(*holder, F<int>(self, 0x334));
    return w2 + w1;
}
