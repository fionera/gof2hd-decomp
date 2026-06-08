#include "class.h"

// ParticleSystemSprite::render(PaintCanvas*, unsigned int handle, unsigned int texture, BlendMode)
// Draws the sprite system with an identity world transform: binds the texture and blend mode,
// builds an identity 3x4 matrix on the stack, pushes it as the world-view matrix, and issues
// the sprite-system draw. A handle of -1 means "no system" and is skipped.

struct ParticleSystemSprite {
    void render(void *canvas, int handle, unsigned int texture, int blend);
};

extern "C" void PaintCanvas_SetTexture(unsigned int canvas, unsigned int texture);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int blend);
extern "C" void PaintCanvas_SetWorldViewMatrix(void *canvas, const void *matrix);
extern "C" void PaintCanvas_DrawSpriteSystem(unsigned int canvas);

// Constant {0,0,1,0} tail used to fill the identity matrix (matches the original literals).
static const float kIdentTail[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

void ParticleSystemSprite::render(void *canvas, int handle, unsigned int texture, int blend)
{
    if (handle == -1)
        return;

    PaintCanvas_SetTexture((unsigned int)(unsigned long)canvas, texture);
    PaintCanvas_SetBlendMode(canvas, blend);

    // Identity affine transform (3x4 rows + trailing vector).
    float m[16];
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = kIdentTail[1];
    m[3] = kIdentTail[2];
    m[4] = kIdentTail[3];
    m[5] = 1.0f;        // local_44
    m[6] = 0.0f;
    m[7] = kIdentTail[1];
    m[8] = kIdentTail[2];
    m[9] = kIdentTail[3];
    m[10] = 0.0f;
    m[11] = kIdentTail[1];
    m[12] = kIdentTail[2];
    m[13] = kIdentTail[3];
    m[14] = 1.0f;       // local_20

    PaintCanvas_SetWorldViewMatrix(canvas, m);
    PaintCanvas_DrawSpriteSystem((unsigned int)(unsigned long)canvas);
}
