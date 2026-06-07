#include "class.h"

struct TargetFollowCamera;
struct Vector;
__attribute__((visibility("hidden"))) extern int *g_fcb_guard; // DAT_188af4 (canary anchor read)

extern "C" Vector *TFC_getCamOffset(TargetFollowCamera *c);
extern "C" float AEMath_VectorLength(Vector *v);
extern "C" void *Vector_assign(Vector *dst, void *src);

// MGame::freeCamTouchBegin(int, int, void*): start a free-camera drag. Records the start
// touch and seeds the camera-offset Vector (built on the stack -> compiler canary).
extern "C" void MGame_freeCamTouchBegin(MGame *self, int x, int y, int id) {
    char buf[12];
    float fy = (float)y;
    float fx = (float)x;
    int slot;
    if (F<int>(self, 0x98) == 0) {
        if (F<int>(self, 0x9c) == 0) F<int>(self, 0xa0) = 0;
        *(volatile float *)(buf + 4) = fy;
        *(volatile float *)(buf + 0) = fx;
        *(volatile int *)(buf + 8) = 0;
        F<int>(self, 0x98) = id;
        slot = 0xa4;
    } else {
        if (F<int>(self, 0xa0) >= 1000) goto tail;
        F<int>(self, 0xbc) = 0;
        float len = AEMath_VectorLength(TFC_getCamOffset(F<TargetFollowCamera *>(self, 0xf4)));
        *(volatile float *)(buf + 4) = fy;
        *(volatile float *)(buf + 0) = fx;
        F<float>(self, 0xbc) = len;
        slot = 0xb0;
        F<int>(self, 0x9c) = id;
    }
    Vector_assign((Vector *)((char *)self + slot), buf);
tail:
    F<int>(self, 0x124) = x;
    F<int>(self, 0x128) = y;
    F<int>(self, 0x154) = x;
    F<int>(self, 0x158) = y;
    F<int>(self, 0x134) = 0;
    F<int>(self, 0x138) = 0;
    F<uint8_t>(self, 0x15c) = 1;
}
