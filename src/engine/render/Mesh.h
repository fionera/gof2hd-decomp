#ifndef GOF2_MESH_H
#define GOF2_MESH_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "engine/math/BSphere.h"
#include "engine/math/Vector.h"

namespace AbyssEngine {
    class Transform;
    class Material;

    class Mesh {
    public:
        uint8_t vertexFormat;

        uint16_t vertexCount;

        float *positions;

        unsigned char *texCoords;

        unsigned char *colors;

        float *normals;

        unsigned char *tangents;

        unsigned char *binormals;

        uint32_t materialId;

        float shaderAnimValue0;

        float shaderAnimValue1;

        uint16_t indexCount;

        uint16_t field_0x2a;

        unsigned char *indices;

        Material *material;

        Transform *animation;

        uint8_t shared;

        AEMath::BSphere bounds;

        AEMath::Vector pivot;

        uint8_t uploaded;

        unsigned int positionVBO;

        unsigned int indexVBO;

        unsigned int texCoordVBO;

        unsigned int normalVBO;

        unsigned int tangentVBO;

        unsigned int binormalVBO;

        unsigned int colorVBO;

        int vboByteSize;

        uint32_t enhancedData;

        uint8_t vboEligible;

        uint8_t hasAnimation;

        // Empty mesh: every member zero except the squared bounds radius.
        Mesh()
            : vertexFormat(0), vertexCount(0), positions(nullptr), texCoords(nullptr), colors(nullptr),
              normals(nullptr), tangents(nullptr), binormals(nullptr), materialId(0), shaderAnimValue0(0.0f),
              indexCount(0), field_0x2a(0), indices(nullptr), material(nullptr),
              animation(nullptr), shared(0), bounds(), pivot(), uploaded(0),
              positionVBO(0), indexVBO(0), texCoordVBO(0), normalVBO(0), tangentVBO(0), binormalVBO(0),
              colorVBO(0), vboByteSize(0), enhancedData(0), vboEligible(0), hasAnimation(0) {
        }

        Mesh(Mesh *src);

        int ReadEnhancedDataFromFile(unsigned int file, unsigned int flags);
    };
}

#endif
