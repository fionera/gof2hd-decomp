#include "quaternion.h"

namespace AbyssEngine {

void Quaternion::Convert(AEMath::Matrix &matrix) {
    float fVar7 = x;
    float fVar8 = y;
    float fVar14f = fVar7 * fVar7;
    float fVar9 = z;
    float fVar11f = fVar8 * fVar8;
    float fVar10 = w;
    double fVar14 = fVar14f;
    double fVar11 = fVar11f;
    float fVar12f = fVar9 * fVar9;
    double fVar12 = fVar12f;
    float fVar13f = fVar10 * fVar10;
    double fVar13 = fVar13f;
    double inv = 1.0 / (fVar14 + fVar11 + fVar12 + fVar13);
    double neg_xx = -fVar14;

    float xyf = fVar7 * fVar8;
    float wyf = fVar10 * fVar8;
    float xzf = fVar7 * fVar9;
    float yzf = fVar8 * fVar9;
    float wxf = fVar10 * fVar7;
    float wzf = fVar10 * fVar9;

    double wz = wzf;
    double xy = xyf;
    double wy = wyf;
    double xz = xzf;
    double yz = yzf;
    double wx = wxf;

    double m5 = fVar11 - fVar14;
    double m0 = fVar14 - fVar11;
    double m10 = neg_xx - fVar11;

    m5 -= fVar12;
    m0 -= fVar12;
    m10 += fVar12;

    double m1 = xy + wz;
    double m4 = xy - wz;
    double m2 = xz - wy;
    double m8 = wy + xz;
    double m6 = wx + yz;
    double m9 = yz - wx;

    m5 += fVar13;
    m0 += fVar13;
    m10 += fVar13;

    m2 += m2;
    m8 += m8;
    m6 += m6;
    m1 += m1;
    m4 += m4;
    m9 += m9;

    m5 *= inv;
    m0 *= inv;
    m10 *= inv;
    m1 *= inv;
    m4 *= inv;
    m2 *= inv;
    m8 *= inv;
    m6 *= inv;
    m9 *= inv;

    matrix.m[0] = static_cast<float>(m0);
    matrix.m[1] = static_cast<float>(m1);
    matrix.m[2] = static_cast<float>(m2);
    matrix.m[4] = static_cast<float>(m4);
    matrix.m[5] = static_cast<float>(m5);
    matrix.m[6] = static_cast<float>(m6);
    matrix.m[8] = static_cast<float>(m8);
    matrix.m[9] = static_cast<float>(m9);
    matrix.m[10] = static_cast<float>(m10);
}

} // namespace AbyssEngine
