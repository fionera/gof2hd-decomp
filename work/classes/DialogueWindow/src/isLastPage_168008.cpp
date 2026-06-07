#include "class.h"

extern "C" int DialogueWindow_length(DialogueWindow *self);

extern "C" bool DialogueWindow_isLastPage(DialogueWindow *self)
{
    int page = F<int>(self, 0x48);
    int len = DialogueWindow_length(self);
    return page == len - 1;
}
