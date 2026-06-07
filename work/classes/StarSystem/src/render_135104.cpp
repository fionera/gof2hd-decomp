#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_StarSystem_render_canvas;
__attribute__((visibility("hidden"))) extern uint32_t *g_StarSystem_render_station_index;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_render_status;

extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas, void *current);
extern "C" void MatrixGetPosition(char *out, void *matrix);
extern "C" void __aeabi_memcpy(void *dst, const void *src, unsigned n);
extern "C" void Vector_add(char *out, const char *a, const char *b);
extern "C" void MatrixGetUp(char *out, const char *matrix);
extern "C" void MatrixGetLookAt(char *out, const char *from, const char *to, const char *up);
extern "C" void Matrix_assign(char *dst, const char *src);
extern "C" void AEGeometry_setRotation3(AEGeometry *geom, float x, float y, float z);
extern "C" void AEGeometry_getScaling(char *out, AEGeometry *geom);
extern "C" void MatrixSetScaling(char *matrix, float x, float y, float z);
extern "C" void AEGeometry_setMatrix(AEGeometry *geom, const char *matrix);
extern "C" void Vector_mul_assign(char *vec, float scale);
extern "C" int Status_inPlanetRingOrbit(void *status);
extern "C" void AEGeometry_setScaling3(AEGeometry *geom, float x, float y, float z);
extern "C" void AEGeometry_setPosition(AEGeometry *geom, const char *pos);
extern "C" void PaintCanvas_SetTexture(void *canvas, uint32_t texture, int mode);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int mode);
extern "C" void AEGeometry_render(AEGeometry *geom);
extern "C" void *AEGeometry_getMatrix(AEGeometry *geom);
extern "C" void StarSystem_renderSunStreak(StarSystem *self);

extern "C" void StarSystem_render(StarSystem *self)
{
    char cameraPos[12];
    char savedCamera[0x3c];
    char lookAt[0x3c];
    char tempMatrix[0x3c];
    char tempVec[12];
    char up[12];
    char scaleBytes[12];

    void **canvasHolder = g_StarSystem_render_canvas;
    void *canvas = *canvasHolder;
    void *current = PaintCanvas_CameraGetCurrent(canvas);
    void *local = PaintCanvas_CameraGetLocal(canvas, current);
    MatrixGetPosition(cameraPos, local);

    uint32_t count = array_len(P(self, 0x1c));
    int posOffset = 0;
    for (uint32_t i = 0; i < count; ++i, posOffset += 0x0c) {
        AEGeometry **geoms = (AEGeometry **)array_data(P(self, 0x1c));
        if (count > 1) {
            if (posOffset == 0) {
                void *canvas2 = *canvasHolder;
                void *current2 = PaintCanvas_CameraGetCurrent(canvas2);
                void *local2 = PaintCanvas_CameraGetLocal(canvas2, current2);
                __aeabi_memcpy(savedCamera, local2, 0x3c);
                Vector_add(tempVec, (const char *)array_data(P(self, 0x20)), cameraPos);
                if (B(self, 0x0c) == 0) {
                    MatrixGetUp(up, savedCamera);
                } else {
                    ((float *)up)[0] = 1.0f;
                    ((float *)up)[1] = 0.0f;
                    ((float *)up)[2] = 0.0f;
                }
                MatrixGetLookAt(lookAt, tempVec, cameraPos, up);
                Matrix_assign(savedCamera, lookAt);
                AEGeometry_setRotation3(geoms[0], 0.0f, 0.0f, 0.0f);
                AEGeometry_getScaling(tempVec, geoms[0]);

                float grow = (FL(P(self, 0x2c), 0) - 10.0f) * 0.001f;
                if (grow < 0.0f) {
                    grow = 0.0f;
                }
                float sx = ((float *)tempVec)[0];
                float sy = ((float *)tempVec)[1];
                float sz = ((float *)tempVec)[2];
                if (B(self, 0x0c) == 0) {
                    sx += grow;
                    sy += grow;
                }
                MatrixSetScaling(lookAt, sx, sy, sz);
                AEGeometry_setMatrix(geoms[0], lookAt);

                if (B(self, 0x0c) != 0) {
                    Vector_mul_assign(tempVec, grow);
                    void *canvas3 = *canvasHolder;
                    void *current3 = PaintCanvas_CameraGetCurrent(canvas3);
                    void *local3 = PaintCanvas_CameraGetLocal(canvas3, current3);
                    Matrix_assign(savedCamera, (const char *)local3);
                    Vector_add(up, (const char *)array_data(P(self, 0x20)), cameraPos);
                    MatrixGetUp(scaleBytes, savedCamera);
                    MatrixGetLookAt(lookAt, up, cameraPos, scaleBytes);
                    Matrix_assign(savedCamera, lookAt);
                    MatrixSetScaling(lookAt, grow + ((float *)tempVec)[0],
                                     ((float *)tempVec)[1],
                                     grow + ((float *)tempVec)[2]);
                    AEGeometry_setMatrix((AEGeometry *)P(self, 0x40), lookAt);
                    StarSystem_renderSunStreak(self);
                }

                MatrixSetScaling(lookAt,
                                 grow * (grow + ((float *)tempVec)[0] + 1.0f),
                                 ((float *)tempVec)[1],
                                 ((float *)tempVec)[1] / ((1.0f - grow) * 6.0f + 6.0f));
                AEGeometry_setMatrix((AEGeometry *)P(self, 0x40), lookAt);
            }

            uint32_t stationIndex = *g_StarSystem_render_station_index;
            bool selected = (i - 1) == stationIndex;
            if (selected && B(self, 0x28) == 0 &&
                Status_inPlanetRingOrbit(*g_StarSystem_render_status) == 0) {
                float ring = ((float *)cameraPos)[2] / 65536.0f;
                float clamped = ring < 0.0f ? ring : 0.0f;
                if (clamped < -1.0f) {
                    clamped = -1.0f;
                }
                FL(self, 0x5c) = clamped;
                AEGeometry_setScaling3(geoms[i], FL(self, 0x58) + clamped,
                                       FL(self, 0x58) + clamped,
                                       FL(self, 0x58) + clamped);
            }

            Vector_add(tempMatrix,
                       (const char *)array_data(P(self, 0x20)) + posOffset,
                       cameraPos);
            AEGeometry_setPosition(geoms[i], tempMatrix);
        }

        PaintCanvas_SetTexture(*canvasHolder, ((uint32_t *)array_data(P(self, 0x14)))[i], -1);
        int blend;
        if (posOffset == 0) {
            blend = 2;
        } else {
            blend = B(self, 0x54) == 0 ? 1 : 0x15;
        }
        PaintCanvas_SetBlendMode(*canvasHolder, blend);
        AEGeometry_render(((AEGeometry **)array_data(P(self, 0x1c)))[i]);

        if (i == U(self, 0x4c)) {
            PaintCanvas_SetTexture(*canvasHolder, U(self, 0x48), -1);
            PaintCanvas_SetBlendMode(*canvasHolder, 1);
            __aeabi_memcpy(savedCamera, AEGeometry_getMatrix(((AEGeometry **)array_data(P(self, 0x1c)))[i]), 0x3c);
            MatrixSetScaling(lookAt, 4.0f, 4.0f, 4.0f);
            AEGeometry_setMatrix((AEGeometry *)P(self, 0x44), lookAt);
            AEGeometry_render((AEGeometry *)P(self, 0x44));
        }
    }
}
