#ifndef GOF2_LENSFLARE_H
#define GOF2_LENSFLARE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include <cmath>

// Galaxy on Fire 2 -- sun/star lens-flare effect.
// Owns three Image2D sprite handles and renders them along the screen-centre ->
// source axis, tinted from a small palette and faded by distance. Drawing is done
// through the engine PaintCanvas.

namespace AbyssEngine { class PaintCanvas; }
using ::AbyssEngine::PaintCanvas;

class LensFlare {
public:
    float intensity;                 // current flare alpha
    AbyssEngine::PaintCanvas *canvas;
    int width;
    int height;
    uint32_t *images;                // three Image2D sprite handles

    explicit LensFlare(AbyssEngine::PaintCanvas *canvas);
    ~LensFlare();

    void render2D(float srcX, float srcY, float alpha, int colorIndex);
    void update(int);
};

#endif
