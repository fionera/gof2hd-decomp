#include "class.h"

extern int Explosion_paintCanvas;

extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void AEGeometry_getPosition(Vector *out, AEGeometry *self);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, Matrix *matrix);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void AEGeometry_setDirection(AEGeometry *self, const Vector *direction, const Vector *up);
extern "C" int PaintCanvas_CameraGetCurrent(int canvas);
extern "C" void *PaintCanvas_CameraGetLocal(int canvas, int current);
extern "C" void MatrixSetTranslation(Matrix *out, Matrix *base, float x, float y, float z);
extern "C" void MatrixSetScaling(Matrix *out, Matrix *base, float x, float y, float z);
extern "C" void MatrixGetPosition(Vector *out, const Matrix *matrix);
extern "C" void MatrixGetUp(Vector *out, const Matrix *matrix);
extern "C" void MatrixGetDir(Vector *out, const Matrix *matrix);
extern "C" void MatrixGetLookAt(Matrix *out, const Vector *position, const Vector *target, const Vector *up);
extern "C" void Matrix_assign(Matrix *self, const Matrix *other);
extern "C" void Matrix_mul_assign(Matrix *self, const Matrix *other);

extern "C" void Explosion_render(Explosion *self)
{
    Matrix cameraLocal;
    Matrix work;
    Vector position;
    Vector cameraPosition;
    Vector up;

    if (UC(self, 0x20) != 0) {
        uint32_t type = U(self, 0x0);
        if ((type > 0xd || ((1u << (type & 0xff)) & 0x2780u) == 0) && G(self, 0x8) != 0) {
            AEGeometry_render(G(self, 0x8));
        }

        int *canvas = &Explosion_paintCanvas;
        int canvasValue = *canvas;
        int current = PaintCanvas_CameraGetCurrent(canvasValue);
        __aeabi_memcpy(&cameraLocal, PaintCanvas_CameraGetLocal(canvasValue, current), 0x3c);

        AEGeometry_getPosition(&position, G(self, 0x4));

        if (type < 0xd && ((1u << (type & 0xff)) & 0x1804u) != 0) {
            MatrixSetTranslation(&work, &cameraLocal, position.x, position.y, position.z);
        } else {
            MatrixGetPosition(&cameraPosition, &cameraLocal);
            MatrixGetUp(&up, &cameraLocal);
            MatrixGetLookAt(&work, &cameraPosition, &position, &up);
            Matrix_assign(&cameraLocal, &work);
        }

        float scale = F(self, 0x24);
        MatrixSetScaling(&work, &cameraLocal, scale, scale, scale);

        if (type - 8 < 3) {
            Matrix_mul_assign(&cameraLocal, (Matrix *)B(self, 0x2c));
        }

        AEGeometry_setMatrix(G(self, 0x4), &cameraLocal);
        AEGeometry_setPosition(G(self, 0x4), &position);

        canvasValue = *canvas;
        current = PaintCanvas_CameraGetCurrent(canvasValue);
        Matrix_assign(&cameraLocal, (Matrix *)PaintCanvas_CameraGetLocal(canvasValue, current));
        Vector *direction = (Vector *)&work;
        MatrixGetDir(direction, &cameraLocal);
        MatrixGetUp(&cameraPosition, &cameraLocal);
        AEGeometry_setDirection(G(self, 0x4), direction, &cameraPosition);
        AEGeometry_render(G(self, 0x4));

        Array *streaks = A(self, 0xc);
        if (streaks != 0) {
            for (uint32_t i = 0; i < U(streaks, 0x0); i++) {
                AEGeometry *geometry = *(AEGeometry **)((char *)P(streaks, 0x4) + i * 4);
                AEGeometry_render(geometry);
                streaks = A(self, 0xc);
            }
        }
    }
}
