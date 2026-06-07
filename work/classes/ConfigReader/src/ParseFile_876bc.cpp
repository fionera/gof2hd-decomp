#include "class.h"

namespace AbyssEngine {

__attribute__((minsize)) void ConfigReader::ParseFile(String name)
{
    volatile uint32_t stackGuard = (uint32_t)__stack_chk_guard;
    if (AEFile::OpenRead(name, &config_file_handle(this)) != 0) {
        String line = GetNewLine();
        while (line.Compare("EOF") != 0) {
            uint16_t *first = line[0];
            if (*first == '[') {
                uint16_t *last = line[*(uint32_t *)((char *)&line + 0x8) - 1];
                if (*last == ']') {
                    for (uint32_t i = 0; i < config_count(this); i++) {
                        TokenStruct *token = *(TokenStruct **)((char *)config_tokens(this) + i * 4);
                        int32_t cmp;
                        {
                            String section =
                                line.SubString(1, *(uint32_t *)((char *)&line + 0x8) - 1);
                            cmp = token->name.Compare(section);
                        }
                        if (cmp == 0) {
                            token->read(this, token->context);
                            break;
                        }
                    }
                }
            }
            {
                String next = GetNewLine();
                line = next;
            }
        }
        AEFile::Close(config_file_handle(this));
    }
    uint32_t stackDifference = (uint32_t)__stack_chk_guard - stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
}

} // namespace AbyssEngine
