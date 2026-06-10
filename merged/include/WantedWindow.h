#ifndef WORK_CLASSES_WANTEDWINDOW_SRC_CLASS_H
#define WORK_CLASSES_WANTEDWINDOW_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct WantedWindow;
struct Wanted;
struct Station;
struct Mission;
struct StarMap;
struct ScrollTouchWindow;
struct TouchButton;
struct Layout;
struct PaintCanvas;
struct ImageFactory;
struct String { char bytes[12]; };

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;
};

template <class T> static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

static inline int *IArrayData(void *arr) {
    return F<int *>(arr, 4);
}

static inline void *ArrayItem(void *arr, int idx) {
    return *(void **)((char *)F<void *>(arr, 4) + idx * 4);
}

struct Blk16 { int a, b, c, d; };

extern "C" void *operator_new(unsigned size);
extern "C" void operator_delete(void *p);

#endif
