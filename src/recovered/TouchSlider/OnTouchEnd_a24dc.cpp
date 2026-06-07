#include "class.h"

struct TouchSlider {
    int OnTouchEnd(int param_1, int param_2);
    float getValue();
    void setValue(float v);
};

int TouchSlider::OnTouchEnd(int param_1, int param_2)
{
    if (F<uint8_t>(this, 0x35) != 0)
        return 0;

    int uVar1;
    if (F<uint8_t>(this, 0x34) == 0) {
        uVar1 = 0;
    } else {
        int iVar4 = F<int>(this, 0x28);
        if (iVar4 > 0) {
            float fVar6 = (float)(iVar4 + 1);
            float fVar2 = getValue();
            float fVar5;
            float fVar7 = 0.0f;
            int iVar3 = 0;
            while (true) {
                fVar5 = fVar2;
                if (iVar3 >= iVar4 + 2)
                    break;
                fVar5 = fVar7;
                if (fVar2 <= (1.0f / fVar6) * 0.5f + fVar7)
                    break;
                fVar7 = 1.0f / fVar6 + fVar7;
                iVar3 = iVar3 + 1;
            }
            setValue(fVar5);
        }
        uVar1 = 1;
    }
    F<uint8_t>(this, 0x34) = 0;
    return uVar1;
}
