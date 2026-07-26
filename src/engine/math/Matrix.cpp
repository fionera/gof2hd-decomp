#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/math/Quaternion.h"

// Matrix::operator= and operator*= are emitted from AEMath.cpp -- that TU's copies are the ones
// the link keeps (object order), matching the original binary.
namespace AbyssEngine {
    namespace AEMath {
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
