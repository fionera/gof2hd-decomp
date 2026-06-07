#include "class.h"

namespace AbyssEngine {

CheatHandler::CheatHandler(KeyCode *k)
{
    func = 0;
    context = 0;
    codes = new Array<CheatCode *>();
    keys = k;
}

} // namespace AbyssEngine
