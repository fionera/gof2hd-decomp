#include "class.h"

// 4-arg Globals::drawLines(uint, Array<String*>*, int, int) forwards to the 6-value
// drawLines variant with the first arg forced to 0, the incoming 5th stack value as the
// "startY", and a trailing 0.
extern "C" void Globals_drawLines5(unsigned p1, void *font, Array<int> *lines, int baseX,
                                   int startY, int centered);

extern "C" void Globals_drawLines4(unsigned p1, void *font, Array<int> *lines, int baseX,
                                   int startY)
{
    (void)p1;
    Globals_drawLines5(0, font, lines, baseX, startY, 0);
}
