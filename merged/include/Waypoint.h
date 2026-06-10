#ifndef WORK_CLASSES_WAYPOINT_SRC_CLASS_H
#define WORK_CLASSES_WAYPOINT_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Waypoint;
struct Route;
struct Player;
struct KIPlayer;
struct AEGeometry;

struct Vector {
    float x;
    float y;
    float z;
};

template <class T>
static inline T &F(void *self, unsigned offset)
{
    return *(T *)((char *)self + offset);
}

#endif
