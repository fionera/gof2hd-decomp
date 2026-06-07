#include "class.h"

void LevelScript::resetStartSequenceOver()
{
    char *self = B(this, 0);
    *(volatile int *)(self + 0x24) = 0;
    *(volatile uint8_t *)(self + 0x21) = 0;
}
