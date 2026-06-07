#ifndef WORK_CLASSES_PLAYERCREATURE_SRC_CLASS_H
#define WORK_CLASSES_PLAYERCREATURE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

struct PlayerCreature;
struct KIPlayer;
struct Player;
struct AEGeometry;
struct FModSound;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;
};

struct Matrix {
    float m[15];
    Matrix();
    Matrix &operator=(const Matrix &other);
};

void MatrixMultiply(Matrix &lhs, const Matrix &rhs);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);

}
}

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;

template <class T>
static inline T &F(PlayerCreature *self, unsigned off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline T &F(void *self, unsigned off)
{
    return *(T *)((char *)self + off);
}

#endif
