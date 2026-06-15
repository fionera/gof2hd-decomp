#ifndef GOF2_CUBENORMALMAPPING_H
#define GOF2_CUBENORMALMAPPING_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {

class Engine;
class Mesh;

class CubeNormalMapping : public ShaderBaseStruct {
public:
    int attribA0;
    int attribA1;
    int attribA2;
    int attribA3;
    int attribA4;
    int uniformU0;
    int uniformU1;
    int uniformU2;
    int uniformU3;
    int uniformU5;
    int uniformU4;
    int uniformU6;
    int uniformU7;
    int uniformU8;
    int uniformU9;
    int uniformU12;
    int uniformU10;
    int uniformU11;

    CubeNormalMapping();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
