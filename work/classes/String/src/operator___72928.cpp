#include "class.h"

// Appends via the long long overload after sign-extending the int.
extern "C" String *String_addAssign_longlong(String *self, const long long *v);

// AbyssEngine::String::operator+=(int const&) - sign-extend to 64-bit and append.
extern "C" String *String_addAssign_int(String *self, const int *v)
{
    long long ext = (long long)*v;
    return String_addAssign_longlong(self, &ext);
}
