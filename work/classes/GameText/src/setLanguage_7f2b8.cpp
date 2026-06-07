#include "class.h"

// Tail veneer to GameText::setLanguage(short, int).
extern "C" void GameText_setLanguage_si(GameText *self, int lang, int param);

// GameText::setLanguage(int) -> forwards to setLanguage(0, param_1).
extern "C" void GameText_setLanguage_i(GameText *self, int param_1)
{
    return GameText_setLanguage_si(self, 0, param_1);
}
