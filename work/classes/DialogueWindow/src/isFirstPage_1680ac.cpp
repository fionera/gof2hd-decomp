#include "class.h"

extern "C" bool DialogueWindow_isFirstPage(DialogueWindow *self)
{
    return F<void *>(self, 0x48) == 0;
}
