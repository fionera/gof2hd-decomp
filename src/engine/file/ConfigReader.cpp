#include "engine/file/ConfigReader.h"

namespace AbyssEngine {
    namespace {
        void config_reader_read_file_callback(void *, void *) {
        }

        void config_reader_keys_for_action_callback(void *, void *) {
        }

        void config_reader_register_token_callback(void *, void *) {
        }
    } // namespace

    ConfigReader::ConfigReader(Engine *engine) {
        this->engine = engine;

        RegisterTokenReadFunction(
            StringFromAscii("ConfigReadFile"),
            (ConfigTokenReadFunction) config_reader_read_file_callback, engine);
        RegisterTokenReadFunction(
            StringFromAscii("ConfigGetKeysForAction"),
            (ConfigTokenReadFunction) config_reader_keys_for_action_callback, engine);
        RegisterTokenReadFunction(
            StringFromAscii("ConfigRegisterTokenReadFunction"),
            (ConfigTokenReadFunction) config_reader_register_token_callback, engine);
    }

    ConfigReader::~ConfigReader() {
        for (uint32_t i = 0; i < tokens.size(); i++) {
            TokenStruct *token = tokens[i];
            if (token != nullptr) {
                delete token;
            }
            tokens[i] = nullptr;
        }
    }

    void ConfigReader::RegisterTokenReadFunction(
        String name, ConfigTokenReadFunction read, void *context) {
        TokenStruct *token = new TokenStruct();
        token->name = name;
        token->read = read;
        token->context = context;
        tokens.push_back(token);
    }

    String ConfigReader::GetNewLine() {
        String line;
        char c = 0;

        while (line.size() == 0) {
            uint32_t read;
            while (true) {
                read = AEFile::Read(c, this->file_handle);
                bool newline = (c == '\n');
                if (read == 0 || newline) {
                    break;
                }
                if (c != '\r') {
                    StringAppendChar(line, c);
                }
            }

            StringTrim(line);

            int32_t commentIndex = StringIndexOf(line, StringFromAscii("//"));
            if (commentIndex != -1) {
                line = StringSubString(line, 0, commentIndex);
                StringTrim(line);
            }

            if (line.size() == 0 && read == 0) {
                StringSetAscii(line, "EOF");
            }
        }

        return line;
    }

    void ConfigReader::ParseFile(String name) {
        if (AEFile::OpenRead(name, &this->file_handle) != 0) {
            String line = GetNewLine();
            while (StringCompareAscii(line, "EOF") != 0) {
                uint16_t *first = StringCharAt(line, 0);
                if (*first == '[') {
                    uint16_t *last = StringCharAt(line, line.size() - 1);
                    if (*last == ']') {
                        for (uint32_t i = 0; i < tokens.size(); i++) {
                            TokenStruct *token = tokens[i];
                            String section = StringSubString(line, 1, line.size() - 1);
                            if (token->name.Compare(section) == 0) {
                                token->read(this, token->context);
                                break;
                            }
                        }
                    }
                }
                line = GetNewLine();
            }
            AEFile::Close(this->file_handle);
        }
    }
} // namespace AbyssEngine
