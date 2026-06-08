#include "class.h"

// AbyssEngine::ImageCreateFromFile(Engine*, char const*, Image**)
// Allocate an Image (0x14 bytes), open the .aei file, validate the 8-byte magic, read the
// header (type byte, width, height, palette count) and then the pixel payload. The pixel
// format byte selects how the payload size/format is decoded. Returns 1 on success, -1 on a
// read/format error (releasing the Image), or -4 on invalid arguments.
//
// Image layout: +0x00 width(u16), +0x02 height(u16), +0x04 format(u32), +0x08 hasMipmaps(u8),
//               +0x0c data ptr, +0x10 data size(u32).
namespace AbyssEngine {

extern "C" {
void *AE_operator_new(size_t_ n);
void *AE_operator_new_arr(size_t_ n);
void AE_operator_delete(void *p);
int  AEFile_OpenRead(const char *path, void *handleOut, void *unused);
int  AEFile_Read(unsigned int n, void *dst, unsigned int handle);
int  AEFile_Skip(unsigned int n, unsigned int handle);
void AEFile_Close(unsigned int handle);
}

void ImageRelease(Image **slot);

int ImageCreateFromFile(Engine *engine, char *path, Image **out)
{
    if (engine == 0 || path == 0)
        return -4;

    char *img = (char *)AE_operator_new(0x14);
    u8(img, 0x8) = 0;
    u32(img, 0x0) = 0;
    u32(img, 0x4) = 0;
    u32(img, 0xc) = 0;
    *out = (Image *)img;

    unsigned int handle = 0;
    if (AEFile_OpenRead(path, &handle, &handle) == 0) {
        if (*out != 0)
            AE_operator_delete((void *)*out);
        *out = 0;
        return -1;
    }

    // Validate "AEImage"-style 8-byte magic.
    static const char magic[8] = {'*', '*', '*', '*', 0, 0, 0, 0};
    char hdr[8];
    for (int i = 0; i < 4; ++i) hdr[i] = '*';
    if (AEFile_Read(8, hdr, handle) == 0)
        goto fail;
    for (unsigned int k = 0; k < 8; ++k) {
        if (magic[k] != hdr[k])
            goto fail;
    }

    {
        char *im = (char *)*out;
        unsigned char fmt = 0;
        unsigned short palCount = 0;
        if (AEFile_Read(1, &fmt, handle) == 0) goto fail;
        if (AEFile_Read(2, im + 0x0, handle) == 0) goto fail;      // width
        if (AEFile_Read(2, im + 0x2, handle) == 0) goto fail;      // height
        if (AEFile_Read(2, &palCount, handle) == 0) goto fail;     // palette entries

        AEFile_Skip((unsigned int)palCount << 3, handle);

        if (fmt & 2)
            u8(im, 0x8) = 1; // mipmapped

        unsigned int dataLen = 0;
        switch (fmt) {
        case 1:
        case 3:
        case 0x81: {
            unsigned int sz = (unsigned int)u16(im, 0x0) * (unsigned int)u16(im, 0x2) * 4;
            void *p = AE_operator_new_arr(sz);
            pp(im, 0xc) = p;
            im = (char *)*out;
            if (AEFile_Read(sz, pp(im, 0xc), handle) == 0) goto fail;
            im = (char *)*out;
            u32(im, 0x10) = sz;
            {
                unsigned int v = (unsigned int)(int)(signed char)fmt;
                unsigned int code = ((int)v < 0) ? ((v > 0x7fffffff) ? 6u : v) : 3u;
                if ((int)v >= 0) code = 3;
                u32(im, 0x4) = code;
            }
            break;
        }
        case 0xd:
        case 0xf:
            if (AEFile_Read(4, &dataLen, handle) == 0) goto fail;
            pp(im, 0xc) = AE_operator_new_arr(dataLen);
            if (AEFile_Read(dataLen, pp(im, 0xc), handle) == 0) goto fail;
            u32(im, 0x4) = 4;
            u32(im, 0x10) = dataLen;
            break;
        case 0x10:
        case 0x12:
            if (AEFile_Read(4, &dataLen, handle) == 0) goto fail;
            pp(im, 0xc) = AE_operator_new_arr(dataLen);
            if (AEFile_Read(dataLen, pp(im, 0xc), handle) == 0) goto fail;
            u32(im, 0x4) = 5;
            u32(im, 0x10) = dataLen;
            break;
        case 0x11:
        case 0x13:
            if (AEFile_Read(4, im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 7;
            pp(im, 0xc) = AE_operator_new_arr(u32(im, 0x10));
            if (AEFile_Read(u32(im, 0x10), pp(im, 0xc), handle) == 0) goto fail;
            break;
        case 0x14:
        case 0x16:
        case 0x17:
        case 0x40:
        case 0x42:
            if (fmt == 0x40)
                u8(im, 0x8) = 0;
            if (AEFile_Read(4, &dataLen, handle) == 0) goto fail;
            pp(im, 0xc) = AE_operator_new_arr(dataLen);
            if (AEFile_Read(dataLen, pp(im, 0xc), handle) == 0) goto fail;
            u32(im, 0x4) = 0xb;
            u32(im, 0x10) = dataLen;
            if (fmt == 0x17)
                u8(im, 0x8) = 0;
            break;
        case 0x20:
        case 0x22:
            if (AEFile_Read(4, im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 8;
            pp(im, 0xc) = AE_operator_new_arr(u32(im, 0x10));
            if (AEFile_Read(u32(im, 0x10), pp(im, 0xc), handle) == 0) goto fail;
            break;
        case 0x21:
        case 0x23:
            if (AEFile_Read(4, im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 9;
            pp(im, 0xc) = AE_operator_new_arr(u32(im, 0x10));
            if (AEFile_Read(u32(im, 0x10), pp(im, 0xc), handle) == 0) goto fail;
            break;
        case 0x24:
        case 0x26:
            if (AEFile_Read(4, im + 0x10, handle) == 0) goto fail;
            u32(im, 0x4) = 10;
            pp(im, 0xc) = AE_operator_new_arr(u32(im, 0x10));
            if (AEFile_Read(u32(im, 0x10), pp(im, 0xc), handle) == 0) goto fail;
            break;
        default:
            break;
        }

        AEFile_Close(handle);
        return 1;
    }

fail:
    ImageRelease(out);
    AEFile_Close(handle);
    return -1;
}

} // namespace AbyssEngine
