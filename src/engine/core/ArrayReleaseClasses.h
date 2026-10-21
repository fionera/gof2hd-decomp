#ifndef GALAXYONFIRE2_ARRAYRELEASECLASSES_H
#define GALAXYONFIRE2_ARRAYRELEASECLASSES_H
#include "engine/core/Array.h"

template<class T>
void ArrayReleaseClasses(Array<T> &a) {
    for (unsigned int i = 0; i < a.capacity_; ++i) {
        if (a.data_[i]) delete a.data_[i];
        a.data_[i] = nullptr;
    }
    if (a.data_) delete[] a.data_;
    a.data_ = nullptr;
}

#endif
