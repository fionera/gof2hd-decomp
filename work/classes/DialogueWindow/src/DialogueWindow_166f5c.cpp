#include "class.h"

extern "C" void String_ctor(String *self);
extern "C" int DialogueWindow_init(DialogueWindow *self);

extern "C" DialogueWindow *DialogueWindow_ctor_default(DialogueWindow *self)
{
    String_ctor((String *)((char *)self + 0x28));
    String_ctor((String *)((char *)self + 0x34));
    DialogueWindow_init(self);
    return self;
}
