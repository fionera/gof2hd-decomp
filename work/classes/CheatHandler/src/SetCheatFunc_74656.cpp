#include "class.h"

namespace AbyssEngine {

void CheatHandler::SetCheatFunc(CheatFunc f, void *ctx)
{
    func = f;
    context = ctx;
}

} // namespace AbyssEngine
