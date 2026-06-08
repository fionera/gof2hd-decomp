#include "class.h"

extern int Explosion_paintCanvas;

extern "C" long long PaintCanvas_TransformGetTransform(int canvas, int transform);
extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" void Transform_Update(long long transform, long long elapsed, int zero);
extern "C" void Transform_Update32(uint32_t transform, uint32_t high, long long elapsed, uint32_t zero);
extern "C" void AEGeometry_getPosition(Vector *out, AEGeometry *self);
extern "C" int PaintCanvas_CameraGetCurrent(int canvas);
extern "C" void *PaintCanvas_CameraGetLocal(int canvas, int current);
extern "C" void MatrixGetPosition(Vector *out, void *matrix);
extern "C" void Vector_sub(Vector *out, const Vector *a, const Vector *b);
extern "C" float VectorLength(const Vector *self);
extern "C" void TargetFollowCamera_setRumblePercentage(TargetFollowCamera *self, float value, int duration);
extern "C" void Explosion_reset(Explosion *self);

extern "C" void Explosion_update_camera(Explosion *self, int dt, TargetFollowCamera *camera)
{
    if (UC(self, 0x20) == 0) {
        return;
    }

    int *canvas = &Explosion_paintCanvas;
    long long delta = (long long)dt;
    Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(G(self, 0x4), 0xc)), delta, 0);

    int lodTransform = I(G(self, 0x4), 0x14);
    if (lodTransform != -1) {
        Transform_Update(PaintCanvas_TransformGetTransform(*canvas, lodTransform), delta, 0);
    }

    AEGeometry *secondary = G(self, 0x8);
    if (secondary != 0) {
        Transform_Update32(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0, delta, 0);
    }

    Array *streaks = A(self, 0xc);
    if (streaks != 0) {
        for (uint32_t i = 0; i < U(streaks, 0x0); i++) {
            AEGeometry *geometry = *(AEGeometry **)((char *)streaks->f_4 + i * 4);
            Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(geometry, 0xc)), delta, 0);
            streaks = A(self, 0xc);
        }
    }

    if (camera != 0 && U(self, 0x0) < 2) {
        char vectors[36];
        Vector *cameraPosition = (Vector *)(vectors + 0);
        Vector *position = (Vector *)(vectors + 12);
        Vector *diff = (Vector *)(vectors + 24);
        AEGeometry_getPosition(position, G(self, 0x4));

        int canvasValue = *canvas;
        int current = PaintCanvas_CameraGetCurrent(canvasValue);
        MatrixGetPosition(cameraPosition, PaintCanvas_CameraGetLocal(canvasValue, current));
        Vector_sub(diff, position, cameraPosition);
        float distance = VectorLength(diff);

        uint32_t transform = PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc));
        int anim = I((void *)transform, 0x110);
        if (anim <= 0x7d0) {
            float capped = 30000.0f;
            if (distance < 30000.0f) {
                capped = distance;
            }
            float value = (1.0f - capped / 30000.0f) * ((float)anim / -2000.0f + 1.0f);
            TargetFollowCamera_setRumblePercentage(camera, value, 0x32);
        }
    }

    long long elapsed = *(long long *)B(self, 0x18) + delta;
    *(long long *)B(self, 0x18) = elapsed;
    if (*(long long *)B(self, 0x10) < elapsed) {
        Explosion_reset(self);
        if (camera != 0) {
            TargetFollowCamera_setRumblePercentage(camera, 0.0f, 0);
        }
    }
}
