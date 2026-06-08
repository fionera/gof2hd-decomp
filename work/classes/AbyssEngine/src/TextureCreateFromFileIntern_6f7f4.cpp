#include "class.h"

// AbyssEngine::TextureCreateFromFileIntern(Engine*, char const* path,
//     void(*cb)(Image*,void*), void* user, unsigned int* outIds, float aniso,
//     AELoadedTexture* outTex, bool ...)
// Load an image from file and upload it as an OpenGL texture. Handles cubemaps (format 6) and
// the full set of (compressed and uncompressed) 2D formats, generating mip levels for the
// compressed paths. Filtering/wrap parameters are chosen from engine/global flags. On success
// the texture id is returned in *outIds (or recorded into an AELoadedTexture cache entry).
// Returns 1 on success, -4 on a GL upload error.
namespace AbyssEngine {

extern "C" {
unsigned int glGetError();
void glGenTextures(int n, unsigned int *ids);
void glDeleteTextures(int n, const unsigned int *ids);
void glBindTexture(unsigned int target, unsigned int id);
void glPixelStorei(unsigned int pname, int param);
void glTexEnvi(unsigned int t, unsigned int p, int v);
void glTexParameteri(unsigned int target, unsigned int pname, int param);
void glTexParameterf(unsigned int target, unsigned int pname, float param);
void glTexImage2D(unsigned int t, int lvl, int ifmt, int w, int h, int b, int fmt, int type, const void *px);
void glCompressedTexImage2D(unsigned int t, int lvl, int ifmt, int w, int h, int b, int sz, const void *px);
void glGenerateMipmap(unsigned int target);
int  __aeabi_uidiv(int a, int b);

void *AE_operator_new(size_t_ n);
void  AE_String_ctor(void *self, const char *s, bool b);
void  AE_String_assign(void *dst, void *src);
void  AE_String_dtor(void *self);
void  AE_String_Set(void *self, const char *s);
void  AELabelObject(unsigned int type, unsigned int id, const char *name);
void  AE_ArrayAdd_TexPtr(void *item, void *arr);

char *g_cubemapEnabledFlag;     // **(DAT + 0x7f860)
char *g_texEnvFlag;             // **(DAT + 0x7f998)
char *g_clampFlag;              // **(DAT + 0x7f9b2)
float *g_anisoMaxPtr;           // **(DAT + 0x7fa00)
char *g_labelObjectsFlag;       // **(DAT + 0x7c9d2) (reused)
}

int  ImageCreateFromFile(Engine *engine, char *path, Image **out);
void ImageRelease(Image **slot);

// Helper to upload one compressed mip chain for the simple formats.
static void uploadCompressedChain(char *img, unsigned int glFmt, int divShift, int minBlock)
{
    unsigned int w = (unsigned int)u16(img, 0x0);
    unsigned int h = (unsigned int)u16(img, 0x2);
    int level = 0;
    for (unsigned int off = 0; off < u32(img, 0x10);) {
        unsigned int blockSz = (w * h) >> divShift;
        if ((int)blockSz < minBlock)
            blockSz = (unsigned int)minBlock;
        glCompressedTexImage2D(0xde1, level, glFmt, (int)w, (int)h, 0, (int)blockSz,
                               (char *)pp(img, 0xc) + off);
        off += blockSz;
        unsigned int nw = 1, nh = 1;
        if (h >> 1 > 1) nh = h >> 1;
        if (w >> 1 > 1) nw = w >> 1;
        ++level;
        w = nw;
        h = nh;
    }
}

int TextureCreateFromFileIntern(Engine *engine, char *path, void (*cb)(Image *, void *),
                                void *user, unsigned int *outIds, float aniso,
                                AELoadedTexture *outTex, bool /*flag*/)
{
    char *en = (char *)engine;
    Image *imgPtr = 0;
    *outIds = 0;
    u32(en, 0x10) = glGetError();

    if (ImageCreateFromFile(engine, path, &imgPtr) != 1)
        return -4;
    if (cb != 0)
        cb(imgPtr, user);

    glGenTextures(1, outIds);
    char *img = (char *)imgPtr;
    int format = (int)u32(img, 0x4);

    if (format == 6) {
        // Cubemap: six square faces packed vertically.
        if (*g_cubemapEnabledFlag != 0) {
            glBindTexture(0x8513, *outIds);
            glTexParameteri(0x8513, 0x2800, 0x2601);
            glTexParameteri(0x8513, 0x2801, 0x2601);
            int faceH = __aeabi_uidiv((int)u16(img, 0x2), 6);
            unsigned int w = u16(img, 0x0);
            const unsigned int faces[6] = {0x8517, 0x8516, 0x8519, 0x8515, 0x851a, 0x8518};
            int faceBytes = faceH * (int)w;
            for (int fi = 0; fi < 6; ++fi) {
                glTexImage2D(faces[fi], 0, 0x1908, (int)w, faceH, 0, 0x1908, 0x1401,
                             (char *)pp(img, 0xc) + faceBytes * 4 * fi);
            }
        }
    } else {
        glBindTexture(0xde1, *outIds);
        glPixelStorei(0xcf5, 1);
        if (*g_texEnvFlag == 0)
            glTexEnvi(0x2300, 0x2200, 0x2100);

        int wrap;
        if (*g_clampFlag != 0) {
            glTexParameteri(0xde1, 0x2802, 0x812f);
            wrap = 0x812f;
        } else {
            glTexParameteri(0xde1, 0x2802, 0x2901);
            wrap = 0x2901;
        }
        glTexParameteri(0xde1, 0x2803, wrap);

        // Min/mag filters depend on mipmap availability and anisotropy settings.
        if (u8(img, 0x8) == 0) {
            if (u8(en, 0x20) != 0) {
                glTexParameteri(0xde1, 0x2800, 0x2601);
                glTexParameteri(0xde1, 0x2801, 0x2601);
            } else {
                glTexParameteri(0xde1, 0x2800, 0x2600);
                glTexParameteri(0xde1, 0x2801, 0x2600);
            }
        } else if (*g_anisoMaxPtr <= 0.0f) {
            glTexParameteri(0xde1, 0x2801, 0x2703);
            glTexParameteri(0xde1, 0x2800, 0x2601);
        } else {
            glTexParameterf(0xde1, 0x84fe, *g_anisoMaxPtr);
            glTexParameteri(0xde1, 0x2801, 0x2703);
        }

        if (*g_clampFlag != 0) {
            glTexParameteri(0xde1, 0x2800, 0x2601);
            glTexParameteri(0xde1, 0x2801, 0x2601);
        }

        int w = (int)u16(img, 0x0);
        int h = (int)u16(img, 0x2);
        switch (format) {
        case 1:
            glTexImage2D(0xde1, 0, 0x1909, w, h, 0, 0x1909, 0x1401, pp(img, 0xc));
            if (u8(img, 0x8) != 0) glGenerateMipmap(0xde1);
            break;
        case 2:
            glTexImage2D(0xde1, 0, 0x1907, w, h, 0, 0x1907, 0x1401, pp(img, 0xc));
            if (u8(img, 0x8) != 0) glGenerateMipmap(0xde1);
            break;
        case 3:
            glTexImage2D(0xde1, 0, 0x1908, w, h, 0, 0x1908, 0x1401, pp(img, 0xc));
            if (u8(img, 0x8) != 0) glGenerateMipmap(0xde1);
            break;
        case 4: // ETC1 / DXT1-ish
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8c03, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8c03, 1, 0x10);
            break;
        case 5:
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8c02, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8c02, 1, 0x10);
            break;
        case 7:
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8c93, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8c93, 0, 0x10);
            break;
        case 8:
        case 9:
        case 10: {
            unsigned int glFmt = 0x83f0;
            if (format == 9) glFmt = 0x83f2;
            if (format == 10) glFmt = 0x83f3;
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, glFmt, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, glFmt, 1, 0x10);
            break;
        }
        case 0xb:
            if (u8(img, 0x8) == 0)
                glCompressedTexImage2D(0xde1, 0, 0x8d64, w, h, 0, (int)u32(img, 0x10), pp(img, 0xc));
            else
                uploadCompressedChain(img, 0x8d64, 1, 8);
            break;
        default:
            break;
        }
    }

    // Verify upload and either label the texture or cache it.
    if (outTex == 0) {
        int err = (int)glGetError();
        u32(en, 0x10) = (unsigned int)err;
        if (err != 0) {
            char tmp[16];
            AE_String_ctor(tmp, path, false);
            AE_String_assign(en + 0x14, tmp);
            AE_String_dtor(tmp);
            glDeleteTextures(1, outIds);
            ImageRelease(&imgPtr);
            return -4;
        }
        AELabelObject(0x1702, *outIds, path);
    } else {
        char *tex = (char *)outTex;
        u8(tex, 0x15) = 1;
        u8(tex, 0x14) = (format == 6) ? 1 : 0;
        if (glGetError() == 0) {
            AELabelObject(0x1702, *outIds, path);
            u32(tex, 0x0) = *outIds;
            u32(tex, 0x18) = u32(img, 0x10);
        } else {
            u32(tex, 0x0) = 0xffffffff;
            u32(tex, 0x18) = 0;
            glDeleteTextures(1, outIds);
        }
        i32(en, 0x70) += (int)u32(tex, 0x18);
    }

    ImageRelease(&imgPtr);
    return 1;
}

} // namespace AbyssEngine
