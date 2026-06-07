#include "class.h"

namespace AbyssEngine {

CheatCode::~CheatCode()
{
    if (keys) {
        delete keys;
    }
    keys = 0;
}

} // namespace AbyssEngine
