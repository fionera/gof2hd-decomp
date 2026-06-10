#include "Sprite.h"




// ---- setPosition_d0e52.cpp ----
void Sprite::setPosition(int x, int y)
{
    field<int32_t>(this, 0x10) = x;
    field<int32_t>(this, 0x14) = y;
}

// ---- setRefPixelPosition_d0e4c.cpp ----
void Sprite::setRefPixelPosition(int x, int y)
{
    field<int32_t>(this, 0x10) = x;
    field<int32_t>(this, 0x14) = y;
}

// ---- _Sprite_d0e30.cpp ----
Sprite::~Sprite()
{
    void *frames = field<void *>(this, 0x0);
    if (frames != 0) {
        operator delete(frames);
    }
    field<void *>(this, 0x0) = 0;
}

// ---- defineReferencePixel_d0e46.cpp ----
void Sprite::defineReferencePixel(int x, int y)
{
    field<int32_t>(this, 0x8) = x;
    field<int32_t>(this, 0xc) = y;
}

// ---- nextFrame_d1018.cpp ----
void Sprite::nextFrame()
{
    return setFrame(field<int32_t>(this, 0x38) + 1);
}

// ---- draw_d0e68.cpp ----
__attribute__((visibility("hidden"))) extern void **g_Sprite_draw_image_canvas;
__attribute__((visibility("hidden"))) extern void **g_Sprite_draw_region_canvas;
extern "C" void PaintCanvas_DrawRegion2DForDraw(void *canvas, uint32_t image, int srcX, int srcY, int w, int h,
                                                long long zeroPair, int flags, int x, int y);

void Sprite::draw(float scaleX, float scaleY)
{
    uint32_t *frames = field<uint32_t *>(this, 0x0);

    if (frames == 0) {
        uint32_t image = field<uint32_t>(this, 0x4);
        int refX = field<int32_t>(this, 0x8);
        int posX = field<int32_t>(this, 0x10);
        int posY = field<int32_t>(this, 0x14);
        int w = field<int32_t>(this, 0x18);
        int h = field<int32_t>(this, 0x1c);
        int y = posY - refX;
        posY = 0;
        int x = posX - refX;
        PaintCanvas_DrawRegion2DForDraw(*(alias_void_ptr *)g_Sprite_draw_region_canvas, image,
                                        field<int32_t>(this, 0x28), field<int32_t>(this, 0x2c), w, h,
                                        (long long)(uint32_t)posY, posY, x, y);
        return;
    }

    void **holder = g_Sprite_draw_image_canvas;
    int frame = field<int32_t>(this, 0x38);
    int refX = field<int32_t>(this, 0x8);
    int posX = field<int32_t>(this, 0x10);
    void *holderValue = *holder;
    uint32_t image = frames[frame];
    int x = posX - refX;
    void *canvas = holderValue;

    if (scaleX == 1.0f || scaleY == 1.0f) {
        return PaintCanvas_DrawImage2D(canvas, image, x, field<int32_t>(this, 0x14) - refX);
    }

    float frameHeight = (float)field<int32_t>(this, 0x1c);
    float frameWidth = (float)field<int32_t>(this, 0x18);
    int y = field<int32_t>(this, 0x14) - refX;
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

    field<alias_void_ptr>(this, 0x0) = 0;
    field<alias_u32>(this, 0x4) = image;
    field<alias_i32>(this, 0x18) = frameWidth;
    field<alias_i32>(this, 0x1c) = frameHeight;

    void *canvas = *(alias_void_ptr *)holder;
    field<int32_t>(this, 0x20) = PaintCanvas_GetImage2DWidth(canvas, image);
    int imageHeight = PaintCanvas_GetImage2DHeight(*(alias_void_ptr *)holder, field<uint32_t>(this, 0x4));

    int rows = imageHeight / field<int32_t>(this, 0x1c);
    int columns = field<int32_t>(this, 0x20) / field<int32_t>(this, 0x18);

    field<int32_t>(this, 0x8) = 0;
    field<int32_t>(this, 0xc) = 0;
    field<int32_t>(this, 0x10) = 0;
    field<int32_t>(this, 0x14) = 0;

    field<int32_t>(this, 0x24) = imageHeight;
    field<int32_t>(this, 0x30) = columns;
    field<int32_t>(this, 0x34) = rows;
    field<int32_t>(this, 0x3c) = rows * columns;
    setFrame(0);
}

// ---- prevFrame_d1020.cpp ----
void Sprite::prevFrame()
{
    return setFrame(field<int32_t>(this, 0x38) - 1);
}

// ---- drawRegion_d0f8c.cpp ----
void Sprite::drawRegion(int srcX, int srcY, int w, int h)
{
    uint32_t *frames = field<uint32_t *>(this, 0x0);
    void **holder = g_Sprite_canvas;
    void *canvas = *holder;

    if (frames != 0) {
        int refX = field<int32_t>(this, 0x8);
        int posX = field<int32_t>(this, 0x10);
        int posY = field<int32_t>(this, 0x14);
        uint32_t image = frames[field<int32_t>(this, 0x38)];
        int y = posY + srcY - refX;
        int x = posX + srcX - refX;
        PaintCanvas_DrawRegion2D(canvas, image, srcX, srcY, w, h, 0.0f, 0, 0, x, y);
        return;
    }

    uint32_t image = field<uint32_t>(this, 0x4);
    int refX = field<int32_t>(this, 0x8);
    int posX = field<int32_t>(this, 0x10);
    int posY = field<int32_t>(this, 0x14);
    int frameX = field<int32_t>(this, 0x28);
    int frameY = field<int32_t>(this, 0x2c);
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

    field<alias_void_ptr>(this, 0x0) = frames;
    field<alias_i32>(this, 0x4) = -1;
    field<alias_i32>(this, 0x18) = frameWidth;
    field<alias_i32>(this, 0x1c) = frameHeight;

    void *canvas = *(alias_void_ptr *)holder;
    field<int32_t>(this, 0x20) = PaintCanvas_GetImage2DWidth(canvas, *frames);
    int imageHeight = PaintCanvas_GetImage2DHeight(*(alias_void_ptr *)holder, *field<uint32_t *>(this, 0x0));

    field<int32_t>(this, 0x8) = 0;
    field<int32_t>(this, 0xc) = 0;
    field<int32_t>(this, 0x10) = 0;
    field<int32_t>(this, 0x14) = 0;

    field<int32_t>(this, 0x24) = imageHeight;
    field<int32_t>(this, 0x30) = 1;
    field<int32_t>(this, 0x34) = 1;
    field<int32_t>(this, 0x3c) = frameCount;
    setFrame(0);
}

// ---- setFrame_d0d88.cpp ----
void Sprite::setFrame(int frame)
{
    if (frame < 0) {
        frame = -frame;
    }

    field<int32_t>(this, 0x38) = frame;
    if (frame >= field<int32_t>(this, 0x3c)) {
        frame = frame % field<int32_t>(this, 0x3c);
        field<int32_t>(this, 0x38) = frame;
    }

    int frameWidth = field<int32_t>(this, 0x18);
    int frameHeight = field<int32_t>(this, 0x1c);
    int columns = field<int32_t>(this, 0x30);
    int row = frame / columns;

    int column = frame - row * columns;
    int frameY = row * frameHeight;
    int frameX = column * frameWidth;

    field<int32_t>(this, 0x28) = frameX;
    field<int32_t>(this, 0x2c) = frameY;
}
