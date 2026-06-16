#include "engine/render/LensFlare.h"

using AbyssEngine::PaintCanvas;

// PC-relative pointer-to-pointer global: the engine PaintCanvas singleton used to
// create the flare's three Image2D sprites, and the same holder reused at draw time.
extern PaintCanvas *const gLensFlareCanvas __attribute__((visibility("hidden")));
extern PaintCanvas *const gLF_Canvas2 __attribute__((visibility("hidden")));

// PC-relative palette tables (R/G/B per flare-colour index).
extern const uint32_t gFlareR[] __attribute__((visibility("hidden")));
extern const uint32_t gFlareG[] __attribute__((visibility("hidden")));
extern const uint32_t gFlareB[] __attribute__((visibility("hidden")));

LensFlare::LensFlare(PaintCanvas *canvas)
{
    this->images = new uint32_t[3];
    PaintCanvas *singleton = *(PaintCanvas **)gLensFlareCanvas;
    for (int i = 0; i != 3; ++i)
        LensFlare_Image2DCreate(singleton, (short)(i + 0x508), &this->images[i]);
    this->width = LensFlare_GetWidth(canvas);
    this->height = LensFlare_GetHeight(canvas);
    this->canvas = canvas;
}

LensFlare::~LensFlare()
{
    delete[] this->images;
    this->images = nullptr;
}

void LensFlare::render2D(float srcX, float srcY, float alpha, int colorIndex)
{
    // --- tint from palette -------------------------------------------------
    uint32_t r, g, b;
    if ((unsigned)colorIndex <= 4) {
        r = gFlareR[colorIndex];
        g = gFlareG[colorIndex];
        b = gFlareB[colorIndex];
    } else {
        r = 0xff;
        g = 0xff;
        b = 0xff;
    }

    // alpha < 0 -> nothing to draw.
    if (alpha < 0.0f)
        return;

    // --- horizontal cull against the [-w, 2w] guard band -------------------
    int w = this->width;
    if ((float)(-w) >= srcX)
        return;
    if ((float)(w * 2) <= srcX)
        return;

    // --- vertical cull against the [-h, 2h] guard band ---------------------
    int h = this->height;
    if ((float)(-h) >= srcY)
        return;
    if ((float)(h * 2) <= srcY)
        return;

    // --- distance from screen centre, fade -> this->intensity --------------
    float cx = (float)(w >> 1);
    float cy = (float)(h >> 1);
    float dx = cx - srcX;
    float dy = cy - srcY;
    float dist = LensFlare_sqrtf(dx * dx + dy * dy);

    // normalize the centre->source direction (guarded by dist > 0).
    float ndx = -dx * 1.5f;
    float ndy = -dy * 1.5f;
    if (dist > 0.0f) {
        ndx = ndx / dist;
        ndy = ndy / dist;
    }

    // fade = palette-scaled (1 - dist / (h/2)); larger colorIndex uses a different base.
    float halfH = (float)(h >> 1);
    float base = (colorIndex == 5) ? 0.875f : 0.5f;
    float fade = base * (1.0f - dist / halfH);
    this->intensity = fade;

    PaintCanvas *canvas = *(PaintCanvas **)gLF_Canvas2;
    void *img0 = (void *)(uintptr_t)this->images[0];
    void *img1 = (void *)(uintptr_t)this->images[1];
    void *img2 = (void *)(uintptr_t)this->images[2];

    // --- sprite 0: the main flare at the source, faded by (1-intensity) -----
    {
        uint32_t a = (uint32_t)(1.0f - (1.0f - fade));
        uint32_t color = (a << 0) | (r << 8) | (g << 16) | (b << 24);
        LensFlare_setColor(canvas, color);
        float px = cx + ndx * 0.5f * 0.5f * 17.0f;
        float py = cy + ndy * 0.5f * 0.5f * 17.0f;
        LensFlare_drawScaled(canvas, img0, (int)px, (int)py);
    }

    // --- sprite at image[0] again, scaled by 0.75 around source -------------
    {
        int iw = LensFlare_imgHandle(img0);
        float s = (float)iw * 0.75f;
        LensFlare_drawScaled(canvas, img0, (int)(cx + ndx * s), (int)(cy + ndy * s));
    }

    // --- sprite image[1]: two passes at 0.5 and 0.25 offsets ----------------
    {
        int ih = LensFlare_imgHandle(img1);
        float s = (float)ih * 0.5f;
        LensFlare_drawScaled(canvas, img1, (int)(cx + ndx * s * 17.0f),
                             (int)(cy + ndy * s * 17.0f));
        float s2 = (float)ih * 0.25f;
        LensFlare_drawScaled(canvas, img1, (int)(cx + ndx * s2 * 17.0f),
                             (int)(cy + ndy * s2 * 17.0f));
    }

    // --- re-fade and draw image[2] (the inner glow) -------------------------
    {
        uint32_t a = (uint32_t)(1.0f - this->intensity);
        uint32_t color = (a << 0) | (r << 8) | (g << 16) | (b << 24);
        LensFlare_setColor(canvas, color);
    }
    if (1.0f - this->intensity < 0.625f) {
        int iw = LensFlare_imgWidth(canvas, img0);
        int ih = LensFlare_imgWidth(canvas, img0);
        float s = (float)iw * 1.25f;
        LensFlare_drawScaled(canvas, img0, (int)(cx + ndx * (float)ih * 0.125f * 17.0f),
                             (int)(cy + ndy * s));
    }

    // --- streak / final overlay --------------------------------------------
    {
        uint32_t a = (uint32_t)(1.0f - this->intensity);
        uint32_t color = (a << 0) | (r << 8) | (g << 16) | (b << 24);
        LensFlare_setColor(canvas, color);
        int iw0 = LensFlare_imgWidth(canvas, img2);
        int iw1 = LensFlare_imgWidth(canvas, img2);
        float s = (float)iw0 * 2.0f;
        LensFlare_drawScaled(canvas, img2, (int)(cx + ndx * (float)iw1 * 0.5f),
                             (int)(cy + ndy * s));
    }

    // final blended draw of image[2] if intensity > 0.
    if (this->intensity > 0.0f) {
        int saved = (int)(uintptr_t)this->canvas;
        LensFlare_pushState(this->canvas);
        uint32_t color = (g << 16) | (b << 8) | (r << 24);
        uint32_t shifted = color + (uint32_t)(int)this->intensity;
        LensFlare_setBlend(this->canvas, shifted);
        LensFlare_drawFinal(this->canvas, this->images, 0, this->height);
        LensFlare_restoreState(this->canvas, saved);
    }
}

void LensFlare::update()
{
    this->intensity = 0.0f;
}
