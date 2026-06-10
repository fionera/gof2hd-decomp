#ifndef GOF2_EASEINOUTMATRIX_H
#define GOF2_EASEINOUTMATRIX_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

namespace AEMath {

}



// Layout (byte offsets):
//   0x00  Matrix  m_min      (15 floats / 60 bytes)
//   0x3c  Quaternion m_q0
//   0x58  Quaternion m_q1
//   0x74  float   m_t        (eased parameter; runs 0.75 .. 1.25, rests at 1.0)
//   0x78  Matrix  m_current
//   0xb4  Matrix  m_max
//   0xf0  float   m_duration


static inline float &f32(EaseInOutMatrix *self, unsigned off) {
    return *(float *)((char *)self + off);
}

} // namespace AbyssEngine

struct EaseInOutMatrix {
    AEMath::Matrix field_0x0;           // +0x0
    AEMath::Vector field_0x4c;          // +0x4c
    AEMath::Vector field_0x68;          // +0x68
    AEMath::Matrix field_0x78;          // +0x78
    AEMath::Matrix field_0xb4;          // +0xb4
};
#endif
