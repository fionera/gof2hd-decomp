#include "class.h"

// Counts the non-empty slots in the mission array.
int Status::getNumberOfMissions() {
    Array<Mission *> *m = missions;
    if (m == 0) {
        return 0;
    }
    int count = 0;
    int i = 0;
    while ((int)m->size != i) {
        Mission *cur = ((Mission **)m->data)[i];
        i = i + 1;
        if (cur != 0) {
            count = count + 1;
        }
    }
    return count;
}
