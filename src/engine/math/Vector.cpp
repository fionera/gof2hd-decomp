#include "engine/math/Vector.h"

// Vector's compound-assignment operators (=, -=, *=, /=) are emitted from AEMath.cpp -- that TU's
// copies are the ones the link keeps (object order), matching the original binary. Only the
// accessors below are emitted from this TU.
namespace AbyssEngine {
    namespace AEMath {
        Vector::operator float *() { return &x; }
        Vector::operator const float *() const { return &x; }

        float &Vector::operator[](int i) { return (&x)[i]; }
        float Vector::operator[](int i) const { return (&x)[i]; }
    }
}
