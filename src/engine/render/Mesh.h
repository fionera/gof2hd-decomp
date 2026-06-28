#ifndef GOF2_MESH_H
#define GOF2_MESH_H
#include "engine/core/Array.h"
#include "../core/AEString.h"

namespace AbyssEngine {
    class Transform;
    class Material;

    class Mesh {
    public:
        uint8_t vertexFormat;

        uint16_t vertexCount;

        unsigned char *positions;

        unsigned char *texCoords;

        unsigned char *colors;

        unsigned char *normals;

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

        float boundsCenterX;

        float boundsCenterY;

        float boundsCenterZ;

        float boundsRadius;

        float boundsRadiusSq;

        float pivotX;
        float pivotY;

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

        Mesh(Mesh *src);

        int ReadEnhancedDataFromFile(unsigned int file, unsigned int flags);
    };
}

#endif
