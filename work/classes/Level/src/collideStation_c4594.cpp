#include "class.h"

struct Status {
    int inEmptyOrbit();
};

__attribute__((visibility("hidden"))) extern Status **g_status_collideStation;

int Level::collideStation(Vector v) {
    int landmarks = *(int *)((char *)this + 0x100);
    if (landmarks != 0 &&
        *(int *)(*(int *)(landmarks + 4)) != 0 &&
        (*g_status_collideStation)->inEmptyOrbit() == 0) {
        int *obj = *(int **)(*(int *)((char *)this + 0x100) + 4);
        return (*(int (**)(int *, Vector))(*obj + 0x38))(obj, v);
    }
    return 0;
}
