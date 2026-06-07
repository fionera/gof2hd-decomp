#ifndef WORK_CLASSES_STARMAP_SRC_CLASS_H
#define WORK_CLASSES_STARMAP_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Mission;

struct StarMap {
    StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4);
    ~StarMap();

    uint8_t missionChanged();
    void setJumpMapMode(bool enabled, bool value);
    bool isInPlanetMode();
    void setStart(int start, int target);
    void initLights();
    void askForJumpIntoAlienWorld();
    void render();
    void depart(bool jump);
    void drawKey();
    uint32_t OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    int init(bool jumpMapMode, Mission *mission, bool param3, int param4);
    void initStarSystem();
    void draw();
    void OnTouchEnd(int x, int y);
    void update(int dt);
    void drawOnScreenInfo(int y, bool selected);
};

template <class T>
static inline T &field(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

static inline void *&ptr_field(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

struct String {
    uint32_t a;
    uint32_t b;
    uint32_t c;
};

struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    uint32_t words[15];
};

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;
};

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;
void *operator new[](uint32_t size);
void operator delete[](void *ptr) noexcept;

#endif
