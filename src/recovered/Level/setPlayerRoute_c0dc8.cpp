#include "class.h"

struct Route {
    ~Route();
};

void operator delete(void *ptr);

void Level::setPlayerRoute(Route *route) {
    Route *old = (Route *)playerRoute;
    if (old != 0) {
        old->~Route();
        operator delete(old);
    }
    playerRoute = (int)route;
}
