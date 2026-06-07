#include "class.h"

namespace AbyssEngine {

void CheatHandler::Update(uint16_t key)
{
    if (func != 0) {
        for (uint32_t i = 0; i < codes->size; i = i + 1) {
            if (codes->data[i]->Update(key)) {
                func(codes->data[i]->value, context);
            }
        }
    }
}

} // namespace AbyssEngine
