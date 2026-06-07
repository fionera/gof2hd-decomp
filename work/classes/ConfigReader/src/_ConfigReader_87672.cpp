#include "class.h"

namespace AbyssEngine {

__attribute__((minsize)) ConfigReader::~ConfigReader()
{
    uint32_t i = 0;
    int32_t byteOffset = 0;
    for (; i < config_count(this); byteOffset += 4, i++) {
        TokenStruct *token = *(TokenStruct **)((char *)config_tokens(this) + byteOffset);
        TokenStruct **slot;
        if (token == 0) {
            slot = (TokenStruct **)((char *)config_tokens(this) + i * 4);
        } else {
            delete token;
            slot = (TokenStruct **)((char *)config_tokens(this) + byteOffset);
        }
        *slot = 0;
    }
    ((Array<TokenStruct *> *)this)->~Array();
}

} // namespace AbyssEngine
