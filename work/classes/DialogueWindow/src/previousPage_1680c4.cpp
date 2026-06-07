#include "class.h"

extern "C" void DialogueWindow_loadContent(DialogueWindow *self);

extern "C" int DialogueWindow_previousPage(DialogueWindow *self)
{
    int page = F<int>(self, 0x48);
    if (page <= 0) return 0;
    F<int>(self, 0x48) = page - 1;
    DialogueWindow_loadContent(self);
    return 1;
}
