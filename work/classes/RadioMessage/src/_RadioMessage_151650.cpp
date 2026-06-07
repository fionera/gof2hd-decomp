#include "class.h"

RadioMessage::~RadioMessage()
{
    void *values = field<void *>(this, 0x1c);
    if (values != 0) {
        operator_delete__(values);
    }
    field<void *>(this, 0x1c) = 0;
}
