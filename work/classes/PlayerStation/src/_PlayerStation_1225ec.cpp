#include "class.h"

extern "C" void *PlayerStation_destructor_body(PlayerStation *self) __attribute__((nothrow));

__attribute__((noreturn)) PlayerStation::~PlayerStation() noexcept(false)
{
    operator_delete(PlayerStation_destructor_body(this));
    __builtin_unreachable();
}
