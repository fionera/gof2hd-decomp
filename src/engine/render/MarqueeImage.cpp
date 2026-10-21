#include "engine/render/MarqueeImage.h"
#include "game/core/Globals.h"
#include "engine/render/PaintCanvas.h"

MarqueeImage::~MarqueeImage() {
}

void MarqueeImage::setSpeed(float speed) {
    this->speed = speed;
}

void MarqueeImage::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
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
