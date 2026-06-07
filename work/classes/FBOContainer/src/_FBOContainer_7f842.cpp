#include "class.h"

namespace AbyssEngine {

FBOContainer::~FBOContainer() {
    Release();
    ((String *)((char *)this + 0x1c))->~String();
}

} // namespace AbyssEngine
