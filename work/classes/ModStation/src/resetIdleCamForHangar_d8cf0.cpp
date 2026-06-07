#include "class.h"

struct Station;

__attribute__((visibility("hidden"))) extern void **g_ModStation_ric_chk;    // *g -> canary src
__attribute__((visibility("hidden"))) extern void **g_ModStation_ric_canvas; // g -> P, *P -> canvas obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_ric_status; // *g -> Status obj
__attribute__((visibility("hidden"))) extern int *g_ModStation_ric_rotX;     // table base (pc-rel)
__attribute__((visibility("hidden"))) extern int *g_ModStation_ric_rotY;     // table base (pc-rel)

extern "C" void CutScene_resetCamera();
extern "C" void *ric_op_new(unsigned int sz);
extern "C" void EaseInOut_ctor(void *self, int a, int b);
extern "C" void EaseInOut_SetRange(void *self, int a, int b);
extern "C" void *PaintCanvas_CameraGetCurrent(void *c);
extern "C" void *PaintCanvas_CameraGetLocal(void *c);
extern "C" void AEMath_MatrixSetTranslation(void *m, int x, int y, int z);
extern "C" void AEMath_MatrixSetRotation(void *m, void *loc, int rx, int ry, int a4, int a5);
extern "C" Station *Status_getStation(void *s);
extern "C" int Station_getIndex(Station *st);
extern "C" void Status_getSystem(void *s);
extern "C" int SolarSystem_getRace();

// ModStation::resetIdleCamForHangar()
extern "C" void ModStation_resetIdleCamForHangar(ModStation *self)
{
    char matrix[60];

    if (P(self, 0x14) != 0)
        CutScene_resetCamera();

    if (P(self, 0x13c) != 0) {
        EaseInOut_SetRange(P(self, 0x13c), I(self, 0x130), I(self, 0x130));
    } else {
        void *p = ric_op_new(0x10);
        EaseInOut_ctor(p, I(self, 0x130), I(self, 0x130));
        P(self, 0x13c) = p;
    }
    if (P(self, 0x140) != 0) {
        EaseInOut_SetRange(P(self, 0x140), I(self, 0x134), I(self, 0x134));
    } else {
        void *p = ric_op_new(0x10);
        EaseInOut_ctor(p, I(self, 0x134), I(self, 0x134));
        P(self, 0x140) = p;
    }
    if (P(self, 0x144) != 0) {
        EaseInOut_SetRange(P(self, 0x144), I(self, 0x138), I(self, 0x138));
    } else {
        void *p = ric_op_new(0x10);
        EaseInOut_ctor(p, I(self, 0x138), I(self, 0x138));
        P(self, 0x144) = p;
    }

    void **canvasHolder = g_ModStation_ric_canvas;
    void *canvas = *canvasHolder;
    PaintCanvas_CameraGetCurrent(canvas);
    void *loc = PaintCanvas_CameraGetLocal(canvas);
    AEMath_MatrixSetTranslation(matrix, I(self, 0x130), I(self, 0x134), I(self, 0x138));

    int race;
    Station *st = Status_getStation(*g_ModStation_ric_status);
    if (Station_getIndex(st) == 0x65) {
        race = 8;
    } else {
        st = Status_getStation(*g_ModStation_ric_status);
        if (Station_getIndex(st) == 100) {
            race = 7;
        } else {
            Status_getSystem(*g_ModStation_ric_status);
            race = SolarSystem_getRace();
        }
    }

    canvas = *canvasHolder;
    PaintCanvas_CameraGetCurrent(canvas);
    void *loc2 = PaintCanvas_CameraGetLocal(canvas);
    AEMath_MatrixSetRotation(matrix, loc2, g_ModStation_ric_rotX[race], g_ModStation_ric_rotY[race], 0, 2);
    (void)loc;
}
