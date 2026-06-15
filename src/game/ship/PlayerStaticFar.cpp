#include "gof2/game/ship/PlayerStaticFar.h"

#include <vector>

#include "gof2/math.h"
#include "gof2/engine/math/AEMath.h"
#include "gof2/engine/math/BoundingVolume.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/ship/PlayerStatic.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/core/PaintCanvasClass.h"

using namespace AbyssEngine::AEMath;

// Engine-resolved placement parameters for the far-projection: the camera holder,
// the distance threshold below which the object is drawn at full scale, the sphere
// radius used to project distant objects, and the scale-down factors.
static PaintCanvas *g_cameraHolder;
static int          g_distLimit;
static float        g_radius;
static float        g_scaleNum;
static float        g_scaleFactor;

PlayerStaticFar::PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z)
    : PlayerStatic(playerId, geometry, x, y, z)
{
    this->viewDirection = {0.0f, 0.0f, 0.0f};
    this->initPosition  = {x, y, z};
    this->player->setRadius(0x1d4c);
    this->boundingVolumes = nullptr;
}

PlayerStaticFar::~PlayerStaticFar() = default;

PlayerStaticFar::Vector PlayerStaticFar::getProjectionVector(const Vector &value)
{
    if (this->boundingVolumes != nullptr) {
        return (*this->boundingVolumes)[0]->getProjectionVector(value);
    }
    return {0.0f, 0.0f, 0.0f};
}

void PlayerStaticFar::render()
{
    this->geometry->render();
}

PlayerStaticFar::Vector PlayerStaticFar::projectCollisionOnSurface(const Vector &value)
{
    if (this->boundingVolumes != nullptr) {
        Vector out;
        reinterpret_cast<BoundingVolume *>(&out)
            ->staticProjectCollisionOnSurface(value, this->boundingVolumes);
        return out;
    }
    return {0.0f, 0.0f, 0.0f};
}

bool PlayerStaticFar::outerCollide(float x, float y, float z)
{
    return this->collide(x, y, z);
}

void PlayerStaticFar::outerCollide(Vector value)
{
    this->collide(value.x, value.y, value.z);
}

PlayerStaticFar::Vector PlayerStaticFar::getInitPosition()
{
    return this->initPosition;
}

// Reproject the object's stored integer position relative to the current camera.
// If the camera-relative distance is below g_distLimit it is drawn at full scale at
// its literal position; otherwise it is placed on a sphere of radius g_radius along
// the view direction and scaled down by distance.
void PlayerStaticFar::update(int /*delta*/)
{
    if (this->geometry == nullptr)
        return;

    PaintCanvas *camera = g_cameraHolder;
    unsigned int cur    = camera->CameraGetCurrent();
    Matrix      *matrix = reinterpret_cast<Matrix *>(camera->CameraGetLocal(cur));

    this->cameraPosition = MatrixGetPosition(*matrix);
    this->objectPosition = {(float)this->posX, (float)this->posY, (float)this->posZ};
    this->viewDirection  = this->objectPosition - this->cameraPosition;

    float len = VectorLength(this->viewDirection);

    if ((int)len < g_distLimit) {
        // Close: full-scale at the literal integer position.
        this->geometry->setScaling(1.0f);
        this->geometry->setPosition(
            Vector{(float)this->posX, (float)this->posY, (float)this->posZ});
    } else {
        // Far: place on a sphere of radius g_radius along the view direction.
        this->viewDirection = VectorNormalize(this->viewDirection);
        this->viewDirection *= g_radius;
        this->viewDirection += this->cameraPosition;
        this->geometry->setPosition(this->viewDirection);

        float s = (float)(int)((g_radius / (float)(int)len) * g_scaleNum);
        s = s * g_scaleFactor;
        this->geometry->setScaling(s);
    }
}

// collide(x,y,z): true iff (x,y,z) lies strictly inside the +/- bound box centered
// at the stored integer position, where bound is the player's radius in world units.
bool PlayerStaticFar::collide(float x, float y, float z)
{
    int   boundI   = this->player->radius;
    float bound    = (float)boundI;
    float negBound = (float)(-boundI);

    float dx = x - (float)this->posX;
    if (dx >= bound || dx <= negBound)
        return false;

    float dy = y - (float)this->posY;
    if (dy >= bound || dy <= negBound)
        return false;

    float dz = z - (float)this->posZ;
    if (dz >= bound || dz <= negBound)
        return false;

    return true;
}
