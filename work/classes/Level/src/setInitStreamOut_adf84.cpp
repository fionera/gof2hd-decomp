#include "class.h"

__attribute__((visibility("hidden"))) extern unsigned char *g_initStreamOut;

void Level::setInitStreamOut() {
    *g_initStreamOut = 1;
}
