#ifndef WORK_CLASSES_SYSTEMPATHFINDER_SRC_CLASS_H
#define WORK_CLASSES_SYSTEMPATHFINDER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

struct SolarSystem;
struct Status;
struct SystemPathFinder;

struct Node {
    char opaque[12];
    Node(int index);
};

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;

    Array();
    ~Array();
};

template <class T>
static inline T &F(void *self, unsigned off) {
    return *(T *)((char *)self + off);
}

template <class T> void ArrayAdd(T item, Array<T> &array);
template <class T> void ArrayRemove(T item, Array<T> &array);
template <class T> void ArrayRelease(Array<T> &array);
template <class T> void ArrayReleaseClasses(Array<T> &array);
template <class T> void ArraySetLength(uint32_t length, Array<T> &array);

void *operator new(unsigned size);
void operator delete(void *ptr) noexcept;

struct SolarSystem {
    int getIndex();
    Array<int> *getRoutes();
};

struct Status {
    Array<uint8_t> *getSystemVisibilities();
};

extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_SystemPathFinder_status;

struct SystemPathFinder {
    int contains(Array<Node *> *nodes, Node *node);
    int getJumpDistance(Array<SolarSystem *> *systems, int from, int to);
    Array<Node *> *constructPath(Node *node);
    Array<Node *> *search(Node *start, Node *goal);
    Array<int> *getSystemPath(Array<SolarSystem *> *systems, int from, int to);
};

#endif
