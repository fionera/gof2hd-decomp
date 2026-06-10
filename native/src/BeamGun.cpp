#include "BeamGun.h"


extern "C" AEGeometry *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *ptr);
extern "C" void BeamGun_setEnemies_tail(void *data);
extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void BeamGun_render_tail(AEGeometry *self);
extern "C" BeamGun *_ZN7BeamGunD1Ev(BeamGun *self);
extern "C" void BeamGun_setEnemy_tail(void *data);
extern "C" void *operator_new(uint32_t size);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, PaintCanvas *canvas, bool flag);
extern "C" int Gun_isPlayerGun(Gun *self);
extern "C" void Gun_update(Gun *self, int elapsed);
extern "C" Transform *PaintCanvas_TransformGetTransform(PaintCanvas *canvas, int transformId);
extern "C" void Transform_SetAnimationState(Transform *self, int state, void *arg);
extern "C" void Transform_Update(Transform *self, long long elapsed, bool flag);
extern "C" void AEGeometry_setScaling(AEGeometry *self, float x, float y, float z);
extern "C" void AEGeometry_setDirection(AEGeometry *self, const Vector *direction, const void *up);
extern "C" PlayerEgo *Level_getPlayer(Level *self);
extern "C" void PlayerEgo_getPosition(void *out, PlayerEgo *self);
extern "C" int Vector_ne(const Vector *lhs, const Vector *rhs);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void Matrix_mul(Matrix *out, const Matrix *lhs, const Matrix *rhs);
extern "C" void Vector_add(Vector *out, const Vector *lhs, const Vector *rhs);
extern "C" void MatrixRotateVector(Vector *out, const Matrix *matrix, const Vector *vector);
extern "C" void Vector_add_assign(void *self, const Vector *rhs);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const void *position);
extern "C" void AEGeometry_setVisible(AEGeometry *self, bool visible);
extern "C" void MatrixGetDir(Vector *out, const Matrix *matrix);

// ---- _BeamGun_177834.cpp ----
__attribute__((visibility("hidden"))) extern void *BeamGun_vtable;


extern "C" BeamGun *_ZN7BeamGunD1Ev(BeamGun *self)
{
    F<void *>(self, 0x0) = (char *)BeamGun_vtable + 8;

    AEGeometry *primary = F<AEGeometry *>(self, 0x18);
    if (primary != 0)
        operator_delete(AEGeometry_dtor(primary));
    F<AEGeometry *>(self, 0x18) = 0;

    AEGeometry *secondary = F<AEGeometry *>(self, 0x1c);
    if (secondary != 0)
        operator_delete(AEGeometry_dtor(secondary));
    F<AEGeometry *>(self, 0x1c) = 0;

    return self;
}

// ---- setEnemies_177b20.cpp ----
extern "C" void BeamGun_setEnemies(Array *enemies)
{
    return BeamGun_setEnemies_tail(F<void *>(enemies, 0x8));
}

// ---- render_17787c.cpp ----
struct BeamGun {
    void render();
};

void BeamGun::render()
{
    Gun *gun = F<Gun *>(this, 0x8);
    if (F<uint8_t>(gun, 0x4c) == 0)
        return;

    AEGeometry_render(F<AEGeometry *>(this, 0x18));

    if (F<uint8_t>(this, 0x21) != 0) {
        AEGeometry *secondary = F<AEGeometry *>(this, 0x1c);
        if (secondary != 0)
            return BeamGun_render_tail(secondary);
    }
}

// ---- _BeamGun_17786c.cpp ----
extern "C" void _ZN7BeamGunD0Ev(BeamGun *self)
{
    return operator_delete(_ZN7BeamGunD1Ev(self));
}

// ---- setEnemy_177b26.cpp ----
extern "C" void BeamGun_setEnemy(Player *enemy)
{
    return BeamGun_setEnemy_tail(F<void *>(enemy, 0x8));
}

// ---- BeamGun_177754.cpp ----
__attribute__((visibility("hidden"))) extern void *BeamGun_vtable;
__attribute__((visibility("hidden"))) extern void **BeamGun_canvas;
__attribute__((visibility("hidden"))) extern int32_t BeamGun_secondaryMeshes[];


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

// ---- update_1778a4.cpp ----
__attribute__((visibility("hidden"))) extern void **BeamGun_canvas_update_a;
__attribute__((visibility("hidden"))) extern void **BeamGun_canvas_update_b;

typedef Transform *(*BeamGunGetTransformFn)(PaintCanvas *canvas, int transformId);
typedef void (*BeamGunSetAnimationFn)(Transform *transform, int state, void *arg);
__attribute__((visibility("hidden"))) extern BeamGunGetTransformFn BeamGun_getTransform_indirect;
__attribute__((visibility("hidden"))) extern BeamGunSetAnimationFn BeamGun_setAnimation_indirect;


struct BeamGun {
    void update(int elapsed);
};

void BeamGun::update(int elapsed)
{
    Vector back;
    Vector rotated;
    Matrix playerMatrix;
    Vector transformed;
    char position[12];

    Gun *gun = F<Gun *>(this, 0x8);
    Gun_update(gun, elapsed);

    gun = F<Gun *>(this, 0x8);
    if (F<uint8_t>(gun, 0x4c) == 0) {
        AEGeometry_setVisible(F<AEGeometry *>(this, 0x18), false);
        return;
    }

    if (F<uint8_t>(gun, 0x4d) != 0) {
        void **canvasHolder = BeamGun_canvas_update_a;
        AEGeometry *geometry = F<AEGeometry *>(this, 0x18);
        Transform *transform =
            PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder,
                                              F<int32_t>(geometry, 0x0c));
        Transform_SetAnimationState(transform, 3, 0);
        transform = PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                      F<int32_t>(geometry, 0x0c));
        Transform_SetAnimationState(transform, 1, 0);
        F<uint8_t>(F<Gun *>(this, 0x8), 0x4d) = 0;
    }

    void **canvasHolder = BeamGun_canvas_update_b;
    AEGeometry *primary = F<AEGeometry *>(this, 0x18);
    Transform *transform =
        PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder, F<int32_t>(primary, 0x0c));
    Transform_Update(transform, (long long)elapsed, false);

    gun = F<Gun *>(this, 0x8);
    AEGeometry_setScaling(F<AEGeometry *>(this, 0x18), 1.0f, 1.0f,
                          (float)F<int32_t>(gun, 0x8c));

    Vector *up = (Vector *)&playerMatrix;
    up->x = 0.0f;
    up->y = 1.0f;
    up->z = 0.0f;
    AEGeometry_setDirection(F<AEGeometry *>(this, 0x18), (Vector *)((char *)gun + 0x90), up);

    PlayerEgo *player = Level_getPlayer(F<Level *>(this, 0x0c));
    PlayerEgo_getPosition(&position, player);

    up->x = 0.0f;
    up->y = 0.0f;
    up->z = 0.0f;
    if (Vector_ne((Vector *)((char *)F<Gun *>(this, 0x8) + 0x7c), up) != 0) {
        player = Level_getPlayer(F<Level *>(this, 0x0c));
        Matrix *firstMatrix = AEGeometry_getMatrix(F<AEGeometry *>(player, 0x8));
        player = Level_getPlayer(F<Level *>(this, 0x0c));
        Matrix *secondMatrix = AEGeometry_getMatrix(F<AEGeometry *>(player, 0x4));
        Matrix_mul(&playerMatrix, firstMatrix, secondMatrix);

        gun = F<Gun *>(this, 0x8);
        back.x = 0.0f;
        back.y = 0.0f;
        back.z = -100.0f;
        Vector_add(&rotated, (Vector *)((char *)gun + 0x7c), &back);
        MatrixRotateVector(&transformed, &playerMatrix, &rotated);
        Vector_add_assign(position, &transformed);
    }

    AEGeometry_setPosition(F<AEGeometry *>(this, 0x18), position);
    AEGeometry_setVisible(F<AEGeometry *>(this, 0x18), true);

    if (F<uint8_t>(this, 0x20) != 0) {
        gun = F<Gun *>(this, 0x8);
        if (F<uint8_t>(gun, 0xa9) == 0) {
            AEGeometry *secondary = F<AEGeometry *>(this, 0x1c);
            BeamGunGetTransformFn getTransform = BeamGun_getTransform_indirect;
            Transform *secondaryTransform =
                getTransform((PaintCanvas *)*canvasHolder, F<int32_t>(secondary, 0x0c));
            BeamGunSetAnimationFn setAnimation = BeamGun_setAnimation_indirect;
            setAnimation(secondaryTransform, 0, 0);
            secondaryTransform =
                getTransform((PaintCanvas *)*canvasHolder, F<int32_t>(secondary, 0x0c));
            setAnimation(secondaryTransform, 3, 0);
            secondaryTransform =
                getTransform((PaintCanvas *)*canvasHolder, F<int32_t>(secondary, 0x0c));
            setAnimation(secondaryTransform, 1, 0);
        } else {
            player = Level_getPlayer(F<Level *>(this, 0x0c));
            PlayerEgo_getPosition(&playerMatrix, player);

            gun = F<Gun *>(this, 0x8);
            transformed = *(Vector *)((char *)gun + 0x7c);
            transformed.z = F<float>(gun, 0x84) + -100.0f;

            MatrixRotateVector(&rotated, (Matrix *)((char *)F<void *>(player, 0x0) + 4),
                               &transformed);
            Vector_add_assign(&playerMatrix, &rotated);
            AEGeometry_setPosition(F<AEGeometry *>(this, 0x1c), &playerMatrix);

            AEGeometry *secondary = F<AEGeometry *>(this, 0x1c);
            transform = PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                          F<int32_t>(secondary, 0x0c));
            Transform_Update(transform, (long long)elapsed, false);

            MatrixGetDir(&rotated, (Matrix *)((char *)F<void *>(player, 0x0) + 4));
            back.x = 0.0f;
            back.y = 1.0f;
            back.z = 0.0f;
            AEGeometry_setDirection(F<AEGeometry *>(this, 0x1c), &rotated, &back);
        }
    }

    F<uint8_t>(this, 0x21) = F<uint8_t>(F<Gun *>(this, 0x8), 0xa9);
}
