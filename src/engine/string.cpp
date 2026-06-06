// AbyssEngine::String — engine string type.
// Layout (== AEString, validated, 8 bytes): { wchar_t* text; unsigned int size; }
// (text is a u16/wchar pointer; size is the char count.)
//
// Recovered method:
//   GetStringLength(const char*) @0x83634 — a plain strlen over the C string.
namespace AbyssEngine {

struct String {
    unsigned short* text;   // engine stores UTF-16-ish text
    unsigned int size;
};

// GetStringLength(char const*) — counts bytes to the NUL terminator (strlen).
unsigned int String_GetStringLength(const char* s) {
    const char* p = s;
    while (*p) ++p;
    return (unsigned int)(p - s);
}

} // namespace AbyssEngine
