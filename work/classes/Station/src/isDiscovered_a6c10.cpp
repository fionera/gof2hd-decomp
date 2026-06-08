#include "class.h"

struct Galaxy;
extern "C" char *Galaxy_getVisited(Galaxy *g);

// Global slot -> P; *P -> the Galaxy* singleton.
extern Galaxy **const gGalaxySingleton __attribute__((visibility("hidden")));

extern "C" uint8_t Station_isDiscovered(Station *self)
{
    char *visited = Galaxy_getVisited(*gGalaxySingleton);
    return visited[self->f_c];
}
