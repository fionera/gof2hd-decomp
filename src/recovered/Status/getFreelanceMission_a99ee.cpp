#include "class.h"
Mission *Status::getFreelanceMission() {
    return ((Mission **)missions->data)[1];
}
