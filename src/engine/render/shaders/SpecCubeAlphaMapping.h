#ifndef GOF2_SPECCUBEALPHAMAPPING_H
#define GOF2_SPECCUBEALPHAMAPPING_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class Mesh;

    class SpecCubeAlphaMapping : public ShaderBaseStruct {
    public:
        int attrA0; // attrib a0 (position)
        int attrA1; // attrib a1 (normal)
        int attrA2; // attrib a2 (texcoord)
        int uniU0; // uniform u0 (mvp)
        int uniU1; // uniform u1 (normal matrix)
        int uniU2;
        int uniU3;
        int uniU5;
        int uniU4;
        int uniU6;
        int uniU7;
        int uniU8;
        int uniU9;
        int uniU10;
        int uniU11;

        SpecCubeAlphaMapping();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
