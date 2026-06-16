#include "gof2/engine/render/PaintCanvas.h"
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
inline void PCArrayDtor(void *arrayHeader) {
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
inline void PCArrayReleaseClasses(void *arrayHeader) {
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
extern "C" void *paintcanvas_ext_str_text(void *);
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
extern "C" void *paintcanvas_ext_getscreenpos_m(void *self, void *m, Vector *a, Vector *b);
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
extern "C" void *paintcanvas_ext_gla_str_new();
extern "C" void paintcanvas_ext_gla_str_copy(void *out, void *src, bool copy);
extern "C" void paintcanvas_ext_gla_str_fromchar(void *out, const char *s, bool copy);
extern "C" void paintcanvas_ext_gla_str_append(void *dst, void *src);
extern "C" void paintcanvas_ext_gla_str_dtor(void *s);
extern "C" void paintcanvas_ext_gla_substr(void *out, void *str, unsigned int begin, unsigned int end);
extern "C" void paintcanvas_ext_gla_getline(void *self, unsigned int font, void *line, int width, void *out);
extern "C" void paintcanvas_ext_gla_str_vdtor(void *s);
extern "C" void paintcanvas_ext_gla_arr_setlen(unsigned int n, void *arr);
extern "C" unsigned short *paintcanvas_ext_gla_str_index(void *s, int i);
extern "C" void paintcanvas_ext_gla_str_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_add_resource(void *, void *);
extern "C" void paintcanvas_ext_remove_meshid(void *, void *);
extern "C" void paintcanvas_ext_get_height(void *);
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
extern "C" unsigned short *paintcanvas_ext_gl_strindex(void *str, unsigned int i);
extern "C" void paintcanvas_ext_gl_substr(void *out, void *str, unsigned int begin, unsigned int end);
extern "C" void paintcanvas_ext_gl_str_fromchar(void *out, const char *s, bool copy);
extern "C" void paintcanvas_ext_gl_str_assign(void *dst, void *src);
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
extern "C" void paintcanvas_ext_set_reslist(void *, unsigned int, void *);
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
extern "C" int paintcanvas_ext_cisvf_inner(void *pt, float radius, void *m, void *cam);
extern "C" void paintcanvas_ext_shader_anim(void *, void *);
extern "C" char *paintcanvas_ext_rs_getAEChar(void *strField);
extern "C" void paintcanvas_ext_rs_deletearr(char *p);
extern "C" void paintcanvas_ext_rs_glActiveTexture(unsigned int tex);
extern "C" void paintcanvas_ext_rs_glBindTexture(unsigned int target, unsigned int tex);
extern "C" void paintcanvas_ext_tami_bsphere_merge(void *dst, void *src);
extern "C" void paintcanvas_ext_tami_setanimlen(void *tf, int hi, int lo);
extern "C" void paintcanvas_ext_tami_setanimstate(void *tf, int a, int b);
extern "C" void paintcanvas_ext_tami_finalize(void *tf);
extern "C" void grs2_GetReverseString_impl(String *out, unsigned r1, String *tmp, int reverse);
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
extern "C" int paintcanvas_ext_cipvf_inner(void *pt, void *m, void *cam);
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
extern "C" void paintcanvas_ext_gsp2_transformvec(void *out, void *vec);
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
extern "C" char *paintcanvas_ext_find_mesh(void *, void *);
extern "C" void paintcanvas_ext_remove_mesh(void *, unsigned int, int);
extern "C" void paintcanvas_ext_enable(int);
extern "C" void paintcanvas_ext_depthmask(int);
extern "C" void paintcanvas_ext_clear2(void *, unsigned int);
extern "C" void paintcanvas_ext_array_remove(void *, void *);
extern "C" void *paintcanvas_ext_tc_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_start_fbo(void *);
extern "C" void paintcanvas_ext_gl_deletetextures(int, void *);
extern "C" void paintcanvas_ext_mtx_mul(void *out, void *a, void *b);
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
extern "C" void paintcanvas_ext_dsc_str_copy(void *out, void *src, bool copy);
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
void RestoreImage2D(void *, void *);
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
extern "C" void paintcanvas_ext_get_width(void *);
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
    if (index < this->field_0x14c) {
        char *img = (this->field_0x150)[index];
        return *(unsigned short *)(img + 0x10);
    }
    return 0;
}

unsigned int PaintCanvas::CameraGetCurrent()
{
    unsigned int cur = this->field_0x170;
    if (cur >= this->field_0x164) {
        cur = 0xffffffff;
    }
    return cur;
}

void PaintCanvas::HasVibration()
{
    return paintcanvas_ext_has_vibration(this->field_0x34);
}

void *PaintCanvas::CameraGetLocal(unsigned int index)
{
    void *result;
    if (index < this->field_0x164) {
        result = (char *)(this->field_0x168)[index] + 0xc;
    } else {
        char tmp[60];
        result = &this->identityMatrix;
        MatrixIdentity(tmp, result);
    }
    return result;
}

void PaintCanvas::DrawTextLines(unsigned int font,
                   AbyssEngine::Array<void *> *arr, int x, int y, bool center)
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

    float *vb = *(float **)(this->field_0x1c8 + 0x4);
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
    paintcanvas_ext_fr_glenable(this->field_0x34, 0xde1, false);
    paintcanvas_ext_fr_meshdraw(this->field_0x34, this->field_0x1c8);
    paintcanvas_ext_fr_glenable(this->field_0x34, 0xde1, true);
}

void PaintCanvas::SpriteSystemSetRGBA(unsigned int index, unsigned short sub,
                         float a, float b, float c, float d)
{
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            return paintcanvas_ext_sprite_rgba(sub, a, b, c, d, s);
        }
    }
}

void PaintCanvas::TransformCreate(unsigned int *out)
{
    void *obj = paintcanvas_ext_alloc(0x180);
    paintcanvas_ext_transform_ctor(obj);
    paintcanvas_ext_add_child(obj, &this->field_0x158);
    *out = this->field_0x158 - 1;
}

__attribute__((visibility("hidden"))) extern char *const g_dr_flag_79368;

void PaintCanvas::DrawRectangle(int param_1, int param_2,
                   int param_3, int param_4)
{
    char abuf[60];

    double dw = (double)param_3;
    double dh = (double)param_4;
    double dy = (double)param_2;
    double dx = (double)param_1;
    float fVar4 = (float)(dw - 0.5 + dx);
    float fVar5 = (float)(dh - 0.5 + dy);

    float *v = this->lineVerts;
    v[0] = (float)(dx + 0.5);
    v[1] = (float)(dy + 0.5);
    v[2] = fVar4;
    v[3] = (float)(dy + 0.5);
    v[4] = fVar4;
    v[5] = fVar5;
    v[6] = (float)(dx + 0.5);
    v[7] = fVar5;

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
        paintcanvas_ext_dr_glcap(this->field_0x34, 0xde1, 0);
        paintcanvas_ext_dr_glVertexPointer(2, 0x1406, 0, this->lineVerts);
        paintcanvas_ext_dr_glColorMask(this->field_0x34, 0x8074, 1);
        paintcanvas_ext_dr_glColorMask(this->field_0x34, 0x8078, 0);
        paintcanvas_ext_dr_glColorMask(this->field_0x34, 0x8075, 0);
        paintcanvas_ext_dr_glColorMask(this->field_0x34, 0x8076, 0);
        paintcanvas_ext_dr_glDrawArrays(2, 0, 4);
        paintcanvas_ext_dr_glcap(this->field_0x34, 0xde1, 1);
    } else {
        paintcanvas_ext_dr_drawline2d(this->field_0x34, this->lineVerts, 4, true);
    }
}

unsigned short PaintCanvas::GetImage2DHeight(unsigned int index)
{
    if (index < this->field_0x14c) {
        char *img = (this->field_0x150)[index];
        return *(unsigned short *)(img + 0x12);
    }
    return 0;
}

extern char paintcanvas_g_flipv;

void PaintCanvas::MeshSetUv(unsigned int index, unsigned short sub,
               float u, float v)
{
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
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
    paintcanvas_ext_fbo_a(this->field_0x34);
    paintcanvas_ext_fbo_b(this, 0);
    paintcanvas_ext_fbo_c(this->field_0x34);
    return paintcanvas_ext_fbo_d(this->field_0x34);
}

void PaintCanvas::SpriteSystemSetAllUv(unsigned int index,
                          float a, float b, float c, float d)
{
    if (this->field_0x180 <= index) {
        return;
    }
    void *sprite = (this->field_0x184)[index];
    if (sprite == 0) {
        return;
    }
    return paintcanvas_ext_sprite_alluv(a, b, c, d, sprite);
}

using AbyssEngine::AEMath::Vector;

void PaintCanvas::SpriteSystemGetPosition(unsigned int index, unsigned short sub, Vector &out)
{
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
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

void PaintCanvas::SpriteSystemSetAllSize(unsigned int index, unsigned int size)
{
    if (index < this->field_0x180) {
        void *sprite = (this->field_0x184)[index];
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
                    unsigned int param3, unsigned int param4)
{
    if (index < this->field_0x174) {
        unsigned int *mat = (unsigned int *)(this->field_0x178)[index];
        mat[8] = param3;
        mat[0] = param4;
    }
}

void PaintCanvas::DrawTextLines(unsigned int font,
                   AbyssEngine::Array<void *> *arr, int x, int y, unsigned int p5, bool flag)
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
    unsigned int count = this->field_0x134;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (this->field_0x138)[i];
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
    if (index < this->field_0x158) {
        result = (this->field_0x15c)[index];
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
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x14);
        *(Vector *)(base + vtx * 12) = value;
    }
}

int PaintCanvas::GetTextWidth(unsigned int index, void *str,
                 unsigned int begin, unsigned int end)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
        void *text = paintcanvas_ext_str_text(str);
        return paintcanvas_ext_text_width_range(font, text, begin, end - begin);
    }
    return 0;
}

void *PaintCanvas::MaterialGetMaterial(unsigned int index)
{
    if (index < this->field_0x174) {
        return (this->field_0x178)[index];
    }
    return 0;
}

__attribute__((visibility("hidden"))) char *paintcanvas_g_bg_flag;

void PaintCanvas::EndBG()
{
    *paintcanvas_g_bg_flag = this->field_0x1f0;
}

void *PaintCanvas::FindResource(unsigned short id)
{
    unsigned int count = this->field_0x134;
    void *found = 0;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (this->field_0x138)[i];
        if (res && *(unsigned short *)res == id) {
            found = res;
            break;
        }
    }
    return found;
}

void *PaintCanvas::MeshGetPointer(unsigned int index)
{
    if (index < this->field_0x24) {
        return (this->field_0x28)[index];
    }
    return 0;
}

void PaintCanvas::MeshChangeResourceMaterial(unsigned int meshIndex, unsigned int resId)
{
    char *r = paintcanvas_ext_find_res(this, resId);
    if (r) {
        int idx = *(int *)(r + 0x8);
        if (idx + 1 != 0) {
            void *mesh = (this->field_0x28)[meshIndex];
            void *mat = (this->field_0x178)[idx];
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
    this->field_0x1fc = c0;
    this->field_0x200 = c1;
    this->field_0x204 = c2;
    this->field_0x208 = c3;
    return paintcanvas_ext_setcolor(this->field_0x34, c0, c1, c2, c3);
}

void PaintCanvas::Vibrate(unsigned short)
{
    return paintcanvas_ext_vibrate(this->field_0x34);
}

void PaintCanvas::SpriteSystemAddSize(unsigned int index, unsigned short sub, short delta)
{
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
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
    if (index < this->field_0x158) {
        Matrix *t = (Matrix *)(this->field_0x15c)[index];
        *t = matrix;
    }
}

void DrawTextLines(void *self, unsigned int p1, void *p2, int p3, int p4)
{
    return paintcanvas_ext_drawtextlines6(self, p1, p2, p3, p4, false);
}

unsigned int PaintCanvas::GetMeshResourceId(void *name, unsigned short p2)
{
    for (unsigned int i = 0; i < this->field_0x134; ++i) {
        char *res = (this->field_0x138)[i];
        if (res && *(int *)(res + 0x4) == 4) {
            if (paintcanvas_ext_strcmp(name, **(void ***)(res + 0xc)) == 0) {
                char *res2 = (this->field_0x138)[i];
                if (*(unsigned short *)(*(char **)(res2 + 0xc) + 0x4) == p2) {
                    return *(unsigned short *)res2;
                }
            }
        }
    }
    return 0xffff;
}

int PaintCanvas::GetTextWidth(unsigned int index, void *str)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        return paintcanvas_ext_text_width(font, len, *(unsigned int *)((char *)str + 0x8));
    }
    return 0;
}

void PaintCanvas::SpriteSystemSetSize(unsigned int index,
                         unsigned int sub, unsigned short value)
{
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
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
    this->field_0x1fc = fr;
    this->field_0x200 = fg;
    this->field_0x204 = fb;
    this->field_0x208 = fa;
    return paintcanvas_ext_setcolor(this->field_0x34, fr, fg, fb, fa);
}

float PaintCanvas::CameraGetCurrentFactor1()
{
    unsigned int cur = this->field_0x170;
    if (cur >= this->field_0x164) {
        return 1.0f;
    }
    char *cam = (char *)(this->field_0x168)[cur];
    return *(float *)(cam + 0x48);
}

void ClearDepth()
{
    return paintcanvas_ext_clear(0x100);
}

void PaintCanvas::SpriteSystemAddPosition(unsigned int index, unsigned short sub,
                             float x, float y, float z)
{
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
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
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x18);
        *(Vector *)(base + vtx * 12) = value;
    }
}

using AbyssEngine::AEMath::Vector;

void *PaintCanvas::GetScreenPosition(Vector *a, Vector *b)
{
    char buf[60];
    float *m = (float *)buf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;
    return paintcanvas_ext_getscreenpos_m(this, buf, a, b);
}

#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_fog_flag HIDDEN;
extern char *paintcanvas_g_fog_ptr HIDDEN;

void PaintCanvas::FogEnable(int mode, int enable)
{
    this->field_0x1f4 = enable;
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
    this->field_0x1f1 = (char)mode;
}

void PaintCanvas::MeshSetColor(unsigned int index, unsigned short sub,
                  unsigned int color)
{
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
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
    void *eng = this->field_0x34;
    float *r;
    r = &this->projOrthoMatrix.m[12]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = &this->projOrthoMatrix.m[8]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = &this->projOrthoMatrix.m[4]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = &this->projOrthoMatrix.m[0]; r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;

    int w = paintcanvas_ext_getdisplaywidth(eng);
    this->projOrthoMatrix.m[0] = (float)(2.0 / (double)w);

    int h = paintcanvas_ext_getdisplayheight(this->field_0x34);
    this->projOrthoMatrix.m[10] = -0.05f;
    this->projOrthoMatrix.m[15] = 1.0f;
    this->projOrthoMatrix.m[12] = -1.0f;
    this->projOrthoMatrix.m[13] = 1.0f;
    this->projOrthoMatrix.m[5] = -(float)(2.0 / (double)h);
}

void PaintCanvas::MeshChangeMaterialIntern(AbyssEngine::Mesh *mesh, void *mat)
{
    if (mesh && mat) {
        mesh->field_0x30 = mat;
        return paintcanvas_ext_mat_intern(this, mesh->field_0x34);
    }
}

void PaintCanvas::MeshResourceChangeAllMaterial(unsigned short matId, unsigned short value)
{
    unsigned int count = this->field_0x134;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (this->field_0x138)[i];
        if (res) {
            char *mat = *(char **)(res + 0xc);
            if (*(unsigned short *)(mat + 0x4) == matId) {
                *(unsigned short *)(mat + 0x4) = value;
            }
        }
    }
}

unsigned int PaintCanvas::GetTextureResourceId(void *name)
{
    for (unsigned int i = 0; i < this->field_0x134; ++i) {
        char *res = (this->field_0x138)[i];
        if (res && *(int *)(res + 0x4) == 2) {
            char *n = *(char **)(res + 0xc);
            if (paintcanvas_ext_strcmp(name, *(void **)n) == 0) {
                return *(unsigned short *)((this->field_0x138)[i]);
            }
        }
    }
    return 0xffff;
}

unsigned int PaintCanvas::GetColor()
{
    float a = this->field_0x1fc;
    float r = this->field_0x200;
    float g = this->field_0x204;
    float b = this->field_0x208;
    return ((unsigned int)(int)(r * 255.0f) << 16) +
           ((unsigned int)(int)(a * 255.0f) << 24) +
           ((unsigned int)(int)(g * 255.0f) << 8) +
           (unsigned int)(int)(b * 255.0f);
}

void PaintCanvas::CameraSetCurrent(unsigned int index)
{
    this->field_0x170 = index;
    if (index < this->field_0x164) {
        unsigned int *cam = (unsigned int *)(this->field_0x168)[index];
        return paintcanvas_ext_camera_apply(this, cam[0], cam[1], cam[2]);
    }
}

void PaintCanvas::MeshSetTriangleCount(unsigned int index, unsigned short count)
{
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
        unsigned short cap = *(unsigned short *)(mesh + 0x2a);
        if (cap < count) {
            count = cap;
        }
        *(unsigned short *)(mesh + 0x28) = (unsigned short)(count * 3);
    }
}

void DisableClip()
{
    return paintcanvas_ext_disable(0xc11);
}

void PaintCanvas::TransformSetColor(unsigned int index, unsigned int color)
{
    if (index < this->field_0x158) {
        char *obj = (this->field_0x15c)[index];
        *(unsigned int *)(obj + 0x48) = color;
    }
}

void PaintCanvas::MeshSetColor(unsigned int index, unsigned short sub,
                  float r, float g, float b, float a)
{
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
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
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x10);
        *(Vector *)(base + vtx * 12) = value;
    }
}

void PaintCanvas::MeshClear2DMask()
{
    this->field_0x20 = 0;
}

unsigned int PaintCanvas::GetMeshResourceId(void *name)
{
    for (unsigned int i = 0; i < this->field_0x134; ++i) {
        char *res = (this->field_0x138)[i];
        if (res && *(int *)(res + 0x4) == 4) {
            char *n = *(char **)(res + 0xc);
            if (paintcanvas_ext_strcmp(name, *(void **)n) == 0) {
                return *(unsigned short *)((this->field_0x138)[i]);
            }
        }
    }
    return 0xffff;
}

void PaintCanvas::MeshTranslatePoint(unsigned int index, unsigned short sub,
                        float x, float y, float z)
{
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
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
        paintcanvas_ext_is_posteffect(this->field_0x34) == 0) {
        return paintcanvas_ext_use_refract(this->field_0x34);
    }
}

void PaintCanvas::SpriteSystemSetUv(unsigned int index, unsigned short sub,
                       float a, float b, float c, float d)
{
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
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
    return paintcanvas_ext_set_wvm(this->field_0x34);
}

using AbyssEngine::AEMath::Matrix;

void PaintCanvas::CameraSetLocal(unsigned int index, const Matrix &matrix)
{
    if (index < this->field_0x164) {
        char *cam = (char *)(this->field_0x168)[index];
        *(Matrix *)(cam + 0xc) = matrix;
    }
}

void PaintCanvas::SetShaderMode(int mode)
{
    *(int *)((char *)this->field_0x34 + 0x4a8) = mode;
}

void PaintCanvas::MeshConvertToVBO(unsigned int index)
{
    if (index < this->field_0x24) {
        return paintcanvas_ext_convert_vbo(
            (this->field_0x28)[index]);
    }
}

__attribute__((visibility("hidden"))) extern const float g_di2_one_88d90;
__attribute__((visibility("hidden"))) extern const float g_di2_def_88d94;

void PaintCanvas::DrawImage2D(unsigned int param_1, int param_2, int param_3,
                 unsigned char param_4)
{
    if (param_1 >= this->field_0x14c) {
        return;
    }
    char *img = ((char **)this->field_0x150)[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di2_restore(*(unsigned char *)(img + 0x14), img);
        img = ((char **)this->field_0x150)[param_1];
    }
    paintcanvas_ext_di2_settexture(this, *(unsigned int *)(img + 4), -1);

    float fx = paintcanvas_ext_di2_signedtofloat(param_2, 0);
    float fy = paintcanvas_ext_di2_signedtofloat(param_3, 0);

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;

    if (param_4 & 1) {
        m[0] = g_di2_one_88d90;
        float off = g_di2_def_88d94;
        if (param_1 < this->field_0x14c) {
            unsigned short w = *(unsigned short *)(((char **)this->field_0x150)[param_1] + 0x10);
            off = paintcanvas_ext_di2_unsignedtofloat(w, 0);
        }
        fx = off + fx;
    }
    if (param_4 & 2) {
        m[5] = g_di2_one_88d90;
        float off = g_di2_def_88d94;
        if (param_1 < this->field_0x14c) {
            unsigned short h = *(unsigned short *)(((char **)this->field_0x150)[param_1] + 0x12);
            off = paintcanvas_ext_di2_unsignedtofloat(h, 0);
        }
        fy = off + fy;
    }
    m[3] = fx;
    m[7] = fy;

    paintcanvas_ext_di2_setwvm(this, m);
    paintcanvas_ext_di2_gldisable(0xb44);
    paintcanvas_ext_di2_meshdraw(this->field_0x34,
                                 *(void **)(((char **)this->field_0x150)[param_1]));
    paintcanvas_ext_di2_glenable(0xb44);
}

void PaintCanvas::FontSetYOffset(unsigned int index, short yoff)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
        return paintcanvas_ext_font_set_yoff(font, yoff);
    }
}

__attribute__((visibility("hidden"))) extern char *const g_sbm_flag_8cb62;
__attribute__((visibility("hidden"))) extern const unsigned int g_sbm_const_8ce34;

void PaintCanvas::SetBlendMode(int param_2)
{
    paintcanvas_ext_sbm_lightenable(this->field_0x34, 0);

    char *flag = g_sbm_flag_8cb62;
    if (*flag != 0) {
        paintcanvas_ext_sbm_glenablecap(this->field_0x34, g_sbm_const_8ce34, 0);
    } else {
        paintcanvas_ext_sbm_glTexEnvi(0x2300, 0x2200, 0x2100);
    }

    switch (param_2) {
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
        paintcanvas_ext_sbm_lightenable(this->field_0x34, 1);
        paintcanvas_ext_sbm_lightsetlight(this->field_0x34, 0x4000);
        /* fallthrough */
    case 0:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 7:
        paintcanvas_ext_sbm_lightenable(this->field_0x34, 1);
        paintcanvas_ext_sbm_lightsetlight(this->field_0x34, 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 8:
        paintcanvas_ext_sbm_lightenable(this->field_0x34, 1);
        paintcanvas_ext_sbm_lightsetlight(this->field_0x34, 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 9:
        paintcanvas_ext_sbm_lightenable(this->field_0x34, 1);
        paintcanvas_ext_sbm_lightsetlight(this->field_0x34, 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 10:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_glDepthMask(1);
        paintcanvas_ext_sbm_setalpha(this->field_0x34, 0x1000000, 1);
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
            paintcanvas_ext_sbm_setalpha(this->field_0x34, g_sbm_const_8ce34, 1);
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

void PaintCanvas::GetLineArray(unsigned int param_1, void *param_2, int param_3,
                  char *param_4)
{
    void *acc = paintcanvas_ext_gla_str_new();

    char src[16];      // working string
    char nl[16];
    paintcanvas_ext_gla_str_copy(src, param_2, false);
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
        paintcanvas_ext_gla_getline(this, param_1, line, param_3, acc);
        paintcanvas_ext_gla_str_dtor(line);
        pos += *(int *)((char *)acc + 8);
        paintcanvas_ext_gla_str_dtor(sub);
        count++;
    }
    paintcanvas_ext_gla_str_vdtor(acc);

    // Allocate the output array of count Strings.
    paintcanvas_ext_gla_arr_setlen(count, param_4);
    for (unsigned int i = 0; i < count; i++) {
        void *s = paintcanvas_ext_gla_str_new();
        (*(void ***)(param_4 + 4))[i] = s;
    }

    // Pass 2: fill and trim each line.
    for (unsigned int i = 0; i < count; i++) {
        char sub[16];
        char line[16];
        paintcanvas_ext_gla_substr(sub, src, 0, (unsigned int)*(int *)(src + 8));
        paintcanvas_ext_gla_str_copy(line, sub, false);
        void *out = (*(void ***)(param_4 + 4))[i];
        paintcanvas_ext_gla_getline(this, param_1, line, param_3, out);
        paintcanvas_ext_gla_str_dtor(line);

        void *cur = (*(void ***)(param_4 + 4))[i];
        int len = *(int *)((char *)cur + 8);
        int lo = 0;
        while (*paintcanvas_ext_gla_str_index((*(void ***)(param_4 + 4))[i], lo) == 0x20) {
            lo++;
        }
        len++;
        do {
            len--;
        } while (*paintcanvas_ext_gla_str_index((*(void ***)(param_4 + 4))[i], len - 2) == 0x20);

        char trimmed[16];
        paintcanvas_ext_gla_substr(trimmed, (*(void ***)(param_4 + 4))[i],
                                   (unsigned int)lo, (unsigned int)len);
        paintcanvas_ext_gla_str_assign((*(void ***)(param_4 + 4))[i], trimmed);
        paintcanvas_ext_gla_str_dtor(trimmed);
        paintcanvas_ext_gla_str_dtor(sub);
    }
    paintcanvas_ext_gla_str_dtor(src);
}

void PaintCanvas::AddResource(void *resource)
{
    return paintcanvas_ext_add_resource(resource, &this->field_0x134);
}

void PaintCanvas::TransformRemoveMeshId(unsigned int transformIndex, unsigned int meshIndex)
{
    if (transformIndex < this->field_0x158 &&
        meshIndex < this->field_0x24) {
        void *mesh = (this->field_0x28)[meshIndex];
        char *t = (this->field_0x15c)[transformIndex];
        return paintcanvas_ext_remove_meshid(mesh, t + 0x3c);
    }
}

void PaintCanvas::GetHeight()
{
    return paintcanvas_ext_get_height(this->field_0x34);
}

extern char paintcanvas_g_cube_enabled;
extern int paintcanvas_g_cube_slot;

void PaintCanvas::ChangeCubeTexture(unsigned int idx)
{
    if (paintcanvas_g_cube_enabled != 0 && idx < this->field_0x10) {
        char *tex = (this->field_0x14)[idx];
        if (*(unsigned char *)(tex + 0x14) == 0) {
            return paintcanvas_ext_cube_tail(paintcanvas_ext_cube_restore(tex + 0x4));
        }
        paintcanvas_g_cube_slot = idx;
        paintcanvas_ext_gl_a(0x84c7);
        char *tex2 = (this->field_0x14)[idx];
        paintcanvas_ext_gl_bind(0x8513, *(unsigned int *)tex2);
        return paintcanvas_ext_gl_c(0x84c0);
    }
}

namespace AbyssEngine {
}

void PaintCanvas::TransformCreate(unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_tfc_findres(this, param_1);
    if (res == 0) {
        return;
    }
    if (*(unsigned int *)(res + 8) != 0xffffffff) {
        *param_2 = *(unsigned int *)(res + 8);
        return;
    }
    char *info = *(char **)(res + 0xc);
    char *tf = (char *)paintcanvas_ext_tfc_new_transform();
    PCArrayAdd< ::Transform *>((::Transform *)tf, &this->field_0x158);
    unsigned int idx = this->field_0x158 - 1;
    *(unsigned int *)(res + 8) = idx;
    *param_2 = idx;
    paintcanvas_ext_tfc_mtx_assign(tf, info);

    unsigned int childMesh = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x3c); i++) {
        unsigned short mid = *(unsigned short *)(*(char **)(info + 0x40) + i * 2);
        paintcanvas_ext_tfc_meshcreate(this, mid, &childMesh, false);
    }
    unsigned int childTf = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x44); i++) {
        unsigned short tid = *(unsigned short *)(*(char **)(info + 0x48) + i * 2);
        this->TransformCreate(tid, &childTf);
    }
}

void PaintCanvas::End2d()
{
    char buf[60];
    int v = this->field_0xc;
    if (v >= 1) {
        float *m = (float *)buf;
        *(short *)(this->field_0x8 + 0x28) = (short)(v * 6);
        m[0] = 1.0f;
        m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
        m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
        m[14] = 1.0f;
        paintcanvas_ext_set_wvm2(this, buf);
        paintcanvas_ext_meshdraw(this->field_0x34, this->field_0x8);
    }
}

extern "C" void paintcanvas_ext_texcreate6(unsigned short, unsigned int &, void *, void *,
                                           unsigned int &, bool);

void TextureCreate(unsigned short w, unsigned int &ref, bool b)
{
    return paintcanvas_ext_texcreate6(w, ref, 0, 0, *(unsigned int *)&b, b);
}

void PaintCanvas::MeshChangeMaterialIntern(char *transform, void *material)
{
    if (transform && material) {
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x3c); ++i) {
            void *m = (*(void ***)(transform + 0x40))[i];
            paintcanvas_ext_mesh_changemat(this, m, material);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x4c); ++i) {
            void *c = (*(void ***)(transform + 0x50))[i];
            paintcanvas_ext_transform_changemat(this, c, material);
        }
    }
}

int PaintCanvas::GetTextHeight(unsigned int index)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
        return paintcanvas_ext_text_height(font);
    }
    return 0;
}

__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b4;
__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b8;

void PaintCanvas::Initialize(bool param_1)
{
    this->field_0x30 = param_1 ? 2 : 0;
    paintcanvas_ext_init_setorientation(this->field_0x34);

    // zero out the four 4-float blocks at 0x78, 0x88, 0x98, 0xa8
    memset(&this->projOrthoMatrix.m[0], 0, 0x10);
    memset(&this->projOrthoMatrix.m[4], 0, 0x10);
    memset(&this->projOrthoMatrix.m[8], 0, 0x10);
    memset(&this->projOrthoMatrix.m[12], 0, 0x10);

    int orient = this->field_0x30;
    int w = paintcanvas_ext_init_dispwidth(this->field_0x34);
    float fw = paintcanvas_ext_init_signedtofloat(w, 0);
    int h = paintcanvas_ext_init_dispheight(this->field_0x34);
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
    if (index < this->field_0x24) {
        void *mesh = (this->field_0x28)[index];
        return paintcanvas_ext_draw_mesh(this->field_0x34, mesh);
    }
}

int PaintCanvas::MeshGetTriCount(char *mesh)
{
    if (mesh) {
        int tri;
        if (*(void **)(mesh + 0x34) == 0) {
            tri = 0;
        } else {
            tri = paintcanvas_ext_transform_tricount(this, *(void **)(mesh + 0x34));
        }
        int q = (int)__aeabi_uidiv(*(unsigned short *)(mesh + 0x28), 3);
        return q + tri;
    }
    return 0;
}

int PaintCanvas::FontGetYOffset(unsigned int index)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
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

void PaintCanvas::SetProjectionMatrix3d(float param_1, float param_2,
                           float param_3)
{

    // stash parameters into the engine globals
    *g_spm_p0_7b826 = param_3;
    *g_spm_p1_7b82a = param_2;
    *g_spm_p4_7b830 = param_1;
    *g_spm_p2_7b82c = param_2;
    *g_spm_p3_7b82e = param_3;

    int w = paintcanvas_ext_spm_dispwidth(this->field_0x34);
    int h = paintcanvas_ext_spm_dispheight(this->field_0x34);
    float half = param_1 * 0.5f;
    float s = paintcanvas_ext_spm_sinf(half);
    float c = paintcanvas_ext_spm_cosf(half);
    float fw = paintcanvas_ext_spm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_spm_signedtofloat(h, 0);

    // zero the four matrix rows at 0x38,0x48,0x58,0x68
    memset(&this->projMatrix3d.m[12], 0, 0x10);
    memset(&this->projMatrix3d.m[8], 0, 0x10);
    memset(&this->projMatrix3d.m[4], 0, 0x10);
    memset(&this->projMatrix3d.m[0], 0, 0x10);

    if (this->field_0x30 <= 3) {
        float aspect = fw / fh;
        float f = 1.0f / (s / c);
        switch (this->field_0x30) {
        case 0:
            this->projMatrix3d.m[4] = f / aspect;
            this->projMatrix3d.m[1] = -f;
            break;
        case 1:
            this->projMatrix3d.m[4] = -(f / aspect);
            this->projMatrix3d.m[1] = -f;
            break;
        case 2:
            this->projMatrix3d.m[5] = -f;
            this->projMatrix3d.m[0] = f / aspect;
            break;
        case 3:
            this->projMatrix3d.m[5] = -f;
            this->projMatrix3d.m[0] = -(f / aspect);
            break;
        }
    }

    this->projMatrix3d.m[11] = g_spm_const_7b950;
    this->projMatrix3d.m[10] = (param_2 + param_3) / (param_2 - param_3);
    this->projMatrix3d.m[14] = ((param_3 + param_3) * param_2) / (param_2 - param_3);
}

__attribute__((visibility("hidden"))) extern const double g_dss1_gravscale_8ac10;

void PaintCanvas::DrawSpriteSystem(unsigned int param_1)
{
    if (param_1 >= this->field_0x180) {
        return;
    }
    if (((void **)this->field_0x184)[param_1] == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f;
    worldM[5] = 1.0f;
    worldM[14] = 1.0f;

    if (this->field_0x170 < this->field_0x164) {
        if (this->field_0x0 == 0) {
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

            void *grav = paintcanvas_ext_dss1_getgrav(this->field_0x34);
            double angle = *(double *)((char *)grav + 8) * g_dss1_gravscale_8ac10;
            float a = (float)angle;
            int orient = this->field_0x30;
            float rot = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dss1_sinf(rot);
            float c = paintcanvas_ext_dss1_cosf(rot);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            // copy the active camera's view matrix (at cam+0xc) into scratch
            char *cam = ((char **)this->field_0x168)[this->field_0x170];
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
    paintcanvas_ext_dss1_ssdraw(this->field_0x34, ident, worldM,
                                ((void **)this->field_0x184)[param_1]);
}

float PaintCanvas::MeshSetPoint(unsigned int index, unsigned short vtx,
                   float x, float y, float z)
{
    if (index < this->field_0x24) {
        char *mesh = (this->field_0x28)[index];
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

extern "C" int paintcanvas_ext_gl_textwidth(void *self, unsigned int font, void *str,
                                            unsigned int begin, unsigned int end);

void PaintCanvas::GetLine(unsigned int param_1, void *param_3, int param_4,
             void *param_5)
{
    char tmp[16];           // String scratch
    unsigned int lastSpace = 0;
    int width = 5;
    unsigned int i = 0;
    unsigned int len = *(unsigned int *)((char *)param_3 + 8);

    while (i < len) {
        unsigned short *ch = paintcanvas_ext_gl_strindex(param_3, i);
        unsigned short c = *ch;
        unsigned int next = i + 1;
        width += paintcanvas_ext_gl_textwidth(this, param_1, param_3, i, next);
        if (c == 0x20) {
            lastSpace = i;
        }
        if (width >= param_4) {
            // break out: word-wrap path
            if ((int)lastSpace < 1) {
                paintcanvas_ext_gl_substr(tmp, param_3, 0, next);
            } else {
                paintcanvas_ext_gl_substr(tmp, param_3, 0, lastSpace + 1);
            }
            paintcanvas_ext_gl_str_assign(param_5, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
        unsigned short *ch2 = paintcanvas_ext_gl_strindex(param_3, i);
        if (*ch2 == 0xa) {
            paintcanvas_ext_gl_substr(tmp, param_3, 0, next);
            paintcanvas_ext_gl_str_assign(param_5, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
        unsigned short *ch3 = paintcanvas_ext_gl_strindex(param_3, i);
        i = next;
        if (*ch3 == 0xd) {
            paintcanvas_ext_gl_substr(tmp, param_3, 0, next);
            paintcanvas_ext_gl_str_assign(param_5, tmp);
            paintcanvas_ext_gl_str_dtor(tmp);
            return;
        }
    }

    if ((int)len < 2) {
        paintcanvas_ext_gl_str_fromchar(tmp, g_getline_empty_7c428, false);
    } else {
        paintcanvas_ext_gl_substr(tmp, param_3, 0, len);
    }
    paintcanvas_ext_gl_str_assign(param_5, tmp);
    paintcanvas_ext_gl_str_dtor(tmp);
}

__attribute__((visibility("hidden"))) extern char *const g_dl_flag_794ee;

void PaintCanvas::DrawLine(int param_1, int param_2, int param_3, int param_4)
{
    char abuf[60];

    float x1 = paintcanvas_ext_dl_signedtofloat(param_1 + 1, 0);
    float y1 = paintcanvas_ext_dl_signedtofloat(param_2, 0);
    float x2 = paintcanvas_ext_dl_signedtofloat(param_3 + 1, 0);
    float y2 = paintcanvas_ext_dl_signedtofloat(param_4, 0);

    float *m = (float *)abuf;
    memset(m, 0, 60);
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[14] = 1.0f;

    float *v = this->lineVerts;

    if (*g_dl_flag_794ee == 0) {
        paintcanvas_ext_dl_glLineWidth(1.0f);
        paintcanvas_ext_dl_glEnable(this->field_0x34, true);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_setwvm(this, abuf);
        paintcanvas_ext_dl_glVertexPointer(2, 0x1406, 0, this->lineVerts);
        paintcanvas_ext_dl_glColorMask(this->field_0x34, 0x8074, 1);
        paintcanvas_ext_dl_glColorMask(this->field_0x34, 0x8078, 0);
        paintcanvas_ext_dl_glColorMask(this->field_0x34, 0x8075, 0);
        paintcanvas_ext_dl_glColorMask(this->field_0x34, 0x8076, 0);
        paintcanvas_ext_dl_glDrawArrays(1, 0, 2);
        paintcanvas_ext_dl_glEnable(this->field_0x34, true);
    } else {
        paintcanvas_ext_dl_setwvm(this, abuf);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_drawline2d(this->field_0x34, this->lineVerts, true);
    }
}

void PaintCanvas::MeshChangeMaterial(unsigned int meshIndex, unsigned int matIndex)
{
    if (matIndex < this->field_0x174 &&
        meshIndex < this->field_0x24) {
        void *mesh = (this->field_0x28)[meshIndex];
        void *mat = (this->field_0x178)[matIndex];
        return paintcanvas_ext_change_mat(this, mesh, mat);
    }
}

void PaintCanvas::CameraSetPerspective(unsigned int index, float a, float b, float c)
{
    if (index < this->field_0x164) {
        float w = (float)paintcanvas_ext_get_w(this);
        float h = (float)paintcanvas_ext_get_h(this);
        void *cam = (this->field_0x168)[index];
        paintcanvas_ext_cam_persp4(a, b, c, w, h, cam);
        if (this->field_0x170 == index) {
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

void PaintCanvas::MeshCloneMaterial(unsigned int index, unsigned int *out)
{
    int result;
    if (index < this->field_0x24) {
        char *obj = (char *)paintcanvas_ext_alloc(0x74);
        char *mesh = (this->field_0x28)[index];
        paintcanvas_ext_material_clone(obj, *(void **)(mesh + 0x30));
        paintcanvas_ext_material_add(obj, &this->field_0x174);
        result = (int)this->field_0x174 - 1;
    } else {
        result = -1;
    }
    *out = result;
}

void PaintCanvas::GetGravValue()
{
    return paintcanvas_ext_get_grav(this->field_0x34);
}

#define HIDDEN __attribute__((visibility("hidden")))

extern char paintcanvas_g_bg_a;
extern char paintcanvas_g_bg_b;

void PaintCanvas::BeginBG()
{
    *(unsigned char *)((char *)this->field_0x34 + 0xfd) = 0;
    paintcanvas_ext_gl_enable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_color(this->field_0x34, 1.0f, 1.0f, 1.0f, 1.0f);
    this->field_0x1f0 = paintcanvas_g_bg_a;
    char flag = paintcanvas_g_bg_b;
    paintcanvas_g_bg_a = 0;
    if (flag != 0) {
        return paintcanvas_ext_matgl_load(this->field_0x34, &this->projMatrix3d.m[0]);
    }
    paintcanvas_ext_glMatrixMode(0x1702);
    paintcanvas_ext_gl_loadidentity();
    paintcanvas_ext_gl_ortho_persp(1.0f, 0.3f, 1.0f);
    paintcanvas_ext_glMatrixMode(0x1701);
    paintcanvas_ext_gl_loadmatrix(&this->projMatrix3d.m[0]);
    paintcanvas_ext_glMatrixMode(0x1700);
    return paintcanvas_ext_gl_done();
}

namespace AbyssEngine {
}

void PaintCanvas::FontCreate(unsigned short param_1, unsigned int *param_2,
                bool param_3)
{
    char *res = (char *)paintcanvas_ext_fc_findres(this, param_1);
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
        *param_2 = *(unsigned int *)(res + 8);
        return;
    }
    void *font = 0;
    char *texpath = (char *)**(char ***)(texres + 0xc);
    int ok = paintcanvas_ext_fc_fontfromfile(this->field_0x34, texpath, info[1], &font);
    if (ok != 1) {
        return;
    }
    if (*(int *)(texres + 8) != -1) {
        *(int *)font = *(int *)(texres + 8);
    }
    PCArrayAdd<AbyssEngine::ImageFont *>((AbyssEngine::ImageFont *)font, &this->field_0x140);
    int idx = this->field_0x140 - 1;
    *(int *)(res + 8) = idx;
    *param_2 = idx;

    Engine *eng = (Engine *)this->field_0x34;
    int cur = eng->field_0x78;
    if (cur == -1) {
        eng->field_0x78 = idx;
    } else {
        char *curFont = (char *)(this->field_0x144)[cur];
        if (*(unsigned short *)curFont <= *(unsigned short *)font) {
            int curH = paintcanvas_ext_fc_fontheight(curFont);
            int newH = paintcanvas_ext_fc_fontheight(font);
            if (newH < curH) {
                *(int *)((char *)this->field_0x34 + 0x78) = *param_2;
            }
        }
    }
}

void PaintCanvas::SetResourceList(void **list, unsigned int count)
{
    return paintcanvas_ext_set_reslist(list, count, &this->field_0x134);
}

void PaintCanvas::MaterialResourceChangeTexture(unsigned short resId,
                                   void *texture, unsigned int slot)
{
    if (slot <= 7) {
        char *r = paintcanvas_ext_find_res(this, resId);
        if (r) {
            unsigned int matIdx = *(unsigned int *)(r + 0x8);
            if (matIdx + 1 != 0 && matIdx < this->field_0x174) {
                char *mat = (char *)(this->field_0x178)[matIdx];
                *(void **)(mat + slot * 4) = texture;
            }
        }
    }
}

void PaintCanvas::TransformAddChild(unsigned int parent, unsigned int child)
{
    unsigned int count = this->field_0x158;
    if (parent != child && child < count && parent < count) {
        char **arr = this->field_0x15c;
        char *p = arr[parent];
        char *c = arr[child];
        PCArrayAdd<void *>(c, p + 0x4c);
        char **arr2 = this->field_0x15c;
        char *p2 = arr2[parent];
        char *c2 = arr2[child];
        paintcanvas_ext_child_link(p2 + 0xd4, c2, p2);
        char **arr3 = this->field_0x15c;
        return paintcanvas_ext_transform_dirty(arr3[parent]);
    }
}

// AbyssEngine::PaintCanvas::TextureCreateGlobal(String name, unsigned unit) — loads a texture
// file into the given texture unit and, on success, leaves it unbound on that unit.
extern "C" int tcg_TextureCreateFromFile(void *engine, const char *path, void *cb, void *ud,
                                         unsigned *outId, bool b, float f);

__attribute__((visibility("hidden"))) extern int **g_tcg_canary;

void PaintCanvas::TextureCreateGlobal(AbyssEngine::String *name, int unit)
{
    int *canary = *g_tcg_canary;
    int saved = *canary;

    char *path = ((String *)name)->GetAEChar();
    unsigned outId;
    int rc = tcg_TextureCreateFromFile(this->field_0x34, path, 0, 0, &outId, false,
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
    *(unsigned char *)((char *)this->field_0x34 + 0xfd) = 0;
    paintcanvas_ext_gl_disable(0xb71);
    paintcanvas_ext_gl_depthmask(1);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_color(this->field_0x34, 1.0f, 1.0f, 1.0f, 1.0f);
    if (paintcanvas_g_use_matgl != 0) {
        return paintcanvas_ext_matgl_load(this->field_0x34, &this->projMatrix3d.m[0]);
    }
    paintcanvas_ext_glMatrixMode(0x1702);
    paintcanvas_ext_gl_loadidentity();
    paintcanvas_ext_gl_ortho_persp(1.0f, 0.3f, 1.0f);
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
    if (transformIndex < this->field_0x158) {
        void *t = (this->field_0x15c)[transformIndex];
        return paintcanvas_ext_transform_addmesh(this, t, meshId, b);
    }
}

// AbyssEngine::PaintCanvas::GetReverseString(String out, _, String in, bool reverse)
// When reverse is false, returns a copy of `in`; otherwise concatenates `in`'s characters in
// reverse order (one SubString per char) into a fresh String.
using AbyssEngine::String;

__attribute__((visibility("hidden"))) extern int **g_grs_canary;
__attribute__((visibility("hidden"))) extern const char g_grs_empty[];

void GetReverseString(AbyssEngine::String *out, unsigned int /*r1*/, AbyssEngine::String *in,
                      int reverse)
{
    int *canary = *g_grs_canary;
    int saved = *canary;

    if (reverse == 0) {
        ((String *)out)->ctor_copy((String *)in, false);
    } else {
        ((String *)out)->ctor_char(g_grs_empty, false);
        int i = (int)in->size();
        while (--i >= 0) {
            String piece;
            ((String *)&piece)->SubString((String *)in, (unsigned)i, (unsigned)(i + 1));
            ((String *)out)->addAssign_str((String *)&piece);
        }
    }

    
}

namespace AbyssEngine {
}

void PaintCanvas::SpriteSystemCreate(unsigned short param_1, bool param_2,
                        unsigned short param_3, unsigned int *param_4)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_ss2_sscreate(this->field_0x34, param_1, param_2, &ss);
    if (ok == 1) {
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_ss2_matcreate(this, param_3, &mat);
        if (mat <= this->field_0x174) {
            AbyssEngine::Node *node = *(AbyssEngine::Node **)((char *)ss + 0x10);
            node->field_0x30 =
                *(unsigned int *)((char *)this->field_0x178 + mat * 4);
        }
        unsigned int i;
        for (i = 0; i < this->field_0x180; i++) {
            void **slot = (void **)(this->field_0x184 + i * 4);
            if (*(int *)slot == 0) {
                *slot = ss;
                ss = 0;
                *param_4 = i;
                return;
            }
        }
        if (ss == 0) {
            return;
        }
        PCArrayAdd<AbyssEngine::SpriteSystem *>((AbyssEngine::SpriteSystem *)ss,
                                          &this->field_0x180);
        result = this->field_0x180 - 1;
    } else {
        result = 0xffffffff;
    }
    *param_4 = result;
}

void PaintCanvas::GetScreenPosition(char *param_1, char *param_2)
{
    if (this->field_0x170 >= this->field_0x164) {
        return;
    }

    // copy {m[0xc], m[0x1c], m[0x2c]} into the output vector
    float src[3];
    src[0] = *(float *)(param_1 + 0xc);
    src[1] = *(float *)(param_1 + 0x1c);
    src[2] = *(float *)(param_1 + 0x2c);
    paintcanvas_ext_gsp_vec_assign(param_2, src);

    char *cam = ((char **)this->field_0x168)[this->field_0x170];
    float z = *(float *)(param_2 + 8);
    float denomX = *(float *)(cam + 0x4c) * z;
    if (denomX == 0.0f) {
        return;
    }
    float denomY = z * *(float *)(cam + 0x48);
    if (denomY == 0.0f) {
        return;
    }

    float px = *(float *)param_2;
    int w0 = paintcanvas_ext_gsp_getwidth(this);
    int w1 = paintcanvas_ext_gsp_getwidth(this);
    double fw = (double)paintcanvas_ext_gsp_signedtofloat(w0, 0);
    double termY = ((double)*(float *)(param_2 + 4) * 0.5) / (double)denomY;
    double halfW = (double)paintcanvas_ext_gsp_signedtofloat(w1 >> 1, 0);
    *(float *)param_2 = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp_getheight(this);
    double fh = (double)paintcanvas_ext_gsp_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp_getheight(this);
    double halfH = (double)paintcanvas_ext_gsp_signedtofloat(h1 >> 1, 0);
    *(float *)(param_2 + 4) = (float)(halfH + termY * fh);

    // remaining clamp/visibility checks have no observable side-effects on output
    // (they compute booleans only); reproduce the visible reads to keep behavior.
    char *cam2 = ((char **)this->field_0x168)[this->field_0x170];
    if (*(float *)(param_2 + 8) <= *(float *)(cam2 + 4)) {
        float fy = *(float *)(param_2 + 4);
        if (fy >= 0.0f) {
            float fx = *(float *)param_2;
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

void PaintCanvas::SpriteSystemCreate(unsigned short param_1,
                        bool param_2, unsigned int *param_3)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_sscreate(this->field_0x34, param_1, param_2, &ss);
    if (ok == 1) {
        unsigned int i;
        for (i = 0; i < this->field_0x180; i++) {
            void **slot = (void **)(this->field_0x184 + i * 4);
            if (*(int *)slot == 0) {
                *slot = ss;
                ss = 0;
                *param_3 = i;
                return;
            }
        }
        if (ss == 0) {
            return;
        }
        PCArrayAdd<AbyssEngine::SpriteSystem *>((AbyssEngine::SpriteSystem *)ss,
                                          &this->field_0x180);
        result = this->field_0x180 - 1;
    } else {
        result = 0xffffffff;
    }
    *param_3 = result;
}

void PaintCanvas::MaterialCreate(unsigned int *out, void *p2, void *p3)
{
    char *obj = (char *)paintcanvas_ext_alloc(0x74);
    paintcanvas_ext_material_ctor(obj);
    *(void **)(obj + 0x0) = p3;
    *(void **)(obj + 0x20) = p2;
    paintcanvas_ext_material_add(obj, &this->field_0x174);
    *out = this->field_0x174 - 1;
}

__attribute__((visibility("hidden"))) extern const double g_cisvf_gravscale_7bcd8;

int PaintCanvas::CameraIsSphereinViewFrustum(void *param_1, float param_2)
{
    if (param_2 == 0.0f ||
        this->field_0x164 <= this->field_0x170) {
        return 1;
    }

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[14] = 1.0f;
    char scratch[60];
    paintcanvas_ext_cisvf_matidentity(scratch, m);

    void *grav = paintcanvas_ext_cisvf_getgrav(this->field_0x34);
    double angle = *(double *)((char *)grav + 8) * g_cisvf_gravscale_7bcd8;
    float a = (float)angle;
    int orient = this->field_0x30;
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cisvf_sinf(rot);
    float c = paintcanvas_ext_cisvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = ((void **)this->field_0x168)[this->field_0x170];
    return paintcanvas_ext_cisvf_inner(param_1, param_2, m, cam);
}

void PaintCanvas::MeshChangeShaderAnimValue(char *mesh, float value, unsigned int mode)
{
    if (!mesh) {
        return;
    }
    float *p;
    if (mode == 4) {
        p = (float *)(mesh + 0x24);
    } else if (mode == 2) {
        p = (float *)(mesh + 0x20);
    } else {
        if (mode != 1) {
            goto tail;
        }
        p = (float *)(mesh + 0x1c);
    }
    *p = value;
tail:
    return paintcanvas_ext_shader_anim(this, *(void **)(mesh + 0x34));
}

__attribute__((visibility("hidden"))) extern int *const g_resume_curtex_7e828;

extern "C" int paintcanvas_ext_rs_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);

void PaintCanvas::Resume()
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < this->field_0x10; i++) {
        char *res = (this->field_0x14)[i];
        char *path = paintcanvas_ext_rs_getAEChar(res + 4);
        float f = *(float *)((this->field_0x14)[i] + 0x10);
        int ok = paintcanvas_ext_rs_texfromfile(this->field_0x34, path, 0, 0,
                                                &out, false, f);
        if (ok == 1) {
            *(int *)((this->field_0x14)[i]) = 0;
        }
        paintcanvas_ext_rs_deletearr(path);
    }
    int *cur = g_resume_curtex_7e828;
    if (*cur != 0) {
        paintcanvas_ext_rs_glActiveTexture(0x84c7);
        char *res = (this->field_0x14)[*cur];
        paintcanvas_ext_rs_glBindTexture(0x8513, *(unsigned int *)res);
        paintcanvas_ext_rs_glActiveTexture(0x84c0);
    }
}

namespace AbyssEngine {
}

void PaintCanvas::TransformAddMeshId(unsigned int param_1, unsigned int param_2)
{
    if (param_1 >= this->field_0x158 ||
        param_2 >= this->field_0x24) {
        return;
    }
    char *tf = (this->field_0x15c)[param_1];
    char *mesh = (this->field_0x28)[param_2];
    PCArrayAdd<AbyssEngine::Mesh *>(*(AbyssEngine::Mesh **)(mesh), tf + 0x3c);
    paintcanvas_ext_tami_bsphere_merge(tf + 0xd4, mesh + 0x3c);

    void *res = *(void **)(mesh + 0x34);
    if (res != 0) {
        long long resLen = *(long long *)((char *)res + 0xf8);
        long long tfLen = *(long long *)(tf + 0xf8);
        if (tfLen < resLen) {
            paintcanvas_ext_tami_setanimlen(tf, (int)(resLen >> 32), (int)resLen);
            tf = (this->field_0x15c)[param_1];
        }
        long long tfStart = *(long long *)(tf + 0x100);
        void *res2 = *(void **)(mesh + 0x34);
        long long resStart = *(long long *)((char *)res2 + 0x100);
        if (tfStart == 0 || resStart < tfStart) {
            *(long long *)(tf + 0x100) = resStart;
        }
        paintcanvas_ext_tami_setanimstate(tf, 2, 0);
    }
    paintcanvas_ext_tami_finalize((this->field_0x15c)[param_1]);
}

// AbyssEngine::PaintCanvas::GetReverseString(String in) — convenience overload that forwards to
// the (String,bool) overload, choosing the reverse flag from a runtime layout-direction byte at
// param2+0x1c (RTL when zero).
using AbyssEngine::String;

__attribute__((visibility("hidden"))) extern int **g_grs2_canary;

void GetReverseString(AbyssEngine::String *out, int param2, AbyssEngine::String *in)
{
    int *canary = *g_grs2_canary;
    int saved = *canary;

    String tmp;
    ((String *)&tmp)->ctor_copy((String *)in, false);
    grs2_GetReverseString_impl(out, 0, &tmp, *(char *)((char *)(unsigned long)param2 + 0x1c) == 0);
    ((String *)&tmp)->dtor();

    
}

void PaintCanvas::GetAccelValue()
{
    return paintcanvas_ext_get_accel(this->field_0x34);
}

__attribute__((visibility("hidden"))) extern float *const g_rpm_fov_8d0dc;
__attribute__((visibility("hidden"))) extern const unsigned int g_rpm_const_8d234;
__attribute__((visibility("hidden"))) extern float *const g_rpm_near_8d196;
__attribute__((visibility("hidden"))) extern float *const g_rpm_far_8d1a4;
__attribute__((visibility("hidden"))) extern char *const g_rpm_flag_8d1b8;

void PaintCanvas::ResetPersMatrix()
{
    int w = paintcanvas_ext_rpm_dispwidth(this->field_0x34);
    int h = paintcanvas_ext_rpm_dispheight(this->field_0x34);
    float fov = *g_rpm_fov_8d0dc;
    float s = paintcanvas_ext_rpm_sinf(fov * 0.5f);
    float c = paintcanvas_ext_rpm_cosf(fov * 0.5f);
    float fw = paintcanvas_ext_rpm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_rpm_signedtofloat(h, 0);

    memset(&this->projMatrix3d.m[12], 0, 0x10);
    memset(&this->projMatrix3d.m[0], 0, 0x10);
    memset(&this->projMatrix3d.m[8], 0, 0x10);
    memset(&this->projMatrix3d.m[4], 0, 0x10);

    if (this->field_0x30 <= 3) {
        float aspect = fw / fh;
        float f = 1.0f / (s / c);
        switch (this->field_0x30) {
        case 0:
            this->projMatrix3d.m[4] = f / aspect;
            this->projMatrix3d.m[1] = -f;
            break;
        case 1:
            this->projMatrix3d.m[4] = -(f / aspect);
            this->projMatrix3d.m[1] = -f;
            break;
        case 2:
            this->projMatrix3d.m[5] = -f;
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
        paintcanvas_ext_rpm_loadproj(this->field_0x34, &this->projMatrix3d.m[0]);
    }
}

__attribute__((visibility("hidden"))) extern const double g_cipvf_gravscale_7bba8;

int PaintCanvas::CameraIsPointinViewFrustum(void *param_1)
{
    if (this->field_0x170 >= this->field_0x164) {
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

    void *grav = paintcanvas_ext_cipvf_getgrav(this->field_0x34);
    double angle = *(double *)((char *)grav + 8) * g_cipvf_gravscale_7bba8;
    float a = (float)angle;
    int orient = this->field_0x30;
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cipvf_sinf(rot);
    float c = paintcanvas_ext_cipvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = ((void **)this->field_0x168)[this->field_0x170];
    return paintcanvas_ext_cipvf_inner(param_1, m, cam);
}

void PaintCanvas::SetTexture(unsigned int, unsigned int)
{
    return paintcanvas_ext_set_texture(this->field_0x34);
}

PaintCanvas::~PaintCanvas()
{
    paintcanvas_ext_dtor_releaseall(this);

    // Walk the resource list (count at 0x134, array at 0x138), releasing each.
    for (unsigned int i = 0; i < this->field_0x134; i++) {
        void *res = (this->field_0x138)[i];
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
            void *cell = (this->field_0x138)[i];
            if (cell != 0) {
                paintcanvas_ext_dtor_op_delete(cell);
            }
            (this->field_0x138)[i] = 0;

            // glow scratch arrays released per-iteration in the original
            PCArrayReleaseClasses(&this->glowMeshes_count);
            PCArrayDtor(&this->glowMatA_count);
            PCArrayDtor(&this->glowMatB_count);
            PCArrayReleaseClasses(&this->glowUints_count);
            PCArrayDtor(&this->glowMatC_count);
        }
    }

    paintcanvas_ext_dtor_meshrelease(this->field_0x34, &this->field_0x8);
    paintcanvas_ext_dtor_meshrelease(this->field_0x34, &this->field_0x1c8);

    // Release the loaded-texture name strings (count at 0x10, array at 0x14).
    for (unsigned int i = 0; i < this->field_0x10; i++) {
        void *tex = (this->field_0x14)[i];
        if (tex != 0) {
            paintcanvas_ext_dtor_str_dtor((char *)tex + 4);
            paintcanvas_ext_dtor_op_delete(tex);
        }
        (this->field_0x14)[i] = 0;
    }

    PCArrayDtor(&this->glowMatC_count);
    PCArrayDtor(&this->glowUints_count);
    PCArrayDtor(&this->glowMatB_count);
    PCArrayDtor(&this->glowMatA_count);
    PCArrayDtor(&this->glowMeshes_count);
    PCArrayDtor(&this->field_0x180);
    PCArrayDtor(&this->field_0x174);
    PCArrayDtor(&this->field_0x164);
    PCArrayDtor(&this->field_0x158);
    PCArrayDtor(&this->field_0x14c);
    PCArrayDtor(&this->field_0x140);
    PCArrayDtor(&this->field_0x134);
    PCArrayDtor(&this->field_0x24);
    PCArrayDtor(&this->field_0x10);
}

namespace AbyssEngine {
}

void PaintCanvas::Image2DCreate(unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_i2d_findres(this, param_1);
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
        int ok = paintcanvas_ext_i2d_imgregion(this->field_0x34, texpath,
                                               info[1], img);
        if (ok != 1) {
            return;
        }
        if (*(int *)(texres + 8) != -1) {
            *(int *)(img + 4) = *(int *)(texres + 8);
        }
        PCArrayAdd<AbyssEngine::Image2D *>((AbyssEngine::Image2D *)img, &this->field_0x14c);
        idx = this->field_0x14c - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    *param_2 = idx;
}

__attribute__((visibility("hidden"))) extern const double g_gsp2_gravscale_8bfa8;

void PaintCanvas::GetScreenPosition(void *param_1, void *param_2, char *param_3)
{

    // first transform the input world position
    char transformed[16];
    paintcanvas_ext_gsp2_transformvec(transformed, param_1);

    if (this->field_0x170 >= this->field_0x164) {
        return;
    }

    char invMat[60];
    void *cam = ((void **)this->field_0x168)[this->field_0x170];
    if (this->field_0x0 == 0) {
        paintcanvas_ext_gsp2_invtransformvec(invMat, (char *)cam + 0xc);
        paintcanvas_ext_gsp2_vec_assign(param_3, invMat);
    } else {
        float m[16];
        char scratch[60];
        memset(m, 0, sizeof(m));
        m[0] = 1.0f; m[5] = 1.0f; m[14] = 1.0f;
        paintcanvas_ext_gsp2_matidentity(scratch, m);

        void *grav = paintcanvas_ext_gsp2_getgrav(this->field_0x34);
        double angle = *(double *)((char *)grav + 8) * g_gsp2_gravscale_8bfa8;
        float a = (float)angle;
        int orient = this->field_0x30;
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
        paintcanvas_ext_gsp2_vec_assign(param_3, invMat);
    }

    float z = *(float *)(param_3 + 8);
    char *cam2 = ((char **)this->field_0x168)[this->field_0x170];
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

    float px = *(float *)param_3;
    int w0 = paintcanvas_ext_gsp2_getwidth(this);
    int w1 = paintcanvas_ext_gsp2_getwidth(this);
    float py = *(float *)(param_3 + 4);
    double fw = (double)paintcanvas_ext_gsp2_signedtofloat(w0, 0);
    double halfW = (double)paintcanvas_ext_gsp2_signedtofloat(w1 >> 1, 0);
    *(float *)param_3 = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp2_getheight(this);
    double fh = (double)paintcanvas_ext_gsp2_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp2_getheight(this);
    double halfH = (double)paintcanvas_ext_gsp2_signedtofloat(h1 >> 1, 0);
    *(float *)(param_3 + 4) = (float)(halfH + (((double)py * 0.5) / (double)denomY) * fh);

    // remaining bound checks compute booleans only (no observable output)
    float nx = *(float *)param_3;
    if (nx >= 0.0f) {
        float ny = *(float *)(param_3 + 4);
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

int PaintCanvas::ResourceLoaded(unsigned int index, unsigned int type)
{
    unsigned int count;
    switch (type) {
    case 1: {
        char *res = (this->field_0x138)[index];
        if (*(int *)(res + 0x4) == 2) {
            int handle = *(int *)(res + 0x8);
            return handle + 1 != 0 ? 1 : 0;
        }
        return 0;
    }
    case 2:
        count = this->field_0x140;
        break;
    case 3:
        count = this->field_0x14c;
        break;
    case 4:
        count = this->field_0x24;
        break;
    case 5:
        count = this->field_0x158;
        break;
    case 6:
        count = this->field_0x174;
        break;
    default:
        return 0;
    }
    return index < count ? 1 : 0;
}

int PaintCanvas::TransformGetTriCount(unsigned int index)
{
    if (index < this->field_0x158) {
        void *t = (this->field_0x15c)[index];
        return paintcanvas_ext_transform_tricount(this, t);
    }
    return 0;
}

float PaintCanvas::SpriteSystemSetPosition(unsigned int index, unsigned short sub,
                              float x, float y, float z)
{
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
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
    if (index < this->field_0x14c) {
        char *img = (this->field_0x150)[index];
        if (*(unsigned char *)(img + 0x14)) {
            paintcanvas_ext_di_restore(*(unsigned char *)(img + 0x14), img);
            img = (this->field_0x150)[index];
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
        paintcanvas_ext_di_meshdraw(this->field_0x34,
                                    *(void **)((this->field_0x150)[index]));
        paintcanvas_ext_di_glenable(0xb44);
    }
}

// The Matrix member at +0xf8 is a single Matrix object, not an Array (kept as an extern ctor).
extern "C" void paintcanvas_ctor_matrix(void *);     // 0x6f118  f8
extern "C" void paintcanvas_ext_meshcreate5(void *, unsigned short, unsigned short,
                                            signed char, void *);

PaintCanvas::PaintCanvas(AbyssEngine::Engine *engine)
{
    PCArrayCtor(&this->field_0x10);   // loaded-texture-name list
    PCArrayCtor(&this->field_0x24);   // meshes
    paintcanvas_ctor_matrix(&this->identityMatrix);
    PCArrayCtor(&this->field_0x134);  // resources
    PCArrayCtor(&this->field_0x140);  // fonts
    PCArrayCtor(&this->field_0x14c);  // images
    PCArrayCtor(&this->field_0x158);  // transforms
    PCArrayCtor(&this->field_0x164);  // cameras
    PCArrayCtor(&this->field_0x174);  // materials
    PCArrayCtor(&this->field_0x180);  // sprite systems
    PCArrayCtor(&this->glowMeshes_count);  // glow meshes
    PCArrayCtor(&this->glowMatA_count);  // glow matrices
    PCArrayCtor(&this->glowMatB_count);  // glow matrices
    PCArrayCtor(&this->glowUints_count);  // glow uints
    PCArrayCtor(&this->glowMatC_count);  // glow matrices

    this->field_0x1f1 = 0;
    this->field_0x20 = 0;
    this->field_0x1f4 = 1;
    this->field_0x4 = 0;
    this->field_0x0 = 0;
    this->field_0x34 = engine;
    this->field_0x170 = 0xffffffff;

    paintcanvas_ext_meshcreate5(engine, 4, 2, 0x11, &this->field_0x1c8);

    int *p = *(int **)(this->field_0x1c8 + 0x2c);
    p[0] = 0x20000;
    p[1] = 1;
    p[2] = *(int *)0x87878;
    this->field_0x1fc = 1.0f;
    this->field_0x200 = 1.0f;
    this->field_0x204 = 1.0f;
    this->field_0x208 = 1.0f;
    this->field_0x1c = 1;
    engine->field_0xfc = 1;

    paintcanvas_ext_meshcreate5(engine, 400, 200, 0x1b, &this->field_0x8);

    short *buf = *(short **)(this->field_0x8 + 0x2c);
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

void PaintCanvas::TransformRemoveMesh(unsigned int transformIndex, void *mesh)
{
    if (this->field_0x158 <= transformIndex) {
        return;
    }
    char *x = paintcanvas_ext_find_mesh(this, mesh);
    if (x) {
        return paintcanvas_ext_remove_mesh(this, transformIndex, *(int *)(x + 0x8));
    }
}

void PaintCanvas::ClearBuffer(unsigned int mask)
{
    paintcanvas_ext_enable(0xb71);
    paintcanvas_ext_depthmask(1);
    return paintcanvas_ext_clear2(this->field_0x34, mask);
}

void PaintCanvas::TransformRemoveChild(unsigned int parent, unsigned int child)
{
    unsigned int count = this->field_0x158;
    if (parent != child && child < count && parent < count) {
        char **arr = this->field_0x15c;
        char *p = arr[parent];
        char *c = arr[child];
        paintcanvas_ext_array_remove(c, p + 0x4c);
        char **arr2 = this->field_0x15c;
        return paintcanvas_ext_transform_dirty(arr2[parent]);
    }
}

extern "C" int paintcanvas_ext_tc_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);
extern "C" int paintcanvas_ext_tc_texfromfileintern(void *eng, char *path, void *cb, void *ud,
                                                    unsigned int *out, float f, void *lt, bool b);

void PaintCanvas::TextureCreate(unsigned short param_1, void *param_2,
                   void *param_3, unsigned int *param_4, bool param_5)
{
    Engine *eng = (Engine *)this->field_0x34;
    eng->boundTextures[0] = -1;
    eng->boundTextures[1] = -1;

    char *res = (char *)paintcanvas_ext_tc_findres(this, param_1);
    if (res != 0) {
        unsigned int idx = *(unsigned int *)(res + 8);
        if (idx == 0xffffffff) {
            void **info = *(void ***)(res + 0xc);
            float f = (float)(int)(*(unsigned int *)((char *)info + 4));
            char *path = (char *)*info;
            int ok;
            if (!param_5) {
                ok = paintcanvas_ext_tc_texfromfileintern(this->field_0x34, path,
                                                          param_2, param_3, &idx, f, 0, false);
            } else {
                ok = paintcanvas_ext_tc_texfromfile(this->field_0x34, path,
                                                    param_2, param_3, &idx, true, f);
            }
            if (ok != 1) {
                return;
            }
            idx = 0;
            *(unsigned int *)(res + 8) = 0;
        }
        *param_4 = idx;
    }
}

// AbyssEngine::PaintCanvas::TextureCreate(unsigned short id, unsigned int* out, bool flag)
// Convenience overload (binary 0x6fbb0): forwards to the full 5-arg loader with the
// optional upload-callback / user-data pointers left null.
void PaintCanvas::TextureCreate(unsigned short id, unsigned int *out, bool flag)
{
    TextureCreate(id, (void *)0, (void *)0, out, flag);
}

// AbyssEngine::PaintCanvas::SwapBuffer() — no-op on the canvas (binary 0x8caae is empty);
// the actual buffer present is driven by AbyssEngine::Engine::SwapBuffer.
void PaintCanvas::SwapBuffer()
{
}

void PaintCanvas::StartDraw2FBO()
{
    return paintcanvas_ext_start_fbo(this->field_0x34);
}

void PaintCanvas::Suspend()
{
    char texId[4];
    for (unsigned int i = 0; i < this->field_0x10; i++) {
        int *p = (int *)(this->field_0x14)[i];
        *(int *)texId = *p;
        if (*p != -1) {
            paintcanvas_ext_gl_deletetextures(1, texId);
            p = (int *)(this->field_0x14)[i];
        }
        *p = -1;
    }
}

extern "C" void paintcanvas_ext_dt_drawmesh(void *self, void *mesh, void *m, void *m2,
                                            unsigned int flag, void *m3);

void PaintCanvas::DrawTransform(char *tf, void *m2, void *m3)
{
    char buf[60];
    if (tf && *(unsigned char *)(tf + 0xec)) {
        paintcanvas_ext_mtx_mul(buf, m2, tf);
        if (*(int *)(tf + 0x11c) != 0) {
            paintcanvas_ext_mtx_muleq(buf, tf + 0x5c);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(tf + 0x3c); i++) {
            void *mesh = (*(void ***)(tf + 0x40))[i];
            paintcanvas_ext_dt_drawmesh(this, mesh, buf, m3, *(unsigned int *)(tf + 0x48), tf + 0x98);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(tf + 0x4c); i++) {
            if (this->field_0x170 < this->field_0x164 &&
                paintcanvas_ext_dt_incamvf((*(void ***)(tf + 0x50))[i], buf,
                    (this->field_0x168)[this->field_0x170])) {
                paintcanvas_ext_dt_drawtransform_rec(this, (*(void ***)(tf + 0x50))[i], buf, m3);
            } else {
                this->field_0x4 += 1;
            }
        }
    }
}

void PaintCanvas::DrawImage2D(unsigned int param_1, int param_2, int param_3,
                 int param_4, int param_5, unsigned char param_6, unsigned char param_7,
                 unsigned char param_8)
{
    if (param_1 >= this->field_0x14c) {
        return;
    }
    char *img = ((char **)this->field_0x150)[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di4_restore(*(unsigned char *)(img + 0x14), img);
        img = ((char **)this->field_0x150)[param_1];
    }
    paintcanvas_ext_di4_settexture(this, *(unsigned int *)(img + 4));

    // horizontal base placement (param_6 low bits)
    int baseX = param_2;
    int hOff;
    if ((param_6 & 7) == 4) {
        hOff = paintcanvas_ext_di4_getwidth(this) >> 1;
    } else if ((param_6 & 7) == 2) {
        baseX = -param_2;
        hOff = paintcanvas_ext_di4_getwidth(this);
    } else {
        hOff = 0;
    }

    // horizontal flip-span (param_6 bit 3)
    int spanW = param_4;
    if (param_6 & 8) {
        spanW = paintcanvas_ext_di4_getwidth(this) - (param_4 + param_2);
    }
    float fSpanW = paintcanvas_ext_di4_signedtofloat(spanW, 0);
    char *region = *(char **)(*(char **)(((char **)this->field_0x150)[param_1]) + 4);
    float regW = *(float *)(region + 0xc);

    // vertical flip-span (param_6 bit 7)
    int spanH = param_5;
    if (param_6 & 0x80) {
        spanH = paintcanvas_ext_di4_getheight(this) - (param_5 + param_3);
        region = *(char **)(*(char **)(((char **)this->field_0x150)[param_1]) + 4);
    }
    float fSpanH = paintcanvas_ext_di4_signedtofloat(spanH, 0);
    float regH = *(float *)(region + 0x1c);

    // vertical base placement (param_6 high nibble)
    int baseY = param_3;
    int vOff;
    if ((param_6 & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di4_getheight(this) >> 1;
    } else if ((param_6 & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di4_getheight(this);
        baseY = -param_3;
    } else {
        vOff = 0;
    }

    // anchor offset X (param_7 low bits)
    int anchorX;
    if ((param_7 & 7) == 4) {
        if ((param_6 & 8) == 0) {
            anchorX = -(param_4 >> 1);
        } else {
            anchorX = ((param_4 + param_2) - paintcanvas_ext_di4_getwidth(this)) >> 1;
        }
    } else if ((param_7 & 7) == 2) {
        if ((param_6 & 8) == 0) {
            anchorX = -param_4;
        } else {
            anchorX = (param_4 + param_2) - paintcanvas_ext_di4_getwidth(this);
        }
    } else {
        anchorX = 0;
    }

    float scaleX = fSpanW / regW;
    float scaleY = fSpanH / regH;

    // anchor offset Y (param_7 high nibble)
    int anchorY;
    if ((param_7 & 0x70) == 0x40) {
        if ((param_6 & 0x80) == 0) {
            anchorY = -(param_5 >> 1);
        } else {
            anchorY = ((param_5 + param_3) - paintcanvas_ext_di4_getheight(this)) >> 1;
        }
    } else if ((param_7 & 0x70) == 0x20) {
        if ((param_6 & 0x80) == 0) {
            anchorY = -param_5;
        } else {
            anchorY = (param_5 + param_3) - paintcanvas_ext_di4_getheight(this);
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

    if (param_8 & 1) {
        m[0] = -scaleX;
        m[3] = paintcanvas_ext_di4_signedtofloat(param_4, 0) + m[3];
    }
    m[5] = scaleY;
    if (param_8 & 2) {
        m[5] = -scaleY;
        m[7] = paintcanvas_ext_di4_signedtofloat(param_5, 0) + m[7];
    }

    paintcanvas_ext_di4_setwvm(this, m);
    paintcanvas_ext_di4_gldisable(0xb44);
    paintcanvas_ext_di4_meshdraw(this->field_0x34,
                                 *(void **)(((char **)this->field_0x150)[param_1]));
    paintcanvas_ext_di4_glenable(0xb44);
}

void PaintCanvas::CameraSetPerspective(unsigned int index, float fov, float aspect)
{
    if (index < this->field_0x164) {
        float w = (float)paintcanvas_ext_get_w(this);
        float h = (float)paintcanvas_ext_get_h(this);
        void *cam = (this->field_0x168)[index];
        paintcanvas_ext_cam_persp(fov, aspect, w, h, cam);
        if (this->field_0x170 == index) {
            return paintcanvas_ext_cam_setcur(this, index);
        }
    }
}

void PaintCanvas::DrawRegion2D(unsigned int param_1, float param_2, int param_3,
                  int param_4, int param_5, int param_6, float param_7, float param_8)
{
    if (param_1 >= this->field_0x14c) {
        return;
    }
    char *img = ((char **)this->field_0x150)[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_dr2_restore(*(unsigned char *)(img + 0x14), img);
        img = ((char **)this->field_0x150)[param_1];
    }
    paintcanvas_ext_dr2_settexture(this, *(unsigned int *)(img + 4));

    // param_3 = rotation steps; param_4/5 = pivot; param_6/param_7? = translation; param_7/8 = scale
    float rot = paintcanvas_ext_dr2_signedtofloat(param_3, 0);
    float px = paintcanvas_ext_dr2_signedtofloat(param_4, 0);
    float py = paintcanvas_ext_dr2_signedtofloat(param_5, 0);
    float tx = paintcanvas_ext_dr2_signedtofloat(param_6, 0);
    float negpx = paintcanvas_ext_dr2_signedtofloat(-param_4, 0);
    float negpy = paintcanvas_ext_dr2_signedtofloat(-param_5, 0);

    // translation matrix local_78 (identity + translation in column 3)
    float transM[16];
    memset(transM, 0, sizeof(transM));
    transM[0] = 1.0f; transM[5] = 1.0f; transM[10] = 1.0f; transM[15] = 1.0f;
    transM[3] = py + rot * param_7;
    transM[7] = tx + px * param_8;

    // pivot matrix local_178
    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[10] = 1.0f; pivotM[15] = 1.0f;
    pivotM[3] = negpx * param_7;
    pivotM[7] = negpy * param_8;

    // rotation matrix local_f8
    float s = paintcanvas_ext_dr2_sinf(negpx * param_7);
    float c = paintcanvas_ext_dr2_cosf(negpx * param_7);
    float rotM[16];
    memset(rotM, 0, sizeof(rotM));
    rotM[0] = c;
    rotM[5] = c;
    rotM[10] = 1.0f;
    rotM[15] = 1.0f;
    *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
    rotM[4] = s;

    float scaleBuf[16];
    paintcanvas_ext_dr2_setscaling(scaleBuf, param_7, param_8, 1.0f);

    // build the composed matrix
    float local138[16];
    paintcanvas_ext_dr2_mtx_mul(scaleBuf, rotM, transM);
    paintcanvas_ext_dr2_mtx_assign(local138, scaleBuf);
    float scratch[16];
    paintcanvas_ext_dr2_mtx_mul(scratch, pivotM, local138);
    paintcanvas_ext_dr2_mtx_mul(scaleBuf, scratch, pivotM);
    paintcanvas_ext_dr2_mtx_assign(local138, scaleBuf);

    paintcanvas_ext_dr2_setwvm(this, local138);
    paintcanvas_ext_dr2_gldisable(0xb44);
    paintcanvas_ext_dr2_meshdraw(this->field_0x34,
                                 *(void **)(((char **)this->field_0x150)[param_1]));
    paintcanvas_ext_dr2_glenable(0xb44);
}

void PaintCanvas::MeshSetTriangle(unsigned int meshIndex, unsigned short tri,
                     unsigned short v0, unsigned short v1, unsigned short v2)
{
    if (meshIndex < this->field_0x24) {
        char *mesh = (this->field_0x28)[meshIndex];
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

void PaintCanvas::SetGameOrientation(int param_2)
{
    if (this->field_0x30 == param_2) {
        return;
    }
    this->field_0x30 = param_2;
    paintcanvas_ext_sgo_setorientation(this->field_0x34, param_2);

    this->projMatrix3d.m[1] = -this->projMatrix3d.m[1];
    this->projMatrix3d.m[4] = -this->projMatrix3d.m[4];

    int w = paintcanvas_ext_sgo_dispwidth(this->field_0x34);
    float fw = paintcanvas_ext_sgo_signedtofloat(w, 0);
    int h = paintcanvas_ext_sgo_dispheight(this->field_0x34);
    float fh = paintcanvas_ext_sgo_signedtofloat(h, 0);

    if (param_2 == 3) {
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
        int w2 = paintcanvas_ext_sgo_dispwidth(this->field_0x34);
        this->worldViewMatrix.m[12] = paintcanvas_ext_sgo_signedtofloat(w2, 0);
        int h2 = paintcanvas_ext_sgo_dispheight(this->field_0x34);
        this->worldViewMatrix.m[13] = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (param_2 == 1) {
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
        int h2 = paintcanvas_ext_sgo_dispheight(this->field_0x34);
        this->worldViewMatrix.m[12] = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (param_2 != 0) {
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
        int w2 = paintcanvas_ext_sgo_dispwidth(this->field_0x34);
        this->worldViewMatrix.m[13] = paintcanvas_ext_sgo_signedtofloat(w2, 0);
    }

    if (this->field_0x170 == -1) {
        return;
    }
    float *cam = ((float **)this->field_0x168)[this->field_0x170];
    paintcanvas_ext_sgo_setpersp(this, cam[0], cam[1], cam[2]);
}

extern "C" int paintcanvas_ext_mc_meshcreate(void *eng, unsigned short a, unsigned short b,
                                             signed char c, void **out);

namespace AbyssEngine {
}

void PaintCanvas::MeshCreate(unsigned short param_1, unsigned short param_2,
                signed char param_3, unsigned short param_4, unsigned int *param_6)
{
    int result = -1;
    unsigned int mat = 0xffffffff;
    void *mesh = 0;
    paintcanvas_ext_mc_matcreate(this, param_4, &mat);
    int ok = paintcanvas_ext_mc_meshcreate(this->field_0x34, param_1, param_2,
                                           param_3, &mesh);
    if (ok == 1) {
        if (0xfffffffe < this->field_0x174) {
            // material pointer back-reference: write last element ptr into mesh+0x30
            void *m = *(void **)(this->field_0x178 - 4);
            if (mesh) {
                ((AbyssEngine::Mesh *)mesh)->field_0x30 = m;
            }
        }
        PCArrayAdd<AbyssEngine::Mesh *>((AbyssEngine::Mesh *)mesh, &this->field_0x24);
        result = this->field_0x24 - 1;
    }
    *param_6 = (unsigned int)result;
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

void PaintCanvas::DrawMesh(char *param_1, const float *param_2,
              const float *param_3, unsigned int param_4, const float *param_5)
{

    float worldM[15];
    float uvM[15];
    paintcanvas_ext_dm_memcpy(worldM, param_2, 0x3c);
    paintcanvas_ext_dm_memcpy(uvM, param_5, 0x3c);

    unsigned int color = param_4;

    // If the mesh has an attached resource (offset 0x34), apply its local transform.
    if (*(int *)(param_1 + 0x34) != 0) {
        float a[16], b[16];
        memset(a, 0, sizeof(a));
        a[0] = 1.0f; a[5] = 1.0f; a[14] = 1.0f;
        memset(b, 0, sizeof(b));
        b[0] = 1.0f; b[5] = 1.0f; b[14] = 1.0f;

        float pivotM[16];
        paintcanvas_ext_dm_settrans(pivotM, *(float *)(param_1 + 0x54));
        paintcanvas_ext_dm_settrans(pivotM, -*(float *)(param_1 + 0x54));
        memset(pivotM, 0, sizeof(pivotM));
        pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[14] = 1.0f;

        float posM[16];
        float resLocal[16];
        paintcanvas_ext_dm_getpos(posM);
        paintcanvas_ext_dm_settrans_vec(resLocal, pivotM);
        paintcanvas_ext_dm_memcpy(resLocal, *(char **)(param_1 + 0x34) + 0x5c, 0x3c);

        float s1[16], s2[16], s3[16];
        paintcanvas_ext_dm_mtx_mul(s1, param_2, pivotM);
        paintcanvas_ext_dm_mtx_mul(s2, s1, b);
        paintcanvas_ext_dm_mtx_mul(s3, s2, resLocal);
        paintcanvas_ext_dm_mtx_mul(posM, s3, a);
        paintcanvas_ext_dm_mtx_assign(worldM, posM);

        unsigned int resColor = *(unsigned int *)(*(char **)(param_1 + 0x34) + 0x48);
        paintcanvas_ext_dm_mtx_assign(uvM, *(char **)(param_1 + 0x34) + 0x98);
        color = mulColors(resColor, param_4);
    }

    float pos[3];
    paintcanvas_ext_dm_memcpy(pos, worldM, 0); // placeholder no-op to anchor worldM use
    paintcanvas_ext_dm_transformvec(uvM, worldM);

    float ext[3];
    ext[0] = *(float *)(param_1 + 0x48);
    ext[1] = ext[0];
    ext[2] = ext[0];
    paintcanvas_ext_dm_rotatevec(uvM, worldM);
    paintcanvas_ext_dm_vec_assign(ext, uvM);

    // radius from the larger absolute component
    float ax = ext[0] < 0.0f ? -ext[0] : ext[0];
    float ay = ext[1] < 0.0f ? -ext[1] : ext[1];
    float az = ext[2] < 0.0f ? -ext[2] : ext[2];
    float maxxy = ax > ay ? ax : ay;
    float radius = maxxy;

    if (*(short *)(param_1 + 2) != 0) {
        float r2 = maxxy > az ? maxxy : az;
        int vis = paintcanvas_ext_dm_spherefrustum(this, uvM, r2 * *(float *)(param_1 + 0x4c));
        if (vis == 0) {
            this->field_0x4 += 1;
            return;
        }
        if (*(short *)(param_1 + 2) != 0) {
            if (*(int *)(param_1 + 0x30) == 0) {
                float fr = paintcanvas_ext_dm_unsignedtofloat(color >> 24, 0);
                float fg = paintcanvas_ext_dm_unsignedtofloat((color >> 16) & 0xff, 0);
                paintcanvas_ext_dm_unsignedtofloat((color >> 8) & 0xff, 0);
                paintcanvas_ext_dm_unsignedtofloat(color & 0xff, 0);
                paintcanvas_ext_dm_setcolor(
                    this->field_0x34,
                    (this->field_0x1fc * fr) / g_dm_255_8ee80, 0.0f,
                    (this->field_0x200 * fg) / g_dm_255_8ee80, 0.0f);

                paintcanvas_ext_dm_mtx_muleq(worldM, param_3);
                paintcanvas_ext_dm_setwvm(this, worldM);
                paintcanvas_ext_dm_setmodelmatrix(this->field_0x34);
                paintcanvas_ext_dm_setuvmatrix(this->field_0x34, uvM);
                paintcanvas_ext_dm_meshdraw(this->field_0x34, param_1);
                paintcanvas_ext_dm_resetuvmatrix(this->field_0x34);
            } else {
                // batch into the glow target at resource+0x44/0x2c/0x38/0x5c/0x50
                char *res = *(char **)(param_1 + 0x30);
                paintcanvas_ext_dm_addcached_mesh(param_1, res + 0x44);
                paintcanvas_ext_dm_pushmat(worldM, res + 0x2c);
                paintcanvas_ext_dm_pushmat(uvM, res + 0x38);
                paintcanvas_ext_dm_pushmat(param_3, res + 0x5c);
                paintcanvas_ext_dm_addcached_uint(color, res + 0x50);
            }
        }
    }

    // recurse over child meshes (count 0x3c / array 0x40) and child transforms (0x4c / 0x50)
    if (*(int *)(param_1 + 0x34) != 0) {
        char *res = *(char **)(param_1 + 0x34);
        unsigned int n = *(unsigned int *)(res + 0x3c);
        while ((int)(--n) >= 0) {
            char *child = (*(char ***)(res + 0x40))[n];
            this->DrawMesh(child, worldM, param_3, param_4, uvM);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(res + 0x4c); i++) {
            if (this->field_0x170 < this->field_0x164) {
                void *cam = ((void **)this->field_0x168)[this->field_0x170];
                char *tf = (*(char ***)(res + 0x50))[i];
                if (paintcanvas_ext_dm_incamvf(tf, (void *)param_2, cam)) {
                    paintcanvas_ext_dm_drawtransform(this, (*(void ***)(res + 0x50))[i],
                                                     worldM, (void *)param_3);
                }
            }
        }
    }
}

#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_b2d_flag HIDDEN;

void PaintCanvas::Begin2d()
{
    *(unsigned char *)((char *)this->field_0x34 + 0xfd) = 1;
    paintcanvas_ext_gl_disable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_blendfunc(0x302, 0x303);
    paintcanvas_ext_setcolor(this->field_0x34, 1.0f, 1.0f, 1.0f, 1.0f);
    paintcanvas_ext_glenable2(this->field_0x34, 0xde1, true);
    if (*paintcanvas_g_b2d_flag == 0) {
        paintcanvas_ext_gl_texenvi(0x2300, 0x2200, 0x2100);
        paintcanvas_ext_glMatrixMode(0x1702);
        paintcanvas_ext_gl_loadidentity();
        paintcanvas_ext_gl_scalef(1.0f, 1.0f, 1.0f);
        paintcanvas_ext_glMatrixMode(0x1701);
        paintcanvas_ext_gl_loadmatrix(&this->projOrthoMatrix.m[0]);
        if (this->field_0x30 != 2) {
            paintcanvas_ext_gl_multmatrix(&this->worldViewMatrix.m[0]);
        }
        paintcanvas_ext_glMatrixMode(0x1700);
        paintcanvas_ext_gl_loadidentity();
    } else {
        paintcanvas_ext_setortho(this->field_0x34, &this->projOrthoMatrix.m[0],
                                 &this->worldViewMatrix.m[0], this->field_0x30 != 2);
    }
    this->field_0xc = 0;
}

extern "C" void paintcanvas_ext_drawstring_raw(void *, const unsigned short *, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void PaintCanvas::DrawString(unsigned int index, const unsigned short *str,
                int x, int y, bool b)
{
    if (index < this->field_0x140) {
        char *font = (char *)(this->field_0x144)[index];
        paintcanvas_ext_string_prep(this, *(void **)(font + 0x8), -1);
        char *font2 = (char *)(this->field_0x144)[index];
        paintcanvas_ext_drawstring_raw(font2, str, x, y, this,
                                       this->field_0x34, b);
    }
}

__attribute__((visibility("hidden"))) extern const char g_dsc_pipe_882c4[];
__attribute__((visibility("hidden"))) extern const char g_dsc_fmt_882ee[];

extern "C" void paintcanvas_ext_dsc_fontdraw(void *font, unsigned short *txt, unsigned int len,
                                             int x, int y, void *self, void *eng, bool b);

void PaintCanvas::DrawStringColor(unsigned int param_1, void *param_2,
                     int param_3, int param_4, bool param_5)
{
    if (param_1 >= this->field_0x140) {
        return;
    }
    void *font0 = ((char **)this->field_0x144)[param_1];
    paintcanvas_ext_dsc_settexture(this, *(unsigned int *)((char *)font0 + 8));
    paintcanvas_ext_dsc_getcolor(this);

    char str[16];
    char sep[16];
    paintcanvas_ext_dsc_str_copy(str, param_2, false);
    paintcanvas_ext_dsc_str_fromchar(sep, g_dsc_pipe_882c4, false);
    char *parts = (char *)paintcanvas_ext_dsc_splittags(str, sep);
    paintcanvas_ext_dsc_str_dtor(sep);

    if (parts != 0) {
        bool draw = true;
        for (unsigned int i = 0; i < *(unsigned int *)parts; i++) {
            char **data = *(char ***)(parts + 4);
            char *part = data[i];
            if (draw) {
                void *font = ((char **)this->field_0x144)[param_1];
                unsigned short *txt = paintcanvas_ext_dsc_str_cast(part);
                paintcanvas_ext_dsc_fontdraw(font, txt, *(unsigned int *)(part + 8), param_3,
                                             param_4, this, this->field_0x34, param_5);
                param_3 += paintcanvas_ext_dsc_textwidth(this, param_1, part);
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

void PaintCanvas::SetMatForGlow(char *param_1)
{
    int off = 0x38;
    for (unsigned int i = 0; i < *(unsigned int *)(param_1 + 0x44); i++) {
        // meshes array at 0x48 -> list at this+0x18c
        PCArrayAdd<AbyssEngine::Mesh *>((*(AbyssEngine::Mesh ***)(param_1 + 0x48))[i], &this->glowMeshes_count);

        const float *m1 = (const float *)(*(char **)(param_1 + 0x30) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m1, &this->glowMatA_count);

        const float *m2 = (const float *)(*(char **)(param_1 + 0x3c) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m2, &this->glowMatB_count);

        PCArrayAdd<unsigned int>((*(unsigned int **)(param_1 + 0x54))[i], &this->glowUints_count);

        const float *m3 = (const float *)(*(char **)(param_1 + 0x60) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m3, &this->glowMatC_count);

        off += 0x3c;
    }
}

void PaintCanvas::EnableClip(int param_1, int param_2, int param_3, int param_4)
{
    paintcanvas_ext_ec_glEnable(0xc11);
    int sx, sy, sw, sh;
    switch (this->field_0x30) {
    case 0:
        sx = param_1;
        sy = param_2;
        sw = param_3;
        sh = param_4;
        break;
    case 1: {
        int h = paintcanvas_ext_ec_getHeight(this);
        int w = paintcanvas_ext_ec_getWidth(this);
        sx = h - (param_2 + param_4);
        sy = w - (param_1 + param_3);
        sw = param_4;
        sh = param_3;
        break;
    }
    case 2: {
        int h = paintcanvas_ext_ec_getHeight(this);
        sx = h - (param_2 + param_4);
        sy = param_1;
        sw = param_3;
        sh = param_4;
        break;
    }
    case 3: {
        int w = paintcanvas_ext_ec_getWidth(this);
        sx = w - (param_1 + param_3);
        sy = param_2;
        sw = param_3;
        sh = param_4;
        break;
    }
    default:
        sx = param_1;
        sy = param_2;
        sw = param_3;
        sh = param_4;
        break;
    }
    paintcanvas_ext_ec_glScissor(sx, sy, sw, sh);
}

void PaintCanvas::DrawImage2D(unsigned int param_1, int param_2, int param_3,
                 unsigned char param_4, unsigned char param_5)
{
    if (param_1 >= this->field_0x14c) {
        return;
    }
    char *img = ((char **)this->field_0x150)[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di3_restore(*(unsigned char *)(img + 0x14), img);
    }

    // Horizontal placement offset (param_5 low bits) + vertical (param_5 high nibble).
    int hOff;
    if ((param_5 & 7) == 4) {
        hOff = paintcanvas_ext_di3_getwidth(this) >> 1;
    } else if ((param_5 & 7) == 2) {
        hOff = paintcanvas_ext_di3_getwidth(this);
        param_3 = -param_3;
    } else {
        hOff = 0;
    }

    int vOff;
    unsigned int yShift = param_4;
    if ((param_5 & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di3_getheight(this) >> 1;
    } else if ((param_5 & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di3_getheight(this);
        yShift = (unsigned int)(-(int)yShift);
    } else {
        vOff = 0;
    }

    // Region-based offsets from the image's source region (param_4 used as a second align flag).
    int rx;
    char *region = *(char **)(img + 4);
    if ((param_4 & 7) == 4) {
        double w = (double)paintcanvas_ext_di3_signedtofloat((int)*(float *)(region + 0xc), 0);
        rx = (int)(long long)(w * -0.5);
    } else if ((param_4 & 7) == 2) {
        rx = -(int)*(float *)(region + 0xc);
    } else {
        rx = 0;
    }

    int ry;
    char *region2 = *(char **)(((char **)this->field_0x150)[param_1] + 4);
    if ((param_4 & 0x70) == 0x20) {
        ry = -(int)*(float *)(region2 + 0x1c);
    } else if ((param_4 & 0x70) == 0x40) {
        double h = (double)paintcanvas_ext_di3_signedtofloat((int)*(float *)(region2 + 0x1c), 0);
        ry = (int)(long long)(h * -0.5);
    } else {
        ry = 0;
    }

    char *img2 = ((char **)this->field_0x150)[param_1];
    paintcanvas_ext_di3_settexture(this, *(unsigned int *)(img2 + 4));

    float fx = paintcanvas_ext_di3_signedtofloat(hOff + param_3 + rx, 0);
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
    paintcanvas_ext_di3_meshdraw(this->field_0x34,
                                 *(void **)(((char **)this->field_0x150)[param_1]));
}

void PaintCanvas::MeshCreate(unsigned short a, unsigned short b,
                signed char c, unsigned int *out)
{
    char mesh[4];
    *(void **)mesh = 0;
    int result = paintcanvas_ext_meshcreate(this->field_0x34, mesh);
    if (result == 1) {
        PCArrayAdd<void *>(*(void **)mesh, &this->field_0x24);
        result = (int)this->field_0x24 - 1;
    } else {
        result = -1;
    }
    *out = result;
}

int PaintCanvas::FontGetSpacing(unsigned int index)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
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
    if (index < this->field_0x158) {
        return (this->field_0x15c)[index];
    }
    return 0;
}

__attribute__((visibility("hidden"))) extern char *const g_meshcreate_vboflag_79d5c;

namespace AbyssEngine {
}

void PaintCanvas::MeshCreate(unsigned short param_1, unsigned int *param_2,
                bool param_3)
{
    char *res = (char *)paintcanvas_ext_mc2_findres(this, param_1);
    if (res == 0) {
        return;
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *info = *(char **)(res + 0xc);
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_mc2_matcreate(this, *(unsigned short *)(info + 4), &mat);
        void *matptr = 0;
        if (0xfffffffe < this->field_0x174) {
            matptr = *(void **)(this->field_0x178 - 4);
        }
        void *mesh = 0;
        int ok = paintcanvas_ext_mc2_meshfromfile(this->field_0x34,
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
        PCArrayAdd<AbyssEngine::Mesh *>((AbyssEngine::Mesh *)mesh, &this->field_0x24);
        idx = this->field_0x24 - 1;
        *(unsigned int *)(res + 8) = idx;
    } else {
        char **meshes = this->field_0x28;
        char *existing = meshes[idx];
        if (*(int *)(existing + 0x34) != 0 || param_3) {
            void *clone = paintcanvas_ext_mc2_new_mesh_copy(
                ((void **)meshes)[*(unsigned int *)(res + 8)]);
            PCArrayAdd<AbyssEngine::Mesh *>((AbyssEngine::Mesh *)clone, &this->field_0x24);
            idx = this->field_0x24 - 1;
        }
    }
    *param_2 = idx;
}

__attribute__((visibility("hidden"))) extern char *const g_fsp_flag_8cf40;
__attribute__((visibility("hidden"))) extern const double g_fsp_255d_8d070;
__attribute__((visibility("hidden"))) extern const float g_fsp_255f_8d078;

void PaintCanvas::FogSetParameter(int param_1, float param_2, float param_3,
                     float param_4, unsigned int param_5)
{
    float col[4];
    if (*g_fsp_flag_8cf40 == 0) {
        float density = paintcanvas_ext_fsp_unsignedtofloat((unsigned int)param_1, 0);
        paintcanvas_ext_fsp_glFogf(0xb65, density);
        paintcanvas_ext_fsp_glFogf(0xb62, param_4);
        paintcanvas_ext_fsp_glFogf(0xb63, param_2);
        paintcanvas_ext_fsp_glFogf(0xb64, param_3);

        float r = paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 16) & 0xff, 0);
        float g = paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 8) & 0xff, 0);
        paintcanvas_ext_fsp_unsignedtofloat(param_5 >> 24, 0);
        float b = paintcanvas_ext_fsp_unsignedtofloat(param_5 & 0xff, 0);
        col[0] = r / g_fsp_255f_8d078;
        col[1] = g / g_fsp_255f_8d078;
        col[2] = b / g_fsp_255f_8d078;
        col[3] = param_2;
        paintcanvas_ext_fsp_glFogfv(0xb66, col);
    } else {
        double r = (double)paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 16) & 0xff, 0);
        r = r / g_fsp_255d_8d070;
        paintcanvas_ext_fsp_unsignedtofloat(param_5 >> 24, 0);
        double g = (double)paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 8) & 0xff, 0);
        Engine *eng = (Engine *)this->field_0x34;
        g = g / g_fsp_255d_8d070;
        eng->fogMinDist = param_2;
        eng->fogMaxDist = param_3;
        col[0] = (float)r;
        col[1] = (float)g;
        paintcanvas_ext_fsp_vec_assign(&eng->fogColor, col);
    }
}

__attribute__((visibility("hidden"))) extern const unsigned int g_dr3_const_88808;

void PaintCanvas::DrawRegion2D(unsigned int param_1, int param_2, int param_3,
                  int param_4, int param_5, float param_6, int param_7, int param_8, int param_9,
                  int param_10)
{
    if (param_1 >= this->field_0x14c) {
        return;
    }
    char *img = ((char **)this->field_0x150)[param_1];
    paintcanvas_ext_dr3_settexture(this, *(unsigned int *)(img + 4));
    char *mesh = ((char **)this->field_0x150)[param_1];
    char *meshObj = *(char **)mesh;

    *(unsigned char *)(mesh + 0x14) = 1;

    // vertex positions (8 floats) at mesh[0]+4 -> +0xc
    float fw = paintcanvas_ext_dr3_signedtofloat(param_4, 0);
    float fh = paintcanvas_ext_dr3_signedtofloat(param_5, 0);
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
    float a2 = paintcanvas_ext_dr3_signedtofloat(param_2, 0);
    float a3 = paintcanvas_ext_dr3_signedtofloat(param_3, 0);
    float a2s = paintcanvas_ext_dr3_signedtofloat((short)param_2, 0);

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

    // pivot translation matrix local_b0
    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[10] = 1.0f; pivotM[15] = 1.0f;
    pivotM[3] = paintcanvas_ext_dr3_signedtofloat(-param_8, 0);
    pivotM[7] = paintcanvas_ext_dr3_signedtofloat(-param_9, 0);

    // inverse-pivot translation local_70
    float invPivotM[16];
    memset(invPivotM, 0, sizeof(invPivotM));
    invPivotM[0] = 1.0f; invPivotM[5] = 1.0f; invPivotM[10] = 1.0f; invPivotM[15] = 1.0f;
    invPivotM[3] = paintcanvas_ext_dr3_signedtofloat(param_10 + param_8, 0);
    invPivotM[7] = paintcanvas_ext_dr3_signedtofloat(param_7 + param_9, 0);

    // rotation matrix local_f0 (angle = invPivotM[7])
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

    float local130[16];
    float scratch[16];
    paintcanvas_ext_dr3_mtx_mul(scratch, rotM);
    paintcanvas_ext_dr3_mtx_assign(local130, scratch);
    paintcanvas_ext_dr3_mtx_mul(scratch, pivotM);
    paintcanvas_ext_dr3_mtx_assign(local130, scratch);

    paintcanvas_ext_dr3_setwvm(this, local130);
    paintcanvas_ext_dr3_gldisable(0xb44);
    paintcanvas_ext_dr3_meshdraw(this->field_0x34,
                                 *(void **)(((char **)this->field_0x150)[param_1]));
    paintcanvas_ext_dr3_glenable(0xb44);
}

void RestoreImage2D(AbyssEngine::PaintCanvas *, char *img)
{
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
    if (index < this->field_0x180) {
        char *s = (this->field_0x184)[index];
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

void PaintCanvas::MeshSet2DMask(unsigned int index, int)
{
    unsigned int i = index;
    if (this->field_0x14c <= index) {
        return;
    }
    char **arr = this->field_0x150;
    char *img = arr[i];
    if (*(unsigned char *)(img + 0x14) != 0) {
        RestoreImage2D(arr, img);
        arr = this->field_0x150;
    }
    this->field_0x20 = arr[i];
}

__attribute__((visibility("hidden"))) extern int *const g_rar_curtex_87c98;
__attribute__((visibility("hidden"))) extern int *const g_rar_texcount_87cce;
__attribute__((visibility("hidden"))) extern int *const g_rar_tricount_87d96;

void PaintCanvas::ReleaseAllResources()
{
    *g_rar_curtex_87c98 = 0;

    // mark all resources as unloaded
    for (int i = 0; i < this->field_0x134; i++) {
        char *res = ((char **)this->field_0x138)[i];
        *(int *)(res + 8) = -1;
    }

    // textures
    for (unsigned int i = 0; i < this->field_0x10; i++) {
        int *tex = ((int **)this->field_0x14)[i];
        if (*tex != -1) {
            unsigned int id = (unsigned int)*tex;
            paintcanvas_ext_rar_gldeltex(1, &id);
            *g_rar_texcount_87cce = *g_rar_texcount_87cce - 1;
            Engine *eng = (Engine *)this->field_0x34;
            char *texEntry = ((char **)this->field_0x14)[i];
            eng->field_0x70 = eng->field_0x70 - *(int *)(texEntry + 0x18);
            tex = ((int **)this->field_0x14)[i];
        }
        if (tex != 0) {
            paintcanvas_ext_rar_str_dtor((char *)tex + 4);
            paintcanvas_ext_rar_op_delete(tex);
        }
        ((int **)this->field_0x14)[i] = 0;
    }
    this->field_0x10 = 0;

    // fonts
    for (unsigned int i = 0; i < this->field_0x140; i++) {
        if (((void **)this->field_0x144)[i] != 0) {
            paintcanvas_ext_rar_fontrelease(this->field_0x34,
                                            &((void **)this->field_0x144)[i]);
        }
    }
    PCArrayRemoveAll(&this->field_0x140);

    // image2D
    for (unsigned int i = 0; i < this->field_0x14c; i++) {
        if (((void **)this->field_0x150)[i] != 0) {
            paintcanvas_ext_rar_img2drelease(this->field_0x34,
                                             &((void **)this->field_0x150)[i]);
        }
    }
    PCArrayRemoveAll(&this->field_0x14c);

    // meshes
    for (unsigned int i = 0; i < this->field_0x24; i++) {
        char *mesh = ((char **)this->field_0x28)[i];
        if (mesh != 0) {
            *g_rar_tricount_87d96 = *g_rar_tricount_87d96 - *(int *)(mesh + 0x7c);
            paintcanvas_ext_rar_meshrelease(this->field_0x34,
                                            &((void **)this->field_0x28)[i]);
        }
    }
    PCArrayRemoveAll(&this->field_0x24);

    // transforms
    for (unsigned int i = 0; i < this->field_0x158; i++) {
        void *tf = ((void **)this->field_0x15c)[i];
        if (tf != 0) {
            paintcanvas_ext_rar_op_delete(paintcanvas_ext_rar_transform_dtor(tf));
            ((void **)this->field_0x15c)[i] = 0;
        }
    }
    PCArrayRemoveAll(&this->field_0x158);

    // cameras
    for (unsigned int i = 0; i < this->field_0x164; i++) {
        void *cam = ((void **)this->field_0x168)[i];
        if (cam != 0) {
            paintcanvas_ext_rar_op_delete(cam);
            ((void **)this->field_0x168)[i] = 0;
        }
    }
    PCArrayRemoveAll(&this->field_0x164);
    this->field_0x170 = -1;

    // materials
    for (unsigned int i = 0; i < this->field_0x174; i++) {
        void *mat = ((void **)this->field_0x178)[i];
        if (mat != 0) {
            paintcanvas_ext_rar_op_delete(paintcanvas_ext_rar_material_dtor(mat));
            ((void **)this->field_0x178)[i] = 0;
        }
    }
    PCArrayRemoveAll(&this->field_0x174);

    // sprite systems
    for (unsigned int i = 0; i < this->field_0x180; i++) {
        if (((void **)this->field_0x184)[i] != 0) {
            paintcanvas_ext_rar_ssrelease(this->field_0x34,
                                          &((void **)this->field_0x184)[i]);
        }
    }
    PCArrayRemoveAll(&this->field_0x180);
    this->field_0x1cc = 0;
}

__attribute__((visibility("hidden"))) extern const unsigned int g_tg2d_defval_7b590;

void TransformGet2DPickedTextureRegion(void *param_1, char *param_2, int param_3, int param_4,
                                       int param_5, int param_6)
{
    char matbuf[60];
    char vecbuf[64];

    if ((unsigned int)param_3 < *(unsigned int *)(param_2 + 0x158)) {
        char *tf = (*(char ***)(param_2 + 0x15c))[param_3];
        paintcanvas_ext_tg2d_memcpy(vecbuf, tf, 0x3c);
        float local74 = paintcanvas_ext_tg2d_signedtofloat(param_5, 0);
        float local70 = paintcanvas_ext_tg2d_signedtofloat(param_6, 0);
        float local6c = paintcanvas_ext_tg2d_signedtofloat(param_4, 0);
        float vin[3];
        vin[0] = local74;
        vin[1] = local70;
        vin[2] = local6c;
        paintcanvas_ext_tg2d_invtransformvec(matbuf, vecbuf);
        paintcanvas_ext_tg2d_vec_assign(vin, matbuf);
        paintcanvas_ext_tg2d_inner(param_1, param_2, tf, (int)vin[0], (int)vin[1]);
    } else {
        *(unsigned int *)param_1 = g_tg2d_defval_7b590;
        *(unsigned int *)((char *)param_1 + 4) = g_tg2d_defval_7b590;
        paintcanvas_ext_tg2d_errmsg(param_1);
    }
}

void PaintCanvas::CheckString(unsigned int index, void *str)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        return paintcanvas_ext_check_string(font, len, *(unsigned int *)((char *)str + 0x8));
    }
}

extern "C" int paintcanvas_ext_rt_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);

void PaintCanvas::ReloadTextures()
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < this->field_0x10; i++) {
        char *res = (this->field_0x14)[i];
        if (*(int *)res == -1) {
            char *path = paintcanvas_ext_rt_getAEChar(res + 4);
            float f = *(float *)((this->field_0x14)[i] + 0x10);
            int ok = paintcanvas_ext_rt_texfromfile(this->field_0x34, path, 0, 0,
                                                    &out, false, f);
            if (ok == 1) {
                *(int *)((this->field_0x14)[i]) = 0;
            }
            paintcanvas_ext_rt_deletearr(path);
        }
    }
}

namespace AbyssEngine {
}

void PaintCanvas::MaterialCreate(unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_matc_findres(this, param_1);
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
        PCArrayAdd<AbyssEngine::Material *>((AbyssEngine::Material *)mat, &this->field_0x174);
        idx = this->field_0x174 - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    *param_2 = idx;
}

__attribute__((visibility("hidden"))) extern const unsigned int g_tg2di_neg1_8b588;

void TransformGet2DPickedTextureRegion(void *param_1, void *param_2, char *param_3, int param_4,
                                       int param_5, int param_6, unsigned int param_7)
{
    // param_4 = transform ptr (Mesh root container), param_5/6/7 = vec components, shift = param?
    // Per disasm: r5=param_3 (transform), shifts applied with r6 = [r7,#0x10] (shift amount)
    char *tf = param_3;
    int shift = (int)param_7;
    float vy = paintcanvas_ext_tg2di_signedtofloat((param_5 >> shift) >> shift, 0);
    float vx = paintcanvas_ext_tg2di_signedtofloat((param_4 >> shift) >> shift, 0);

    // First: test meshes (offset 0x3c count, 0x40 array)
    unsigned int i = 0;
    bool found = false;
    while (i < *(unsigned int *)(tf + 0x3c)) {
        void *mesh = (*(void ***)(tf + 0x40))[i];
        paintcanvas_ext_tg2di_meshintersect(param_1, vx, vy, mesh);
        i++;
        if (*(float *)param_1 != -1.0f && *(float *)((char *)param_1 + 4) != -1.0f) {
            found = true;
            break;
        }
    }
    if (found) {
        return;
    }

    // Then: recurse over child transforms (0x4c count, 0x50 array)
    float fy = paintcanvas_ext_tg2di_signedtofloat(param_5, 0);
    float fx = paintcanvas_ext_tg2di_signedtofloat(param_4, 0);
    float fz = paintcanvas_ext_tg2di_signedtofloat((int)param_7, 0);

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
        paintcanvas_ext_tg2di_inner(param_1, param_2, (*(void ***)(tf + 0x50))[i],
                                    (int)vin[0], (int)vin[1]);
        i++;
        if (*(float *)param_1 != -1.0f && *(float *)((char *)param_1 + 4) != -1.0f) {
            return;
        }
    }
    *(unsigned int *)param_1 = g_tg2di_neg1_8b588;
    *(unsigned int *)((char *)param_1 + 4) = g_tg2di_neg1_8b588;
}

__attribute__((visibility("hidden"))) extern const double g_dt_gravscale_898d8;

void PaintCanvas::DrawTransform(unsigned int param_1, const float *param_2)
{
    if (param_1 >= this->field_0x158) {
        return;
    }
    char *tf = ((char **)this->field_0x15c)[param_1];
    if (*(unsigned char *)(tf + 0xec) == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f; worldM[5] = 1.0f; worldM[14] = 1.0f;

    if (this->field_0x170 < this->field_0x164) {
        void *cam = ((void **)this->field_0x168)[this->field_0x170];
        if (this->field_0x0 == 0) {
            int vis = paintcanvas_ext_dt2_incamvf(((void **)this->field_0x15c)[param_1], 0, cam);
            if (vis == 0) {
                this->field_0x4 += 1;
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

            void *grav = paintcanvas_ext_dt2_getgrav(this->field_0x34);
            double angle = *(double *)((char *)grav + 8) * g_dt_gravscale_898d8;
            float a = (float)angle;
            int orient = this->field_0x30;
            float ang = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dt2_sinf(ang);
            float c = paintcanvas_ext_dt2_cosf(ang);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            int vis = paintcanvas_ext_dt2_incamvf(((void **)this->field_0x15c)[param_1], rotM, cam);
            if (vis == 0) {
                this->field_0x4 += 1;
                return;
            }

            float viewM[16];
            const float *src = param_2;
            if (src == 0) {
                src = (const float *)(((char **)this->field_0x168)[this->field_0x170] + 0xc);
            }
            paintcanvas_ext_dt2_mtx_assign(viewM, src);
            paintcanvas_ext_dt2_mtx_muleq(viewM, rotM);
            paintcanvas_ext_dt2_mtx_getinv(scratch, viewM);
            paintcanvas_ext_dt2_mtx_assign(worldM, scratch);
        }
        void *cam2 = ((void **)this->field_0x168)[this->field_0x170];
        paintcanvas_ext_dt2_seteye(this->field_0x34,
                                   *(float *)((char *)cam2 + 0x18),
                                   *(float *)((char *)cam2 + 0x28),
                                   *(float *)((char *)cam2 + 0x38));
        tf = ((char **)this->field_0x15c)[param_1];
    }

    float ident[16];
    memset(ident, 0, sizeof(ident));
    ident[0] = 1.0f; ident[5] = 1.0f; ident[14] = 1.0f;
    paintcanvas_ext_dt2_drawrec(this, tf, ident, worldM);
}

void PaintCanvas::FontSetSpacing(unsigned int index, short spacing)
{
    if (index < this->field_0x140) {
        void *font = (this->field_0x144)[index];
        return paintcanvas_ext_font_set_spacing(font, spacing);
    }
}

void PaintCanvas::GetWidth()
{
    return paintcanvas_ext_get_width(this->field_0x34);
}

// AbyssEngine::PaintCanvas::CameraCreate(unsigned& out) — allocates a Camera sized to the
// current viewport and appends it to the camera array at +0x164, returning its index.

void PaintCanvas::CameraCreate(unsigned int *out)
{
    void *cam = operator new(0x5c);
    int w = pc_GetWidth(this);
    int h = pc_GetHeight(this);
    pc_Camera_ctor(cam, (float)h, (float)w);
    pc_ArrayAdd_Camera(cam, &this->field_0x164);
    *out = this->field_0x164 - 1;
}

__attribute__((visibility("hidden"))) extern const double g_dss_gravscale_8ada0;

void PaintCanvas::DrawSpriteSystem(unsigned int param_1, const float *mat)
{
    if (param_1 >= this->field_0x180) {
        return;
    }
    void *ss = ((void **)this->field_0x184)[param_1];
    if (ss == 0) {
        return;
    }

    // local copy of the incoming matrix
    float local[15];
    memcpy(local, mat, sizeof(local));

    float identbuf[16];
    char scratch[60];
    float inv[16];

    if (this->field_0x0 == 0) {
        paintcanvas_ext_dss_mtx_getinv(inv, local);
        paintcanvas_ext_dss_mtx_assign(local, inv);
    } else {
        memset(identbuf, 0, sizeof(identbuf));
        identbuf[0] = 1.0f;
        identbuf[5] = 1.0f;
        identbuf[14] = 1.0f;
        paintcanvas_ext_dss_matidentity(scratch, identbuf);

        void *grav = paintcanvas_ext_dss_getgrav(this->field_0x34);
        double angle = *(double *)((char *)grav + 8) * g_dss_gravscale_8ada0;
        float a = (float)angle;
        int orient = this->field_0x30;
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
    paintcanvas_ext_dss_ssdraw(this->field_0x34, ident2, local,
                               ((void **)this->field_0x184)[param_1]);
}

extern "C" void paintcanvas_ext_drawstring_str(void *, unsigned int, unsigned int, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void PaintCanvas::DrawString(unsigned int index, void *str,
                int x, int y, bool b)
{
    if (index < this->field_0x140) {
        char *font = (char *)(this->field_0x144)[index];
        paintcanvas_ext_string_prep(this, *(void **)(font + 0x8), -1);
        char *font2 = (char *)(this->field_0x144)[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        paintcanvas_ext_drawstring_str(font2, len, *(unsigned int *)((char *)str + 0x8), x, y,
                                       this, this->field_0x34, b);
    }
}

int PaintCanvas::TransformGetTriCount(char *transform)
{
    if (!transform) {
        return 0;
    }
    int total = 0;
    unsigned int n1 = *(unsigned int *)(transform + 0x3c);
    for (unsigned int i = 0; i != n1; ++i) {
        void *m = (*(void ***)(transform + 0x40))[i];
        total += paintcanvas_ext_mesh_tricount(this, m);
    }
    unsigned int n2 = *(unsigned int *)(transform + 0x4c);
    for (unsigned int i = 0; i != n2; ++i) {
        void *c = (*(void ***)(transform + 0x50))[i];
        total += paintcanvas_ext_transform_tricount(this, c);
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
    if (index < this->field_0x180) {
        void *ctx = this->field_0x34;
        char **arr = this->field_0x184;
        return paintcanvas_ext_release_sprite_res(ctx, arr + index);
    }
}

__attribute__((visibility("hidden"))) extern const double g_dss2_gravscale_8af58;

void PaintCanvas::DrawSpriteSystem(unsigned int param_1,
                      const float *matA, const float *matB)
{
    if (param_1 >= this->field_0x180) {
        return;
    }
    if (((void **)this->field_0x184)[param_1] == 0) {
        return;
    }

    // local copies: matA -> world (local_a0), matB -> view (local_64)
    float world[15];
    float view[15];
    memcpy(world, matA, sizeof(world));
    memcpy(view, matB, sizeof(view));

    if (this->field_0x0 == 0) {
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

        void *grav = paintcanvas_ext_dss2_getgrav(this->field_0x34);
        double angle = *(double *)((char *)grav + 8) * g_dss2_gravscale_8af58;
        int ia = (int)(long long)angle;
        if (this->field_0x30 == 1) {
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

    paintcanvas_ext_dss2_ssdraw(this->field_0x34, world, view,
                                ((void **)this->field_0x184)[param_1]);
}
