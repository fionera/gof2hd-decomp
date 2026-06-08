#include "class.h"

// AbyssEngine::KeyCode::operator=(KeyCode const&)
// Copies the scalar key code, then delegates to String::operator= for the embedded name.

namespace AbyssEngine {

// String::operator=(String const&)
extern "C" String *_ZN11AbyssEngine6StringaSERKS0_(String *self, const String *other);

KeyCode &KeyCode::operator=(const KeyCode &other)
{
    this->code = other.code;
    _ZN11AbyssEngine6StringaSERKS0_(&this->name, &other.name);
    return *this;
}

} // namespace AbyssEngine
