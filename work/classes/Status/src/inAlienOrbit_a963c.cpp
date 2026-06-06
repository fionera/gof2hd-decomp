#include "class.h"
extern "C" bool Status_inAlienOrbit_impl(Station *station, Station *playerStation);
bool Status::inAlienOrbit() {
    return Status_inAlienOrbit_impl(station, playerStation);
}
