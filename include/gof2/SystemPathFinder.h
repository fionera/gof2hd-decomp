#ifndef GOF2_SYSTEMPATHFINDER_H
#define GOF2_SYSTEMPATHFINDER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct SolarSystem;
struct Status;
struct SystemPathFinder;

struct Node {
    Array<Node *> *field_0x0;   // +0x00  children / outgoing edges
    Node *field_0x4;            // +0x04  parent link (path back-pointer)
    int field_0x8;              // +0x08  system index value
    Node(int index);
};





template <class T> void ArrayAdd(T item, Array<T> &array);
template <class T> void ArrayRemove(T item, Array<T> &array);
template <class T> void ArrayRelease(Array<T> &array);
template <class T> void ArrayReleaseClasses(Array<T> &array);
template <class T> void ArraySetLength(uint32_t length, Array<T> &array);

void *operator new(__SIZE_TYPE__ size);
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

class SystemPathFinder {
public:
    void *_opaque;  // no offset accesses observed
    int contains(Array<Node *> *nodes, Node *node);
    Array<Node *> *search(Node *start, Node *goal);
    int getJumpDistance(Array<SolarSystem *> *systems, int from, int to);
    Array<Node *> *constructPath(Node *node);
    Array<int> *getSystemPath(Array<SolarSystem *> *systems, int from, int to);
};
#endif
