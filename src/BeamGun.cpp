#include "gof2/BeamGun.h"
#include "gof2/Gun.h"
#include "gof2/PlayerEgo.h"
#include "gof2/AEGeometry.h"

// AEGeometry's full header is not yet native-clean; Gun.h already completes the
// global forward declaration with a minimal view exposing field_0xc (transform id),
// which is the only AEGeometry field this TU reads by name.

extern "C" AEGeometry *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *ptr);
extern "C" void BeamGun_setEnemies_tail(void *data);
extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void BeamGun_render_tail(AEGeometry *self);
BeamGun *_ZN7BeamGunD1Ev(BeamGun *self);
extern "C" void BeamGun_setEnemy_tail(void *data);
extern "C" void *operator_new(uint32_t size);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, PaintCanvas *canvas, bool flag);
extern "C" Transform *PaintCanvas_TransformGetTransform(PaintCanvas *canvas, int transformId);
extern "C" void Transform_SetAnimationState(Transform *self, int state, void *arg);
extern "C" void Transform_Update(Transform *self, long long elapsed, bool flag);
extern "C" void AEGeometry_setScaling(AEGeometry *self, float x, float y, float z);
extern "C" void AEGeometry_setDirection(AEGeometry *self, const Vector *direction, const void *up);
extern "C" PlayerEgo *Level_getPlayer(Level *self);
extern "C" int Vector_ne(const Vector *lhs, const Vector *rhs);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void Matrix_mul(Matrix *out, const Matrix *lhs, const Matrix *rhs);
extern "C" void Vector_add(Vector *out, const Vector *lhs, const Vector *rhs);
void MatrixRotateVector(Vector *out, const Matrix *matrix, const Vector *vector);
extern "C" void Vector_add_assign(void *self, const Vector *rhs);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const void *position);
extern "C" void AEGeometry_setVisible(AEGeometry *self, bool visible);
void MatrixGetDir(Vector *out, const Matrix *matrix);

// ---- _BeamGun_177834.cpp ----
__attribute__((visibility("hidden"))) extern void *BeamGun_vtable;


BeamGun *_ZN7BeamGunD1Ev(BeamGun *self)
{
    self->field_0x0 = (char *)BeamGun_vtable + 8;

    AEGeometry *primary = self->field_0x18;
    if (primary != 0)
        operator_delete(AEGeometry_dtor(primary));
    self->field_0x18 = 0;

    AEGeometry *secondary = self->field_0x1c;
    if (secondary != 0)
        operator_delete(AEGeometry_dtor(secondary));
    self->field_0x1c = 0;

    return self;
}

// ---- setEnemies_177b20.cpp ----
// BeamGun::setEnemies(Array<Player*>*): forwards the enemy list's data to the
// engine handler.
void BeamGun::setEnemies(Array<Player *> *enemies)
{
    return BeamGun_setEnemies_tail(enemies->data());
}

// ---- render_17787c.cpp ----
void BeamGun::render()
{
    Gun *gun = this->field_0x8;
    if (gun->field_0x4c == 0)
        return;

    AEGeometry_render(this->field_0x18);

    if (this->field_0x21 != 0) {
        AEGeometry *secondary = this->field_0x1c;
        if (secondary != 0)
            return BeamGun_render_tail(secondary);
    }
}

// ---- _BeamGun_17786c.cpp ----
void _ZN7BeamGunD0Ev(BeamGun *self)
{
    return operator_delete(_ZN7BeamGunD1Ev(self));
}

// ---- setEnemy_177b26.cpp ----
// BeamGun::setEnemy(Player*): forwards the player's secondary geometry pointer
// (object field at +0x8) to the engine handler.
void BeamGun::setEnemy(Player *enemy)
{
    return BeamGun_setEnemy_tail(((void **)enemy)[2]);
}

// ---- BeamGun_177754.cpp ----
__attribute__((visibility("hidden"))) extern void **BeamGun_canvas;
__attribute__((visibility("hidden"))) extern int32_t BeamGun_secondaryMeshes[];


BeamGun::BeamGun(int param_1, Gun *gun, int param_3, Level *level)
{
    int type;
    AEGeometry *geometry;
    int mesh;

    type = param_3;
    param_3 = type - 9;
    if (type == 0xe4)
        param_3 = 2;

    this->field_0x0 = (char *)BeamGun_vtable + 8;
    this->field_0x4 = 0;
    this->field_0x8 = gun;
    this->field_0xc = level;
    this->field_0x10 = param_1;
    this->field_0x14 = param_3;

    geometry = (AEGeometry *)operator_new(0xc0);
    void **canvasHolder = BeamGun_canvas;
    uint32_t primaryMesh = (uint32_t)(param_3 + 0x3795);
    if (type == 0xe4)
        primaryMesh = 0x4a92;
    AEGeometry_ctor(geometry, (uint16_t)primaryMesh, (PaintCanvas *)*canvasHolder, false);
    this->field_0x18 = geometry;

    mesh = ((Gun *)(gun))->isPlayerGun();
    if (mesh == 0 ||
        (mesh = BeamGun_secondaryMeshes[gun->itemIndex], mesh < 0)) {
        geometry = 0;
        this->field_0x20 = 0;
    } else {
        int gunKind = gun->weaponType;
        this->field_0x20 = gunKind != 0xb;
        if (gunKind == 0xb) {
            geometry = 0;
        } else {
            geometry = (AEGeometry *)operator_new(0xc0);
            AEGeometry_ctor(geometry, (uint16_t)mesh, (PaintCanvas *)*canvasHolder, false);
        }
    }

    this->field_0x21 = 0;
    this->field_0x1c = geometry;
}

// ---- update_1778a4.cpp ----
__attribute__((visibility("hidden"))) extern void **BeamGun_canvas_update_a;
__attribute__((visibility("hidden"))) extern void **BeamGun_canvas_update_b;

typedef Transform *(*BeamGunGetTransformFn)(PaintCanvas *canvas, int transformId);
typedef void (*BeamGunSetAnimationFn)(Transform *transform, int state, void *arg);
__attribute__((visibility("hidden"))) extern BeamGunGetTransformFn BeamGun_getTransform_indirect;
__attribute__((visibility("hidden"))) extern BeamGunSetAnimationFn BeamGun_setAnimation_indirect;


void BeamGun::update(int elapsed)
{
    Vector back;
    Vector rotated;
    Matrix playerMatrix;
    Vector transformed;
    char position[12];

    Gun *gun = this->field_0x8;
    ((Gun *)(gun))->update(elapsed);

    gun = this->field_0x8;
    if (gun->field_0x4c == 0) {
        AEGeometry_setVisible(this->field_0x18, false);
        return;
    }

    if (gun->field_0x4d != 0) {
        void **canvasHolder = BeamGun_canvas_update_a;
        AEGeometry *geometry = this->field_0x18;
        Transform *transform =
            PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder,
                                              geometry->transform);
        Transform_SetAnimationState(transform, 3, 0);
        transform = PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                      geometry->transform);
        Transform_SetAnimationState(transform, 1, 0);
        this->field_0x8->field_0x4d = 0;
    }

    void **canvasHolder = BeamGun_canvas_update_b;
    AEGeometry *primary = this->field_0x18;
    Transform *transform =
        PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder, primary->transform);
    Transform_Update(transform, (long long)elapsed, false);

    gun = this->field_0x8;
    AEGeometry_setScaling(this->field_0x18, 1.0f, 1.0f,
                          (float)gun->field_0x8c);

    Vector *up = (Vector *)&playerMatrix;
    up->x = 0.0f;
    up->y = 1.0f;
    up->z = 0.0f;
    AEGeometry_setDirection(this->field_0x18, (Vector *)((char *)gun + 0x90), up);

    PlayerEgo *player = Level_getPlayer(this->field_0xc);
    ((PlayerEgo *)(&position))->getPosition();

    up->x = 0.0f;
    up->y = 0.0f;
    up->z = 0.0f;
    if (Vector_ne((Vector *)((char *)this->field_0x8 + 0x7c), up) != 0) {
        player = Level_getPlayer(this->field_0xc);
        Matrix *firstMatrix = AEGeometry_getMatrix(player->geometry);
        player = Level_getPlayer(this->field_0xc);
        Matrix *secondMatrix = AEGeometry_getMatrix(player->field_0x4);
        Matrix_mul(&playerMatrix, firstMatrix, secondMatrix);

        gun = this->field_0x8;
        back.x = 0.0f;
        back.y = 0.0f;
        back.z = -100.0f;
        Vector_add(&rotated, (Vector *)((char *)gun + 0x7c), &back);
        MatrixRotateVector(&transformed, &playerMatrix, &rotated);
        Vector_add_assign(position, &transformed);
    }

    AEGeometry_setPosition(this->field_0x18, position);
    AEGeometry_setVisible(this->field_0x18, true);

    if (this->field_0x20 != 0) {
        gun = this->field_0x8;
        if (gun->delayActive == 0) {
            AEGeometry *secondary = this->field_0x1c;
            BeamGunGetTransformFn getTransform = BeamGun_getTransform_indirect;
            Transform *secondaryTransform =
                getTransform((PaintCanvas *)*canvasHolder, secondary->transform);
            BeamGunSetAnimationFn setAnimation = BeamGun_setAnimation_indirect;
            setAnimation(secondaryTransform, 0, 0);
            secondaryTransform =
                getTransform((PaintCanvas *)*canvasHolder, secondary->transform);
            setAnimation(secondaryTransform, 3, 0);
            secondaryTransform =
                getTransform((PaintCanvas *)*canvasHolder, secondary->transform);
            setAnimation(secondaryTransform, 1, 0);
        } else {
            player = Level_getPlayer(this->field_0xc);
            ((PlayerEgo *)(&playerMatrix))->getPosition();

            gun = this->field_0x8;
            transformed = *(Vector *)&gun->offsetX;
            transformed.z = gun->offsetZ + -100.0f;

            MatrixRotateVector(&rotated, (Matrix *)((char *)player->player + 4),
                               &transformed);
            Vector_add_assign(&playerMatrix, &rotated);
            AEGeometry_setPosition(this->field_0x1c, &playerMatrix);

            AEGeometry *secondary = this->field_0x1c;
            transform = PaintCanvas_TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                          secondary->transform);
            Transform_Update(transform, (long long)elapsed, false);

            MatrixGetDir(&rotated, (Matrix *)((char *)player->player + 4));
            back.x = 0.0f;
            back.y = 1.0f;
            back.z = 0.0f;
            AEGeometry_setDirection(this->field_0x1c, &rotated, &back);
        }
    }

    this->field_0x21 = this->field_0x8->delayActive;
}
