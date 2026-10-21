#include "engine/file/ConfigReader.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/KeyCode.h"
#include "engine/render/Engine.h"

namespace AbyssEngine {
    namespace {
        // TIMER: one line, the engine timer interval.
        void ConfigReadTimer(ConfigReader *reader, void *context) { // lint: void_ptr (ConfigTokenReadFunction ctx)
            String line = reader->GetNewLine();
            if (line.Compare("EOF") != 0) {
                Engine *engine = static_cast<Engine *>(context);
                engine->timerInterval = line.ValueOf();
            }
        }

        // KEYMAP: up to 64 "code,name" lines; only a complete table replaces the key mapping.
        void ConfigReadKeyMap(ConfigReader *reader, void *context) { // lint: void_ptr (ConfigTokenReadFunction ctx)
            Array<KeyCode *> keyCodes;
            String line;
            for (unsigned int i = 0; i < 64 && line.Compare("EOF") != 0; i++) {
                String newLine = reader->GetNewLine();
                Array<String *> *parts = static_cast<Array<String *> *>(newLine.Split(String(",", false)));
                if (parts == nullptr || parts->count != 2) {
                    delete[] newLine.GetAEChar();
                    break;
                }
                KeyCode *keyCode = new KeyCode();
                keyCode->code = (*parts)[0]->ValueOf();
                keyCode->name = *(*parts)[1];
                ArrayAdd(keyCode, keyCodes);
                for (unsigned int j = 0; j < parts->count; j++) {
                    if ((*parts)[j] != nullptr)
                        delete (*parts)[j];
                    (*parts)[j] = nullptr;
                }
                ArrayRemoveAll(*parts);
                delete parts;
            }
            if (keyCodes.count == 64) {
                Engine *engine = static_cast<Engine *>(context);
                engine->appManager->KeyCodeSetMapping(&keyCodes);
            }
            for (unsigned int i = 0; i < keyCodes.count; i++) {
                if (keyCodes[i] != nullptr)
                    delete keyCodes[i];
                keyCodes[i] = nullptr;
            }
            ArrayRemoveAll(keyCodes);
        }

        // VIBRATION: "ON" enables device vibration.
        void ConfigReadVibration(ConfigReader *reader, void *context) { // lint: void_ptr (ConfigTokenReadFunction ctx)
            String line = reader->GetNewLine();
            if (line.Compare("EOF") != 0) {
                Engine *engine = static_cast<Engine *>(context);
                if (line.Compare("ON") == 0)
                    engine->vibrationSupported = true;
            }
        }
    }

    ConfigReader::ConfigReader(Engine *engine) {
        this->engine = engine;
        RegisterTokenReadFunction(String("TIMER", false), ConfigReadTimer, engine);
        RegisterTokenReadFunction(String("KEYMAP", false), ConfigReadKeyMap, engine);
        RegisterTokenReadFunction(String("VIBRATION", false), ConfigReadVibration, engine);
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
        String name, ConfigTokenReadFunction read, void *context) { // lint: void_ptr (param baked into RegisterTokenReadFunction mangled symbol)
        // lint: void_ptr (param baked into RegisterTokenReadFunction mangled symbol)
        TokenStruct *token = new TokenStruct();
        token->name = name;
        token->read = read;
        token->context = context;
        ArrayAdd(token, tokens);
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
                    {
                        int _nl = line.length + 1;
                        unsigned short *_nd = new unsigned short[_nl + 1];
                        for (int _i = 0; _i < line.length; _i++) _nd[_i] = line.data[_i];
                        _nd[line.length] = (unsigned short) (static_cast<char16_t>(static_cast<unsigned char>(c)));
                        _nd[_nl] = 0;
                        if (line.data) delete[] line.data;
                        line.data = _nd;
                        line.length = _nl;
                    }
                }
            }

            {
                int b = 0, e = line.length;
                while (b < line.length && line.data[b] == static_cast<unsigned short>(' ')) b++;
                while (e > b && line.data[e - 1] == static_cast<unsigned short>(' ')) e--;
                if (b >= e) {
                    {
                        if (line.data) delete[] line.data;
                        line.data = nullptr;
                        line.length = 0;
                    }
                } else {
                    line = line.SubString(static_cast<unsigned int>(b), static_cast<unsigned int>(e));
                }
            }

            String commentNeedle;
            for (const char *p = "//"; p && *p; ++p) {
                int _nl = commentNeedle.length + 1;
                unsigned short *_nd = new unsigned short[_nl + 1];
                for (int _i = 0; _i < commentNeedle.length; _i++) _nd[_i] = commentNeedle.data[_i];
                _nd[commentNeedle.length] = (unsigned short) (static_cast<char16_t>(static_cast<unsigned char>(*p)));
                _nd[_nl] = 0;
                if (commentNeedle.data) delete[] commentNeedle.data;
                commentNeedle.data = _nd;
                commentNeedle.length = _nl;
            }
            unsigned int commentPos = line.IndexOf(commentNeedle);
            int32_t commentIndex = commentPos == 0xffffffffu ? -1 : static_cast<int32_t>(commentPos);
            if (commentIndex != -1) {
                line = line.SubString(0, 0 + static_cast<uint32_t>(commentIndex));
                {
                    int b = 0, e = line.length;
                    while (b < line.length && line.data[b] == static_cast<unsigned short>(' ')) b++;
                    while (e > b && line.data[e - 1] == static_cast<unsigned short>(' ')) e--;
                    if (b >= e) {
                        {
                            if (line.data) delete[] line.data;
                            line.data = nullptr;
                            line.length = 0;
                        }
                    } else {
                        line = line.SubString(static_cast<unsigned int>(b), static_cast<unsigned int>(e));
                    }
                }
            }

            if (line.size() == 0 && read == 0) {
                line.Set("EOF");
            }
        }

        return line;
    }

    void ConfigReader::ParseFile(String name) {
        if (AEFile::OpenRead(name, &this->file_handle) != 0) {
            String line = GetNewLine();
            while ((line.Compare("EOF") == 0 ? 0 : 1) != 0) {
                uint16_t *first = reinterpret_cast<uint16_t *>(&line.data[0]);
                if (*first == '[') {
                    uint16_t *last = reinterpret_cast<uint16_t *>(&line.data[line.size() - 1]);
                    if (*last == ']') {
                        for (uint32_t i = 0; i < tokens.size(); i++) {
                            TokenStruct *token = tokens[i];
                            String section = line.SubString(1, 1 + (line.size() - 1));
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
}
