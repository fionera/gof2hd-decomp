#include "class.h"

__attribute__((visibility("hidden"))) extern int g_dw_successDialogueIds[];

extern "C" bool DialogueWindow_hasSuccessDialogue(int id)
{
    if (id > 0xa1) return false;
    return g_dw_successDialogueIds[id] > 0;
}
