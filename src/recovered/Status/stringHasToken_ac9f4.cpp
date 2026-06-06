#include "class.h"
extern "C" int AEString_IndexOf(String *haystack, String *needle);
bool Status::stringHasToken(String haystack, String needle) {
    return AEString_IndexOf(&haystack, &needle) > -1;
}
