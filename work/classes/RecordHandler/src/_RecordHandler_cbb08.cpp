#include "class.h"

// String::~String function pointer (single pc-rel deref to the fn-ptr global).
__attribute__((visibility("hidden"))) extern void (*g_RH_stringDtor)(void *s);

// RecordHandler::~RecordHandler() — destroys the three String members at +0x20,
// +0x14, +0x8 (high to low) and returns `this`.
extern "C" RecordHandler *RecordHandler_dtor(RecordHandler *self)
{
    void (*dtor)(void *) = g_RH_stringDtor;
    dtor((char *)self + 0x20);
    dtor((char *)self + 0x14);
    dtor((char *)self + 0x8);
    return self;
}
