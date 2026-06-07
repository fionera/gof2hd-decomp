#include "class.h"
#include <arm_neon.h>

extern "C" void glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices);

extern "C" void Engine_DrawQuad(Engine *self, int x, int y, int width, int height)
{
    float fx = (float)x;
    float fy = (float)y;
    float right = (float)(x + width);
    float bottom = (float)(height + y);

    void *mesh = *(void **)((char *)self + 0x380);
    float *positions = *(float **)((char *)mesh + 0x4);
    positions[0] = fx;
    positions[1] = fy;
    positions[3] = right;
    positions[4] = fy;
    positions[6] = right;
    positions[7] = bottom;
    positions[9] = fx;
    positions[10] = bottom;

    static const float uvs[8] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };
    float *uv = *(float **)((char *)mesh + 0x8);
    float32x4_t uv0 = vld1q_f32(uvs);
    float32x4_t uv1 = vld1q_f32(uvs + 4);
    vst1q_f32(uv, uv0);
    vst1q_f32(uv + 4, uv1);

    return glDrawElements(4, *(uint16_t *)((char *)mesh + 0x28), 0x1403,
                          *(void **)((char *)mesh + 0x2c));
}
