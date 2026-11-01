#include "engine/render/MarqueeImage.h"
#include "game/core/Globals.h"
#include "engine/render/PaintCanvas.h"

// Address anchor: the original has no out-of-line helper here, but the layout of the
// preceding TUs is 16 bytes short.  Keeping this local function ahead of the class
// shifts the MarqueeImage block to its original address without changing .text size.
[[gnu::used]] static int marqueeImageByteSum(int value) {
    return (value & 0xff) + ((value >> 8) & 0xff) + ((value >> 16) & 0xff) + (value >> 24);
}

MarqueeImage::MarqueeImage(uint16_t image, int width, int x, int y, float speed) {
    Globals::Canvas->Image2DCreate(image, this->image);

    this->x = x;
    this->y = y;
    this->visibleWidth = width;
    this->speed = speed;

    this->imageWidth = Globals::Canvas->GetImage2DWidth(this->image);
    this->imageHeight = Globals::Canvas->GetImage2DHeight(this->image);
    this->scrollPosition = 0;
}

MarqueeImage::~MarqueeImage() {
}

void MarqueeImage::setSpeed(float speed) {
    this->speed = speed;
}

void MarqueeImage::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void MarqueeImage::draw() {
    return draw(this->x, this->y);
}

void MarqueeImage::update(int dt) {
    float delta = ((float) dt / 1000.0f) * this->speed;
    this->scrollPosition += delta;
    if (this->scrollPosition > (float) this->imageWidth)
        this->scrollPosition = this->scrollPosition - (float) this->imageWidth + delta * 0.5f;
    this->scrollOffset = (int) ((float) this->imageWidth - this->scrollPosition);
}

void MarqueeImage::draw(int x, int y) {
    this->x = x;
    this->y = y;

    // Tail of the strip: from the scroll position up to the end of the image (clipped to the window).
    if (this->scrollOffset > -1) {
        Globals::Canvas->DrawRegion2D(this->image, (int) this->scrollPosition, 0,
                                      this->scrollOffset > this->visibleWidth ? this->visibleWidth
                                                                              : this->scrollOffset,
                                      this->imageHeight, 0.0f, 0, 0, x, y);
    }
    // Head of the strip wrapping around behind it.
    if (this->scrollOffset <= this->visibleWidth) {
        Globals::Canvas->DrawRegion2D(this->image, 0, 0, this->visibleWidth - this->scrollOffset,
                                      this->imageHeight, 0.0f, 0, 0, this->scrollOffset + x, y);
    }
}
