#include "class.h"

__attribute__((visibility("hidden"))) extern void *BeamGun_vtable;
__attribute__((visibility("hidden"))) extern void **BeamGun_canvas;
__attribute__((visibility("hidden"))) extern int32_t BeamGun_secondaryMeshes[];

extern "C" void *operator_new(uint32_t size);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, PaintCanvas *canvas, bool flag);
extern "C" int Gun_isPlayerGun(Gun *self);

struct BeamGun {
    BeamGun(int param_1, Gun *gun, int param_3, Level *level);
};

BeamGun::BeamGun(int param_1, Gun *gun, int param_3, Level *level)
{
    int type;
    AEGeometry *geometry;
    int mesh;

    type = param_3;
    param_3 = type - 9;
    if (type == 0xe4)
        param_3 = 2;

    F<void *>(this, 0x0) = (char *)BeamGun_vtable + 8;
    F<int32_t>(this, 0x4) = 0;
    F<Gun *>(this, 0x8) = gun;
    F<Level *>(this, 0xc) = level;
    F<int32_t>(this, 0x10) = param_1;
    F<int32_t>(this, 0x14) = param_3;

    geometry = (AEGeometry *)operator_new(0xc0);
    void **canvasHolder = BeamGun_canvas;
    uint32_t primaryMesh = (uint32_t)(param_3 + 0x3795);
    if (type == 0xe4)
        primaryMesh = 0x4a92;
    AEGeometry_ctor(geometry, (uint16_t)primaryMesh, (PaintCanvas *)*canvasHolder, false);
    F<AEGeometry *>(this, 0x18) = geometry;

    mesh = Gun_isPlayerGun(gun);
    if (mesh == 0 ||
        (mesh = BeamGun_secondaryMeshes[F<int32_t>(gun, 0x58)], mesh < 0)) {
        geometry = 0;
        F<uint8_t>(this, 0x20) = 0;
    } else {
        int gunKind = F<int32_t>(gun, 0x5c);
        F<uint8_t>(this, 0x20) = gunKind != 0xb;
        if (gunKind == 0xb) {
            geometry = 0;
        } else {
            geometry = (AEGeometry *)operator_new(0xc0);
            AEGeometry_ctor(geometry, (uint16_t)mesh, (PaintCanvas *)*canvasHolder, false);
        }
    }

    F<uint8_t>(this, 0x21) = 0;
    F<AEGeometry *>(this, 0x1c) = geometry;
}
