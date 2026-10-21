#ifndef GOF2_MATRIX_H
#define GOF2_MATRIX_H
#include "engine/math/Vector.h"

namespace AbyssEngine {
    namespace AEMath {
        struct Matrix {
            // Canonical 15-float storage. Named element views were removed (former union);
            // cross-file uses now index m[] directly: e3=m[3], m11_rightY=m[4], m12_upY=m[5],
            // e7=m[7], e11=m[11].
            float m[15];

            Matrix() {
                m[0] = 1.0f;
                m[1] = 0.0f;
                m[2] = 0.0f;
                m[3] = 0.0f;
                m[4] = 0.0f;
                m[5] = 1.0f;
                m[6] = 0.0f;
                m[7] = 0.0f;
                m[8] = 0.0f;
                m[9] = 0.0f;
                m[10] = 1.0f;
                m[11] = 0.0f;
                // Through the inline scaleVector() accessor: at codegen time the ctor body contains a call
                // to a not-yet-emitted inline function, so the ctor is not inferred nounwind and callers in
                // a cleanup scope keep it as an invoke (the original's landing pads).
                Vector &scale = scaleVector();
                scale.x = 1.0f;
                scale.y = 1.0f;
                scale.z = 1.0f;
            }

            operator float *();

            operator const float *() const;

            // The original Matrix is { float rows[12]; Vector scale; }: m[12..14] is a Vector that the
            // binary copies/multiplies through the (out-of-line) Vector operators.
            Vector &scaleVector() { return *reinterpret_cast<Vector *>(&m[12]); }
            const Vector &scaleVector() const { return *reinterpret_cast<const Vector *>(&m[12]); }

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
