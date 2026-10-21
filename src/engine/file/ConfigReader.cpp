#include "engine/file/ConfigReader.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/KeyCode.h"
#include "engine/render/Engine.h"

namespace AbyssEngine {
    namespace {
        void ReadTimer(ConfigReader *reader, Engine *engine);

        void ReadKeyMap(ConfigReader *reader, Engine *engine);

        void ReadVibration(ConfigReader *reader, Engine *engine);
    }

    ConfigReader::ConfigReader(Engine *engine) {
        this->engine = engine;
        RegisterTokenReadFunction(String("TIMER"), (ConfigTokenReadFunction) ReadTimer, engine);
        RegisterTokenReadFunction(String("KEYMAP"), (ConfigTokenReadFunction) ReadKeyMap, engine);
        RegisterTokenReadFunction(String("VIBRATION"), (ConfigTokenReadFunction) ReadVibration, engine);
    }

    void ConfigReader::RegisterTokenReadFunction(
        String name, ConfigTokenReadFunction read, void *context) { // lint: void_ptr (param baked into RegisterTokenReadFunction mangled symbol)
        TokenStruct *token = new TokenStruct();
        token->name = name;
        token->read = read;
        token->context = context;
        ArrayAdd(token, tokens);
    }

    namespace {
        // [TIMER]: one line with the timer value.
        void ReadTimer(ConfigReader *reader, Engine *engine) {
            String line = reader->GetNewLine();
            if (line.Compare("EOF") != 0)
                engine->timerInterval = line.ValueOf();
        }

        // [KEYMAP]: up to 64 "<code>,<name>" lines; the mapping is only applied when all 64 are present.
        void ReadKeyMap(ConfigReader *reader, Engine *engine) {
            Array<KeyCode *> keys;
            String last; // never assigned: the EOF check below never triggers (as in the original)
            for (unsigned int i = 0; i < 64 && last.Compare("EOF") != 0; i++) {
                String line = reader->GetNewLine();
                Array<String *> *parts = (Array<String *> *) line.Split(String(","));
                if (parts == nullptr || parts->size() != 2) {
                    delete[] line.GetAEChar();
                    break;
                }
                KeyCode *key = new KeyCode();
                key->code = (*parts)[0]->ValueOf();
                key->name = *(*parts)[1];
                ArrayAdd(key, keys);
                for (unsigned int j = 0; j < parts->size(); j++) {
                    if ((*parts)[j] != nullptr)
                        delete (*parts)[j];
                    (*parts)[j] = nullptr;
                }
                ArrayRemoveAll(*parts);
                delete parts;
            }
            if (keys.size() == 64)
                engine->appManager->KeyCodeSetMapping(&keys);
            for (unsigned int i = 0; i < keys.size(); i++) {
                if (keys[i] != nullptr)
                    delete keys[i];
                keys[i] = nullptr;
            }
            ArrayRemoveAll(keys);
        }

        // [VIBRATION]: "ON" enables vibration support.
        void ReadVibration(ConfigReader *reader, Engine *engine) {
            String line = reader->GetNewLine();
            if (line.Compare("EOF") != 0 && line.Compare("ON") == 0)
                engine->vibrationSupported = true;
        }
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

    void ConfigReader::ParseFile(String name) {
        if (AEFile::OpenRead(name, &this->file_handle) == 0)
            return;
        String line = GetNewLine();
        while (line.Compare("EOF") != 0) {
            // "[NAME]" starts a section handled by the registered token reader.
            if (*line[0] == '[' && *line[line.length - 1] == ']') {
                for (uint32_t i = 0; i < tokens.size(); i++) {
                    if (tokens[i]->name.Compare(line.SubString(1, (unsigned short) line.length - 1)) == 0) {
                        tokens[i]->read(this, tokens[i]->context);
                        break;
                    }
                }
            }
            line = GetNewLine();
        }
        AEFile::Close(this->file_handle);
    }

    // Next non-empty line without trailing "//" comment; "EOF" at end of file.
    String ConfigReader::GetNewLine() {
        String line;
        char c = 0;
        while (line.length == 0) {
            bool read;
            while ((read = AEFile::Read(c, this->file_handle)) && c != '\n') {
                if (c != '\r')
                    line += c;
            }
            line.Trim();
            unsigned int comment = line.IndexOf(String("//"));
            if (comment != 0xffffffffu) {
                line = line.SubString(0, comment);
                line.Trim();
            }
            if (line.length == 0 && !read)
                line.Set("EOF");
        }
        return line;
    }
}
