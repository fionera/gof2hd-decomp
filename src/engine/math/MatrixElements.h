#ifndef GOF2_MATRIXELEMENTS_H
#define GOF2_MATRIXELEMENTS_H

namespace AbyssEngine {
    namespace AEMath {
        // The 15 scalar elements of a Matrix, indexed as m[i]. Not a C array: see Matrix.h.
        struct MatrixElements {
            float e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14;

            float &operator[](int i) { return (&e0)[i]; }

            const float &operator[](int i) const { return (&e0)[i]; }

            operator float *() { return &e0; }

            operator const float *() const { return &e0; }
        };
    }
}

#endif
