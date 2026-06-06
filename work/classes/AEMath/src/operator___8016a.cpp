#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Matrix &Matrix::operator*=(const Matrix &other) {
    float y0 = m[1];
    float b10 = other.m[4];
    float x0 = m[0];
    float result = y0 * b10;
    float b00 = other.m[0];
    float z0 = m[2];
    float b20 = other.m[8];
    float w0 = m[3];
    result += x0 * b00;
    result += z0 * b20;
    m[0] = result;

    float b11 = other.m[5];
    float b01 = other.m[1];
    result = y0 * b11;
    float b21 = other.m[9];
    result += x0 * b01;
    result += z0 * b21;
    m[1] = result;

    float b12 = other.m[6];
    float b02 = other.m[2];
    result = y0 * b12;
    float b22 = other.m[10];
    result += x0 * b02;
    result += z0 * b22;
    m[2] = result;

    float b13 = other.m[7];
    float b03 = other.m[3];
    result = y0 * b13;
    float b23 = other.m[11];
    result += x0 * b03;
    result += z0 * b23;
    m[3] = w0 + result;

    float x1 = m[4];
    b00 = other.m[0];
    float y1 = m[5];
    result = x1 * b00;
    float z1 = m[6];
    result += b10 * y1;
    result += b20 * z1;
    m[4] = result;

    b01 = other.m[1];
    result = x1 * b01;
    result += b11 * y1;
    result += b21 * z1;
    m[5] = result;

    b02 = other.m[2];
    result = x1 * b02;
    result += b12 * y1;
    result += b22 * z1;
    m[6] = result;

    b03 = other.m[3];
    result = x1 * b03;
    result += b13 * y1;
    w0 = m[7];
    result += b23 * z1;
    m[7] = w0 + result;

    float y2 = m[9];
    b10 = other.m[4];
    float x2 = m[8];
    result = y2 * b10;
    float z2 = m[10];
    result += b00 * x2;
    result += b20 * z2;
    m[8] = result;

    b11 = other.m[5];
    result = y2 * b11;
    result += b01 * x2;
    result += b21 * z2;
    m[9] = result;

    b12 = other.m[6];
    result = y2 * b12;
    result += b02 * x2;
    result += b22 * z2;
    m[10] = result;

    b13 = other.m[7];
    result = y2 * b13;
    result += b03 * x2;
    result += b23 * z2;
    m[11] += result;

    reinterpret_cast<Vector *>(&m[12])->operator*=(*reinterpret_cast<const Vector *>(&other.m[12]));
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
