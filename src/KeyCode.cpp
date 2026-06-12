#include "gof2/KeyCode.h"


// ---- operator__81ffa.cpp ----
// AbyssEngine::KeyCode::operator=(KeyCode const&)
// Copies the scalar key code, then delegates to String::operator= for the embedded name.

namespace AbyssEngine {

KeyCode &KeyCode::operator=(const KeyCode &other)
{
    this->code = other.code;
    this->name = other.name;
    return *this;
}

} // namespace AbyssEngine
