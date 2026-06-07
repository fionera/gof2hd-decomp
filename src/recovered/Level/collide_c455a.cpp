#include "class.h"

int Level::collide(Vector v) {
    int *vol = *(int **)((char *)this + 0xc4);
    if (vol != 0) {
        return (*(int (**)(int *, Vector))(*vol + 8))(vol, v);
    }
    return 0;
}
