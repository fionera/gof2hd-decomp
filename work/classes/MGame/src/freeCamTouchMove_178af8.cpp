#include "class.h"

struct PlayerEgo;
struct Vector;

extern "C" int PlayerEgo_isMining(PlayerEgo *p);                      // 0x77adc
extern "C" void Vector_sub(Vector *out, Vector *a, Vector *b);        // 0x6ec38
extern "C" float Vector_length(Vector *v);                           // 0x6ec44
extern "C" void Vector_assign(Vector *dst, Vector *src);             // 0x6eb3c
extern "C" void TFC_zoomTarget(void *cam, float z);                  // 0x78178
// Pan-finish tail helper @0x1ac798 (no-op stack-guard-ok path).
extern "C" void MGame_freeCamPanDone(MGame *self, int p3);
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

__attribute__((visibility("hidden"))) extern int *g_fcGuard;   // @0x188b10 (stack guard [0])
__attribute__((visibility("hidden"))) extern float g_fcRotScale;  // @0x188d20 (DAT)
__attribute__((visibility("hidden"))) extern float g_fcZoomMax;   // @0x188d24
__attribute__((visibility("hidden"))) extern float g_fcZoomMin;   // @0x188d28

// MGame::freeCamTouchMove(int x, int y, void* touchId): rotate/zoom the free camera.
extern "C" void MGame_freeCamTouchMove(MGame *self, int x, int y, void *touchId) {
    int *guard = g_fcGuard;
    int g0 = *guard;

    if (PlayerEgo_isMining(F<PlayerEgo *>(self, 0x58)) != 0) {
        F<uint8_t>(self, 0x111) = 1;
        if (*guard - g0 == 0)
            return MGame_freeCamPanDone(self, (int)touchId);
        __stack_chk_fail();
        return;
    }
    F<uint8_t>(self, 0x111) = 0;

    int t0 = F<int>(self, 0x98);
    int t1 = F<int>(self, 0x9c);
    if (t0 == 0 || t1 == 0) {
        // Single-finger pan: accumulate rotation deltas.
        int dy = y - F<int>(self, 0x124);
        int dx = (int)touchId - F<int>(self, 0x128);
        F<int>(self, 0x124) = x;
        F<int>(self, 0x128) = (int)touchId;
        F<int>(self, 0x134) = dy;
        F<int>(self, 0x138) = dx;
        F<int>(self, 0x13c) = 0x3f800000;
        F<int>(self, 0x140) = 0x3f800000;
        F<float>(self, 0x118) += (float)dy;
        F<float>(self, 0x11c) += (float)dx;

        if (t0 == 0) {
            float v[4]; v[0] = (float)x; v[1] = (float)(int)touchId; v[2] = 0;
            Vector_assign((Vector *)((char *)self + 0xb0), (Vector *)v);
        } else if (t1 == 0) {
            float v[4]; v[0] = (float)x; v[1] = (float)(int)touchId; v[2] = 0;
            Vector_assign((Vector *)((char *)self + 0xa4), (Vector *)v);
        }
        if (*guard != g0) __stack_chk_fail();
        return;
    }

    // Two-finger pinch: zoom by the change in finger distance.
    Vector *base = (Vector *)((char *)self + 0xa4);
    float tmp[4];
    Vector_sub((Vector *)tmp, base, (Vector *)((char *)self + 0xb0));
    float oldLen = Vector_length((Vector *)tmp);
    float newLen = oldLen;

    if (F<int>(self, 0x98) == (int)touchId) {
        float v[4]; v[0] = (float)x; v[1] = (float)(int)touchId; v[2] = 0;
        Vector_sub((Vector *)tmp, base, (Vector *)v);
        newLen = Vector_length((Vector *)tmp);
        Vector_assign(base, (Vector *)v);
    } else if (F<int>(self, 0x9c) == (int)touchId) {
        float v[4]; v[0] = (float)x; v[1] = (float)(int)touchId; v[2] = 0;
        Vector_sub((Vector *)tmp, base, (Vector *)v);
        newLen = Vector_length((Vector *)tmp);
        Vector_assign((Vector *)((char *)self + 0xb0), (Vector *)v);
    }

    float zoom = F<float>(self, 0xbc) + (newLen - oldLen) * g_fcRotScale;
    F<float>(self, 0xbc) = zoom;
    if (zoom > g_fcZoomMax || zoom < g_fcZoomMin) {
        zoom = (zoom > g_fcZoomMax) ? g_fcZoomMax : g_fcZoomMin;
        F<float>(self, 0xbc) = zoom;
    }
    TFC_zoomTarget(F<void *>(self, 0xf4), zoom);

    if (*guard != g0) __stack_chk_fail();
}
