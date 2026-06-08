#include "class.h"

// AbyssEngine::ImageCreateRegionFromFile(Engine*, char const*, unsigned short index, Image2D*)
// Open a region atlas file, skip to the requested region index, and build a 2-triangle quad
// mesh whose vertex positions and UVs are derived from the region's offset/size fields. The
// Image2D out-param at +0x08.. holds the parsed atlas/region shorts; the generated mesh's
// position buffer (mesh+0x04) and UV buffer (mesh+0x08) are filled, and a draw-state block at
// mesh+0x2c is initialized. Returns 1 on success; negative codes on error.
namespace AbyssEngine {

extern "C" {
int   AEFile_OpenRead(const char *path, void *handleOut, void *unused);
int   AEFile_Read(unsigned int n, void *dst, unsigned int handle);
int   AEFile_Skip(unsigned int n, unsigned int handle);
void  AEFile_Close(unsigned int handle);
float AE_VectorUnsignedToFloat(unsigned int v, unsigned char mode);
float AE_VectorSignedToFloat(int v, unsigned char mode);
}

int  MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                unsigned int vertexFormat, void **out);
void MeshRelease(Engine *engine, Mesh **slot);

int ImageCreateRegionFromFile(Engine *engine, char *path, unsigned short index, Image2D *region)
{
    if (engine == 0 || path == 0)
        return -4;

    char *r = (char *)region;
    unsigned int handle = 0;
    if (AEFile_OpenRead(path, &handle, &handle) == 0)
        return -1;

    char magic[8];
    for (int i = 0; i < 4; ++i) magic[i] = '*';
    if (AEFile_Read(8, magic, handle) == 0)
        return -1;
    static const char want[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
    for (unsigned int k = 0; k < 8; ++k)
        if (want[k] != magic[k])
            return -1;

    // Header: skip 1 byte, then atlas width/height into region+0x08, +0x0a, and region count.
    unsigned short regionCount = 0;
    if (AEFile_Skip(1, handle) == 0) return -1;
    if (AEFile_Read(2, r + 0x08, handle) == 0) return -1;   // atlas width
    if (AEFile_Read(2, r + 0x0a, handle) == 0) return -1;   // atlas height
    if (AEFile_Read(2, &regionCount, handle) == 0) return -1;
    if (regionCount <= index) return -1;

    if (MeshCreate(engine, 4, 2, 0x13, (void **)region) != 1)
        return -2;

    unsigned char mode = 0; // fpscr rounding mode bits; 0 is the common default
    for (unsigned short i = 0; i < regionCount; ++i) {
        if (i == index) {
            // Region record: offX, offY, sizeX, sizeY shorts into region+0x0c..0x12.
            if (AEFile_Read(2, r + 0x0c, handle) == 0) goto fail;
            if (AEFile_Read(2, r + 0x0e, handle) == 0) goto fail;
            if (AEFile_Read(2, r + 0x10, handle) == 0) goto fail;
            if (AEFile_Read(2, r + 0x12, handle) == 0) goto fail;

            int mesh = *(int *)r;
            float *pos = *(float **)(mesh + 4);
            // Half width/height of the quad in object units (region size).
            float halfW = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x10), mode);
            pos[0] = 0; pos[1] = 0; pos[2] = 0;
            pos[4] = 0; pos[5] = 0; pos[3] = halfW; pos[6] = halfW;
            float halfH = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x12), mode);
            pos[8] = 0; pos[9] = 0; pos[11] = 0; pos[7] = halfH; pos[10] = halfH;

            double atlasH = (double)AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x0a), mode);
            double atlasW = (double)AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x08), mode);
            float offYs = AE_VectorSignedToFloat((int)(short)u16(r, 0x0e), mode);
            float offXs = AE_VectorSignedToFloat((int)(short)u16(r, 0x0c), mode);
            float offYu = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x0e), mode);
            float offXu = AE_VectorUnsignedToFloat((unsigned int)u16(r, 0x0c), mode);

            float *uv = *(float **)(mesh + 8);
            float u0 = offXu * (float)(1.0 / atlasW);
            float v0 = offYu * (float)(1.0 / atlasH);
            float v1 = (halfH + offYs) * (float)(1.0 / atlasH);
            float u1 = (halfW + offXs) * (float)(1.0 / atlasW);
            uv[0] = u0; uv[1] = v0; uv[2] = u1;
            uv[3] = v0; uv[4] = u1; uv[5] = v1;
            uv[6] = u0; uv[7] = v1;

            unsigned int *draw = *(unsigned int **)(mesh + 0x2c);
            draw[0] = 0x20000;
            draw[1] = 1;
            draw[2] = 0; // DAT default

            // i++ then loop falls through to close.
        } else {
            if (AEFile_Skip(8, handle) == 0)
                goto fail;
        }
    }

    AEFile_Close(handle);
    return 1;

fail:
    MeshRelease(engine, (Mesh **)region);
    return -1;
}

} // namespace AbyssEngine
