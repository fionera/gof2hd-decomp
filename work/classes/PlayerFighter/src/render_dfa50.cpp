#include "class.h"

extern "C" void AEGeometry_render(int geom);
extern "C" int Player_isActive(int player);
extern "C" void Explosion_render(int e);
extern "C" int PaintCanvas_TransformGetLocal(unsigned t);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned n);
extern "C" void PaintCanvas_TransformSetLocal(unsigned t, void *m);
extern "C" void Trail_render(int t);
extern "C" void PlayerFighter_render_tail(int geom);
extern void *const gR_g3 __attribute__((visibility("hidden")));   // case 3 transform-id global
extern void *const gR_g4 __attribute__((visibility("hidden")));   // case 4 transform-id global
extern void *const gR_g5 __attribute__((visibility("hidden")));   // default-case transform-id global

extern "C" void PlayerFighter_render(PlayerFighter *self)
{
    if (F<int32_t>(self, 0x78) != 0) {
        AEGeometry_render(0);
    }
    int active = Player_isActive(F<int32_t>(self, 4));
    int st = F<int32_t>(self, 0x88);
    unsigned *idp;
    if (active != 0 && (unsigned)(st - 3) < 2) {
        if (F<int32_t>(self, 0x124) != 0) {
            Explosion_render(0);
            st = F<int32_t>(self, 0x88);
        }
        if (st == 4) {
            if (F<int32_t>(self, 0x1f8) >= 300) {
                goto done;
            }
            idp = *(unsigned **)gR_g4;
        } else {
            if (st != 3) {
                goto done;
            }
            if (F<int32_t>(self, 0xc) == 0) {
                return PlayerFighter_render_tail(F<int32_t>(self, 8));
            }
            idp = *(unsigned **)gR_g3;
        }
    } else {
        if (active != 0 || st != 5) {
            goto done;
        }
        if (F<int32_t>(self, 0xc) == 0) {
            AEGeometry_render(0);
        } else {
            idp = *(unsigned **)gR_g5;
            unsigned char tmp[60];
            unsigned id = *idp;
            void *src = (void *)PaintCanvas_TransformGetLocal(id);
            __aeabi_memcpy(tmp, src, 0x3c);
            PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
            AEGeometry_render(0);
            PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
        }
        if (F<int32_t>(self, 0x154) != 0) {
            Trail_render(0);
        }
        goto done;
    }
    {
        unsigned char tmp[60];
        unsigned id = *idp;
        void *src = (void *)PaintCanvas_TransformGetLocal(id);
        __aeabi_memcpy(tmp, src, 0x3c);
        PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
        AEGeometry_render(0);
        PaintCanvas_TransformSetLocal(*idp, *(void **)(F<int32_t>(self, 0xc) + 0xc));
    }
done:
    ;
}
