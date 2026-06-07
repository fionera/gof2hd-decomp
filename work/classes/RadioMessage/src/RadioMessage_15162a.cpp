#include "class.h"

RadioMessage::RadioMessage(int param_1, int param_2, Objective *param_3)
{
    field<int>(this, 0x1c) = 0;
    field<int>(this, 0x00) = 0;
    field<Objective *>(this, 0x04) = param_3;
    field<int>(this, 0x08) = param_1;
    field<int>(this, 0x0c) = param_2;
    field<int>(this, 0x10) = 0xb;
    field<uint16_t>(this, 0x20) = 0;
    field<int>(this, 0x24) = 0;
}
