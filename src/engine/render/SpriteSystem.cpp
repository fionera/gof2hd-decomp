#include "engine/core/AbyssEngine.h"
#include "engine/file/AEFile.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount, signed char vertexFormat,
                   Mesh **out);

    void MeshRelease(Engine * engine, Mesh * *slot);

    int SpriteSystemCreate(Engine *engine, unsigned short count, bool sharedSize, SpriteSystem **out) {
        unsigned int n = count;
        if (n == 0)
            return -4;

        SpriteSystem *sys = new SpriteSystem;
        sys->posCpu = 0;
        sys->sizeCpu = 0;
        sys->mesh = 0;
        *out = sys;
        sys->count = count;

        unsigned int triCount = (n << 1) & 0xffff;
        int rc = MeshCreate(engine, (unsigned short) ((n & 0x3fff) << 2), (unsigned short) triCount,
                            0x1f, &sys->mesh);
        SpriteSystem *s = *out;
        Mesh *mesh = s->mesh;
        unsigned short *indices = (unsigned short *) mesh->indices;
        if (rc != 1) {
            MeshRelease(engine, &s->mesh);
            if ((*out)->posCpu != 0)
                delete[] (*out)->posCpu;
            (*out)->posCpu = 0;
            if ((*out)->sizeCpu != 0)
                delete[] (*out)->sizeCpu;
            (*out)->sizeCpu = 0;
            if (*out != 0)
                delete *out;
            *out = 0;
            return -1;
        }

        s->posCpu = new float[n * 3];
        memset((*out)->posCpu, 0, n * 0xc);

        s = *out;
        s->sharedSize = sharedSize;
        if (sharedSize) {
            s->sizeCpu = new int16_t[1];
            (*out)->sizeCpu[0] = 0;
        } else {
            s->sizeCpu = new int16_t[n];
            memset((*out)->sizeCpu, 0, n << 1);
        }

        // Two triangles per sprite quad.
        unsigned short off = 0;
        unsigned short base = 0;
        for (; off < n * 6; off += 6) {
            unsigned short *p = indices + off;
            p[0] = base;
            p[1] = base | 1;
            p[2] = base | 2;
            p[3] = base;
            p[4] = base | 2;
            p[5] = base | 3;
            base = base + 4;
        }

        Mesh *m = (*out)->mesh;
        unsigned int vcount = m->vertexCount;
        for (unsigned short i = 0; i < vcount * 4; ++i)
            ((float *) m->colors)[i] = 1.0f;

        unsigned char shader = Engine::enableShader;
        for (unsigned int v = 0; v < vcount; ++v) {
            float *nrm = (float *) m->normals + v * 3;
            nrm[0] = 0.0f;
            nrm[1] = 0.0f;
            nrm[2] = 1.0f;
            if (shader) {
                float *tan = (float *) m->tangents + v * 3;
                tan[0] = 1.0f;
                tan[1] = 0.0f;
                tan[2] = 0.0f;
                float *bin = (float *) m->binormals + v * 3;
                bin[0] = 0.0f;
                bin[1] = 1.0f;
                bin[2] = 0.0f;
            }
        }

        return 1;
    }
}

namespace AbyssEngine {
    void SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem *sys) {
        if (sys == 0)
            return;

        unsigned short count = sys->count;
        float *uvBase = (float *) sys->mesh->texCoords;

        for (unsigned short idx = 0; idx < (count << 3); idx += 8) {
            float *p = uvBase + idx;
            p[0] = u0;
            p[1] = 1.0f - v0;
            p[2] = u1;
            p[3] = 1.0f - v0;
            p[4] = u1;
            p[5] = 1.0f - v1;
            p[6] = u0;
            p[7] = 1.0f - v1;
        }
    }
}

namespace AbyssEngine {
    void SpriteSystemSetUv(unsigned short idx, float u0, float v0, float u1, float v1, SpriteSystem *sys) {
        unsigned int count = 0;
        if (sys != 0)
            count = sys->count;
        if (sys == 0 || idx >= count)
            return;

        unsigned char shader = Engine::enableShader;
        float *p = (float *) sys->mesh->texCoords + (unsigned int) idx * 8;
        p[0] = u0;
        if (shader) {
            p[1] = v0;
            p[2] = u1;
            p[3] = v0;
            p[4] = u1;
            p[5] = v1;
            p[6] = u0;
            p[7] = v1;
        } else {
            p[1] = 1.0f - v0;
            p[2] = u1;
            p[3] = 1.0f - v0;
            p[4] = u1;
            p[5] = 1.0f - v1;
            p[6] = u0;
            p[7] = 1.0f - v1;
        }
    }
}

namespace AbyssEngine {
    void SpriteSystemSetRGBA(unsigned short idx, float r, float g, float b, float a, SpriteSystem *sys) {
        unsigned int count = 0;
        if (sys != 0)
            count = sys->count;
        if (sys == 0 || idx >= count)
            return;

        float *p = (float *) sys->mesh->colors + (unsigned int) idx * 16;
        p[0] = r;
        p[1] = g;
        p[2] = b;
        p[3] = a;
        p[4] = r;
        p[5] = g;
        p[6] = b;
        p[7] = a;
        p[8] = r;
        p[9] = g;
        p[10] = b;
        p[11] = a;
        p[12] = r;
        p[13] = g;
        p[14] = b;
        p[15] = a;
    }
}

namespace AbyssEngine {
    void SpriteSystemSetAllSize(short size, SpriteSystem *sys) {
        if (sys == 0)
            return;

        short *sizes = sys->sizeCpu;
        if (sys->sharedSize != 0) {
            sizes[0] = size;
            return;
        }
        for (unsigned int i = 0; i < sys->count; ++i)
            sizes[i] = size;
    }
}

namespace AbyssEngine {
    int MeshDraw(Engine * engine, Mesh * mesh);

    static inline unsigned int f2u(float f) {
        unsigned int u;
        memcpy(&u, &f, sizeof(u));
        return u;
    }

    void SpriteSystemDraw(Engine *engine, const Matrix &view, const Matrix &world, SpriteSystem *sys) {
        if (sys == 0)
            return;

        float *vertices = (float *) sys->mesh->positions;
        Matrix combined = view * world;  // computed by the original as well; the quads use `world`

        unsigned int floats = sys->count * 12;
        unsigned char sizeStep = sys->sharedSize ^ 1;
        const float *positions = sys->posCpu;
        const short *sizes = sys->sizeCpu;
        unsigned short positionIndex = 0;
        unsigned short sizeIndex = 0;
        for (unsigned short q = 0; q < floats; q += 12) {
            const float *p = positions + positionIndex;
            positionIndex += 3;
            float half = (float) (sizes[sizeIndex] >> 1);
            sizeIndex += sizeStep;

            float x = world.m[0] * p[0] + world.m[1] * p[1] + world.m[2] * p[2] + world.m[3];
            float y = p[0] * world.m[4] + p[1] * world.m[5] + p[2] * world.m[6] + world.m[7];
            float z = p[0] * world.m[8] + p[1] * world.m[9] + p[2] * world.m[10] + world.m[11];

            float *quad = vertices + q;
            quad[0] = x - half;
            quad[1] = y - half;
            quad[2] = z;
            quad[3] = x + half;
            quad[4] = y - half;
            quad[5] = z;
            quad[6] = x + half;
            quad[7] = y + half;
            quad[8] = z;
            quad[9] = x - half;
            quad[10] = y + half;
            quad[11] = z;
        }

        Mesh *mesh = sys->mesh;
        if (mesh->material == 0) {
            MeshDraw(engine, mesh);
            return;
        }
        ArrayAddCached(mesh, mesh->material->meshes);
        ArrayAddCached(view, sys->mesh->material->arr_2c);
        Matrix flipY;
        flipY.m[5] = -1.0f;
        ArrayAddCached(flipY, sys->mesh->material->arr_38);
        ArrayAddCached(world, sys->mesh->material->arr_5c);
        ArrayAddCached(0xffffffffu, sys->mesh->material->arr_50);
    }
}

namespace AbyssEngine {
    void MeshRelease(Engine * engine, Mesh * *slot);

    void SpriteSystemRelease(Engine *engine, SpriteSystem **slot) {
        if (*slot == 0)
            return;

        unsigned char *p = (unsigned char *) (*slot)->posCpu;
        if (p != 0)
            operator delete[](p); // lint: operator_call (a builtin delete[] lets -Oz drop the null test the original keeps)
        (*slot)->posCpu = 0;

        p = (unsigned char *) (*slot)->sizeCpu;
        if (p != 0)
            operator delete[](p); // lint: operator_call (a builtin delete[] lets -Oz drop the null test the original keeps)
        (*slot)->sizeCpu = 0;

        MeshRelease(engine, &(*slot)->mesh);

        if (*slot != 0)
            delete *slot;
        *slot = 0;
    }
}
