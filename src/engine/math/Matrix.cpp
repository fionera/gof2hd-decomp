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
