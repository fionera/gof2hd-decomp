#ifndef GOF2_MATRIX_H
#define GOF2_MATRIX_H

namespace AbyssEngine {
    namespace AEMath {
        struct Matrix {
            // 3x4 row-major affine matrix padded to 16 floats. The raw `m[16]` array and the
            // named element view alias the SAME storage (anonymous union), so existing index-based
            // code keeps working while byte-offset accesses resolve to named members at the exact
            // offset/width. Row 1 (bytes 0x10..0x1c) is the matrix's "up"/right basis row used by
            // PlayerEgo::roll: +0x10 is the right-vector Y (m11_rightY), +0x14 is the up-vector Y
            // (m12_upY). Adding these named members does not change the type's size/layout/mangling.
            union {
                float m[16];

                struct {
                    float e0, e1, e2, e3; // +0x00 +0x04 +0x08 +0x0c
                    float m11_rightY, m12_upY, e6, e7; // +0x10 +0x14 +0x18 +0x1c
                    float e8, e9, e10, e11; // +0x20 +0x24 +0x28 +0x2c
                    float e12, e13, e14, e15; // +0x30 +0x34 +0x38 +0x3c
                };
            };

            Matrix(); // out-of-line: initializes to the identity transform
            operator float *(); // out-of-line (the binary emits these accessors)
            operator const float *() const;

            float &operator[](int i) { return m[i]; }
            float operator[](int i) const { return m[i]; }

            Matrix &operator=(const Matrix &o);

            Matrix &operator*=(const Matrix &o);
        };
    } // namespace AEMath

    static_assert(__builtin_offsetof(AEMath::Matrix, m) == 0x00, "AEMath::Matrix::m offset");
    static_assert(__builtin_offsetof(AEMath::Matrix, m11_rightY) == 0x10, "AEMath::Matrix::m11_rightY offset");
    static_assert(__builtin_offsetof(AEMath::Matrix, m12_upY) == 0x14, "AEMath::Matrix::m12_upY offset");
    static_assert(sizeof(AEMath::Matrix) == 0x40, "AEMath::Matrix size");
} // namespace AbyssEngine

#endif // GOF2_MATRIX_H
