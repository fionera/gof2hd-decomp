#include "class.h"

namespace AbyssEngine {

__attribute__((minsize)) void ConfigReader::RegisterTokenReadFunction(
    String name, ConfigTokenReadFunction read, void *context)
{
    TokenStruct *token = new TokenStruct();
    token->name = name;
    token->read = read;
    token->context = context;
    ArrayAdd(token, *(Array<TokenStruct *> *)this);
}

} // namespace AbyssEngine
