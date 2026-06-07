#include "class.h"

extern "C" int DialogueWindow_length(DialogueWindow *self);
extern "C" void DialogueWindow_loadContent(DialogueWindow *self);

extern "C" int DialogueWindow_nextPage(DialogueWindow *self)
{
    int page = F<int>(self, 0x48);
    int len = DialogueWindow_length(self);
    if (page < len - 1) {
        F<int>(self, 0x48) = F<int>(self, 0x48) + 1;
        DialogueWindow_loadContent(self);
        return 1;
    }
    return 0;
}
