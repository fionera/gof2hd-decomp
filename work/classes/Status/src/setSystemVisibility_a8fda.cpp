#include "class.h"
void Status::setSystemVisibility(int index, bool value) {
    ((bool *)systemVisibilities->data)[index] = value;
}
