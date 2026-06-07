#include "class.h"

void Radio::setMessages(Array<RadioMessage *> *messages)
{
    Radio *self = this;
    Array<RadioMessage *> *list = messages;
    bool hasMessages = list != 0;
    F<Array<RadioMessage *> *>(this, 0x00) = messages;
    if (hasMessages) {
        for (uint32_t i = 0; i < list->length; ++i) {
            list->data[i]->setRadio(self);
        }
    }
}
