#include "class.h"

bool HackingGame::isRotating()
{
    if (B(this, 0x128) != 0)
        return true;
    return B(this, 0x129) != 0;
}
