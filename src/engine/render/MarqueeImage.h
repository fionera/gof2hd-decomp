#ifndef GOF2_MARQUEEIMAGE_H
#define GOF2_MARQUEEIMAGE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern PaintCanvas **g_MarqueeImage_canvas;

class MarqueeImage {
public:
    uint32_t image; // image2D handle (filled by PaintCanvas::Image2DCreate)
    int32_t imageWidth;
    int32_t imageHeight;
    int32_t x;
    int32_t y;
    int32_t visibleWidth;
    volatile float scrollPosition;
    float speed;
    int32_t scrollOffset;

    MarqueeImage(uint16_t image, int width, int x, int y, float speed);

    ~MarqueeImage();

    void setSpeed(float speed);

    void setPosition(int x, int y);

    void update(int dt);

    void draw();

    void draw(int x, int y);
};
#endif
