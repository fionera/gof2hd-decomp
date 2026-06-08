#include "class.h"

namespace AbyssEngine {

__attribute__((minsize)) String ConfigReader::GetNewLine()
{
    volatile uint32_t stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    String line;
    char c = 0;

    while (*(uint32_t *)((char *)&line + 0x8) == 0) {
        uint32_t read;
        while (true) {
            read = AEFile::Read(c, config_file_handle(this));
            uint32_t newline = __builtin_clz((uint32_t)c - 10) >> 5;
            uint32_t done = (read ^ 1) | newline;
            if (done != 0) {
                break;
            }
            if (c != '\r') {
                line += c;
            }
        }

        line.Trim();
        int32_t commentIndex;
        {
            String comment("//", false);
            commentIndex = line.IndexOf(comment);
        }

        if (commentIndex != -1) {
            {
                String stripped = line.SubString(0, commentIndex);
                line = stripped;
            }
            line.Trim();
        }

        if (*(uint32_t *)((char *)&line + 0x8) == 0 && read == 0) {
            line.Set("EOF");
        }
    }
    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return line;
}

} // namespace AbyssEngine
