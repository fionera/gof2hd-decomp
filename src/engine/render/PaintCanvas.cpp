#include "engine/render/PaintCanvas.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GLES/gl.h>
#include "engine/core/AbyssEngine.h"
#include "engine/render/AELoadedTexture.h"
#include "engine/render/Material.h"
#include "engine/core/Array.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "engine/core/Node.h"
#include "engine/math/Transform.h"
#include "engine/render/ResourceTexture.h"
#include "engine/render/ResourceMesh.h"
#include "engine/render/ResourceTransform.h"
#include "engine/render/Camera.h"
#include <cstdint>
#include <cstddef>

namespace AbyssEngine { class Engine; class Mesh; void MeshDraw(Engine*, Mesh*); void MeshRelease(Engine*, Mesh**); void ImageFontRelease(Engine*, ImageFont**); void Image2DRelease(Engine*, Image2D**); void SpriteSystemRelease(Engine*, SpriteSystem**); }
namespace AbyssEngine {
    typedef void (*ImageCallback)(Image *, void *); // lint: void_ptr (exported TextureCreateFromFile signature)
    int TextureCreateFromFile(Engine *engine, const char *path, ImageCallback cb, void *user, unsigned int *outIds, bool flag, float scale); // lint: void_ptr (exported TextureCreateFromFile signature)
    int TextureCreateFromFileIntern(Engine *engine, const char *path, ImageCallback cb, void *user, unsigned int *outIds, float scale, AELoadedTexture *outTex, bool flag); // lint: void_ptr (exported TextureCreateFromFileIntern signature)
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triangleCount,
                   signed char meshType, Mesh **out);
    int MeshCreateFromFile(Engine *engine, const char *path, Mesh **out, Material *mat);
    int ImageCreateRegionFromFile(Engine *engine, const char *path, unsigned short index, Image2D *region);
    int MeshConvertToVBO(Mesh *mesh);
    void CameraSetPerspective(float a, float b, float c, float width, float height,
                               Camera *camera);
    void CameraSetPerspective(float fov, float aspect, float width, float height,
                               Camera *camera);
    int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius, AEMath::Matrix *view,
                                    Camera *camera);
    int CameraIsPointinViewFrustum(const AEMath::Vector &point, AEMath::Matrix *view, Camera *camera);
    void MaterialDraw(PaintCanvas *canvas, Engine *engine, Material *mat, bool setTextures);
    PickedTextureRegion MeshIntersect(float qx, float qz, Mesh *mesh);
}

namespace AbyssEngine {
    class SpriteSystem;
    class ImageFont;
    int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int count);
    int ImageFontGetWidth(ImageFont *font, const unsigned short *str, unsigned int start, unsigned int count);
    int ImageFontCheckString(ImageFont *font, const unsigned short *str, unsigned int count);
    void ImageFontDrawString(ImageFont *font, const unsigned short *str, unsigned int len, int x, int y, PaintCanvas *canvas, Engine *engine, bool flag);
    void ImageFontDrawString(ImageFont *font, const unsigned short *str, int x, int y, PaintCanvas *canvas, Engine *engine, bool flag);
    int ImageFontGetHeight(ImageFont *font);
    short ImageFontGetSpacing(ImageFont *font);
    void ImageFontSetSpacing(ImageFont *font, short spacing);
    short ImageFontGetYOffset(ImageFont *font);
    void ImageFontSetYOffset(ImageFont *font, short yOffset);
    int ImageCreateFontFromFile(Engine *engine, const char *path, unsigned short index, ImageFont **out);
    void SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem *sys);
    void SpriteSystemSetUv(unsigned short idx, float a, float b, float c, float d, SpriteSystem *sys);
    void SpriteSystemSetRGBA(unsigned short idx, float r, float g, float b, float a, SpriteSystem *sys);
    void SpriteSystemDraw(Engine *engine, const AEMath::Matrix &view, const AEMath::Matrix &world, SpriteSystem *sys);
}

namespace {
    struct PCMeshView;
    struct PCRegionView;
    struct PCTransformView;

    struct PCImage2DView {
        PCMeshView *mesh;
        uint32_t textureId;
        uint16_t regionA;
        uint16_t regionB;
        uint16_t u0;
        uint16_t v0;
        uint16_t width;
        uint16_t height;
        uint8_t restoreFlag;

        PCImage2DView()
                : mesh(0), textureId(0), regionA(0), regionB(0), u0(0), v0(0), width(0), height(0),
                  restoreFlag(0) {
        }
    };

    struct PCRegionView {
        uint8_t pad0[0x0c];
        float width;
        uint8_t pad1[0x1c - 0x10];
        float height;
    };

#pragma pack(push, 1)
    struct PCMeshView {
        uint16_t _half0;
        uint16_t vertexCount;
        char *positions;
        char *uvs;
        char *colors;
        char *normals;
        char *tangents;
        char *bitangents;
        uint8_t pad1c[0x28 - 0x1c];
        uint16_t indexCount;
        uint16_t triCapacity;
        char *indexBuffer;
        AbyssEngine::Material *material;
        PCTransformView *materialRes;
        uint8_t pad38[0x3c - 0x38];
        uint8_t bounds3c;
        uint8_t pad3d[0x7c - 0x3d];
        int triCountContribution;
    };
#pragma pack(pop)

    struct PCSpriteSystemView {
        int16_t count;
        uint8_t pad02[0x04 - 0x02];
        char *positions;
        char *sizes;
        uint8_t uniformSize;
    };

    struct PCSpriteSystemView2 {
        uint8_t pad00[0x10];
        ::Node *node;
    };

    struct PCResourceView {
        uint16_t id;
        uint8_t pad02[0x04 - 0x02];
        int type;
        int handle;
        char *payload;
    };

    // Material resource payload: texture resource ids followed by the Material fields to copy.
    struct PCMaterialInfoView {
        uint16_t textureIds[8];
        int blendMode;
        unsigned char *addData;
        int addDataSize;
        AbyssEngine::AEMath::Vector ambientColor;
    };

    struct PCMeshInfoView {
        char *path;
        uint16_t matResId;
    };

    struct PCMaterialView {
        uint32_t textureSlots[8];
        uint32_t flags0;
        uint32_t flags1;
        uint32_t flags2;
        uint8_t pad2c[0x68 - 0x2c];
        float vec[3];
    };

    struct PCMaterialIdView {
        uint8_t pad00[0x04];
        uint16_t materialId;
    };

    struct PCCameraView {
        uint32_t param0;
        float param1;
        float param2;
        float localMatrix[15];
        float factor48;
        float factor4c;
    };

    struct PCCameraParamsView {
        uint32_t raw[3];
    };

    struct PCCameraEyeView {
        uint8_t pad00[0x18];
        float eyeX;
        uint8_t pad1c[0x28 - 0x1c];
        float eyeY;
        uint8_t pad2c[0x38 - 0x2c];
        float eyeZ;
    };

    struct PCTransformInfoView {
        uint8_t pad00[0x3c];
        uint16_t childMeshCount;
        uint8_t pad3e[0x40 - 0x3e];
        char *childMeshIds;
        uint16_t childTfCount;
        uint8_t pad46[0x48 - 0x46];
        char *childTfIds;
    };

#pragma pack(push, 1)
    struct PCTransformView {
        uint8_t pad00[0x3c];
        uint32_t meshCount;
        char **meshData;
        uint32_t meshCap;
        uint32_t color;
        uint32_t childCount;
        char **childData;
        uint8_t pad54[0xd4 - 0x54];
        uint8_t bsphere;
        uint8_t padd5[0xec - 0xd5];
        uint8_t visible;
        uint8_t paded[0xf8 - 0xed];
        int64_t animLength;
        int64_t animStart;
    };
#pragma pack(pop)

    struct PCCubeTexView {
        int glTexId;
        char pathField[0x10 - 0x04];
        float scale;
        uint8_t restoreFlag;
        uint8_t pad15[0x18 - 0x15];
        int memSize;
    };

    struct PCFontView {
        uint16_t key;
        uint8_t pad02[0x08 - 0x02];
        int texture;  // 0x08: texture handle of the glyph atlas
    };

    struct PCGravView {
        uint8_t pad00[0x08];
        double angle;
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(sizeof(PCSpriteSystemView) >= 0x0d, "sprite view");
    static_assert(offsetof(PCImage2DView, restoreFlag) == 0x14, "img restore");
    static_assert(offsetof(PCImage2DView, width) == 0x10, "img width");
    static_assert(offsetof(PCMeshView, vertexCount) == 0x02, "mesh vtxcount");
    static_assert(offsetof(PCMeshView, positions) == 0x04, "mesh pos");
    static_assert(offsetof(PCMeshView, indexCount) == 0x28, "mesh idxcount");
    static_assert(offsetof(PCMeshView, triCapacity) == 0x2a, "mesh tricap");
    static_assert(offsetof(PCMeshView, material) == 0x30, "mesh material");
    static_assert(offsetof(PCMeshView, materialRes) == 0x34, "mesh materialRes");
    static_assert(offsetof(PCMeshView, triCountContribution) == 0x7c, "mesh tri");
    static_assert(offsetof(PCSpriteSystemView, positions) == 0x04, "ss pos");
    static_assert(offsetof(PCSpriteSystemView, sizes) == 0x08, "ss size");
    static_assert(offsetof(PCSpriteSystemView, uniformSize) == 0x0c, "ss flag");
    static_assert(offsetof(PCResourceView, type) == 0x04, "res type");
    static_assert(offsetof(PCResourceView, handle) == 0x08, "res handle");
    static_assert(offsetof(PCResourceView, payload) == 0x0c, "res payload");
    static_assert(offsetof(PCMaterialView, flags0) == 0x20, "mat flags0");
    static_assert(offsetof(PCMaterialView, vec) == 0x68, "mat vec");
    static_assert(offsetof(PCCameraView, localMatrix) == 0x0c, "cam local");
    static_assert(offsetof(PCCameraView, factor48) == 0x48, "cam f48");
    static_assert(offsetof(PCCameraView, factor4c) == 0x4c, "cam f4c");
    static_assert(offsetof(PCCameraEyeView, eyeX) == 0x18, "cam eyeX");
    static_assert(offsetof(PCCameraEyeView, eyeZ) == 0x38, "cam eyeZ");
    static_assert(offsetof(PCTransformInfoView, childMeshCount) == 0x3c, "tfi mc");
    static_assert(offsetof(PCTransformInfoView, childTfIds) == 0x48, "tfi tf");
    static_assert(offsetof(PCTransformView, meshCount) == 0x3c, "tf mc");
    static_assert(offsetof(PCTransformView, bsphere) == 0xd4, "tf bsphere");
    static_assert(offsetof(PCTransformView, childCount) == 0x4c, "tf childcount");
    static_assert(offsetof(PCTransformView, visible) == 0xec, "tf vis");
    static_assert(offsetof(PCTransformView, animLength) == 0xf8, "tf animlen");
    static_assert(offsetof(PCTransformView, animStart) == 0x100, "tf animstart");
    static_assert(offsetof(PCCubeTexView, scale) == 0x10, "cube scale");
    static_assert(offsetof(PCCubeTexView, restoreFlag) == 0x14, "cube restore");
    static_assert(offsetof(PCCubeTexView, memSize) == 0x18, "cube mem");
    static_assert(offsetof(PCFontView, texture) == 0x08, "font texture");
    static_assert(offsetof(PCGravView, angle) == 0x08, "grav angle");
    static_assert(offsetof(PCRegionView, height) == 0x1c, "region h");
#endif
}

namespace {
    struct PCArrayHeader {
        uint32_t count;
        char *data;
        uint32_t capacity;
    };

    inline void PCArrayCtor(PCArrayHeader *arrayHeader) {
        PCArrayHeader *a = arrayHeader;
        a->count = 0;
        a->data = nullptr;
        a->capacity = 0;
    }

    template<class T>
    inline void PCArrayAdd(T item, PCArrayHeader *arrayHeader) {
        PCArrayHeader *a = arrayHeader;
        if (a->count >= a->capacity) {
            uint32_t newCap = a->count + 1;
            T *grown = (T *) ::operator new((size_t) newCap * sizeof(T));
            for (uint32_t i = 0; i < a->count; ++i)
                grown[i] = ((T *) a->data)[i];
            ::operator delete(a->data);
            a->data = (char *) grown;
            a->capacity = newCap;
        }
        ((T *) a->data)[a->count] = item;
        a->count += 1;
    }

    inline void PCArrayRemoveAll(PCArrayHeader *arrayHeader) {
        PCArrayHeader *a = arrayHeader;
        a->count = 0;
    }
}

void paintcanvas_ext_has_vibration(void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_sprite_allsize(unsigned int, void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_change_mat(void *, void *, void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_mat_intern(void *, void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_camera_apply(void *, unsigned int, unsigned int, unsigned int); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_convert_vbo(void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_add_resource(void *, void *); // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_set_wvm2(void *self, void *m) { ((PaintCanvas*)self)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_meshdraw(void *eng, void *mesh) { AbyssEngine::MeshDraw((AbyssEngine::Engine*)eng, (AbyssEngine::Mesh*)mesh); } // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_draw_mesh(void *, void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_get_grav(void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_set_reslist(AbyssEngine::Resource * const *, unsigned int, void *); // lint: void_ptr (external symbol; mangling must match lib)

namespace AbyssEngine {
    int SpriteSystemCreate(Engine *engine, unsigned short count, bool sharedSize, SpriteSystem **out);
}

void paintcanvas_ext_get_accel(void *); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dr2_restore(unsigned int flag, void *img); // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_dr2_settexture(void *self, unsigned int tex) { ((PaintCanvas*)self)->SetTexture(tex, 0xffffffff); } // lint: void_ptr (external symbol; mangling must match lib)

static inline float paintcanvas_ext_dr2_signedtofloat(int v, unsigned int mode) { (void)mode; return (float)v; }

static inline float paintcanvas_ext_dr2_sinf(float v) { return sinf(v); }

static inline float paintcanvas_ext_dr2_cosf(float v) { return cosf(v); }

void paintcanvas_ext_dr2_setscaling(void *out, float x, float y, float z); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dr2_mtx_mul(void *out, void *a, void *b); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dr2_mtx_assign(void *dst, void *src); // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_dr2_setwvm(void *self, void *m) { ((PaintCanvas*)self)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_dr2_gldisable(unsigned int cap) { glDisable(cap); }

static inline void paintcanvas_ext_dr2_meshdraw(void *eng, void *mesh) { AbyssEngine::MeshDraw((AbyssEngine::Engine*)eng, (AbyssEngine::Mesh*)mesh); }

static inline void paintcanvas_ext_dr2_glenable(unsigned int cap) { glEnable(cap); }

static inline void paintcanvas_ext_dm_memcpy(void *dst, const void *src, unsigned int n) { memcpy(dst, src, n); } // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dm_settrans(void *out, float v); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dm_getpos(void *out); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dm_settrans_vec(void *out, void *vec); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dm_transformvec(void *m, void *vec); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dm_rotatevec(void *m, void *vec); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dm_vec_assign(void *dst, const void *src); // lint: void_ptr (external symbol; mangling must match lib)

int paintcanvas_ext_dm_spherefrustum(void *self, void *pt, float radius); // lint: void_ptr (external symbol; mangling must match lib)

static inline float paintcanvas_ext_dm_unsignedtofloat(unsigned int v, unsigned int mode) { (void)mode; return (float)v; }

static inline void paintcanvas_ext_dm_setwvm(void *self, void *m) { ((PaintCanvas*)self)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dm_setmodelmatrix(void *eng); // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_dm_meshdraw(void *eng, void *mesh) { AbyssEngine::MeshDraw((AbyssEngine::Engine*)eng, (AbyssEngine::Mesh*)mesh); } // lint: void_ptr (external symbol; mangling must match lib)















static inline void paintcanvas_ext_dr3_settexture(void *self, unsigned int tex) { ((PaintCanvas*)self)->SetTexture(tex, 0xffffffff); } // lint: void_ptr (external symbol; mangling must match lib)

static inline float paintcanvas_ext_dr3_signedtofloat(int v, unsigned int mode) { (void)mode; return (float)v; }

static inline float paintcanvas_ext_dr3_unsignedtofloat(unsigned int v, unsigned int mode) { (void)mode; return (float)v; }

static inline float paintcanvas_ext_dr3_sinf(float v) { return sinf(v); }

static inline float paintcanvas_ext_dr3_cosf(float v) { return cosf(v); }

void paintcanvas_ext_dr3_mtx_mul(void *out, void *m); // lint: void_ptr (external symbol; mangling must match lib)

void paintcanvas_ext_dr3_mtx_assign(void *dst, void *src); // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_dr3_setwvm(void *self, void *m) { ((PaintCanvas*)self)->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix*)m); } // lint: void_ptr (external symbol; mangling must match lib)

static inline void paintcanvas_ext_dr3_gldisable(unsigned int cap) { glDisable(cap); }

static inline void paintcanvas_ext_dr3_meshdraw(void *eng, void *mesh) { AbyssEngine::MeshDraw((AbyssEngine::Engine*)eng, (AbyssEngine::Mesh*)mesh); }

static inline void paintcanvas_ext_dr3_glenable(unsigned int cap) { glEnable(cap); }

static inline int pc_GetWidth(AbyssEngine::PaintCanvas *self) { return self->GetWidth(); }
static inline int pc_GetHeight(AbyssEngine::PaintCanvas *self) { return self->GetHeight(); }

void pc_Camera_ctor(void *cam, float h, float w); // lint: void_ptr (external symbol; mangling must match lib)

void pc_ArrayAdd_Camera(void *cam, void *arr); // lint: void_ptr (external symbol; mangling must match lib)

int PaintCanvas::GetImage2DWidth(unsigned int index) {
    if (index < this->images.count) {
        PCImage2DView *img = (PCImage2DView *) (this->images.data_)[index];
        return img->width;
    }
    return 0;
}

unsigned int PaintCanvas::CameraGetCurrent() {
    unsigned int cur = this->currentCamera;
    if (cur >= this->cameras.count) {
        cur = 0xffffffff;
    }
    return cur;
}

void PaintCanvas::HasVibration() {
    return paintcanvas_ext_has_vibration(this->engine);
}

float *PaintCanvas::CameraGetLocal(unsigned int index) {
    float *result;
    if (index < this->cameras.count) {
        result = ((PCCameraView *) (this->cameras.data_)[index])->localMatrix;
    } else {
        char tmp[60];
        result = (float *) &this->identityMatrix;
        AbyssEngine::AEMath::MatrixIdentity(
            *reinterpret_cast<AbyssEngine::AEMath::Matrix *>(result));
    }
    return result;
}

void PaintCanvas::DrawTextLines(unsigned int font,
                                Array<AbyssEngine::String *> *arr, int x, int y) {
    this->DrawTextLines(font, arr, x, y, false);
}

void PaintCanvas::DrawTextLines(unsigned int font,
                                Array<AbyssEngine::String *> *arr, int x, int y, bool center) {
    int xoff = 0;
    for (unsigned int i = 0; i < arr->size(); i++) {
        if (center) {
            int w = this->GetTextWidth(font, *arr->data()[i]);
            xoff = -(w >> 1);
        }
        this->DrawString(font, *arr->data()[i], xoff + x, y, false);
        y += this->GetTextHeight(font);
    }
}

void PaintCanvas::FillRectangle(int x, int y, int w, int h) {
    float fx = (float) x;
    float fy = (float) y;
    float fx2 = (float) (x + w);
    float fy2 = (float) (h + y);

    float *vb = (float *) ((PCMeshView *) this->lineMesh)->positions;
    vb[0] = fx;
    vb[1] = fy;
    vb[3] = fx2;
    vb[4] = fy;
    vb[6] = fx2;
    vb[7] = fy2;
    vb[9] = fx;
    vb[10] = fy2;

    this->SetWorldViewMatrix(AbyssEngine::AEMath::Matrix());
    this->engine->GlEnable(0xde1, false);
    AbyssEngine::MeshDraw(this->engine, (AbyssEngine::Mesh *) this->lineMesh);
    this->engine->GlEnable(0xde1, true);
}

void PaintCanvas::SpriteSystemSetRGBA(unsigned int index, unsigned short sub,
                                      float a, float b, float c, float d) {
    if (index < this->spriteSystems.count) {
        AbyssEngine::SpriteSystem *s = (this->spriteSystems.data_)[index];
        if (s) {
            if ((unsigned int) (unsigned short) s->count <= (unsigned int) sub) {
                return;
            }
            return AbyssEngine::SpriteSystemSetRGBA(sub, a, b, c, d, s);
        }
    }
}

void PaintCanvas::TransformCreate(unsigned int &out) {
    AbyssEngine::Transform *obj = new AbyssEngine::Transform();
    ArrayAdd<AbyssEngine::Transform *>(obj, this->transforms);
    out = this->transforms.count - 1;
}

void PaintCanvas::DrawRectangle(int x, int y, int w, int h) {
    double dw = (double) w;
    double dh = (double) h;
    double dy = (double) y;
    double dx = (double) x;
    float right = (float) (dw - 0.5 + dx);
    float bottom = (float) (dh - 0.5 + dy);

    float *v = this->lineVerts;
    v[0] = (float) (dx + 0.5);
    v[1] = (float) (dy + 0.5);
    v[2] = right;
    v[3] = (float) (dy + 0.5);
    v[4] = right;
    v[5] = bottom;
    v[6] = (float) (dx + 0.5);
    v[7] = bottom;

    this->SetWorldViewMatrix(AbyssEngine::AEMath::Matrix());
    if (Engine::enableShader == 0) {
        glLineWidth(1.0f);
        this->engine->GlEnable(0xde1, false);
        glVertexPointer(2, 0x1406, 0, this->lineVerts);
        this->engine->AEClientState(0x8074, true);
        this->engine->AEClientState(0x8078, false);
        this->engine->AEClientState(0x8075, false);
        this->engine->AEClientState(0x8076, false);
        glDrawArrays(2, 0, 4);
        this->engine->GlEnable(0xde1, true);
    } else {
        this->engine->DrawLine2D(this->lineVerts, 4, true);
    }
}

int PaintCanvas::GetImage2DHeight(unsigned int index) {
    if (index < this->images.count) {
        PCImage2DView *img = (PCImage2DView *) (this->images.data_)[index];
        return img->height;
    }
    return 0;
}

void PaintCanvas::MeshSetUv(unsigned int index, unsigned short sub,
                            float u, float v) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if ((unsigned int) sub < (unsigned int) mesh->vertexCount) {
            float *p = (float *) (mesh->uvs + sub * 8);
            p[0] = u;
            if (Engine::enableShader != 0) {
                p[1] = 1.0f - v;
                return;
            }
            p[1] = v;
        }
    }
}

void PaintCanvas::StopDraw2FBO() {
    this->engine->DeactivateRender2TextureFBO();
    this->SetBlendMode((AbyssEngine::BlendMode)0);
    this->engine->DoPostEffect();
    return this->engine->ActivateViewBuffer();
}

void PaintCanvas::SpriteSystemSetAllUv(unsigned int index,
                                       float a, float b, float c, float d) {
    if (this->spriteSystems.count <= index) {
        return;
    }
    AbyssEngine::SpriteSystem *sprite = (this->spriteSystems.data_)[index];
    if (sprite == 0) {
        return;
    }
    return AbyssEngine::SpriteSystemSetAllUv(a, b, c, d, sprite);
}

void PaintCanvas::SpriteSystemGetPosition(unsigned int index, unsigned short sub, Vector &out) {
    if (index < this->spriteSystems.count) {
        PCSpriteSystemView *s = (PCSpriteSystemView *) (this->spriteSystems.data_)[index];
        if (s) {
            if ((unsigned int) (unsigned short) s->count <= (unsigned int) sub) {
                return;
            }
            float *p = (float *) (s->positions + sub * 12);
            out.x = p[0];
            out.y = p[1];
            out.z = p[2];
        }
    }
}

void PaintCanvas::SpriteSystemSetAllSize(unsigned int index, short size) {
    if (index < this->spriteSystems.count) {
        AbyssEngine::SpriteSystem *sprite = (this->spriteSystems.data_)[index];
        if (sprite) {
            return paintcanvas_ext_sprite_allsize(size, sprite);
        }
    }
}

void PaintCanvas::RemoveAllMatsForGlow() {
    ArrayRemoveAll(this->glowMeshes);
    ArrayRemoveAll(this->glowMatA);
    ArrayRemoveAll(this->glowMatB);
    ArrayRemoveAll(this->glowUints);
    ArrayRemoveAll(this->glowMatC);
}

void PaintCanvas::MaterialChange(unsigned int index,
                                 AbyssEngine::BlendMode param3, unsigned int param4) {
    if (index < this->materials.count) {
        PCMaterialView *mat = (PCMaterialView *) (this->materials.data_)[index];
        mat->flags0 = param3;
        mat->textureSlots[0] = param4;
    }
}

void PaintCanvas::DrawTextLines(unsigned int font,
                                Array<AbyssEngine::String *> *arr, int x, int y, unsigned int p5,
                                bool flag) {
    int xoff = 0;
    for (unsigned int i = 0; i < arr->size(); i++) {
        if (flag == 0) {
            int w = this->GetTextWidth(font, *arr->data()[i]);
            xoff = (int) p5 - w;
        }
        this->DrawString(font, *arr->data()[i], xoff + x, y, false);
        y += this->GetTextHeight(font);
    }
}

void PaintCanvas::MeshResourceChangeMaterial(unsigned short matId, unsigned short value) {
    unsigned int count = this->resources.count;
    for (unsigned int i = 0; i < count; ++i) {
        PCResourceView *res = (PCResourceView *) (this->resources.data_)[i];
        if (res) {
            if (res->id == matId) {
                PCMaterialIdView *mat = (PCMaterialIdView *) res->payload;
                mat->materialId = value;
            }
        }
    }
}

void *PaintCanvas::TransformGetLocal(unsigned int index) { // lint: void_ptr (external symbol; mangling must match lib)
    char *result;
    if (index < this->transforms.count) {
        result = (char *) this->transforms.data_[index];
    } else {
        char tmp[60];
        result = (char *) &this->identityMatrix;
        AbyssEngine::AEMath::MatrixIdentity(
            *reinterpret_cast<AbyssEngine::AEMath::Matrix *>(result));
    }
    return result;
}

void PaintCanvas::MeshSetTangent(unsigned int index, unsigned short vtx, const Vector &value) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if (vtx >= mesh->vertexCount) {
            return;
        }
        char *base = mesh->tangents;
        ((Vector *) base)[vtx] = value;
    }
}

int PaintCanvas::GetTextWidth(unsigned int index, const AbyssEngine::String &str,
                              unsigned int begin, unsigned int end) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        const unsigned short *text = (const unsigned short *) str;
        return AbyssEngine::ImageFontGetWidth(font, text, begin, end - begin);
    }
    return 0;
}

void *PaintCanvas::MaterialGetMaterial(unsigned int index) { // lint: void_ptr (external symbol; mangling must match lib)
    if (index < this->materials.count) {
        return (this->materials.data_)[index];
    }
    return 0;
}

void PaintCanvas::EndBG() {
    Engine::vfc = this->bgFlagSaved;
}

AbyssEngine::Mesh *PaintCanvas::MeshGetPointer(unsigned int index) {
    if (index < this->meshes.count) {
        AbyssEngine::Mesh *mesh;
        char *slot = (char *) this->meshes.data_[index];
        memcpy(&mesh, &slot, sizeof(mesh));
        return mesh;
    }
    return 0;
}

void PaintCanvas::MeshChangeResourceMaterial(unsigned int meshIndex, unsigned short resId) {
    char *r = (char *) this->FindResource(resId);
    if (r) {
        int idx = ((PCResourceView *) r)->handle;
        if (idx + 1 != 0) {
            PCMeshView *mesh = (PCMeshView *) this->meshes.data_[meshIndex];
            AbyssEngine::Material *mat = (this->materials.data_)[idx];
            return paintcanvas_ext_change_mat(this, mesh, mat);
        }
    }
}

void PaintCanvas::SetColor(unsigned int color) {
    float c0 = (float) ((double) (color >> 24) / 255.0);
    float c1 = (float) ((double) ((color >> 16) & 0xff) / 255.0);
    float c2 = (float) ((double) ((color >> 8) & 0xff) / 255.0);
    float c3 = (float) ((double) (color & 0xff) / 255.0);
    this->colorR = c0;
    this->colorG = c1;
    this->colorB = c2;
    this->colorA = c3;
    return this->engine->SetColor(c0, c1, c2, c3);
}

void PaintCanvas::Vibrate(unsigned short duration) {
    this->engine->Vibrate(duration);
}

void PaintCanvas::SpriteSystemAddSize(unsigned int index, unsigned short sub, short delta) {
    if (index < this->spriteSystems.count) {
        PCSpriteSystemView *s = (PCSpriteSystemView *) (this->spriteSystems.data_)[index];
        if (s) {
            if ((unsigned short) s->count <= (unsigned int) sub) {
                return;
            }
            short *vals = (short *) s->sizes;
            if (s->uniformSize != 0) {
                vals[0] += delta;
                return;
            }
            vals[sub] += delta;
        }
    }
}

void PaintCanvas::TransformSetLocal(unsigned int index, const Matrix &matrix) {
    if (index < this->transforms.count) {
        Matrix *t = (Matrix *) this->transforms.data_[index];
        *t = matrix;
    }
}

unsigned int PaintCanvas::GetMeshResourceId(AbyssEngine::String &name, unsigned short p2) {
    for (unsigned int i = 0; i < this->resources.count; ++i) {
        PCResourceView *res = (PCResourceView *) (this->resources.data_)[i];
        if (res && res->type == 4) {
            if (name.Compare(*(const char **) res->payload) == 0) {
                PCResourceView *res2 = (PCResourceView *) (this->resources.data_)[i];
                if (((PCMaterialIdView *) res2->payload)->materialId == p2) {
                    return res2->id;
                }
            }
        }
    }
    return 0xffff;
}

int PaintCanvas::GetTextWidth(unsigned int index, const AbyssEngine::String &str) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        const unsigned short *data = (const unsigned short *) str;
        return AbyssEngine::ImageFontGetWidth(font, data, str.length);
    }
    return 0;
}

void PaintCanvas::SpriteSystemSetSize(unsigned int index,
                                      unsigned short sub, short value) {
    if (index < this->spriteSystems.count) {
        PCSpriteSystemView *s = (PCSpriteSystemView *) (this->spriteSystems.data_)[index];
        if (s) {
            if (sub < (unsigned short) s->count) {
                unsigned short *p = (unsigned short *) s->sizes;
                if (s->uniformSize) {
                    p[0] = value;
                } else {
                    p[sub] = value;
                }
            }
        }
    }
}

void PaintCanvas::SetColor(unsigned char r, unsigned char g,
                           unsigned char b, unsigned char a) {
    float fr = (float) ((double) (unsigned int) r / 255.0);
    float fg = (float) ((double) (unsigned int) g / 255.0);
    float fb = (float) ((double) (unsigned int) b / 255.0);
    float fa = (float) ((double) (unsigned int) a / 255.0);
    this->colorR = fr;
    this->colorG = fg;
    this->colorB = fb;
    this->colorA = fa;
    return this->engine->SetColor(fr, fg, fb, fa);
}

float PaintCanvas::CameraGetCurrentFactor1() {
    unsigned int cur = this->currentCamera;
    if (cur >= this->cameras.count) {
        return 1.0f;
    }
    PCCameraView *cam = (PCCameraView *) (this->cameras.data_)[cur];
    return cam->factor48;
}

void PaintCanvas::SpriteSystemAddPosition(unsigned int index, unsigned short sub,
                                          float x, float y, float z) {
    if (index < this->spriteSystems.count) {
        PCSpriteSystemView *s = (PCSpriteSystemView *) (this->spriteSystems.data_)[index];
        if (s) {
            if ((unsigned int) (unsigned short) s->count <= (unsigned int) sub) {
                return;
            }
            float *p = (float *) (s->positions + sub * 12);
            p[0] = p[0] + x;
            p[1] = p[1] + y;
            p[2] = p[2] + z;
        }
    }
}

void PaintCanvas::MeshSetBiTangent(unsigned int index, unsigned short vtx, const Vector &value) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if (vtx >= mesh->vertexCount) {
            return;
        }
        char *base = mesh->bitangents;
        ((Vector *) base)[vtx] = value;
    }
}

int PaintCanvas::GetScreenPosition(const AbyssEngine::AEMath::Vector &a,
                                   AbyssEngine::AEMath::Vector &b) {
    char buf[60];
    float *m = (float *) buf;
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;
    m[12] = 1.0f;
    m[13] = 1.0f;
    m[14] = 1.0f;

    return this->GetScreenPosition(*(AbyssEngine::AEMath::Matrix *) buf, a, b);
}

void PaintCanvas::FogEnable(bool enable, AbyssEngine::FogMode mode) {
    this->fogEnableFlag = mode;
    if (mode == 0) {
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = enable;
        } else if (enable) {
            glEnable(0xb60);
        } else {
            glDisable(0xb60);
        }
        enable = false;
    }
    this->fogMode = enable;
}

void PaintCanvas::MeshSetColor(unsigned int index, unsigned short sub,
                               unsigned int color) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if ((unsigned int) sub < (unsigned int) mesh->vertexCount) {
            float *p = (float *) (mesh->colors + sub * 0x10);
            p[0] = (float) ((double) (color >> 24) / 255.0);
            p[1] = (float) ((double) ((color >> 16) & 0xff) / 255.0);
            p[2] = (float) ((double) ((color >> 8) & 0xff) / 255.0);
            p[3] = (float) ((double) (color & 0xff) / 255.0);
        }
    }
}

void PaintCanvas::SetProjOrthoMatrix() {
    if (AbyssEngine::i_fov != -1.0f) {
        this->SetProjectionMatrix3d(AbyssEngine::i_fov, AbyssEngine::i_zNear, AbyssEngine::i_zFar);
    }
    float *m = this->projOrthoMatrix;
    memset(m, 0, sizeof(this->projOrthoMatrix));
    int w = this->engine->GetDisplayWidth();
    m[0] = (float) (2.0 / (double) w);
    int h = this->engine->GetDisplayHeight();
    m[10] = -0.05f;
    m[15] = 1.0f;
    m[12] = -1.0f;
    m[13] = 1.0f;
    m[5] = -(float) (2.0 / (double) h);
}

void PaintCanvas::MeshChangeMaterialIntern(AbyssEngine::Mesh *mesh, AbyssEngine::Material *mat) {
    if (mesh && mat) {
        mesh->material = mat;
        return paintcanvas_ext_mat_intern(this, mesh->animation);
    }
}

void PaintCanvas::MeshResourceChangeAllMaterial(unsigned short matId, unsigned short value) {
    unsigned int count = this->resources.count;
    for (unsigned int i = 0; i < count; ++i) {
        PCResourceView *res = (PCResourceView *) (this->resources.data_)[i];
        if (res) {
            PCMaterialIdView *mat = (PCMaterialIdView *) res->payload;
            if (mat->materialId == matId) {
                mat->materialId = value;
            }
        }
    }
}

unsigned int PaintCanvas::GetTextureResourceId(AbyssEngine::String &name) {
    for (unsigned int i = 0; i < this->resources.count; ++i) {
        PCResourceView *res = (PCResourceView *) (this->resources.data_)[i];
        if (res && res->type == 2) {
            char *namePtr = *(char **) res->payload;
            if (name.Compare(namePtr) == 0) {
                return ((PCResourceView *) (this->resources.data_)[i])->id;
            }
        }
    }
    return 0xffff;
}

unsigned int PaintCanvas::GetColor() {
    float a = this->colorR;
    float r = this->colorG;
    float g = this->colorB;
    float b = this->colorA;
    return ((unsigned int) (int) (r * 255.0f) << 16) +
           ((unsigned int) (int) (a * 255.0f) << 24) +
           ((unsigned int) (int) (g * 255.0f) << 8) +
           (unsigned int) (int) (b * 255.0f);
}

void PaintCanvas::CameraSetCurrent(unsigned int index) {
    this->currentCamera = index;
    if (index < this->cameras.count) {
        PCCameraParamsView *cam = (PCCameraParamsView *) (this->cameras.data_)[index];
        return paintcanvas_ext_camera_apply(this, cam->raw[0], cam->raw[1], cam->raw[2]);
    }
}

void PaintCanvas::MeshSetTriangleCount(unsigned int index, unsigned short count) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        unsigned short cap = mesh->triCapacity;
        if (cap < count) {
            count = cap;
        }
        mesh->indexCount = (unsigned short) (count * 3);
    }
}

void PaintCanvas::DisableClip() {
    glDisable(0xc11);
}

void PaintCanvas::TransformSetColor(unsigned int index, unsigned int color) {
    if (index < this->transforms.count) {
        PCTransformView *obj = (PCTransformView *) this->transforms.data_[index];
        obj->color = color;
    }
}

void PaintCanvas::MeshSetColor(unsigned int index, unsigned short sub,
                               float r, float g, float b, float a) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if ((unsigned int) sub < (unsigned int) mesh->vertexCount) {
            float *p = (float *) (mesh->colors + sub * 0x10);
            p[0] = r;
            p[1] = g;
            p[2] = b;
            p[3] = a;
        }
    }
}

void PaintCanvas::MeshSetNormal(unsigned int index, unsigned short vtx, const Vector &value) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if (vtx >= mesh->vertexCount) {
            return;
        }
        char *base = mesh->normals;
        ((Vector *) base)[vtx] = value;
    }
}

void PaintCanvas::MeshClear2DMask() {
    this->field_0x20 = 0;
}

unsigned int PaintCanvas::GetMeshResourceId(AbyssEngine::String &name) {
    for (unsigned int i = 0; i < this->resources.count; ++i) {
        PCResourceView *res = (PCResourceView *) (this->resources.data_)[i];
        if (res && res->type == 4) {
            char *namePtr = *(char **) res->payload;
            if (name.Compare(namePtr) == 0) {
                return ((PCResourceView *) (this->resources.data_)[i])->id;
            }
        }
    }
    return 0xffff;
}

void PaintCanvas::MeshTranslatePoint(unsigned int index, unsigned short sub,
                                     float x, float y, float z) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if ((unsigned int) mesh->vertexCount <= (unsigned int) sub) {
            return;
        }
        float *p = (float *) (mesh->positions + sub * 12);
        p[0] = p[0] + x;
        p[1] = p[1] + y;
        p[2] = p[2] + z;
    }
}

void PaintCanvas::CheckNUseRefractFBO(bool) {
    if (Engine::enableShader != 0 && Engine::EnableRefract != 0 &&
        this->engine->IsPostEffectActivated() == 0) {
        return this->engine->ActivateRender2FracFBO();
    }
}

void PaintCanvas::SpriteSystemSetUv(unsigned int index, unsigned short sub,
                                    float a, float b, float c, float d) {
    if (index < this->spriteSystems.count) {
        AbyssEngine::SpriteSystem *s = (this->spriteSystems.data_)[index];
        if (s) {
            if ((unsigned int) (unsigned short) s->count <= (unsigned int) sub) {
                return;
            }
            return AbyssEngine::SpriteSystemSetUv(sub, a, b, c, d, s);
        }
    }
}

void PaintCanvas::SetWorldViewMatrix(const AbyssEngine::AEMath::Matrix &matrix) {
    return this->engine->SetWorldViewMatrix(matrix);
}

void PaintCanvas::CameraSetLocal(unsigned int index, const Matrix &matrix) {
    if (index < this->cameras.count) {
        PCCameraView *cam = (PCCameraView *) (this->cameras.data_)[index];
        *(Matrix *) cam->localMatrix = matrix;
    }
}

void PaintCanvas::SetShaderMode(int mode) {
    ((Engine *) this->engine)->field_0x4a8 = mode;
}

void PaintCanvas::MeshConvertToVBO(unsigned int index) {
    if (index < this->meshes.count) {
        return paintcanvas_ext_convert_vbo(
            this->meshes.data_[index]);
    }
}



void PaintCanvas::DrawImage2D(unsigned int index, int x, int y,
                              unsigned char flipFlags) {
    if (index >= this->images.count) {
        return;
    }
    PCImage2DView *img = (PCImage2DView *) this->images.data_[index];
    if (img->restoreFlag != 0) {
        this->RestoreImage2D((AbyssEngine::Image2D *) img);
        img = (PCImage2DView *) this->images.data_[index];
    }
    this->SetTexture(img->textureId, -1);

    float fx = (float) x;
    float fy = (float) y;
    AbyssEngine::AEMath::Matrix m;
    m.m[7] = fy;
    m.m[3] = fx;
    if (flipFlags & 1) {
        m.m[0] = -1.0f;
        m.m[3] = (float) this->GetImage2DWidth(index) + m.m[3];
    }
    if (flipFlags & 2) {
        m.m[5] = -1.0f;
        m.m[7] = (float) this->GetImage2DHeight(index) + m.m[7];
    }
    this->SetWorldViewMatrix(m);
    glDisable(0xb44);
    AbyssEngine::MeshDraw(this->engine, (AbyssEngine::Mesh *) ((PCImage2DView *) this->images.data_[index])->mesh);
    glEnable(0xb44);
}

void PaintCanvas::FontSetYOffset(unsigned int index, short yoff) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        return AbyssEngine::ImageFontSetYOffset(font, yoff);
    }
}

void PaintCanvas::SetBlendMode(AbyssEngine::BlendMode mode) {
    this->engine->LightEnable(false);

    if (Engine::enableShader != 0) {
        this->engine->GlEnable(0x1100020, false);
    } else {
        glTexEnvi(0x2300, 0x2200, 0x2100);
    }

    switch (mode) {
        case 1:
        case 5:
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(0x302, 0x303);
            glDepthMask(0);
            return;
        case 2:
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(1, 1);
            glDepthMask(0);
            return;
        case 3:
            glDisable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(1, 1);
            glDepthMask(0);
            return;
        case 4:
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(0, 0x301);
            glDepthMask(0);
            return;
        case 6:
            this->engine->LightEnable(true);
            this->engine->LightSetLight(0x4000);
        /* fallthrough */
        case 0:
            glEnable(0xb44);
            glDisable(0xbe2);
            glDepthMask(1);
            return;
        case 7:
            this->engine->LightEnable(true);
            this->engine->LightSetLight(0x4000);
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(1, 1);
            glDepthMask(0);
            return;
        case 8:
            this->engine->LightEnable(true);
            this->engine->LightSetLight(0x4000);
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(0x302, 0x303);
            glDepthMask(0);
            return;
        case 9:
            this->engine->LightEnable(true);
            this->engine->LightSetLight(0x4000);
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(0x302, 0x303);
            glDepthMask(1);
            return;
        case 10:
            glEnable(0xb44);
            glDisable(0xbe2);
            glDepthMask(1);
            this->engine->GlEnable(0x1000000, true);
            if (Engine::enableShader == 0) {
                glAlphaFunc(0x204, 0.5f);
            }
            return;
        case 0x15:
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(0x302, 0x303);
            glDepthMask(0);
            if (Engine::enableShader != 0) {
                this->engine->GlEnable(0x1100020, true);
                return;
            }
            glTexEnvi(0x2300, 0x2200, 0x8570);
            glTexEnvi(0x2300, 0x8571, 0x104);
            glTexEnvi(0x2300, 0x8572, 0x1e01);
            glTexEnvi(0x2300, 0x8580, 0x1702);
            glTexEnvi(0x2300, 0x8590, 0x300);
            glTexEnvi(0x2300, 0x8581, 0x8577);
            glTexEnvi(0x2300, 0x8591, 0x300);
            return;
        case 0x16:
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(0x302, 0x303);
            glDepthMask(0);
            if (Engine::enableShader == 0) {
                glTexEnvi(0x2300, 0x2200, 0x8570);
                glTexEnvi(0x2300, 0x8571, 0x8575);
                glTexEnvi(0x2300, 0x8572, 0x1e01);
                glTexEnvi(0x2300, 0x8580, 0x1702);
                glTexEnvi(0x2300, 0x8590, 0x300);
                glTexEnvi(0x2300, 0x8588, 0x1702);
                glTexEnvi(0x2300, 0x8598, 0x302);
                glTexEnvi(0x2300, 0x8581, 0x8577);
                glTexEnvi(0x2300, 0x8591, 0x300);
                glTexEnvi(0x2300, 0x8582, 0x8577);
                glTexEnvi(0x2300, 0x8592, 0x302);
            }
            return;
        case 0x25:
            glEnable(0xb44);
            glEnable(0xbe2);
            glBlendFunc(0x302, 0x303);
            glDepthMask(1);
            return;
        default:
            return;
    }
}

static const char g_gla_nl_8c4c0[] = "\n";

void PaintCanvas::GetLineArray(unsigned int font, const AbyssEngine::String &str, int width,
                               ::Array<AbyssEngine::String *> *outArray) {
    AbyssEngine::String *line = new AbyssEngine::String();
    AbyssEngine::String text(str, false);
    text += AbyssEngine::String(g_gla_nl_8c4c0, false);

    // First pass: count the wrapped lines.
    unsigned int count = 0;
    for (int pos = 0; pos < (int) text.size(); count++) {
        AbyssEngine::String rest = text.SubString((unsigned int) pos, text.size());
        this->GetLine(font, rest, width, line);
        pos += (int) line->size();
    }
    delete line;

    ArraySetLength<AbyssEngine::String *>(count, *outArray);
    for (unsigned int i = 0; i < count; i++) {
        outArray->data_[i] = new AbyssEngine::String();
    }

    // Second pass: fill each line and trim surrounding blanks.
    int pos = 0;
    for (unsigned int i = 0; i < count; i++) {
        AbyssEngine::String rest = text.SubString((unsigned int) pos, text.size());
        this->GetLine(font, rest, width, outArray->data_[i]);
        int end = (int) outArray->data_[i]->size();
        pos += end;
        int start = 0;
        while (*(*outArray->data_[i])[start] == ' ') {
            start++;
        }
        while (*(*outArray->data_[i])[end - 1] == ' ') {
            end--;
        }
        *outArray->data_[i] = outArray->data_[i]->SubString((unsigned int) start, (unsigned int) end);
    }
}

void PaintCanvas::AddResource(AbyssEngine::Resource *resource) {
    return paintcanvas_ext_add_resource(resource, &this->resources);
}

void PaintCanvas::TransformRemoveMeshId(unsigned int transformIndex, unsigned int meshIndex) {
    if (transformIndex < this->transforms.count &&
        meshIndex < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[meshIndex];
        PCTransformView *t = (PCTransformView *) this->transforms.data_[transformIndex];
        return ArrayRemove<AbyssEngine::Mesh *>((AbyssEngine::Mesh *) mesh,
                                              *reinterpret_cast<Array<AbyssEngine::Mesh *> *>(&t->meshCount));
    }
}

int PaintCanvas::GetHeight() {
    return this->engine->GetDisplayHeight();
}

void PaintCanvas::ChangeCubeTexture(unsigned int idx) {
    if (Engine::enableShader != 0 && idx < this->cubeTextures.count) {
        PCCubeTexView *tex = (PCCubeTexView *) (this->cubeTextures.data_)[idx];
        if (tex->restoreFlag == 0) {
            // The original fetches the texture name (for a log line compiled out of release).
            char *name = reinterpret_cast<AbyssEngine::String *>(tex->pathField)->GetAEChar();
            delete[] name;
            return;
        }
        AbyssEngine::CubeMapSetIndex = idx;
        glActiveTexture(0x84c7);
        PCCubeTexView *tex2 = (PCCubeTexView *) (this->cubeTextures.data_)[idx];
        glBindTexture(0x8513, (unsigned int) tex2->glTexId);
        return glActiveTexture(0x84c0);
    }
}

void PaintCanvas::TransformCreate(unsigned short resId, unsigned int &out) {
    PCResourceView *res = (PCResourceView *) this->FindResource(resId);
    if (res == 0) {
        return;
    }
    if ((unsigned int) res->handle != 0xffffffff) {
        out = (unsigned int) res->handle;
        return;
    }
    PCTransformInfoView *info = (PCTransformInfoView *) res->payload;
    AbyssEngine::Transform *tf = new AbyssEngine::Transform();
    ArrayAdd<Transform *>(tf, this->transforms);
    unsigned int idx = this->transforms.count - 1;
    res->handle = (int) idx;
    out = idx;
    tf->worldMatrix = *(const AbyssEngine::AEMath::Matrix *) info;

    for (unsigned int i = 0; i < info->childMeshCount; i++) {
        unsigned int meshId = 0xffffffff;
        this->MeshCreate(((unsigned short *) info->childMeshIds)[i], meshId, false);
        if (meshId != 0xffffffff) {
            ArrayAdd<AbyssEngine::Mesh *>(this->meshes.data_[meshId], tf->meshes);
        }
    }
    for (unsigned int i = 0; i < info->childTfCount; i++) {
        unsigned int childId = 0xffffffff;
        this->TransformCreate(((unsigned short *) info->childTfIds)[i], childId);
        if (childId != 0xffffffff) {
            ArrayAdd<AbyssEngine::Transform *>(this->transforms.data_[childId], tf->children);
        }
    }
}

void PaintCanvas::End2d() {
    char buf[60];
    int v = this->field_0xc;
    if (v >= 1) {
        float *m = (float *) buf;
        ((PCMeshView *) this->quad2dMesh)->indexCount = (unsigned short) (short) (v * 6);
        m[0] = 1.0f;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = 1.0f;
        m[11] = 0.0f;
        m[12] = 1.0f;
        m[13] = 1.0f;
        m[14] = 1.0f;
        paintcanvas_ext_set_wvm2(this, buf);
        paintcanvas_ext_meshdraw(this->engine, this->quad2dMesh);
    }
}

void PaintCanvas::MeshChangeMaterialIntern(Transform *transform, AbyssEngine::Material *material) {
    if (transform && material) {
        for (unsigned int i = 0; i < transform->meshes.size(); ++i) {
            this->MeshChangeMaterialIntern(transform->meshes[i], material);
        }
        for (unsigned int i = 0; i < transform->children.size(); ++i) {
            this->MeshChangeMaterialIntern(transform->children[i], material);
        }
    }
}

int PaintCanvas::GetTextHeight(unsigned int index) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        return AbyssEngine::ImageFontGetHeight(font);
    }
    return 0;
}

static const unsigned int g_init_const_7e7b4 = 0;

static const unsigned int g_init_const_7e7b8 = 0;

void PaintCanvas::Initialize(bool landscape) {
    this->gameOrientation = landscape ? 2 : 0;
    this->engine->SetScreenOrientation((AbyssEngine::LandscapeMode) this->gameOrientation);

    float *proj = this->projOrthoMatrix;
    memset(proj, 0, sizeof(this->projOrthoMatrix));
    int orient = this->gameOrientation;
    float fw = (float) (int) this->engine->GetDisplayWidth();
    float fh = (float) (int) this->engine->GetDisplayHeight();

    if (orient != 2) {
        proj[0] = 2.0f / fh;
        proj[5] = -2.0f / fw;
        float *view = this->worldViewMatrix;
        memset(view, 0, sizeof(this->worldViewMatrix));
        view[1] = -1.0f;
        view[4] = 1.0f;
        view[10] = 1.0f;
        view[15] = 1.0f;
        view[13] = fw;
    } else {
        proj[0] = 2.0f / fw;
        proj[5] = -2.0f / fh;
    }
    proj[10] = -0.05f;
    proj[15] = 1.0f;
    proj[12] = -1.0f;
    proj[13] = 1.0f;

    return this->SetProjectionMatrix3d(16384.0f, 512.0f, 65536.0f);
}

void PaintCanvas::DrawMesh(unsigned int index) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        return paintcanvas_ext_draw_mesh(this->engine, mesh);
    }
}

int PaintCanvas::MeshGetTriCount(AbyssEngine::Mesh *mesh) {
    if (mesh) {
        int tri;
        if (mesh->animation == 0) {
            tri = 0;
        } else {
            tri = this->TransformGetTriCount(mesh->animation);
        }
        int q = (int) ((unsigned) (mesh->indexCount) / (unsigned) (3));
        return q + tri;
    }
    return 0;
}

short PaintCanvas::FontGetYOffset(unsigned int index) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        return AbyssEngine::ImageFontGetYOffset(font);
    }
    return 0;
}

void PaintCanvas::SetProjectionMatrix3d(float fov, float nearPlane,
                                        float farPlane) {
    Engine::nearPlane = nearPlane;
    Engine::farPlane = farPlane;
    AbyssEngine::i_fov = fov;
    AbyssEngine::i_zNear = nearPlane;
    AbyssEngine::i_zFar = farPlane;

    float half = fov * 0.5f;
    int w = this->engine->GetDisplayWidth();
    int h = this->engine->GetDisplayHeight();
    float s = AbyssEngine::AEMath::Sinf(half);
    float c = AbyssEngine::AEMath::Cosf(half);
    float aspect = (float) w / (float) h;
    float f = 1.0f / (s / c);

    float *m = this->projMatrix3d;
    memset(m, 0, sizeof(this->projMatrix3d));
    switch (this->gameOrientation) {
        case 0:
            m[4] = -f;
            m[1] = f / aspect;
            break;
        case 1:
            m[4] = f;
            m[1] = -(f / aspect);
            break;
        case 2:
            m[5] = f;
            m[0] = f / aspect;
            break;
        case 3:
            m[5] = -f;
            m[0] = -(f / aspect);
            break;
    }
    m[11] = -1.0f;
    m[10] = (nearPlane + farPlane) / (nearPlane - farPlane);
    m[14] = (float) ((2.0 * (double) farPlane * (double) nearPlane) / (double) (nearPlane - farPlane));
}


void PaintCanvas::DrawSpriteSystem(unsigned int index) {
    if (index >= this->spriteSystems.count || this->spriteSystems.data_[index] == 0) {
        return;
    }
    AbyssEngine::AEMath::Matrix world;
    if (this->currentCamera < this->cameras.count) {
        if (this->initialized != 0) {
            AbyssEngine::AEMath::Matrix roll;
            AbyssEngine::AEMath::MatrixIdentity(roll);
            float angle = (float) (this->engine->GetGravValue()[1] * 1.5707963705062866);
            float rot = (this->gameOrientation == 1) ? angle : -angle;
            float s = AbyssEngine::AEMath::Sinf(rot);
            float c = AbyssEngine::AEMath::Cosf(rot);
            roll.m[0] = c;
            roll.m[5] = c;
            roll.m[1] = -s;
            roll.m[4] = s;
            AbyssEngine::AEMath::Matrix view = this->cameras.data_[this->currentCamera]->projection;
            view *= roll;
            world = AbyssEngine::AEMath::MatrixGetInverse(view);
        } else {
            world = AbyssEngine::AEMath::MatrixGetInverse(this->cameras.data_[this->currentCamera]->projection);
        }
    }
    Engine *engine = this->engine;
    AbyssEngine::AEMath::Matrix identity;
    AbyssEngine::SpriteSystemDraw(engine, identity, world, this->spriteSystems.data_[index]);
}

void PaintCanvas::MeshSetPoint(unsigned int index, unsigned short vtx,
                               float x, float y, float z) {
    if (index < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[index];
        if (vtx < mesh->vertexCount) {
            float *p = (float *) (mesh->positions + vtx * 12);
            p[0] = x;
            p[1] = y;
            p[2] = z;
        }
    }
}

static const char g_getline_empty_7c428[] = "";

void PaintCanvas::GetLine(unsigned int font, AbyssEngine::String str, int maxWidth,
                          AbyssEngine::String *out) {
    unsigned int i = 0;
    int width = 5;
    int lastSpace = 0;
    for (; i < str.size(); i++) {
        unsigned short c = *str[i];
        width += this->GetTextWidth(font, str, i, i + 1);
        if (c == ' ') {
            lastSpace = (int) i;
        }
        if (width >= maxWidth) {
            if (lastSpace > 0) {
                *out = str.SubString(0, lastSpace + 1);
            } else {
                *out = str.SubString(0, i + 1);
            }
            return;
        }
        if (*str[i] == '\n' || *str[i] == '\r') {
            *out = str.SubString(0, i + 1);
            return;
        }
    }

    if ((int) str.size() > 1) {
        *out = str.SubString(0, str.size());
    } else {
        *out = AbyssEngine::String(g_getline_empty_7c428, false);
    }
}

void PaintCanvas::DrawLine(int x0, int y0, int x1i, int y1i) {
    if (Engine::enableShader != 0) {
        this->SetWorldViewMatrix(AbyssEngine::AEMath::Matrix());

        this->lineVerts[0] = (float) (x0 + 1);
        this->lineVerts[1] = (float) y0;
        this->lineVerts[2] = (float) (x1i + 1);
        this->lineVerts[3] = (float) y1i;
        this->engine->DrawLine2D(this->lineVerts, 2, false);
    } else {
        glLineWidth(1.0f);
        this->engine->GlEnable(0xde1, false);

        this->lineVerts[0] = (float) (x0 + 1);
        this->lineVerts[1] = (float) y0;
        this->lineVerts[2] = (float) (x1i + 1);
        this->lineVerts[3] = (float) y1i;

        this->SetWorldViewMatrix(AbyssEngine::AEMath::Matrix());
        glVertexPointer(2, 0x1406, 0, this->lineVerts);
        this->engine->AEClientState(0x8074, true);
        this->engine->AEClientState(0x8078, false);
        this->engine->AEClientState(0x8075, false);
        this->engine->AEClientState(0x8076, false);
        glDrawArrays(1, 0, 2);
        this->engine->GlEnable(0xde1, true);
    }
}

void PaintCanvas::MeshChangeMaterial(unsigned int meshIndex, unsigned short matIndex) {
    if (matIndex < this->materials.count &&
        meshIndex < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[meshIndex];
        AbyssEngine::Material *mat = (this->materials.data_)[matIndex];
        return paintcanvas_ext_change_mat(this, mesh, mat);
    }
}

void PaintCanvas::CameraSetPerspective(unsigned int index, float a, float b, float c) {
    if (index < this->cameras.count) {
        float w = (float) this->GetWidth();
        float h = (float) this->GetHeight();
        AbyssEngine::Camera *cam = (this->cameras.data_)[index];
        AbyssEngine::CameraSetPerspective(a, b, c, w, h, cam);
        if (this->currentCamera == index) {
            return this->CameraSetCurrent(index);
        }
    }
}

void PaintCanvas::End3d() {
    if (Engine::enableShader != 0) {
        this->RemoveAllMatsForGlow();
    }
    if (this->fogEnableFlag == 1 && this->fogMode != 0) {
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = true;
        } else {
            glEnable(0xb60);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 0);

    // Opaque pass; glow-flagged materials are queued and their draw lists copied to the glow set.
    bool glowPending = false;
    for (unsigned int i = 0; i < this->materials.count; i++) {
        AbyssEngine::Material *mat = this->materials.data_[i];
        if (mat->blendMode == 0) {
            if (Engine::enableShader != 0 && this->engine->IsPostEffectActivated() &&
                Engine::EnableGlow != 0 && this->materials.data_[i]->addData != 0) {
                this->SetMatForGlow(this->materials.data_[i]);
                glowPending = true;
            }
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
        if (Engine::enableShader != 0 && this->engine->IsPostEffectActivated() &&
            Engine::EnableGlow != 0 && glowPending) {
            for (unsigned int j = 0; j < this->glowMatA.count; j++) {
                ArrayAdd<AbyssEngine::Mesh *>(this->glowMeshes.data_[j], this->materials.data_[i]->meshes);
                ArrayAdd<AbyssEngine::AEMath::Matrix>(this->glowMatA.data_[j], this->materials.data_[i]->arr_2c);
                ArrayAdd<AbyssEngine::AEMath::Matrix>(this->glowMatB.data_[j], this->materials.data_[i]->arr_38);
                ArrayAdd<unsigned int>(this->glowUints.data_[j], this->materials.data_[i]->arr_50);
                ArrayAdd<AbyssEngine::AEMath::Matrix>(this->glowMatC.data_[j], this->materials.data_[i]->arr_5c);
            }
            this->RemoveAllMatsForGlow();
            glowPending = false;
        }
    }

    if (this->fogEnableFlag == 1 && this->fogMode != 0) {
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = false;
        } else {
            glDisable(0xb60);
        }
        for (unsigned int i = 0; i < this->materials.count; i++) {
            if (this->materials.data_[i]->blendMode == 23) {
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
            }
        }
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = true;
        } else {
            glEnable(0xb60);
        }
    }

    this->engine->LightEnable(true);
    AbyssEngine::AEMath::Matrix camera = *(AbyssEngine::AEMath::Matrix *) this->CameraGetLocal(this->currentCamera);
    this->engine->SetWorldViewMatrix(AbyssEngine::AEMath::MatrixGetInverse(camera));
    this->engine->LightSetLight(0x4000);

    for (unsigned int i = 0; i < this->materials.count; i++) {
        AbyssEngine::Material *mat = this->materials.data_[i];
        unsigned int cap;
        switch (mat->blendMode) {
            case 6:
                AbyssEngine::MaterialDraw(this, this->engine, mat, true);
                break;
            case 11:
                this->engine->GlEnable(0x1100000, true);
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                this->engine->GlEnable(0x1100000, false);
                break;
            case 12:
            case 28:
            case 29:
                if (Engine::enableShader != 0 && this->engine->IsPostEffectActivated() &&
                    Engine::EnableGlow != 0 && this->materials.data_[i]->addData != 0) {
                    this->SetMatForGlow(this->materials.data_[i]);
                    glowPending = true;
                }
                if (AbyssEngine::CubeMapSetIndex == 0) {
                    this->engine->GlEnable(0x1100013, true);
                    AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                    this->engine->GlEnable(0x1100013, false);
                } else {
                    this->engine->GlEnable(0x1100014, true);
                    AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                    this->engine->GlEnable(0x1100014, false);
                }
                break;
            case 13:
                if (Engine::enableShader != 0 && this->engine->IsPostEffectActivated() &&
                    Engine::EnableGlow != 0 && this->materials.data_[i]->addData != 0) {
                    this->SetMatForGlow(this->materials.data_[i]);
                    glowPending = true;
                }
                this->engine->GlEnable(0x1100014, true);
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                this->engine->GlEnable(0x1100014, false);
                break;
            case 15:
                this->engine->GlEnable(0x1100001, true);
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                this->engine->GlEnable(0x1100001, false);
                break;
            case 19:
                this->engine->GlEnable(0x1100006, true);
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                this->engine->GlEnable(0x1100006, false);
                break;
            case 38:
                this->engine->GlEnable(0x1100022, true);
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                this->engine->GlEnable(0x1100022, false);
                break;
        }
        if (Engine::enableShader != 0 && this->engine->IsPostEffectActivated() &&
            Engine::EnableGlow != 0 && glowPending) {
            for (unsigned int j = 0; j < this->glowMatA.count; j++) {
                ArrayAdd<AbyssEngine::Mesh *>(this->glowMeshes.data_[j], this->materials.data_[i]->meshes);
                ArrayAdd<AbyssEngine::AEMath::Matrix>(this->glowMatA.data_[j], this->materials.data_[i]->arr_2c);
                ArrayAdd<AbyssEngine::AEMath::Matrix>(this->glowMatB.data_[j], this->materials.data_[i]->arr_38);
                ArrayAdd<unsigned int>(this->glowUints.data_[j], this->materials.data_[i]->arr_50);
                ArrayAdd<AbyssEngine::AEMath::Matrix>(this->glowMatC.data_[j], this->materials.data_[i]->arr_5c);
            }
            this->RemoveAllMatsForGlow();
            glowPending = false;
        }
    }

    this->SetBlendMode((AbyssEngine::BlendMode) 8);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        AbyssEngine::Material *mat = this->materials.data_[i];
        if (mat->blendMode == 36) {
            this->engine->GlEnable(0x1100021, true);
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
            this->engine->GlEnable(0x1100021, false);
        } else if (mat->blendMode == 16) {
            this->engine->GlEnable(0x1100016, true);
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
            this->engine->GlEnable(0x1100016, false);
        } else if (mat->blendMode == 8) {
            AbyssEngine::MaterialDraw(this, this->engine, mat, true);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 9);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 9) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 7);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 7) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->engine->LightEnable(false);
    this->SetBlendMode((AbyssEngine::BlendMode) 37);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 37) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 1);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 1) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    if (this->fogEnableFlag == 1 && this->fogMode != 0) {
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = false;
        } else {
            glDisable(0xb60);
        }
        for (unsigned int i = 0; i < this->materials.count; i++) {
            if (this->materials.data_[i]->blendMode == 24) {
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
            }
        }
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = true;
        } else {
            glEnable(0xb60);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 21);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 21) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 22);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 22) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 10);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 10) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->engine->GlEnable(0x1000000, false);
    this->SetBlendMode((AbyssEngine::BlendMode) 2);
    if (this->fogEnableFlag == 1 && this->fogMode != 0) {
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = false;
        } else {
            glDisable(0xb60);
        }
    }
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 2) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 3);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 3) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    if (this->fogEnableFlag == 1 && this->fogMode != 0) {
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = true;
        } else {
            glEnable(0xb60);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 4);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 4) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 5);
    unsigned int savedColor = this->engine->packedColor;
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 5) {
            this->SetColor(255, 255, 255, 127);
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }
    this->SetColor(savedColor);
    this->SetBlendMode((AbyssEngine::BlendMode) 1);
    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i]->blendMode == 18) {
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
        }
    }

    // Refraction pass: refractive materials keep the refraction FBO alive while they have meshes.
    if (Engine::enableShader != 0 && Engine::EnableRefract != 0) {
        this->engine->CopyFBO();
        this->engine->SetPerspMatrix(this->projMatrix3d);
    }
    this->SetBlendMode((AbyssEngine::BlendMode) 0);
    bool refract = false;
    for (unsigned int i = 0; i < this->materials.count; i++) {
        AbyssEngine::Material *mat = this->materials.data_[i];
        if (mat->blendMode == 14) {
            unsigned int meshCount = mat->meshes.count;
            this->engine->GlEnable(0x1100015, true);
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
            this->engine->GlEnable(0x1100015, false);
            refract |= meshCount != 0;
        } else if (mat->blendMode == 34) {
            unsigned int meshCount = mat->meshes.count;
            this->engine->GlEnable(0x1100018, true);
            AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
            this->engine->GlEnable(0x1100018, false);
            refract |= meshCount != 0;
        }
    }
    bool disableRefract = Engine::DisableRefract;
    this->SetBlendMode((AbyssEngine::BlendMode) 3);
    if (disableRefract) {
    for (unsigned int i = 0; i < this->materials.count; i++) {
            if (this->materials.data_[i]->blendMode == 39) {
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
            }
        }
    } else {
        for (unsigned int i = 0; i < this->materials.count; i++) {
            AbyssEngine::Material *mat = this->materials.data_[i];
            if (mat->blendMode == 39) {
                unsigned int meshCount = mat->meshes.count;
                this->engine->GlEnable(0x1100024, true);
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                this->engine->GlEnable(0x1100024, false);
                refract |= meshCount != 0;
            }
        }
    }
    Engine::EnableRefract = refract;

    if (this->fogMode != 0) {
        if (Engine::enableShader != 0) {
            Engine::fogEnabled = false;
        } else {
            glDisable(0xb60);
        }
    }
    if (Engine::enableShader != 0 && this->engine->IsPostEffectActivated() && Engine::EnableGlow != 0) {
        this->SetBlendMode((AbyssEngine::BlendMode) 0);
        this->engine->GlowBeginGlow(0x203);
        for (unsigned int i = 0; i < this->materials.count; i++) {
            if (this->materials.data_[i]->addData != 0) {
                this->engine->GlEnable(0x1100017, true);
                AbyssEngine::MaterialDraw(this, this->engine, this->materials.data_[i], true);
                this->engine->GlEnable(0x1100017, false);
            }
        }
        this->engine->GlowEndGlow();
    }
}

void PaintCanvas::MeshCloneMaterial(unsigned int index, unsigned int &out) {
    int result;
    if (index < this->meshes.count) {
        AbyssEngine::Material *obj =
                new AbyssEngine::Material(((PCMeshView *) this->meshes.data_[index])->material);
        ArrayAdd<AbyssEngine::Material *>(obj, this->materials);
        result = (int) this->materials.count - 1;
    } else {
        result = -1;
    }
    out = result;
}

void PaintCanvas::GetGravValue() {
    return paintcanvas_ext_get_grav(this->engine);
}

void PaintCanvas::BeginBG() {
    ((Engine *) this->engine)->statsEnabled = 0;
    glDisable(0xb71);
    glDepthMask(0);
    glDisable(0xbe2);
    this->engine->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    this->bgFlagSaved = Engine::vfc;
    Engine::vfc = 0;
    if (Engine::enableShader != 0) {
        return this->engine->SetPerspMatrix(this->projMatrix3d);
    }
    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    glMatrixMode(0x1701);
    glLoadMatrixf(this->projMatrix3d);
    glMatrixMode(0x1700);
    return glLoadIdentity();
}

void PaintCanvas::FontCreate(unsigned short resId, unsigned int &out,
                             bool /*unused*/) {
    PCResourceView *res = (PCResourceView *) this->FindResource(resId);
    if (res == 0) {
        return;
    }
    // Payload: texture resource id, font index.
    unsigned short *info = (unsigned short *) res->payload;
    PCResourceView *texres = (PCResourceView *) this->FindResource(info[0]);
    if (texres == 0) {
        return;
    }
    if (texres->handle == -1) {
        unsigned int texture = 0;
        this->TextureCreate(info[0], texture, false);
    }
    if (res->handle != -1) {
        out = (unsigned int) res->handle;
        return;
    }
    AbyssEngine::ImageFont *font = 0;
    if (AbyssEngine::ImageCreateFontFromFile(this->engine, *(char **) texres->payload, info[1], &font) != 1) {
        return;
    }
    if (texres->handle != -1) {
        ((PCFontView *) font)->texture = texres->handle;
    }
    ArrayAdd<AbyssEngine::ImageFont *>(font, this->fonts);
    res->handle = (int) this->fonts.count - 1;
    out = (unsigned int) res->handle;

    // Track the smallest font (by glyph height) as the engine default.
    Engine *eng = this->engine;
    if (eng->field_0x78 == -1) {
        eng->field_0x78 = (int) out;
    } else if (((PCFontView *) this->fonts.data_[eng->field_0x78])->key <= ((PCFontView *) font)->key) {
        int curH = AbyssEngine::ImageFontGetHeight(this->fonts.data_[eng->field_0x78]);
        if (curH > AbyssEngine::ImageFontGetHeight(font)) {
            this->engine->field_0x78 = (int) out;
        }
    }
}

void PaintCanvas::SetResourceList(AbyssEngine::Resource * const * list, unsigned int count) {
    return paintcanvas_ext_set_reslist(list, count, &this->resources);
}

void PaintCanvas::MaterialResourceChangeTexture(unsigned int resId,
                                                unsigned int texture, int slot) {
    if ((unsigned int) slot < 8) {
        char *r = (char *) this->FindResource((unsigned short) resId);
        if (r) {
            unsigned int matIdx = (unsigned int) ((PCResourceView *) r)->handle;
            if (matIdx + 1 != 0 && matIdx < this->materials.count) {
                PCMaterialView *mat = (PCMaterialView *) (this->materials.data_)[matIdx];
                mat->textureSlots[slot] = texture;
            }
        }
    }
}

void PaintCanvas::TransformAddChild(unsigned int parent, unsigned int child) {
    if (parent != child) {
        unsigned int count = this->transforms.count;
        if (child < count && parent < count) {
            char **arr = (char **) this->transforms.data_;
            AbyssEngine::Transform *p = (AbyssEngine::Transform *) arr[parent];
            AbyssEngine::Transform *c = (AbyssEngine::Transform *) arr[child];
            ArrayAdd<AbyssEngine::Transform *>(c, p->children);
            arr = (char **) this->transforms.data_;
            p = (AbyssEngine::Transform *) arr[parent];
            c = (AbyssEngine::Transform *) arr[child];
            p->bounds().Merge(*c);
            arr = (char **) this->transforms.data_;
            return ((AbyssEngine::Transform*)arr[parent])->CollectAnimationData();
        }
    }
}

void PaintCanvas::TextureCreateGlobal(AbyssEngine::String name, unsigned int unit) {
    unsigned int outId = 0;
    char *path = name.GetAEChar();
    int rc = AbyssEngine::TextureCreateFromFile(this->engine, path, nullptr, nullptr,
                                                &outId, false, 0.0f);
    if (rc == 1) {
        glActiveTexture(unit + 0x84c0);
        glBindTexture(0xde1, outId);
        glActiveTexture(0x84c0);
    }
    delete[] path;
}

void PaintCanvas::Begin3d() {
    ((Engine *) this->engine)->statsEnabled = 0;
    glEnable(0xb71);
    glDepthMask(1);
    glDisable(0xbe2);
    this->engine->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    if (Engine::enableShader != 0) {
        return this->engine->SetPerspMatrix(this->projMatrix3d);
    }
    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    glMatrixMode(0x1701);
    glLoadMatrixf(this->projMatrix3d);
    glMatrixMode(0x1700);
    return glLoadIdentity();
}

void PaintCanvas::TransformAddMesh(unsigned int transformIndex,
                                   unsigned short meshId, bool b) {
    if (transformIndex < this->transforms.count) {
        unsigned int meshIndex = 0xffffffff;
        this->MeshCreate(meshId, meshIndex, b);
        if (meshIndex == 0xffffffff) {
            return;
        }
        ArrayAdd<AbyssEngine::Mesh *>(this->meshes.data_[meshIndex],
                                      this->transforms.data_[transformIndex]->meshes);
        this->transforms.data_[transformIndex]->bounds().Merge(
                *(const AbyssEngine::AEMath::BSphere *) &this->meshes.data_[meshIndex]->boundsCenterX);
        AbyssEngine::Transform *anim = this->meshes.data_[meshIndex]->animation;
        if (anim != 0) {
            if (this->transforms.data_[transformIndex]->animationLength < anim->animationLength) {
                this->transforms.data_[transformIndex]->SetAnimationLength(anim->animationLength);
            }
            this->transforms.data_[transformIndex]->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
        }
        this->transforms.data_[transformIndex]->CollectAnimationData();
    }
}

AbyssEngine::String PaintCanvas::GetReverseString(AbyssEngine::String in, bool reverse) {
    if (reverse) {
        String out("");
        for (unsigned int i = in.size() - 1; (int) i >= 0; --i) {
            out += in.SubString(i, i + 1);
        }
        return out;
    }
    return in;
}

void PaintCanvas::SpriteSystemCreate(unsigned short resId, bool flag,
                                     unsigned short matResId, unsigned int &out) {
    AbyssEngine::SpriteSystem *ss = 0;
    int ok = AbyssEngine::SpriteSystemCreate(this->engine, resId, flag, &ss);
    if (ok == 1) {
        unsigned int mat = 0xffffffff;
        this->MaterialCreate(matResId, mat);
        if (mat <= this->materials.count) {
            ::Node *node = ((PCSpriteSystemView2 *) ss)->node;
            node->spriteMaterial =
                    (unsigned int) (uintptr_t) this->materials.data_[mat];
        }
        for (unsigned int i = 0; i < this->spriteSystems.count; i++) {
            if (this->spriteSystems.data_[i] == nullptr) {
                this->spriteSystems.data_[i] = ss;
                ss = 0;
                out = i;
                break;
            }
        }
        if (ss != 0) {
            ArrayAdd<AbyssEngine::SpriteSystem *>(ss, this->spriteSystems);
            out = this->spriteSystems.count - 1;
        }
    } else {
        out = 0xffffffff;
    }
}

int PaintCanvas::GetScreenPosition(const AbyssEngine::AEMath::Matrix &srcMatrix,
                                   AbyssEngine::AEMath::Vector &outVec) {
    int visible = 0;
    if (this->currentCamera < this->cameras.count) {
        float tx = srcMatrix.m[3];
        float ty = srcMatrix.m[7];
        float tz = srcMatrix.m[11];
        AbyssEngine::AEMath::Vector position = {tx, ty, tz};
        outVec = position;

        // Perspective divide against the camera frustum, then map to pixels.
        AbyssEngine::Camera *camera = this->cameras.data_[this->currentCamera];
        float w = camera->field_0x4c * outVec.z;
        float h = outVec.z * camera->frustumTanHalfFov;
        if (w != 0.0f && h != 0.0f) {
            double px = (double) outVec.x * 0.5;
            outVec.x = (float) (-(px / (double) w) * (double) this->GetWidth() + (double) (this->GetWidth() >> 1));
            double py = (double) outVec.y * 0.5 / (double) h;
            outVec.y = (float) (py * (double) this->GetHeight() + (double) (this->GetHeight() >> 1));
            if (!(outVec.z <= this->cameras.data_[this->currentCamera]->position[1])) {
                return 0;
            }
            if (outVec.y >= 0.0f && outVec.x >= 0.0f && outVec.x < (float) this->GetWidth()) {
                visible = outVec.y < (float) this->GetHeight();
            }
        }
    }
    return visible;
}

void PaintCanvas::SpriteSystemCreate(unsigned short resId,
                                     bool flag, unsigned int &out) {
    AbyssEngine::SpriteSystem *ss = 0;
    unsigned int result;
    int ok = AbyssEngine::SpriteSystemCreate(this->engine, resId, flag, &ss);
    if (ok == 1) {
        unsigned int i;
        for (i = 0; i < this->spriteSystems.count; i++) {
            AbyssEngine::SpriteSystem **slot = &this->spriteSystems.data_[i];
            if (*slot == nullptr) {
                *slot = ss;
                ss = 0;
                out = i;
                return;
            }
        }
        if (ss == 0) {
            return;
        }
        ArrayAdd<AbyssEngine::SpriteSystem *>(
            static_cast<AbyssEngine::SpriteSystem *>(ss),
            *reinterpret_cast<Array<AbyssEngine::SpriteSystem *> *>(
                &this->spriteSystems));
        result = this->spriteSystems.count - 1;
    } else {
        result = 0xffffffff;
    }
    out = result;
}

void PaintCanvas::MaterialCreate(unsigned int &out, AbyssEngine::BlendMode mode,
                                 unsigned int textures, unsigned short p4) {
    (void) p4;
    AbyssEngine::Material *mat = new AbyssEngine::Material();
    mat->blendMode = (int)mode;
    mat->textures[0] = (int)textures;
    ArrayAdd<AbyssEngine::Material*>(mat, this->materials);
    out = this->materials.count - 1;
}

int PaintCanvas::CameraIsSphereinViewFrustum(const AbyssEngine::AEMath::Vector &point, float radius) {
    if (radius == 0.0f || this->currentCamera >= this->cameras.count) {
        return 1;
    }
    if (this->initialized == 0) {
        return AbyssEngine::CameraIsSphereinViewFrustum(point, radius, nullptr,
                                                        this->cameras.data_[this->currentCamera]);
    }
    AbyssEngine::AEMath::Matrix m;
    AbyssEngine::AEMath::MatrixIdentity(m);
    double *grav = this->engine->GetGravValue();
    float angle = (float) (grav[1] * (double) 1.57079637f);
    float rot = (this->gameOrientation == 1) ? angle : -angle;
    float s = AbyssEngine::AEMath::Sinf(rot);
    float c = AbyssEngine::AEMath::Cosf(rot);
    m.m[0] = c;
    m.m[5] = c;
    m.m[1] = -s;
    m.m[4] = s;
    return AbyssEngine::CameraIsSphereinViewFrustum(point, radius, &m, this->cameras.data_[this->currentCamera]);
}

void PaintCanvas::MeshChangeShaderAnimValue(AbyssEngine::Mesh *mesh, float value, unsigned int mode) {
    if (!mesh) {
        return;
    }
    if (mode == 1) {
        memcpy(&mesh->materialId, &value, sizeof(value));  // Mesh+0x1c holds a float anim value
    } else if (mode == 2) {
        mesh->shaderAnimValue0 = value;
    } else if (mode == 4) {
        mesh->shaderAnimValue1 = value;
    }
    return this->MeshChangeShaderAnimValue(mesh->animation, value, mode);
}

void PaintCanvas::Resume() {
    for (unsigned int i = 0; i < this->cubeTextures.count; i++) {
        unsigned int out = 0;
        PCCubeTexView *res = (PCCubeTexView *) (this->cubeTextures.data_)[i];
        char *path = reinterpret_cast<AbyssEngine::String *>(res->pathField)->GetAEChar();
        float f = ((PCCubeTexView *) (this->cubeTextures.data_)[i])->scale;
        int ok = AbyssEngine::TextureCreateFromFile(this->engine, path, nullptr, nullptr,
                                                    &out, false, f);
        if (ok == 1) {
            ((PCCubeTexView *) (this->cubeTextures.data_)[i])->glTexId = out;
        }
        delete[] path;
    }
    if (AbyssEngine::CubeMapSetIndex != 0) {
        glActiveTexture(0x84c7);
        PCCubeTexView *res = (PCCubeTexView *) (this->cubeTextures.data_)[AbyssEngine::CubeMapSetIndex];
        glBindTexture(0x8513, (unsigned int) res->glTexId);
        glActiveTexture(0x84c0);
    }
}

void PaintCanvas::TransformAddMeshId(unsigned int transformIndex, unsigned int meshIndex) {
    if (transformIndex >= this->transforms.count || meshIndex >= this->meshes.count) {
        return;
    }
    ArrayAdd<AbyssEngine::Mesh *>(this->meshes.data_[meshIndex], this->transforms.data_[transformIndex]->meshes);
    this->transforms.data_[transformIndex]->bounds().Merge(
            *(const AbyssEngine::AEMath::BSphere *) &this->meshes.data_[meshIndex]->boundsCenterX);

    AbyssEngine::Transform *animation = this->meshes.data_[meshIndex]->animation;
    if (animation != 0) {
        if (this->transforms.data_[transformIndex]->animationLength < animation->animationLength) {
            this->transforms.data_[transformIndex]->SetAnimationLength(animation->animationLength);
        }
        if (this->transforms.data_[transformIndex]->rangeStart == 0 ||
            this->meshes.data_[meshIndex]->animation->rangeStart < this->transforms.data_[transformIndex]->rangeStart) {
            this->transforms.data_[transformIndex]->rangeStart = this->meshes.data_[meshIndex]->animation->rangeStart;
        }
        this->transforms.data_[transformIndex]->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
    }
    this->transforms.data_[transformIndex]->CollectAnimationData();
}

AbyssEngine::String PaintCanvas::GetReverseString(AbyssEngine::String in) {
    return this->GetReverseString(AbyssEngine::String(in, false), this->field_0x1c == 0);
}

void PaintCanvas::GetAccelValue() {
    return paintcanvas_ext_get_accel(this->engine);
}

void PaintCanvas::ResetPersMatrix() {
    int w = this->engine->GetDisplayWidth();
    int h = this->engine->GetDisplayHeight();
    float s = AbyssEngine::AEMath::Sinf(AbyssEngine::i_fov * 0.5f);
    float c = AbyssEngine::AEMath::Cosf(AbyssEngine::i_fov * 0.5f);
    float aspect = (float) w / (float) h;
    float f = 1.0f / (s / c);

    float *m = this->projMatrix3d;
    memset(m, 0, sizeof(this->projMatrix3d));
    switch (this->gameOrientation) {
        case 0:
            m[4] = -f;
            m[1] = f / aspect;
            break;
        case 1:
            m[4] = f;
            m[1] = -(f / aspect);
            break;
        case 2:
            m[5] = f;
            m[0] = f / aspect;
            break;
        case 3:
            m[5] = -f;
            m[0] = -(f / aspect);
            break;
    }
    float zFar = AbyssEngine::i_zFar;
    float zNear = AbyssEngine::i_zNear;
    m[11] = -1.0f;
    m[10] = (zFar + zNear) / (zNear - zFar);
    m[14] = (float) ((2.0 * (double) zFar * (double) zNear) / (double) (zNear - zFar));

    if (Engine::enableShader != 0) {
        return this->engine->SetPerspMatrix(m);
    }
    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    glMatrixMode(0x1701);
    glLoadMatrixf(m);
    glMatrixMode(0x1700);
    return glLoadIdentity();
}

int PaintCanvas::CameraIsPointinViewFrustum(const AbyssEngine::AEMath::Vector &point) {
    if (this->currentCamera >= this->cameras.count) {
        return 1;
    }
    if (this->initialized == 0) {
        return AbyssEngine::CameraIsPointinViewFrustum(point, nullptr,
                                                       this->cameras.data_[this->currentCamera]);
    }
    AbyssEngine::AEMath::Matrix m;
    AbyssEngine::AEMath::MatrixIdentity(m);
    double *grav = this->engine->GetGravValue();
    float angle = (float) (grav[1] * (double) 1.57079637f);
    float rot = (this->gameOrientation == 1) ? angle : -angle;
    float s = AbyssEngine::AEMath::Sinf(rot);
    float c = AbyssEngine::AEMath::Cosf(rot);
    m.m[0] = c;
    m.m[5] = c;
    m.m[1] = -s;
    m.m[4] = s;
    return AbyssEngine::CameraIsPointinViewFrustum(point, &m, this->cameras.data_[this->currentCamera]);
}

void PaintCanvas::SetTexture(unsigned int first, unsigned int second) {
    return this->engine->SetTextures(first, second);
}

PaintCanvas::~PaintCanvas() {
    this->ReleaseAllResources();

    for (unsigned int i = 0; i < this->resources.count; i++) {
        PCResourceView *res = (PCResourceView *) this->resources.data_[i];
        if (res == 0) {
            continue;
        }
        switch (res->type) {
            case 1:
            case 3:
            case 6:
                operator delete(res->payload);
                break;
            case 2:
                delete (AbyssEngine::ResourceTexture *) res->payload;
                break;
            case 4:
                delete (AbyssEngine::ResourceMesh *) res->payload;
                break;
            case 5:
                delete (AbyssEngine::ResourceTransform *) res->payload;
                break;
        }
        delete (PCResourceView *) this->resources.data_[i];
        this->resources.data_[i] = 0;
        ArrayRelease(this->glowMeshes);
        ArrayRelease(this->glowMatA);
        ArrayRelease(this->glowMatB);
        ArrayRelease(this->glowUints);
        ArrayRelease(this->glowMatC);
    }

    AbyssEngine::MeshRelease(this->engine, (AbyssEngine::Mesh **) &this->quad2dMesh);
    AbyssEngine::MeshRelease(this->engine, (AbyssEngine::Mesh **) &this->lineMesh);

    for (unsigned int i = 0; i < this->cubeTextures.count; i++) {
        delete this->cubeTextures.data_[i];
        this->cubeTextures.data_[i] = 0;
    }
}

void PaintCanvas::Image2DCreate(unsigned short resId, unsigned int &out) {
    PCResourceView *res = (PCResourceView *) this->FindResource(resId);
    if (res == 0) {
        return;
    }
    // Payload: texture resource id, region index.
    unsigned short *info = (unsigned short *) res->payload;
    PCResourceView *texres = (PCResourceView *) this->FindResource(info[0]);
    if (texres == 0) {
        return;
    }
    if (texres->handle == -1) {
        unsigned int texture = 0;
        this->TextureCreate(info[0], texture, false);
    }
    unsigned int idx = (unsigned int) res->handle;
    if (idx == 0xffffffff) {
        PCImage2DView *img = new PCImage2DView();
        if (AbyssEngine::ImageCreateRegionFromFile(this->engine, *(char **) texres->payload, info[1],
                                                   (AbyssEngine::Image2D *) img) != 1) {
            return;
        }
        if (texres->handle != -1) {
            img->textureId = (uint32_t) texres->handle;
        }
        ArrayAdd<AbyssEngine::Image2D *>((AbyssEngine::Image2D *) img, this->images);
        idx = this->images.count - 1;
        res->handle = (int) idx;
    }
    out = idx;
}


int PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Matrix &m,
                                   const AbyssEngine::AEMath::Vector &worldPos,
                                   AbyssEngine::AEMath::Vector &outVec) {
    AbyssEngine::AEMath::Vector transformed = AbyssEngine::AEMath::MatrixTransformVector(m, worldPos);
    if (this->currentCamera >= this->cameras.count) {
        return 0;
    }
    if (this->initialized != 0) {
        AbyssEngine::AEMath::Matrix roll;
        AbyssEngine::AEMath::MatrixIdentity(roll);
        float angle = (float) (this->engine->GetGravValue()[1] * 1.5707963705062866);
        float rot = (this->gameOrientation == 1) ? angle : -angle;
        float s = AbyssEngine::AEMath::Sinf(rot);
        float c = AbyssEngine::AEMath::Cosf(rot);
        roll.m[0] = c;
        roll.m[5] = c;
        roll.m[1] = -s;
        roll.m[4] = s;
        AbyssEngine::AEMath::Matrix view = this->cameras.data_[this->currentCamera]->projection;
        view *= roll;
        outVec = AbyssEngine::AEMath::MatrixInverseTransformVector(view, transformed);
    } else {
        outVec = AbyssEngine::AEMath::MatrixInverseTransformVector(
                this->cameras.data_[this->currentCamera]->projection, transformed);
    }
    AbyssEngine::Camera *camera = this->cameras.data_[this->currentCamera];
    if (outVec.z > camera->position[1]) {
        return 0;
    }
    int visible = 0;
    float w = outVec.z * camera->field_0x4c;
    float h = outVec.z * camera->frustumTanHalfFov;
    if (w != 0.0f && h != 0.0f) {
        double px = (double) outVec.x * 0.5;
        outVec.x = (float) (-(px / (double) w) * (double) this->GetWidth() + (double) (this->GetWidth() >> 1));
        double py = (double) outVec.y * 0.5 / (double) h;
        outVec.y = (float) (py * (double) this->GetHeight() + (double) (this->GetHeight() >> 1));
        if (outVec.x >= 0.0f && outVec.y >= 0.0f && outVec.x < (float) this->GetWidth()) {
            visible = outVec.y < (float) this->GetHeight();
        }
    }
    return visible;
}

int PaintCanvas::ResourceLoaded(unsigned int index, AbyssEngine::ResourceType type) {
    switch (type) {
        case 1:
            return index < this->fonts.count;
        case 2: {
            PCResourceView *res = (PCResourceView *) (this->resources.data_)[index];
            if (res->type == 2) {
                return res->handle != -1;
            }
            return 0;
        }
        case 3:
            return index < this->images.count;
        case 4:
            return index < this->meshes.count;
        case 5:
            return index < this->transforms.count;
        case 6:
            return index < this->materials.count;
        default:
            return 0;
    }
}

int PaintCanvas::TransformGetTriCount(unsigned int index) {
    if (index < this->transforms.count) {
        PCTransformView *t = (PCTransformView *) this->transforms.data_[index];
        return this->TransformGetTriCount((AbyssEngine::Transform *) t);
    }
    return 0;
}

void PaintCanvas::SpriteSystemSetPosition(unsigned int index, unsigned short sub,
                                          float x, float y, float z) {
    if (index < this->spriteSystems.count) {
        PCSpriteSystemView *s = (PCSpriteSystemView *) (this->spriteSystems.data_)[index];
        if (s) {
            if (sub < (unsigned short) s->count) {
                float *p = (float *) (s->positions + sub * 12);
                p[0] = x;
                p[1] = y;
                p[2] = z;
            }
        }
    }
}

void PaintCanvas::DrawImage2D(unsigned int index, int x, int y) {
    if (index < this->images.count) {
        PCImage2DView *img = (PCImage2DView *) (this->images.data_)[index];
        if (img->restoreFlag) {
            this->RestoreImage2D((AbyssEngine::Image2D *) img);
            img = (PCImage2DView *) (this->images.data_)[index];
        }
        this->SetTexture(img->textureId, -1);

        AbyssEngine::AEMath::Matrix m;
        m.m[7] = (float) y;
        m.m[3] = (float) x;
        this->SetWorldViewMatrix(m);
        glDisable(0xb44);
        AbyssEngine::MeshDraw(this->engine,
                              (AbyssEngine::Mesh *) ((PCImage2DView *) (this->images.data_)[index])->mesh);
        glEnable(0xb44);
    }
}

PaintCanvas::PaintCanvas(AbyssEngine::Engine *engine) {
    this->field_0x20 = 0;
    this->fogMode = 0;
    this->fogEnableFlag = 1;
    this->culledCount = 0;
    this->initialized = 0;
    this->engine = engine;
    this->currentCamera = 0xffffffff;

    AbyssEngine::MeshCreate(engine, 4, 2, 0x11, (AbyssEngine::Mesh **) &this->lineMesh);
    unsigned short *quad = (unsigned short *) ((PCMeshView *) this->lineMesh)->indexBuffer;
    quad[0] = 0;
    quad[1] = 2;
    quad[2] = 1;
    quad[3] = 0;
    quad[4] = 3;
    quad[5] = 2;
    this->colorR = 1.0f;
    this->colorG = 1.0f;
    this->colorB = 1.0f;
    this->colorA = 1.0f;
    this->field_0x1c = 1;
    engine->statsBucketFlag = 1;

    AbyssEngine::MeshCreate(engine, 400, 200, 0x1b, (AbyssEngine::Mesh **) &this->quad2dMesh);
    unsigned short *buf = (unsigned short *) ((PCMeshView *) this->quad2dMesh)->indexBuffer;
    for (int i = 0; i < 100; i++) {
        unsigned short *e = buf + i * 6;
        e[0] = (unsigned short) (i << 2);
        e[1] = (unsigned short) ((i << 2) | 2);
        e[2] = (unsigned short) ((i << 2) | 1);
        e[3] = (unsigned short) (i << 2);
        e[4] = (unsigned short) ((i << 2) | 3);
        e[5] = (unsigned short) ((i << 2) | 2);
    }
    this->field_0x1cc = 0;
    this->field_0x1f8 = 1;
}

void PaintCanvas::TransformRemoveMesh(unsigned int transformIndex, unsigned short meshResId) {
    if (this->transforms.count <= transformIndex) {
        return;
    }
    char *x = (char *) this->FindResource(meshResId);
    if (x) {
        return this->TransformRemoveMeshId(transformIndex, (unsigned int) ((PCResourceView *) x)->handle);
    }
}

void PaintCanvas::ClearBuffer(unsigned int mask) {
    glEnable(0xb71);
    glDepthMask(1);
    return this->engine->ClearBuffer(mask);
}

void PaintCanvas::ClearDepth() {
    glClear(0x100);
}

void PaintCanvas::DrawFrameBufferTexture(int, int, int, int, int, int) {
}

bool PaintCanvas::WarmUpTexture() {
    return true;
}

void PaintCanvas::TransformRemoveChild(unsigned int parent, unsigned int child) {
    if (parent != child) {
        unsigned int count = this->transforms.count;
        if (child < count && parent < count) {
            char **arr = (char **) this->transforms.data_;
            AbyssEngine::Transform *p = (AbyssEngine::Transform *) arr[parent];
            AbyssEngine::Transform *c = (AbyssEngine::Transform *) arr[child];
            ArrayRemove<AbyssEngine::Transform *>(c, p->children);
            arr = (char **) this->transforms.data_;
            return ((AbyssEngine::Transform*)arr[parent])->CollectAnimationData();
        }
    }
}

void PaintCanvas::TextureCreate(unsigned short resId, void (*callback)(AbyssEngine::Image *, void *), // lint: void_ptr (external symbol; mangling must match lib)
                                void *userData, unsigned int &out, bool useCallbackLoader) { // lint: void_ptr (external symbol; mangling must match lib)
    Engine *eng = (Engine *) this->engine;
    eng->boundTextures[0] = -1;
    eng->boundTextures[1] = -1;

    PCResourceView *res = (PCResourceView *) this->FindResource(resId);
    if (res == 0) {
        return;
    }
    if (res->handle != -1) {
        out = (unsigned int) res->handle;
        return;
    }
    AbyssEngine::ResourceTexture *info = (AbyssEngine::ResourceTexture *) res->payload;
    unsigned int texture = 0;
    int ok;
    if (useCallbackLoader) {
        ok = AbyssEngine::TextureCreateFromFile(this->engine, info->name, callback, userData,
                                                &texture, true, info->value);
    } else {
        ok = AbyssEngine::TextureCreateFromFileIntern(this->engine, info->name, callback, userData,
                                                      &texture, info->value, nullptr, true);
    }
    if (ok != 1) {
        return;
    }
    res->handle = (int) texture;
    out = texture;
}

void PaintCanvas::TextureCreate(unsigned short id, unsigned int &out, bool flag) {
    TextureCreate(id, nullptr, nullptr, out, flag);
}

void PaintCanvas::SwapBuffer() {
}

void PaintCanvas::StartDraw2FBO() {
    return this->engine->ActivateRender2TextureFBO();
}

void PaintCanvas::Suspend() {
    char texId[4];
    for (unsigned int i = 0; i < this->cubeTextures.count; i++) {
        int *p = (int *) (this->cubeTextures.data_)[i];
        int val = *p;
        *(int *) texId = val;
        if (val != -1) {
            glDeleteTextures(1, (const GLuint*)texId);
            p = (int *) (this->cubeTextures.data_)[i];
        }
        *p = -1;
    }
}

void PaintCanvas::DrawTransform(Transform *tf, const AbyssEngine::AEMath::Matrix &m2,
                                AbyssEngine::AEMath::Matrix &m3) {
    if (tf && tf->visible) {
        AbyssEngine::AEMath::Matrix world = m2 * tf->worldMatrix;
        if (tf->keyFrames.size() != 0) {
            world *= tf->rotationMatrix;
        }
        for (unsigned int i = 0; i < tf->meshes.size(); i++) {
            this->DrawMesh(tf->meshes[i], world, m3, tf->color, tf->localMatrix);
        }
        for (unsigned int i = 0; i < tf->children.size(); i++) {
            if (this->currentCamera < this->cameras.count &&
                tf->children[i]->InCameraVF(&world, (this->cameras.data_)[this->currentCamera])) {
                this->DrawTransform(tf->children[i], world, m3);
            } else {
                this->culledCount += 1;
            }
        }
    }
}

void PaintCanvas::DrawImage2D(unsigned int index, int x, int y,
                              int w, int h, unsigned char alignFlags, unsigned char anchorFlags,
                              unsigned char flipFlags) {
    if (index >= this->images.count) {
        return;
    }
    PCImage2DView *img = (PCImage2DView *) this->images.data_[index];
    if (img->restoreFlag != 0) {
        this->RestoreImage2D((AbyssEngine::Image2D *) img);
        img = (PCImage2DView *) this->images.data_[index];
    }
    this->SetTexture(img->textureId, (unsigned int) -1);

    // Horizontal screen alignment: left / centre / right edge.
    int hOff;
    int baseX = x;
    if ((alignFlags & 7) == 2) {
        baseX = -x;
        hOff = this->GetWidth();
    } else if ((alignFlags & 7) == 4) {
        hOff = this->GetWidth() >> 1;
    } else {
        hOff = 0;
    }

    // Bit 3 / bit 7: stretch to the right / bottom screen edge instead of using w / h.
    int spanW = w;
    if (alignFlags & 8) {
        spanW = this->GetWidth() - (w + x);
    }
    float scaleX = (float) spanW / ((float *) ((PCImage2DView *) this->images.data_[index])->mesh->positions)[3];
    int spanH = h;
    if (alignFlags & 0x80) {
        spanH = this->GetHeight() - (h + y);
    }
    float scaleY = (float) spanH / ((float *) ((PCImage2DView *) this->images.data_[index])->mesh->positions)[7];

    // Vertical screen alignment: top / centre / bottom edge.
    int baseY = y;
    int vOff;
    if ((alignFlags & 0x70) == 0x20) {
        vOff = this->GetHeight();
        baseY = -y;
    } else if ((alignFlags & 0x70) == 0x40) {
        vOff = this->GetHeight() >> 1;
    } else {
        vOff = 0;
    }

    // Image anchor point.
    int anchorX;
    if ((anchorFlags & 7) == 2) {
        if (alignFlags & 8) {
            anchorX = (w + x) - this->GetWidth();
        } else {
            anchorX = -w;
        }
    } else if ((anchorFlags & 7) == 4) {
        if (alignFlags & 8) {
            anchorX = ((w + x) - this->GetWidth()) >> 1;
        } else {
            anchorX = -(w >> 1);
        }
    } else {
        anchorX = 0;
    }
    int anchorY;
    if ((anchorFlags & 0x70) == 0x20) {
        if (alignFlags & 0x80) {
            anchorY = (h + y) - this->GetHeight();
        } else {
            anchorY = -h;
        }
    } else if ((anchorFlags & 0x70) == 0x40) {
        if (alignFlags & 0x80) {
            anchorY = ((h + y) - this->GetHeight()) >> 1;
        } else {
            anchorY = -(h >> 1);
        }
    } else {
        anchorY = 0;
    }

    AbyssEngine::AEMath::Matrix m;
    m.m[3] = (float) (baseX + hOff + anchorX);
    m.m[7] = (float) (vOff + baseY + anchorY);
    m.m[0] = scaleX;
    m.m[5] = scaleY;
    if (flipFlags & 1) {
        m.m[0] = -scaleX;
        m.m[3] = (float) w + m.m[3];
    }
    if (flipFlags & 2) {
        m.m[5] = -scaleY;
        m.m[7] = (float) h + m.m[7];
    }
    this->SetWorldViewMatrix(m);
    glDisable(0xb44);
    AbyssEngine::MeshDraw(this->engine, (AbyssEngine::Mesh *) ((PCImage2DView *) this->images.data_[index])->mesh);
    glEnable(0xb44);
}

void PaintCanvas::CameraSetPerspective(unsigned int index, float fov, float aspect) {
    if (index < this->cameras.count) {
        float w = (float) this->GetWidth();
        float h = (float) this->GetHeight();
        AbyssEngine::Camera *cam = (this->cameras.data_)[index];
        AbyssEngine::CameraSetPerspective(fov, aspect, w, h, cam);
        if (this->currentCamera == index) {
            return this->CameraSetCurrent(index);
        }
    }
}

void PaintCanvas::DrawRegion2D(unsigned int index, float /*unused*/, int rotSteps,
                               int pivotX, int pivotY, int transX, float scaleX, float scaleY) {
    if (index >= this->images.count) {
        return;
    }
    PCImage2DView *img = (PCImage2DView *) ((char **) this->images.data_)[index];
    if (img->restoreFlag != 0) {
        paintcanvas_ext_dr2_restore(img->restoreFlag, img);
        img = (PCImage2DView *) ((char **) this->images.data_)[index];
    }
    paintcanvas_ext_dr2_settexture(this, img->textureId);

    float rot = paintcanvas_ext_dr2_signedtofloat(rotSteps, 0);
    float px = paintcanvas_ext_dr2_signedtofloat(pivotX, 0);
    float py = paintcanvas_ext_dr2_signedtofloat(pivotY, 0);
    float tx = paintcanvas_ext_dr2_signedtofloat(transX, 0);
    float negpx = paintcanvas_ext_dr2_signedtofloat(-pivotX, 0);
    float negpy = paintcanvas_ext_dr2_signedtofloat(-pivotY, 0);

    float transM[16];
    memset(transM, 0, sizeof(transM));
    transM[0] = 1.0f;
    transM[5] = 1.0f;
    transM[10] = 1.0f;
    transM[15] = 1.0f;
    transM[3] = py + rot * scaleX;
    transM[7] = tx + px * scaleY;

    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f;
    pivotM[5] = 1.0f;
    pivotM[10] = 1.0f;
    pivotM[15] = 1.0f;
    pivotM[3] = negpx * scaleX;
    pivotM[7] = negpy * scaleY;

    float s = paintcanvas_ext_dr2_sinf(negpx * scaleX);
    float c = paintcanvas_ext_dr2_cosf(negpx * scaleX);
    float rotM[16];
    memset(rotM, 0, sizeof(rotM));
    rotM[0] = c;
    rotM[5] = c;
    rotM[10] = 1.0f;
    rotM[15] = 1.0f;
    *(unsigned int *) &rotM[1] = *(unsigned int *) &s ^ 0x80000000;
    rotM[4] = s;

    float scaleBuf[16];
    paintcanvas_ext_dr2_setscaling(scaleBuf, scaleX, scaleY, 1.0f);

    float composed[16];
    paintcanvas_ext_dr2_mtx_mul(scaleBuf, rotM, transM);
    paintcanvas_ext_dr2_mtx_assign(composed, scaleBuf);
    float scratch[16];
    paintcanvas_ext_dr2_mtx_mul(scratch, pivotM, composed);
    paintcanvas_ext_dr2_mtx_mul(scaleBuf, scratch, pivotM);
    paintcanvas_ext_dr2_mtx_assign(composed, scaleBuf);

    paintcanvas_ext_dr2_setwvm(this, composed);
    paintcanvas_ext_dr2_gldisable(0xb44);
    paintcanvas_ext_dr2_meshdraw(this->engine,
                                 ((PCImage2DView *) ((char **) this->images.data_)[index])->mesh);
    paintcanvas_ext_dr2_glenable(0xb44);
}

void PaintCanvas::MeshSetTriangle(unsigned int meshIndex, unsigned short tri,
                                  unsigned short v0, unsigned short v1, unsigned short v2) {
    if (meshIndex < this->meshes.count) {
        PCMeshView *mesh = (PCMeshView *) this->meshes.data_[meshIndex];
        if (tri * 3 < mesh->indexCount) {
            unsigned short *buf = (unsigned short *) mesh->indexBuffer;
            buf[tri * 3] = v0;
            buf[tri * 3 + 1] = v1;
            buf[tri * 3 + 2] = v2;
        }
    }
}



static void zero16(void *p) { // lint: void_ptr (external symbol; mangling must match lib)
    memset(p, 0, 0x10);
}

void PaintCanvas::SetGameOrientation(AbyssEngine::LandscapeMode orientation) {
    if (this->gameOrientation == orientation) {
        return;
    }
    this->gameOrientation = orientation;
    this->engine->SetScreenOrientation(orientation);

    this->projMatrix3d[1] = -this->projMatrix3d[1];
    this->projMatrix3d[4] = -this->projMatrix3d[4];

    float fw = (float) (int) this->engine->GetDisplayWidth();
    float fh = (float) (int) this->engine->GetDisplayHeight();
    float *proj = this->projOrthoMatrix;
    float *view = this->worldViewMatrix;
    // 2D ortho projection plus a view matrix that rotates the canvas into the device orientation.
    switch (orientation) {
        case 0:
            memset(proj, 0, sizeof(this->projOrthoMatrix));
            proj[13] = 1.0f;
            proj[15] = 1.0f;
            memset(view, 0, sizeof(this->worldViewMatrix));
            view[1] = -1.0f;
            proj[10] = -0.05f;
            proj[12] = -1.0f;
            view[10] = 1.0f;
            view[4] = 1.0f;
            proj[0] = 2.0f / fh;
            proj[5] = -(2.0f / fw);
            view[15] = 1.0f;
            view[13] = (float) (int) this->engine->GetDisplayWidth();
            break;
        case 1:
            memset(proj, 0, sizeof(this->projOrthoMatrix));
            proj[13] = 1.0f;
            proj[15] = 1.0f;
            memset(view, 0, sizeof(this->worldViewMatrix));
            view[1] = 1.0f;
            proj[10] = -0.05f;
            proj[12] = -1.0f;
            view[10] = 1.0f;
            view[4] = -1.0f;
            proj[0] = 2.0f / fh;
            proj[5] = -(2.0f / fw);
            view[15] = 1.0f;
            view[12] = (float) (int) this->engine->GetDisplayHeight();
            break;
        case 3:
            memset(proj, 0, sizeof(this->projOrthoMatrix));
            proj[13] = 1.0f;
            proj[15] = 1.0f;
            proj[10] = -0.05f;
            proj[12] = -1.0f;
            proj[0] = 2.0f / fw;
            proj[5] = -(2.0f / fh);
            memset(view, 0, sizeof(this->worldViewMatrix));
            view[0] = -1.0f;
            view[5] = -1.0f;
            view[10] = 1.0f;
            view[15] = 1.0f;
            view[12] = (float) (int) this->engine->GetDisplayWidth();
            view[13] = (float) (int) this->engine->GetDisplayHeight();
            break;
        default:
            memset(proj, 0, sizeof(this->projOrthoMatrix));
            proj[13] = 1.0f;
            proj[15] = 1.0f;
            proj[10] = -0.05f;
            proj[12] = -1.0f;
            proj[0] = 2.0f / fw;
            proj[5] = -(2.0f / fh);
            memset(view, 0, sizeof(this->worldViewMatrix));
            view[0] = 1.0f;
            view[5] = 1.0f;
            view[10] = 1.0f;
            view[15] = 1.0f;
            break;
    }

    if (this->currentCamera != 0xffffffff) {
        AbyssEngine::Camera *camera = this->cameras.data_[this->currentCamera];
        this->SetProjectionMatrix3d(camera->position[0], camera->position[1], camera->position[2]);
    }
}

void PaintCanvas::MeshCreate(unsigned short vertexCount, unsigned short triangleCount,
                             signed char meshType, unsigned short matResId, unsigned int &out) {
    unsigned int mat = 0xffffffff;
    this->MaterialCreate(matResId, mat);
    AbyssEngine::Mesh *mesh = 0;
    int ok = AbyssEngine::MeshCreate(this->engine, vertexCount, triangleCount, meshType, &mesh);
    int result = -1;
    if (ok == 1) {
        if (mat <= this->materials.count) {
            mesh->material = this->materials.data_[mat];
        }
        ArrayAdd<AbyssEngine::Mesh *>(mesh, this->meshes);
        result = this->meshes.count - 1;
    }
    out = (unsigned int) result;
}

static const float g_dm_255_8ee80 = 0;

static unsigned int mulColors(unsigned int a, unsigned int b) {
    unsigned int r = (((a >> 16) & 0xff) * ((b >> 16) & 0xff)) & 0xffffff00;
    unsigned int g = (((a >> 8) & 0xff) * ((b >> 8) & 0xff)) & 0xffffff00;
    unsigned int bl = ((a & 0xff) * (b & 0xff)) >> 8;
    unsigned int al = (((a >> 24) & 0xff) * ((b >> 24) & 0xff)) & 0xffffff00;
    return (bl) | (g << 8) | (r << 8) | (al << 16);
}

void PaintCanvas::DrawMesh(AbyssEngine::Mesh *mesh, AbyssEngine::AEMath::Matrix &worldMatrix,
                           AbyssEngine::AEMath::Matrix &viewMatrix, unsigned int color,
                           AbyssEngine::AEMath::Matrix &uvMatrix) {
    AbyssEngine::AEMath::Matrix worldM;
    AbyssEngine::AEMath::Matrix uvM;
    paintcanvas_ext_dm_memcpy(&worldM, worldMatrix, 0x3c);
    paintcanvas_ext_dm_memcpy(&uvM, uvMatrix, 0x3c);

    unsigned int meshColor = color;

    if (mesh->animation != 0) {
        Transform *resTf = mesh->animation;
        float a[16], b[16];
        memset(a, 0, sizeof(a));
        a[0] = 1.0f;
        a[5] = 1.0f;
        a[14] = 1.0f;
        memset(b, 0, sizeof(b));
        b[0] = 1.0f;
        b[5] = 1.0f;
        b[14] = 1.0f;

        float pivotM[16];
        paintcanvas_ext_dm_settrans(pivotM, mesh->pivotY);
        paintcanvas_ext_dm_settrans(pivotM, -mesh->pivotY);
        memset(pivotM, 0, sizeof(pivotM));
        pivotM[0] = 1.0f;
        pivotM[5] = 1.0f;
        pivotM[14] = 1.0f;

        float posM[16];
        float resLocal[16];
        paintcanvas_ext_dm_getpos(posM);
        paintcanvas_ext_dm_settrans_vec(resLocal, pivotM);
        paintcanvas_ext_dm_memcpy(resLocal, resTf->rotationMatrix, 0x3c);

        float s1[16], s2[16], s3[16];
        *(AbyssEngine::AEMath::Matrix*)s1 = worldMatrix * *(const AbyssEngine::AEMath::Matrix*)pivotM;
        *(AbyssEngine::AEMath::Matrix*)s2 = *(const AbyssEngine::AEMath::Matrix*)s1 * *(const AbyssEngine::AEMath::Matrix*)b;
        *(AbyssEngine::AEMath::Matrix*)s3 = *(const AbyssEngine::AEMath::Matrix*)s2 * *(const AbyssEngine::AEMath::Matrix*)resLocal;
        *(AbyssEngine::AEMath::Matrix*)posM = *(const AbyssEngine::AEMath::Matrix*)s3 * *(const AbyssEngine::AEMath::Matrix*)a;
        worldM = *(const AbyssEngine::AEMath::Matrix*)posM;

        unsigned int resColor = resTf->color;
        uvM = resTf->localMatrix;
        meshColor = mulColors(resColor, color);
    }

    paintcanvas_ext_dm_transformvec(&uvM, &worldM);

    float ext[3];
    ext[0] = mesh->boundsRadius;
    ext[1] = ext[0];
    ext[2] = ext[0];
    paintcanvas_ext_dm_rotatevec(&uvM, &worldM);
    paintcanvas_ext_dm_vec_assign(ext, &uvM);

    float ax = ext[0] < 0.0f ? -ext[0] : ext[0];
    float ay = ext[1] < 0.0f ? -ext[1] : ext[1];
    float az = ext[2] < 0.0f ? -ext[2] : ext[2];
    float maxxy = ax > ay ? ax : ay;
    float radius = maxxy;

    if (mesh->vertexCount != 0) {
        float r2 = maxxy > az ? maxxy : az;
        int vis = paintcanvas_ext_dm_spherefrustum(this, &uvM, r2 * mesh->boundsRadiusSq);
        if (vis == 0) {
            this->culledCount += 1;
            return;
        }
        if (mesh->vertexCount != 0) {
            if (mesh->material == 0) {
                float fr = paintcanvas_ext_dm_unsignedtofloat(meshColor >> 24, 0);
                float fg = paintcanvas_ext_dm_unsignedtofloat((meshColor >> 16) & 0xff, 0);
                float fb = paintcanvas_ext_dm_unsignedtofloat((meshColor >> 8) & 0xff, 0);
                float fa = paintcanvas_ext_dm_unsignedtofloat(meshColor & 0xff, 0);
                ((AbyssEngine::Engine*)this->engine)->SetColor(
                    (this->colorR * fr) / g_dm_255_8ee80,
                    (this->colorG * fg) / g_dm_255_8ee80,
                    (this->colorB * fb) / g_dm_255_8ee80,
                    (this->colorA * fa) / g_dm_255_8ee80);

                worldM *= viewMatrix;
                paintcanvas_ext_dm_setwvm(this, &worldM);
                paintcanvas_ext_dm_setmodelmatrix(this->engine);
                ((AbyssEngine::Engine*)this->engine)->SetUVMatrix(uvM);
                paintcanvas_ext_dm_meshdraw(this->engine, mesh);
                ((AbyssEngine::Engine*)this->engine)->ResetUVMatrix();
            } else {
                AbyssEngine::Material *res = (AbyssEngine::Material *) mesh->material;
                ArrayAddCached<AbyssEngine::Mesh*>(mesh, res->meshes);
                ArrayAddCached<AbyssEngine::AEMath::Matrix>(worldM, res->arr_2c);
                ArrayAddCached<AbyssEngine::AEMath::Matrix>(uvM, res->arr_38);
                ArrayAddCached<AbyssEngine::AEMath::Matrix>(viewMatrix, res->arr_5c);
                ArrayAddCached<uint32_t>(meshColor, res->arr_50);
            }
        }
    }

    if (mesh->animation != 0) {
        Transform *res = mesh->animation;
        unsigned int n = res->meshes.size();
        while ((int) (--n) >= 0) {
            this->DrawMesh(res->meshes[n], worldM, viewMatrix, color, uvM);
        }
        for (unsigned int i = 0; i < res->children.size(); i++) {
            if (this->currentCamera < this->cameras.count) {
                AbyssEngine::Camera *cam = this->cameras.data_[this->currentCamera];
                Transform *tf = res->children[i];
                if (tf->InCameraVF(&worldMatrix, cam)) {
                    this->DrawTransform(res->children[i], worldM, viewMatrix);
                }
            }
        }
    }
}

void PaintCanvas::Begin2d() {
    ((Engine *) this->engine)->statsEnabled = 1;
    glDisable(0xb71);
    glDepthMask(0);
    glEnable(0xbe2);
    glBlendFunc(0x302, 0x303);
    this->engine->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    this->engine->GlEnable(0xde1, true);
    if (Engine::enableShader != 0) {
        this->engine->SetOrthoMatrix(this->projOrthoMatrix, this->worldViewMatrix,
                                     this->gameOrientation != 2);
    } else {
        glTexEnvi(0x2300, 0x2200, 0x2100);
        glMatrixMode(0x1702);
        glLoadIdentity();
        glScalef(1.0f, 1.0f, 1.0f);
        glMatrixMode(0x1701);
        glLoadMatrixf(this->projOrthoMatrix);
        if (this->gameOrientation != 2) {
            glMultMatrixf(this->worldViewMatrix);
        }
        glMatrixMode(0x1700);
        glLoadIdentity();
    }
    this->field_0xc = 0;
}

void PaintCanvas::DrawString(unsigned int index, const unsigned short *str,
                             int x, int y, bool b) {
    if (index < this->fonts.count) {
        PCFontView *font = (PCFontView *) (this->fonts.data_)[index];
        this->SetTexture((unsigned int) font->texture, (unsigned int)-1);
        AbyssEngine::ImageFont *font2 = (this->fonts.data_)[index];
        AbyssEngine::ImageFontDrawString(font2, str, x, y, this, this->engine, b);
    }
}

static const char g_dsc_tag_sep[] = "c"; // colour-tag separator (orig .rodata 0x1ca491)

static const char g_dsc_fmt_882ee[] = "%x";


void PaintCanvas::DrawStringColor(unsigned int index, const AbyssEngine::String &text,
                                  int x, int y, bool b) {
    if (index >= this->fonts.count) {
        return;
    }
    PCFontView *font0 = (PCFontView *) this->fonts.data_[index];
    this->SetTexture((unsigned int) font0->texture, (unsigned int) -1);
    unsigned int savedColor = this->GetColor();

    // "c"-separated segments alternate between text and a hex colour ("" restores the colour).
    AbyssEngine::String str(text, false);
    ::Array<AbyssEngine::String *> *parts = str.SplitTags(AbyssEngine::String(g_dsc_tag_sep, false));
    if (parts != 0) {
        bool draw = true;
        for (unsigned int i = 0; i < parts->count; i++) {
            if (draw) {
                AbyssEngine::ImageFontDrawString(this->fonts.data_[index], (unsigned short *) *parts->data_[i],
                                                 parts->data_[i]->size(), x, y, this, this->engine, b);
                x += this->GetTextWidth(index, *parts->data_[i]);
            } else if (parts->data_[i]->size() == 0) {
                this->SetColor(savedColor);
            } else {
                unsigned int color = 0;
                sscanf(parts->data_[i]->GetAEChar(), g_dsc_fmt_882ee, &color);
                this->SetColor(color);
            }
            draw = !draw;
        }
        this->SetColor(savedColor);
        ArrayReleaseClasses<AbyssEngine::String *>(*parts);
        delete parts;
    }
}

void PaintCanvas::SetMatForGlow(AbyssEngine::Material *glowSource) {
    for (unsigned int i = 0; i < glowSource->meshes.count; i++) {
        ArrayAdd<AbyssEngine::Mesh *>(glowSource->meshes.data_[i],
                                      this->glowMeshes);

        ArrayAdd<AbyssEngine::AEMath::Matrix>(glowSource->arr_2c.data_[i], this->glowMatA);
        ArrayAdd<AbyssEngine::AEMath::Matrix>(glowSource->arr_38.data_[i], this->glowMatB);
        ArrayAdd<unsigned int>(glowSource->arr_50.data_[i], this->glowUints);
        ArrayAdd<AbyssEngine::AEMath::Matrix>(glowSource->arr_5c.data_[i], this->glowMatC);
    }
}

void PaintCanvas::EnableClip(int x, int y, int w, int h) {
    glEnable(0xc11);
    // The scissor box is given in game coordinates; rotate it into the GL framebuffer orientation.
    switch (this->gameOrientation) {
        case 0: {
            int t = x;
            x = y;
            y = t;
            t = h;
            h = w;
            w = t;
            break;
        }
        case 1: {
            int nx = this->GetHeight() - (y + h);
            int ny = this->GetWidth() - (x + w);
            int t = h;
            h = w;
            w = t;
            x = nx;
            y = ny;
            break;
        }
        case 2:
            y = this->GetHeight() - (y + h);
            break;
        case 3:
            x = this->GetWidth() - (x + w);
            break;
    }
    glScissor(x, y, w, h);
}

void PaintCanvas::DrawImage2D(unsigned int index, int x, int y,
                              unsigned char regionAlignFlags, unsigned char placeFlags) {
    if (index >= this->images.count) {
        return;
    }
    PCImage2DView *img = (PCImage2DView *) this->images.data_[index];
    if (img->restoreFlag != 0) {
        this->RestoreImage2D((AbyssEngine::Image2D *) img);
    }

    // Screen anchor (low nibble: horizontal, high nibble: vertical; 4 = centre, 2 = far edge).
    int anchorX;
    if ((regionAlignFlags & 7) == 2) {
        anchorX = this->GetWidth();
        x = -x;
    } else if ((regionAlignFlags & 7) == 4) {
        anchorX = this->GetWidth() >> 1;
    } else {
        anchorX = 0;
    }
    int anchorY;
    if ((regionAlignFlags & 0x70) == 0x20) {
        anchorY = this->GetHeight();
        y = -y;
    } else if ((regionAlignFlags & 0x70) == 0x40) {
        anchorY = this->GetHeight() >> 1;
    } else {
        anchorY = 0;
    }

    // Image pivot, from the quad's own extent.
    int pivotX;
    if ((placeFlags & 7) == 2) {
        float *quad = (float *) ((PCImage2DView *) this->images.data_[index])->mesh->positions;
        pivotX = -(int) quad[3];
    } else if ((placeFlags & 7) == 4) {
        float *quad = (float *) ((PCImage2DView *) this->images.data_[index])->mesh->positions;
        pivotX = (int) ((double) (int) quad[3] * -0.5);
    } else {
        pivotX = 0;
    }
    int pivotY;
    if ((placeFlags & 0x70) == 0x40) {
        float *quad = (float *) ((PCImage2DView *) this->images.data_[index])->mesh->positions;
        pivotY = (int) ((double) (int) quad[7] * -0.5);
    } else if ((placeFlags & 0x70) == 0x20) {
        float *quad = (float *) ((PCImage2DView *) this->images.data_[index])->mesh->positions;
        pivotY = -(int) quad[7];
    } else {
        pivotY = 0;
    }

    this->SetTexture(((PCImage2DView *) this->images.data_[index])->textureId, -1);
    AbyssEngine::AEMath::Matrix m;
    m.m[3] = (float) (anchorX + x + pivotX);
    m.m[7] = (float) (anchorY + y + pivotY);
    this->SetWorldViewMatrix(m);
    AbyssEngine::MeshDraw(this->engine, (AbyssEngine::Mesh *) ((PCImage2DView *) this->images.data_[index])->mesh);
}

void PaintCanvas::MeshCreate(unsigned short a, unsigned short b,
                             signed char c, unsigned int &out) {
    char mesh[4];
    *(AbyssEngine::Mesh **) mesh = 0;
    int result = AbyssEngine::MeshCreate(
        this->engine, a, b, c, reinterpret_cast<AbyssEngine::Mesh **>(mesh));
    if (result == 1) {
        ArrayAdd<AbyssEngine::Mesh *>(*(AbyssEngine::Mesh **) mesh,
                                      this->meshes);
        result = (int) this->meshes.count - 1;
    } else {
        result = -1;
    }
    out = result;
}

short PaintCanvas::FontGetSpacing(unsigned int index) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        return AbyssEngine::ImageFontGetSpacing(font);
    }
    return 0;
}

void *PaintCanvas::TransformGetTransform(unsigned int index) { // lint: void_ptr (external symbol; mangling must match lib)
    if (index < this->transforms.count) {
        return this->transforms.data_[index];
    }
    return 0;
}

void PaintCanvas::MeshCreate(unsigned short resId, unsigned int &out,
                             bool forceClone) {
    PCResourceView *res = (PCResourceView *) this->FindResource(resId);
    if (res == 0) {
        return;
    }
    unsigned int idx;
    if (res->handle == -1) {
        PCMeshInfoView *info = (PCMeshInfoView *) res->payload;
        unsigned int mat = 0xffffffff;
        this->MaterialCreate(info->matResId, mat);
        AbyssEngine::Mesh *mesh = 0;
        if (AbyssEngine::MeshCreateFromFile(this->engine, info->path, &mesh,
                                            mat <= this->materials.count ? this->materials.data_[mat] : 0) != 1) {
            return;
        }
        if (Engine::vboSupported) {
            mesh->vboEligible = 1;
            AbyssEngine::MeshConvertToVBO(mesh);
        }
        ArrayAdd<AbyssEngine::Mesh *>(mesh, this->meshes);
        idx = this->meshes.count - 1;
        res->handle = (int) idx;
    } else {
        AbyssEngine::Mesh **meshes = this->meshes.data_;
        idx = (unsigned int) res->handle;
        if (((PCMeshView *) meshes[idx])->materialRes != 0 || forceClone) {
            AbyssEngine::Mesh *clone = new AbyssEngine::Mesh(meshes[res->handle]);
            ArrayAdd<AbyssEngine::Mesh *>(clone, this->meshes);
            idx = this->meshes.count - 1;
        }
    }
    out = idx;
}

void PaintCanvas::FogSetParameter(AbyssEngine::FogMode mode, float fogStart, float fogEnd,
                                  float fogDensity, unsigned int color) {
    if (Engine::enableShader != 0) {
        Engine *eng = this->engine;
        eng->fogMinDist = fogStart;
        eng->fogMaxDist = fogEnd;
        eng->fogColor = Vector{(float) ((double) (color >> 24) / 255.0),
                               (float) ((double) ((color >> 16) & 0xff) / 255.0),
                               (float) ((double) ((color >> 8) & 0xff) / 255.0)};
        return;
    }
    glFogf(0xb65, (float) (unsigned int) mode);
    glFogf(0xb62, fogDensity);
    glFogf(0xb63, fogStart);
    glFogf(0xb64, fogEnd);
    float rgba[4];
    rgba[0] = (float) (color >> 24) / 255.0f;
    rgba[1] = (float) ((color >> 16) & 0xff) / 255.0f;
    rgba[2] = (float) ((color >> 8) & 0xff) / 255.0f;
    rgba[3] = (float) (color & 0xff) / 255.0f;
    glFogfv(0xb66, rgba);
}

static const unsigned int g_dr3_const_88808 = 0;

void PaintCanvas::DrawRegion2D(unsigned int index, int srcX, int srcY,
                               int destW, int destH, float /*unused*/, int transY, int pivotX, int pivotY,
                               int transX) {
    if (index >= this->images.count) {
        return;
    }
    PCImage2DView *img = (PCImage2DView *) ((char **) this->images.data_)[index];
    paintcanvas_ext_dr3_settexture(this, img->textureId);
    PCImage2DView *mesh = (PCImage2DView *) ((char **) this->images.data_)[index];
    PCMeshView *meshObj = (PCMeshView *) mesh->mesh;

    mesh->restoreFlag = 1;

    float fw = paintcanvas_ext_dr3_signedtofloat(destW, 0);
    float fh = paintcanvas_ext_dr3_signedtofloat(destH, 0);
    float *verts = (float *) meshObj->positions;
    verts[0] = 0.0f;
    verts[1] = 0.0f;
    verts[2] = 0.0f;
    verts[3] = fw;
    verts[4] = 0.0f;
    verts[5] = 0.0f;
    verts[6] = fw;
    verts[7] = fh;
    verts[8] = 0.0f;
    verts[9] = 0.0f;
    verts[10] = fh;

    float u0 = paintcanvas_ext_dr3_unsignedtofloat(mesh->regionB, 0);
    float w = paintcanvas_ext_dr3_unsignedtofloat(mesh->regionA, 0);
    float h = paintcanvas_ext_dr3_unsignedtofloat(mesh->u0, 0);
    float v0 = paintcanvas_ext_dr3_unsignedtofloat(mesh->v0, 0);
    float a2 = paintcanvas_ext_dr3_signedtofloat(srcX, 0);
    float a3 = paintcanvas_ext_dr3_signedtofloat(srcY, 0);
    float a2s = paintcanvas_ext_dr3_signedtofloat((short) srcX, 0);

    float *uv = (float *) meshObj->uvs;
    float invW = 1.0f / w;
    float invH = 1.0f / u0;
    float ulo = (a2 + h) * invW;
    float vlo = (a3 + v0) * invH;
    float uhi = (fw + h + a2s) * invW;
    float vhi = (fh + v0 + a3) * invH;
    uv[0] = ulo;
    uv[1] = vlo;
    uv[2] = uhi;
    uv[3] = vlo;
    uv[4] = uhi;
    uv[5] = vhi;
    uv[6] = ulo;
    uv[7] = vhi;

    unsigned int *blend = (unsigned int *) meshObj->indexBuffer;
    blend[0] = 0x20000;
    blend[1] = 1;
    blend[2] = g_dr3_const_88808;

    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f;
    pivotM[5] = 1.0f;
    pivotM[10] = 1.0f;
    pivotM[15] = 1.0f;
    pivotM[3] = paintcanvas_ext_dr3_signedtofloat(-pivotX, 0);
    pivotM[7] = paintcanvas_ext_dr3_signedtofloat(-pivotY, 0);

    float invPivotM[16];
    memset(invPivotM, 0, sizeof(invPivotM));
    invPivotM[0] = 1.0f;
    invPivotM[5] = 1.0f;
    invPivotM[10] = 1.0f;
    invPivotM[15] = 1.0f;
    invPivotM[3] = paintcanvas_ext_dr3_signedtofloat(transX + pivotX, 0);
    invPivotM[7] = paintcanvas_ext_dr3_signedtofloat(transY + pivotY, 0);

    float rotAng = invPivotM[7];
    float s = paintcanvas_ext_dr3_sinf(rotAng);
    float c = paintcanvas_ext_dr3_cosf(rotAng);
    float rotM[16];
    memset(rotM, 0, sizeof(rotM));
    rotM[0] = c;
    rotM[5] = c;
    rotM[10] = 1.0f;
    rotM[15] = 1.0f;
    *(unsigned int *) &rotM[1] = *(unsigned int *) &s ^ 0x80000000;
    rotM[4] = s;

    float composed[16];
    float scratch[16];
    paintcanvas_ext_dr3_mtx_mul(scratch, rotM);
    paintcanvas_ext_dr3_mtx_assign(composed, scratch);
    paintcanvas_ext_dr3_mtx_mul(scratch, pivotM);
    paintcanvas_ext_dr3_mtx_assign(composed, scratch);

    paintcanvas_ext_dr3_setwvm(this, composed);
    paintcanvas_ext_dr3_gldisable(0xb44);
    paintcanvas_ext_dr3_meshdraw(this->engine,
                                 ((PCImage2DView *) ((char **) this->images.data_)[index])->mesh);
    paintcanvas_ext_dr3_glenable(0xb44);
}

void PaintCanvas::RestoreImage2D(AbyssEngine::Image2D *image) {
    PCImage2DView *img = (PCImage2DView *) image;
    PCMeshView *m = (PCMeshView *) img->mesh;
    float *s = (float *) m->positions;
    img->restoreFlag = 0;

    s[0] = 0.0f;
    s[1] = 0.0f;
    s[2] = 0.0f;
    float w = (float) (unsigned int) img->width;
    s[3] = w;
    s[4] = 0.0f;
    s[5] = 0.0f;
    s[6] = w;
    float h = (float) (unsigned int) img->height;
    s[7] = h;
    s[8] = 0.0f;
    s[9] = 0.0f;
    s[10] = h;
    s[11] = 0.0f;

    float invH = (float) (1.0 / (double) (unsigned int) img->regionA);
    float invW = (float) (1.0 / (double) (unsigned int) img->regionB);
    float u0 = (float) (unsigned int) img->u0;
    float v0 = (float) (unsigned int) img->v0;

    float *buf = (float *) m->uvs;
    float bottom = h + v0;
    float right = w + u0;
    float a = u0 * invH;
    float b = v0 * invW;
    float c = bottom * invW;
    float d = right * invH;
    buf[0] = a;
    buf[1] = b;
    buf[2] = d;
    buf[3] = b;
    buf[4] = d;
    buf[5] = c;
    buf[6] = a;
    buf[7] = c;

    unsigned short *indices = (unsigned short *) m->indexBuffer;
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;
    indices[3] = 0;
    indices[4] = 3;
    indices[5] = 2;
}

void PaintCanvas::SpriteSystemGetPosition(unsigned int index, unsigned short sub,
                                          const Matrix &m, Vector &out) {
    if (index < this->spriteSystems.count) {
        PCSpriteSystemView *s = (PCSpriteSystemView *) (this->spriteSystems.data_)[index];
        if (s) {
            if ((unsigned int) (unsigned short) s->count <= (unsigned int) sub) {
                return;
            }
            const float *mm = (const float *) &m;
            float *p = (float *) (s->positions + sub * 12);
            float half = (float) (*(const short *) s->sizes >> 1);
            float x = mm[0] * p[0] + mm[1] * p[1] + mm[2] * p[2] + mm[3];
            float y = mm[4] * p[0] + mm[5] * p[1] + mm[6] * p[2] + mm[7];
            float z = mm[8] * p[0] + mm[9] * p[1] + mm[10] * p[2] + mm[11];
            out.x = x - half;
            out.y = y + half;
            out.z = z;
        }
    }
}

void PaintCanvas::MeshSet2DMask(unsigned int index, int, int) {
    if (this->images.count <= index) {
        return;
    }
    unsigned int i = index;
    char **arr = (char **) this->images.data_;
    char *img = arr[i];
    if (((PCImage2DView *) img)->restoreFlag != 0) {
        RestoreImage2D(reinterpret_cast<AbyssEngine::Image2D *>(img));
        arr = (char **) this->images.data_;
    }
    // The 2D mask lives at field_0x20 (offset 32), not mask2dImage (0x2c)
    reinterpret_cast<char *&>(this->field_0x20) = arr[i];
}

void PaintCanvas::ReleaseAllResources() {
    AbyssEngine::CubeMapSetIndex = 0;

    for (unsigned int i = 0; i < this->resources.count; i++) {
        ((PCResourceView *) this->resources.data_[i])->handle = -1;
    }

    for (unsigned int i = 0; i < this->cubeTextures.count; i++) {
        AbyssEngine::AELoadedTexture *tex = this->cubeTextures.data_[i];
        if (tex->glId != 0xffffffff) {
            unsigned int id = tex->glId;
            glDeleteTextures(1, &id);
            Engine::ImageCount -= 1;
            this->engine->textureByteCounter -= this->cubeTextures.data_[i]->byteSize;
        }
        delete this->cubeTextures.data_[i];
        this->cubeTextures.data_[i] = 0;
    }
    this->cubeTextures.count = 0;

    for (unsigned int i = 0; i < this->fonts.count; i++) {
        if (this->fonts.data_[i] != 0) {
            AbyssEngine::ImageFontRelease(this->engine, &this->fonts.data_[i]);
        }
    }
    ArrayRemoveAll(this->fonts);

    for (unsigned int i = 0; i < this->images.count; i++) {
        if (this->images.data_[i] != 0) {
            AbyssEngine::Image2DRelease(this->engine, &this->images.data_[i]);
        }
    }
    ArrayRemoveAll(this->images);

    for (unsigned int i = 0; i < this->meshes.count; i++) {
        if (this->meshes.data_[i] != 0) {
            Engine::vboSize -= this->meshes.data_[i]->vboByteSize;
            AbyssEngine::MeshRelease(this->engine, &this->meshes.data_[i]);
        }
    }
    ArrayRemoveAll(this->meshes);

    for (unsigned int i = 0; i < this->transforms.count; i++) {
        if (this->transforms.data_[i] != 0) {
            delete this->transforms.data_[i];
            this->transforms.data_[i] = 0;
        }
    }
    ArrayRemoveAll(this->transforms);

    for (unsigned int i = 0; i < this->cameras.count; i++) {
        if (this->cameras.data_[i] != 0) {
            delete this->cameras.data_[i];
            this->cameras.data_[i] = 0;
        }
    }
    ArrayRemoveAll(this->cameras);
    this->currentCamera = -1;

    for (unsigned int i = 0; i < this->materials.count; i++) {
        if (this->materials.data_[i] != 0) {
            delete this->materials.data_[i];
            this->materials.data_[i] = 0;
        }
    }
    ArrayRemoveAll(this->materials);

    for (unsigned int i = 0; i < this->spriteSystems.count; i++) {
        if (this->spriteSystems.data_[i] != 0) {
            AbyssEngine::SpriteSystemRelease(this->engine, &this->spriteSystems.data_[i]);
        }
    }
    ArrayRemoveAll(this->spriteSystems);
    this->field_0x1cc = 0;
}

// The Resource* payload classes' destructors live in the PaintCanvas TU in the original
// (0x77ebc.., right after ReleaseAllResources; iOS dump: Resource* classes in PaintCanvas.cpp).
AbyssEngine::ResourceTexture::~ResourceTexture() {
    delete[] this->name;
    this->name = nullptr;
}

AbyssEngine::ResourceMesh::~ResourceMesh() {
    delete[] this->name;
    this->name = nullptr;
}

AbyssEngine::ResourceTransform::~ResourceTransform() {
    if (this->dataA != nullptr) {
        ::operator delete[](this->dataA);
    }
    this->dataA = nullptr;

    if (this->dataB != nullptr) {
        ::operator delete[](this->dataB);
    }
    this->dataB = nullptr;
}

AbyssEngine::PickedTextureRegion
PaintCanvas::TransformGet2DPickedTextureRegion(unsigned int transformIndex, int x, int y, int z,
                                               int shift) {
    if (transformIndex >= this->transforms.count) {
        AbyssEngine::PickedTextureRegion none;
        none.u = -1.0f;
        none.v = -1.0f;
        fprintf(stderr, "TransformGet2DPickedTextureRegion Error %d >= %d\n", transformIndex, this->transforms.count);
        return none;
    }
    AbyssEngine::Transform *transform = this->transforms.data_[transformIndex];
    AbyssEngine::AEMath::Matrix world = transform->worldMatrix;
    AbyssEngine::AEMath::Vector local = {(float) x, (float) y, (float) z};
    local = AbyssEngine::AEMath::MatrixInverseTransformVector(world, local);
    return this->TransformGet2DPickedTextureRegion(transform, (int) local.x, (int) local.y, (int) local.z, shift);
}

void PaintCanvas::CheckString(unsigned int index, const AbyssEngine::String &str) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        const unsigned short *data = (const unsigned short *) str;
        AbyssEngine::ImageFontCheckString(font, data, str.length);
    }
}

void PaintCanvas::ReloadTextures() {
    for (unsigned int i = 0; i < this->cubeTextures.count; i++) {
        PCCubeTexView *res = (PCCubeTexView *) (this->cubeTextures.data_)[i];
        if (res->glTexId == -1) {
            unsigned int out = 0;
            char *path = reinterpret_cast<AbyssEngine::String *>(res->pathField)->GetAEChar();
            float f = ((PCCubeTexView *) (this->cubeTextures.data_)[i])->scale;
            int ok = AbyssEngine::TextureCreateFromFile(this->engine, path, 0, 0,
                                                        &out, false, f);
            if (ok == 1) {
                ((PCCubeTexView *) (this->cubeTextures.data_)[i])->glTexId = out;
            }
            delete[] path;
        }
    }
}

void PaintCanvas::MaterialCreate(unsigned short resId, unsigned int &out) {
    PCResourceView *res = (PCResourceView *) this->FindResource(resId);
    if (res == 0) {
        return;
    }
    if (res->handle == -1) {
        PCMaterialInfoView *info = (PCMaterialInfoView *) res->payload;
        AbyssEngine::Material *mat = new AbyssEngine::Material();
        for (unsigned int i = 0; i < 8; i++) {
            if (info->textureIds[i] == 0xffff) {
                continue;
            }
            PCResourceView *tres = (PCResourceView *) this->FindResource(info->textureIds[i]);
            if (tres == 0) {
                break;
            }
            if (tres->handle == -1) {
                unsigned int texture = 0;
                this->TextureCreate(info->textureIds[i], texture, true);
            }
            mat->textures[i] = tres->handle;
        }
        mat->blendMode = info->blendMode;
        mat->addData = info->addData;
        mat->addDataSize = info->addDataSize;
        mat->ambientColor = info->ambientColor;
        ArrayAdd<AbyssEngine::Material *>(mat, this->materials);
        res->handle = (int) this->materials.count - 1;
    }
    out = (unsigned int) res->handle;
}

AbyssEngine::PickedTextureRegion
PaintCanvas::TransformGet2DPickedTextureRegion(Transform *transform, int x, int y, int z, int shift) {
    float qx = (float) ((x >> shift) >> shift);
    float qz = (float) ((z >> shift) >> shift);
    for (unsigned int i = 0; i < transform->meshes.count; i++) {
        AbyssEngine::PickedTextureRegion hit = AbyssEngine::MeshIntersect(qx, qz, transform->meshes.data_[i]);
        if (hit.u != -1.0f && hit.v != -1.0f) {
            return hit;
        }
    }
    for (unsigned int i = 0; i < transform->children.count; i++) {
        AbyssEngine::AEMath::Matrix world = transform->children.data_[i]->worldMatrix;
        AbyssEngine::AEMath::Vector local = {(float) x, (float) y, (float) z};
        local = AbyssEngine::AEMath::MatrixInverseTransformVector(world, local);
        AbyssEngine::PickedTextureRegion hit = this->TransformGet2DPickedTextureRegion(
                transform->children.data_[i], (int) local.x, (int) local.y, (int) local.z, shift);
        if (hit.u != -1.0f && hit.v != -1.0f) {
            return hit;
        }
    }
    AbyssEngine::PickedTextureRegion none;
    none.u = -1.0f;
    none.v = -1.0f;
    return none;
}

void PaintCanvas::DrawTransform(unsigned int index, const AbyssEngine::AEMath::Matrix *viewMatrix) {
    if (index >= this->transforms.count || !this->transforms.data_[index]->visible) {
        return;
    }
    AbyssEngine::AEMath::Matrix world;
    if (this->currentCamera < this->cameras.count) {
        if (this->initialized != 0) {
            // Tilt-controlled view: roll the camera by the device gravity angle.
            AbyssEngine::AEMath::Matrix roll;
            AbyssEngine::AEMath::MatrixIdentity(roll);
            float angle = (float) (this->engine->GetGravValue()[1] * 1.5707963705062866);
            float rot = (this->gameOrientation == 1) ? angle : -angle;
            float s = AbyssEngine::AEMath::Sinf(rot);
            float c = AbyssEngine::AEMath::Cosf(rot);
            roll.m[0] = c;
            roll.m[5] = c;
            roll.m[1] = -s;
            roll.m[4] = s;
            if (!this->transforms.data_[index]->InCameraVF(&roll, this->cameras.data_[this->currentCamera])) {
                this->culledCount += 1;
                return;
            }
            AbyssEngine::AEMath::Matrix view;
            if (viewMatrix != 0) {
                view = *viewMatrix;
            } else {
                view = this->cameras.data_[this->currentCamera]->projection;
            }
            view *= roll;
            world = AbyssEngine::AEMath::MatrixGetInverse(view);
        } else {
            if (!this->transforms.data_[index]->InCameraVF(nullptr, this->cameras.data_[this->currentCamera])) {
                this->culledCount += 1;
                return;
            }
            if (viewMatrix != 0) {
                world = AbyssEngine::AEMath::MatrixGetInverse(*viewMatrix);
            } else {
                world = AbyssEngine::AEMath::MatrixGetInverse(this->cameras.data_[this->currentCamera]->projection);
            }
        }
        AbyssEngine::Camera *camera = this->cameras.data_[this->currentCamera];
        this->engine->SetEyePosition(camera->projection.m[3], camera->projection.m[7], camera->projection.m[11]);
    }
    AbyssEngine::Transform *transform = this->transforms.data_[index];
    AbyssEngine::AEMath::Matrix identity;
    this->DrawTransform(transform, identity, world);
}

void PaintCanvas::FontSetSpacing(unsigned int index, short spacing) {
    if (index < this->fonts.count) {
        AbyssEngine::ImageFont *font = (this->fonts.data_)[index];
        return AbyssEngine::ImageFontSetSpacing(font, spacing);
    }
}

int PaintCanvas::GetWidth() {
    return this->engine->GetDisplayWidth();
}

void PaintCanvas::CameraCreate(unsigned int &out) {
    AbyssEngine::Camera *cam = new AbyssEngine::Camera(8000.0f, 256.0f, 16384.0f, (float) this->GetWidth(), (float) this->GetHeight());
    ArrayAdd<AbyssEngine::Camera *>(cam, this->cameras);
    out = this->cameras.count - 1;
}


void PaintCanvas::DrawSpriteSystem(unsigned int index, AbyssEngine::AEMath::Matrix mat) {
    if (index >= this->spriteSystems.count || this->spriteSystems.data_[index] == 0) {
        return;
    }
    AbyssEngine::AEMath::Matrix world = mat;
    if (this->initialized != 0) {
        AbyssEngine::AEMath::Matrix roll;
        AbyssEngine::AEMath::MatrixIdentity(roll);
        float angle = (float) (this->engine->GetGravValue()[1] * 1.5707963705062866);
        float rot = (this->gameOrientation == 1) ? angle : -angle;
        float s = AbyssEngine::AEMath::Sinf(rot);
        float c = AbyssEngine::AEMath::Cosf(rot);
        roll.m[0] = c;
        roll.m[5] = c;
        roll.m[1] = -s;
        roll.m[4] = s;
        world *= roll;
        world = AbyssEngine::AEMath::MatrixGetInverse(world);
    } else {
        world = AbyssEngine::AEMath::MatrixGetInverse(world);
    }
    Engine *engine = this->engine;
    AbyssEngine::AEMath::Matrix identity;
    AbyssEngine::SpriteSystemDraw(engine, identity, world, this->spriteSystems.data_[index]);
}

void PaintCanvas::DrawString(unsigned int index, const AbyssEngine::String &str,
                             int x, int y, bool b) {
    if (index < this->fonts.count) {
        PCFontView *font = (PCFontView *) (this->fonts.data_)[index];
        this->SetTexture((unsigned int) font->texture, (unsigned int)-1);
        AbyssEngine::ImageFont *font2 = (this->fonts.data_)[index];
        const unsigned short *data = (const unsigned short *)str;
        AbyssEngine::ImageFontDrawString(font2, data, str.size(), x, y, this, this->engine, b);
    }
}

int PaintCanvas::TransformGetTriCount(Transform *transform) {
    if (!transform) {
        return 0;
    }
    int total = 0;
    unsigned int n1 = transform->meshes.size();
    for (unsigned int i = 0; i != n1; ++i) {
        total += this->MeshGetTriCount(transform->meshes[i]);
    }
    unsigned int n2 = transform->children.size();
    for (unsigned int i = 0; i != n2; ++i) {
        total += this->TransformGetTriCount(transform->children[i]);
    }
    return total;
}

void PaintCanvas::MeshChangeShaderAnimValue(Transform *transform, float value, unsigned int mode) {
    if (transform) {
        for (unsigned int i = 0; i < transform->meshes.size(); ++i) {
            AbyssEngine::Mesh *m = transform->meshes[i];
            this->MeshChangeShaderAnimValue(m, value, mode);
        }
        for (unsigned int i = 0; i < transform->children.size(); ++i) {
            AbyssEngine::Transform *c = transform->children[i];
            this->MeshChangeShaderAnimValue(c, value, mode);
        }
    }
}

void PaintCanvas::ReleaseSpriteSystemResource(unsigned int index) {
    if (index < this->spriteSystems.count) {
        Engine *ctx = this->engine;
        char **arr = (char **) this->spriteSystems.data_;
        return AbyssEngine::SpriteSystemRelease(ctx, (AbyssEngine::SpriteSystem **) (arr + index));
    }
}

static const double g_dss2_gravscale_8af58 = 0;

void PaintCanvas::DrawSpriteSystem(unsigned int index, AbyssEngine::AEMath::Matrix matA,
                                   AbyssEngine::AEMath::Matrix matB) {
    if (index >= this->spriteSystems.count || this->spriteSystems.data_[index] == 0) {
        return;
    }
    AbyssEngine::AEMath::Matrix world = matB;
    AbyssEngine::AEMath::Matrix view = matA;
    if (this->initialized != 0) {
        AbyssEngine::AEMath::Matrix roll;
        AbyssEngine::AEMath::MatrixIdentity(roll);
        // Note: this overload truncates the tilt angle to whole radians.
        int angle = (int) (this->engine->GetGravValue()[1] * -1.5707963705062866);
        float rot = (float) ((this->gameOrientation == 1) ? -angle : angle);
        float s = AbyssEngine::AEMath::Sinf(rot);
        float c = AbyssEngine::AEMath::Cosf(rot);
        roll.m[0] = c;
        roll.m[5] = c;
        roll.m[1] = -s;
        roll.m[4] = s;
        world *= roll;
        world = AbyssEngine::AEMath::MatrixGetInverse(world);
        view *= roll;
    } else {
        world = AbyssEngine::AEMath::MatrixGetInverse(world);
    }
    AbyssEngine::SpriteSystemDraw(this->engine, view, world, this->spriteSystems.data_[index]);
}
