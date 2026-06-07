// Globals::getKeyBindingReplaceString(int) — returns a String by value.
// Default-construct a stack String, upper-case it (returns a String& copy-constructed into the
// sret slot), then the stack String is destroyed. The on-stack String is guarded by an
// explicit stack canary, mirroring the matched ConfigReader::GetNewLine / AEFile idiom.

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

namespace AbyssEngine {
struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;
    String();
    ~String();
    String &ToUpperCase();
    String(const String &other, bool copy);
};
}
using AbyssEngine::String;

struct Globals;

__attribute__((minsize)) String Globals_getKeyBindingReplaceString(Globals *, int key)
{
    volatile uint32_t stackGuard = (uint32_t)__stack_chk_guard;
    (void)key;

    String tmp;
    String result(tmp.ToUpperCase(), false);

    uint32_t diff = (uint32_t)__stack_chk_guard - stackGuard;
    if (diff == 0) {
        return result;
    }
    __stack_chk_fail(diff);
}
