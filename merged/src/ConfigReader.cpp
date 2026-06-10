#include "ConfigReader.h"




// ---- _ConfigReader_87672.cpp ----
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

// ---- ConfigReader_87258.cpp ----
static void config_reader_read_file_callback(void *, void *) {}
static void config_reader_keys_for_action_callback(void *, void *) {}
static void config_reader_register_token_callback(void *, void *) {}

namespace AbyssEngine {

__attribute__((minsize)) ConfigReader::ConfigReader(Engine *engine)
{
    volatile uint32_t stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    new ((Array<TokenStruct *> *)this) Array<TokenStruct *>();
    config_engine(this) = engine;

    RegisterTokenReadFunction(
        String("ConfigReadFile", false),
        (ConfigTokenReadFunction)config_reader_read_file_callback, engine);
    RegisterTokenReadFunction(
        String("ConfigGetKeysForAction", false),
        (ConfigTokenReadFunction)config_reader_keys_for_action_callback, engine);
    RegisterTokenReadFunction(
        String("ConfigRegisterTokenReadFunction", false),
        (ConfigTokenReadFunction)config_reader_register_token_callback, engine);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
}

} // namespace AbyssEngine

// ---- RegisterTokenReadFunction_8734c.cpp ----
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

// ---- GetNewLine_877d4.cpp ----
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

// ---- ParseFile_876bc.cpp ----
namespace AbyssEngine {

__attribute__((minsize)) void ConfigReader::ParseFile(String name)
{
    volatile uint32_t stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
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
    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
}

} // namespace AbyssEngine
