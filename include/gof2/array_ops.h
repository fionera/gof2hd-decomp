#ifndef GOF2_ARRAY_OPS_H
#define GOF2_ARRAY_OPS_H
#include "gof2/common.h"   // Array<T> == std::vector<T>
//
// Owning-array teardown helpers.
//
// Array<T> is std::vector<T>, so a container's lifetime is just `new Array<T>` /
// `delete`, its size is `size()`, growth is `resize()`, and append is `push_back()`
// — those are written directly at the call sites. The one operation std::vector
// does not provide is destroying the *pointees* of an Array<T*> that owns its
// elements; the engine called that `releaseClasses` (and `releaseArrays` for an
// Array of owned Array*). Those map to the helpers below.
//
namespace AbyssEngine {
namespace arr {

// An Array<T*> that OWNS its elements: delete each element, then clear.
template <class T> inline void releaseClasses(Array<T *> &a) {
    for (T *e : a) delete e;
    a.clear();
}
template <class T> inline void releaseClasses(Array<T *> *a) { releaseClasses(*a); }

} // namespace arr
} // namespace AbyssEngine

#endif // GOF2_ARRAY_OPS_H
