#ifndef GOF2_GENERICSHADER1_H
#define GOF2_GENERICSHADER1_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// AbyssEngine::GenericShader1 — GLES2 normal-mapped lighting shader.
// Binds five vertex attributes (position/normal/tangent/binormal/texCoord) and the
// u_m0..u_m8 uniform block, and pushes per-mesh material/transform data each frame.
namespace AbyssEngine {

class Engine;
class Mesh;

class GenericShader1 : public ShaderBaseStruct {
public:
    int aPosition;      // a_position
    int aNormal;        // a_normal
    int aTangent;       // a_tangent
    int aBinormal;      // a_binormal
    int aTexCoord;      // a_texCoord
    int uM0;            // u_m0
    int uM1;            // u_m1
    int uM2;            // u_m2
    int uM3;            // u_m3
    int uM4;            // u_m4
    int uM5;            // u_m5
    int uM6;            // u_m6
    int uM7;            // u_m7
    int uM8;            // u_m8

    GenericShader1();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
