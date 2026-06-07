#include "class.h"

RadioMessage *Radio::getMessage(int index)
{
    void *messages = F<void *>(this, 0x00);
    void **data = F<void **>(messages, 0x04);
    return (RadioMessage *)data[index];
}
