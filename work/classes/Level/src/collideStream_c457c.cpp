#include "class.h"

int Level::collideStream(Vector v) {
    int *obj = *(int **)(*(int *)(*(int *)((char *)this + 0x100) + 4) + 4);
    if (obj != 0) {
        return (*(int (**)(int *, Vector))(*obj + 0x38))(obj, v);
    }
    return 0;
}
