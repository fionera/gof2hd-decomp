#include "class.h"

// PC-relative palette tables (R/G/B per flare-color index, DAT_0012d0bc/c0/c4) and the
// engine PaintCanvas singleton holder (DAT_0012d1c4 etc.).
extern const uint32_t gFlareR[] __attribute__((visibility("hidden")));
extern const uint32_t gFlareG[] __attribute__((visibility("hidden")));
extern const uint32_t gFlareB[] __attribute__((visibility("hidden")));
extern void *const gLF_Canvas2 __attribute__((visibility("hidden")));

// LensFlare::render2D(float srcX, float srcY, float alpha, int colorIndex)
//
// Faithful reconstruction (logic, not byte-exact): pick the flare tint from a palette by
// colorIndex (>4 -> white), cull when alpha < 0 or the source point lies outside the
// [-w,2w]x[-h,2h] guard band, then fade by distance from screen-centre and draw the three
// flare sprites at interpolated positions along the centre->source vector, finishing with a
// streak overlay.
struct LensFlare { void render2D(float srcX, float srcY, float alpha, int colorIndex); };
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
    int w = i32(this, 0x08);
    if ((float)(-w) >= srcX)
        return;
    if ((float)(w * 2) <= srcX)
        return;

    // --- vertical cull against the [-h, 2h] guard band ---------------------
    int h = i32(this, 0x0c);
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
    f32(this, 0x00) = fade;

    void *canvas = *(void **)gLF_Canvas2;
    void **images = (void **)pp(this, 0x10);

    // --- sprite 0: the main flare at the source, faded by (1-intensity) -----
    {
        uint32_t a = (uint32_t)(1.0f - (1.0f - fade));
        uint32_t color = (a << 0) | (r << 8) | (g << 16) | (b << 24);
        LensFlare_setColor(canvas, color);
        float px = cx + ndx * 0.5f * 0.5f * 17.0f;
        float py = cy + ndy * 0.5f * 0.5f * 17.0f;
        LensFlare_drawScaled(canvas, images[0], (int)px, (int)py);
    }

    // --- sprite at image[0] again, scaled by 0.75 around source -------------
    {
        int iw = LensFlare_imgHandle(images[0]);
        float s = (float)iw * 0.75f;
        LensFlare_drawScaled(canvas, images[0], (int)(cx + ndx * s), (int)(cy + ndy * s));
    }

    // --- sprite image[1]: two passes at 0.5 and 0.25 offsets ----------------
    {
        int ih = LensFlare_imgHandle(images[1]);
        float s = (float)ih * 0.5f;
        LensFlare_drawScaled(canvas, images[1], (int)(cx + ndx * s * 17.0f),
                             (int)(cy + ndy * s * 17.0f));
        float s2 = (float)ih * 0.25f;
        LensFlare_drawScaled(canvas, images[1], (int)(cx + ndx * s2 * 17.0f),
                             (int)(cy + ndy * s2 * 17.0f));
    }

    // --- re-fade and draw image[2] (the inner glow) -------------------------
    {
        uint32_t a = (uint32_t)(1.0f - f32(this, 0x00));
        uint32_t color = (a << 0) | (r << 8) | (g << 16) | (b << 24);
        LensFlare_setColor(canvas, color);
    }
    if (1.0f - f32(this, 0x00) < 0.625f) {
        int iw = LensFlare_imgWidth(canvas, images[0]);
        int ih = LensFlare_imgWidth(canvas, images[0]);
        float s = (float)iw * 1.25f;
        LensFlare_drawScaled(canvas, images[0], (int)(cx + ndx * (float)ih * 0.125f * 17.0f),
                             (int)(cy + ndy * s));
    }

    // --- streak / final overlay --------------------------------------------
    {
        uint32_t a = (uint32_t)(1.0f - f32(this, 0x00));
        uint32_t color = (a << 0) | (r << 8) | (g << 16) | (b << 24);
        LensFlare_setColor(canvas, color);
        int iw0 = LensFlare_imgWidth(canvas, images[2]);
        int iw1 = LensFlare_imgWidth(canvas, images[2]);
        float s = (float)iw0 * 2.0f;
        LensFlare_drawScaled(canvas, images[2], (int)(cx + ndx * (float)iw1 * 0.5f),
                             (int)(cy + ndy * s));
    }

    // final blended draw of image[2] if intensity > 0.
    if (f32(this, 0x00) > 0.0f) {
        int saved = (int)(unsigned long)pp(this, 0x04);
        LensFlare_pushState(pp(this, 0x04));
        uint32_t color = (g << 16) | (b << 8) | (r << 24);
        uint32_t shifted = color + (uint32_t)(int)f32(this, 0x00);
        LensFlare_setBlend(pp(this, 0x04), shifted);
        LensFlare_drawFinal(pp(this, 0x04), pp(this, 0x10), 0, i32(this, 0x0c));
        LensFlare_restoreState(pp(this, 0x04), saved);
    }
}
