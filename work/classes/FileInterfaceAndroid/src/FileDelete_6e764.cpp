#include "class.h"

// AbyssEngine::String::String(String* out, const String* src, bool) (0x6f028) and ~String (0x6ee30).
extern "C" void String_copy_ctor(void *out, const void *src, bool);
extern "C" void String_dtor(void *self);

// FileInterfaceAndroid::FileDelete(AbyssEngine::String) — static. The String arrives by value
// (homed to the stack); a local copy is built and immediately destroyed. The on-stack String
// buffer trips the stack-protector canary.
// NOTE: near-match. The copy/dtor sequence and canary are correct, but the target homes the
// by-value String through the prologue push {r0-r3} and overlaps the temp at sp+0, giving a
// 16-byte frame; clang -Oz lays the temp out at a higher slot with a 32-byte frame. The
// by-value-aggregate homing/frame layout is not reachable from source form.
extern "C" void FileInterfaceAndroid_FileDelete(String12 s)
{
    char tmp[12];
    String_copy_ctor(tmp, &s, false);
    String_dtor(tmp);
}
