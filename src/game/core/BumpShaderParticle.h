#ifndef GOF2_BUMPSHADERPARTICLE_H
#define GOF2_BUMPSHADERPARTICLE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::BumpShaderParticle — GLES2 bump-mapping shader for particle meshes.
// Derives from ShaderBaseStruct (which owns the GL program handle, dirty flag and name).
// This class adds the cached attribute/uniform location handles bound after Init, named
// after the literal "aN"/"uN" identifiers passed to glGet{Attrib,Uniform}Location.

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class BumpShaderParticle : public ShaderBaseStruct {
public:
    int attribA0;       // attribute "a0" (position)
    int attribA1;       // attribute "a1" (texcoord)
    int attribA2;       // attribute "a2"
    int attribA3;       // attribute "a3"
    int attribA4;       // attribute "a4"
    int uniformU0;      // uniform "u0"
    int uniformU1;      // uniform "u1" (modelview matrix)
    int uniformU2;      // uniform "u2" (normal matrix)
    int uniformU3;      // uniform "u3" (light dir / pos)
    int uniformU4;      // uniform "u4"
    int uniformU5;      // uniform "u5" (sampler, indexed base)
    int uniformU6;      // uniform "u6" (sampler)
    int uniformU7;      // uniform "u7"
    int uniformU8;      // uniform "u8"
    int uniformU9;      // uniform "u9"
    int uniformU10;     // uniform "u10"
    int uniformU11;     // uniform "u11"
    int uniformU12;     // uniform "u12" (global param A)
    int uniformU13;     // uniform "u13" (global param B)

    static int ShaderIndex;

    BumpShaderParticle();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
