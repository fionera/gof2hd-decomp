#include "gof2/KeyCode.h"


extern "C" String *_ZN11AbyssEngine6StringaSERKS0_(String *self, const String *other);

// ---- operator__81ffa.cpp ----
// AbyssEngine::KeyCode::operator=(KeyCode const&)
// Copies the scalar key code, then delegates to String::operator= for the embedded name.

namespace AbyssEngine {

// String::operator=(String const&)

KeyCode &KeyCode::operator=(const KeyCode &other)
{
    this->code = other.code;
    _ZN11AbyssEngine6StringaSERKS0_(&this->name, &other.name);
    return *this;
}

} // namespace AbyssEngine
