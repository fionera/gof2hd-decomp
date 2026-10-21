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
    void ImageRelease(Image **slot) {
        if (*slot != 0) {
            delete[] (*slot)->data;
            (*slot)->data = 0;
            if (*slot != 0)
                delete *slot;
            *slot = 0;
        }
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount, signed char vertexFormat,
                   Mesh **out);

    void MeshRelease(Engine * engine, Mesh * *slot);

    int ImageCreateRegionFromFile(Engine *engine, const char *path, unsigned short index, Image2D *region) {
        if (engine == 0 || path == 0)
            return -4;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, (uint32_t *) &handle) == 0)
            return -1;

        char magic[8] = "*******";
        if (AEFile::Read((uint32_t) 8, magic, handle) == 0)
            return -1;
        static const char want[8] = "AEimage";
        for (unsigned int k = 0; k < 8; ++k)
            if (want[k] != magic[k])
                return -1;

        if (AEFile::Skip((uint32_t) 1, handle) == 0)
            return -1;
        if (AEFile::Read((uint32_t) 2, &region->atlasW, handle) == 0)
            return -1;
        if (AEFile::Read((uint32_t) 2, &region->atlasH, handle) == 0)
            return -1;
        unsigned short regionCount = 0;
        if (AEFile::Read((uint32_t) 2, &regionCount, handle) == 0)
            return -1;
        if (regionCount <= index)
            return -1;

        if (MeshCreate(engine, 4, 2, 0x13, (Mesh **) region) != 1)
            return -2;

        for (unsigned short i = 0; i < regionCount; ++i) {
            if (i != index) {
                if (AEFile::Skip((uint32_t) 8, handle) == 0)
                    goto fail;
                continue;
            }
            if (AEFile::Read((uint32_t) 2, &region->offX, handle) == 0)
                goto fail;
            if (AEFile::Read((uint32_t) 2, &region->offY, handle) == 0)
                goto fail;
            if (AEFile::Read((uint32_t) 2, &region->sizeX, handle) == 0)
                goto fail;
            if (AEFile::Read((uint32_t) 2, &region->sizeY, handle) == 0)
                goto fail;

            {
                Mesh *mesh = region->mesh;
                float *pos = (float *) mesh->positions;
                pos[0] = 0;
                pos[1] = 0;
                pos[2] = 0;
                float w = (float) region->sizeX;
                pos[3] = w;
                pos[4] = 0;
                pos[5] = 0;
                pos[6] = w;
                float h = (float) region->sizeY;
                pos[7] = h;
                pos[8] = 0;
                pos[9] = 0;
                pos[10] = h;
                pos[11] = 0;

                float invW = (float) (1.0 / (double) region->atlasW);
                float invH = (float) (1.0 / (double) region->atlasH);
                float u0 = (float) region->offX * invW;
                float v0 = (float) region->offY * invH;
                float u1 = (w + (float) (short) region->offX) * invW;
                float v1 = (h + (float) (short) region->offY) * invH;
                float *uv = (float *) mesh->texCoords;
                uv[0] = u0;
                uv[1] = v0;
                uv[2] = u1;
                uv[3] = v0;
                uv[4] = u1;
                uv[5] = v1;
                uv[6] = u0;
                uv[7] = v1;

                unsigned short *tri = (unsigned short *) mesh->indices;
                tri[0] = 0;
                tri[1] = 2;
                tri[2] = 1;
                tri[3] = 0;
                tri[4] = 3;
                tri[5] = 2;
            }
        }

        AEFile::Close(handle);
        return 1;

    fail:
        MeshRelease(engine, (Mesh **) region);
        return -1;
    }
}

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount, signed char vertexFormat,
                   Mesh **out);

    void ImageFontRelease(Engine * engine, ImageFont * *slot);


    int ImageCreateFontFromFile(Engine *engine, const char *path, unsigned short index, ImageFont **out) {
        if (engine == 0 || path == 0)
            return -4;

        unsigned int handle = 0;
        unsigned short fontCount = 0;
        unsigned short regionCount = 0;
        unsigned short atlasW = 0;
        unsigned short atlasH = 0;
        unsigned char format = 0;
        if (AEFile::OpenRead(path, (uint32_t *) &handle) == 0)
            return -1;

        char magic[8] = "*******";
        if (AEFile::Read((uint32_t) 8, magic, handle) == 0)
            goto fail;
        {
            static const char want[8] = "AEimage";
            for (unsigned int k = 0; k < 8; ++k)
                if (want[k] != magic[k])
                    return -1;
        }

        if (AEFile::Read((uint32_t) 1, &format, handle) == 0)
            goto fail;
        if (AEFile::Read((uint32_t) 2, &atlasW, handle) == 0)
            goto fail;
        if (AEFile::Read((uint32_t) 2, &atlasH, handle) == 0)
            goto fail;
        if (AEFile::Read((uint32_t) 2, &regionCount, handle) == 0)
            goto fail;
        if (AEFile::Skip((uint32_t) regionCount * 8, handle) == 0)
            goto fail;

        // Skip the embedded pixel data: a length-prefixed compressed block or raw RGBA.
        switch (format) {
            case 13:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 22:
            case 23:
            case 32:
            case 33:
            case 36:
            case 64:
            case 66: {
                unsigned int size;
                if (AEFile::Read((uint32_t) 4, &size, handle) == 0)
                    goto fail;
                if (AEFile::Skip(size, handle) == 0)
                    goto fail;
                break;
            }
            case 1:
            case 3:
                if (AEFile::Skip((uint32_t) atlasH * atlasW * 4, handle) == 0)
                    goto fail;
                break;
            default:
                break;
        }

        if (AEFile::Read((uint32_t) 2, &fontCount, handle) == 0)
            goto fail;
        if (fontCount <= index)
            goto fail;

        *out = new ImageFont();
        for (unsigned short f = 0; f < fontCount; ++f) {
            if (f != index) {
                unsigned short glyphs = 0;
                if (AEFile::Read((uint32_t) 2, &glyphs, handle) == 0)
                    goto fail;
                if (AEFile::Skip((uint32_t) glyphs * 10, handle) == 0)
                    goto fail;
                continue;
            }

            if (AEFile::Read((uint32_t) 2, &(*out)->glyphCount, handle) == 0)
                goto fail;
            (*out)->codes = new uint16_t[(*out)->glyphCount];
            if (AEFile::Read((uint32_t) (*out)->glyphCount * 2, (*out)->codes, handle) == 0)
                goto fail;
            (*out)->glyphMeshes = new Mesh *[(*out)->glyphCount];

            for (unsigned int g = 0; g < (*out)->glyphCount; ++g) {
                if (MeshCreate(engine, 4, 2, 0x13, &(*out)->glyphMeshes[g]) != 1)
                    goto fail;
                unsigned short u = 0;
                unsigned short v = 0;
                unsigned short sizeX = 0;
                unsigned short sizeY = 0;
                if (AEFile::Read((uint32_t) 2, &u, handle) == 0)
                    goto fail;
                if (AEFile::Read((uint32_t) 2, &v, handle) == 0)
                    goto fail;
                if (AEFile::Read((uint32_t) 2, &sizeX, handle) == 0)
                    goto fail;
                if (AEFile::Read((uint32_t) 2, &sizeY, handle) == 0)
                    goto fail;

                Mesh *mesh = (*out)->glyphMeshes[g];
                float *pos = (float *) mesh->positions;
                pos[0] = 0;
                pos[1] = 0;
                pos[2] = 0;
                float w = (float) sizeX;
                pos[3] = w;
                pos[4] = 0;
                pos[5] = 0;
                pos[6] = w;
                float h = (float) sizeY;
                pos[7] = h;
                pos[8] = 0;
                pos[9] = 0;
                pos[10] = h;
                pos[11] = 0;

                float invH = (float) (1.0 / (double) atlasH);
                float invW = (float) (1.0 / (double) atlasW);
                float u0 = (float) u * invW;
                float v0 = (float) v * invH;
                float u1 = (w + (float) u) * invW;
                float v1 = (h + (float) v) * invH;
                float *uv = (float *) mesh->texCoords;
                uv[0] = u0;
                uv[1] = v0;
                uv[2] = u1;
                uv[3] = v0;
                uv[4] = u1;
                uv[5] = v1;
                uv[6] = u0;
                uv[7] = v1;

                unsigned short *tri = (unsigned short *) mesh->indices;
                tri[0] = 0;
                tri[1] = 2;
                tri[2] = 1;
                tri[3] = 0;
                tri[4] = 3;
                tri[5] = 2;
            }
        }

        AEFile::Close(handle);
        return 1;

    fail:
        ImageFontRelease(engine, out);
        return -1;
    }
}

namespace AbyssEngine {
    void ImageRelease(Image * *slot);

    int ImageCreateFromFile(Engine *engine, const char *path, Image **out) {
        if (engine == 0 || path == 0)
            return -4;

        Image *img = new Image;
        img->hasMipmaps = 0;
        *(uint32_t *) &img->width = 0;
        img->format = 0;
        img->data = 0;
        *out = img;

        unsigned int handle = 0;
        if (AEFile::OpenRead(path, (uint32_t *) (&handle)) == 0) {
            if (*out != 0)
                delete *out;
            *out = 0;
            return -1;
        }

        static const char magic[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
        char hdr[8];
        for (int i = 0; i < 4; ++i) hdr[i] = '*';
        if (AEFile::Read((uint32_t)(8), hdr, handle) == 0)
            goto fail;
        for (unsigned int k = 0; k < 8; ++k) {
            if (magic[k] != hdr[k])
                goto fail;
        }

        {
            Image *im = *out;
            unsigned char fmt = 0;
            unsigned short palCount = 0;
            if (AEFile::Read((uint32_t)(1), &fmt, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), &im->width, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), &im->height, handle) == 0) goto fail;
            if (AEFile::Read((uint32_t)(2), &palCount, handle) == 0) goto fail;

            AEFile::Skip((uint32_t)((unsigned int) palCount << 3), handle);

            if (fmt & 2)
                im->hasMipmaps = 1;

            unsigned int dataLen = 0;
            switch (fmt) {
                case 1:
                case 3:
                case 0x81: {
                    unsigned int sz = (unsigned int) im->width * (unsigned int) im->height * 4;
                    unsigned char *p = new unsigned char[sz];
                    im->data = p;
                    im = *out;
                    if (AEFile::Read((uint32_t)(sz), im->data, handle) == 0) goto fail;
                    im = *out;
                    im->dataLen = sz;
                    {
                        unsigned int v = (unsigned int) (int) (signed char) fmt;
                        unsigned int code = ((int) v < 0) ? ((v > 0x7fffffff) ? 6u : v) : 3u;
                        if ((int) v >= 0) code = 3;
                        im->format = code;
                    }
                    break;
                }
                case 0xd:
                case 0xf:
                    if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
                    im->data = new unsigned char[dataLen];
                    if (AEFile::Read((uint32_t)(dataLen), im->data, handle) == 0) goto fail;
                    im->format = 4;
                    im->dataLen = dataLen;
                    break;
                case 0x10:
                case 0x12:
                    if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
                    im->data = new unsigned char[dataLen];
                    if (AEFile::Read((uint32_t)(dataLen), im->data, handle) == 0) goto fail;
                    im->format = 5;
                    im->dataLen = dataLen;
                    break;
                case 0x11:
                case 0x13:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 7;
                    im->data = new unsigned char[im->dataLen];
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
                    break;
                case 0x14:
                case 0x16:
                case 0x17:
                case 0x40:
                case 0x42:
                    if (fmt == 0x40)
                        im->hasMipmaps = 0;
                    if (AEFile::Read((uint32_t)(4), &dataLen, handle) == 0) goto fail;
                    im->data = new unsigned char[dataLen];
                    if (AEFile::Read((uint32_t)(dataLen), im->data, handle) == 0) goto fail;
                    im->format = 0xb;
                    im->dataLen = dataLen;
                    if (fmt == 0x17)
                        im->hasMipmaps = 0;
                    break;
                case 0x20:
                case 0x22:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 8;
                    im->data = new unsigned char[im->dataLen];
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
                    break;
                case 0x21:
                case 0x23:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 9;
                    im->data = new unsigned char[im->dataLen];
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
                    break;
                case 0x24:
                case 0x26:
                    if (AEFile::Read((uint32_t)(4), &im->dataLen, handle) == 0) goto fail;
                    im->format = 10;
                    im->data = new unsigned char[im->dataLen];
                    if (AEFile::Read((uint32_t)(im->dataLen), im->data, handle) == 0) goto fail;
                    break;
                default:
                    break;
            }

            AEFile::Close(handle);
            return 1;
        }

    fail:
        ImageRelease(out);
        AEFile::Close(handle);
        return -1;
    }
}

namespace AbyssEngine {
    typedef void (*ImageCallback)(Image *, void *); // lint: void_ptr (exported texture-load callback signature)

    int TextureCreateFromFileIntern(Engine *engine, const char *path, ImageCallback cb, void *user, // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    unsigned int *outIds, float scale, AELoadedTexture *outTex,
                                    bool flag);

    int TextureCreateFromFile(Engine *engine, const char *path, ImageCallback cb, void *user, // lint: void_ptr (exported TextureCreateFromFile signature)
                              // lint: void_ptr (exported TextureCreateFromFile signature)
                              unsigned int *outIds, bool flag, float scale) {
        TextureCreateFromFileIntern(engine, path, cb, user, outIds, scale,
                                    (AELoadedTexture *) 0, flag);
        return 1;
    }
}

namespace AbyssEngine {
    extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    void glTexEnvi(unsigned int t, unsigned int p, int v);
    }

    int ImageCreateFromFile(Engine *engine, const char *path, Image **out);

    void ImageRelease(Image * *slot);

    int GenerateCompressedTexture(Image * /*image*/) {
        return 1;
    }

    int TextureCreateFromFileIntern(Engine *engine, const char *path, void (*cb)(Image *, void *), // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    void *user, unsigned int *outIds, float scale, // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    AELoadedTexture *outTex, bool registerTexture) {
        Image *image = 0;
        *outIds = 0;
        engine->lastGlError = glGetError();

        int result = ImageCreateFromFile(engine, path, &image);
        if (result != 1)
            return result;
        if (cb != 0)
            cb(image, user);

        glGenTextures(1, outIds);
        if (image->format == 6) {
            if (Engine::enableShader != 0) {
                glBindTexture(0x8513, *outIds);
                glTexParameteri(0x8513, 0x2800, 0x2601);
                glTexParameteri(0x8513, 0x2801, 0x2601);
                int faceHeight = image->height / 6;
                int faceWidth = image->width;
                glTexImage2D(0x8517, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data);
                int facePixels = faceHeight * faceWidth;
                glTexImage2D(0x8516, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 4);
                glTexImage2D(0x8519, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 8);
                glTexImage2D(0x8515, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 12);
                glTexImage2D(0x851a, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 16);
                glTexImage2D(0x8518, 0, 0x1908, faceWidth, faceHeight, 0, 0x1908, 0x1401, image->data + facePixels * 20);
            }
        } else {
            glBindTexture(0xde1, *outIds);
            glPixelStorei(0xcf5, 1);
            if (Engine::enableShader == 0)
                glTexEnvi(0x2300, 0x2200, 0x2100);
            if (Engine::clampTextures) {
                glTexParameteri(0xde1, 0x2802, 0x812f);
                glTexParameteri(0xde1, 0x2803, 0x812f);
            } else {
                glTexParameteri(0xde1, 0x2802, 0x2901);
                glTexParameteri(0xde1, 0x2803, 0x2901);
            }
            if (image->hasMipmaps) {
                if (Engine::AnisotropyValue > 0.0f) {
                    glTexParameterf(0xde1, 0x84fe, Engine::AnisotropyValue);
                    glTexParameteri(0xde1, 0x2801, 0x2703);
                } else {
                    glTexParameteri(0xde1, 0x2801, 0x2703);
                    glTexParameteri(0xde1, 0x2800, 0x2601);
                }
            } else if (engine->linearFilterFlag) {
                glTexParameteri(0xde1, 0x2800, 0x2601);
                glTexParameteri(0xde1, 0x2801, 0x2601);
            } else {
                glTexParameteri(0xde1, 0x2800, 0x2600);
                glTexParameteri(0xde1, 0x2801, 0x2600);
            }
            if (Engine::clampTextures) {
                glTexParameteri(0xde1, 0x2800, 0x2601);
                glTexParameteri(0xde1, 0x2801, 0x2601);
            }

            switch (image->format) {
            case 1:
                glTexImage2D(0xde1, 0, 0x1909, image->width, image->height, 0, 0x1909, 0x1401, image->data);
                if (image->hasMipmaps)
                    glGenerateMipmap(0xde1);
                break;
            case 2:
                glTexImage2D(0xde1, 0, 0x1907, image->width, image->height, 0, 0x1907, 0x1401, image->data);
                if (image->hasMipmaps)
                    glGenerateMipmap(0xde1);
                break;
            case 3:
                glTexImage2D(0xde1, 0, 0x1908, image->width, image->height, 0, 0x1908, 0x1401, image->data);
                if (image->hasMipmaps)
                    glGenerateMipmap(0xde1);
                break;
            case 4: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8c03, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blocks = width < 16 ? 2 : width >> 3;
                    unsigned int rows = height < 8 ? 16 : (height << 1) & ~7u;
                    unsigned int blockBytes = rows * blocks;
                    glCompressedTexImage2D(0xde1, level, 0x8c03, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 5: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8c02, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blocks = height < 8 ? 2 : height >> 2;
                    unsigned int rows = width < 8 ? 16 : (width << 1) & ~7u;
                    unsigned int blockBytes = rows * blocks;
                    glCompressedTexImage2D(0xde1, level, 0x8c02, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 7: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8c93, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blockBytes = width * height;
                    if (blockBytes <= 16)
                        blockBytes = 16;
                    glCompressedTexImage2D(0xde1, level, 0x8c93, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 8:
            case 9:
            case 10: {
                unsigned int format = 0x83f0;
                if (image->format == 9)
                    format = 0x83f2;
                if (image->format == 10)
                    format = 0x83f3;
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, format, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blockBytes = height * width;
                    if (image->format - 9 <= 1) {
                        if ((int) blockBytes <= 16)
                            blockBytes = 16;
                    } else if (blockBytes > 15) {
                        blockBytes >>= 1;
                    } else {
                        blockBytes = image->format == 8 ? 8 : 16;
                    }
                    glCompressedTexImage2D(0xde1, level, format, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            case 11: {
                unsigned int width = image->width;
                unsigned int height = image->height;
                if (!image->hasMipmaps) {
                    glCompressedTexImage2D(0xde1, 0, 0x8d64, width, height, 0, image->dataLen, image->data);
                    break;
                }
                
                unsigned int level = 0;
                for (unsigned int offset = 0; offset < image->dataLen; level++) {
                    unsigned int blockBytes = (width * height) >> 1;
                    if (width < 8)
                        blockBytes = 8;
                    glCompressedTexImage2D(0xde1, level, 0x8d64, width, height, 0, blockBytes, image->data + offset);
                    unsigned int nextHeight = height >> 1 > 1 ? height >> 1 : 1;
                    unsigned int nextWidth = width >> 1 > 1 ? width >> 1 : 1;
                    offset += blockBytes;
                    height = nextHeight;
                    width = nextWidth;
                }
                break;
            }
            default:
                break;
            }
        }

        if (registerTexture) {
            if (outTex != 0) {
                outTex->valid = 1;
                outTex->isCube = image->format == 6;
                if (glGetError() != 0) {
                    outTex->glId = 0xffffffff;
                    outTex->byteSize = 0;
                    glDeleteTextures(1, outIds);
                } else {
                    AELabelObject(0x1702, *outIds, path);
                    outTex->glId = *outIds;
                    outTex->byteSize = image->dataLen;
                    ++Engine::ImageCount;
                }
                engine->textureByteCounter += outTex->byteSize;
            } else {
                AELoadedTexture *texture = new AELoadedTexture;
                texture->scale = scale;
                texture->valid = 1;
                texture->isCube = image->format == 6;
                texture->name.Set(path);
                if (glGetError() != 0) {
                    glDeleteTextures(1, outIds);
                    texture->byteSize = 0;
                    texture->glId = 0xffffffff;
                } else {
                    AELabelObject(0x1702, *outIds, path);
                    texture->glId = *outIds;
                    texture->byteSize = image->dataLen;
                    ++Engine::ImageCount;
                }
                engine->textureByteCounter += texture->byteSize;
                ArrayAdd(texture, engine->appManager->paintCanvas->cubeTextures);
                *outIds = engine->appManager->paintCanvas->cubeTextures.count - 1;
            }
        } else {
            engine->lastGlError = glGetError();
            if (engine->lastGlError != 0) {
                engine->lastErrorPath = String(path);
                glDeleteTextures(1, outIds);
                ImageRelease(&image);
                return -4;
            }
            AELabelObject(0x1702, *outIds, path);
        }

        ImageRelease(&image);
        return 1;
    }
}
