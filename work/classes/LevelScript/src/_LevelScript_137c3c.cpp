#include "class.h"

extern "C" void *AEGeometry_dtor(void *geometry);
extern "C" void *Explosion_dtor(void *explosion);
extern "C" void operator_delete(void *ptr);

LevelScript::~LevelScript()
{
    void *geometry = P(this, 0xdc);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xdc) = 0;

    geometry = P(this, 0xd8);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xd8) = 0;

    geometry = P(this, 0xb8);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xb8) = 0;

    geometry = P(this, 0xbc);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xbc) = 0;

    geometry = P(this, 0xc0);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xc0) = 0;

    geometry = P(this, 0xc4);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xc4) = 0;

    geometry = P(this, 0xac);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xac) = 0;

    geometry = P(this, 0xb0);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xb0) = 0;

    geometry = P(this, 0xb4);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xb4) = 0;

    void *explosion = P(this, 0xc8);
    if (explosion != 0) {
        operator_delete(Explosion_dtor(explosion));
    }
    P(this, 0xc8) = 0;

    geometry = P(this, 0xcc);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(this, 0xcc) = 0;
}
