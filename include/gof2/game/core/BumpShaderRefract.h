#ifndef GOF2_BUMPSHADERREFRACT_H
#define GOF2_BUMPSHADERREFRACT_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::BumpShaderRefract (GLES2 refraction bump shader).

extern "C" char _ZTVN11AbyssEngine17BumpShaderRefractE[];

// Engine is the global ::Engine type (defined in Engine.h); referenced here by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class BumpShaderRefract : public ShaderBaseStruct {
public:
    int aPosition;      // attribute "a0"
    int aTexCoord;      // attribute "a1"
    int uSampler0;      // uniform "u0"
    int uMvpMatrix;     // uniform "u1"
    int uSampler2;      // uniform "u2" (sampler base, indexed)
    int uSampler3;      // uniform "u3"
    int uPixelSize;     // uniform "u4" (texel size)
    int uRefractSampler;// uniform "u8" (refract sampler)
    int uViewMatrix;    // uniform "u5"
    int uColorIndex;    // uniform "u6"
    int uColor;         // uniform "u7"

    static int ShaderIndex;

    BumpShaderRefract();
    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
};

} // namespace AbyssEngine

#endif
