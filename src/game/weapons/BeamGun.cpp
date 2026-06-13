#include "gof2/game/weapons/BeamGun.h"
#include "gof2/game/world/Level.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/engine/render/AEGeometry.h"
#include <cstring>

namespace AbyssEngine { namespace AEMath {
Vector operator+(const Vector &lhs, const Vector &rhs);
Matrix operator*(const Matrix &lhs, const Matrix &rhs);
} }

// AEGeometry's full header is not yet native-clean; Gun.h already completes the
// global forward declaration with a minimal view exposing field_0xc (transform id),
// which is the only AEGeometry field this TU reads by name.

BeamGun *_ZN7BeamGunD1Ev(BeamGun *self);
namespace AbyssEngine { namespace PaintCanvas {
::Transform *TransformGetTransform(::PaintCanvas *canvas, int transformId);
} }
void MatrixRotateVector(Vector *out, const Matrix *matrix, const Vector *vector);
void MatrixGetDir(Vector *out, const Matrix *matrix);

__attribute__((visibility("hidden"))) extern void *BeamGun_vtable;

BeamGun *_ZN7BeamGunD1Ev(BeamGun *self)
{
    self->vtable = (char *)BeamGun_vtable + 8;

    AEGeometry *primary = self->primaryGeometry;
    if (primary != 0) {
        ((AEGeometry *)primary)->~AEGeometry();
        ::operator delete(primary);
    }
    self->primaryGeometry = 0;

    AEGeometry *secondary = self->secondaryGeometry;
    if (secondary != 0) {
        ((AEGeometry *)secondary)->~AEGeometry();
        ::operator delete(secondary);
    }
    self->secondaryGeometry = 0;

    return self;
}

// BeamGun::setEnemies(Array<Player*>*): forwards the enemy list's data to the
// engine handler.
void BeamGun::setEnemies(Array<Player *> *enemies)
{
    return this->setEnemies_tail(enemies->data());
}

void BeamGun::render()
{
    Gun *gun = this->gun;
    if (gun->field_0x4c == 0)
        return;

    ((AEGeometry *)(this->primaryGeometry))->render();

    if (this->secondaryVisible != 0) {
        AEGeometry *secondary = this->secondaryGeometry;
        if (secondary != 0)
            return this->render_tail(secondary);
    }
}

void _ZN7BeamGunD0Ev(BeamGun *self)
{
    return ::operator delete(_ZN7BeamGunD1Ev(self));
}

// BeamGun::setEnemy(Player*): forwards the player's secondary geometry pointer
// (object field at +0x8) to the engine handler.
void BeamGun::setEnemy(Player *enemy)
{
    return this->setEnemy_tail(((void **)enemy)[2]);
}

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

    this->vtable = (char *)BeamGun_vtable + 8;
    this->field_0x4 = 0;
    this->gun = gun;
    this->level = level;
    this->owner = param_1;
    this->meshKind = param_3;

    geometry = (AEGeometry *)::operator new(0xc0);
    void **canvasHolder = BeamGun_canvas;
    uint32_t primaryMesh = (uint32_t)(param_3 + 0x3795);
    if (type == 0xe4)
        primaryMesh = 0x4a92;
    new ((void *)geometry) AEGeometry((uint16_t)primaryMesh, (PaintCanvas *)*canvasHolder, false);
    this->primaryGeometry = geometry;

    mesh = ((Gun *)(gun))->isPlayerGun();
    if (mesh == 0 ||
        (mesh = BeamGun_secondaryMeshes[gun->itemIndex], mesh < 0)) {
        geometry = 0;
        this->hasSecondary = 0;
    } else {
        int gunKind = gun->weaponType;
        this->hasSecondary = gunKind != 0xb;
        if (gunKind == 0xb) {
            geometry = 0;
        } else {
            geometry = (AEGeometry *)::operator new(0xc0);
            new ((void *)geometry) AEGeometry((uint16_t)mesh, (PaintCanvas *)*canvasHolder, false);
        }
    }

    this->secondaryVisible = 0;
    this->secondaryGeometry = geometry;
}

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

    Gun *gun = this->gun;
    ((Gun *)(gun))->update(elapsed);

    gun = this->gun;
    if (gun->field_0x4c == 0) {
        ((AEGeometry *)(this->primaryGeometry))->setVisible(false);
        return;
    }

    if (gun->field_0x4d != 0) {
        void **canvasHolder = BeamGun_canvas_update_a;
        AEGeometry *geometry = this->primaryGeometry;
        Transform *transform =
            AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder,
                                              geometry->transform);
        ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
        transform = AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                      geometry->transform);
        ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
        this->gun->field_0x4d = 0;
    }

    void **canvasHolder = BeamGun_canvas_update_b;
    AEGeometry *primary = this->primaryGeometry;
    Transform *transform =
        AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder, primary->transform);
    ((AbyssEngine::Transform *)(transform))->Update((long long)elapsed, false);

    gun = this->gun;
    ((AEGeometry *)(this->primaryGeometry))->setScaling(1.0f);

    Vector *up = (Vector *)&playerMatrix;
    up->x = 0.0f;
    up->y = 1.0f;
    up->z = 0.0f;
    ((AEGeometry *)(this->primaryGeometry))->setDirection(*(Vector *)&gun->field_0x90, *up);

    PlayerEgo *player = (PlayerEgo *)((Level *)(this->level))->getPlayer();
    ((PlayerEgo *)(&position))->getPosition();

    up->x = 0.0f;
    up->y = 0.0f;
    up->z = 0.0f;
    if ((memcmp((Vector *)&this->gun->offset, (up), sizeof(float) * 3) != 0) != 0) {
        player = (PlayerEgo *)((Level *)(this->level))->getPlayer();
        Matrix *firstMatrix = &((AEGeometry *)(player->geometry))->getMatrix();
        player = (PlayerEgo *)((Level *)(this->level))->getPlayer();
        Matrix *secondMatrix = &((AEGeometry *)(player->field_0x4))->getMatrix();
        *(Matrix *)(&playerMatrix) = *(const Matrix *)(firstMatrix) * *(const Matrix *)(secondMatrix);

        gun = this->gun;
        back.x = 0.0f;
        back.y = 0.0f;
        back.z = -100.0f;
        *(Vector *)(&rotated) = *(const Vector *)((Vector *)&gun->offset) + *(const Vector *)(&back);
        MatrixRotateVector(&transformed, &playerMatrix, &rotated);
        *(Vector *)(position) += *(const Vector *)(&transformed);
    }

    ((AEGeometry *)(this->primaryGeometry))->setPosition(*(Vector *)position);
    ((AEGeometry *)(this->primaryGeometry))->setVisible(true);

    if (this->hasSecondary != 0) {
        gun = this->gun;
        if (gun->delayActive == 0) {
            AEGeometry *secondary = this->secondaryGeometry;
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
            player = (PlayerEgo *)((Level *)(this->level))->getPlayer();
            ((PlayerEgo *)(&playerMatrix))->getPosition();

            gun = this->gun;
            transformed = *(Vector *)&gun->offset;
            transformed.z = gun->offset.z + -100.0f;

            MatrixRotateVector(&rotated, (Matrix *)((char *)player->player + 4),
                               &transformed);
            *(Vector *)(&playerMatrix) += *(const Vector *)(&rotated);
            ((AEGeometry *)(this->secondaryGeometry))->setPosition(*(Vector *)&playerMatrix);

            AEGeometry *secondary = this->secondaryGeometry;
            transform = AbyssEngine::PaintCanvas::TransformGetTransform((PaintCanvas *)*canvasHolder,
                                                          secondary->transform);
            ((AbyssEngine::Transform *)(transform))->Update((long long)elapsed, false);

            MatrixGetDir(&rotated, (Matrix *)((char *)player->player + 4));
            back.x = 0.0f;
            back.y = 1.0f;
            back.z = 0.0f;
            ((AEGeometry *)(this->secondaryGeometry))->setDirection(rotated, back);
        }
    }

    this->secondaryVisible = this->gun->delayActive;
}

// ---- tail-call veneer fragments ----
// Each is the terminal b.w of a BeamGun method into a relocated slot that lands
// in the inherited engine implementation (the enemy-list handler shared by the
// gun hierarchy, and AEGeometry::render). They have no static body of their own
// (pure GOT veneer), so the work lives behind the extern shim the linker
// resolves. Modeling them as members keeps the original control flow intact.

// The veneers do a terminal b.w into a relocated slot that lands in the inherited
// engine implementation. We model those landing pads as extern engine routines.
void AEGeometryRender(AEGeometry *self);                // AEGeometry::render landing pad

// setEnemies() tail: hand the enemy list's raw element buffer to the engine.
void BeamGun::setEnemies_tail(void *data) {
    this->setEnemiesEngine(data);
}

// setEnemy() tail: hand the single enemy's secondary geometry pointer to the engine.
void BeamGun::setEnemy_tail(void *data) {
    this->setEnemyEngine(data);
}

// ---------------------------------------------------------------------------
// Inherited gun-hierarchy enemy handlers (the landing pads behind the tail
// veneers above).
//
// In the binary, BeamGun::setEnemies / setEnemy are themselves veneers whose
// terminal b.w dispatches through a single relocated GOT slot (the same slot
// shared by every *Gun::setEnemies in the hierarchy). The dynamic linker fills
// that slot at load time with the inherited base-class enemy handler, so the
// target has no static body of its own in this image -- it is reached purely by
// the runtime relocation. We model that indirect dispatch faithfully: read the
// resolved handler out of the engine's relocation slot and forward the data
// pointer the tail already extracted.
// ---------------------------------------------------------------------------
__attribute__((visibility("hidden"))) extern void (*BeamGun_enemiesHandler_slot)(void *);
__attribute__((visibility("hidden"))) extern void (*BeamGun_enemyHandler_slot)(void *);

// BeamGun::setEnemiesEngine / setEnemyEngine — the inherited gun-hierarchy enemy
// landing pads behind the setEnemies()/setEnemy() tail veneers. The terminal b.w
// of each method dispatches through a relocated GOT slot that the dynamic linker
// fills with the shared base-class handler; we read the resolved handler out of
// the engine's relocation slot and forward the data pointer the tail extracted.
void BeamGun::setEnemiesEngine(void *data) {
    BeamGun_enemiesHandler_slot(data);
}

void BeamGun::setEnemyEngine(void *data) {
    BeamGun_enemyHandler_slot(data);
}

// render() tail: render the secondary beam geometry via AEGeometry::render.
void BeamGun::render_tail(AEGeometry *self) {
    AEGeometryRender(self);
}
