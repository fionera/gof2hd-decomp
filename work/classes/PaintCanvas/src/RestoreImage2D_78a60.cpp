#include "class.h"

void RestoreImage2D(AbyssEngine::PaintCanvas *, char *img)
{
    char *m = *(char **)img;
    *(unsigned char *)(img + 0x14) = 0;
    char *s = *(char **)(m + 0x4);

    float w = (float)(unsigned int)*(unsigned short *)(img + 0x10);
    float h = (float)(unsigned int)*(unsigned short *)(img + 0x12);

    *(float *)(s + 0x0) = 0.0f;
    *(float *)(s + 0x4) = 0.0f;
    *(float *)(s + 0x8) = 0.0f;
    *(float *)(s + 0xc) = w;
    *(float *)(s + 0x10) = 0.0f;
    *(float *)(s + 0x14) = 0.0f;
    *(float *)(s + 0x18) = w;
    *(float *)(s + 0x1c) = h;
    *(float *)(s + 0x20) = 0.0f;
    *(float *)(s + 0x24) = 0.0f;
    *(float *)(s + 0x28) = h;
    *(float *)(s + 0x2c) = 0.0f;

    float invW = (float)(1.0 / (double)(unsigned int)*(unsigned short *)(img + 0xa));
    float invH = (float)(1.0 / (double)(unsigned int)*(unsigned short *)(img + 0x8));
    float u0 = (float)(unsigned int)*(unsigned short *)(img + 0xc);
    float v0 = (float)(unsigned int)*(unsigned short *)(img + 0xe);

    float *buf = *(float **)(m + 0x8);
    float a = u0 * invH;
    float b = v0 * invW;
    float c = (h + v0) * invW;
    float d = (w + u0) * invH;
    buf[0] = a;
    buf[1] = b;
    buf[2] = d;
    buf[3] = b;
    buf[4] = d;
    buf[5] = c;
    buf[6] = a;
    buf[7] = c;

    int *ipd = *(int **)(m + 0x2c);
    ipd[0] = 0x20000;
    ipd[1] = 1;
    ipd[2] = 0x20003;
}
