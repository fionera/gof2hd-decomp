#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/math/Quaternion.h"

// Matrix::operator= and operator*= are emitted from AEMath.cpp -- that TU's copies are the ones
// the link keeps (object order), matching the original binary.
namespace AbyssEngine {
    namespace AEMath {
        Matrix::Matrix() {
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
            m[10] = 0.0f;
            m[11] = 0.0f;
            m[12] = 1.0f;
            m[13] = 0.0f;
            m[14] = 1.0f;
        }

        Matrix::operator float *() { return m; }
        Matrix::operator const float *() const { return m; }
    }
}

namespace AbyssEngine {
    Quaternion::~Quaternion() {
    }

    Quaternion::operator float *() { return &x; }
    Quaternion::operator const float *() const { return &x; }
    float &Quaternion::operator[](int i) { return (&x)[i]; }
    float Quaternion::operator[](int i) const { return (&x)[i]; }
}
