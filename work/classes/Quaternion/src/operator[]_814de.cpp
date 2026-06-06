#include "quaternion.h"

extern "C" float quaternion_operator_index_const(const AbyssEngine::Quaternion *self, int index)
    __asm__("operator[]");

extern "C" float quaternion_operator_index_const(const AbyssEngine::Quaternion *self, int index) {
    return (&self->x)[index];
}
