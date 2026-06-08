#include "class.h"

extern "C" {
void *AEGeometry_getMatrix2(void *geom);                          // 0x721cc (this->0x8 matrix)
void AEMath_MatrixRotateVector(void *out, void *m, void *v);      // 0x6f694
void AEMath_VectorAdd(void *out, void *a, void *b);              // 0x6f1cc
void AEMath_VectorSub(void *out, void *a, void *b);              // 0x6ec38
float AEMath_VectorLength(void *v);                              // 0x6ec44
int SpacePoint_isFree(void *sp);                                 // 0x732c4
// virtual getPosition(out) at vtable slot +0x28.
}

// KIPlayer::getNearestNavigationPoint(AbyssEngine::AEMath::Vector const& dir, SpacePoint* target)
//   Among the player's space points of type 1 (navigation), returns the nearest free one
//   (or the explicitly requested `target`) measured against the player's transformed position.
extern "C" void *KIPlayer_getNearestNavigationPoint(KIPlayer *self, Vector *dir, void *target)
{
    void *arr = F<void *>(self, 0xcc);
    if (arr == 0)
        return 0;

    // this->getPosition(&selfPos) via vtable +0x28.
    Vector selfPos;
    {
        void **vtbl = *(void ***)self;
        typedef void (*fn_t)(KIPlayer *, Vector *);
        ((fn_t)vtbl[0x28 / 4])(self, &selfPos);
    }

    void *best = 0;
    float bestLen = 1e30f;
    unsigned count = *(unsigned *)arr;
    for (unsigned i = 0; i < count; i = i + 1) {
        void *pt = ((void **)(*(char **)((char *)arr + 4)))[i];
        if (*(int *)((char *)pt + 0x18) != 1)
            continue;

        void *mat = AEGeometry_getMatrix2(F<void *>(self, 0x8));
        Vector rotated;
        AEMath_MatrixRotateVector(&rotated, mat, ((void **)(*(char **)((char *)arr + 4)))[i]);
        Vector world;
        AEMath_VectorAdd(&world, &selfPos, &rotated);
        Vector delta;
        AEMath_VectorSub(&delta, &world, dir);
        float len = AEMath_VectorLength(&delta);
        float alen = len < 0.0f ? -len : len;
        if (alen < bestLen) {
            void *cand = ((void **)(*(char **)((char *)F<void *>(self, 0xcc) + 4)))[i];
            if (SpacePoint_isFree(cand) != 0 || cand == target) {
                best = cand;
                bestLen = alen;
            } else {
                best = target;
                bestLen = alen;
            }
        }
        count = *(unsigned *)F<void *>(self, 0xcc);
    }
    return best;
}
