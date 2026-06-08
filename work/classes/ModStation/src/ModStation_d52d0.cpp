#include "class.h"

struct Station;
struct SolarSystem;
struct EaseInOut;
struct EaseInOutMatrix;
struct Matrix;

__attribute__((visibility("hidden"))) extern int  *g_msc_stack;     // [DAT_000e5698]
__attribute__((visibility("hidden"))) extern int   g_msc_vtable;    // [DAT_000e569c] vtable base
__attribute__((visibility("hidden"))) extern int **g_msc_canvas;    // [DAT_000e56bc] PaintCanvas holder

extern "C" {
void  String_ctor_default_msc(String *s);
int   Status_getStation_msc();
int   Station_getIndex_msc(Station *s);
int   Status_getSystem_msc();
int   SolarSystem_getRace_msc();
void *ModStation_opnew_msc(unsigned size);
void  EaseInOut_ctor_msc(EaseInOut *e);
int   PaintCanvas_CameraGetCurrent_msc();
void  PaintCanvas_CameraSetLocal_msc(unsigned cam, Matrix *m);
// Builds the two hangar camera key matrices for the given station race (MatrixSetTranslation /
// MatrixSetRotation cascade the decompiler corrupted) and constructs the 3000ms EaseInOutMatrix
// camera tween; returns the new EaseInOutMatrix and writes the resting cam coords into self+0x278.
EaseInOutMatrix *ModStation_msc_buildCameraTween(ModStation *self, int race);
}

// ModStation::ModStation() — constructor: zero-inits the station-module state, picks the home
// station's race, and sets up the hangar idle camera tween.
extern "C" void ModStation_ModStation(ModStation *self)
{
    I(self, 0x00) = g_msc_vtable + 8;          // vtable
    String_ctor_default_msc((String *)B(self, 0x38));

    // zero / default field block.
    I(self, 0x28) = 0;
    C(self, 0x23) = 0;     // this[1].field_20 + 3
    US(self, 0x20) = 0;    // partial of field_20 region — kept as a 16-bit clear
    C(self, 0x24) = 0;
    C(self, 0x69) = 0;     // field_68 + 1
    I(self, 0x0c) = 100;
    I(self, 0x10) = 0;
    I(self, 0x14) = 0;
    I(self, 0x1c) = 0;
    I(self, 0x20) = 0;
    I(self, 0x50) = 0;
    I(self, 0x54) = 0;
    I(self, 0x58) = 0;
    C(self, 0x6e) = 0;     // field_6C + 2
    C(self, 0x18) = 0;
    I(self, 0x80) = 0;     // m_pDialogueWindow
    I(self, 0x70) = 0;

    // pick the station race (0x65 -> raidstation race 8, 100 -> race 7, else system race).
    Station *st = (Station *)Status_getStation_msc();
    int race;
    if (Station_getIndex_msc(st) == 0x65) {
        race = 8;
    } else {
        st = (Station *)Status_getStation_msc();
        if (Station_getIndex_msc(st) == 100) {
            race = 7;
        } else {
            Status_getSystem_msc();
            race = SolarSystem_getRace_msc();
        }
    }

    EaseInOutMatrix *cam = ModStation_msc_buildCameraTween(self, race);
    I(self, 0x20) = (int)cam;

    unsigned camHandle = **(unsigned **)g_msc_canvas;
    Matrix *cur = (Matrix *)PaintCanvas_CameraGetCurrent_msc();
    PaintCanvas_CameraSetLocal_msc(camHandle, cur);

    // three scalar EaseInOut tweens (fov / fade etc.) stored at self+0x288..0x290.
    EaseInOut *e;
    e = (EaseInOut *)ModStation_opnew_msc(0x10); EaseInOut_ctor_msc(e); I(self, 0x288) = (int)e;
    e = (EaseInOut *)ModStation_opnew_msc(0x10); EaseInOut_ctor_msc(e); I(self, 0x28c) = (int)e;
    e = (EaseInOut *)ModStation_opnew_msc(0x10); EaseInOut_ctor_msc(e); I(self, 0x290) = (int)e;
}
