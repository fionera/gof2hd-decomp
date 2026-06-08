#include "class.h"

// AbyssEngine::ImageCreateFontFromFile(Engine*, char const*, unsigned short index, ImageFont**)
// Parse a bitmap-font atlas file and build an ImageFont (0x14 bytes): a glyph-codepoint array
// (+0x04) and a parallel array of per-glyph quad meshes (+0x0c). The file is scanned for the
// requested font index; matching fonts have their glyph table read and a 2-triangle textured
// quad mesh generated per glyph (positions in mesh+0x04, UVs in mesh+0x08). Non-matching font
// entries are skipped. Returns 1 on success, -1 on error, -4 on bad arguments.
namespace AbyssEngine {

extern "C" {
void *AE_operator_new(size_t_ n);
void *AE_operator_new_arr(size_t_ n);
int   AEFile_OpenRead(const char *path, void *handleOut, void *unused);
int   AEFile_Read(unsigned int n, void *dst, unsigned int handle);
int   AEFile_Skip(unsigned int n, unsigned int handle);
void  AEFile_Close(unsigned int handle);
float AE_VectorUnsignedToFloat(unsigned int v, unsigned char mode);
float AE_VectorSignedToFloat(int v, unsigned char mode);
}

int  MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                unsigned int vertexFormat, void **out);
void ImageFontRelease(Engine *engine, ImageFont **slot);

// Build one glyph quad into the mesh at *meshSlot using the four region shorts already read.
static void buildGlyphQuad(int mesh, unsigned int offX, unsigned int offY, unsigned int sizeX,
                           unsigned int sizeY, unsigned int atlasW, unsigned int atlasH,
                           unsigned char mode)
{
    float *pos = *(float **)(mesh + 4);
    float halfW = AE_VectorUnsignedToFloat(sizeX, mode);
    pos[0] = 0; pos[1] = 0; pos[2] = 0;
    pos[4] = 0; pos[5] = 0; pos[3] = halfW; pos[6] = halfW;
    float halfH = AE_VectorUnsignedToFloat(offY, mode); // mirrors decompile's field reuse
    pos[8] = 0; pos[9] = 0; pos[11] = 0; pos[7] = halfH; pos[10] = halfH;

    double aH = (double)AE_VectorUnsignedToFloat(atlasW, mode);
    double aW = (double)AE_VectorUnsignedToFloat(atlasH, mode);
    float offYs = AE_VectorSignedToFloat((int)(short)sizeY, mode);
    float offXs = AE_VectorSignedToFloat((int)(short)sizeX, mode);
    float u0 = AE_VectorUnsignedToFloat(offX, mode) * (float)(1.0 / aW);
    float v0 = AE_VectorUnsignedToFloat(offX, mode) * (float)(1.0 / aH);

    float *uv = *(float **)(mesh + 8);
    float v1 = (halfH + offYs) * (float)(1.0 / aH);
    float u1 = (halfW + offXs) * (float)(1.0 / aW);
    uv[0] = u0; uv[1] = v0; uv[2] = u1;
    uv[3] = v0; uv[4] = u1; uv[5] = v1;
    uv[6] = u0; uv[7] = v1;

    unsigned int *draw = *(unsigned int **)(mesh + 0x2c);
    draw[0] = 0x20000;
    draw[1] = 1;
    draw[2] = 0;
}

int ImageCreateFontFromFile(Engine *engine, char *path, unsigned short index, ImageFont **out)
{
    if (engine == 0 || path == 0)
        return -4;

    unsigned int handle = 0;
    if (AEFile_OpenRead(path, &handle, &handle) == 0)
        return -1;

    char magic[8];
    for (int i = 0; i < 4; ++i) magic[i] = '*';
    if (AEFile_Read(8, magic, handle) == 0) { ImageFontRelease(engine, out); return -1; }
    static const char want[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
    for (unsigned int k = 0; k < 8; ++k)
        if (want[k] != magic[k]) { ImageFontRelease(engine, out); return -1; }

    unsigned char mode = 0;
    unsigned char fmt = 0;
    unsigned short atlasH = 0, atlasW = 0, fontCount = 0;
    unsigned short glyphCount = 0;
    if (AEFile_Read(1, &fmt, handle) == 0) goto fail;
    if (AEFile_Read(2, &atlasH, handle) == 0) goto fail;
    if (AEFile_Read(2, &atlasW, handle) == 0) goto fail;
    if (AEFile_Read(2, &fontCount, handle) == 0) goto fail;
    if (AEFile_Skip((unsigned int)fontCount << 3, handle) == 0) goto fail;

    // Skip the format-specific palette/data block (size depends on fmt).
    {
        unsigned int fb = (unsigned int)fmt - 3;
        if (fb < 0x1f) {
            unsigned int extra = 0;
            if (((1u << (fb & 0xff)) & 0u) != 0 || (fmt >= 0x24)) {
                if (AEFile_Read(4, &extra, handle) == 0) goto fail;
            } else if (fb == 0) {
                extra = (unsigned int)atlasW * (unsigned int)atlasH * 4;
            }
            if (AEFile_Skip(extra, handle) == 0) goto fail;
        }
    }

    if (AEFile_Read(2, &glyphCount, handle) == 0) goto fail;
    if (glyphCount <= index) goto fail;

    {
        char *font = (char *)AE_operator_new(0x14);
        u16(font, 0x0) = 0;
        *(unsigned long long *)(font + 4) = 0;
        *(unsigned long long *)(font + 0xc) = 0;
        *out = (ImageFont *)font;
    }

    for (unsigned short g = 0; g < glyphCount; ++g) {
        if (g == index) {
            char *font = (char *)*out;
            if (AEFile_Read(2, font + 0x0, handle) == 0) goto fail; // glyph count
            void *codes = AE_operator_new_arr((unsigned int)u16(font, 0x0) << 1);
            pp(font, 0x4) = codes;
            if (AEFile_Read((unsigned int)u16((char *)*out, 0x0) << 1, pp((char *)*out, 0x4),
                            handle) == 0)
                goto fail;
            font = (char *)*out;
            void *meshes = AE_operator_new_arr((unsigned int)u16(font, 0x0) << 2);
            pp(font, 0xc) = meshes;

            unsigned int n = u16((char *)*out, 0x0);
            for (unsigned int gi = 0; gi < n; ++gi) {
                void **meshSlot = (void **)(*(int *)(pp((char *)*out, 0xc)) + gi * 4);
                if (MeshCreate(engine, 4, 2, 0x13, meshSlot) != 1)
                    goto fail;
                unsigned short sizeY = 0, sizeX = 0, offX = 0, offY = 0;
                if (AEFile_Read(2, &sizeY, handle) == 0) goto fail;
                if (AEFile_Read(2, &sizeX, handle) == 0) goto fail;
                if (AEFile_Read(2, &offX, handle) == 0) goto fail;
                if (AEFile_Read(2, &offY, handle) == 0) goto fail;
                int mesh = *(int *)(*(int *)(pp((char *)*out, 0xc)) + gi * 4);
                buildGlyphQuad(mesh, offX, offY, sizeX, sizeY, atlasW, atlasH, mode);
            }
        } else {
            unsigned short subCount = 0;
            if (AEFile_Read(2, &subCount, handle) == 0) goto fail;
            if (AEFile_Skip((unsigned int)subCount * 10, handle) == 0) goto fail;
        }
    }

    AEFile_Close(handle);
    return 1;

fail:
    ImageFontRelease(engine, out);
    return -1;
}

} // namespace AbyssEngine
