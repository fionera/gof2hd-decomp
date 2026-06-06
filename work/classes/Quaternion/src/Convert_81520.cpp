#include "quaternion.h"

namespace AbyssEngine {

void Quaternion::Convert(AEMath::Matrix &matrix) {
    float x_ = x;
    float y_ = y;
    float z_ = z;
    float w_ = w;

    double xx = x_ * x_;
    double yy = y_ * y_;
    double zz = z_ * z_;
    double ww = w_ * w_;
    double nxx = -xx;
    double inv = 1.0 / (xx + yy + zz + ww);

    double xy = x_ * y_;
    double wz = w_ * z_;
    double wy = y_ * w_;
    double xz = x_ * z_;
    double yz = y_ * z_;
    double wx = w_ * x_;

    double m5 = yy - xx;
    double m0 = xx - yy;
    double m10 = nxx - yy;
    double m1 = xy + wz;
    double m4 = xy - wz;
    double m2 = xz - wy;
    double m8 = wy + xz;
    double m6 = wx + yz;
    double m9 = yz - wx;

    m5 -= zz;
    m0 -= zz;
    m10 += zz;
    m5 += ww;
    m0 += ww;
    m10 += ww;

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

    float out0 = m0;
    float out5 = m5;
    float out10 = m10;
    float out1 = m1;
    float out4 = m4;
    float out2 = m2;
    float out8 = m8;
    float out6 = m6;
    float out9 = m9;

    matrix.m[0] = out0;
    matrix.m[1] = out1;
    matrix.m[2] = out2;
    matrix.m[4] = out4;
    matrix.m[5] = out5;
    matrix.m[6] = out6;
    matrix.m[8] = out8;
    matrix.m[9] = out9;
    matrix.m[10] = out10;
}

} // namespace AbyssEngine
