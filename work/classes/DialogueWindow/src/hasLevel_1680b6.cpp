#include "class.h"

extern "C" bool DialogueWindow_hasLevel(DialogueWindow *self)
{
    return F<void *>(self, 0x58) != 0;
}
