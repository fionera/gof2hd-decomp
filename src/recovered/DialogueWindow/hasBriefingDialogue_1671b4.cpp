#include "class.h"

__attribute__((visibility("hidden"))) extern int g_dw_briefingDialogueIds[];

extern "C" bool DialogueWindow_hasBriefingDialogue(int id)
{
    if (id > 0xa1) return false;
    return g_dw_briefingDialogueIds[id] > 0;
}
