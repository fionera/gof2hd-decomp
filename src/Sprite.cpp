#include "gof2/Sprite.h"



// ---- setPosition_d0e52.cpp ----
void Sprite::setPosition(int x, int y)
{
    this->posX = x;
    this->posY = y;
}

// ---- setRefPixelPosition_d0e4c.cpp ----
void Sprite::setRefPixelPosition(int x, int y)
{
    this->posX = x;
    this->posY = y;
}

// ---- _Sprite_d0e30.cpp ----
Sprite::~Sprite()
{
    void *frames = this->frames;
    if (frames != 0) {
        operator delete(frames);
    }
    this->frames = 0;
}

// ---- defineReferencePixel_d0e46.cpp ----
void Sprite::defineReferencePixel(int x, int y)
{
    this->refPixelX = x;
    this->refPixelY = y;
}

// ---- nextFrame_d1018.cpp ----
void Sprite::nextFrame()
{
    return setFrame(this->currentFrame + 1);
}

// ---- draw_d0e68.cpp ----
__attribute__((visibility("hidden"))) extern void **g_Sprite_draw_image_canvas;
__attribute__((visibility("hidden"))) extern void **g_Sprite_draw_region_canvas;
extern "C" void PaintCanvas_DrawRegion2DForDraw(void *canvas, uint32_t image, int srcX, int srcY, int w, int h,
                                                long long zeroPair, int flags, int x, int y);

void Sprite::draw(float scaleX, float scaleY)
{
    uint32_t *frames = this->frames;

    if (frames == 0) {
        uint32_t image = this->image;
        int refX = this->refPixelX;
        int posX = this->posX;
        int posY = this->posY;
        int w = this->frameWidth;
        int h = this->frameHeight;
        int y = posY - refX;
        posY = 0;
        int x = posX - refX;
        PaintCanvas_DrawRegion2DForDraw(*g_Sprite_draw_region_canvas, image,
                                        this->frameSrcX, this->frameSrcY, w, h,
                                        (long long)(uint32_t)posY, posY, x, y);
        return;
    }

    void **holder = g_Sprite_draw_image_canvas;
    int frame = this->currentFrame;
    int refX = this->refPixelX;
    int posX = this->posX;
    void *holderValue = *holder;
    uint32_t image = frames[frame];
    int x = posX - refX;
    void *canvas = holderValue;

    if (scaleX == 1.0f || scaleY == 1.0f) {
        return PaintCanvas_DrawImage2D(canvas, image, x, this->posY - refX);
    }

    float frameHeight = (float)this->frameHeight;
    float frameWidth = (float)this->frameWidth;
    int y = this->posY - refX;
    int scaledHeight = (int)(frameHeight * scaleY);
    int scaledWidth = (int)(frameWidth * scaleX);
    int drawY = (int)((float)y - ((scaleY - 1.0f) * frameHeight) * 0.5f);
    int drawX = (int)((float)x - ((scaleX - 1.0f) * frameWidth) * 0.5f);

    PaintCanvas_DrawImage2DScaled(canvas, image, drawX, drawY, scaledWidth, scaledHeight, 0x11, 0x11, 0);
}

// ---- Sprite_d0d18.cpp ----
Sprite::Sprite(uint32_t image, int frameWidth, int frameHeight)
{
    void **holder = g_Sprite_canvas;

    this->frames = 0;
    this->image = image;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;

    void *canvas = *holder;
    this->imageWidth = PaintCanvas_GetImage2DWidth(canvas, image);
    int imageHeight = PaintCanvas_GetImage2DHeight(*holder, this->image);

    int rows = imageHeight / this->frameHeight;
    int columns = this->imageWidth / this->frameWidth;

    this->refPixelX = 0;
    this->refPixelY = 0;
    this->posX = 0;
    this->posY = 0;

    this->imageHeight = imageHeight;
    this->columns = columns;
    this->rows = rows;
    this->frameCount = rows * columns;
    setFrame(0);
}

// ---- prevFrame_d1020.cpp ----
void Sprite::prevFrame()
{
    return setFrame(this->currentFrame - 1);
}

// ---- drawRegion_d0f8c.cpp ----
void Sprite::drawRegion(int srcX, int srcY, int w, int h)
{
    uint32_t *frames = this->frames;
    void **holder = g_Sprite_canvas;
    void *canvas = *holder;

    if (frames != 0) {
        int refX = this->refPixelX;
        int posX = this->posX;
        int posY = this->posY;
        uint32_t image = frames[this->currentFrame];
        int y = posY + srcY - refX;
        int x = posX + srcX - refX;
        PaintCanvas_DrawRegion2D(canvas, image, srcX, srcY, w, h, 0.0f, 0, 0, x, y);
        return;
    }

    uint32_t image = this->image;
    int refX = this->refPixelX;
    int posX = this->posX;
    int posY = this->posY;
    int frameX = this->frameSrcX;
    int frameY = this->frameSrcY;
    int y = posY + srcY - refX;
    int x = posX + srcX - refX;
    srcX += frameX;
    srcY += frameY;
    PaintCanvas_DrawRegion2D(canvas, image, srcX, srcY, w, h, 0.0f, 0, 0, x, y);
}

// ---- Sprite_d0dd0.cpp ----
Sprite::Sprite(uint32_t *frames, int frameCount, int frameWidth, int frameHeight)
{
    void **holder = g_Sprite_canvas;

    this->frames = frames;
    this->image = (uint32_t)-1;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;

    void *canvas = *holder;
    this->imageWidth = PaintCanvas_GetImage2DWidth(canvas, *frames);
    int imageHeight = PaintCanvas_GetImage2DHeight(*holder, *this->frames);

    this->refPixelX = 0;
    this->refPixelY = 0;
    this->posX = 0;
    this->posY = 0;

    this->imageHeight = imageHeight;
    this->columns = 1;
    this->rows = 1;
    this->frameCount = frameCount;
    setFrame(0);
}

// ---- setFrame_d0d88.cpp ----
void Sprite::setFrame(int frame)
{
    if (frame < 0) {
        frame = -frame;
    }

    this->currentFrame = frame;
    if (frame >= this->frameCount) {
        frame = frame % this->frameCount;
        this->currentFrame = frame;
    }

    int frameWidth = this->frameWidth;
    int frameHeight = this->frameHeight;
    int columns = this->columns;
    int row = frame / columns;

    int column = frame - row * columns;
    int frameY = row * frameHeight;
    int frameX = column * frameWidth;

    this->frameSrcX = frameX;
    this->frameSrcY = frameY;
}
