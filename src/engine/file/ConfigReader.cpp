#include "gof2/engine/file/ConfigReader.h"

namespace AbyssEngine {

__attribute__((minsize)) ConfigReader::~ConfigReader()
{
    for (uint32_t i = 0; i < tokens.size(); i++) {
        TokenStruct *token = tokens[i];
        if (token != 0) {
            delete token;
        }
        tokens[i] = 0;
    }
}

} // namespace AbyssEngine

static void config_reader_read_file_callback(void *, void *) {}
static void config_reader_keys_for_action_callback(void *, void *) {}
static void config_reader_register_token_callback(void *, void *) {}

namespace AbyssEngine {

__attribute__((minsize)) ConfigReader::ConfigReader(Engine *engine)
{

    this->engine = engine;

    RegisterTokenReadFunction(
        StringFromAscii("ConfigReadFile"),
        (ConfigTokenReadFunction)config_reader_read_file_callback, engine);
    RegisterTokenReadFunction(
        StringFromAscii("ConfigGetKeysForAction"),
        (ConfigTokenReadFunction)config_reader_keys_for_action_callback, engine);
    RegisterTokenReadFunction(
        StringFromAscii("ConfigRegisterTokenReadFunction"),
        (ConfigTokenReadFunction)config_reader_register_token_callback, engine);

    
}

} // namespace AbyssEngine

namespace AbyssEngine {

__attribute__((minsize)) void ConfigReader::RegisterTokenReadFunction(
    String name, ConfigTokenReadFunction read, void *context)
{
    TokenStruct *token = new TokenStruct();
    token->name = name;
    token->read = read;
    token->context = context;
    tokens.push_back(token);
}

} // namespace AbyssEngine

namespace AbyssEngine {

__attribute__((minsize)) String ConfigReader::GetNewLine()
{
    String line;
    char c = 0;

    while (line.size() == 0) {
        uint32_t read;
        while (true) {
            read = AEFile::Read(c, this->file_handle);
            uint32_t newline = __builtin_clz((uint32_t)c - 10) >> 5;
            uint32_t done = (read ^ 1) | newline;
            if (done != 0) {
                break;
            }
            if (c != '\r') {
                StringAppendChar(line, c);
            }
        }

        StringTrim(line);
        int32_t commentIndex;
        {
            String comment = StringFromAscii("//");
            commentIndex = StringIndexOf(line, comment);
        }

        if (commentIndex != -1) {
            {
                String stripped = StringSubString(line, 0, commentIndex);
                line = stripped;
            }
            StringTrim(line);
        }

        if (line.size() == 0 && read == 0) {
            StringSetAscii(line, "EOF");
        }
    }
    
    return line;
}

} // namespace AbyssEngine

namespace AbyssEngine {

__attribute__((minsize)) void ConfigReader::ParseFile(String name)
{
    if (AEFile::OpenRead(name, &this->file_handle) != 0) {
        String line = GetNewLine();
        while (StringCompareAscii(line, "EOF") != 0) {
            uint16_t *first = StringCharAt(line, 0);
            if (*first == '[') {
                uint16_t *last = StringCharAt(line, line.size() - 1);
                if (*last == ']') {
                    for (uint32_t i = 0; i < tokens.size(); i++) {
                        TokenStruct *token = tokens[i];
                        int32_t cmp;
                        {
                            String section =
                                StringSubString(line, 1, line.size() - 1);
                            cmp = (token->name.s == section.s) ? 0 : 1;
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
        AEFile::Close(this->file_handle);
    }
    
}

} // namespace AbyssEngine
