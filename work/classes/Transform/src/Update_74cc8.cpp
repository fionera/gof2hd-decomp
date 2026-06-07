#include "class.h"

namespace AbyssEngine {

void Transform::Update(longlong time, bool updateBounds) {
    InternUpdate(time, updateBounds);
}

} // namespace AbyssEngine
