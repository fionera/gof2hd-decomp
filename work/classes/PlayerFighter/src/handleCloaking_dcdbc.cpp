#include "class.h"

extern "C" int KIPlayer_isDead(PlayerFighter *self);
extern "C" void PaintCanvas_MeshCloneMaterial(void *canvas, unsigned mesh, unsigned *out);
extern "C" int PaintCanvas_MeshGetPointer(void *canvas, unsigned mesh);
extern "C" int PaintCanvas_MaterialGetMaterial(void *canvas, unsigned mat);
extern "C" void PaintCanvas_MeshChangeMaterial(void *canvas, unsigned mesh, unsigned short mat);
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" int AERandom_nextInt(int rng);
extern "C" void PlayerFighter_setExhaustVisible(PlayerFighter *self, bool vis);
// Tail-call veneers (cloak start / stop / material-fade-apply).
extern "C" void PF_cloakStart(PlayerFighter *self);                       // -> 0x1ac1e8
extern "C" void PF_cloakStop(PlayerFighter *self, int on);               // -> 0x1ac1f8
extern "C" void PF_cloakApply(void *meshPtr, int arg, float alpha, int flag); // -> 0x1ac208

extern void *const gHC_rng1 __attribute__((visibility("hidden")));        // DAT_000ecfb0
extern void *const gHC_canvasClone __attribute__((visibility("hidden"))); // DAT_000ecfb8
extern void *const gHC_canvasMat __attribute__((visibility("hidden")));   // DAT_000ecfbc
extern void *const gHC_rng2 __attribute__((visibility("hidden")));        // DAT_000ecfb4
extern void *const gHC_canvasA __attribute__((visibility("hidden")));     // DAT_000ecfc0
extern void *const gHC_canvasB __attribute__((visibility("hidden")));     // DAT_000ecfc4
extern void *const gHC_canvasC __attribute__((visibility("hidden")));     // DAT_000ecfc8
extern const float gHC_divIn __attribute__((visibility("hidden")));       // DAT_000ecfac
extern const float gHC_divOut __attribute__((visibility("hidden")));      // DAT_000ecfa8

// PlayerFighter::handleCloaking() — self in r0.
extern "C" void PlayerFighter_handleCloaking(PlayerFighter *self)
{
    if (F<int32_t>(self, 0x28) != 10) return;
    if (F<int32_t>(self, 8) == 0) return;
    if (KIPlayer_isDead(self) != 0) return;
    if (*(char *)(F<int32_t>(self, 4) + 0x68) != 0) return;
    if (F<signed char>(self, 0x2d8) == 0) return;

    if (F<signed char>(self, 0x2d0) != 0) {
        int delta = F<int32_t>(self, 0x2c8);
        if (delta == 0) {
            unsigned matId = F<uint32_t>(self, 0x2dc);
            F<signed char>(self, 0x13c) = 1;
            if (matId == 0xffffffff) {
                void **cv = *(void ***)gHC_canvasClone;
                PaintCanvas_MeshCloneMaterial(*cv, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c),
                                              (unsigned *)((char *)self + 0x2dc));
                int mp = PaintCanvas_MeshGetPointer(*cv,
                                                    *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c));
                matId = F<uint32_t>(self, 0x2dc);
                F<int32_t>(self, 0x2e0) = *(int *)(*(int *)(mp + 0x30) + 0x20);
            }
            void **cvm = *(void ***)gHC_canvasMat;
            int mat = PaintCanvas_MaterialGetMaterial(*cvm, matId);
            *(int *)(mat + 0x20) = 0xe;
            PaintCanvas_MeshChangeMaterial(*cvm, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c),
                                           *(unsigned short *)((char *)self + 0x2dc));
            if (F<signed char>(self, 0x2d9) == 0) {
                delta = F<int32_t>(self, 0x2c8);
            } else {
                F<signed char>(self, 0x2d9) = 0;
                delta = 2000;
                F<int32_t>(self, 0x2c8) = 2000;
            }
        }
        long long acc = (long long)delta +
                        (((long long)F<int32_t>(self, 0x1d4) << 32) | (unsigned)F<int32_t>(self, 0x1d0));
        int total = (int)acc;
        F<int32_t>(self, 0x2c8) = total;

        if (total - F<int32_t>(self, 0x1d0) <= 2000) {
            if (1999 < total) {
                PlayerFighter_setExhaustVisible(self, false);
                F<signed char>(self, 0x74) = 1;
            }
            void *cv = *(void **)gHC_canvasA;
            cv = *(void **)cv;
            int mp = PaintCanvas_MeshGetPointer(cv, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c));
            float a = VectorSignedToFloat(F<int32_t>(self, 0x2c8), 0) / gHC_divIn;
            PF_cloakApply((void *)(long)mp, (int)(long)cv, a, 1);
            return;
        } else {
            if (F<int32_t>(self, 0x2cc) < total) {
                int restore = F<int32_t>(self, 0x2e0);
                void **cvc = *(void ***)gHC_canvasC;
                F<int32_t>(self, 0x2c8) = 0;
                F<signed char>(self, 0x2d0) = 0;
                F<signed char>(self, 0x13c) = 0;
                int mat = PaintCanvas_MaterialGetMaterial(*cvc, F<uint32_t>(self, 0x2dc));
                *(int *)(mat + 0x20) = restore;
                PF_cloakStop(self, 1);
                return;
            }
            if (total <= F<int32_t>(self, 0x2cc) - 2000) {
                return;
            }
            void **cvb = *(void ***)gHC_canvasB;
            F<signed char>(self, 0x74) = 0;
            void *cv = *cvb;
            int mp = PaintCanvas_MeshGetPointer(cv, *(unsigned *)(F<int32_t>(self, 0xc) + 0x1c));
            float a = VectorSignedToFloat(F<int32_t>(self, 0x2c8), 0);
            float b = VectorSignedToFloat(F<int32_t>(self, 0x2cc) - 2000, 0);
            float alpha = (a - b) / gHC_divOut + 1.0f;
            PF_cloakApply((void *)(long)mp, (int)(long)cv, alpha, 1);
            return;
        }
    }

    if (F<signed char>(self, 0x1e0) != 0 && AERandom_nextInt(**(int **)gHC_rng1) < 0x32) {
        PF_cloakStart(self);
        return;
    }
    int acc = F<int32_t>(self, 0x1d0) + F<int32_t>(self, 0x2d4);
    F<int32_t>(self, 0x2d4) = acc;
    if (8000 < acc) {
        F<int32_t>(self, 0x2d4) = 0;
        if (AERandom_nextInt(**(int **)gHC_rng2) < 0x1e) {
            PF_cloakStart(self);
        }
    }
}
