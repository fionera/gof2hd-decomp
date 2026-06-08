#include "class.h"

int Radar::getPlanetDockIndex()
{
    SolarSystem *system = gStatus->getSystem();
    Array<Station *> *stations = system->getStations();
    return (int)stations->data[this->f_40];
}
