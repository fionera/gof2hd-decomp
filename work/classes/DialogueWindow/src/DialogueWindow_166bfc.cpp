#include "class.h"

extern "C" void String_ctor(String *self);
extern "C" int DialogueWindow_init(DialogueWindow *self);
extern "C" void DialogueWindow_set(DialogueWindow *self, Mission *mission, int kind, int campaign);

extern "C" DialogueWindow *DialogueWindow_ctor_mission(DialogueWindow *self, Mission *mission, Level *level, int kind)
{
    String_ctor((String *)((char *)self + 0x28));
    String_ctor((String *)((char *)self + 0x34));
    DialogueWindow_init(self);
    F<Level *>(self, 0x58) = level;
    DialogueWindow_set(self, mission, kind, -1);
    return self;
}
