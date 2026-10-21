#ifndef GOF2_AETYPES_H
#define GOF2_AETYPES_H
// Short global type aliases + small typedefs the recovered code expects in scope, plus the one
// engine free-function (AELabelObject) that has no better home. The math types themselves live in
// mathtypes.h; here we just surface their short names globally.
#include <cstdint>
#include "mathtypes.h"

// ---- global aliases so recovered code's short type names resolve --------------------------
namespace AEMath = AbyssEngine::AEMath;   // so global `AEMath::Vector` resolves
using AbyssEngine::AEMath::Vector;        // recovered structs use bare Vector/Matrix
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::Quaternion;
typedef unsigned char  byte;
typedef unsigned int   uint;
typedef unsigned short ushort;
struct u32x4 { uint32_t v[4]; };          // NEON 4x u32 (best-effort, non-matching)

// AELabelObject(glIdentifier, name, label): in the shipped binary this is the GLES debug-object
// labelling stub (KHR_debug). On the production build it is compiled out to a no-op that simply
// returns its first argument, so callers stay byte-identical without any GL side effect.
unsigned int AELabelObject(unsigned int glIdentifier, unsigned int name, const char *label);

#endif // GOF2_AETYPES_H
