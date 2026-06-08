#ifndef WORK_CLASSES_OBJECTGUN_SRC_CLASS_H
#define WORK_CLASSES_OBJECTGUN_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

struct AEGeometry;
struct Array;
struct Explosion;
struct Gun;
struct Level;
struct ObjectGun;
struct PaintCanvas;
struct Player;
struct TargetFollowCamera;

struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    uint32_t words[15];
};

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

struct MeshId {
    uint16_t id;
    uint16_t pad;
};

struct ObjectGun {
    ObjectGun(int owner, Gun *gun, int mesh, uint32_t flags, Level *level);
    ~ObjectGun();

    static void setEnemies(Array *enemies);
    void setScaling(int x, int y);
    void replaceGun(int mesh);
    void update(int dt);
    void render();
};

template <class T>
static inline T &F(void *self, unsigned off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline const T &F(const void *self, unsigned off)
{
    return *(const T *)((const char *)self + off);
}

#endif
