#include "gof2/BeamGun.h"
#include "gof2/Level.h"
#include "gof2/Transform.h"
#include "gof2/Gun.h"
#include "gof2/PlayerEgo.h"
#include "gof2/AEGeometry.h"
#include <cstring>

namespace AbyssEngine { namespace AEMath {
Vector operator+(const Vector &lhs, const Vector &rhs);
Matrix operator*(const Matrix &lhs, const Matrix &rhs);
} }

// AEGeometry's full header is not yet native-clean; Gun.h already completes the
// global forward declaration with a minimal view exposing field_0xc (transform id),
// which is the only AEGeometry field this TU reads by name.

extern "C" void BeamGun_setEnemies_tail(void *data);
extern "C" void BeamGun_render_tail(AEGeometry *self);
BeamGun *_ZN7BeamGunD1Ev(BeamGun *self);
extern "C" void BeamGun_setEnemy_tail(void *data);
namespace AbyssEngine { namespace PaintCanvas {
::Transform *TransformGetTransform(::PaintCanvas *canvas, int transformId);
} }
void MatrixRotateVector(Vector *out, const Matrix *matrix, const Vector *vector);
void MatrixGetDir(Vector *out, const Matrix *matrix);

// ---- _BeamGun_177834.cpp ----
__attribute__((visibility("hidden"))) extern void *BeamGun_vtable;


BeamGun *_ZN7BeamGunD1Ev(BeamGun *self)
{
    self->field_0x0 = (char *)BeamGun_vtable + 8;

    AEGeometry *primary = self->field_0x18;
    if (primary != 0) {
        ((AEGeometry *)primary)->~AEGeometry();
        ::operator delete(primary);
    }
    self->field_0x18 = 0;

    AEGeometry *secondary = self->field_0x1c;
    if (secondary != 0) {
        ((AEGeometry *)secondary)->~AEGeometry();
        ::operator delete(secondary);
    }
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

    ((AEGeometry *)(this->field_0x18))->render();

    if (this->field_0x21 != 0) {
        AEGeometry *secondary = this->field_0x1c;
        if (secondary != 0)
            return BeamGun_render_tail(secondary);
    }
}

// ---- _BeamGun_17786c.cpp ----
void _ZN7BeamGunD0Ev(BeamGun *self)
{
    return ::operator delete(_ZN7BeamGunD1Ev(self));
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

    geometry = (AEGeometry *)::operator new(0xc0);
    void **canvasHolder = BeamGun_canvas;
    uint32_t primaryMesh = (uint32_t)(param_3 + 0x3795);
    if (type == 0xe4)
        primaryMesh = 0x4a92;
    new ((void *)geometry) AEGeometry((uint16_t)primaryMesh, (PaintCanvas *)*canvasHolder, false);
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
            geometry = (AEGeometry *)::operator new(0xc0);
            new ((void *)geometry) AEGeometry((uint16_t)mesh, (PaintCanvas *)*canvasHolder, false);
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
        ((AEGeometry *)(this->field_0x18))->setVisible(false);
        return;
    }

    if (gun->field_0x4d != 0) {
        void **canvasHolder = BeamGun_canvas_update_a;
        AEGeometry *geometry = this->field_0x18;
        Transform *transform =
            AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder,
                                              geometry->transform);
        ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
        transform = AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                      geometry->transform);
        ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
        this->field_0x8->field_0x4d = 0;
    }

    void **canvasHolder = BeamGun_canvas_update_b;
    AEGeometry *primary = this->field_0x18;
    Transform *transform =
        AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder, primary->transform);
    ((AbyssEngine::Transform *)(transform))->Update((long long)elapsed, false);

    gun = this->field_0x8;
    ((AEGeometry *)(this->field_0x18))->setScaling(1.0f);

    Vector *up = (Vector *)&playerMatrix;
    up->x = 0.0f;
    up->y = 1.0f;
    up->z = 0.0f;
    ((AEGeometry *)(this->field_0x18))->setDirection(*(Vector *)((char *)gun + 0x90), *up);

    PlayerEgo *player = (PlayerEgo *)((Level *)(this->field_0xc))->getPlayer();
    ((PlayerEgo *)(&position))->getPosition();

    up->x = 0.0f;
    up->y = 0.0f;
    up->z = 0.0f;
    if ((memcmp(((Vector *)((char *)this->field_0x8 + 0x7c)), (up), sizeof(float) * 3) != 0) != 0) {
        player = (PlayerEgo *)((Level *)(this->field_0xc))->getPlayer();
        Matrix *firstMatrix = &((AEGeometry *)(player->geometry))->getMatrix();
        player = (PlayerEgo *)((Level *)(this->field_0xc))->getPlayer();
        Matrix *secondMatrix = &((AEGeometry *)(player->field_0x4))->getMatrix();
        *(Matrix *)(&playerMatrix) = *(const Matrix *)(firstMatrix) * *(const Matrix *)(secondMatrix);

        gun = this->field_0x8;
        back.x = 0.0f;
        back.y = 0.0f;
        back.z = -100.0f;
        *(Vector *)(&rotated) = *(const Vector *)((Vector *)((char *)gun + 0x7c)) + *(const Vector *)(&back);
        MatrixRotateVector(&transformed, &playerMatrix, &rotated);
        *(Vector *)(position) += *(const Vector *)(&transformed);
    }

    ((AEGeometry *)(this->field_0x18))->setPosition(*(Vector *)position);
    ((AEGeometry *)(this->field_0x18))->setVisible(true);

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
            player = (PlayerEgo *)((Level *)(this->field_0xc))->getPlayer();
            ((PlayerEgo *)(&playerMatrix))->getPosition();

            gun = this->field_0x8;
            transformed = *(Vector *)&gun->offsetX;
            transformed.z = gun->offsetZ + -100.0f;

            MatrixRotateVector(&rotated, (Matrix *)((char *)player->player + 4),
                               &transformed);
            *(Vector *)(&playerMatrix) += *(const Vector *)(&rotated);
            ((AEGeometry *)(this->field_0x1c))->setPosition(*(Vector *)&playerMatrix);

            AEGeometry *secondary = this->field_0x1c;
            transform = AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                          secondary->transform);
            ((AbyssEngine::Transform *)(transform))->Update((long long)elapsed, false);

            MatrixGetDir(&rotated, (Matrix *)((char *)player->player + 4));
            back.x = 0.0f;
            back.y = 1.0f;
            back.z = 0.0f;
            ((AEGeometry *)(this->field_0x1c))->setDirection(rotated, back);
        }
    }

    this->field_0x21 = this->field_0x8->delayActive;
}
