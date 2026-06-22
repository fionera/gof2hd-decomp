#ifndef GOF2_MESH_H
#define GOF2_MESH_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {
    struct Transform;

    class Mesh {
    public:
        // +0x0 vertex-format flag byte:
        //   bit0 = drawable, bit1 = has UVs, bit3 = has normals, bit4 = has colors.
        union {
            uint8_t vertexFormat;
            uint8_t field_0x0;
        };

        // +0x2 vertex count.
        union {
            uint16_t vertexCount;
            uint16_t field_0x2;
        };

        // +0x4 CPU-side per-vertex arrays (freed once uploaded to a VBO).
        union {
            void *positions;
            void *field_0x4;
        }; // xyz per vertex
        union {
            void *texCoords;
            void *field_0x8;
        }; // uv per vertex
        union {
            void *colors;
            void *field_0xc;
        }; // rgba per vertex
        union {
            void *normals;
            void *field_0x10;
        }; // xyz per vertex
        union {
            void *tangents;
            void *field_0x14;
        }; // xyz per vertex
        union {
            void *binormals;
            void *field_0x18;
        }; // xyz per vertex
        // +0x1c material/shader scalar slot. Also doubles as the skybox post-effect/darken flag:
        // Level::createSpace() writes 0 here to disable the material darken when post-FX are on.
        union {
            uint32_t materialId;
            uint32_t field_0x1c;
            int32_t meshPostEffectFlag;
        }; // shader anim/darken scalar (also read as float)
        // +0x20/+0x24 shader animation scalars (refraction strength, scale-anim), read as float.
        // +0x20 is also taken by address (PlayerEgo cloak/turret setShip tail) as the base of a
        //   local-offset Vector copy; the slot itself is a single 4-byte word in the real struct.
        union {
            float shaderAnimValue0;
            float localOffset;
            float field_0x20;
        };

        union {
            float shaderAnimValue1;
            float field_0x24;
        };

        // +0x28 index count, +0x2c index array (u16 triangle-list indices).
        union {
            uint16_t indexCount;
            uint16_t field_0x28;
        };

        uint16_t field_0x2a;

        union {
            void *indices;
            void *field_0x2c;
        };

        // +0x30 owning Material sub-struct pointer (read in place as char*/int* by the shader
        //   TUs; PlayerEgo dereferences it then reads +0x20 of the pointee = materialBlock->0x20).
        union {
            void *material;
            void *materialBlock;
            void *field_0x30;
        };

        union {
            Transform *animation;
            Transform *field_0x34;
        }; // animation track
        union {
            uint8_t shared;
            uint8_t field_0x38;
        }; // aliased/shared mesh
        // +0x3c embedded bounding sphere (center xyz, radius, radius^2).
        union {
            float boundsCenterX;
            float field_0x3c;
        };

        union {
            float boundsCenterY;
            float field_0x40;
        };

        union {
            float boundsCenterZ;
            float field_0x44;
        };

        union {
            float boundsRadius;
            float field_0x48;
        };

        union {
            float boundsRadiusSq;
            float field_0x4c;
        };

        // +0x50 embedded pivot Vector (xy used).
        float pivotX; // +0x50
        float pivotY; // +0x54
        // +0x5c set once the CPU arrays have been uploaded into GL buffers.
        union {
            uint8_t uploaded;
            uint8_t field_0x5c;
        };

        // +0x60..0x78 GL buffer object names (0 when unused).
        union {
            unsigned int positionVBO;
            unsigned int field_0x60;
        };

        union {
            unsigned int indexVBO;
            uint32_t field_0x64;
        };

        union {
            unsigned int texCoordVBO;
            unsigned int field_0x68;
        };

        union {
            unsigned int normalVBO;
            uint32_t field_0x6c;
        };

        union {
            unsigned int tangentVBO;
            uint32_t field_0x70;
        };

        union {
            unsigned int binormalVBO;
            uint32_t field_0x74;
        };

        union {
            unsigned int colorVBO;
            uint32_t field_0x78;
        };

        // +0x7c running byte size of the uploaded buffers.
        union {
            int vboByteSize;
            uint32_t field_0x7c;
        };

        // +0x80 enhanced/raw data pointer; non-null marks the mesh as VBO-eligible.
        union {
            uint32_t enhancedData;
            uint32_t field_0x80;
        };

        union {
            uint8_t vboEligible;
            uint8_t field_0x84;
        };

        union {
            uint8_t hasAnimation;
            uint8_t field_0x85;
        };

        // ---- methods (recovered from the binary) ----
        Mesh(Mesh *src); // copy constructor
        int ReadEnhancedDataFromFile(unsigned int file, unsigned int flags);
    };
} // namespace AbyssEngine

#endif
