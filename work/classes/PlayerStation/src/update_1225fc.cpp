#include "class.h"

typedef long long (*TransformGetFn)(void *canvas, int mesh);
typedef void (*TransformUpdateFn)(uint32_t lo, uint32_t hi, int delta, int deltaHigh,
                                  int zero);

extern "C" void *Status_holder __attribute__((visibility("hidden")));
extern "C" int Status_inAlienOrbit(void *status);
extern "C" void *PaintCanvas_holder __attribute__((visibility("hidden")));
extern "C" long long PaintCanvas_TransformGetTransform(void *canvas, int mesh);
extern "C" void Transform_Update(long long transform, long long delta, int zero);
extern "C" TransformGetFn PlayerStation_transformGet __attribute__((visibility("hidden")));
extern "C" TransformUpdateFn PlayerStation_transformUpdate __attribute__((visibility("hidden")));

void PlayerStation::update(int delta)
{
    register int savedDelta = delta;
    bool active = F<int32_t>(P(this, 0x140), 0x14) != -1;
    int type = 0;
    if (active) {
        type = F<int32_t>(this, 0x148);
    }
    if (!active || type == 0x65) {
        return;
    }

    if (Status_inAlienOrbit(P(Status_holder, 0x0)) != 0) {
        return;
    }

    void **canvasHolder = (void **)PaintCanvas_holder;
    void *canvas = *canvasHolder;
    void *root = P(this, 0x140);
    long long delta64 = (long long)savedDelta;
    Transform_Update(PaintCanvas_TransformGetTransform(canvas, F<int32_t>(root, 0x14)),
                     delta64, 0);

    type = F<int32_t>(this, 0x148);
    if (type == 100) {
        Transform_Update(PaintCanvas_TransformGetTransform(canvas, F<int32_t>(this, 0x144)),
                         delta64, 0);
    } else if (type == 0x6c) {
        TransformGetFn getTransform = PlayerStation_transformGet;
        long long transform = getTransform(canvas, F<int32_t>(this, 0x164));
        uint32_t updateSlot = (uint32_t)&PlayerStation_transformUpdate;
        TransformUpdateFn updateTransform = PlayerStation_transformUpdate;
        int deltaHigh = savedDelta >> 31;
        updateTransform((uint32_t)transform, updateSlot, savedDelta, deltaHigh, 0);

        transform = getTransform(canvas, F<int32_t>(this, 0x168));
        updateTransform((uint32_t)transform, (uint32_t)((uint64_t)transform >> 32),
                        savedDelta, deltaHigh, 0);
        transform = getTransform(canvas, F<int32_t>(this, 0x16c));
        updateTransform((uint32_t)transform, (uint32_t)((uint64_t)transform >> 32),
                        savedDelta, deltaHigh, 0);
        transform = getTransform(canvas, F<int32_t>(this, 0x170));
        updateTransform((uint32_t)transform, (uint32_t)((uint64_t)transform >> 32),
                        savedDelta, deltaHigh, 0);
    }
}
