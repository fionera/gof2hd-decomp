#include "class.h"

RadioMessage::RadioMessage(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    field<int>(this, 0x1c) = 0;
    field<int>(this, 0x00) = 0;
    field<int>(this, 0x04) = 0;
    field<int>(this, 0x08) = param_1;
    field<int>(this, 0x0c) = param_2;
    field<int>(this, 0x10) = param_3;
    field<int>(this, 0x14) = param_4;

    int *values = new int[param_5];
    field<int *>(this, 0x1c) = values;
    for (int i = 0; i < param_5; ++i) {
        values[i] = param_4 + i;
    }

    volatile int *count = &field<int>(this, 0x18);
    volatile uint16_t *flags = &field<uint16_t>(this, 0x20);
    volatile int *timer = &field<int>(this, 0x24);
    *count = param_5;
    *flags = 0;
    *timer = 0;
}
