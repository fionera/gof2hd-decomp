#include "class.h"

extern "C" void *operator_new(unsigned int size);
extern "C" String *String_ctor_string(String *self, String *text, bool copy);

extern "C" void _ZN9Objective15setAchievedTextEPN11AbyssEngine6StringE(Objective *self,
                                                                        String *text)
{
    String *copy = (String *)operator_new(sizeof(String));
    String_ctor_string(copy, text, false);
    F<String *>(self, 0x14) = copy;
}
