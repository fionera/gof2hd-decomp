#include "class.h"

extern "C" void *Level_getEnemies(void *level);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas);
extern "C" void PaintCanvas_GetScreenPosition(void *canvas, void *vec);
extern "C" void MatrixGetRight(void *out, void *matrix);
extern "C" void MatrixGetPosition(void *out, void *matrix);
extern "C" void MatrixGetUp(void *out, void *matrix);
extern "C" void MatrixGetLookAt(void *out, void *pos, void *target, void *up);
extern "C" void MatrixGetDir(void *out, void *matrix);
extern "C" void Vector_mul(void *out, void *vec, float scale);
extern "C" void Vector_assign(void *dst, void *src);
extern "C" void Vector_sub(void *out, void *a, void *b);
extern "C" void Vector_add(void *out, void *a, void *b);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void Matrix_mul_assign(void *dst, void *rhs);
extern "C" void AEGeometry_setMatrix(void *matrix);
extern "C" void *Status_getSystem();
extern "C" int SolarSystem_getRace(void *system);
extern "C" void MatrixSetRotation(void *matrix, float x, float y, float z);

extern "C" void *SpaceLounge_screen_level_slot;
extern "C" void *SpaceLounge_screen_canvas_slot;
extern "C" void *SpaceLounge_screen_projector;

extern "C" void SpaceLounge_updateScreenPositions(SpaceLounge *self)
{
    char camera[60];
    char look[60];
    char pos[12];
    char up[12];
    char target[12];
    char halfRight[12];
    char screen[12];

    if (P(self, 0x24) == 0) {
        return;
    }

    void *level = *(void **)&SpaceLounge_screen_level_slot;
    void *enemies = Level_getEnemies(level);
    void *canvasSlot = *(void **)&SpaceLounge_screen_canvas_slot;
    void *canvas = *(void **)canvasSlot;
    void *project = *(void **)&SpaceLounge_screen_projector;

    void *current = PaintCanvas_CameraGetCurrent(canvas);
    void *local = PaintCanvas_CameraGetLocal(canvas);
    MatrixGetRight(pos, local);
    Vector_mul(halfRight, pos, 0.5f);

    unsigned count = U(P(self, 0x24), 0x0);
    for (unsigned i = 0; i < count; ++i) {
        void *enemy = ((void **)P(enemies, 0x4))[i];
        void (**vt)(void) = *(void (***)(void))enemy;
        ((void (*)(void *, void *))vt[0x28 / 4])(enemy, target);

        ((void (*)(void *, void *))project)(screen, target);
        Vector_sub(pos, target, halfRight);
        ((void (*)(void *, void *))project)(screen, pos);
        PaintCanvas_GetScreenPosition(canvas, screen);

        Vector_add(pos, target, halfRight);
        ((void (*)(void *, void *))project)(screen, pos);
        PaintCanvas_GetScreenPosition(canvas, screen);

        current = PaintCanvas_CameraGetCurrent(canvas);
        local = PaintCanvas_CameraGetLocal(canvas);
        Matrix_assign(camera, local);
        MatrixGetPosition(pos, camera);
        MatrixGetUp(up, camera);
        MatrixGetLookAt(look, pos, target, up);
        Matrix_assign(camera, look);

        void *mapped = ((void **)P(enemies, 0x4))[U(P(self, 0x24), 0x0) + i];
        AEGeometry_setMatrix(P(mapped, 0x8));

        ((void (*)(void *, void *))project)(B(self, 0x4c), target);
        MatrixGetDir(pos, look);
        F(self, 0x4c) -= (*(float *)pos) * 20.0f;
        MatrixGetDir(pos, look);
        F(self, 0x54) -= (*(float *)(pos + 8)) * 20.0f;

        ((void (*)(void *, void *))(*(void ***)mapped)[0x44 / 4])(mapped, B(self, 0x4c));

        if (SolarSystem_getRace(Status_getSystem()) == 0) {
            MatrixSetRotation(look, 0.0f, 0.0f, 0.0f);
            Matrix_mul_assign(camera, look);
        }

        AEGeometry_setMatrix(P(enemy, 0x8));
        ((void (*)(void *, void *))vt[0x44 / 4])(enemy, target);
    }
}
