#ifndef GOF2_CONFIGREADER_H
#define GOF2_CONFIGREADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/file/AEFile.h"

// Engine is the renderer/device root; its real type lives in namespace AbyssEngine.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class ConfigReader;

// --- thin String helpers -------------------------------------------------------------
// The shared common.h String stores UTF-16 in std::u16string; these small ASCII-oriented
// helpers are local to the config parser and have no separate home of their own. The
// original binary inlines every one of them into its call sites and never emits a
// standalone definition, so force-inline them here ('always_inline') to keep the
// generated code symbol-for-symbol faithful — no outlined helper ever leaks out.
#define GOF2_CFG_INLINE __attribute__((always_inline)) inline

GOF2_CFG_INLINE String StringFromAscii(const char *cstr) {
    String r;
    for (const char *p = cstr; p && *p; ++p) r.s.push_back(static_cast<char16_t>(static_cast<unsigned char>(*p)));
    return r;
}
GOF2_CFG_INLINE void StringAppendChar(String &str, char c) {
    str.s.push_back(static_cast<char16_t>(static_cast<unsigned char>(c)));
}
GOF2_CFG_INLINE void StringTrim(String &str) {
    size_t b = str.s.find_first_not_of(static_cast<char16_t>(' '));
    size_t e = str.s.find_last_not_of(static_cast<char16_t>(' '));
    if (b == std::u16string::npos) { str.s.clear(); return; }
    str.s = str.s.substr(b, e - b + 1);
}
GOF2_CFG_INLINE int32_t StringIndexOf(const String &str, const String &needle) {
    size_t p = str.s.find(needle.s);
    return p == std::u16string::npos ? -1 : static_cast<int32_t>(p);
}
GOF2_CFG_INLINE String StringSubString(const String &str, uint32_t start, uint32_t len) {
    String r; r.s = str.s.substr(start, len); return r;
}
GOF2_CFG_INLINE int32_t StringCompareAscii(const String &str, const char *cstr) {
    return str.s == StringFromAscii(cstr).s ? 0 : 1;
}
GOF2_CFG_INLINE void StringSetAscii(String &str, const char *cstr) { str.s = StringFromAscii(cstr).s; }
GOF2_CFG_INLINE uint16_t *StringCharAt(String &str, uint32_t idx) {
    return reinterpret_cast<uint16_t *>(&str.s[idx]);
}

#undef GOF2_CFG_INLINE

typedef void (*ConfigTokenReadFunction)(ConfigReader *, void *);

// A named config section bound to its handler callback and that handler's context.
struct TokenStruct {
    String name;
    ConfigTokenReadFunction read;
    void *context;
};

// Parses simple "[section]"-style config files, dispatching each section to a
// registered token-read callback.
class ConfigReader {
public:
    Array<TokenStruct *> tokens;
    Engine *engine;
    uint32_t file_handle;

    ConfigReader(Engine *engine);
    ~ConfigReader();

    void RegisterTokenReadFunction(String name, ConfigTokenReadFunction read, void *context);
    String GetNewLine();
    void ParseFile(String name);
};

} // namespace AbyssEngine
#endif
