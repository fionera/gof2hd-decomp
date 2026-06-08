#ifndef WORK_CLASSES_PLAYERASTEROID_SRC_CLASS_H
#define WORK_CLASSES_PLAYERASTEROID_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {

struct String {
    void *text;
    uint32_t field_04;
    uint32_t size;

    String(const char *text, bool copy);
};

namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    float m[16];
};
} // namespace AEMath

} // namespace AbyssEngine

using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

struct AEGeometry;
struct Player;
struct Explosion;
struct TargetFollowCamera;
struct ArrayInt;

template <class T>
static inline T &F(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline T const &F(const void *self, uint32_t off)
{
    return *(const T *)((const char *)self + off);
}

struct PlayerAsteroid {
    PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                   const Vector &position, float scaling, int quality);

    void setAsteroidIndex(int asteroidIndex);
    Vector getPosition();
    void setPosition(const Vector &position);
    void translate(const Vector &delta);
    uint8_t isMinable();
    int getQuality();
    float getScaling();
    int getQualityFrameIndex();
    String getQualityString();
    void setAsteroidCenter(Vector center);
    void setRotationEnabled(bool enabled);
    void initPush(const Vector &target, int duration);
    void push(int delta);
    void update(int delta);
    void render();
    void outerCollide(float x, float y, float z);
    void outerCollide(Vector value);
    Vector getProjectionVector(const Vector &value);
    bool collide(float x, float y, float z);
};

extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr);

#endif
