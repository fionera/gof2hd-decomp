#include "class.h"

Radar::~Radar()
{
    Array<KIPlayer *> *players = F<Array<KIPlayer *> *>(this, 0x34);
    if (players != 0) {
        players->~Array<KIPlayer *>();
        operator delete(players);
    }
    F<Array<KIPlayer *> *>(this, 0x34) = 0;
    ((AbyssEngine::String *)((char *)this + 0x18c))->~String();
}
