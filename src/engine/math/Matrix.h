#ifndef GOF2_MATRIX_H
#define GOF2_MATRIX_H

namespace AbyssEngine {
    namespace AEMath {
        struct Matrix {
            // Canonical 15-float storage. Named element views were removed (former union);
            // cross-file uses now index m[] directly: e3=m[3], m11_rightY=m[4], m12_upY=m[5],
            // e7=m[7], e11=m[11].
            float m[15];

            Matrix();

            operator float *();

            operator const float *() const;

            float &operator[](int i) { return m[i]; }
            float operator[](int i) const { return m[i]; }

            Matrix &operator=(const Matrix &o);

            Matrix &operator*=(const Matrix &o);
        };
    }

    using AEMath::Matrix;

    static_assert(__builtin_offsetof(AEMath::Matrix, m) == 0x00, "AEMath::Matrix::m offset");
    static_assert(sizeof(AEMath::Matrix) == 0x3c, "AEMath::Matrix size");
}

using ::AbyssEngine::Matrix;

#endif
