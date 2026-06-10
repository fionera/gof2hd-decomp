#ifndef GOF2_SYSTEMPATHFINDER_H
#define GOF2_SYSTEMPATHFINDER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct SolarSystem;
struct Status;
struct SystemPathFinder;

struct Node {
    char opaque[12];
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

struct SystemPathFinder { void* _opaque; };  // no offset accesses observed
#endif
