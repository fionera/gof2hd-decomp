#include "class.h"
void Status::setFreelanceMission(Mission *m) {
    ((Mission **)missions->data)[1] = m;
}
