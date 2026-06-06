#include "quaternion.h"

namespace AbyssEngine {

void Quaternion::Convert(AEMath::Matrix &matrix) {
    float fVar10 = w;
    float fVar7 = x;
    float fVar8 = y;
    float fVar9 = z;
    double fVar14 = fVar7 * fVar7;
    double fVar11 = fVar8 * fVar8;
    double fVar12 = fVar9 * fVar9;
    double fVar13 = fVar10 * fVar10;
    double fVar1 = 1.0 / (fVar14 + fVar11 + fVar12 + fVar13);
    double fVar6 = (double)(fVar7 * fVar8) + (double)(fVar10 * fVar9);
    double fVar5 = (double)(fVar7 * fVar8) - (double)(fVar10 * fVar9);
    double fVar4 = (double)(fVar7 * fVar9) - (double)(fVar10 * fVar8);
    double fVar3 = (double)(fVar10 * fVar8) + (double)(fVar7 * fVar9);
    double fVar2 = (double)(fVar10 * fVar7) + (double)(fVar8 * fVar9);
    double fVar7b = (double)(fVar8 * fVar9) - (double)(fVar10 * fVar7);
    matrix.m[0] = (((fVar14 - fVar11) - fVar12) + fVar13) * fVar1;
    matrix.m[1] = (fVar6 + fVar6) * fVar1;
    matrix.m[2] = (fVar4 + fVar4) * fVar1;
    matrix.m[4] = (fVar5 + fVar5) * fVar1;
    matrix.m[5] = (((fVar11 - fVar14) - fVar12) + fVar13) * fVar1;
    matrix.m[6] = (fVar2 + fVar2) * fVar1;
    matrix.m[8] = (fVar3 + fVar3) * fVar1;
    matrix.m[9] = (fVar7b + fVar7b) * fVar1;
    matrix.m[10] = ((-fVar14 - fVar11) + fVar12 + fVar13) * fVar1;
}

} // namespace AbyssEngine
