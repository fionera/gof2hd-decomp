#include "engine/render/Sprite.h"
#include "game/core/Globals.h"
#include "engine/core/AbyssEngine.h"
#include "engine/render/PaintCanvas.h"

Sprite::Sprite(uint32_t image, int frameWidth, int frameHeight) {
    this->frames = nullptr;
    this->image = image;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->imageWidth = Globals::Canvas->GetImage2DWidth(this->image);
    this->imageHeight = Globals::Canvas->GetImage2DHeight(this->image);
    this->columns = this->imageWidth / this->frameWidth;
    this->rows = this->imageHeight / this->frameHeight;
    this->refPixelX = 0;
    this->refPixelY = 0;
    this->posX = 0;
    this->posY = 0;
    this->frameCount = this->rows * this->columns;
    setFrame(0);
}

void Sprite::setFrame(int frame) {
    if (frame < 0) {
        frame = -frame;
    }

    this->currentFrame = frame;
    if (frame >= this->frameCount) {
        frame = frame % this->frameCount;
        this->currentFrame = frame;
    }

    this->frameSrcX = (frame % this->columns) * this->frameWidth;
    this->frameSrcY = (frame / this->columns) * this->frameHeight;
}

Sprite::Sprite(uint32_t *frames, int frameCount, int frameWidth, int frameHeight) {
    this->frames = frames;
    this->image = (uint32_t) -1;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->imageWidth = Globals::Canvas->GetImage2DWidth(this->frames[0]);
    this->imageHeight = Globals::Canvas->GetImage2DHeight(this->frames[0]);
    this->refPixelX = 0;
    this->refPixelY = 0;
    this->posX = 0;
    this->posY = 0;
    this->columns = 1;
    this->rows = 1;
    this->frameCount = frameCount;
    setFrame(0);
}

Sprite::~Sprite() {
    delete[] this->frames;
    this->frames = nullptr;
}

void Sprite::defineReferencePixel(int x, int y) {
    this->refPixelX = x;
    this->refPixelY = y;
}

void Sprite::setRefPixelPosition(int x, int y) {
    this->posX = x;
    this->posY = y;
}

void Sprite::setPosition(int x, int y) {
    this->posX = x;
    this->posY = y;
}

int Sprite::getFrame() {
    return this->currentFrame;
}

int Sprite::getRawFrameCount() {
    return this->frameCount;
}

int Sprite::getFrameWidth() {
    return this->frameWidth;
}

int Sprite::getFrameHeight() {
    return this->frameHeight;
}

// Note: the original uses refPixelX for both the x and the y reference offset.
void Sprite::draw(float scaleX, float scaleY) {
    if (this->frames != nullptr) {
        uint32_t image = this->frames[this->currentFrame];
        int x = this->posX - this->refPixelX;
        PaintCanvas *canvas = Globals::Canvas;
        if (scaleX == 1.0f || scaleY == 1.0f)
            return canvas->DrawImage2D(image, x, this->posY - this->refPixelX);

        float frameWidth = (float) this->frameWidth;
        float frameHeight = (float) this->frameHeight;
        canvas->DrawImage2D(
            image, (int) ((float) x - (scaleX - 1.0f) * frameWidth * 0.5f),
            (int) ((float) (this->posY - this->refPixelX) - (scaleY - 1.0f) * frameHeight * 0.5f),
            (int) (frameWidth * scaleX), (int) (frameHeight * scaleY), 0x11, 0x11, 0);
    } else {
        Globals::Canvas->DrawRegion2D(this->image, this->frameSrcX, this->frameSrcY, this->frameWidth,
                                      this->frameHeight, 0.0f, 0, 0, this->posX - this->refPixelX,
                                      this->posY - this->refPixelX);
    }
}

void Sprite::drawRegion(int srcX, int srcY, int w, int h) {
    if (this->frames != nullptr) {
        Globals::Canvas->DrawRegion2D(this->frames[this->currentFrame], srcX, srcY, w, h, 0.0f, 0, 0,
                                      this->posX + srcX - this->refPixelX, srcY - this->refPixelX + this->posY);
    } else {
        Globals::Canvas->DrawRegion2D(this->image, this->frameSrcX + srcX, this->frameSrcY + srcY, w, h, 0.0f, 0, 0,
                                      this->posX + srcX - this->refPixelX, srcY - this->refPixelX + this->posY);
    }
}

void Sprite::nextFrame() {
    return setFrame(this->currentFrame + 1);
}

void Sprite::prevFrame() {
    return setFrame(this->currentFrame - 1);
}
