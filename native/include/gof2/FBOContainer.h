#ifndef GOF2_FBOCONTAINER_H
#define GOF2_FBOCONTAINER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct FBOContainer {
    GLuint field_0x0;                   // +0x0
    GLuint field_0x4;                   // +0x4
    GLuint field_0x8;                   // +0x8
    GLsizei field_0xc;                  // +0xc
    GLsizei field_0x10;                 // +0x10
    Engine* field_0x14;                 // +0x14
    unsigned char field_0x18;           // +0x18
};
#endif
