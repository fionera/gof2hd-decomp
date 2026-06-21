#include "engine/render/PaintCanvas.h"
#include "engine/render/Material.h"
#include "engine/core/Array.h"
PaintCanvas* gCanvas = nullptr;       // canonical render canvas singleton
// NOTE: Engine.h / Mesh.h / Node.h / String.h are intentionally NOT included.
// PaintCanvas.h is a self-contained translation unit that models the few
// cross-class shapes (Mesh/Transform/Node/Engine/String) it actually touches;
// pulling the real per-class headers in here causes redefinition/ambiguity
// conflicts because each was generated to be the only class in its TU.

// ---- engine Array<T> container helpers (real std::vector semantics) -------------------------
// PaintCanvas stores its collections as the engine's plain three-word array header embedded at
// fixed byte offsets inside the object (mesh/transform/camera/material/sprite/resource lists,
// the glow scratch buffers, the loaded-texture-name list):
//   +0x0 uint32 count   +0x4 T* data   +0x8 uint32 capacity
// The decompiler emitted these collections' compiler-generated template special members
// (Array_<T>_ctor/_dtor, ArrayAdd_<T>, ArrayRemoveAll_<T>, ArrayReleaseClasses_<T>) as
// undefined externs. We give them here as real Array<T> operations (new/delete/push_back/clear,
// with proper element destruction) acting directly on that embedded header, so the link errors
// disappear while the byte layout the surrounding offset-access code and PaintCanvasClass.h rely
// on stays identical. Element strides are sizeof(T), fixing the latent 64-bit pointer-width bug.
namespace {
struct PCArrayHeader { uint32_t count; void *data; uint32_t capacity; };

// Array_<T>_ctor: zero the embedded header.
inline void PCArrayCtor(void *arrayHeader) {
    PCArrayHeader *a = (PCArrayHeader *)arrayHeader;
    a->count = 0;
    a->data = nullptr;
    a->capacity = 0;
}

// Array_<T>_dtor: free the backing store (elements are released separately, like ~vector).
// always_inline: the original fully inlined this at every call site (no standalone symbol).
__attribute__((always_inline)) inline void PCArrayDtor(void *arrayHeader) {
    PCArrayHeader *a = (PCArrayHeader *)arrayHeader;
    ::operator delete(a->data);
    a->data = nullptr;
    a->count = 0;
    a->capacity = 0;
}

// ArrayAdd_<T>: append one element, growing the backing store by one slot.
template <class T>
inline void PCArrayAdd(T item, void *arrayHeader) {
    PCArrayHeader *a = (PCArrayHeader *)arrayHeader;
    if (a->count >= a->capacity) {
        uint32_t newCap = a->count + 1;
        T *grown = (T *)::operator new((size_t)newCap * sizeof(T));
        for (uint32_t i = 0; i < a->count; ++i)
            grown[i] = ((T *)a->data)[i];
        ::operator delete(a->data);
        a->data = grown;
        a->capacity = newCap;
    }
    ((T *)a->data)[a->count] = item;
    a->count += 1;
}

// ArrayRemoveAll_<T>: clear (count -> 0; backing store retained, matching std::vector::clear).
inline void PCArrayRemoveAll(void *arrayHeader) {
    PCArrayHeader *a = (PCArrayHeader *)arrayHeader;
    a->count = 0;
}

// ArrayReleaseClasses_<T>: delete every owned pointee, then clear.
// always_inline: the original fully inlined this at every call site (no standalone symbol).
__attribute__((always_inline)) inline void PCArrayReleaseClasses(void *arrayHeader) {
    PCArrayHeader *a = (PCArrayHeader *)arrayHeader;
    for (uint32_t i = 0; i < a->count; ++i) {
        void *e = ((void **)a->data)[i];
        if (e != nullptr)
            ::operator delete(e);
    }
    a->count = 0;
}
} // namespace

extern "C" void paintcanvas_ext_has_vibration(void *);
extern "C" __attribute__((disable_tail_calls)) void MatrixIdentity(void *result, void *matrix);
extern "C" int paintcanvas_ext_dtl_textwidth(void *, unsigned int, void *);
extern "C" void paintcanvas_ext_dtl_drawstring(void *, unsigned int, void *, int, int, bool);
extern "C" int paintcanvas_ext_dtl_textheight(void *, unsigned int);
extern "C" void paintcanvas_ext_fr_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_fr_glenable(void *eng, unsigned int cap, bool on);
extern "C" void paintcanvas_ext_fr_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_sprite_rgba(unsigned int, float, float, float, float, void *);
extern "C" void *paintcanvas_ext_alloc(unsigned int);
extern "C" void *paintcanvas_ext_transform_ctor(void *);
extern "C" void paintcanvas_ext_add_child(void *, void *);
extern "C" void paintcanvas_ext_dr_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dr_glLineWidth(float w);
extern "C" void paintcanvas_ext_dr_glcap(void *eng, unsigned int cap, int on);
extern "C" void paintcanvas_ext_dr_glVertexPointer(int a, int b, int c, void *p);
extern "C" void paintcanvas_ext_dr_glColorMask(void *eng, unsigned int cap, int on);
extern "C" void paintcanvas_ext_dr_glDrawArrays(int a, int b, int c);
extern "C" void paintcanvas_ext_dr_drawline2d(void *eng, void *p, int n, bool b);
extern "C" void paintcanvas_ext_fbo_a(void *);
extern "C" void paintcanvas_ext_fbo_b(void *, int);
extern "C" void paintcanvas_ext_fbo_c(void *);
extern "C" void paintcanvas_ext_fbo_d(void *);
extern "C" void paintcanvas_ext_sprite_alluv(float, float, float, float, void *);
extern "C" void paintcanvas_ext_sprite_allsize(unsigned int, void *);
extern "C" void *paintcanvas_ext_str_text(const AbyssEngine::String *);
extern "C" int paintcanvas_ext_text_width_range(void *, void *, unsigned int, unsigned int);
extern "C" char *paintcanvas_ext_find_res(void *, unsigned int);
extern "C" void paintcanvas_ext_change_mat(void *, void *, void *);
extern "C" void paintcanvas_ext_setcolor(void *, float, float, float, float);
extern "C" void paintcanvas_ext_vibrate(void *);
extern "C" void paintcanvas_ext_drawtextlines6(void *, unsigned int, void *, int, int, bool);
extern "C" int paintcanvas_ext_strcmp(void *, void *);
extern "C" unsigned int paintcanvas_ext_strlen(void *);
extern "C" int paintcanvas_ext_text_width(void *, unsigned int, unsigned int);
extern "C" void paintcanvas_ext_clear(int);
extern "C" int paintcanvas_ext_getscreenpos_m(void *self, void *m, const Vector *a, Vector *b);
extern "C" void paintcanvas_ext_gl_disable(unsigned int);
extern "C" void paintcanvas_ext_gl_enable(unsigned int);
extern "C" void paintcanvas_ext_setprojmatrix3d(void *, float, float, float);
extern "C" int paintcanvas_ext_getdisplaywidth(void *);
extern "C" int paintcanvas_ext_getdisplayheight(void *);
extern "C" void paintcanvas_ext_mat_intern(void *, void *);
extern "C" void paintcanvas_ext_camera_apply(void *, unsigned int, unsigned int, unsigned int);
extern "C" void paintcanvas_ext_disable(int);
extern "C" int paintcanvas_ext_is_posteffect(void *);
extern "C" void paintcanvas_ext_use_refract(void *);
extern "C" void paintcanvas_ext_sprite_uv(unsigned int, float, float, float, float, void *);
extern "C" void paintcanvas_ext_set_wvm(void *);
extern "C" void paintcanvas_ext_convert_vbo(void *);
extern "C" void paintcanvas_ext_di2_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_di2_settexture(void *self, unsigned int tex, int slot);
extern "C" float paintcanvas_ext_di2_signedtofloat(int v, unsigned int mode);
extern "C" float paintcanvas_ext_di2_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" void paintcanvas_ext_di2_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di2_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_di2_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_di2_glenable(unsigned int cap);
extern "C" void paintcanvas_ext_font_set_yoff(void *, int);
extern "C" void paintcanvas_ext_sbm_lightenable(void *eng, int on);
extern "C" void paintcanvas_ext_sbm_lightsetlight(void *eng, int v);
extern "C" void paintcanvas_ext_sbm_glenablecap(void *eng, unsigned int v, int on);
extern "C" void paintcanvas_ext_sbm_glTexEnvi(unsigned int a, unsigned int b, unsigned int c);
extern "C" void paintcanvas_ext_sbm_glEnable(unsigned int cap);
extern "C" void paintcanvas_ext_sbm_glDisable(unsigned int cap);
extern "C" void paintcanvas_ext_sbm_glBlendFunc(unsigned int a, unsigned int b);
extern "C" void paintcanvas_ext_sbm_glDepthMask(int v);
extern "C" void paintcanvas_ext_sbm_glAlphaFunc(unsigned int a, float ref);
extern "C" void paintcanvas_ext_sbm_setalpha(void *eng, unsigned int v, int on);
extern "C" void paintcanvas_ext_sbm_setlight(int on);
extern "C" void paintcanvas_ext_sbm_texcombine(unsigned int a, unsigned int b, unsigned int c);
extern "C" AbyssEngine::String *paintcanvas_ext_gla_str_new();
extern "C" void paintcanvas_ext_gla_str_copy(void *out, void *src, bool copy);
extern "C" void paintcanvas_ext_gla_str_fromchar(void *out, const char *s, bool copy);
extern "C" void paintcanvas_ext_gla_str_append(void *dst, void *src);
extern "C" void paintcanvas_ext_gla_str_dtor(void *s);
extern "C" void paintcanvas_ext_gla_substr(void *out, const void *str, unsigned int begin, unsigned int end);
extern "C" void paintcanvas_ext_gla_getline(void *self, unsigned int font, void *line, int width, void *out);
extern "C" void paintcanvas_ext_gla_str_vdtor(void *s);
extern "C" void paintcanvas_ext_gla_arr_setlen(unsigned int n, ::Array<AbyssEngine::String *> *arr);
extern "C" unsigned short *paintcanvas_ext_gla_str_index(const void *s, int i);
extern "C" void paintcanvas_ext_gla_str_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_add_resource(void *, void *);
extern "C" void paintcanvas_ext_remove_meshid(void *, void *);
extern "C" int paintcanvas_ext_get_height(void *);
extern "C" void paintcanvas_ext_gl_a(unsigned int);
extern "C" void paintcanvas_ext_gl_bind(unsigned int, unsigned int);
extern "C" void paintcanvas_ext_gl_c(unsigned int);
extern "C" void *paintcanvas_ext_cube_restore(void *);
extern "C" void paintcanvas_ext_cube_tail(void *);
extern "C" void *paintcanvas_ext_tfc_findres(void *self, unsigned short id);
extern "C" void *paintcanvas_ext_tfc_new_transform();
extern "C" void paintcanvas_ext_tfc_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_tfc_meshcreate(void *self, unsigned short id, unsigned int *out, bool b);
extern "C" void paintcanvas_ext_set_wvm2(void *self, void *m);
extern "C" void paintcanvas_ext_meshdraw(void *engine, void *mesh);
extern "C" void paintcanvas_ext_mesh_changemat(AbyssEngine::PaintCanvas *, void *, void *);
extern "C" void paintcanvas_ext_transform_changemat(AbyssEngine::PaintCanvas *, void *, void *);
extern "C" int paintcanvas_ext_text_height(void *);
extern "C" void paintcanvas_ext_init_setorientation(void *eng);
extern "C" int paintcanvas_ext_init_dispwidth(void *eng);
extern "C" int paintcanvas_ext_init_dispheight(void *eng);
extern "C" float paintcanvas_ext_init_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_init_setpersp(void *self, float a, float b, float c);
extern "C" void paintcanvas_ext_draw_mesh(void *, void *);
extern "C" unsigned int __aeabi_uidiv(unsigned int, unsigned int);
extern "C" int paintcanvas_ext_font_get_yoff(void *);
extern "C" int paintcanvas_ext_spm_dispwidth(void *eng);
extern "C" int paintcanvas_ext_spm_dispheight(void *eng);
extern "C" float paintcanvas_ext_spm_sinf(float v);
extern "C" float paintcanvas_ext_spm_cosf(float v);
extern "C" float paintcanvas_ext_spm_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_dss1_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dss1_getgrav(void *eng);
extern "C" float paintcanvas_ext_dss1_sinf(float v);
extern "C" float paintcanvas_ext_dss1_cosf(float v);
extern "C" void paintcanvas_ext_dss1_memcpy(void *dst, void *src, unsigned int n);
extern "C" void paintcanvas_ext_dss1_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dss1_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dss1_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dss1_ssdraw(void *eng, void *ident, void *m, void *ss);
extern "C" unsigned short *paintcanvas_ext_gl_strindex(const AbyssEngine::String *str, unsigned int i);
extern "C" void paintcanvas_ext_gl_substr(void *out, const AbyssEngine::String *str, unsigned int begin, unsigned int end);
extern "C" void paintcanvas_ext_gl_str_fromchar(void *out, const char *s, bool copy);
extern "C" void paintcanvas_ext_gl_str_assign(AbyssEngine::String *dst, void *src);
extern "C" void paintcanvas_ext_gl_str_dtor(void *s);
extern "C" void paintcanvas_ext_dl_glLineWidth(float w);
extern "C" void paintcanvas_ext_dl_glEnable(void *eng, bool on);
extern "C" float paintcanvas_ext_dl_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_dl_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dl_glVertexPointer(int a, int b, int c, void *p);
extern "C" void paintcanvas_ext_dl_glColorMask(void *eng, unsigned int cap, int on);
extern "C" void paintcanvas_ext_dl_glDrawArrays(int a, int b, int c);
extern "C" void paintcanvas_ext_dl_drawline2d(void *eng, void *p, bool b);
extern "C" int paintcanvas_ext_get_w(AbyssEngine::PaintCanvas *);
extern "C" int paintcanvas_ext_get_h(AbyssEngine::PaintCanvas *);
extern "C" void paintcanvas_ext_cam_persp4(float, float, float, float, float, void *);
extern "C" void paintcanvas_ext_cam_setcur(AbyssEngine::PaintCanvas *, unsigned int);
extern "C" void paintcanvas_ext_end3d(AbyssEngine::PaintCanvas *);
extern "C" void paintcanvas_ext_material_clone(void *, void *);
extern "C" void paintcanvas_ext_material_add(void *, void *);
extern "C" void paintcanvas_ext_get_grav(void *);
extern "C" void paintcanvas_ext_glMatrixMode(unsigned int);
extern "C" void paintcanvas_ext_gl_depthmask(unsigned int);
extern "C" void paintcanvas_ext_gl_color(void *, float, float, float, float);
extern "C" void paintcanvas_ext_matgl_load(void *, void *);
extern "C" void paintcanvas_ext_gl_loadidentity();
extern "C" void paintcanvas_ext_gl_ortho_persp(float, float, float);
extern "C" void paintcanvas_ext_gl_loadmatrix(void *);
extern "C" void paintcanvas_ext_gl_done();
extern "C" void *paintcanvas_ext_fc_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_fc_texcreate(void *self, unsigned short id, bool b);
extern "C" int paintcanvas_ext_fc_fontfromfile(void *eng, char *path, unsigned short region, void **out);
extern "C" int paintcanvas_ext_fc_fontheight(void *font);
extern "C" void paintcanvas_ext_set_reslist(AbyssEngine::Resource *const *, unsigned int, void *);
extern "C" void paintcanvas_ext_child_link(void *, void *, void *);
extern "C" void paintcanvas_ext_transform_dirty(void *);
extern "C" void tcg_glActiveTexture(unsigned unit);
extern "C" void tcg_glBindTexture(unsigned target, unsigned tex);
extern "C" int paintcanvas_ext_ss2_sscreate(void *eng, unsigned short id, bool b, void **out);
extern "C" void paintcanvas_ext_ss2_matcreate(void *self, unsigned short id, unsigned int *out);
extern "C" void paintcanvas_ext_gsp_vec_assign(void *dst, void *src);
extern "C" int paintcanvas_ext_gsp_getwidth(void *self);
extern "C" int paintcanvas_ext_gsp_getheight(void *self);
extern "C" float paintcanvas_ext_gsp_signedtofloat(int v, unsigned int mode);
extern "C" int paintcanvas_ext_sscreate(void *eng, unsigned short id, bool b, void **out);
extern "C" void *paintcanvas_ext_material_ctor(void *);
extern "C" void paintcanvas_ext_cisvf_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_cisvf_getgrav(void *eng);
extern "C" float paintcanvas_ext_cisvf_sinf(float v);
extern "C" float paintcanvas_ext_cisvf_cosf(float v);
extern "C" int paintcanvas_ext_cisvf_inner(const float *pt, float radius, void *m, void *cam);
extern "C" void paintcanvas_ext_shader_anim(void *, void *);
extern "C" char *paintcanvas_ext_rs_getAEChar(void *strField);
extern "C" void paintcanvas_ext_rs_deletearr(char *p);
extern "C" void paintcanvas_ext_rs_glActiveTexture(unsigned int tex);
extern "C" void paintcanvas_ext_rs_glBindTexture(unsigned int target, unsigned int tex);
extern "C" void paintcanvas_ext_tami_bsphere_merge(void *dst, void *src);
extern "C" void paintcanvas_ext_tami_setanimlen(void *tf, int hi, int lo);
extern "C" void paintcanvas_ext_tami_setanimstate(void *tf, int a, int b);
extern "C" void paintcanvas_ext_tami_finalize(void *tf);
extern "C" void paintcanvas_ext_get_accel(void *);
extern "C" int paintcanvas_ext_rpm_dispwidth(void *eng);
extern "C" int paintcanvas_ext_rpm_dispheight(void *eng);
extern "C" float paintcanvas_ext_rpm_sinf(float v);
extern "C" float paintcanvas_ext_rpm_cosf(float v);
extern "C" float paintcanvas_ext_rpm_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_rpm_glMatrixMode(unsigned int mode);
extern "C" void paintcanvas_ext_rpm_glLoadIdentity();
extern "C" void paintcanvas_ext_rpm_glScalef(float x, float y, float z);
extern "C" void paintcanvas_ext_rpm_glLoadMatrixf(void *m);
extern "C" void paintcanvas_ext_rpm_glFinish();
extern "C" void paintcanvas_ext_rpm_loadproj(void *eng, void *m);
extern "C" void paintcanvas_ext_cipvf_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_cipvf_getgrav(void *eng);
extern "C" float paintcanvas_ext_cipvf_sinf(float v);
extern "C" float paintcanvas_ext_cipvf_cosf(float v);
extern "C" int paintcanvas_ext_cipvf_inner(const float *pt, void *m, void *cam);
extern "C" void paintcanvas_ext_set_texture(void *);
extern "C" void paintcanvas_ext_dtor_releaseall(void *self);
extern "C" void paintcanvas_ext_dtor_op_delete(void *p);
extern "C" void *paintcanvas_ext_dtor_restex_dtor(void *p);
extern "C" void *paintcanvas_ext_dtor_resmesh_dtor(void *p);
extern "C" void *paintcanvas_ext_dtor_restransform_dtor(void *p);
extern "C" void paintcanvas_ext_dtor_str_dtor(void *p);
extern "C" void paintcanvas_ext_dtor_meshrelease(void *eng, void *meshptr);
extern "C" void *paintcanvas_ext_i2d_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_i2d_texcreate(void *self, unsigned short id, bool b);
extern "C" int paintcanvas_ext_i2d_imgregion(void *eng, char *path, unsigned short region, void *img);
extern "C" void paintcanvas_ext_gsp2_transformvec(void *out, const void *vec);
extern "C" void paintcanvas_ext_gsp2_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_gsp2_getgrav(void *eng);
extern "C" float paintcanvas_ext_gsp2_sinf(float v);
extern "C" float paintcanvas_ext_gsp2_cosf(float v);
extern "C" void paintcanvas_ext_gsp2_memcpy(void *dst, void *src, unsigned int n);
extern "C" void paintcanvas_ext_gsp2_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_gsp2_invtransformvec(void *outMat, void *vec);
extern "C" void paintcanvas_ext_gsp2_vec_assign(void *dst, void *src);
extern "C" int paintcanvas_ext_gsp2_getwidth(void *self);
extern "C" int paintcanvas_ext_gsp2_getheight(void *self);
extern "C" float paintcanvas_ext_gsp2_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_di_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_di_settexture(void *self, unsigned int tex, int slot);
extern "C" void paintcanvas_ext_di_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_di_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_di_glenable(unsigned int cap);
extern "C" char *paintcanvas_ext_find_mesh(void *, unsigned short);
extern "C" void paintcanvas_ext_remove_mesh(void *, unsigned int, int);
extern "C" void paintcanvas_ext_enable(int);
extern "C" void paintcanvas_ext_depthmask(int);
extern "C" void paintcanvas_ext_clear2(void *, unsigned int);
extern "C" void paintcanvas_ext_array_remove(void *, void *);
extern "C" void *paintcanvas_ext_tc_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_start_fbo(void *);
extern "C" void paintcanvas_ext_gl_deletetextures(int, void *);
extern "C" void paintcanvas_ext_mtx_mul(void *out, const void *a, void *b);
extern "C" void paintcanvas_ext_mtx_muleq(void *m, void *rhs);
extern "C" int paintcanvas_ext_dt_incamvf(void *tf, void *m, void *cam);
extern "C" void paintcanvas_ext_dt_drawtransform_rec(void *self, void *tf, void *m, void *m3);
extern "C" void paintcanvas_ext_di4_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_di4_settexture(void *self, unsigned int tex);
extern "C" int paintcanvas_ext_di4_getwidth(void *self);
extern "C" int paintcanvas_ext_di4_getheight(void *self);
extern "C" float paintcanvas_ext_di4_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_di4_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di4_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_di4_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_di4_glenable(unsigned int cap);
extern "C" void paintcanvas_ext_cam_persp(float, float, float, float, void *);
extern "C" void paintcanvas_ext_dr2_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_dr2_settexture(void *self, unsigned int tex);
extern "C" float paintcanvas_ext_dr2_signedtofloat(int v, unsigned int mode);
extern "C" float paintcanvas_ext_dr2_sinf(float v);
extern "C" float paintcanvas_ext_dr2_cosf(float v);
extern "C" void paintcanvas_ext_dr2_setscaling(void *out, float x, float y, float z);
extern "C" void paintcanvas_ext_dr2_mtx_mul(void *out, void *a, void *b);
extern "C" void paintcanvas_ext_dr2_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dr2_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dr2_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_dr2_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_dr2_glenable(unsigned int cap);
extern "C" void paintcanvas_ext_sgo_setorientation(void *eng, int mode);
extern "C" int paintcanvas_ext_sgo_dispwidth(void *eng);
extern "C" int paintcanvas_ext_sgo_dispheight(void *eng);
extern "C" float paintcanvas_ext_sgo_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_sgo_setpersp(void *self, float a, float b, float c);
extern "C" void paintcanvas_ext_mc_matcreate(void *self, unsigned short id, unsigned int *out);
extern "C" void paintcanvas_ext_dm_memcpy(void *dst, const void *src, unsigned int n);
extern "C" void paintcanvas_ext_dm_settrans(void *out, float v);
extern "C" void paintcanvas_ext_dm_getpos(void *out);
extern "C" void paintcanvas_ext_dm_settrans_vec(void *out, void *vec);
extern "C" void paintcanvas_ext_dm_mtx_mul(void *out, const void *a, const void *b);
extern "C" void paintcanvas_ext_dm_mtx_assign(void *dst, const void *src);
extern "C" void paintcanvas_ext_dm_transformvec(void *m, void *vec);
extern "C" void paintcanvas_ext_dm_rotatevec(void *m, void *vec);
extern "C" void paintcanvas_ext_dm_vec_assign(void *dst, const void *src);
extern "C" int paintcanvas_ext_dm_spherefrustum(void *self, void *pt, float radius);
extern "C" float paintcanvas_ext_dm_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" void paintcanvas_ext_dm_setcolor(void *eng, float r, float g, float b, float a);
extern "C" void paintcanvas_ext_dm_mtx_muleq(void *m, const void *rhs);
extern "C" void paintcanvas_ext_dm_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dm_setmodelmatrix(void *eng);
extern "C" void paintcanvas_ext_dm_setuvmatrix(void *eng, void *m);
extern "C" void paintcanvas_ext_dm_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_dm_resetuvmatrix(void *eng);
extern "C" void paintcanvas_ext_dm_addcached_mesh(void *mesh, void *arr);
extern "C" void paintcanvas_ext_dm_addcached_uint(unsigned int v, void *arr);
extern "C" void paintcanvas_ext_dm_pushmat(const float *m, void *arr);
extern "C" int paintcanvas_ext_dm_incamvf(void *tf, void *m, void *cam);
extern "C" void paintcanvas_ext_dm_drawtransform(void *self, void *tf, void *m, void *m2);
extern "C" void paintcanvas_ext_gl_blendfunc(unsigned int, unsigned int);
extern "C" void paintcanvas_ext_glenable2(void *, unsigned int, bool);
extern "C" void paintcanvas_ext_setortho(void *, void *, void *, bool);
extern "C" void paintcanvas_ext_gl_texenvi(unsigned int, unsigned int, unsigned int);
extern "C" void paintcanvas_ext_gl_scalef(float, float, float);
extern "C" void paintcanvas_ext_gl_multmatrix(void *);
extern "C" void paintcanvas_ext_string_prep(AbyssEngine::PaintCanvas *, void *, int);
extern "C" void paintcanvas_ext_dsc_settexture(void *self, unsigned int tex);
extern "C" void paintcanvas_ext_dsc_getcolor(void *self);
extern "C" void paintcanvas_ext_dsc_str_copy(void *out, const AbyssEngine::String *src, bool copy);
extern "C" void paintcanvas_ext_dsc_str_fromchar(void *out, const char *s, bool copy);
extern "C" void *paintcanvas_ext_dsc_splittags(void *str, void *sep);
extern "C" void paintcanvas_ext_dsc_str_dtor(void *s);
extern "C" unsigned short *paintcanvas_ext_dsc_str_cast(void *str);
extern "C" int paintcanvas_ext_dsc_textwidth(void *self, unsigned int font, void *str);
extern "C" void paintcanvas_ext_dsc_setcolor(void *self);
extern "C" char *paintcanvas_ext_dsc_getAEChar(void *str);
extern "C" int paintcanvas_ext_dsc_sscanf(const char *s, const char *fmt, void *out);
extern "C" void paintcanvas_ext_dsc_releaseclasses(void *arr);
extern "C" void *paintcanvas_ext_dsc_arr_dtor(void *arr);
extern "C" void paintcanvas_ext_dsc_op_delete(void *p);
extern "C" void paintcanvas_ext_smfg_pushmat(const float *m, void *array);
extern "C" void paintcanvas_ext_ec_glEnable(unsigned int cap);
extern "C" int paintcanvas_ext_ec_getHeight(void *self);
extern "C" int paintcanvas_ext_ec_getWidth(void *self);
extern "C" void paintcanvas_ext_ec_glScissor(int x, int y, int w, int h);
extern "C" void paintcanvas_ext_di3_restore(unsigned int flag, void *img);
extern "C" int paintcanvas_ext_di3_getwidth(void *self);
extern "C" int paintcanvas_ext_di3_getheight(void *self);
extern "C" float paintcanvas_ext_di3_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_di3_settexture(void *self, unsigned int tex);
extern "C" void paintcanvas_ext_di3_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di3_meshdraw(void *eng, void *mesh);
extern "C" int paintcanvas_ext_meshcreate(void *, void *);
extern "C" int paintcanvas_ext_font_get_spacing(void *);
extern "C" void *paintcanvas_ext_mc2_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_mc2_matcreate(void *self, unsigned short id, unsigned int *out);
extern "C" int paintcanvas_ext_mc2_meshfromfile(void *eng, char *path, void **out, void *mat);
extern "C" void *paintcanvas_ext_mc2_new_mesh_copy(void *src);
extern "C" void paintcanvas_ext_mc2_converttovbo(void *mesh);
extern "C" float paintcanvas_ext_fsp_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" void paintcanvas_ext_fsp_glFogf(unsigned int pname, float v);
extern "C" void paintcanvas_ext_fsp_glFogfv(unsigned int pname, void *v);
extern "C" void paintcanvas_ext_fsp_vec_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dr3_settexture(void *self, unsigned int tex);
extern "C" float paintcanvas_ext_dr3_signedtofloat(int v, unsigned int mode);
extern "C" float paintcanvas_ext_dr3_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" float paintcanvas_ext_dr3_sinf(float v);
extern "C" float paintcanvas_ext_dr3_cosf(float v);
extern "C" void paintcanvas_ext_dr3_mtx_mul(void *out, void *m);
extern "C" void paintcanvas_ext_dr3_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dr3_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dr3_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_dr3_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_dr3_glenable(unsigned int cap);
extern "C" void paintcanvas_ext_rar_gldeltex(int n, void *ids);
extern "C" void paintcanvas_ext_rar_str_dtor(void *s);
extern "C" void paintcanvas_ext_rar_op_delete(void *p);
extern "C" void paintcanvas_ext_rar_fontrelease(void *eng, void *fontptr);
extern "C" void paintcanvas_ext_rar_img2drelease(void *eng, void *imgptr);
extern "C" void paintcanvas_ext_rar_meshrelease(void *eng, void *meshptr);
extern "C" void *paintcanvas_ext_rar_transform_dtor(void *p);
extern "C" void *paintcanvas_ext_rar_material_dtor(void *p);
extern "C" void paintcanvas_ext_rar_ssrelease(void *eng, void *ssptr);
extern "C" void paintcanvas_ext_tg2d_memcpy(void *dst, void *src, unsigned int n);
extern "C" float paintcanvas_ext_tg2d_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_tg2d_invtransformvec(void *outMat, void *vec);
extern "C" void paintcanvas_ext_tg2d_vec_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_tg2d_inner(void *out, void *self, void *tf, int x, int y);
extern "C" void paintcanvas_ext_tg2d_errmsg(void *out);
extern "C" void paintcanvas_ext_check_string(void *, unsigned int, unsigned int);
extern "C" char *paintcanvas_ext_rt_getAEChar(void *strField);
extern "C" void paintcanvas_ext_rt_deletearr(char *p);
extern "C" void *paintcanvas_ext_matc_findres(void *self, unsigned short id);
extern "C" void *paintcanvas_ext_matc_new_material();
extern "C" void paintcanvas_ext_matc_texcreate(void *self, unsigned short id, bool b);
extern "C" void paintcanvas_ext_matc_vec_assign(void *dst, void *src);
extern "C" float paintcanvas_ext_tg2di_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_tg2di_meshintersect(void *out, float a, float b, void *mesh);
extern "C" void paintcanvas_ext_tg2di_memcpy(void *dst, void *src, unsigned int n);
extern "C" void paintcanvas_ext_tg2di_invtransformvec(void *outMat, void *vec);
extern "C" void paintcanvas_ext_tg2di_vec_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_tg2di_inner(void *out, void *self, void *childtf, int x, int y);
extern "C" void paintcanvas_ext_dt2_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dt2_getgrav(void *eng);
extern "C" float paintcanvas_ext_dt2_sinf(float v);
extern "C" float paintcanvas_ext_dt2_cosf(float v);
extern "C" int paintcanvas_ext_dt2_incamvf(void *tf, void *m, void *cam);
extern "C" void paintcanvas_ext_dt2_mtx_assign(void *dst, const void *src);
extern "C" void paintcanvas_ext_dt2_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dt2_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dt2_seteye(void *eng, float a, float b, float c);
extern "C" void paintcanvas_ext_dt2_drawrec(void *self, void *tf, void *m, void *m2);
extern "C" void paintcanvas_ext_font_set_spacing(void *, int);
extern "C" int paintcanvas_ext_get_width(void *);
extern "C" int pc_GetWidth(AbyssEngine::PaintCanvas *self);
extern "C" int pc_GetHeight(AbyssEngine::PaintCanvas *self);
extern "C" void pc_Camera_ctor(void *cam, float h, float w);
extern "C" void pc_ArrayAdd_Camera(void *cam, void *arr);
extern "C" void paintcanvas_ext_dss_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dss_getgrav(void *eng);
extern "C" float paintcanvas_ext_dss_sinf(float v);
extern "C" float paintcanvas_ext_dss_cosf(float v);
extern "C" void paintcanvas_ext_dss_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dss_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dss_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dss_ssdraw(void *eng, void *ident, void *m, void *ss);
extern "C" int paintcanvas_ext_mesh_tricount(AbyssEngine::PaintCanvas *, void *);
extern "C" int paintcanvas_ext_transform_tricount(AbyssEngine::PaintCanvas *, void *);
extern "C" void paintcanvas_ext_mesh_shaderanim(AbyssEngine::PaintCanvas *, void *, float, unsigned int);
extern "C" void paintcanvas_ext_transform_shaderanim(AbyssEngine::PaintCanvas *, void *, float, unsigned int);
extern "C" void paintcanvas_ext_release_sprite_res(void *, void *);
extern "C" void paintcanvas_ext_dss2_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dss2_getgrav(void *eng);
extern "C" float paintcanvas_ext_dss2_sinf(float v);
extern "C" float paintcanvas_ext_dss2_cosf(float v);
extern "C" float paintcanvas_ext_dss2_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_dss2_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dss2_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dss2_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dss2_ssdraw(void *eng, void *worldM, void *viewM, void *ss);

unsigned short PaintCanvas::GetImage2DWidth(unsigned int index)
{
    if (index < this->imageCount) {
        char *img = (this->images)[index];
        return *(unsigned short *)(img + 0x10);
    }
    return 0;
}

unsigned int PaintCanvas::CameraGetCurrent()
{
    unsigned int cur = this->currentCamera;
    if (cur >= this->cameraCount) {
        cur = 0xffffffff;
    }
    return cur;
}

void PaintCanvas::HasVibration()
{
    return paintcanvas_ext_has_vibration(this->engine);
}

void *PaintCanvas::CameraGetLocal(unsigned int index)
{
    void *result;
    if (index < this->cameraCount) {
        result = (char *)(this->cameras)[index] + 0xc;
    } else {
        char tmp[60];
        result = &this->identityMatrix;
        MatrixIdentity(tmp, result);
    }
    return result;
}

void PaintCanvas::DrawTextLines(unsigned int font,
                   AbyssEngine::Array<AbyssEngine::String *> *arr, int x, int y)
{
    // Thin forwarder to the (font, arr, x, y, p5, flag) overload.
    this->DrawTextLines(font, arr, x, y, 0u, false);
}

void PaintCanvas::DrawTextLines(unsigned int font,
                   AbyssEngine::Array<AbyssEngine::String *> *arr, int x, int y, bool center)
{
    int xoff = 0;
    for (unsigned int i = 0; i < arr->size(); i++) {
        if (center) {
            int w = paintcanvas_ext_dtl_textwidth(this, font, arr->data()[i]);
            xoff = -(w >> 1);
        }
        paintcanvas_ext_dtl_drawstring(this, font, arr->data()[i], xoff + x, y, false);
        y += paintcanvas_ext_dtl_textheight(this, font);
    }
}

void PaintCanvas::FillRectangle(int x, int y, int w, int h)
{
    char abuf[60];
    float fx = (float)x;
    float fy = (float)y;
    float fx2 = (float)(x + w);
    float fy2 = (float)(h + y);

    float *vb = *(float **)(this->lineMesh + 0x4);
    vb[0] = fx; vb[1] = fy;
    vb[3] = fx2; vb[4] = fy;
    vb[6] = fx2; vb[7] = fy2;
    vb[9] = fx; vb[10] = fy2;

    float *m = (float *)abuf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;

    paintcanvas_ext_fr_setwvm(this, abuf);
    paintcanvas_ext_fr_glenable(this->engine, 0xde1, false);
    paintcanvas_ext_fr_meshdraw(this->engine, this->lineMesh);
    paintcanvas_ext_fr_glenable(this->engine, 0xde1, true);
}

void PaintCanvas::SpriteSystemSetRGBA(unsigned int index, unsigned short sub,
                         float a, float b, float c, float d)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            return paintcanvas_ext_sprite_rgba(sub, a, b, c, d, s);
        }
    }
}

void PaintCanvas::TransformCreate(unsigned int &out)
{
    void *obj = paintcanvas_ext_alloc(0x180);
    paintcanvas_ext_transform_ctor(obj);
    paintcanvas_ext_add_child(obj, &this->transformCount);
    out = this->transformCount - 1;
}

__attribute__((visibility("hidden"))) extern char *const g_dr_flag_79368;

void PaintCanvas::DrawRectangle(int x, int y, int w, int h)
{
    char abuf[60];

    double dw = (double)w;
    double dh = (double)h;
    double dy = (double)y;
    double dx = (double)x;
    float right = (float)(dw - 0.5 + dx);
    float bottom = (float)(dh - 0.5 + dy);

    float *v = this->lineVerts;
    v[0] = (float)(dx + 0.5);
    v[1] = (float)(dy + 0.5);
    v[2] = right;
    v[3] = (float)(dy + 0.5);
    v[4] = right;
    v[5] = bottom;
    v[6] = (float)(dx + 0.5);
    v[7] = bottom;

    float *m = (float *)abuf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;

    paintcanvas_ext_dr_setwvm(this, abuf);
    if (*g_dr_flag_79368 == 0) {
        paintcanvas_ext_dr_glLineWidth(1.0f);
        paintcanvas_ext_dr_glcap(this->engine, 0xde1, 0);
        paintcanvas_ext_dr_glVertexPointer(2, 0x1406, 0, this->lineVerts);
        paintcanvas_ext_dr_glColorMask(this->engine, 0x8074, 1);
        paintcanvas_ext_dr_glColorMask(this->engine, 0x8078, 0);
        paintcanvas_ext_dr_glColorMask(this->engine, 0x8075, 0);
        paintcanvas_ext_dr_glColorMask(this->engine, 0x8076, 0);
        paintcanvas_ext_dr_glDrawArrays(2, 0, 4);
        paintcanvas_ext_dr_glcap(this->engine, 0xde1, 1);
    } else {
        paintcanvas_ext_dr_drawline2d(this->engine, this->lineVerts, 4, true);
    }
}

unsigned short PaintCanvas::GetImage2DHeight(unsigned int index)
{
    if (index < this->imageCount) {
        char *img = (this->images)[index];
        return *(unsigned short *)(img + 0x12);
    }
    return 0;
}

extern char paintcanvas_g_flipv;

void PaintCanvas::MeshSetUv(unsigned int index, unsigned short sub,
               float u, float v)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0x8) + sub * 8);
            p[0] = u;
            if (paintcanvas_g_flipv != 0) {
                p[1] = 1.0f - v;
                return;
            }
            p[1] = v;
        }
    }
}

void PaintCanvas::StopDraw2FBO()
{
    paintcanvas_ext_fbo_a(this->engine);
    paintcanvas_ext_fbo_b(this, 0);
    paintcanvas_ext_fbo_c(this->engine);
    return paintcanvas_ext_fbo_d(this->engine);
}

void PaintCanvas::SpriteSystemSetAllUv(unsigned int index,
                          float a, float b, float c, float d)
{
    if (this->spriteSystemCount <= index) {
        return;
    }
    void *sprite = (this->spriteSystems)[index];
    if (sprite == 0) {
        return;
    }
    return paintcanvas_ext_sprite_alluv(a, b, c, d, sprite);
}

using AbyssEngine::AEMath::Vector;

void PaintCanvas::SpriteSystemGetPosition(unsigned int index, unsigned short sub, Vector &out)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            float *p = (float *)(*(char **)((char *)s + 0x4) + sub * 12);
            out.x = p[0];
            out.y = p[1];
            out.z = p[2];
        }
    }
}

void PaintCanvas::SpriteSystemSetAllSize(unsigned int index, short size)
{
    if (index < this->spriteSystemCount) {
        void *sprite = (this->spriteSystems)[index];
        if (sprite) {
            return paintcanvas_ext_sprite_allsize(size, sprite);
        }
    }
}

void PaintCanvas::RemoveAllMatsForGlow()
{
    PCArrayRemoveAll(&this->glowMeshes_count);
    PCArrayRemoveAll(&this->glowMatA_count);
    PCArrayRemoveAll(&this->glowMatB_count);
    PCArrayRemoveAll(&this->glowUints_count);
    return PCArrayRemoveAll(&this->glowMatC_count);
}

void PaintCanvas::MaterialChange(unsigned int index,
                    AbyssEngine::BlendMode param3, unsigned int param4)
{
    if (index < this->materialCount) {
        unsigned int *mat = (unsigned int *)(this->materials)[index];
        mat[8] = param3;
        mat[0] = param4;
    }
}

void PaintCanvas::DrawTextLines(unsigned int font,
                   AbyssEngine::Array<AbyssEngine::String *> *arr, int x, int y, unsigned int p5, bool flag)
{
    int xoff = 0;
    for (unsigned int i = 0; i < arr->size(); i++) {
        if (flag == 0) {
            int w = paintcanvas_ext_dtl_textwidth(this, font, arr->data()[i]);
            xoff = (int)p5 - w;
        }
        paintcanvas_ext_dtl_drawstring(this, font, arr->data()[i], xoff + x, y, false);
        y += paintcanvas_ext_dtl_textheight(this, font);
    }
}

void PaintCanvas::MeshResourceChangeMaterial(unsigned short matId, unsigned short value)
{
    unsigned int count = this->resourceCount;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (this->resources)[i];
        if (res) {
            if (*(unsigned short *)res == matId) {
                char *mat = *(char **)(res + 0xc);
                *(unsigned short *)(mat + 0x4) = value;
            }
        }
    }
}

void *PaintCanvas::TransformGetLocal(unsigned int index)
{
    void *result;
    if (index < this->transformCount) {
        result = (this->transforms)[index];
    } else {
        char tmp[60];
        result = &this->identityMatrix;
        MatrixIdentity(tmp, result);
    }
    return result;
}

using AbyssEngine::AEMath::Vector;

void PaintCanvas::MeshSetTangent(unsigned int index, unsigned short vtx, const Vector &value)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x14);
        *(Vector *)(base + vtx * 12) = value;
    }
}

int PaintCanvas::GetTextWidth(unsigned int index, const AbyssEngine::String &str,
                 unsigned int begin, unsigned int end)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        void *text = paintcanvas_ext_str_text(&str);
        return paintcanvas_ext_text_width_range(font, text, begin, end - begin);
    }
    return 0;
}

void *PaintCanvas::MaterialGetMaterial(unsigned int index)
{
    if (index < this->materialCount) {
        return (this->materials)[index];
    }
    return 0;
}

__attribute__((visibility("hidden"))) char *paintcanvas_g_bg_flag;

void PaintCanvas::EndBG()
{
    *paintcanvas_g_bg_flag = this->bgFlagSaved;
}

void *PaintCanvas::FindResource(unsigned short id)
{
    unsigned int count = this->resourceCount;
    void *found = 0;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (this->resources)[i];
        if (res && *(unsigned short *)res == id) {
            found = res;
            break;
        }
    }
    return found;
}

AbyssEngine::Mesh *PaintCanvas::MeshGetPointer(unsigned int index)
{
    if (index < this->meshCount) {
        // meshes[index] is a mesh handle stored in a char* slot; reinterpret it
        // as the Mesh* it actually is without a pointer cast (C++14, no-cast rule).
        AbyssEngine::Mesh *mesh;
        char *slot = (this->meshes)[index];
        memcpy(&mesh, &slot, sizeof(mesh));
        return mesh;
    }
    return 0;
}

void PaintCanvas::MeshChangeResourceMaterial(unsigned int meshIndex, unsigned short resId)
{
    char *r = paintcanvas_ext_find_res(this, resId);
    if (r) {
        int idx = *(int *)(r + 0x8);
        if (idx + 1 != 0) {
            void *mesh = (this->meshes)[meshIndex];
            void *mat = (this->materials)[idx];
            return paintcanvas_ext_change_mat(this, mesh, mat);
        }
    }
}

void PaintCanvas::SetColor(unsigned int color)
{
    float c0 = (float)((double)(color >> 24) / 255.0);
    float c1 = (float)((double)((color >> 16) & 0xff) / 255.0);
    float c2 = (float)((double)((color >> 8) & 0xff) / 255.0);
    float c3 = (float)((double)(color & 0xff) / 255.0);
    this->colorR = c0;
    this->colorG = c1;
    this->colorB = c2;
    this->colorA = c3;
    return paintcanvas_ext_setcolor(this->engine, c0, c1, c2, c3);
}

void PaintCanvas::Vibrate(unsigned short)
{
    return paintcanvas_ext_vibrate(this->engine);
}

void PaintCanvas::SpriteSystemAddSize(unsigned int index, unsigned short sub, short delta)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if ((unsigned short)*(short *)s <= (unsigned int)sub) {
                return;
            }
            short *vals = *(short **)((char *)s + 0x8);
            if (*(char *)((char *)s + 0xc) != 0) {
                vals[0] += delta;
                return;
            }
            vals[sub] += delta;
        }
    }
}

using AbyssEngine::AEMath::Matrix;

void PaintCanvas::TransformSetLocal(unsigned int index, const Matrix &matrix)
{
    if (index < this->transformCount) {
        Matrix *t = (Matrix *)(this->transforms)[index];
        *t = matrix;
    }
}

unsigned int PaintCanvas::GetMeshResourceId(AbyssEngine::String &name, unsigned short p2)
{
    for (unsigned int i = 0; i < this->resourceCount; ++i) {
        char *res = (this->resources)[i];
        if (res && *(int *)(res + 0x4) == 4) {
            if (paintcanvas_ext_strcmp(&name, **(void ***)(res + 0xc)) == 0) {
                char *res2 = (this->resources)[i];
                if (*(unsigned short *)(*(char **)(res2 + 0xc) + 0x4) == p2) {
                    return *(unsigned short *)res2;
                }
            }
        }
    }
    return 0xffff;
}

int PaintCanvas::GetTextWidth(unsigned int index, const AbyssEngine::String &str)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        void *data = paintcanvas_ext_str_text(&str);
        unsigned int len = paintcanvas_ext_strlen(data);
        return paintcanvas_ext_text_width(font, len, str.size());
    }
    return 0;
}

void PaintCanvas::SpriteSystemSetSize(unsigned int index,
                         unsigned short sub, short value)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if (sub < *(unsigned short *)s) {
                unsigned short *p = *(unsigned short **)(s + 0x8);
                if (*(unsigned char *)(s + 0xc)) {
                    p[0] = value;
                } else {
                    p[sub] = value;
                }
            }
        }
    }
}

void PaintCanvas::SetColor(unsigned char r, unsigned char g,
              unsigned char b, unsigned char a)
{
    float fr = (float)((double)(unsigned int)r / 255.0);
    float fg = (float)((double)(unsigned int)g / 255.0);
    float fb = (float)((double)(unsigned int)b / 255.0);
    float fa = (float)((double)(unsigned int)a / 255.0);
    this->colorR = fr;
    this->colorG = fg;
    this->colorB = fb;
    this->colorA = fa;
    return paintcanvas_ext_setcolor(this->engine, fr, fg, fb, fa);
}

float PaintCanvas::CameraGetCurrentFactor1()
{
    unsigned int cur = this->currentCamera;
    if (cur >= this->cameraCount) {
        return 1.0f;
    }
    char *cam = (char *)(this->cameras)[cur];
    return *(float *)(cam + 0x48);
}

void PaintCanvas::SpriteSystemAddPosition(unsigned int index, unsigned short sub,
                             float x, float y, float z)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            float *p = (float *)(*(char **)(s + 0x4) + sub * 12);
            p[0] = p[0] + x;
            p[1] = p[1] + y;
            p[2] = p[2] + z;
        }
    }
}

using AbyssEngine::AEMath::Vector;

void PaintCanvas::MeshSetBiTangent(unsigned int index, unsigned short vtx, const Vector &value)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x18);
        *(Vector *)(base + vtx * 12) = value;
    }
}

using AbyssEngine::AEMath::Vector;

int PaintCanvas::GetScreenPosition(const AbyssEngine::AEMath::Vector &a,
                                   AbyssEngine::AEMath::Vector &b)
{
    char buf[60];
    float *m = (float *)buf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;
    // The delegated 4-arg overload returns the on-screen visibility flag in r0
    // (see android GetScreenPosition @ 0x7bd58: r6 = 1 only when fully on-screen).
    return paintcanvas_ext_getscreenpos_m(this, buf, &a, &b);
}

#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_fog_flag HIDDEN;
extern char *paintcanvas_g_fog_ptr HIDDEN;

void PaintCanvas::FogEnable(bool mode, AbyssEngine::FogMode enable)
{
    this->fogEnableFlag = enable;
    if (enable == 0) {
        if (*paintcanvas_g_fog_flag != 0) {
            *paintcanvas_g_fog_ptr = (char)mode;
        } else if (mode == 0) {
            paintcanvas_ext_gl_disable(0xb60);
        } else {
            paintcanvas_ext_gl_enable(0xb60);
        }
        mode = 0;
    }
    this->fogMode = (char)mode;
}

void PaintCanvas::MeshSetColor(unsigned int index, unsigned short sub,
                  unsigned int color)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0xc) + sub * 0x10);
            p[0] = (float)((double)(color >> 24) / 255.0);
            p[1] = (float)((double)((color >> 16) & 0xff) / 255.0);
            p[2] = (float)((double)((color >> 8) & 0xff) / 255.0);
            p[3] = (float)((double)(color & 0xff) / 255.0);
        }
    }
}

#define HIDDEN __attribute__((visibility("hidden")))
extern float *paintcanvas_g_pom_persp HIDDEN;
extern float *paintcanvas_g_pom_a HIDDEN;
extern float *paintcanvas_g_pom_b HIDDEN;

void PaintCanvas::SetProjOrthoMatrix()
{
    float g = *paintcanvas_g_pom_persp;
    if (g != -1.0f) {
        paintcanvas_ext_setprojmatrix3d(this, g, *paintcanvas_g_pom_a, *paintcanvas_g_pom_b);
    }
    void *eng = this->engine;
    float *r;
    r = &this->projOrthoMatrix.m[12]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = &this->projOrthoMatrix.m[8]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = &this->projOrthoMatrix.m[4]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = &this->projOrthoMatrix.m[0]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;

    int w = paintcanvas_ext_getdisplaywidth(eng);
    this->projOrthoMatrix.m[0] = (float)(2.0 / (double)w);

    int h = paintcanvas_ext_getdisplayheight(this->engine);
    this->projOrthoMatrix.m[10] = -0.05f;
    this->projOrthoMatrix.m[15] = 1.0f;
    this->projOrthoMatrix.m[12] = -1.0f;
    this->projOrthoMatrix.m[13] = 1.0f;
    this->projOrthoMatrix.m[5] = -(float)(2.0 / (double)h);
}

void PaintCanvas::MeshChangeMaterialIntern(AbyssEngine::Mesh *mesh, AbyssEngine::Material *mat)
{
    if (mesh && mat) {
        mesh->field_0x30 = mat;
        return paintcanvas_ext_mat_intern(this, mesh->field_0x34);
    }
}

void PaintCanvas::MeshResourceChangeAllMaterial(unsigned short matId, unsigned short value)
{
    unsigned int count = this->resourceCount;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (this->resources)[i];
        if (res) {
            char *mat = *(char **)(res + 0xc);
            if (*(unsigned short *)(mat + 0x4) == matId) {
                *(unsigned short *)(mat + 0x4) = value;
            }
        }
    }
}

unsigned int PaintCanvas::GetTextureResourceId(AbyssEngine::String &name)
{
    for (unsigned int i = 0; i < this->resourceCount; ++i) {
        char *res = (this->resources)[i];
        if (res && *(int *)(res + 0x4) == 2) {
            char *n = *(char **)(res + 0xc);
            if (paintcanvas_ext_strcmp(&name, *(void **)n) == 0) {
                return *(unsigned short *)((this->resources)[i]);
            }
        }
    }
    return 0xffff;
}

unsigned int PaintCanvas::GetColor()
{
    float a = this->colorR;
    float r = this->colorG;
    float g = this->colorB;
    float b = this->colorA;
    return ((unsigned int)(int)(r * 255.0f) << 16) +
           ((unsigned int)(int)(a * 255.0f) << 24) +
           ((unsigned int)(int)(g * 255.0f) << 8) +
           (unsigned int)(int)(b * 255.0f);
}

void PaintCanvas::CameraSetCurrent(unsigned int index)
{
    this->currentCamera = index;
    if (index < this->cameraCount) {
        unsigned int *cam = (unsigned int *)(this->cameras)[index];
        return paintcanvas_ext_camera_apply(this, cam[0], cam[1], cam[2]);
    }
}

void PaintCanvas::MeshSetTriangleCount(unsigned int index, unsigned short count)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        unsigned short cap = *(unsigned short *)(mesh + 0x2a);
        if (cap < count) {
            count = cap;
        }
        *(unsigned short *)(mesh + 0x28) = (unsigned short)(count * 3);
    }
}

void PaintCanvas::DisableClip()
{
    paintcanvas_ext_disable(0xc11);
}

void PaintCanvas::TransformSetColor(unsigned int index, unsigned int color)
{
    if (index < this->transformCount) {
        char *obj = (this->transforms)[index];
        *(unsigned int *)(obj + 0x48) = color;
    }
}

void PaintCanvas::MeshSetColor(unsigned int index, unsigned short sub,
                  float r, float g, float b, float a)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0xc) + sub * 0x10);
            p[0] = r;
            p[1] = g;
            p[2] = b;
            p[3] = a;
        }
    }
}

using AbyssEngine::AEMath::Vector;

void PaintCanvas::MeshSetNormal(unsigned int index, unsigned short vtx, const Vector &value)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x10);
        *(Vector *)(base + vtx * 12) = value;
    }
}

void PaintCanvas::MeshClear2DMask()
{
    this->mask2dImage = 0;
}

unsigned int PaintCanvas::GetMeshResourceId(AbyssEngine::String &name)
{
    for (unsigned int i = 0; i < this->resourceCount; ++i) {
        char *res = (this->resources)[i];
        if (res && *(int *)(res + 0x4) == 4) {
            char *n = *(char **)(res + 0xc);
            if (paintcanvas_ext_strcmp(&name, *(void **)n) == 0) {
                return *(unsigned short *)((this->resources)[i]);
            }
        }
    }
    return 0xffff;
}

void PaintCanvas::MeshTranslatePoint(unsigned int index, unsigned short sub,
                        float x, float y, float z)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if ((unsigned int)*(unsigned short *)(mesh + 0x2) <= (unsigned int)sub) {
            return;
        }
        float *p = (float *)(*(char **)(mesh + 0x4) + sub * 12);
        p[0] = p[0] + x;
        p[1] = p[1] + y;
        p[2] = p[2] + z;
    }
}

extern char paintcanvas_g_refract1;
extern char paintcanvas_g_refract2;

void PaintCanvas::CheckNUseRefractFBO(bool)
{
    if (paintcanvas_g_refract1 != 0 && paintcanvas_g_refract2 != 0 &&
        paintcanvas_ext_is_posteffect(this->engine) == 0) {
        return paintcanvas_ext_use_refract(this->engine);
    }
}

void PaintCanvas::SpriteSystemSetUv(unsigned int index, unsigned short sub,
                       float a, float b, float c, float d)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            return paintcanvas_ext_sprite_uv(sub, a, b, c, d, s);
        }
    }
}

void PaintCanvas::SetWorldViewMatrix(const AbyssEngine::AEMath::Matrix &)
{
    return paintcanvas_ext_set_wvm(this->engine);
}

using AbyssEngine::AEMath::Matrix;

void PaintCanvas::CameraSetLocal(unsigned int index, const Matrix &matrix)
{
    if (index < this->cameraCount) {
        char *cam = (char *)(this->cameras)[index];
        *(Matrix *)(cam + 0xc) = matrix;
    }
}

void PaintCanvas::SetShaderMode(int mode)
{
    ((Engine *)this->engine)->field_0x4a8 = mode;
}

void PaintCanvas::MeshConvertToVBO(unsigned int index)
{
    if (index < this->meshCount) {
        return paintcanvas_ext_convert_vbo(
            (this->meshes)[index]);
    }
}

__attribute__((visibility("hidden"))) extern const float g_di2_one_88d90;
__attribute__((visibility("hidden"))) extern const float g_di2_def_88d94;

void PaintCanvas::DrawImage2D(unsigned int index, int x, int y,
                 unsigned char flipFlags)
{
    if (index >= this->imageCount) {
        return;
    }
    char *img = ((char **)this->images)[index];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di2_restore(*(unsigned char *)(img + 0x14), img);
        img = ((char **)this->images)[index];
    }
    paintcanvas_ext_di2_settexture(this, *(unsigned int *)(img + 4), -1);

    float fx = paintcanvas_ext_di2_signedtofloat(x, 0);
    float fy = paintcanvas_ext_di2_signedtofloat(y, 0);

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;

    if (flipFlags & 1) {
        m[0] = g_di2_one_88d90;
        float off = g_di2_def_88d94;
        if (index < this->imageCount) {
            unsigned short w = *(unsigned short *)(((char **)this->images)[index] + 0x10);
            off = paintcanvas_ext_di2_unsignedtofloat(w, 0);
        }
        fx = off + fx;
    }
    if (flipFlags & 2) {
        m[5] = g_di2_one_88d90;
        float off = g_di2_def_88d94;
        if (index < this->imageCount) {
            unsigned short h = *(unsigned short *)(((char **)this->images)[index] + 0x12);
            off = paintcanvas_ext_di2_unsignedtofloat(h, 0);
        }
        fy = off + fy;
    }
    m[3] = fx;
    m[7] = fy;

    paintcanvas_ext_di2_setwvm(this, m);
    paintcanvas_ext_di2_gldisable(0xb44);
    paintcanvas_ext_di2_meshdraw(this->engine,
                                 *(void **)(((char **)this->images)[index]));
    paintcanvas_ext_di2_glenable(0xb44);
}

void PaintCanvas::FontSetYOffset(unsigned int index, short yoff)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        return paintcanvas_ext_font_set_yoff(font, yoff);
    }
}

__attribute__((visibility("hidden"))) extern char *const g_sbm_flag_8cb62;
__attribute__((visibility("hidden"))) extern const unsigned int g_sbm_const_8ce34;

void PaintCanvas::SetBlendMode(AbyssEngine::BlendMode mode)
{
    paintcanvas_ext_sbm_lightenable(this->engine, 0);

    char *flag = g_sbm_flag_8cb62;
    if (*flag != 0) {
        paintcanvas_ext_sbm_glenablecap(this->engine, g_sbm_const_8ce34, 0);
    } else {
        paintcanvas_ext_sbm_glTexEnvi(0x2300, 0x2200, 0x2100);
    }

    switch (mode) {
    case 1:
    case 5:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 2:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 3:
        paintcanvas_ext_sbm_glDisable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 4:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0, 0x301);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 6:
        paintcanvas_ext_sbm_lightenable(this->engine, 1);
        paintcanvas_ext_sbm_lightsetlight(this->engine, 0x4000);
        /* fallthrough */
    case 0:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 7:
        paintcanvas_ext_sbm_lightenable(this->engine, 1);
        paintcanvas_ext_sbm_lightsetlight(this->engine, 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 8:
        paintcanvas_ext_sbm_lightenable(this->engine, 1);
        paintcanvas_ext_sbm_lightsetlight(this->engine, 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 9:
        paintcanvas_ext_sbm_lightenable(this->engine, 1);
        paintcanvas_ext_sbm_lightsetlight(this->engine, 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 10:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_glDepthMask(1);
        paintcanvas_ext_sbm_setalpha(this->engine, 0x1000000, 1);
        if (*flag == 0) {
            paintcanvas_ext_sbm_glAlphaFunc(0x204, 0.5f);
        }
        return;
    case 0x15:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_glDepthMask(0);
        if (*flag != 0) {
            paintcanvas_ext_sbm_setalpha(this->engine, g_sbm_const_8ce34, 1);
            return;
        }
        paintcanvas_ext_sbm_texcombine(0x2300, 0x2200, 0x8570);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8571, 0x104);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8572, 0x1e01);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8580, 0x1702);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8590, 0x300);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8581, 0x8577);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8591, 0x300);
        return;
    case 0x16:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_glDepthMask(0);
        if (*flag == 0) {
            paintcanvas_ext_sbm_texcombine(0x2300, 0x2200, 0x8570);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8571, 0x8575);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8572, 0x1e01);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8580, 0x1702);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8590, 0x300);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8588, 0x1702);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8598, 0x302);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8581, 0x8577);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8591, 0x300);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8582, 0x8577);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8592, 0x302);
        }
        return;
    case 0x25:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(1);
        return;
    default:
        return;
    }
}

__attribute__((visibility("hidden"))) extern const char g_gla_nl_8c4c0[];

void PaintCanvas::GetLineArray(unsigned int font, const AbyssEngine::String &str, int width,
                  ::Array<AbyssEngine::String *> *outArray)
{
    AbyssEngine::String *acc = paintcanvas_ext_gla_str_new();

    char src[16];      // working string
    char nl[16];
    paintcanvas_ext_dsc_str_copy(src, &str, false);
    paintcanvas_ext_gla_str_fromchar(nl, g_gla_nl_8c4c0, false);
    paintcanvas_ext_gla_str_append(src, nl);
    paintcanvas_ext_gla_str_dtor(nl);

    // Pass 1: count lines.
    unsigned int count = 0;
    int pos = 0;
    int srcLen = *(int *)(src + 8);
    while (pos < srcLen) {
        char sub[16];
        char line[16];
        paintcanvas_ext_gla_substr(sub, src, 0, (unsigned int)srcLen);
        paintcanvas_ext_gla_str_copy(line, sub, false);
        paintcanvas_ext_gla_getline(this, font, line, width, acc);
        paintcanvas_ext_gla_str_dtor(line);
        pos += (int)acc->size();
        paintcanvas_ext_gla_str_dtor(sub);
        count++;
    }
    paintcanvas_ext_gla_str_vdtor(acc);

    // Allocate the output array of count Strings.
    paintcanvas_ext_gla_arr_setlen(count, outArray);
    for (unsigned int i = 0; i < count; i++) {
        outArray->data_[i] = paintcanvas_ext_gla_str_new();
    }

    // Pass 2: fill and trim each line.
    for (unsigned int i = 0; i < count; i++) {
        char sub[16];
        char line[16];
        paintcanvas_ext_gla_substr(sub, src, 0, (unsigned int)*(int *)(src + 8));
        paintcanvas_ext_gla_str_copy(line, sub, false);
        AbyssEngine::String *out = outArray->data_[i];
        paintcanvas_ext_gla_getline(this, font, line, width, out);
        paintcanvas_ext_gla_str_dtor(line);

        AbyssEngine::String *cur = outArray->data_[i];
        int len = (int)cur->size();
        int lo = 0;
        while (*paintcanvas_ext_gla_str_index(outArray->data_[i], lo) == 0x20) {
            lo++;
        }
        len++;
        do {
            len--;
        } while (*paintcanvas_ext_gla_str_index(outArray->data_[i], len - 2) == 0x20);

        char trimmed[16];
        paintcanvas_ext_gla_substr(trimmed, outArray->data_[i],
                                   (unsigned int)lo, (unsigned int)len);
        paintcanvas_ext_gla_str_assign(outArray->data_[i], trimmed);
        paintcanvas_ext_gla_str_dtor(trimmed);
        paintcanvas_ext_gla_str_dtor(sub);
    }
    paintcanvas_ext_gla_str_dtor(src);
}

void PaintCanvas::AddResource(AbyssEngine::Resource *resource)
{
    return paintcanvas_ext_add_resource(resource, &this->resourceCount);
}

void PaintCanvas::TransformRemoveMeshId(unsigned int transformIndex, unsigned int meshIndex)
{
    if (transformIndex < this->transformCount &&
        meshIndex < this->meshCount) {
        void *mesh = (this->meshes)[meshIndex];
        char *t = (this->transforms)[transformIndex];
        return paintcanvas_ext_remove_meshid(mesh, t + 0x3c);
    }
}

int PaintCanvas::GetHeight()
{
    return paintcanvas_ext_get_height(this->engine);
}

extern char paintcanvas_g_cube_enabled;
extern int paintcanvas_g_cube_slot;

void PaintCanvas::ChangeCubeTexture(unsigned int idx)
{
    if (paintcanvas_g_cube_enabled != 0 && idx < this->cubeTextureCount) {
        char *tex = (this->cubeTextures)[idx];
        if (*(unsigned char *)(tex + 0x14) == 0) {
            return paintcanvas_ext_cube_tail(paintcanvas_ext_cube_restore(tex + 0x4));
        }
        paintcanvas_g_cube_slot = idx;
        paintcanvas_ext_gl_a(0x84c7);
        char *tex2 = (this->cubeTextures)[idx];
        paintcanvas_ext_gl_bind(0x8513, *(unsigned int *)tex2);
        return paintcanvas_ext_gl_c(0x84c0);
    }
}

namespace AbyssEngine {
}

void PaintCanvas::TransformCreate(unsigned short resId, unsigned int &out)
{
    char *res = (char *)paintcanvas_ext_tfc_findres(this, resId);
    if (res == 0) {
        return;
    }
    if (*(unsigned int *)(res + 8) != 0xffffffff) {
        out = *(unsigned int *)(res + 8);
        return;
    }
    char *info = *(char **)(res + 0xc);
    char *tf = (char *)paintcanvas_ext_tfc_new_transform();
    ArrayAdd< ::Transform *>((::Transform *)tf, *reinterpret_cast<Array< ::Transform *> *>(&this->transformCount));
    unsigned int idx = this->transformCount - 1;
    *(unsigned int *)(res + 8) = idx;
    out = idx;
    paintcanvas_ext_tfc_mtx_assign(tf, info);

    unsigned int childMesh = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x3c); i++) {
        unsigned short mid = *(unsigned short *)(*(char **)(info + 0x40) + i * 2);
        paintcanvas_ext_tfc_meshcreate(this, mid, &childMesh, false);
    }
    unsigned int childTf = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x44); i++) {
        unsigned short tid = *(unsigned short *)(*(char **)(info + 0x48) + i * 2);
        this->TransformCreate(tid, childTf);
    }
}

void PaintCanvas::End2d()
{
    char buf[60];
    int v = this->field_0xc;
    if (v >= 1) {
        float *m = (float *)buf;
        *(short *)(this->quad2dMesh + 0x28) = (short)(v * 6);
        m[0] = 1.0f;
        m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
        m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
        m[14] = 1.0f;
        paintcanvas_ext_set_wvm2(this, buf);
        paintcanvas_ext_meshdraw(this->engine, this->quad2dMesh);
    }
}

void PaintCanvas::MeshChangeMaterialIntern(::Transform *transform, AbyssEngine::Material *material)
{
    if (transform && material) {
        for (unsigned int i = 0; i < transform->field_0x3c; ++i) {
            this->MeshChangeMaterialIntern(transform->field_0x40[i], material);
        }
        for (unsigned int i = 0; i < transform->field_0x4c; ++i) {
            this->MeshChangeMaterialIntern(transform->field_0x50[i], material);
        }
    }
}

int PaintCanvas::GetTextHeight(unsigned int index)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        return paintcanvas_ext_text_height(font);
    }
    return 0;
}

__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b4;
__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b8;

void PaintCanvas::Initialize(bool landscape)
{
    this->gameOrientation = landscape ? 2 : 0;
    paintcanvas_ext_init_setorientation(this->engine);

    // zero out the four 4-float blocks at 0x78, 0x88, 0x98, 0xa8
    memset(&this->projOrthoMatrix.m[0], 0, 0x10);
    memset(&this->projOrthoMatrix.m[4], 0, 0x10);
    memset(&this->projOrthoMatrix.m[8], 0, 0x10);
    memset(&this->projOrthoMatrix.m[12], 0, 0x10);

    int orient = this->gameOrientation;
    int w = paintcanvas_ext_init_dispwidth(this->engine);
    float fw = paintcanvas_ext_init_signedtofloat(w, 0);
    int h = paintcanvas_ext_init_dispheight(this->engine);
    float fh = paintcanvas_ext_init_signedtofloat(h, 0);

    float ymul;
    if (orient == 2) {
        this->projOrthoMatrix.m[0] = 2.0f / fw;
        ymul = fh;
    } else {
        memset(&this->worldViewMatrix.m[0], 0, 0x10);
        memset(&this->worldViewMatrix.m[4], 0, 0x10);
        memset(&this->worldViewMatrix.m[8], 0, 0x10);
        memset(&this->worldViewMatrix.m[11], 0, 0x10);
        this->worldViewMatrix.m[15] = 1.0f;
        this->projOrthoMatrix.m[0] = 2.0f / fh;
        this->worldViewMatrix.m[1] = g_init_const_7e7b4;
        this->worldViewMatrix.m[10] = 1.0f;
        this->worldViewMatrix.m[4] = 1.0f;
        this->worldViewMatrix.m[13] = fw;
        ymul = fw;
    }
    this->projOrthoMatrix.m[10] = g_init_const_7e7b8;
    this->projOrthoMatrix.m[15] = 1.0f;
    this->projOrthoMatrix.m[12] = g_init_const_7e7b4;
    this->projOrthoMatrix.m[13] = 1.0f;
    this->projOrthoMatrix.m[5] = -2.0f / ymul;

    paintcanvas_ext_init_setpersp(this, 16384.0f, 512.0f, 65536.0f);
}

void PaintCanvas::DrawMesh(unsigned int index)
{
    if (index < this->meshCount) {
        void *mesh = (this->meshes)[index];
        return paintcanvas_ext_draw_mesh(this->engine, mesh);
    }
}

int PaintCanvas::MeshGetTriCount(AbyssEngine::Mesh *mesh)
{
    if (mesh) {
        int tri;
        if (mesh->animation == 0) {
            tri = 0;
        } else {
            tri = paintcanvas_ext_transform_tricount(this, mesh->animation);
        }
        int q = (int)__aeabi_uidiv(mesh->indexCount, 3);
        return q + tri;
    }
    return 0;
}

int PaintCanvas::FontGetYOffset(unsigned int index)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        return paintcanvas_ext_font_get_yoff(font);
    }
    return 0;
}

__attribute__((visibility("hidden"))) extern float *const g_spm_p0_7b826;
__attribute__((visibility("hidden"))) extern float *const g_spm_p1_7b82a;
__attribute__((visibility("hidden"))) extern float *const g_spm_p2_7b82c;
__attribute__((visibility("hidden"))) extern float *const g_spm_p3_7b82e;
__attribute__((visibility("hidden"))) extern float *const g_spm_p4_7b830;
__attribute__((visibility("hidden"))) extern const unsigned int g_spm_const_7b950;

void PaintCanvas::SetProjectionMatrix3d(float fov, float nearPlane,
                           float farPlane)
{

    // stash parameters into the engine globals
    *g_spm_p0_7b826 = farPlane;
    *g_spm_p1_7b82a = nearPlane;
    *g_spm_p4_7b830 = fov;
    *g_spm_p2_7b82c = nearPlane;
    *g_spm_p3_7b82e = farPlane;

    int w = paintcanvas_ext_spm_dispwidth(this->engine);
    int h = paintcanvas_ext_spm_dispheight(this->engine);
    float half = fov * 0.5f;
    float s = paintcanvas_ext_spm_sinf(half);
    float c = paintcanvas_ext_spm_cosf(half);
    float fw = paintcanvas_ext_spm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_spm_signedtofloat(h, 0);

    // zero the four matrix rows at 0x38,0x48,0x58,0x68
    memset(&this->projMatrix3d.m[12], 0, 0x10);
    memset(&this->projMatrix3d.m[8], 0, 0x10);
    memset(&this->projMatrix3d.m[4], 0, 0x10);
    memset(&this->projMatrix3d.m[0], 0, 0x10);

    if (this->gameOrientation <= 3) {
        float aspect = fw / fh;
        float f = 1.0f / (s / c);
        switch (this->gameOrientation) {
        case 0:
            this->projMatrix3d.m[4] = -f;
            this->projMatrix3d.m[1] = f / aspect;
            break;
        case 1:
            this->projMatrix3d.m[4] = f;
            this->projMatrix3d.m[1] = -(f / aspect);
            break;
        case 2:
            this->projMatrix3d.m[5] = f;
            this->projMatrix3d.m[0] = f / aspect;
            break;
        case 3:
            this->projMatrix3d.m[5] = -f;
            this->projMatrix3d.m[0] = -(f / aspect);
            break;
        }
    }

    this->projMatrix3d.m[11] = g_spm_const_7b950;
    this->projMatrix3d.m[10] = (nearPlane + farPlane) / (nearPlane - farPlane);
    this->projMatrix3d.m[14] = ((farPlane + farPlane) * nearPlane) / (nearPlane - farPlane);
}

__attribute__((visibility("hidden"))) extern const double g_dss1_gravscale_8ac10;

void PaintCanvas::DrawSpriteSystem(unsigned int index)
{
    if (index >= this->spriteSystemCount) {
        return;
    }
    if (((void **)this->spriteSystems)[index] == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f;
    worldM[5] = 1.0f;
    worldM[14] = 1.0f;

    if (this->currentCamera < this->cameraCount) {
        if (this->initialized == 0) {
            float inv[16];
            memset(inv, 0, sizeof(inv));
            inv[0] = 1.0f;
            inv[5] = 1.0f;
            inv[14] = 1.0f;
            paintcanvas_ext_dss1_mtx_getinv(inv, worldM);
            paintcanvas_ext_dss1_mtx_assign(worldM, inv);
        } else {
            float rotM[16];
            char scratch[60];
            memset(rotM, 0, sizeof(rotM));
            rotM[0] = 1.0f;
            rotM[5] = 1.0f;
            rotM[14] = 1.0f;
            paintcanvas_ext_dss1_matidentity(scratch, rotM);

            void *grav = paintcanvas_ext_dss1_getgrav(this->engine);
            double angle = *(double *)((char *)grav + 8) * g_dss1_gravscale_8ac10;
            float a = (float)angle;
            int orient = this->gameOrientation;
            float rot = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dss1_sinf(rot);
            float c = paintcanvas_ext_dss1_cosf(rot);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            // copy the active camera's view matrix (at cam+0xc) into scratch
            char *cam = ((char **)this->cameras)[this->currentCamera];
            paintcanvas_ext_dss1_memcpy(scratch, cam + 0xc, 0x3c);
            paintcanvas_ext_dss1_mtx_muleq(scratch, rotM);
            paintcanvas_ext_dss1_mtx_getinv(scratch, scratch);
            paintcanvas_ext_dss1_mtx_assign(worldM, scratch);
        }
    }

    float ident[16];
    memset(ident, 0, sizeof(ident));
    ident[0] = 1.0f;
    ident[5] = 1.0f;
    ident[14] = 1.0f;
    paintcanvas_ext_dss1_ssdraw(this->engine, ident, worldM,
                                ((void **)this->spriteSystems)[index]);
}

float PaintCanvas::MeshSetPoint(unsigned int index, unsigned short vtx,
                   float x, float y, float z)
{
    if (index < this->meshCount) {
        char *mesh = (this->meshes)[index];
        if (vtx < *(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0x4) + vtx * 12);
            p[0] = x;
            p[1] = y;
            p[2] = z;
            return z;
        }
    }
    return x;
}

__attribute__((visibility("hidden"))) extern const char g_getline_empty_7c428[];

extern "C" int paintcanvas_ext_gl_textwidth(void *self, unsigned int font, const AbyssEngine::String *str,
                                            unsigned int begin, unsigned int end);

void PaintCanvas::GetLine(unsigned int font, AbyssEngine::String str, int maxWidth,
             AbyssEngine::String *out)
{
    char tmp[16];           // String scratch
    unsigned int lastSpace = 0;
    int width = 5;
    unsigned int i = 0;
    unsigned int len = str.size();

    while (i < len) {
        unsigned short *ch = paintcanvas_ext_gl_strindex(&str, i);
        unsigned short c = *ch;
        unsigned int next = i + 1;
        width += paintcanvas_ext_gl_textwidth(this, font, &str, i, next);
        if (c == 0x20) {
            lastSpace = i;
        }
        if (width >= maxWidth) {
            // break out: word-wrap path
            if ((int)lastSpace < 1) {
                paintcanvas_ext_gl_substr(tmp, &str, 0, next);
            } else {
                paintcanvas_ext_gl_substr(tmp, &str, 0, lastSpace + 1);
            }
            paintcanvas_ext_gl_str_assign(out, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
        unsigned short *ch2 = paintcanvas_ext_gl_strindex(&str, i);
        if (*ch2 == 0xa) {
            paintcanvas_ext_gl_substr(tmp, &str, 0, next);
            paintcanvas_ext_gl_str_assign(out, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
        unsigned short *ch3 = paintcanvas_ext_gl_strindex(&str, i);
        i = next;
        if (*ch3 == 0xd) {
            paintcanvas_ext_gl_substr(tmp, &str, 0, next);
            paintcanvas_ext_gl_str_assign(out, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
    }

    if ((int)len < 2) {
        paintcanvas_ext_gl_str_fromchar(tmp, g_getline_empty_7c428, false);
    } else {
        paintcanvas_ext_gl_substr(tmp, &str, 0, len);
    }
    paintcanvas_ext_gl_str_assign(out, tmp);
    paintcanvas_ext_gl_str_dtor(tmp);
}

__attribute__((visibility("hidden"))) extern char *const g_dl_flag_794ee;

void PaintCanvas::DrawLine(int x0, int y0, int x1i, int y1i)
{
    char abuf[60];

    float x1 = paintcanvas_ext_dl_signedtofloat(x0 + 1, 0);
    float y1 = paintcanvas_ext_dl_signedtofloat(y0, 0);
    float x2 = paintcanvas_ext_dl_signedtofloat(x1i + 1, 0);
    float y2 = paintcanvas_ext_dl_signedtofloat(y1i, 0);

    float *m = (float *)abuf;
    memset(m, 0, 60);
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[14] = 1.0f;

    float *v = this->lineVerts;

    if (*g_dl_flag_794ee == 0) {
        paintcanvas_ext_dl_glLineWidth(1.0f);
        paintcanvas_ext_dl_glEnable(this->engine, true);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_setwvm(this, abuf);
        paintcanvas_ext_dl_glVertexPointer(2, 0x1406, 0, this->lineVerts);
        paintcanvas_ext_dl_glColorMask(this->engine, 0x8074, 1);
        paintcanvas_ext_dl_glColorMask(this->engine, 0x8078, 0);
        paintcanvas_ext_dl_glColorMask(this->engine, 0x8075, 0);
        paintcanvas_ext_dl_glColorMask(this->engine, 0x8076, 0);
        paintcanvas_ext_dl_glDrawArrays(1, 0, 2);
        paintcanvas_ext_dl_glEnable(this->engine, true);
    } else {
        paintcanvas_ext_dl_setwvm(this, abuf);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_drawline2d(this->engine, this->lineVerts, true);
    }
}

void PaintCanvas::MeshChangeMaterial(unsigned int meshIndex, unsigned short matIndex)
{
    if (matIndex < this->materialCount &&
        meshIndex < this->meshCount) {
        void *mesh = (this->meshes)[meshIndex];
        void *mat = (this->materials)[matIndex];
        return paintcanvas_ext_change_mat(this, mesh, mat);
    }
}

void PaintCanvas::CameraSetPerspective(unsigned int index, float a, float b, float c)
{
    if (index < this->cameraCount) {
        float w = (float)paintcanvas_ext_get_w(this);
        float h = (float)paintcanvas_ext_get_h(this);
        void *cam = (this->cameras)[index];
        paintcanvas_ext_cam_persp4(a, b, c, w, h, cam);
        if (this->currentCamera == index) {
            return paintcanvas_ext_cam_setcur(this, index);
        }
    }
}

// AbyssEngine::PaintCanvas::End3d() — ends the current 3D pass (counterpart to Begin3d).
// Ghidra mis-decoded this tiny tail-call; it simply forwards to the internal end-3D helper.

void PaintCanvas::End3d()
{
    return paintcanvas_ext_end3d(this);
}

void PaintCanvas::MeshCloneMaterial(unsigned int index, unsigned int &out)
{
    int result;
    if (index < this->meshCount) {
        char *obj = (char *)paintcanvas_ext_alloc(0x74);
        char *mesh = (this->meshes)[index];
        paintcanvas_ext_material_clone(obj, *(void **)(mesh + 0x30));
        paintcanvas_ext_material_add(obj, &this->materialCount);
        result = (int)this->materialCount - 1;
    } else {
        result = -1;
    }
    out = result;
}

void PaintCanvas::GetGravValue()
{
    return paintcanvas_ext_get_grav(this->engine);
}

#define HIDDEN __attribute__((visibility("hidden")))

extern char paintcanvas_g_bg_a;
extern char paintcanvas_g_bg_b;

void PaintCanvas::BeginBG()
{
    // field_0xfd is a uint16_t flag; the original store is byte-width, preserve that.
    *(unsigned char *)&((Engine *)this->engine)->field_0xfd = 0;
    paintcanvas_ext_gl_disable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_disable(0xbe2);
    paintcanvas_ext_gl_color(this->engine, 1.0f, 1.0f, 1.0f, 1.0f);
    this->bgFlagSaved = paintcanvas_g_bg_a;
    char flag = paintcanvas_g_bg_b;
    paintcanvas_g_bg_a = 0;
    if (flag != 0) {
        return paintcanvas_ext_matgl_load(this->engine, &this->projMatrix3d.m[0]);
    }
    paintcanvas_ext_glMatrixMode(0x1702);
    paintcanvas_ext_gl_loadidentity();
    paintcanvas_ext_gl_ortho_persp(1.0f, -1.0f, 1.0f);
    paintcanvas_ext_glMatrixMode(0x1701);
    paintcanvas_ext_gl_loadmatrix(&this->projMatrix3d.m[0]);
    paintcanvas_ext_glMatrixMode(0x1700);
    return paintcanvas_ext_gl_done();
}

namespace AbyssEngine {
}

void PaintCanvas::FontCreate(unsigned short resId, unsigned int &out,
                bool /*unused*/)
{
    char *res = (char *)paintcanvas_ext_fc_findres(this, resId);
    if (res == 0) {
        return;
    }
    unsigned short *info = *(unsigned short **)(res + 0xc);
    char *texres = (char *)paintcanvas_ext_fc_findres(this, *info);
    if (texres == 0) {
        return;
    }
    if (*(int *)(texres + 8) == -1) {
        paintcanvas_ext_fc_texcreate(this, *info, true);
    }
    if (*(int *)(res + 8) != -1) {
        out = *(unsigned int *)(res + 8);
        return;
    }
    void *font = 0;
    char *texpath = (char *)**(char ***)(texres + 0xc);
    int ok = paintcanvas_ext_fc_fontfromfile(this->engine, texpath, info[1], &font);
    if (ok != 1) {
        return;
    }
    if (*(int *)(texres + 8) != -1) {
        *(int *)font = *(int *)(texres + 8);
    }
    PCArrayAdd<AbyssEngine::ImageFont *>((AbyssEngine::ImageFont *)font, &this->fontCount);
    int idx = this->fontCount - 1;
    *(int *)(res + 8) = idx;
    out = idx;

    Engine *eng = (Engine *)this->engine;
    int cur = eng->field_0x78;
    if (cur == -1) {
        eng->field_0x78 = idx;
    } else {
        char *curFont = (char *)(this->fonts)[cur];
        if (*(unsigned short *)curFont <= *(unsigned short *)font) {
            int curH = paintcanvas_ext_fc_fontheight(curFont);
            int newH = paintcanvas_ext_fc_fontheight(font);
            if (newH < curH) {
                eng->field_0x78 = out;
            }
        }
    }
}

void PaintCanvas::SetResourceList(AbyssEngine::Resource *const *list, unsigned int count)
{
    return paintcanvas_ext_set_reslist(list, count, &this->resourceCount);
}

void PaintCanvas::MaterialResourceChangeTexture(unsigned int resId,
                                   unsigned int texture, int slot)
{
    if ((unsigned int)slot < 8) {
        char *r = paintcanvas_ext_find_res(this, resId);
        if (r) {
            unsigned int matIdx = *(unsigned int *)(r + 0x8);
            if (matIdx + 1 != 0 && matIdx < this->materialCount) {
                char *mat = (char *)(this->materials)[matIdx];
                *(unsigned int *)(mat + slot * 4) = texture;
            }
        }
    }
}

void PaintCanvas::TransformAddChild(unsigned int parent, unsigned int child)
{
    unsigned int count = this->transformCount;
    if (parent != child && child < count && parent < count) {
        char **arr = this->transforms;
        char *p = arr[parent];
        char *c = arr[child];
        ArrayAdd< ::Transform *>((::Transform *)c, *reinterpret_cast<Array< ::Transform *> *>(p + 0x4c));
        char **arr2 = this->transforms;
        char *p2 = arr2[parent];
        char *c2 = arr2[child];
        paintcanvas_ext_child_link(p2 + 0xd4, c2, p2);
        char **arr3 = this->transforms;
        return paintcanvas_ext_transform_dirty(arr3[parent]);
    }
}

// AbyssEngine::PaintCanvas::TextureCreateGlobal(String name, unsigned unit) — loads a texture
// file into the given texture unit and, on success, leaves it unbound on that unit.
extern "C" int tcg_TextureCreateFromFile(void *engine, const char *path, void *cb, void *ud,
                                         unsigned *outId, bool b, float f);

__attribute__((visibility("hidden"))) extern int **g_tcg_canary;

void PaintCanvas::TextureCreateGlobal(AbyssEngine::String name, unsigned int unit)
{
    int *canary = *g_tcg_canary;
    int saved = *canary;

    char *path = name.GetAEChar();
    unsigned outId;
    int rc = tcg_TextureCreateFromFile(this->engine, path, 0, 0, &outId, false,
                                       0.0f);
    if (rc == 1) {
        tcg_glActiveTexture(unit + 0x84c0);
        tcg_glBindTexture(0xde1, 0);
        tcg_glActiveTexture(0x84c0);
    }
    ::operator delete[](path);

    
}

#define HIDDEN __attribute__((visibility("hidden")))

extern char paintcanvas_g_use_matgl;

void PaintCanvas::Begin3d()
{
    // field_0xfd is a uint16_t flag; the original store is byte-width, preserve that.
    *(unsigned char *)&((Engine *)this->engine)->field_0xfd = 0;
    paintcanvas_ext_gl_enable(0xb71);
    paintcanvas_ext_gl_depthmask(1);
    paintcanvas_ext_gl_disable(0xbe2);
    paintcanvas_ext_gl_color(this->engine, 1.0f, 1.0f, 1.0f, 1.0f);
    if (paintcanvas_g_use_matgl != 0) {
        return paintcanvas_ext_matgl_load(this->engine, &this->projMatrix3d.m[0]);
    }
    paintcanvas_ext_glMatrixMode(0x1702);
    paintcanvas_ext_gl_loadidentity();
    paintcanvas_ext_gl_ortho_persp(1.0f, -1.0f, 1.0f);
    paintcanvas_ext_glMatrixMode(0x1701);
    paintcanvas_ext_gl_loadmatrix(&this->projMatrix3d.m[0]);
    paintcanvas_ext_glMatrixMode(0x1700);
    return paintcanvas_ext_gl_done();
}

// AbyssEngine::PaintCanvas::TransformAddMesh(unsigned transformIndex, unsigned short meshId, bool b)
// Adds a mesh (by global mesh id) to a transform. Ghidra mis-decoded this tiny wrapper; modeled
// on TransformAddMeshId — it creates/resolves the mesh then forwards to the internal add helper.
extern "C" void paintcanvas_ext_transform_addmesh(AbyssEngine::PaintCanvas *, void *,
                                                  unsigned short, bool);

void PaintCanvas::TransformAddMesh(unsigned int transformIndex,
                      unsigned short meshId, bool b)
{
    if (transformIndex < this->transformCount) {
        void *t = (this->transforms)[transformIndex];
        return paintcanvas_ext_transform_addmesh(this, t, meshId, b);
    }
}

// AbyssEngine::PaintCanvas::GetReverseString(String in, bool reverse)
// When `reverse` is false, returns a plain copy of `in`; otherwise returns `in` with its
// characters in reverse order (built one SubString per character).
using AbyssEngine::String;

AbyssEngine::String PaintCanvas::GetReverseString(AbyssEngine::String in, bool reverse)
{
    if (!reverse) {
        return in;
    }
    String out("");
    for (int i = (int)in.size() - 1; i >= 0; --i) {
        out += in.SubString((unsigned)i, (unsigned)(i + 1));
    }
    return out;
}

namespace AbyssEngine {
}

void PaintCanvas::SpriteSystemCreate(unsigned short resId, bool flag,
                        unsigned short matResId, unsigned int &out)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_ss2_sscreate(this->engine, resId, flag, &ss);
    if (ok == 1) {
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_ss2_matcreate(this, matResId, &mat);
        if (mat <= this->materialCount) {
            AbyssEngine::Node *node = *(AbyssEngine::Node **)((char *)ss + 0x10);
            node->field_0x30 =
                *(unsigned int *)((char *)this->materials + mat * 4);
        }
        unsigned int i;
        for (i = 0; i < this->spriteSystemCount; i++) {
            void **slot = (void **)(this->spriteSystems + i * 4);
            if (*(int *)slot == 0) {
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
                &this->spriteSystemCount));
        result = this->spriteSystemCount - 1;
    } else {
        result = 0xffffffff;
    }
    out = result;
}

void PaintCanvas::GetScreenPosition(const AbyssEngine::AEMath::Matrix &srcMatrix,
                                    AbyssEngine::AEMath::Vector &outVec)
{
    if (this->currentCamera >= this->cameraCount) {
        return;
    }

    // copy {m[0xc], m[0x1c], m[0x2c]} into the output vector
    float src[3];
    src[0] = srcMatrix.m[3];
    src[1] = srcMatrix.m[7];
    src[2] = srcMatrix.m[11];
    paintcanvas_ext_gsp_vec_assign(&outVec, src);

    char *cam = ((char **)this->cameras)[this->currentCamera];
    float z = outVec[2];
    float denomX = *(float *)(cam + 0x4c) * z;
    if (denomX == 0.0f) {
        return;
    }
    float denomY = z * *(float *)(cam + 0x48);
    if (denomY == 0.0f) {
        return;
    }

    float px = outVec[0];
    int w0 = paintcanvas_ext_gsp_getwidth(this);
    int w1 = paintcanvas_ext_gsp_getwidth(this);
    double fw = (double)paintcanvas_ext_gsp_signedtofloat(w0, 0);
    double termY = ((double)outVec[1] * 0.5) / (double)denomY;
    double halfW = (double)paintcanvas_ext_gsp_signedtofloat(w1 >> 1, 0);
    outVec[0] = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp_getheight(this);
    double fh = (double)paintcanvas_ext_gsp_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp_getheight(this);
    double halfH = (double)paintcanvas_ext_gsp_signedtofloat(h1 >> 1, 0);
    outVec[1] = (float)(halfH + termY * fh);

    // remaining clamp/visibility checks have no observable side-effects on output
    // (they compute booleans only); reproduce the visible reads to keep behavior.
    char *cam2 = ((char **)this->cameras)[this->currentCamera];
    if (outVec[2] <= *(float *)(cam2 + 4)) {
        float fy = outVec[1];
        if (fy >= 0.0f) {
            float fx = outVec[0];
            if (fx >= 0.0f) {
                int ww = paintcanvas_ext_gsp_getwidth(this);
                float fww = paintcanvas_ext_gsp_signedtofloat(ww, 0);
                if (fx < fww) {
                    int hh = paintcanvas_ext_gsp_getheight(this);
                    paintcanvas_ext_gsp_signedtofloat(hh, 0);
                }
            }
        }
    }
}

namespace AbyssEngine {
}

void PaintCanvas::SpriteSystemCreate(unsigned short resId,
                        bool flag, unsigned int &out)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_sscreate(this->engine, resId, flag, &ss);
    if (ok == 1) {
        unsigned int i;
        for (i = 0; i < this->spriteSystemCount; i++) {
            void **slot = (void **)(this->spriteSystems + i * 4);
            if (*(int *)slot == 0) {
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
                &this->spriteSystemCount));
        result = this->spriteSystemCount - 1;
    } else {
        result = 0xffffffff;
    }
    out = result;
}

void PaintCanvas::MaterialCreate(unsigned int &out, AbyssEngine::BlendMode mode,
                                 unsigned int textures, unsigned short p4)
{
    char *obj = (char *)paintcanvas_ext_alloc(0x74);
    paintcanvas_ext_material_ctor(obj);
    (void)p4;
    *(unsigned int *)(obj + 0x0) = textures;
    *(unsigned int *)(obj + 0x20) = mode;
    paintcanvas_ext_material_add(obj, &this->materialCount);
    out = this->materialCount - 1;
}

__attribute__((visibility("hidden"))) extern const double g_cisvf_gravscale_7bcd8;

int PaintCanvas::CameraIsSphereinViewFrustum(const AbyssEngine::AEMath::Vector &point, float radius)
{
    if (radius == 0.0f ||
        this->cameraCount <= this->currentCamera) {
        return 1;
    }

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[14] = 1.0f;
    char scratch[60];
    paintcanvas_ext_cisvf_matidentity(scratch, m);

    void *grav = paintcanvas_ext_cisvf_getgrav(this->engine);
    double angle = *(double *)((char *)grav + 8) * g_cisvf_gravscale_7bcd8;
    float a = (float)angle;
    int orient = this->gameOrientation;
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cisvf_sinf(rot);
    float c = paintcanvas_ext_cisvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = ((void **)this->cameras)[this->currentCamera];
    return paintcanvas_ext_cisvf_inner(point, radius, m, cam);
}

void PaintCanvas::MeshChangeShaderAnimValue(AbyssEngine::Mesh *mesh, float value, unsigned int mode)
{
    if (!mesh) {
        return;
    }
    if (mode == 4) {
        mesh->shaderAnimValue1 = value;
    } else if (mode == 2) {
        mesh->shaderAnimValue0 = value;
    } else if (mode == 1) {
        memcpy(&mesh->materialId, &value, sizeof(value));
    }
    return paintcanvas_ext_shader_anim(this, mesh->animation);
}

__attribute__((visibility("hidden"))) extern int *const g_resume_curtex_7e828;

extern "C" int paintcanvas_ext_rs_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);

void PaintCanvas::Resume()
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < this->cubeTextureCount; i++) {
        char *res = (this->cubeTextures)[i];
        char *path = paintcanvas_ext_rs_getAEChar(res + 4);
        float f = *(float *)((this->cubeTextures)[i] + 0x10);
        int ok = paintcanvas_ext_rs_texfromfile(this->engine, path, 0, 0,
                                                &out, false, f);
        if (ok == 1) {
            *(int *)((this->cubeTextures)[i]) = 0;
        }
        paintcanvas_ext_rs_deletearr(path);
    }
    int *cur = g_resume_curtex_7e828;
    if (*cur != 0) {
        paintcanvas_ext_rs_glActiveTexture(0x84c7);
        char *res = (this->cubeTextures)[*cur];
        paintcanvas_ext_rs_glBindTexture(0x8513, *(unsigned int *)res);
        paintcanvas_ext_rs_glActiveTexture(0x84c0);
    }
}

namespace AbyssEngine {
}

void PaintCanvas::TransformAddMeshId(unsigned int transformIndex, unsigned int meshIndex)
{
    if (transformIndex >= this->transformCount ||
        meshIndex >= this->meshCount) {
        return;
    }
    char *tf = (this->transforms)[transformIndex];
    char *mesh = (this->meshes)[meshIndex];
    ArrayAdd<AbyssEngine::Mesh *>(*(AbyssEngine::Mesh **)(mesh),
                                  *reinterpret_cast<::Array<AbyssEngine::Mesh *> *>(tf + 0x3c));
    paintcanvas_ext_tami_bsphere_merge(tf + 0xd4, mesh + 0x3c);

    void *res = *(void **)(mesh + 0x34);
    if (res != 0) {
        long long resLen = *(long long *)((char *)res + 0xf8);
        long long tfLen = *(long long *)(tf + 0xf8);
        if (tfLen < resLen) {
            paintcanvas_ext_tami_setanimlen(tf, (int)(resLen >> 32), (int)resLen);
            tf = (this->transforms)[transformIndex];
        }
        long long tfStart = *(long long *)(tf + 0x100);
        void *res2 = *(void **)(mesh + 0x34);
        long long resStart = *(long long *)((char *)res2 + 0x100);
        if (tfStart == 0 || resStart < tfStart) {
            *(long long *)(tf + 0x100) = resStart;
        }
        paintcanvas_ext_tami_setanimstate(tf, 2, 0);
    }
    paintcanvas_ext_tami_finalize((this->transforms)[transformIndex]);
}

// AbyssEngine::PaintCanvas::GetReverseString(String in) — convenience overload that forwards to
// the (String,bool) overload, choosing the reverse flag from the canvas's layout-direction byte at
// +0x1c (reverse when that byte is zero, i.e. right-to-left).
AbyssEngine::String PaintCanvas::GetReverseString(AbyssEngine::String in)
{
    return this->GetReverseString(in, this->field_0x1c == 0);
}

void PaintCanvas::GetAccelValue()
{
    return paintcanvas_ext_get_accel(this->engine);
}

__attribute__((visibility("hidden"))) extern float *const g_rpm_fov_8d0dc;
__attribute__((visibility("hidden"))) extern const unsigned int g_rpm_const_8d234;
__attribute__((visibility("hidden"))) extern float *const g_rpm_near_8d196;
__attribute__((visibility("hidden"))) extern float *const g_rpm_far_8d1a4;
__attribute__((visibility("hidden"))) extern char *const g_rpm_flag_8d1b8;

void PaintCanvas::ResetPersMatrix()
{
    int w = paintcanvas_ext_rpm_dispwidth(this->engine);
    int h = paintcanvas_ext_rpm_dispheight(this->engine);
    float fov = *g_rpm_fov_8d0dc;
    float s = paintcanvas_ext_rpm_sinf(fov * 0.5f);
    float c = paintcanvas_ext_rpm_cosf(fov * 0.5f);
    float fw = paintcanvas_ext_rpm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_rpm_signedtofloat(h, 0);

    memset(&this->projMatrix3d.m[12], 0, 0x10);
    memset(&this->projMatrix3d.m[0], 0, 0x10);
    memset(&this->projMatrix3d.m[8], 0, 0x10);
    memset(&this->projMatrix3d.m[4], 0, 0x10);

    if (this->gameOrientation <= 3) {
        float aspect = fw / fh;
        float f = 1.0f / (s / c);
        switch (this->gameOrientation) {
        case 0:
            this->projMatrix3d.m[4] = -f;
            this->projMatrix3d.m[1] = f / aspect;
            break;
        case 1:
            this->projMatrix3d.m[4] = f;
            this->projMatrix3d.m[1] = -(f / aspect);
            break;
        case 2:
            this->projMatrix3d.m[5] = f;
            this->projMatrix3d.m[0] = f / aspect;
            break;
        case 3:
            this->projMatrix3d.m[5] = -f;
            this->projMatrix3d.m[0] = -(f / aspect);
            break;
        }
    }

    float n = *g_rpm_near_8d196;
    float fr = *g_rpm_far_8d1a4;
    this->projMatrix3d.m[11] = g_rpm_const_8d234;
    this->projMatrix3d.m[10] = (n + fr) / (fr - n);
    this->projMatrix3d.m[14] = ((n + n) * fr) / (fr - n);

    if (*g_rpm_flag_8d1b8 == 0) {
        paintcanvas_ext_rpm_glMatrixMode(0x1702);
        paintcanvas_ext_rpm_glLoadIdentity();
        float sc;
        *(unsigned int *)&sc = g_rpm_const_8d234;
        paintcanvas_ext_rpm_glScalef(1.0f, sc, 1.0f);
        paintcanvas_ext_rpm_glMatrixMode(0x1701);
        paintcanvas_ext_rpm_glLoadMatrixf(&this->projMatrix3d.m[0]);
        paintcanvas_ext_rpm_glMatrixMode(0x1700);
        paintcanvas_ext_rpm_glFinish();
    } else {
        paintcanvas_ext_rpm_loadproj(this->engine, &this->projMatrix3d.m[0]);
    }
}

__attribute__((visibility("hidden"))) extern const double g_cipvf_gravscale_7bba8;

int PaintCanvas::CameraIsPointinViewFrustum(const AbyssEngine::AEMath::Vector &point)
{
    if (this->currentCamera >= this->cameraCount) {
        return 1;
    }

    // identity matrix scratch
    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[14] = 1.0f;
    char scratch[60];
    paintcanvas_ext_cipvf_matidentity(scratch, m);

    void *grav = paintcanvas_ext_cipvf_getgrav(this->engine);
    double angle = *(double *)((char *)grav + 8) * g_cipvf_gravscale_7bba8;
    float a = (float)angle;
    int orient = this->gameOrientation;
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cipvf_sinf(rot);
    float c = paintcanvas_ext_cipvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = ((void **)this->cameras)[this->currentCamera];
    return paintcanvas_ext_cipvf_inner(point, m, cam);
}

void PaintCanvas::SetTexture(unsigned int, unsigned int)
{
    return paintcanvas_ext_set_texture(this->engine);
}

PaintCanvas::~PaintCanvas()
{
    paintcanvas_ext_dtor_releaseall(this);

    // Walk the resource list (count at 0x134, array at 0x138), releasing each.
    for (unsigned int i = 0; i < this->resourceCount; i++) {
        void *res = (this->resources)[i];
        if (res != 0) {
            void *payload = *(void **)((char *)res + 0xc);
            int type = *(int *)((char *)res + 4);
            switch (type) {
            case 1:
            case 3:
            case 6:
                paintcanvas_ext_dtor_op_delete(payload);
                break;
            case 2:
                if (payload) {
                    paintcanvas_ext_dtor_op_delete(paintcanvas_ext_dtor_restex_dtor(payload));
                }
                break;
            case 4:
                if (payload) {
                    paintcanvas_ext_dtor_op_delete(paintcanvas_ext_dtor_resmesh_dtor(payload));
                }
                break;
            case 5:
                if (payload) {
                    paintcanvas_ext_dtor_op_delete(paintcanvas_ext_dtor_restransform_dtor(payload));
                }
                break;
            default:
                break;
            }
            void *cell = (this->resources)[i];
            if (cell != 0) {
                paintcanvas_ext_dtor_op_delete(cell);
            }
            (this->resources)[i] = 0;

            // glow scratch arrays released per-iteration in the original
            PCArrayReleaseClasses(&this->glowMeshes_count);
            PCArrayDtor(&this->glowMatA_count);
            PCArrayDtor(&this->glowMatB_count);
            PCArrayReleaseClasses(&this->glowUints_count);
            PCArrayDtor(&this->glowMatC_count);
        }
    }

    paintcanvas_ext_dtor_meshrelease(this->engine, &this->quad2dMesh);
    paintcanvas_ext_dtor_meshrelease(this->engine, &this->lineMesh);

    // Release the loaded-texture name strings (count at 0x10, array at 0x14).
    for (unsigned int i = 0; i < this->cubeTextureCount; i++) {
        void *tex = (this->cubeTextures)[i];
        if (tex != 0) {
            paintcanvas_ext_dtor_str_dtor((char *)tex + 4);
            paintcanvas_ext_dtor_op_delete(tex);
        }
        (this->cubeTextures)[i] = 0;
    }

    PCArrayDtor(&this->glowMatC_count);
    PCArrayDtor(&this->glowUints_count);
    PCArrayDtor(&this->glowMatB_count);
    PCArrayDtor(&this->glowMatA_count);
    PCArrayDtor(&this->glowMeshes_count);
    PCArrayDtor(&this->spriteSystemCount);
    PCArrayDtor(&this->materialCount);
    PCArrayDtor(&this->cameraCount);
    PCArrayDtor(&this->transformCount);
    PCArrayDtor(&this->imageCount);
    PCArrayDtor(&this->fontCount);
    PCArrayDtor(&this->resourceCount);
    PCArrayDtor(&this->meshCount);
    PCArrayDtor(&this->cubeTextureCount);
}

namespace AbyssEngine {
}

void PaintCanvas::Image2DCreate(unsigned short resId, unsigned int &out)
{
    char *res = (char *)paintcanvas_ext_i2d_findres(this, resId);
    if (res == 0) {
        return;
    }
    unsigned short *info = *(unsigned short **)(res + 0xc);
    char *texres = (char *)paintcanvas_ext_i2d_findres(this, *info);
    if (texres == 0) {
        return;
    }
    if (*(int *)(texres + 8) == -1) {
        paintcanvas_ext_i2d_texcreate(this, *info, true);
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *img = (char *)operator new(0x18);
        memset(img, 0, 0x18);
        char *texpath = (char *)**(char ***)(texres + 0xc);
        int ok = paintcanvas_ext_i2d_imgregion(this->engine, texpath,
                                               info[1], img);
        if (ok != 1) {
            return;
        }
        if (*(int *)(texres + 8) != -1) {
            *(int *)(img + 4) = *(int *)(texres + 8);
        }
        PCArrayAdd<AbyssEngine::Image2D *>((AbyssEngine::Image2D *)img, &this->imageCount);
        idx = this->imageCount - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    out = idx;
}

__attribute__((visibility("hidden"))) extern const double g_gsp2_gravscale_8bfa8;

void PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Matrix &m,
                                    const AbyssEngine::AEMath::Vector &worldPos,
                                    AbyssEngine::AEMath::Vector &outVec)
{
    (void)m;

    // first transform the input world position
    char transformed[16];
    paintcanvas_ext_gsp2_transformvec(transformed, &worldPos);

    if (this->currentCamera >= this->cameraCount) {
        return;
    }

    char invMat[60];
    void *cam = ((void **)this->cameras)[this->currentCamera];
    if (this->initialized == 0) {
        paintcanvas_ext_gsp2_invtransformvec(invMat, (char *)cam + 0xc);
        paintcanvas_ext_gsp2_vec_assign(&outVec, invMat);
    } else {
        float m[16];
        char scratch[60];
        memset(m, 0, sizeof(m));
        m[0] = 1.0f; m[5] = 1.0f; m[14] = 1.0f;
        paintcanvas_ext_gsp2_matidentity(scratch, m);

        void *grav = paintcanvas_ext_gsp2_getgrav(this->engine);
        double angle = *(double *)((char *)grav + 8) * g_gsp2_gravscale_8bfa8;
        float a = (float)angle;
        int orient = this->gameOrientation;
        float rot = (orient == 1) ? a : -a;
        float s = paintcanvas_ext_gsp2_sinf(rot);
        float c = paintcanvas_ext_gsp2_cosf(rot);
        m[0] = c;
        m[5] = c;
        *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
        m[4] = s;

        paintcanvas_ext_gsp2_memcpy(scratch, (char *)cam + 0xc, 0x3c);
        paintcanvas_ext_gsp2_mtx_muleq(scratch, m);
        paintcanvas_ext_gsp2_invtransformvec(invMat, scratch);
        paintcanvas_ext_gsp2_vec_assign(&outVec, invMat);
    }

    float z = outVec[2];
    char *cam2 = ((char **)this->cameras)[this->currentCamera];
    if (z > *(float *)(cam2 + 4)) {
        return;
    }
    float denomX = z * *(float *)(cam2 + 0x4c);
    if (denomX == 0.0f) {
        return;
    }
    float denomY = z * *(float *)(cam2 + 0x48);
    if (denomY == 0.0f) {
        return;
    }

    float px = outVec[0];
    int w0 = paintcanvas_ext_gsp2_getwidth(this);
    int w1 = paintcanvas_ext_gsp2_getwidth(this);
    float py = outVec[1];
    double fw = (double)paintcanvas_ext_gsp2_signedtofloat(w0, 0);
    double halfW = (double)paintcanvas_ext_gsp2_signedtofloat(w1 >> 1, 0);
    outVec[0] = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp2_getheight(this);
    double fh = (double)paintcanvas_ext_gsp2_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp2_getheight(this);
    double halfH = (double)paintcanvas_ext_gsp2_signedtofloat(h1 >> 1, 0);
    outVec[1] = (float)(halfH + (((double)py * 0.5) / (double)denomY) * fh);

    // remaining bound checks compute booleans only (no observable output)
    float nx = outVec[0];
    if (nx >= 0.0f) {
        float ny = outVec[1];
        if (ny >= 0.0f) {
            int ww = paintcanvas_ext_gsp2_getwidth(this);
            float fww = paintcanvas_ext_gsp2_signedtofloat(ww, 0);
            if (nx < fww) {
                int hh = paintcanvas_ext_gsp2_getheight(this);
                paintcanvas_ext_gsp2_signedtofloat(hh, 0);
            }
        }
    }
}

int PaintCanvas::ResourceLoaded(unsigned int index, AbyssEngine::ResourceType type)
{
    unsigned int count;
    switch (type) {
    case 1: {
        char *res = (this->resources)[index];
        if (*(int *)(res + 0x4) == 2) {
            int handle = *(int *)(res + 0x8);
            return handle + 1 != 0 ? 1 : 0;
        }
        return 0;
    }
    case 2:
        count = this->fontCount;
        break;
    case 3:
        count = this->imageCount;
        break;
    case 4:
        count = this->meshCount;
        break;
    case 5:
        count = this->transformCount;
        break;
    case 6:
        count = this->materialCount;
        break;
    default:
        return 0;
    }
    return index < count ? 1 : 0;
}

int PaintCanvas::TransformGetTriCount(unsigned int index)
{
    if (index < this->transformCount) {
        void *t = (this->transforms)[index];
        return paintcanvas_ext_transform_tricount(this, t);
    }
    return 0;
}

float PaintCanvas::SpriteSystemSetPosition(unsigned int index, unsigned short sub,
                              float x, float y, float z)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if (sub < *(unsigned short *)s) {
                float *p = (float *)(*(char **)(s + 0x4) + sub * 12);
                p[0] = x;
                p[1] = y;
                p[2] = z;
                return z;
            }
        }
    }
    return x;
}

void PaintCanvas::DrawImage2D(unsigned int index, int x, int y)
{
    char abuf[60];
    if (index < this->imageCount) {
        char *img = (this->images)[index];
        if (*(unsigned char *)(img + 0x14)) {
            paintcanvas_ext_di_restore(*(unsigned char *)(img + 0x14), img);
            img = (this->images)[index];
        }
        paintcanvas_ext_di_settexture(this, *(unsigned int *)(img + 0x4), -1);

        float fx = (float)x;
        float fy = (float)y;
        float *m = (float *)abuf;
        m[0] = 1.0f;
        m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
        m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
        m[14] = 1.0f;
        m[3] = fx;
        m[7] = fy;

        paintcanvas_ext_di_setwvm(this, abuf);
        paintcanvas_ext_di_gldisable(0xb44);
        paintcanvas_ext_di_meshdraw(this->engine,
                                    *(void **)((this->images)[index]));
        paintcanvas_ext_di_glenable(0xb44);
    }
}

// The Matrix member at +0xf8 is a single Matrix object, not an Array (kept as an extern ctor).
extern "C" void paintcanvas_ctor_matrix(void *);     // 0x6f118  f8
extern "C" void paintcanvas_ext_meshcreate5(void *, unsigned short, unsigned short,
                                            signed char, void *);

PaintCanvas::PaintCanvas(AbyssEngine::Engine *engine)
{
    PCArrayCtor(&this->cubeTextureCount);   // loaded-texture-name list
    PCArrayCtor(&this->meshCount);   // meshes
    paintcanvas_ctor_matrix(&this->identityMatrix);
    PCArrayCtor(&this->resourceCount);  // resources
    PCArrayCtor(&this->fontCount);  // fonts
    PCArrayCtor(&this->imageCount);  // images
    PCArrayCtor(&this->transformCount);  // transforms
    PCArrayCtor(&this->cameraCount);  // cameras
    PCArrayCtor(&this->materialCount);  // materials
    PCArrayCtor(&this->spriteSystemCount);  // sprite systems
    PCArrayCtor(&this->glowMeshes_count);  // glow meshes
    PCArrayCtor(&this->glowMatA_count);  // glow matrices
    PCArrayCtor(&this->glowMatB_count);  // glow matrices
    PCArrayCtor(&this->glowUints_count);  // glow uints
    PCArrayCtor(&this->glowMatC_count);  // glow matrices

    this->fogMode = 0;
    this->mask2dImage = 0;
    this->fogEnableFlag = 1;
    this->culledCount = 0;
    this->initialized = 0;
    this->engine = engine;
    this->currentCamera = 0xffffffff;

    paintcanvas_ext_meshcreate5(engine, 4, 2, 0x11, &this->lineMesh);

    int *p = *(int **)(this->lineMesh + 0x2c);
    p[0] = 0x20000;
    p[1] = 1;
    p[2] = *(int *)0x87878;
    this->colorR = 1.0f;
    this->colorG = 1.0f;
    this->colorB = 1.0f;
    this->colorA = 1.0f;
    this->field_0x1c = 1;
    engine->field_0xfc = 1;

    paintcanvas_ext_meshcreate5(engine, 400, 200, 0x1b, &this->quad2dMesh);

    short *buf = *(short **)(this->quad2dMesh + 0x2c);
    int j = 0;
    for (int i = 0; i != 0x4b0; i += 0xc) {
        short *e = buf + i / 2;
        short a = (short)(j << 2);
        e[0] = a;
        e[1] = (short)((j << 2) | 2);
        e[2] = (short)((j << 2) | 1);
        e[3] = a;
        e[4] = (short)((j << 2) | 3);
        e[5] = (short)((j << 2) | 2);
        j++;
    }
    this->field_0x1f8 = 1;
    this->field_0x1cc = 0;
}

void PaintCanvas::TransformRemoveMesh(unsigned int transformIndex, unsigned short meshResId)
{
    if (this->transformCount <= transformIndex) {
        return;
    }
    char *x = paintcanvas_ext_find_mesh(this, meshResId);
    if (x) {
        return paintcanvas_ext_remove_mesh(this, transformIndex, *(int *)(x + 0x8));
    }
}

void PaintCanvas::ClearBuffer(unsigned int mask)
{
    paintcanvas_ext_enable(0xb71);
    paintcanvas_ext_depthmask(1);
    return paintcanvas_ext_clear2(this->engine, mask);
}

// Clear only the depth buffer (GL_DEPTH_BUFFER_BIT == 0x100).
void PaintCanvas::ClearDepth()
{
    ClearBuffer(0x100);
}

// Renders the captured frame-buffer texture as a fullscreen blit. The Android
// build ships an empty stub for this entry point (no observable side effects).
void PaintCanvas::DrawFrameBufferTexture(int, int, int, int, int, int)
{
}

// Pre-warm the texture cache. Always reports success on this platform.
bool PaintCanvas::WarmUpTexture()
{
    return true;
}

void PaintCanvas::TransformRemoveChild(unsigned int parent, unsigned int child)
{
    unsigned int count = this->transformCount;
    if (parent != child && child < count && parent < count) {
        char **arr = this->transforms;
        char *p = arr[parent];
        char *c = arr[child];
        paintcanvas_ext_array_remove(c, p + 0x4c);
        char **arr2 = this->transforms;
        return paintcanvas_ext_transform_dirty(arr2[parent]);
    }
}

extern "C" int paintcanvas_ext_tc_texfromfile(void *eng, char *path,
                                              void (*cb)(AbyssEngine::Image *, void *), void *ud,
                                              unsigned int *out, bool b, float f);
extern "C" int paintcanvas_ext_tc_texfromfileintern(void *eng, char *path,
                                                    void (*cb)(AbyssEngine::Image *, void *), void *ud,
                                                    unsigned int *out, float f, void *lt, bool b);

void PaintCanvas::TextureCreate(unsigned short resId, void (*callback)(AbyssEngine::Image *, void *),
                   void *userData, unsigned int &out, bool useCallbackLoader)
{
    Engine *eng = (Engine *)this->engine;
    eng->boundTextures[0] = -1;
    eng->boundTextures[1] = -1;

    char *res = (char *)paintcanvas_ext_tc_findres(this, resId);
    if (res != 0) {
        unsigned int idx = *(unsigned int *)(res + 8);
        if (idx == 0xffffffff) {
            void **info = *(void ***)(res + 0xc);
            float f = (float)(int)(*(unsigned int *)((char *)info + 4));
            char *path = (char *)*info;
            int ok;
            if (!useCallbackLoader) {
                ok = paintcanvas_ext_tc_texfromfileintern(this->engine, path,
                                                          callback, userData, &idx, f, 0, false);
            } else {
                ok = paintcanvas_ext_tc_texfromfile(this->engine, path,
                                                    callback, userData, &idx, true, f);
            }
            if (ok != 1) {
                return;
            }
            idx = 0;
            *(unsigned int *)(res + 8) = 0;
        }
        out = idx;
    }
}

// AbyssEngine::PaintCanvas::TextureCreate(unsigned short id, unsigned int* out, bool flag)
// Convenience overload (binary 0x6fbb0): forwards to the full 5-arg loader with the
// optional upload-callback / user-data pointers left null.
void PaintCanvas::TextureCreate(unsigned short id, unsigned int &out, bool flag)
{
    TextureCreate(id, nullptr, (void *)0, out, flag);
}

// AbyssEngine::PaintCanvas::SwapBuffer() — no-op on the canvas (binary 0x8caae is empty);
// the actual buffer present is driven by AbyssEngine::Engine::SwapBuffer.
void PaintCanvas::SwapBuffer()
{
}

void PaintCanvas::StartDraw2FBO()
{
    return paintcanvas_ext_start_fbo(this->engine);
}

void PaintCanvas::Suspend()
{
    char texId[4];
    for (unsigned int i = 0; i < this->cubeTextureCount; i++) {
        int *p = (int *)(this->cubeTextures)[i];
        *(int *)texId = *p;
        if (*p != -1) {
            paintcanvas_ext_gl_deletetextures(1, texId);
            p = (int *)(this->cubeTextures)[i];
        }
        *p = -1;
    }
}

extern "C" void paintcanvas_ext_dt_drawmesh(void *self, void *mesh, void *m, void *m2,
                                            unsigned int flag, void *m3);

void PaintCanvas::DrawTransform(::Transform *tf, const AbyssEngine::AEMath::Matrix &m2,
                               AbyssEngine::AEMath::Matrix &m3)
{
    char buf[60];
    if (tf && tf->field_0xec) {
        paintcanvas_ext_mtx_mul(buf, &m2, tf);
        if (tf->field_0x11c != 0) {
            paintcanvas_ext_mtx_muleq(buf, tf->field_0x5c);
        }
        for (unsigned int i = 0; i < tf->field_0x3c; i++) {
            paintcanvas_ext_dt_drawmesh(this, tf->field_0x40[i], buf, &m3, tf->field_0x48, tf->field_0x98);
        }
        for (unsigned int i = 0; i < tf->field_0x4c; i++) {
            if (this->currentCamera < this->cameraCount &&
                paintcanvas_ext_dt_incamvf(tf->field_0x50[i], buf,
                    (this->cameras)[this->currentCamera])) {
                paintcanvas_ext_dt_drawtransform_rec(this, tf->field_0x50[i], buf, &m3);
            } else {
                this->culledCount += 1;
            }
        }
    }
}

void PaintCanvas::DrawImage2D(unsigned int index, int x, int y,
                 int w, int h, unsigned char alignFlags, unsigned char anchorFlags,
                 unsigned char flipFlags)
{
    if (index >= this->imageCount) {
        return;
    }
    char *img = ((char **)this->images)[index];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di4_restore(*(unsigned char *)(img + 0x14), img);
        img = ((char **)this->images)[index];
    }
    paintcanvas_ext_di4_settexture(this, *(unsigned int *)(img + 4));

    // horizontal base placement (alignFlags low bits)
    int baseX = x;
    int hOff;
    if ((alignFlags & 7) == 4) {
        hOff = paintcanvas_ext_di4_getwidth(this) >> 1;
    } else if ((alignFlags & 7) == 2) {
        baseX = -x;
        hOff = paintcanvas_ext_di4_getwidth(this);
    } else {
        hOff = 0;
    }

    // horizontal flip-span (alignFlags bit 3)
    int spanW = w;
    if (alignFlags & 8) {
        spanW = paintcanvas_ext_di4_getwidth(this) - (w + x);
    }
    float fSpanW = paintcanvas_ext_di4_signedtofloat(spanW, 0);
    char *region = *(char **)(*(char **)(((char **)this->images)[index]) + 4);
    float regW = *(float *)(region + 0xc);

    // vertical flip-span (alignFlags bit 7)
    int spanH = h;
    if (alignFlags & 0x80) {
        spanH = paintcanvas_ext_di4_getheight(this) - (h + y);
        region = *(char **)(*(char **)(((char **)this->images)[index]) + 4);
    }
    float fSpanH = paintcanvas_ext_di4_signedtofloat(spanH, 0);
    float regH = *(float *)(region + 0x1c);

    // vertical base placement (alignFlags high nibble)
    int baseY = y;
    int vOff;
    if ((alignFlags & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di4_getheight(this) >> 1;
    } else if ((alignFlags & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di4_getheight(this);
        baseY = -y;
    } else {
        vOff = 0;
    }

    // anchor offset X (anchorFlags low bits)
    int anchorX;
    if ((anchorFlags & 7) == 4) {
        if ((alignFlags & 8) == 0) {
            anchorX = -(w >> 1);
        } else {
            anchorX = ((w + x) - paintcanvas_ext_di4_getwidth(this)) >> 1;
        }
    } else if ((anchorFlags & 7) == 2) {
        if ((alignFlags & 8) == 0) {
            anchorX = -w;
        } else {
            anchorX = (w + x) - paintcanvas_ext_di4_getwidth(this);
        }
    } else {
        anchorX = 0;
    }

    float scaleX = fSpanW / regW;
    float scaleY = fSpanH / regH;

    // anchor offset Y (anchorFlags high nibble)
    int anchorY;
    if ((anchorFlags & 0x70) == 0x40) {
        if ((alignFlags & 0x80) == 0) {
            anchorY = -(h >> 1);
        } else {
            anchorY = ((h + y) - paintcanvas_ext_di4_getheight(this)) >> 1;
        }
    } else if ((anchorFlags & 0x70) == 0x20) {
        if ((alignFlags & 0x80) == 0) {
            anchorY = -h;
        } else {
            anchorY = (h + y) - paintcanvas_ext_di4_getheight(this);
        }
    } else {
        anchorY = 0;
    }

    float m[16];
    memset(m, 0, sizeof(m));
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[3] = paintcanvas_ext_di4_signedtofloat(baseX + hOff + anchorX, 0);
    m[7] = paintcanvas_ext_di4_signedtofloat(anchorY + vOff + baseY, 0);
    m[0] = scaleX;

    if (flipFlags & 1) {
        m[0] = -scaleX;
        m[3] = paintcanvas_ext_di4_signedtofloat(w, 0) + m[3];
    }
    m[5] = scaleY;
    if (flipFlags & 2) {
        m[5] = -scaleY;
        m[7] = paintcanvas_ext_di4_signedtofloat(h, 0) + m[7];
    }

    paintcanvas_ext_di4_setwvm(this, m);
    paintcanvas_ext_di4_gldisable(0xb44);
    paintcanvas_ext_di4_meshdraw(this->engine,
                                 *(void **)(((char **)this->images)[index]));
    paintcanvas_ext_di4_glenable(0xb44);
}

void PaintCanvas::CameraSetPerspective(unsigned int index, float fov, float aspect)
{
    if (index < this->cameraCount) {
        float w = (float)paintcanvas_ext_get_w(this);
        float h = (float)paintcanvas_ext_get_h(this);
        void *cam = (this->cameras)[index];
        paintcanvas_ext_cam_persp(fov, aspect, w, h, cam);
        if (this->currentCamera == index) {
            return paintcanvas_ext_cam_setcur(this, index);
        }
    }
}

void PaintCanvas::DrawRegion2D(unsigned int index, float /*unused*/, int rotSteps,
                  int pivotX, int pivotY, int transX, float scaleX, float scaleY)
{
    if (index >= this->imageCount) {
        return;
    }
    char *img = ((char **)this->images)[index];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_dr2_restore(*(unsigned char *)(img + 0x14), img);
        img = ((char **)this->images)[index];
    }
    paintcanvas_ext_dr2_settexture(this, *(unsigned int *)(img + 4));

    float rot = paintcanvas_ext_dr2_signedtofloat(rotSteps, 0);
    float px = paintcanvas_ext_dr2_signedtofloat(pivotX, 0);
    float py = paintcanvas_ext_dr2_signedtofloat(pivotY, 0);
    float tx = paintcanvas_ext_dr2_signedtofloat(transX, 0);
    float negpx = paintcanvas_ext_dr2_signedtofloat(-pivotX, 0);
    float negpy = paintcanvas_ext_dr2_signedtofloat(-pivotY, 0);

    // translation matrix (identity + translation in column 3)
    float transM[16];
    memset(transM, 0, sizeof(transM));
    transM[0] = 1.0f; transM[5] = 1.0f; transM[10] = 1.0f; transM[15] = 1.0f;
    transM[3] = py + rot * scaleX;
    transM[7] = tx + px * scaleY;

    // pivot matrix
    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[10] = 1.0f; pivotM[15] = 1.0f;
    pivotM[3] = negpx * scaleX;
    pivotM[7] = negpy * scaleY;

    // rotation matrix
    float s = paintcanvas_ext_dr2_sinf(negpx * scaleX);
    float c = paintcanvas_ext_dr2_cosf(negpx * scaleX);
    float rotM[16];
    memset(rotM, 0, sizeof(rotM));
    rotM[0] = c;
    rotM[5] = c;
    rotM[10] = 1.0f;
    rotM[15] = 1.0f;
    *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
    rotM[4] = s;

    float scaleBuf[16];
    paintcanvas_ext_dr2_setscaling(scaleBuf, scaleX, scaleY, 1.0f);

    // build the composed matrix
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
                                 *(void **)(((char **)this->images)[index]));
    paintcanvas_ext_dr2_glenable(0xb44);
}

void PaintCanvas::MeshSetTriangle(unsigned int meshIndex, unsigned short tri,
                     unsigned short v0, unsigned short v1, unsigned short v2)
{
    if (meshIndex < this->meshCount) {
        char *mesh = (this->meshes)[meshIndex];
        unsigned int t3 = tri * 3;
        if (t3 >= *(unsigned short *)(mesh + 0x28)) {
            return;
        }
        unsigned short *buf = *(unsigned short **)(mesh + 0x2c);
        buf[t3] = v0;
        buf[t3 + 1] = v1;
        buf[t3 + 2] = v2;
    }
}

__attribute__((visibility("hidden"))) extern const unsigned int g_sgo_const_8e6b4;
__attribute__((visibility("hidden"))) extern const unsigned int g_sgo_const_8e6b8;

static void zero16(void *p)
{
    memset(p, 0, 0x10);
}

void PaintCanvas::SetGameOrientation(AbyssEngine::LandscapeMode orientation)
{
    if (this->gameOrientation == orientation) {
        return;
    }
    this->gameOrientation = orientation;
    paintcanvas_ext_sgo_setorientation(this->engine, orientation);

    this->projMatrix3d.m[1] = -this->projMatrix3d.m[1];
    this->projMatrix3d.m[4] = -this->projMatrix3d.m[4];

    int w = paintcanvas_ext_sgo_dispwidth(this->engine);
    float fw = paintcanvas_ext_sgo_signedtofloat(w, 0);
    int h = paintcanvas_ext_sgo_dispheight(this->engine);
    float fh = paintcanvas_ext_sgo_signedtofloat(h, 0);

    if (orientation == 3) {
        zero16(&this->projOrthoMatrix.m[9]); zero16(&this->worldViewMatrix.m[8]); zero16(&this->worldViewMatrix.m[4]);
        zero16(&this->projOrthoMatrix.m[5]); zero16(&this->projOrthoMatrix.m[1]); zero16(&this->worldViewMatrix.m[11]); zero16(&this->worldViewMatrix.m[0]);
        this->projOrthoMatrix.m[14] = 0;
        this->projOrthoMatrix.m[13] = 1.0f;
        this->worldViewMatrix.m[13] = 0;
        this->worldViewMatrix.m[14] = 0;
        this->projOrthoMatrix.m[15] = 1.0f;
        this->worldViewMatrix.m[0] = g_sgo_const_8e6b8;
        this->worldViewMatrix.m[1] = g_sgo_const_8e6b8;
        this->projOrthoMatrix.m[10] = g_sgo_const_8e6b4;
        this->projOrthoMatrix.m[12] = g_sgo_const_8e6b8;
        this->worldViewMatrix.m[10] = 1.0f;
        this->worldViewMatrix.m[4] = 1.0f;
        this->projOrthoMatrix.m[0] = 2.0f / fw;
        this->projOrthoMatrix.m[5] = -(2.0f / fh);
        this->worldViewMatrix.m[15] = 1.0f;
        int w2 = paintcanvas_ext_sgo_dispwidth(this->engine);
        this->worldViewMatrix.m[12] = paintcanvas_ext_sgo_signedtofloat(w2, 0);
        int h2 = paintcanvas_ext_sgo_dispheight(this->engine);
        this->worldViewMatrix.m[13] = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (orientation == 1) {
        zero16(&this->projOrthoMatrix.m[9]); zero16(&this->worldViewMatrix.m[8]); zero16(&this->worldViewMatrix.m[4]);
        zero16(&this->projOrthoMatrix.m[5]); zero16(&this->projOrthoMatrix.m[1]); zero16(&this->worldViewMatrix.m[11]); zero16(&this->worldViewMatrix.m[1]);
        this->projOrthoMatrix.m[14] = 0;
        this->projOrthoMatrix.m[13] = 1.0f;
        this->projOrthoMatrix.m[15] = 1.0f;
        this->worldViewMatrix.m[1] = 1.0f;
        this->projOrthoMatrix.m[10] = g_sgo_const_8e6b4;
        this->projOrthoMatrix.m[12] = g_sgo_const_8e6b8;
        this->worldViewMatrix.m[10] = 1.0f;
        this->worldViewMatrix.m[4] = g_sgo_const_8e6b8;
        this->projOrthoMatrix.m[0] = 2.0f / fh;
        this->projOrthoMatrix.m[5] = -(2.0f / fw);
        this->worldViewMatrix.m[15] = 1.0f;
        int h2 = paintcanvas_ext_sgo_dispheight(this->engine);
        this->worldViewMatrix.m[12] = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (orientation != 0) {
        zero16(&this->projOrthoMatrix.m[9]); zero16(&this->worldViewMatrix.m[8]); zero16(&this->worldViewMatrix.m[4]);
        zero16(&this->projOrthoMatrix.m[5]); zero16(&this->projOrthoMatrix.m[1]); zero16(&this->worldViewMatrix.m[1]);
        this->projOrthoMatrix.m[14] = 0;
        this->projOrthoMatrix.m[13] = 1.0f;
        this->worldViewMatrix.m[13] = 0;
        this->worldViewMatrix.m[14] = 0;
        this->projOrthoMatrix.m[15] = 1.0f;
        this->worldViewMatrix.m[0] = 1.0f;
        this->projOrthoMatrix.m[10] = g_sgo_const_8e6b4;
        this->projOrthoMatrix.m[12] = g_sgo_const_8e6b8;
        this->worldViewMatrix.m[10] = 1.0f;
        this->worldViewMatrix.m[5] = 1.0f;
        this->projOrthoMatrix.m[0] = 2.0f / fw;
        this->projOrthoMatrix.m[5] = -(2.0f / fh);
        this->worldViewMatrix.m[15] = 1.0f;
    } else {
        zero16(&this->projOrthoMatrix.m[9]); zero16(&this->worldViewMatrix.m[8]); zero16(&this->worldViewMatrix.m[4]);
        zero16(&this->projOrthoMatrix.m[5]); zero16(&this->projOrthoMatrix.m[1]); zero16(&this->worldViewMatrix.m[11]); zero16(&this->worldViewMatrix.m[1]);
        this->projOrthoMatrix.m[14] = 0;
        this->projOrthoMatrix.m[13] = 1.0f;
        this->projOrthoMatrix.m[15] = 1.0f;
        this->worldViewMatrix.m[1] = g_sgo_const_8e6b8;
        this->projOrthoMatrix.m[10] = g_sgo_const_8e6b4;
        this->projOrthoMatrix.m[12] = g_sgo_const_8e6b8;
        this->worldViewMatrix.m[10] = 1.0f;
        this->worldViewMatrix.m[4] = 1.0f;
        this->projOrthoMatrix.m[0] = 2.0f / fh;
        this->projOrthoMatrix.m[5] = -(2.0f / fw);
        this->worldViewMatrix.m[15] = 1.0f;
        int w2 = paintcanvas_ext_sgo_dispwidth(this->engine);
        this->worldViewMatrix.m[13] = paintcanvas_ext_sgo_signedtofloat(w2, 0);
    }

    if (this->currentCamera == -1) {
        return;
    }
    float *cam = ((float **)this->cameras)[this->currentCamera];
    paintcanvas_ext_sgo_setpersp(this, cam[0], cam[1], cam[2]);
}

extern "C" int paintcanvas_ext_mc_meshcreate(void *eng, unsigned short a, unsigned short b,
                                             signed char c, void **out);

namespace AbyssEngine {
}

void PaintCanvas::MeshCreate(unsigned short vertexCount, unsigned short triangleCount,
                signed char meshType, unsigned short matResId, unsigned int &out)
{
    int result = -1;
    unsigned int mat = 0xffffffff;
    void *mesh = 0;
    paintcanvas_ext_mc_matcreate(this, matResId, &mat);
    int ok = paintcanvas_ext_mc_meshcreate(this->engine, vertexCount, triangleCount,
                                           meshType, &mesh);
    if (ok == 1) {
        if (0xfffffffe < this->materialCount) {
            // material pointer back-reference: write last element ptr into mesh+0x30
            void *m = *(void **)(this->materials - 4);
            if (mesh) {
                ((AbyssEngine::Mesh *)mesh)->field_0x30 = m;
            }
        }
        ArrayAdd<AbyssEngine::Mesh *>((AbyssEngine::Mesh *)mesh,
                                      *reinterpret_cast<::Array<AbyssEngine::Mesh *> *>(&this->meshCount));
        result = this->meshCount - 1;
    }
    out = (unsigned int)result;
}

__attribute__((visibility("hidden"))) extern const float g_dm_255_8ee80;

// multiply two 8-bit-per-channel packed colors component-wise (>>8)
static unsigned int mulColors(unsigned int a, unsigned int b)
{
    unsigned int r = (((a >> 16) & 0xff) * ((b >> 16) & 0xff)) & 0xffffff00;
    unsigned int g = (((a >> 8) & 0xff) * ((b >> 8) & 0xff)) & 0xffffff00;
    unsigned int bl = ((a & 0xff) * (b & 0xff)) >> 8;
    unsigned int al = (((a >> 24) & 0xff) * ((b >> 24) & 0xff)) & 0xffffff00;
    return (bl) | (g << 8) | (r << 8) | (al << 16);
}

void PaintCanvas::DrawMesh(AbyssEngine::Mesh *mesh, AbyssEngine::AEMath::Matrix &worldMatrix,
              AbyssEngine::AEMath::Matrix &viewMatrix, unsigned int color,
              AbyssEngine::AEMath::Matrix &uvMatrix)
{

    AbyssEngine::AEMath::Matrix worldM;
    AbyssEngine::AEMath::Matrix uvM;
    paintcanvas_ext_dm_memcpy(&worldM, worldMatrix, 0x3c);
    paintcanvas_ext_dm_memcpy(&uvM, uvMatrix, 0x3c);

    unsigned int meshColor = color;

    // If the mesh has an attached resource (animation transform), apply its local transform.
    if (mesh->animation != 0) {
        ::Transform *resTf = mesh->animation;
        float a[16], b[16];
        memset(a, 0, sizeof(a));
        a[0] = 1.0f; a[5] = 1.0f; a[14] = 1.0f;
        memset(b, 0, sizeof(b));
        b[0] = 1.0f; b[5] = 1.0f; b[14] = 1.0f;

        float pivotM[16];
        paintcanvas_ext_dm_settrans(pivotM, mesh->pivotY);
        paintcanvas_ext_dm_settrans(pivotM, -mesh->pivotY);
        memset(pivotM, 0, sizeof(pivotM));
        pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[14] = 1.0f;

        float posM[16];
        float resLocal[16];
        paintcanvas_ext_dm_getpos(posM);
        paintcanvas_ext_dm_settrans_vec(resLocal, pivotM);
        paintcanvas_ext_dm_memcpy(resLocal, resTf->field_0x5c, 0x3c);

        float s1[16], s2[16], s3[16];
        paintcanvas_ext_dm_mtx_mul(s1, worldMatrix, pivotM);
        paintcanvas_ext_dm_mtx_mul(s2, s1, b);
        paintcanvas_ext_dm_mtx_mul(s3, s2, resLocal);
        paintcanvas_ext_dm_mtx_mul(posM, s3, a);
        paintcanvas_ext_dm_mtx_assign(&worldM, posM);

        unsigned int resColor = resTf->field_0x48;
        paintcanvas_ext_dm_mtx_assign(&uvM, resTf->field_0x98);
        meshColor = mulColors(resColor, color);
    }

    paintcanvas_ext_dm_transformvec(&uvM, &worldM);

    float ext[3];
    ext[0] = mesh->boundsRadius;
    ext[1] = ext[0];
    ext[2] = ext[0];
    paintcanvas_ext_dm_rotatevec(&uvM, &worldM);
    paintcanvas_ext_dm_vec_assign(ext, &uvM);

    // radius from the larger absolute component
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
                paintcanvas_ext_dm_setcolor(
                    this->engine,
                    (this->colorR * fr) / g_dm_255_8ee80,
                    (this->colorG * fg) / g_dm_255_8ee80,
                    (this->colorB * fb) / g_dm_255_8ee80,
                    (this->colorA * fa) / g_dm_255_8ee80);

                paintcanvas_ext_dm_mtx_muleq(&worldM, &viewMatrix);
                paintcanvas_ext_dm_setwvm(this, &worldM);
                paintcanvas_ext_dm_setmodelmatrix(this->engine);
                paintcanvas_ext_dm_setuvmatrix(this->engine, &uvM);
                paintcanvas_ext_dm_meshdraw(this->engine, mesh);
                paintcanvas_ext_dm_resetuvmatrix(this->engine);
            } else {
                // batch into the glow target at resource+0x44/0x2c/0x38/0x5c/0x50
                char *res = (char *)mesh->material;
                paintcanvas_ext_dm_addcached_mesh(mesh, res + 0x44);
                paintcanvas_ext_dm_pushmat(worldM, res + 0x2c);
                paintcanvas_ext_dm_pushmat(uvM, res + 0x38);
                paintcanvas_ext_dm_pushmat(viewMatrix, res + 0x5c);
                paintcanvas_ext_dm_addcached_uint(meshColor, res + 0x50);
            }
        }
    }

    // recurse over child meshes (count 0x3c / array 0x40) and child transforms (0x4c / 0x50)
    if (mesh->animation != 0) {
        ::Transform *res = mesh->animation;
        unsigned int n = res->field_0x3c;
        while ((int)(--n) >= 0) {
            this->DrawMesh(res->field_0x40[n], worldM, viewMatrix, color, uvM);
        }
        for (unsigned int i = 0; i < res->field_0x4c; i++) {
            if (this->currentCamera < this->cameraCount) {
                void *cam = ((void **)this->cameras)[this->currentCamera];
                ::Transform *tf = res->field_0x50[i];
                if (paintcanvas_ext_dm_incamvf(tf, &worldMatrix, cam)) {
                    this->DrawTransform(res->field_0x50[i], worldM, viewMatrix);
                }
            }
        }
    }
}

#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_b2d_flag HIDDEN;

void PaintCanvas::Begin2d()
{
    // field_0xfd is a uint16_t flag; the original store is byte-width, preserve that.
    *(unsigned char *)&((Engine *)this->engine)->field_0xfd = 1;
    paintcanvas_ext_gl_disable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_blendfunc(0x302, 0x303);
    paintcanvas_ext_setcolor(this->engine, 1.0f, 1.0f, 1.0f, 1.0f);
    paintcanvas_ext_glenable2(this->engine, 0xde1, true);
    if (*paintcanvas_g_b2d_flag == 0) {
        paintcanvas_ext_gl_texenvi(0x2300, 0x2200, 0x2100);
        paintcanvas_ext_glMatrixMode(0x1702);
        paintcanvas_ext_gl_loadidentity();
        paintcanvas_ext_gl_scalef(1.0f, 1.0f, 1.0f);
        paintcanvas_ext_glMatrixMode(0x1701);
        paintcanvas_ext_gl_loadmatrix(&this->projOrthoMatrix.m[0]);
        if (this->gameOrientation != 2) {
            paintcanvas_ext_gl_multmatrix(&this->worldViewMatrix.m[0]);
        }
        paintcanvas_ext_glMatrixMode(0x1700);
        paintcanvas_ext_gl_loadidentity();
    } else {
        paintcanvas_ext_setortho(this->engine, &this->projOrthoMatrix.m[0],
                                 &this->worldViewMatrix.m[0], this->gameOrientation != 2);
    }
    this->field_0xc = 0;
}

extern "C" void paintcanvas_ext_drawstring_raw(void *, const unsigned short *, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void PaintCanvas::DrawString(unsigned int index, const unsigned short *str,
                int x, int y, bool b)
{
    if (index < this->fontCount) {
        char *font = (char *)(this->fonts)[index];
        paintcanvas_ext_string_prep(this, *(void **)(font + 0x8), -1);
        char *font2 = (char *)(this->fonts)[index];
        paintcanvas_ext_drawstring_raw(font2, str, x, y, this,
                                       this->engine, b);
    }
}

__attribute__((visibility("hidden"))) extern const char g_dsc_pipe_882c4[];
__attribute__((visibility("hidden"))) extern const char g_dsc_fmt_882ee[];

extern "C" void paintcanvas_ext_dsc_fontdraw(void *font, unsigned short *txt, unsigned int len,
                                             int x, int y, void *self, void *eng, bool b);

void PaintCanvas::DrawStringColor(unsigned int index, const AbyssEngine::String &text,
                     int x, int y, bool b)
{
    if (index >= this->fontCount) {
        return;
    }
    void *font0 = ((char **)this->fonts)[index];
    paintcanvas_ext_dsc_settexture(this, *(unsigned int *)((char *)font0 + 8));
    paintcanvas_ext_dsc_getcolor(this);

    char str[16];
    char sep[16];
    paintcanvas_ext_dsc_str_copy(str, &text, false);
    paintcanvas_ext_dsc_str_fromchar(sep, g_dsc_pipe_882c4, false);
    char *parts = (char *)paintcanvas_ext_dsc_splittags(str, sep);
    paintcanvas_ext_dsc_str_dtor(sep);

    if (parts != 0) {
        bool draw = true;
        for (unsigned int i = 0; i < *(unsigned int *)parts; i++) {
            char **data = *(char ***)(parts + 4);
            char *part = data[i];
            if (draw) {
                void *font = ((char **)this->fonts)[index];
                unsigned short *txt = paintcanvas_ext_dsc_str_cast(part);
                paintcanvas_ext_dsc_fontdraw(font, txt, *(unsigned int *)(part + 8), x,
                                             y, this, this->engine, b);
                x += paintcanvas_ext_dsc_textwidth(this, index, part);
            } else if (*(int *)(part + 8) == 0) {
                paintcanvas_ext_dsc_setcolor(this);
            } else {
                int color = 0;
                char *s = paintcanvas_ext_dsc_getAEChar(part);
                paintcanvas_ext_dsc_sscanf(s, g_dsc_fmt_882ee, &color);
                paintcanvas_ext_dsc_setcolor(this);
            }
            draw = !draw;
        }
        paintcanvas_ext_dsc_setcolor(this);
        paintcanvas_ext_dsc_releaseclasses(parts);
        void *p = paintcanvas_ext_dsc_arr_dtor(parts);
        paintcanvas_ext_dsc_op_delete(p);
    }
    paintcanvas_ext_dsc_str_dtor(str);
}

namespace AbyssEngine {
}

// Adds a matrix (15 floats) plus the target Array pointer onto a glow-matrix list.

void PaintCanvas::SetMatForGlow(AbyssEngine::Material *glowSource)
{
    // Mirror each entry of the material's per-mesh glow state onto this canvas'
    // glow lists: the mesh handles (meshes @0x44/0x48), the three parallel matrix
    // lists (arr_2c @0x30, arr_38 @0x3c, arr_5c @0x60) and the uint list
    // (arr_50 @0x54). The matrices convert to const float* via Matrix::operator.
    for (unsigned int i = 0; i < glowSource->meshes->size_; i++) {
        ArrayAdd<AbyssEngine::Mesh *>(glowSource->meshes->data_[i],
                                      *reinterpret_cast<::Array<AbyssEngine::Mesh *> *>(&this->glowMeshes_count));

        paintcanvas_ext_smfg_pushmat(glowSource->arr_2c->data_[i], &this->glowMatA_count);
        paintcanvas_ext_smfg_pushmat(glowSource->arr_38->data_[i], &this->glowMatB_count);

        PCArrayAdd<unsigned int>(glowSource->arr_50->data_[i], &this->glowUints_count);

        paintcanvas_ext_smfg_pushmat(glowSource->arr_5c->data_[i], &this->glowMatC_count);
    }
}

void PaintCanvas::EnableClip(int x, int y, int w, int h)
{
    paintcanvas_ext_ec_glEnable(0xc11);
    int sx, sy, sw, sh;
    switch (this->gameOrientation) {
    case 0:
        sx = x;
        sy = y;
        sw = w;
        sh = h;
        break;
    case 1: {
        int dispH = paintcanvas_ext_ec_getHeight(this);
        int dispW = paintcanvas_ext_ec_getWidth(this);
        sx = dispH - (y + h);
        sy = dispW - (x + w);
        sw = h;
        sh = w;
        break;
    }
    case 2: {
        int dispH = paintcanvas_ext_ec_getHeight(this);
        sx = dispH - (y + h);
        sy = x;
        sw = w;
        sh = h;
        break;
    }
    case 3: {
        int dispW = paintcanvas_ext_ec_getWidth(this);
        sx = dispW - (x + w);
        sy = y;
        sw = w;
        sh = h;
        break;
    }
    default:
        sx = x;
        sy = y;
        sw = w;
        sh = h;
        break;
    }
    paintcanvas_ext_ec_glScissor(sx, sy, sw, sh);
}

void PaintCanvas::DrawImage2D(unsigned int index, int x, int y,
                 unsigned char regionAlignFlags, unsigned char placeFlags)
{
    if (index >= this->imageCount) {
        return;
    }
    char *img = ((char **)this->images)[index];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di3_restore(*(unsigned char *)(img + 0x14), img);
    }

    // Horizontal placement offset (placeFlags low bits) + vertical (placeFlags high nibble).
    int hOff;
    if ((placeFlags & 7) == 4) {
        hOff = paintcanvas_ext_di3_getwidth(this) >> 1;
    } else if ((placeFlags & 7) == 2) {
        hOff = paintcanvas_ext_di3_getwidth(this);
        y = -y;
    } else {
        hOff = 0;
    }

    int vOff;
    unsigned int yShift = regionAlignFlags;
    if ((placeFlags & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di3_getheight(this) >> 1;
    } else if ((placeFlags & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di3_getheight(this);
        yShift = (unsigned int)(-(int)yShift);
    } else {
        vOff = 0;
    }

    // Region-based offsets from the image's source region (regionAlignFlags = second align flag).
    int rx;
    char *region = *(char **)(img + 4);
    if ((regionAlignFlags & 7) == 4) {
        double w = (double)paintcanvas_ext_di3_signedtofloat((int)*(float *)(region + 0xc), 0);
        rx = (int)(long long)(w * -0.5);
    } else if ((regionAlignFlags & 7) == 2) {
        rx = -(int)*(float *)(region + 0xc);
    } else {
        rx = 0;
    }

    int ry;
    char *region2 = *(char **)(((char **)this->images)[index] + 4);
    if ((regionAlignFlags & 0x70) == 0x20) {
        ry = -(int)*(float *)(region2 + 0x1c);
    } else if ((regionAlignFlags & 0x70) == 0x40) {
        double h = (double)paintcanvas_ext_di3_signedtofloat((int)*(float *)(region2 + 0x1c), 0);
        ry = (int)(long long)(h * -0.5);
    } else {
        ry = 0;
    }

    char *img2 = ((char **)this->images)[index];
    paintcanvas_ext_di3_settexture(this, *(unsigned int *)(img2 + 4));

    float fx = paintcanvas_ext_di3_signedtofloat(hOff + y + rx, 0);
    float fy = paintcanvas_ext_di3_signedtofloat(vOff + (int)yShift + ry, 0);

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[3] = fx;
    m[7] = fy;

    paintcanvas_ext_di3_setwvm(this, m);
    paintcanvas_ext_di3_meshdraw(this->engine,
                                 *(void **)(((char **)this->images)[index]));
}

void PaintCanvas::MeshCreate(unsigned short a, unsigned short b,
                signed char c, unsigned int &out)
{
    char mesh[4];
    *(void **)mesh = 0;
    int result = paintcanvas_ext_meshcreate(this->engine, mesh);
    if (result == 1) {
        ArrayAdd<AbyssEngine::Mesh *>(*(AbyssEngine::Mesh **)mesh,
                                      *reinterpret_cast<::Array<AbyssEngine::Mesh *> *>(&this->meshCount));
        result = (int)this->meshCount - 1;
    } else {
        result = -1;
    }
    out = result;
}

int PaintCanvas::FontGetSpacing(unsigned int index)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        return paintcanvas_ext_font_get_spacing(font);
    }
    return 0;
}

// AbyssEngine::PaintCanvas::TransformGetTransform(unsigned index)
// Returns the internal Transform pointer for a transform handle. (Ghidra mis-decoded this tiny
// accessor; modeled on its sibling TransformGetLocal/TransformGetTriCount which index the
// transform array at +0x158 count / +0x15c data.)
void *PaintCanvas::TransformGetTransform(unsigned int index)
{
    if (index < this->transformCount) {
        return (this->transforms)[index];
    }
    return 0;
}

__attribute__((visibility("hidden"))) extern char *const g_meshcreate_vboflag_79d5c;

namespace AbyssEngine {
}

void PaintCanvas::MeshCreate(unsigned short resId, unsigned int &out,
                bool forceClone)
{
    char *res = (char *)paintcanvas_ext_mc2_findres(this, resId);
    if (res == 0) {
        return;
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *info = *(char **)(res + 0xc);
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_mc2_matcreate(this, *(unsigned short *)(info + 4), &mat);
        void *matptr = 0;
        if (0xfffffffe < this->materialCount) {
            matptr = *(void **)(this->materials - 4);
        }
        void *mesh = 0;
        int ok = paintcanvas_ext_mc2_meshfromfile(this->engine,
                                                  *(char **)info, &mesh, matptr);
        if (ok != 1) {
            return;
        }
        if (*g_meshcreate_vboflag_79d5c != 0) {
            if (mesh) {
                ((AbyssEngine::Mesh *)mesh)->field_0x84 = 1;
            }
            paintcanvas_ext_mc2_converttovbo(mesh);
        }
        ArrayAdd<AbyssEngine::Mesh *>((AbyssEngine::Mesh *)mesh,
                                      *reinterpret_cast<::Array<AbyssEngine::Mesh *> *>(&this->meshCount));
        idx = this->meshCount - 1;
        *(unsigned int *)(res + 8) = idx;
    } else {
        char **meshes = this->meshes;
        char *existing = meshes[idx];
        if (*(int *)(existing + 0x34) != 0 || forceClone) {
            void *clone = paintcanvas_ext_mc2_new_mesh_copy(
                ((void **)meshes)[*(unsigned int *)(res + 8)]);
            ArrayAdd<AbyssEngine::Mesh *>((AbyssEngine::Mesh *)clone,
                                          *reinterpret_cast<::Array<AbyssEngine::Mesh *> *>(&this->meshCount));
            idx = this->meshCount - 1;
        }
    }
    out = idx;
}

__attribute__((visibility("hidden"))) extern char *const g_fsp_flag_8cf40;
__attribute__((visibility("hidden"))) extern const double g_fsp_255d_8d070;
__attribute__((visibility("hidden"))) extern const float g_fsp_255f_8d078;

void PaintCanvas::FogSetParameter(AbyssEngine::FogMode mode, float fogStart, float fogEnd,
                     float fogDensity, unsigned int color)
{
    float col[4];
    if (*g_fsp_flag_8cf40 == 0) {
        float modeF = paintcanvas_ext_fsp_unsignedtofloat((unsigned int)mode, 0);
        paintcanvas_ext_fsp_glFogf(0xb65, modeF);
        paintcanvas_ext_fsp_glFogf(0xb62, fogDensity);
        paintcanvas_ext_fsp_glFogf(0xb63, fogStart);
        paintcanvas_ext_fsp_glFogf(0xb64, fogEnd);

        float r = paintcanvas_ext_fsp_unsignedtofloat((color >> 16) & 0xff, 0);
        float g = paintcanvas_ext_fsp_unsignedtofloat((color >> 8) & 0xff, 0);
        paintcanvas_ext_fsp_unsignedtofloat(color >> 24, 0);
        float b = paintcanvas_ext_fsp_unsignedtofloat(color & 0xff, 0);
        col[0] = r / g_fsp_255f_8d078;
        col[1] = g / g_fsp_255f_8d078;
        col[2] = b / g_fsp_255f_8d078;
        col[3] = fogStart;
        paintcanvas_ext_fsp_glFogfv(0xb66, col);
    } else {
        double r = (double)paintcanvas_ext_fsp_unsignedtofloat((color >> 16) & 0xff, 0);
        r = r / g_fsp_255d_8d070;
        paintcanvas_ext_fsp_unsignedtofloat(color >> 24, 0);
        double g = (double)paintcanvas_ext_fsp_unsignedtofloat((color >> 8) & 0xff, 0);
        Engine *eng = (Engine *)this->engine;
        g = g / g_fsp_255d_8d070;
        eng->fogMinDist = fogStart;
        eng->fogMaxDist = fogEnd;
        col[0] = (float)r;
        col[1] = (float)g;
        paintcanvas_ext_fsp_vec_assign(&eng->fogColor, col);
    }
}

__attribute__((visibility("hidden"))) extern const unsigned int g_dr3_const_88808;

void PaintCanvas::DrawRegion2D(unsigned int index, int srcX, int srcY,
                  int destW, int destH, float /*unused*/, int transY, int pivotX, int pivotY,
                  int transX)
{
    if (index >= this->imageCount) {
        return;
    }
    char *img = ((char **)this->images)[index];
    paintcanvas_ext_dr3_settexture(this, *(unsigned int *)(img + 4));
    char *mesh = ((char **)this->images)[index];
    char *meshObj = *(char **)mesh;

    *(unsigned char *)(mesh + 0x14) = 1;

    // vertex positions (8 floats) at mesh[0]+4 -> +0xc
    float fw = paintcanvas_ext_dr3_signedtofloat(destW, 0);
    float fh = paintcanvas_ext_dr3_signedtofloat(destH, 0);
    float *verts = (float *)(*(char **)(meshObj + 4));
    verts[0] = 0.0f; verts[1] = 0.0f; verts[2] = 0.0f;
    verts[3] = fw;       // +0xc
    verts[4] = 0.0f;     // +0x10
    verts[5] = 0.0f;
    verts[6] = fw;
    verts[7] = fh;       // +0x1c
    verts[8] = 0.0f;
    verts[9] = 0.0f;
    verts[10] = fh;      // +0x28

    // UV computations from the source region metadata stored in mesh fields 0x8..0xe
    float u0 = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 0xa), 0);
    float w  = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 8), 0);
    float h  = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 0xc), 0);
    float v0 = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 0xe), 0);
    float a2 = paintcanvas_ext_dr3_signedtofloat(srcX, 0);
    float a3 = paintcanvas_ext_dr3_signedtofloat(srcY, 0);
    float a2s = paintcanvas_ext_dr3_signedtofloat((short)srcX, 0);

    float *uv = (float *)(*(char **)(meshObj + 8));
    float invW = 1.0f / w;
    float invH = 1.0f / u0;
    float ulo = (a2 + h) * invW;
    float vlo = (a3 + v0) * invH;
    float uhi = (fw + h + a2s) * invW;
    float vhi = (fh + v0 + a3) * invH;
    uv[0] = ulo; uv[1] = vlo;
    uv[2] = uhi; uv[3] = vlo;
    uv[4] = uhi; uv[5] = vhi;
    uv[6] = ulo; uv[7] = vhi;

    // blend-mode field at meshObj+0x2c
    unsigned int *blend = *(unsigned int **)(meshObj + 0x2c);
    blend[0] = 0x20000;
    blend[1] = 1;
    blend[2] = g_dr3_const_88808;

    // pivot translation matrix
    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[10] = 1.0f; pivotM[15] = 1.0f;
    pivotM[3] = paintcanvas_ext_dr3_signedtofloat(-pivotX, 0);
    pivotM[7] = paintcanvas_ext_dr3_signedtofloat(-pivotY, 0);

    // inverse-pivot translation
    float invPivotM[16];
    memset(invPivotM, 0, sizeof(invPivotM));
    invPivotM[0] = 1.0f; invPivotM[5] = 1.0f; invPivotM[10] = 1.0f; invPivotM[15] = 1.0f;
    invPivotM[3] = paintcanvas_ext_dr3_signedtofloat(transX + pivotX, 0);
    invPivotM[7] = paintcanvas_ext_dr3_signedtofloat(transY + pivotY, 0);

    // rotation matrix (angle = invPivotM[7])
    float rotAng = invPivotM[7];
    float s = paintcanvas_ext_dr3_sinf(rotAng);
    float c = paintcanvas_ext_dr3_cosf(rotAng);
    float rotM[16];
    memset(rotM, 0, sizeof(rotM));
    rotM[0] = c;
    rotM[5] = c;
    rotM[10] = 1.0f;
    rotM[15] = 1.0f;
    *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
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
                                 *(void **)(((char **)this->images)[index]));
    paintcanvas_ext_dr3_glenable(0xb44);
}

// Re-seed an Image2D's 2D quad vertex/UV buffers from its source rect, resetting the
// sprite to draw its full source region.
void PaintCanvas::RestoreImage2D(AbyssEngine::Image2D *image)
{
    char *img = reinterpret_cast<char *>(image);
    char *m = *(char **)img;
    *(unsigned char *)(img + 0x14) = 0;
    char *s = *(char **)(m + 0x4);

    float w = (float)(unsigned int)*(unsigned short *)(img + 0x10);
    float h = (float)(unsigned int)*(unsigned short *)(img + 0x12);

    *(float *)(s + 0x0) = 0.0f;
    *(float *)(s + 0x4) = 0.0f;
    *(float *)(s + 0x8) = 0.0f;
    *(float *)(s + 0xc) = w;
    *(float *)(s + 0x10) = 0.0f;
    *(float *)(s + 0x14) = 0.0f;
    *(float *)(s + 0x18) = w;
    *(float *)(s + 0x1c) = h;
    *(float *)(s + 0x20) = 0.0f;
    *(float *)(s + 0x24) = 0.0f;
    *(float *)(s + 0x28) = h;
    *(float *)(s + 0x2c) = 0.0f;

    float invW = (float)(1.0 / (double)(unsigned int)*(unsigned short *)(img + 0xa));
    float invH = (float)(1.0 / (double)(unsigned int)*(unsigned short *)(img + 0x8));
    float u0 = (float)(unsigned int)*(unsigned short *)(img + 0xc);
    float v0 = (float)(unsigned int)*(unsigned short *)(img + 0xe);

    float *buf = *(float **)(m + 0x8);
    float a = u0 * invH;
    float b = v0 * invW;
    float c = (h + v0) * invW;
    float d = (w + u0) * invH;
    buf[0] = a;
    buf[1] = b;
    buf[2] = d;
    buf[3] = b;
    buf[4] = d;
    buf[5] = c;
    buf[6] = a;
    buf[7] = c;

    int *ipd = *(int **)(m + 0x2c);
    ipd[0] = 0x20000;
    ipd[1] = 1;
    ipd[2] = 0x20003;
}

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

void PaintCanvas::SpriteSystemGetPosition(unsigned int index, unsigned short sub,
                             const Matrix &m, Vector &out)
{
    if (index < this->spriteSystemCount) {
        char *s = (this->spriteSystems)[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            const float *mm = (const float *)&m;
            float *p = (float *)(*(char **)(s + 0x4) + sub * 12);
            float p0 = p[0];
            float p1 = p[1];
            float p2 = p[2];
            float half = (float)(*(short *)s >> 1);
            float x = mm[0] * p0 + mm[1] * p1 + mm[2] * p2 + mm[3];
            float y = mm[4] * p0 + mm[5] * p1 + mm[6] * p2 + mm[7];
            float z = mm[8] * p0 + mm[9] * p1 + mm[10] * p2 + mm[11];
            out.x = x - half;
            out.y = y + half;
            out.z = z;
        }
    }
}

void PaintCanvas::MeshSet2DMask(unsigned int index, int, int)
{
    unsigned int i = index;
    if (this->imageCount <= index) {
        return;
    }
    char **arr = this->images;
    char *img = arr[i];
    if (*(unsigned char *)(img + 0x14) != 0) {
        RestoreImage2D(reinterpret_cast<AbyssEngine::Image2D *>(img));
        arr = this->images;
    }
    this->mask2dImage = arr[i];
}

__attribute__((visibility("hidden"))) extern int *const g_rar_curtex_87c98;
__attribute__((visibility("hidden"))) extern int *const g_rar_texcount_87cce;
__attribute__((visibility("hidden"))) extern int *const g_rar_tricount_87d96;

void PaintCanvas::ReleaseAllResources()
{
    *g_rar_curtex_87c98 = 0;

    // mark all resources as unloaded
    for (int i = 0; i < this->resourceCount; i++) {
        char *res = ((char **)this->resources)[i];
        *(int *)(res + 8) = -1;
    }

    // textures
    for (unsigned int i = 0; i < this->cubeTextureCount; i++) {
        int *tex = ((int **)this->cubeTextures)[i];
        if (*tex != -1) {
            unsigned int id = (unsigned int)*tex;
            paintcanvas_ext_rar_gldeltex(1, &id);
            *g_rar_texcount_87cce = *g_rar_texcount_87cce - 1;
            Engine *eng = (Engine *)this->engine;
            char *texEntry = ((char **)this->cubeTextures)[i];
            eng->field_0x70 = eng->field_0x70 - *(int *)(texEntry + 0x18);
            tex = ((int **)this->cubeTextures)[i];
        }
        if (tex != 0) {
            paintcanvas_ext_rar_str_dtor((char *)tex + 4);
            paintcanvas_ext_rar_op_delete(tex);
        }
        ((int **)this->cubeTextures)[i] = 0;
    }
    this->cubeTextureCount = 0;

    // fonts
    for (unsigned int i = 0; i < this->fontCount; i++) {
        if (((void **)this->fonts)[i] != 0) {
            paintcanvas_ext_rar_fontrelease(this->engine,
                                            &((void **)this->fonts)[i]);
        }
    }
    PCArrayRemoveAll(&this->fontCount);

    // image2D
    for (unsigned int i = 0; i < this->imageCount; i++) {
        if (((void **)this->images)[i] != 0) {
            paintcanvas_ext_rar_img2drelease(this->engine,
                                             &((void **)this->images)[i]);
        }
    }
    PCArrayRemoveAll(&this->imageCount);

    // meshes
    for (unsigned int i = 0; i < this->meshCount; i++) {
        char *mesh = ((char **)this->meshes)[i];
        if (mesh != 0) {
            *g_rar_tricount_87d96 = *g_rar_tricount_87d96 - *(int *)(mesh + 0x7c);
            paintcanvas_ext_rar_meshrelease(this->engine,
                                            &((void **)this->meshes)[i]);
        }
    }
    PCArrayRemoveAll(&this->meshCount);

    // transforms
    for (unsigned int i = 0; i < this->transformCount; i++) {
        void *tf = ((void **)this->transforms)[i];
        if (tf != 0) {
            paintcanvas_ext_rar_op_delete(paintcanvas_ext_rar_transform_dtor(tf));
            ((void **)this->transforms)[i] = 0;
        }
    }
    PCArrayRemoveAll(&this->transformCount);

    // cameras
    for (unsigned int i = 0; i < this->cameraCount; i++) {
        void *cam = ((void **)this->cameras)[i];
        if (cam != 0) {
            paintcanvas_ext_rar_op_delete(cam);
            ((void **)this->cameras)[i] = 0;
        }
    }
    PCArrayRemoveAll(&this->cameraCount);
    this->currentCamera = -1;

    // materials
    for (unsigned int i = 0; i < this->materialCount; i++) {
        void *mat = ((void **)this->materials)[i];
        if (mat != 0) {
            paintcanvas_ext_rar_op_delete(paintcanvas_ext_rar_material_dtor(mat));
            ((void **)this->materials)[i] = 0;
        }
    }
    PCArrayRemoveAll(&this->materialCount);

    // sprite systems
    for (unsigned int i = 0; i < this->spriteSystemCount; i++) {
        if (((void **)this->spriteSystems)[i] != 0) {
            paintcanvas_ext_rar_ssrelease(this->engine,
                                          &((void **)this->spriteSystems)[i]);
        }
    }
    PCArrayRemoveAll(&this->spriteSystemCount);
    this->field_0x1cc = 0;
}

__attribute__((visibility("hidden"))) extern const unsigned int g_tg2d_defval_7b590;

// Picks a transform (by index) from the canvas's transform list, projects the screen-space pick
// coordinate (x,y,z) into the transform's local space and forwards to the Transform* overload to
// resolve the (u,v) texture region under the ray. Returns {-1,-1} when the index is out of range.
PaintCanvas::PickedTextureRegion
PaintCanvas::TransformGet2DPickedTextureRegion(unsigned int transformIndex, int x, int y, int z,
                                               int w)
{
    PickedTextureRegion result;
    char matbuf[60];
    char vecbuf[64];

    if (transformIndex < this->transformCount) {
        char *tf = ((char **)this->transforms)[transformIndex];
        paintcanvas_ext_tg2d_memcpy(vecbuf, tf, 0x3c);
        float fy = paintcanvas_ext_tg2d_signedtofloat(y, 0);
        float fz = paintcanvas_ext_tg2d_signedtofloat(z, 0);
        float fx = paintcanvas_ext_tg2d_signedtofloat(w, 0);
        float vin[3];
        vin[0] = fy;
        vin[1] = fz;
        vin[2] = fx;
        paintcanvas_ext_tg2d_invtransformvec(matbuf, vecbuf);
        paintcanvas_ext_tg2d_vec_assign(vin, matbuf);
        result = this->TransformGet2DPickedTextureRegion(
            reinterpret_cast<::Transform *>(tf), x, (int)vin[0], (int)vin[1], 0);
    } else {
        result.u = *(const float *)&g_tg2d_defval_7b590;
        result.v = *(const float *)&g_tg2d_defval_7b590;
        paintcanvas_ext_tg2d_errmsg(&result);
    }
    return result;
}

void PaintCanvas::CheckString(unsigned int index, const AbyssEngine::String &str)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        void *data = paintcanvas_ext_str_text(&str);
        unsigned int len = paintcanvas_ext_strlen(data);
        return paintcanvas_ext_check_string(font, len, str.size());
    }
}

extern "C" int paintcanvas_ext_rt_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);

void PaintCanvas::ReloadTextures()
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < this->cubeTextureCount; i++) {
        char *res = (this->cubeTextures)[i];
        if (*(int *)res == -1) {
            char *path = paintcanvas_ext_rt_getAEChar(res + 4);
            float f = *(float *)((this->cubeTextures)[i] + 0x10);
            int ok = paintcanvas_ext_rt_texfromfile(this->engine, path, 0, 0,
                                                    &out, false, f);
            if (ok == 1) {
                *(int *)((this->cubeTextures)[i]) = 0;
            }
            paintcanvas_ext_rt_deletearr(path);
        }
    }
}

namespace AbyssEngine {
}

void PaintCanvas::MaterialCreate(unsigned short resId, unsigned int &out)
{
    char *res = (char *)paintcanvas_ext_matc_findres(this, resId);
    if (res == 0) {
        return;
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *info = *(char **)(res + 0xc);
        char *mat = (char *)paintcanvas_ext_matc_new_material();
        for (unsigned int i = 0; i < 8; i++) {
            unsigned short tid = *(unsigned short *)(info + i * 2);
            if (tid != 0xffff) {
                char *tres = (char *)paintcanvas_ext_matc_findres(this, tid);
                if (tres == 0) {
                    break;
                }
                int t = *(int *)(tres + 8);
                if (t == -1) {
                    paintcanvas_ext_matc_texcreate(this, *(unsigned short *)(info + i * 2), true);
                    t = *(int *)(tres + 8);
                }
                *(int *)(mat + i * 4) = t;
            }
        }
        *(unsigned int *)(mat + 0x20) = *(unsigned int *)(info + 0x10);
        *(unsigned int *)(mat + 0x24) = *(unsigned int *)(info + 0x14);
        *(unsigned int *)(mat + 0x28) = *(unsigned int *)(info + 0x18);
        paintcanvas_ext_matc_vec_assign(mat + 0x68, info + 0x1c);
        PCArrayAdd<AbyssEngine::Material *>((AbyssEngine::Material *)mat, &this->materialCount);
        idx = this->materialCount - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    out = idx;
}

__attribute__((visibility("hidden"))) extern const unsigned int g_tg2di_neg1_8b588;

// Resolves the (u,v) texture region under a pick ray for a single Transform node. First intersects
// the node's own meshes (count @0x3c, array @0x40); if none is hit, projects the ray into each
// child transform's (count @0x4c, array @0x50) local space and recurses. Returns {-1,-1} on a miss.
PaintCanvas::PickedTextureRegion
PaintCanvas::TransformGet2DPickedTextureRegion(::Transform *transform, int x, int y, int z, int w)
{
    PickedTextureRegion result;
    char *tf = reinterpret_cast<char *>(transform);
    int shift = w;
    float vy = paintcanvas_ext_tg2di_signedtofloat((y >> shift) >> shift, 0);
    float vx = paintcanvas_ext_tg2di_signedtofloat((z >> shift) >> shift, 0);

    // First: test the node's own meshes (offset 0x3c count, 0x40 array)
    unsigned int i = 0;
    bool found = false;
    while (i < *(unsigned int *)(tf + 0x3c)) {
        void *mesh = (*(void ***)(tf + 0x40))[i];
        paintcanvas_ext_tg2di_meshintersect(&result, vx, vy, mesh);
        i++;
        if (result.u != -1.0f && result.v != -1.0f) {
            found = true;
            break;
        }
    }
    if (found) {
        return result;
    }

    // Then: recurse over child transforms (0x4c count, 0x50 array)
    float fy = paintcanvas_ext_tg2di_signedtofloat(z, 0);
    float fx = paintcanvas_ext_tg2di_signedtofloat(y, 0);
    float fz = paintcanvas_ext_tg2di_signedtofloat(w, 0);

    i = 0;
    while (i < *(unsigned int *)(tf + 0x4c)) {
        char vecbuf[64];
        char matbuf[60];
        char *child = (*(char ***)(tf + 0x50))[i];
        paintcanvas_ext_tg2di_memcpy(vecbuf, child, 0x3c);
        float vin[3];
        vin[0] = fx;
        vin[1] = fy;
        vin[2] = fz;
        paintcanvas_ext_tg2di_invtransformvec(matbuf, vecbuf);
        paintcanvas_ext_tg2di_vec_assign(vin, matbuf);
        result = this->TransformGet2DPickedTextureRegion(
            reinterpret_cast<::Transform *>(child), x, (int)vin[0], (int)vin[1], 0);
        i++;
        if (result.u != -1.0f && result.v != -1.0f) {
            return result;
        }
    }
    result.u = *(const float *)&g_tg2di_neg1_8b588;
    result.v = *(const float *)&g_tg2di_neg1_8b588;
    return result;
}

__attribute__((visibility("hidden"))) extern const double g_dt_gravscale_898d8;

void PaintCanvas::DrawTransform(unsigned int index, const AbyssEngine::AEMath::Matrix *viewMatrix)
{
    if (index >= this->transformCount) {
        return;
    }
    char *tf = ((char **)this->transforms)[index];
    if (*(unsigned char *)(tf + 0xec) == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f; worldM[5] = 1.0f; worldM[14] = 1.0f;

    if (this->currentCamera < this->cameraCount) {
        void *cam = ((void **)this->cameras)[this->currentCamera];
        if (this->initialized == 0) {
            int vis = paintcanvas_ext_dt2_incamvf(((void **)this->transforms)[index], 0, cam);
            if (vis == 0) {
                this->culledCount += 1;
                return;
            }
            float inv[16];
            paintcanvas_ext_dt2_mtx_getinv(inv, worldM);
            paintcanvas_ext_dt2_mtx_assign(worldM, inv);
        } else {
            float rotM[16];
            char scratch[60];
            memset(rotM, 0, sizeof(rotM));
            rotM[0] = 1.0f; rotM[5] = 1.0f; rotM[14] = 1.0f;
            paintcanvas_ext_dt2_matidentity(scratch, rotM);

            void *grav = paintcanvas_ext_dt2_getgrav(this->engine);
            double angle = *(double *)((char *)grav + 8) * g_dt_gravscale_898d8;
            float a = (float)angle;
            int orient = this->gameOrientation;
            float ang = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dt2_sinf(ang);
            float c = paintcanvas_ext_dt2_cosf(ang);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            int vis = paintcanvas_ext_dt2_incamvf(((void **)this->transforms)[index], rotM, cam);
            if (vis == 0) {
                this->culledCount += 1;
                return;
            }

            float viewM[16];
            const float *src;
            if (viewMatrix == 0) {
                src = (const float *)(((char **)this->cameras)[this->currentCamera] + 0xc);
            } else {
                src = *viewMatrix;
            }
            paintcanvas_ext_dt2_mtx_assign(viewM, src);
            paintcanvas_ext_dt2_mtx_muleq(viewM, rotM);
            paintcanvas_ext_dt2_mtx_getinv(scratch, viewM);
            paintcanvas_ext_dt2_mtx_assign(worldM, scratch);
        }
        void *cam2 = ((void **)this->cameras)[this->currentCamera];
        paintcanvas_ext_dt2_seteye(this->engine,
                                   *(float *)((char *)cam2 + 0x18),
                                   *(float *)((char *)cam2 + 0x28),
                                   *(float *)((char *)cam2 + 0x38));
        tf = ((char **)this->transforms)[index];
    }

    float ident[16];
    memset(ident, 0, sizeof(ident));
    ident[0] = 1.0f; ident[5] = 1.0f; ident[14] = 1.0f;
    paintcanvas_ext_dt2_drawrec(this, tf, ident, worldM);
}

void PaintCanvas::FontSetSpacing(unsigned int index, short spacing)
{
    if (index < this->fontCount) {
        void *font = (this->fonts)[index];
        return paintcanvas_ext_font_set_spacing(font, spacing);
    }
}

int PaintCanvas::GetWidth()
{
    return paintcanvas_ext_get_width(this->engine);
}

// AbyssEngine::PaintCanvas::CameraCreate(unsigned& out) — allocates a Camera sized to the
// current viewport and appends it to the camera array at +0x164, returning its index.

void PaintCanvas::CameraCreate(unsigned int &out)
{
    void *cam = operator new(0x5c);
    int w = pc_GetWidth(this);
    int h = pc_GetHeight(this);
    pc_Camera_ctor(cam, (float)h, (float)w);
    pc_ArrayAdd_Camera(cam, &this->cameraCount);
    out = this->cameraCount - 1;
}

__attribute__((visibility("hidden"))) extern const double g_dss_gravscale_8ada0;

void PaintCanvas::DrawSpriteSystem(unsigned int index, AbyssEngine::AEMath::Matrix mat)
{
    if (index >= this->spriteSystemCount) {
        return;
    }
    void *ss = ((void **)this->spriteSystems)[index];
    if (ss == 0) {
        return;
    }

    // local copy of the incoming matrix
    float local[15];
    memcpy(local, mat, sizeof(local));

    float identbuf[16];
    char scratch[60];
    float inv[16];

    if (this->initialized == 0) {
        paintcanvas_ext_dss_mtx_getinv(inv, local);
        paintcanvas_ext_dss_mtx_assign(local, inv);
    } else {
        memset(identbuf, 0, sizeof(identbuf));
        identbuf[0] = 1.0f;
        identbuf[5] = 1.0f;
        identbuf[14] = 1.0f;
        paintcanvas_ext_dss_matidentity(scratch, identbuf);

        void *grav = paintcanvas_ext_dss_getgrav(this->engine);
        double angle = *(double *)((char *)grav + 8) * g_dss_gravscale_8ada0;
        float a = (float)angle;
        int orient = this->gameOrientation;
        float rot = (orient == 1) ? a : -a;
        float s = paintcanvas_ext_dss_sinf(rot);
        float c = paintcanvas_ext_dss_cosf(rot);
        identbuf[0] = c;
        identbuf[5] = c;
        *(unsigned int *)&identbuf[1] = *(unsigned int *)&s ^ 0x80000000;
        identbuf[4] = s;

        paintcanvas_ext_dss_mtx_muleq(local, identbuf);
        paintcanvas_ext_dss_mtx_getinv(scratch, local);
        paintcanvas_ext_dss_mtx_assign(local, scratch);
    }

    float ident2[16];
    memset(ident2, 0, sizeof(ident2));
    ident2[0] = 1.0f;
    ident2[5] = 1.0f;
    ident2[14] = 1.0f;
    paintcanvas_ext_dss_ssdraw(this->engine, ident2, local,
                               ((void **)this->spriteSystems)[index]);
}

extern "C" void paintcanvas_ext_drawstring_str(void *, unsigned int, unsigned int, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void PaintCanvas::DrawString(unsigned int index, const AbyssEngine::String &str,
                int x, int y, bool b)
{
    if (index < this->fontCount) {
        char *font = (char *)(this->fonts)[index];
        paintcanvas_ext_string_prep(this, *(void **)(font + 0x8), -1);
        char *font2 = (char *)(this->fonts)[index];
        void *data = paintcanvas_ext_str_text(&str);
        unsigned int len = paintcanvas_ext_strlen(data);
        paintcanvas_ext_drawstring_str(font2, len, str.size(), x, y,
                                       this, this->engine, b);
    }
}

int PaintCanvas::TransformGetTriCount(::Transform *transform)
{
    if (!transform) {
        return 0;
    }
    int total = 0;
    unsigned int n1 = transform->field_0x3c;
    for (unsigned int i = 0; i != n1; ++i) {
        total += this->MeshGetTriCount(transform->field_0x40[i]);
    }
    unsigned int n2 = transform->field_0x4c;
    for (unsigned int i = 0; i != n2; ++i) {
        total += this->TransformGetTriCount(transform->field_0x50[i]);
    }
    return total;
}

// Distinct overload from the single-mesh variant above: recurses over a Transform
// node's child meshes and child transforms. The first object param is a Transform*,
// which also disambiguates it from the (PaintCanvas*, Mesh-bytes char*, ...) overload.
void PaintCanvas::MeshChangeShaderAnimValue(::Transform *transform, float value, unsigned int mode)
{
    if (transform) {
        for (unsigned int i = 0; i < transform->field_0x3c; ++i) {
            void *m = transform->field_0x40[i];
            paintcanvas_ext_mesh_shaderanim(this, m, value, mode);
        }
        for (unsigned int i = 0; i < transform->field_0x4c; ++i) {
            void *c = transform->field_0x50[i];
            paintcanvas_ext_transform_shaderanim(this, c, value, mode);
        }
    }
}

void PaintCanvas::ReleaseSpriteSystemResource(unsigned int index)
{
    if (index < this->spriteSystemCount) {
        void *ctx = this->engine;
        char **arr = this->spriteSystems;
        return paintcanvas_ext_release_sprite_res(ctx, arr + index);
    }
}

__attribute__((visibility("hidden"))) extern const double g_dss2_gravscale_8af58;

void PaintCanvas::DrawSpriteSystem(unsigned int index,
                      AbyssEngine::AEMath::Matrix matA, AbyssEngine::AEMath::Matrix matB)
{
    if (index >= this->spriteSystemCount) {
        return;
    }
    if (((void **)this->spriteSystems)[index] == 0) {
        return;
    }

    // local copies: matA -> world, matB -> view
    float world[15];
    float view[15];
    memcpy(world, matA, sizeof(world));
    memcpy(view, matB, sizeof(view));

    if (this->initialized == 0) {
        float inv[16];
        memset(inv, 0, sizeof(inv));
        inv[0] = 1.0f;
        inv[5] = 1.0f;
        inv[14] = 1.0f;
        paintcanvas_ext_dss2_mtx_getinv(inv, view);
        paintcanvas_ext_dss2_mtx_assign(view, inv);
    } else {
        float rotM[16];
        char scratch[60];
        memset(rotM, 0, sizeof(rotM));
        rotM[0] = 1.0f;
        rotM[5] = 1.0f;
        rotM[14] = 1.0f;
        paintcanvas_ext_dss2_matidentity(scratch, rotM);

        void *grav = paintcanvas_ext_dss2_getgrav(this->engine);
        double angle = *(double *)((char *)grav + 8) * g_dss2_gravscale_8af58;
        int ia = (int)(long long)angle;
        if (this->gameOrientation == 1) {
            ia = -ia;
        }
        float a = paintcanvas_ext_dss2_signedtofloat(ia, 0);
        float s = paintcanvas_ext_dss2_sinf(a);
        float c = paintcanvas_ext_dss2_cosf(a);
        rotM[0] = c;
        rotM[5] = c;
        *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
        rotM[4] = s;

        paintcanvas_ext_dss2_mtx_muleq(view, rotM);
        paintcanvas_ext_dss2_mtx_getinv(scratch, view);
        paintcanvas_ext_dss2_mtx_assign(view, scratch);
        paintcanvas_ext_dss2_mtx_muleq(world, rotM);
    }

    paintcanvas_ext_dss2_ssdraw(this->engine, world, view,
                                ((void **)this->spriteSystems)[index]);
}
