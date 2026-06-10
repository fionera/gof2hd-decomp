#include "gof2/PaintCanvas.h"
// NOTE: Engine.h / Mesh.h / Node.h / String.h are intentionally NOT included.
// PaintCanvas.h is a self-contained translation unit that models the few
// cross-class shapes (Mesh/Transform/Node/Engine/String) it actually touches;
// pulling the real per-class headers in here causes redefinition/ambiguity
// conflicts because each was generated to be the only class in its TU.


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
extern "C" void paintcanvas_ext_dr_drawline2d(void *eng, int p, int n, bool b);
extern "C" void paintcanvas_ext_fbo_a(void *);
extern "C" void paintcanvas_ext_fbo_b(void *, int);
extern "C" void paintcanvas_ext_fbo_c(void *);
extern "C" void paintcanvas_ext_fbo_d(void *);
extern "C" void paintcanvas_ext_sprite_alluv(float, float, float, float, void *);
extern "C" void paintcanvas_ext_sprite_allsize(unsigned int, void *);
extern "C" void paintcanvas_ext_array_removeall_mesh(void *);
extern "C" void paintcanvas_ext_array_removeall_uint(void *);
extern "C" void paintcanvas_ext_array_removeall(void *);
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
extern "C" void paintcanvas_ext_dl_glEnable(unsigned int eng, bool on);
extern "C" float paintcanvas_ext_dl_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_dl_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dl_glVertexPointer(int a, int b, int c, void *p);
extern "C" void paintcanvas_ext_dl_glColorMask(void *eng, unsigned int cap, int on);
extern "C" void paintcanvas_ext_dl_glDrawArrays(int a, int b, int c);
extern "C" void paintcanvas_ext_dl_drawline2d(void *eng, int p, bool b);
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
extern "C" void paintcanvas_ext_array_add(void *, void *);
extern "C" void paintcanvas_ext_child_link(void *, void *, void *);
extern "C" void paintcanvas_ext_transform_dirty(void *);
extern "C" char *tcg_String_GetAEChar(AbyssEngine::String *name);
extern "C" void tcg_glActiveTexture(unsigned unit);
extern "C" void tcg_glBindTexture(unsigned target, unsigned tex);
extern "C" void tcg_operator_delete_arr(void *p);
extern "C" void tcg___stack_chk_fail();
extern "C" void grs_String_ctor_copy(String *self, const String *other, bool copy);
extern "C" void grs_String_ctor_cstr(String *self, const char *text, bool copy);
extern "C" void grs_String_substr(String *out, const String *self, unsigned begin, unsigned end);
extern "C" void grs_String_append(String *self, const String *other);
extern "C" void grs_String_dtor(String *self);
extern "C" void grs___stack_chk_fail();
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
extern "C" void grs2_String_ctor_copy(String *self, const String *other, bool copy);
extern "C" void grs2_String_dtor(String *self);
extern "C" void grs2_GetReverseString_impl(String *out, unsigned r1, String *tmp, int reverse);
extern "C" void grs2___stack_chk_fail();
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
extern "C" void paintcanvas_ext_dtor_arr_release_mesh(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_release_uint(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_mesh(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_uint(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_ss(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_mat(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_cam(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_tf(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_img(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_font(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_res(void *arr);
extern "C" void paintcanvas_ext_dtor_arr_dtor_tex(void *arr);
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
extern "C" void paintcanvas_ext_array_add_mesh(void *, void *);
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
extern "C" void RestoreImage2D(void *, void *);
extern "C" void paintcanvas_ext_rar_gldeltex(int n, void *ids);
extern "C" void paintcanvas_ext_rar_str_dtor(void *s);
extern "C" void paintcanvas_ext_rar_op_delete(void *p);
extern "C" void paintcanvas_ext_rar_fontrelease(void *eng, void *fontptr);
extern "C" void paintcanvas_ext_rar_arr_removeall_font(void *arr);
extern "C" void paintcanvas_ext_rar_img2drelease(void *eng, void *imgptr);
extern "C" void paintcanvas_ext_rar_arr_removeall_img(void *arr);
extern "C" void paintcanvas_ext_rar_meshrelease(void *eng, void *meshptr);
extern "C" void paintcanvas_ext_rar_arr_removeall_mesh(void *arr);
extern "C" void *paintcanvas_ext_rar_transform_dtor(void *p);
extern "C" void paintcanvas_ext_rar_arr_removeall_tf(void *arr);
extern "C" void paintcanvas_ext_rar_arr_removeall_cam(void *arr);
extern "C" void *paintcanvas_ext_rar_material_dtor(void *p);
extern "C" void paintcanvas_ext_rar_arr_removeall_mat(void *arr);
extern "C" void paintcanvas_ext_rar_ssrelease(void *eng, void *ssptr);
extern "C" void paintcanvas_ext_rar_arr_removeall_ss(void *arr);
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

// ---- GetImage2DWidth_78d9c.cpp ----
unsigned short GetImage2DWidth(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x14c) {
        char *img = (self->field_0x150)[index];
        return *(unsigned short *)(img + 0x10);
    }
    return 0;
}

// ---- CameraGetCurrent_7b968.cpp ----
unsigned int CameraGetCurrent(AbyssEngine::PaintCanvas *self)
{
    unsigned int cur = self->field_0x170;
    if (cur >= self->field_0x164) {
        cur = 0xffffffff;
    }
    return cur;
}

// ---- HasVibration_7c2f0.cpp ----
void HasVibration(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_has_vibration(self->field_0x34);
}

// ---- CameraGetLocal_7b7a4.cpp ----
void *CameraGetLocal(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    void *result;
    if (index < self->field_0x164) {
        result = (char *)(self->field_0x168)[index] + 0xc;
    } else {
        char tmp[60];
        result = (char *)self + 0xf8;
        MatrixIdentity(tmp, result);
    }
    return result;
}

// ---- DrawTextLines_7c6c0.cpp ----
void DrawTextLines(AbyssEngine::PaintCanvas *self, unsigned int font,
                   AbyssEngine::Array<void *> *arr, int x, int y, bool center)
{
    int xoff = 0;
    for (unsigned int i = 0; i < arr->size(); i++) {
        if (center) {
            int w = paintcanvas_ext_dtl_textwidth(self, font, arr->data()[i]);
            xoff = -(w >> 1);
        }
        paintcanvas_ext_dtl_drawstring(self, font, arr->data()[i], xoff + x, y, false);
        y += paintcanvas_ext_dtl_textheight(self, font);
    }
}

// ---- FillRectangle_79288.cpp ----
void FillRectangle(AbyssEngine::PaintCanvas *self, int x, int y, int w, int h)
{
    char abuf[60];
    float fx = (float)x;
    float fy = (float)y;
    float fx2 = (float)(x + w);
    float fy2 = (float)(h + y);

    float *vb = *(float **)(self->field_0x1c8 + 0x4);
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

    paintcanvas_ext_fr_setwvm(self, abuf);
    paintcanvas_ext_fr_glenable(self->field_0x34, 0xde1, false);
    paintcanvas_ext_fr_meshdraw(self->field_0x34, self->field_0x1c8);
    paintcanvas_ext_fr_glenable(self->field_0x34, 0xde1, true);
}

// ---- SpriteSystemSetRGBA_7a840.cpp ----
void SpriteSystemSetRGBA(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                         float a, float b, float c, float d)
{
    if (index < self->field_0x180) {
        unsigned short *s = (self->field_0x184)[index];
        if (s) {
            if ((unsigned int)*s <= (unsigned int)sub) {
                return;
            }
            return paintcanvas_ext_sprite_rgba(sub, a, b, c, d, s);
        }
    }
}

// ---- TransformCreate_7b070.cpp ----
void TransformCreate(AbyssEngine::PaintCanvas *self, unsigned int *out)
{
    void *obj = paintcanvas_ext_alloc(0x180);
    paintcanvas_ext_transform_ctor(obj);
    paintcanvas_ext_add_child(obj, (char *)self + 0x158);
    *out = self->field_0x158 - 1;
}

// ---- DrawRectangle_79368.cpp ----
__attribute__((visibility("hidden"))) extern char *const g_dr_flag_79368;


void DrawRectangle(AbyssEngine::PaintCanvas *self, int param_1, int param_2,
                   int param_3, int param_4)
{
    char abuf[60];

    double dw = (double)param_3;
    double dh = (double)param_4;
    double dy = (double)param_2;
    double dx = (double)param_1;
    float fVar4 = (float)(dw - 0.5 + dx);
    float fVar5 = (float)(dh - 0.5 + dy);

    float *v = (float *)((char *)self + 0x1d0);
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

    paintcanvas_ext_dr_setwvm(self, abuf);
    if (*g_dr_flag_79368 == 0) {
        paintcanvas_ext_dr_glLineWidth(1.0f);
        paintcanvas_ext_dr_glcap(self->field_0x34, 0xde1, 0);
        paintcanvas_ext_dr_glVertexPointer(2, 0x1406, 0, (char *)self + 0x1d0);
        paintcanvas_ext_dr_glColorMask(self->field_0x34, 0x8074, 1);
        paintcanvas_ext_dr_glColorMask(self->field_0x34, 0x8078, 0);
        paintcanvas_ext_dr_glColorMask(self->field_0x34, 0x8075, 0);
        paintcanvas_ext_dr_glColorMask(self->field_0x34, 0x8076, 0);
        paintcanvas_ext_dr_glDrawArrays(2, 0, 4);
        paintcanvas_ext_dr_glcap(self->field_0x34, 0xde1, 1);
    } else {
        paintcanvas_ext_dr_drawline2d(self->field_0x34, (int)((char *)self + 0x1d0), 4, true);
    }
}

// ---- GetImage2DHeight_78db4.cpp ----
unsigned short GetImage2DHeight(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x14c) {
        char *img = (self->field_0x150)[index];
        return *(unsigned short *)(img + 0x12);
    }
    return 0;
}

// ---- MeshSetUv_7a0b8.cpp ----
extern char paintcanvas_g_flipv;

void MeshSetUv(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
               float u, float v)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
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

// ---- StopDraw2FBO_7d08e.cpp ----
void StopDraw2FBO(AbyssEngine::PaintCanvas *self)
{
    paintcanvas_ext_fbo_a(self->field_0x34);
    paintcanvas_ext_fbo_b(self, 0);
    paintcanvas_ext_fbo_c(self->field_0x34);
    return paintcanvas_ext_fbo_d(self->field_0x34);
}

// ---- SpriteSystemSetAllUv_7a7bc.cpp ----
void SpriteSystemSetAllUv(AbyssEngine::PaintCanvas *self, unsigned int index,
                          float a, float b, float c, float d)
{
    if (self->field_0x180 <= index) {
        return;
    }
    void *sprite = (self->field_0x184)[index];
    if (sprite == 0) {
        return;
    }
    return paintcanvas_ext_sprite_alluv(a, b, c, d, sprite);
}

// ---- SpriteSystemGetPosition_7a9a2.cpp ----
using AbyssEngine::AEMath::Vector;

void SpriteSystemGetPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub, Vector &out)
{
    if (index < self->field_0x180) {
        unsigned short *s = (self->field_0x184)[index];
        if (s) {
            if ((unsigned int)*s <= (unsigned int)sub) {
                return;
            }
            float *p = (float *)(*(char **)((char *)s + 0x4) + sub * 12);
            out.x = p[0];
            out.y = p[1];
            out.z = p[2];
        }
    }
}

// ---- SpriteSystemSetAllSize_7a7a0.cpp ----
void SpriteSystemSetAllSize(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int size)
{
    if (index < self->field_0x180) {
        void *sprite = (self->field_0x184)[index];
        if (sprite) {
            return paintcanvas_ext_sprite_allsize(size, sprite);
        }
    }
}

// ---- RemoveAllMatsForGlow_7d484.cpp ----
void RemoveAllMatsForGlow(AbyssEngine::PaintCanvas *self)
{
    paintcanvas_ext_array_removeall_mesh((char *)self + 0x18c);
    paintcanvas_ext_array_removeall((char *)self + 0x198);
    paintcanvas_ext_array_removeall((char *)self + 0x1a4);
    paintcanvas_ext_array_removeall_uint((char *)self + 0x1b0);
    return paintcanvas_ext_array_removeall((char *)self + 0x1bc);
}

// ---- MaterialChange_7c220.cpp ----
void MaterialChange(AbyssEngine::PaintCanvas *self, unsigned int index,
                    unsigned int param3, unsigned int param4)
{
    if (index < self->field_0x174) {
        unsigned int *mat = (self->field_0x178)[index];
        mat[8] = param3;
        mat[0] = param4;
    }
}

// ---- DrawTextLines_7c726.cpp ----
void DrawTextLines(AbyssEngine::PaintCanvas *self, unsigned int font,
                   AbyssEngine::Array<void *> *arr, int x, int y, unsigned int p5, bool flag)
{
    int xoff = 0;
    for (unsigned int i = 0; i < arr->size(); i++) {
        if (flag == 0) {
            int w = paintcanvas_ext_dtl_textwidth(self, font, arr->data()[i]);
            xoff = (int)p5 - w;
        }
        paintcanvas_ext_dtl_drawstring(self, font, arr->data()[i], xoff + x, y, false);
        y += paintcanvas_ext_dtl_textheight(self, font);
    }
}

// ---- MeshResourceChangeMaterial_7a562.cpp ----
void MeshResourceChangeMaterial(AbyssEngine::PaintCanvas *self, unsigned short matId, unsigned short value)
{
    unsigned int count = self->field_0x134;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (self->field_0x138)[i];
        if (res) {
            if (*(unsigned short *)res == matId) {
                char *mat = *(char **)(res + 0xc);
                *(unsigned short *)(mat + 0x4) = value;
            }
        }
    }
}

// ---- TransformGetLocal_7b3cc.cpp ----
void *TransformGetLocal(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    void *result;
    if (index < self->field_0x158) {
        result = (self->field_0x15c)[index];
    } else {
        char tmp[60];
        result = (char *)self + 0xf8;
        MatrixIdentity(tmp, result);
    }
    return result;
}

// ---- MeshSetTangent_7a232.cpp ----
using AbyssEngine::AEMath::Vector;

void MeshSetTangent(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short vtx, const Vector &value)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x14);
        *(Vector *)(base + vtx * 12) = value;
    }
}

// ---- GetTextWidth_7c2fc.cpp ----
int GetTextWidth(AbyssEngine::PaintCanvas *self, unsigned int index, void *str,
                 unsigned int begin, unsigned int end)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        void *text = paintcanvas_ext_str_text(str);
        return paintcanvas_ext_text_width_range(font, text, begin, end - begin);
    }
    return 0;
}

// ---- MaterialGetMaterial_7c274.cpp ----
void *MaterialGetMaterial(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x174) {
        return (self->field_0x178)[index];
    }
    return 0;
}

// ---- EndBG_7e28c.cpp ----
__attribute__((visibility("hidden"))) char *paintcanvas_g_bg_flag;

void EndBG(AbyssEngine::PaintCanvas *self)
{
    *paintcanvas_g_bg_flag = self->field_0x1f0;
}

// ---- FindResource_79ad0.cpp ----
void *FindResource(AbyssEngine::PaintCanvas *self, unsigned short id)
{
    unsigned int count = self->field_0x134;
    void *found = 0;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (self->field_0x138)[i];
        if (res && *(unsigned short *)res == id) {
            found = res;
            break;
        }
    }
    return found;
}

// ---- MeshGetPointer_7a5bc.cpp ----
void *MeshGetPointer(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x24) {
        return (self->field_0x28)[index];
    }
    return 0;
}

// ---- MeshChangeResourceMaterial_7a510.cpp ----
void MeshChangeResourceMaterial(AbyssEngine::PaintCanvas *self, unsigned int meshIndex, unsigned int resId)
{
    char *r = paintcanvas_ext_find_res(self, resId);
    if (r) {
        int idx = *(int *)(r + 0x8);
        if (idx + 1 != 0) {
            void *mesh = (self->field_0x28)[meshIndex];
            void *mat = (self->field_0x178)[idx];
            return paintcanvas_ext_change_mat(self, mesh, mat);
        }
    }
}

// ---- SetColor_78460.cpp ----
void SetColor(AbyssEngine::PaintCanvas *self, unsigned int color)
{
    float c0 = (float)((double)(color >> 24) / 255.0);
    float c1 = (float)((double)((color >> 16) & 0xff) / 255.0);
    float c2 = (float)((double)((color >> 8) & 0xff) / 255.0);
    float c3 = (float)((double)(color & 0xff) / 255.0);
    self->field_0x1fc = c0;
    self->field_0x200 = c1;
    self->field_0x204 = c2;
    self->field_0x208 = c3;
    return paintcanvas_ext_setcolor(self->field_0x34, c0, c1, c2, c3);
}

// ---- Vibrate_7c2f6.cpp ----
void Vibrate(AbyssEngine::PaintCanvas *self, unsigned short)
{
    return paintcanvas_ext_vibrate(self->field_0x34);
}

// ---- SpriteSystemAddSize_7a8b2.cpp ----
void SpriteSystemAddSize(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub, short delta)
{
    if (index < self->field_0x180) {
        short *s = (self->field_0x184)[index];
        if (s) {
            if ((unsigned short)*s <= (unsigned int)sub) {
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

// ---- TransformSetLocal_7b2cc.cpp ----
using AbyssEngine::AEMath::Matrix;

void TransformSetLocal(AbyssEngine::PaintCanvas *self, unsigned int index, const Matrix &matrix)
{
    if (index < self->field_0x158) {
        Matrix *t = (self->field_0x15c)[index];
        *t = matrix;
    }
}

// ---- DrawTextLines_7c78c.cpp ----
void DrawTextLines(void *self, unsigned int p1, void *p2, int p3, int p4)
{
    return paintcanvas_ext_drawtextlines6(self, p1, p2, p3, p4, false);
}

// ---- GetMeshResourceId_77fb4.cpp ----
unsigned int GetMeshResourceId(AbyssEngine::PaintCanvas *self, void *name, unsigned short p2)
{
    for (unsigned int i = 0; i < self->field_0x134; ++i) {
        char *res = (self->field_0x138)[i];
        if (res && *(int *)(res + 0x4) == 4) {
            if (paintcanvas_ext_strcmp(name, **(void ***)(res + 0xc)) == 0) {
                char *res2 = (self->field_0x138)[i];
                if (*(unsigned short *)(*(char **)(res2 + 0xc) + 0x4) == p2) {
                    return *(unsigned short *)res2;
                }
            }
        }
    }
    return 0xffff;
}

// ---- GetTextWidth_7842c.cpp ----
int GetTextWidth(AbyssEngine::PaintCanvas *self, unsigned int index, void *str)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        return paintcanvas_ext_text_width(font, len, *(unsigned int *)((char *)str + 0x8));
    }
    return 0;
}

// ---- SpriteSystemSetSize_7a888.cpp ----
void SpriteSystemSetSize(AbyssEngine::PaintCanvas *self, unsigned int index,
                         unsigned int sub, unsigned short value)
{
    if (index < self->field_0x180) {
        char *s = (self->field_0x184)[index];
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

// ---- SetColor_7c938.cpp ----
void SetColor(AbyssEngine::PaintCanvas *self, unsigned char r, unsigned char g,
              unsigned char b, unsigned char a)
{
    float fr = (float)((double)(unsigned int)r / 255.0);
    float fg = (float)((double)(unsigned int)g / 255.0);
    float fb = (float)((double)(unsigned int)b / 255.0);
    float fa = (float)((double)(unsigned int)a / 255.0);
    self->field_0x1fc = fr;
    self->field_0x200 = fg;
    self->field_0x204 = fb;
    self->field_0x208 = fa;
    return paintcanvas_ext_setcolor(self->field_0x34, fr, fg, fb, fa);
}

// ---- CameraGetCurrentFactor1_7b97a.cpp ----
float CameraGetCurrentFactor1(AbyssEngine::PaintCanvas *self)
{
    unsigned int cur = self->field_0x170;
    if (cur >= self->field_0x164) {
        return 1.0f;
    }
    char *cam = (self->field_0x168)[cur];
    return *(float *)(cam + 0x48);
}

// ---- ClearDepth_7caa6.cpp ----
void ClearDepth()
{
    return paintcanvas_ext_clear(0x100);
}

// ---- SpriteSystemAddPosition_7aa14.cpp ----
void SpriteSystemAddPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                             float x, float y, float z)
{
    if (index < self->field_0x180) {
        char *s = (self->field_0x184)[index];
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

// ---- MeshSetBiTangent_7a262.cpp ----
using AbyssEngine::AEMath::Vector;

void MeshSetBiTangent(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short vtx, const Vector &value)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x18);
        *(Vector *)(base + vtx * 12) = value;
    }
}

// ---- GetScreenPosition_7bce8.cpp ----
using AbyssEngine::AEMath::Vector;


void *GetScreenPosition(AbyssEngine::PaintCanvas *self, Vector *a, Vector *b)
{
    char buf[60];
    float *m = (float *)buf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;
    return paintcanvas_ext_getscreenpos_m(self, buf, a, b);
}

// ---- FogEnable_7cee0.cpp ----
#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_fog_flag HIDDEN;
extern char *paintcanvas_g_fog_ptr HIDDEN;

void FogEnable(AbyssEngine::PaintCanvas *self, int mode, int enable)
{
    self->field_0x1f4 = enable;
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
    self->field_0x1f1 = (char)mode;
}

// ---- MeshSetColor_7a100.cpp ----
void MeshSetColor(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                  unsigned int color)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0xc) + sub * 0x10);
            p[0] = (float)((double)(color >> 24) / 255.0);
            p[1] = (float)((double)((color >> 16) & 0xff) / 255.0);
            p[2] = (float)((double)((color >> 8) & 0xff) / 255.0);
            p[3] = (float)((double)(color & 0xff) / 255.0);
        }
    }
}

// ---- SetProjOrthoMatrix_7c9b8.cpp ----
#define HIDDEN __attribute__((visibility("hidden")))
extern float *paintcanvas_g_pom_persp HIDDEN;
extern float *paintcanvas_g_pom_a HIDDEN;
extern float *paintcanvas_g_pom_b HIDDEN;

void SetProjOrthoMatrix(AbyssEngine::PaintCanvas *self)
{
    char *t = (char *)self;
    float g = *paintcanvas_g_pom_persp;
    if (g != -1.0f) {
        paintcanvas_ext_setprojmatrix3d(self, g, *paintcanvas_g_pom_a, *paintcanvas_g_pom_b);
    }
    void *eng = *(void **)(t + 0x34);
    float *r;
    r = (float *)(t + 0xa8); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = (float *)(t + 0x98); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = (float *)(t + 0x88); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = (float *)(t + 0x78); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;

    int w = paintcanvas_ext_getdisplaywidth(eng);
    *(float *)(t + 0x78) = (float)(2.0 / (double)w);

    int h = paintcanvas_ext_getdisplayheight(*(void **)(t + 0x34));
    *(float *)(t + 0xa0) = -0.05f;
    *(float *)(t + 0xb4) = 1.0f;
    *(float *)(t + 0xa8) = -1.0f;
    *(float *)(t + 0xac) = 1.0f;
    *(float *)(t + 0x8c) = -(float)(2.0 / (double)h);
}

// ---- MeshChangeMaterialIntern_7a372.cpp ----
void MeshChangeMaterialIntern(AbyssEngine::PaintCanvas *self, void *mesh, void *mat)
{
    if (mesh && mat) {
        mesh->field_0x30 = mat;
        return paintcanvas_ext_mat_intern(self, mesh->field_0x34);
    }
}

// ---- MeshResourceChangeAllMaterial_7a58e.cpp ----
void MeshResourceChangeAllMaterial(AbyssEngine::PaintCanvas *self, unsigned short matId, unsigned short value)
{
    unsigned int count = self->field_0x134;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (self->field_0x138)[i];
        if (res) {
            char *mat = *(char **)(res + 0xc);
            if (*(unsigned short *)(mat + 0x4) == matId) {
                *(unsigned short *)(mat + 0x4) = value;
            }
        }
    }
}

// ---- GetTextureResourceId_78056.cpp ----
unsigned int GetTextureResourceId(AbyssEngine::PaintCanvas *self, void *name)
{
    for (unsigned int i = 0; i < self->field_0x134; ++i) {
        char *res = (self->field_0x138)[i];
        if (res && *(int *)(res + 0x4) == 2) {
            char *n = *(char **)(res + 0xc);
            if (paintcanvas_ext_strcmp(name, *(void **)n) == 0) {
                return *(unsigned short *)((self->field_0x138)[i]);
            }
        }
    }
    return 0xffff;
}

// ---- GetColor_783d4.cpp ----
unsigned int GetColor(AbyssEngine::PaintCanvas *self)
{
    float a = self->field_0x1fc;
    float r = self->field_0x200;
    float g = self->field_0x204;
    float b = self->field_0x208;
    return ((unsigned int)(int)(r * 255.0f) << 16) +
           ((unsigned int)(int)(a * 255.0f) << 24) +
           ((unsigned int)(int)(g * 255.0f) << 8) +
           (unsigned int)(int)(b * 255.0f);
}

// ---- CameraSetCurrent_7b7ec.cpp ----
void CameraSetCurrent(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    self->field_0x170 = index;
    if (index < self->field_0x164) {
        unsigned int *cam = (self->field_0x168)[index];
        return paintcanvas_ext_camera_apply(self, cam[0], cam[1], cam[2]);
    }
}

// ---- MeshSetTriangleCount_7a00c.cpp ----
void MeshSetTriangleCount(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short count)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
        unsigned short cap = *(unsigned short *)(mesh + 0x2a);
        if (cap < count) {
            count = cap;
        }
        *(unsigned short *)(mesh + 0x28) = (unsigned short)(count * 3);
    }
}

// ---- DisableClip_7cb36.cpp ----
void DisableClip()
{
    return paintcanvas_ext_disable(0xc11);
}

// ---- TransformSetColor_7b410.cpp ----
void TransformSetColor(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int color)
{
    if (index < self->field_0x158) {
        char *obj = (self->field_0x15c)[index];
        *(unsigned int *)(obj + 0x48) = color;
    }
}

// ---- MeshSetColor_7a188.cpp ----
void MeshSetColor(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                  float r, float g, float b, float a)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0xc) + sub * 0x10);
            p[0] = r;
            p[1] = g;
            p[2] = b;
            p[3] = a;
        }
    }
}

// ---- MeshSetNormal_7a202.cpp ----
using AbyssEngine::AEMath::Vector;

void MeshSetNormal(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short vtx, const Vector &value)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x10);
        *(Vector *)(base + vtx * 12) = value;
    }
}

// ---- MeshClear2DMask_78b60.cpp ----
void MeshClear2DMask(AbyssEngine::PaintCanvas *self)
{
    self->field_0x20 = 0;
}

// ---- GetMeshResourceId_7800a.cpp ----
unsigned int GetMeshResourceId(AbyssEngine::PaintCanvas *self, void *name)
{
    for (unsigned int i = 0; i < self->field_0x134; ++i) {
        char *res = (self->field_0x138)[i];
        if (res && *(int *)(res + 0x4) == 4) {
            char *n = *(char **)(res + 0xc);
            if (paintcanvas_ext_strcmp(name, *(void **)n) == 0) {
                return *(unsigned short *)((self->field_0x138)[i]);
            }
        }
    }
    return 0xffff;
}

// ---- MeshTranslatePoint_7a064.cpp ----
void MeshTranslatePoint(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                        float x, float y, float z)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
        if ((unsigned int)*(unsigned short *)(mesh + 0x2) <= (unsigned int)sub) {
            return;
        }
        float *p = (float *)(*(char **)(mesh + 0x4) + sub * 12);
        p[0] = p[0] + x;
        p[1] = p[1] + y;
        p[2] = p[2] + z;
    }
}

// ---- CheckNUseRefractFBO_7d504.cpp ----
extern char paintcanvas_g_refract1;
extern char paintcanvas_g_refract2;

void CheckNUseRefractFBO(AbyssEngine::PaintCanvas *self, bool)
{
    if (paintcanvas_g_refract1 != 0 && paintcanvas_g_refract2 != 0 &&
        paintcanvas_ext_is_posteffect(self->field_0x34) == 0) {
        return paintcanvas_ext_use_refract(self->field_0x34);
    }
}

// ---- SpriteSystemSetUv_7a7f8.cpp ----
void SpriteSystemSetUv(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                       float a, float b, float c, float d)
{
    if (index < self->field_0x180) {
        unsigned short *s = (self->field_0x184)[index];
        if (s) {
            if ((unsigned int)*s <= (unsigned int)sub) {
                return;
            }
            return paintcanvas_ext_sprite_uv(sub, a, b, c, d, s);
        }
    }
}

// ---- SetWorldViewMatrix_78810.cpp ----
void SetWorldViewMatrix(AbyssEngine::PaintCanvas *self, const AbyssEngine::AEMath::Matrix &)
{
    return paintcanvas_ext_set_wvm(self->field_0x34);
}

// ---- CameraSetLocal_7b780.cpp ----
using AbyssEngine::AEMath::Matrix;

void CameraSetLocal(AbyssEngine::PaintCanvas *self, unsigned int index, const Matrix &matrix)
{
    if (index < self->field_0x164) {
        char *cam = (self->field_0x168)[index];
        *(Matrix *)(cam + 0xc) = matrix;
    }
}

// ---- SetShaderMode_7cb3e.cpp ----
void SetShaderMode(AbyssEngine::PaintCanvas *self, int mode)
{
    *(int *)(self->field_0x34 + 0x4a8) = mode;
}

// ---- MeshConvertToVBO_7a292.cpp ----
void MeshConvertToVBO(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x24) {
        return paintcanvas_ext_convert_vbo(
            (self->field_0x28)[index]);
    }
}

// ---- DrawImage2D_78c48.cpp ----
__attribute__((visibility("hidden"))) extern const float g_di2_one_88d90;
__attribute__((visibility("hidden"))) extern const float g_di2_def_88d94;


void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                 unsigned char param_4)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di2_restore(*(unsigned char *)(img + 0x14), img);
        img = (*(char ***)(t + 0x150))[param_1];
    }
    paintcanvas_ext_di2_settexture(self, *(unsigned int *)(img + 4), -1);

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
        if (param_1 < *(unsigned int *)(t + 0x14c)) {
            unsigned short w = *(unsigned short *)((*(char ***)(t + 0x150))[param_1] + 0x10);
            off = paintcanvas_ext_di2_unsignedtofloat(w, 0);
        }
        fx = off + fx;
    }
    if (param_4 & 2) {
        m[5] = g_di2_one_88d90;
        float off = g_di2_def_88d94;
        if (param_1 < *(unsigned int *)(t + 0x14c)) {
            unsigned short h = *(unsigned short *)((*(char ***)(t + 0x150))[param_1] + 0x12);
            off = paintcanvas_ext_di2_unsignedtofloat(h, 0);
        }
        fy = off + fy;
    }
    m[3] = fx;
    m[7] = fy;

    paintcanvas_ext_di2_setwvm(self, m);
    paintcanvas_ext_di2_gldisable(0xb44);
    paintcanvas_ext_di2_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_di2_glenable(0xb44);
}

// ---- FontSetYOffset_79c48.cpp ----
void FontSetYOffset(AbyssEngine::PaintCanvas *self, unsigned int index, short yoff)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        return paintcanvas_ext_font_set_yoff(font, yoff);
    }
}

// ---- SetBlendMode_7cb48.cpp ----
__attribute__((visibility("hidden"))) extern char *const g_sbm_flag_8cb62;
__attribute__((visibility("hidden"))) extern const unsigned int g_sbm_const_8ce34;


void SetBlendMode(AbyssEngine::PaintCanvas *self, int param_2)
{
    char *t = (char *)self;
    paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 0);

    char *flag = g_sbm_flag_8cb62;
    if (*flag != 0) {
        paintcanvas_ext_sbm_glenablecap(*(void **)(t + 0x34), g_sbm_const_8ce34, 0);
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
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        /* fallthrough */
    case 0:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 7:
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 8:
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 9:
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 10:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_glDepthMask(1);
        paintcanvas_ext_sbm_setalpha(*(void **)(t + 0x34), 0x1000000, 1);
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
            paintcanvas_ext_sbm_setalpha(*(void **)(t + 0x34), g_sbm_const_8ce34, 1);
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

// ---- GetLineArray_7c488.cpp ----
__attribute__((visibility("hidden"))) extern const char g_gla_nl_8c4c0[];


void GetLineArray(AbyssEngine::PaintCanvas *self, unsigned int param_1, void *param_2, int param_3,
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
        paintcanvas_ext_gla_getline(self, param_1, line, param_3, acc);
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
        paintcanvas_ext_gla_getline(self, param_1, line, param_3, out);
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

// ---- AddResource_77f84.cpp ----
void AddResource(AbyssEngine::PaintCanvas *self, void *resource)
{
    return paintcanvas_ext_add_resource(resource, (char *)self + 0x134);
}

// ---- TransformRemoveMeshId_7b1a4.cpp ----
void TransformRemoveMeshId(AbyssEngine::PaintCanvas *self, unsigned int transformIndex, unsigned int meshIndex)
{
    if (transformIndex < self->field_0x158 &&
        meshIndex < self->field_0x24) {
        void *mesh = (self->field_0x28)[meshIndex];
        char *t = (self->field_0x15c)[transformIndex];
        return paintcanvas_ext_remove_meshid(mesh, t + 0x3c);
    }
}

// ---- GetHeight_78fba.cpp ----
void GetHeight(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_height(self->field_0x34);
}

// ---- ChangeCubeTexture_7c288.cpp ----
extern char paintcanvas_g_cube_enabled;
extern int paintcanvas_g_cube_slot;

void ChangeCubeTexture(AbyssEngine::PaintCanvas *self, unsigned int idx)
{
    if (paintcanvas_g_cube_enabled != 0 && idx < self->field_0x10) {
        char *tex = (self->field_0x14)[idx];
        if (*(unsigned char *)(tex + 0x14) == 0) {
            return paintcanvas_ext_cube_tail(paintcanvas_ext_cube_restore(tex + 0x4));
        }
        paintcanvas_g_cube_slot = idx;
        paintcanvas_ext_gl_a(0x84c7);
        char *tex2 = (self->field_0x14)[idx];
        paintcanvas_ext_gl_bind(0x8513, *(unsigned int *)tex2);
        return paintcanvas_ext_gl_c(0x84c0);
    }
}

// ---- TransformCreate_7af64.cpp ----
namespace AbyssEngine {
void ArrayAddTransform(Transform *item, void *array);
}

void TransformCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_tfc_findres(self, param_1);
    if (res == 0) {
        return;
    }
    if (*(unsigned int *)(res + 8) != 0xffffffff) {
        *param_2 = *(unsigned int *)(res + 8);
        return;
    }
    char *info = *(char **)(res + 0xc);
    char *tf = (char *)paintcanvas_ext_tfc_new_transform();
    AbyssEngine::ArrayAddTransform((AbyssEngine::Transform *)tf, (char *)self + 0x158);
    unsigned int idx = self->field_0x158 - 1;
    *(unsigned int *)(res + 8) = idx;
    *param_2 = idx;
    paintcanvas_ext_tfc_mtx_assign(tf, info);

    unsigned int childMesh = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x3c); i++) {
        unsigned short mid = *(unsigned short *)(*(char **)(info + 0x40) + i * 2);
        paintcanvas_ext_tfc_meshcreate(self, mid, &childMesh, false);
    }
    unsigned int childTf = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x44); i++) {
        unsigned short tid = *(unsigned short *)(*(char **)(info + 0x48) + i * 2);
        TransformCreate(self, tid, &childTf);
    }
}

// ---- End2d_7e378.cpp ----
void End2d(AbyssEngine::PaintCanvas *self)
{
    char buf[60];
    int v = self->field_0xc;
    if (v >= 1) {
        float *m = (float *)buf;
        *(short *)(self->field_0x8 + 0x28) = (short)(v * 6);
        m[0] = 1.0f;
        m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
        m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
        m[14] = 1.0f;
        paintcanvas_ext_set_wvm2(self, buf);
        paintcanvas_ext_meshdraw(self->field_0x34, self->field_0x8);
    }
}

// ---- TextureCreate_79b00.cpp ----
extern "C" void paintcanvas_ext_texcreate6(unsigned short, unsigned int &, void *, void *,
                                           unsigned int &, bool);

void TextureCreate(unsigned short w, unsigned int &ref, bool b)
{
    return paintcanvas_ext_texcreate6(w, ref, 0, 0, *(unsigned int *)&b, b);
}

// ---- MeshChangeMaterialIntern_7a322.cpp ----
void MeshChangeMaterialIntern(AbyssEngine::PaintCanvas *self, char *transform, void *material)
{
    if (transform && material) {
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x3c); ++i) {
            void *m = (*(void ***)(transform + 0x40))[i];
            paintcanvas_ext_mesh_changemat(self, m, material);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x4c); ++i) {
            void *c = (*(void ***)(transform + 0x50))[i];
            paintcanvas_ext_transform_changemat(self, c, material);
        }
    }
}

// ---- GetTextHeight_7c33c.cpp ----
int GetTextHeight(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        return paintcanvas_ext_text_height(font);
    }
    return 0;
}

// ---- Initialize_7e6bc.cpp ----
__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b4;
__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b8;


void Initialize(AbyssEngine::PaintCanvas *self, bool param_1)
{
    char *t = (char *)self;
    *(unsigned int *)(t + 0x30) = param_1 ? 2 : 0;
    paintcanvas_ext_init_setorientation(*(void **)(t + 0x34));

    // zero out the four 4-float blocks at 0x78, 0x88, 0x98, 0xa8
    memset(t + 0x78, 0, 0x10);
    memset(t + 0x88, 0, 0x10);
    memset(t + 0x98, 0, 0x10);
    memset(t + 0xa8, 0, 0x10);

    int orient = *(int *)(t + 0x30);
    int w = paintcanvas_ext_init_dispwidth(*(void **)(t + 0x34));
    float fw = paintcanvas_ext_init_signedtofloat(w, 0);
    int h = paintcanvas_ext_init_dispheight(*(void **)(t + 0x34));
    float fh = paintcanvas_ext_init_signedtofloat(h, 0);

    float ymul;
    if (orient == 2) {
        *(float *)(t + 0x78) = 2.0f / fw;
        ymul = fh;
    } else {
        memset(t + 0xb8, 0, 0x10);
        memset(t + 0xc8, 0, 0x10);
        memset(t + 0xd8, 0, 0x10);
        memset(t + 0xe4, 0, 0x10);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fh;
        *(unsigned int *)(t + 0xbc) = g_init_const_7e7b4;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = 0x3f800000;
        *(float *)(t + 0xec) = fw;
        ymul = fw;
    }
    *(unsigned int *)(t + 0xa0) = g_init_const_7e7b8;
    *(unsigned int *)(t + 0xb4) = 0x3f800000;
    *(unsigned int *)(t + 0xa8) = g_init_const_7e7b4;
    *(unsigned int *)(t + 0xac) = 0x3f800000;
    *(float *)(t + 0x8c) = -2.0f / ymul;

    paintcanvas_ext_init_setpersp(self, 16384.0f, 512.0f, 65536.0f);
}

// ---- DrawMesh_7968c.cpp ----
void DrawMesh(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x24) {
        void *mesh = (self->field_0x28)[index];
        return paintcanvas_ext_draw_mesh(self->field_0x34, mesh);
    }
}

// ---- MeshGetTriCount_7a5cc.cpp ----
int MeshGetTriCount(AbyssEngine::PaintCanvas *self, char *mesh)
{
    if (mesh) {
        int tri;
        if (*(void **)(mesh + 0x34) == 0) {
            tri = 0;
        } else {
            tri = paintcanvas_ext_transform_tricount(self, *(void **)(mesh + 0x34));
        }
        int q = (int)__aeabi_uidiv(*(unsigned short *)(mesh + 0x28), 3);
        return q + tri;
    }
    return 0;
}

// ---- FontGetYOffset_79c60.cpp ----
int FontGetYOffset(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        return paintcanvas_ext_font_get_yoff(font);
    }
    return 0;
}

// ---- SetProjectionMatrix3d_7b808.cpp ----
__attribute__((visibility("hidden"))) extern float *const g_spm_p0_7b826;
__attribute__((visibility("hidden"))) extern float *const g_spm_p1_7b82a;
__attribute__((visibility("hidden"))) extern float *const g_spm_p2_7b82c;
__attribute__((visibility("hidden"))) extern float *const g_spm_p3_7b82e;
__attribute__((visibility("hidden"))) extern float *const g_spm_p4_7b830;
__attribute__((visibility("hidden"))) extern const unsigned int g_spm_const_7b950;


void SetProjectionMatrix3d(AbyssEngine::PaintCanvas *self, float param_1, float param_2,
                           float param_3)
{
    char *t = (char *)self;

    // stash parameters into the engine globals
    *g_spm_p0_7b826 = param_3;
    *g_spm_p1_7b82a = param_2;
    *g_spm_p4_7b830 = param_1;
    *g_spm_p2_7b82c = param_2;
    *g_spm_p3_7b82e = param_3;

    int w = paintcanvas_ext_spm_dispwidth(*(void **)(t + 0x34));
    int h = paintcanvas_ext_spm_dispheight(*(void **)(t + 0x34));
    float half = param_1 * 0.5f;
    float s = paintcanvas_ext_spm_sinf(half);
    float c = paintcanvas_ext_spm_cosf(half);
    float fw = paintcanvas_ext_spm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_spm_signedtofloat(h, 0);

    // zero the four matrix rows at 0x38,0x48,0x58,0x68
    memset(t + 0x68, 0, 0x10);
    memset(t + 0x58, 0, 0x10);
    memset(t + 0x48, 0, 0x10);
    memset(t + 0x38, 0, 0x10);

    if (*(unsigned int *)(t + 0x30) <= 3) {
        float aspect = fw / fh;
        float f = 1.0f / (s / c);
        switch (*(unsigned int *)(t + 0x30)) {
        case 0:
            *(float *)(t + 0x48) = f / aspect;
            *(float *)(t + 0x3c) = -f;
            break;
        case 1:
            *(float *)(t + 0x48) = -(f / aspect);
            *(float *)(t + 0x3c) = -f;
            break;
        case 2:
            *(float *)(t + 0x4c) = -f;
            *(float *)(t + 0x38) = f / aspect;
            break;
        case 3:
            *(float *)(t + 0x4c) = -f;
            *(float *)(t + 0x38) = -(f / aspect);
            break;
        }
    }

    *(unsigned int *)(t + 0x64) = g_spm_const_7b950;
    *(float *)(t + 0x60) = (param_2 + param_3) / (param_2 - param_3);
    *(float *)(t + 0x70) = ((param_3 + param_3) * param_2) / (param_2 - param_3);
}

// ---- DrawSpriteSystem_7aa70.cpp ----
__attribute__((visibility("hidden"))) extern const double g_dss1_gravscale_8ac10;


void DrawSpriteSystem(AbyssEngine::PaintCanvas *self, unsigned int param_1)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x180)) {
        return;
    }
    if ((*(void ***)(t + 0x184))[param_1] == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f;
    worldM[5] = 1.0f;
    worldM[14] = 1.0f;

    if (*(unsigned int *)(t + 0x170) < *(unsigned int *)(t + 0x164)) {
        if (*t == 0) {
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

            void *grav = paintcanvas_ext_dss1_getgrav(*(void **)(t + 0x34));
            double angle = *(double *)((char *)grav + 8) * g_dss1_gravscale_8ac10;
            float a = (float)angle;
            int orient = *(int *)(t + 0x30);
            float rot = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dss1_sinf(rot);
            float c = paintcanvas_ext_dss1_cosf(rot);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            // copy the active camera's view matrix (at cam+0xc) into scratch
            char *cam = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
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
    paintcanvas_ext_dss1_ssdraw(*(void **)(t + 0x34), ident, worldM,
                                (*(void ***)(t + 0x184))[param_1]);
}

// ---- MeshSetPoint_7a02a.cpp ----
float MeshSetPoint(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short vtx,
                   float x, float y, float z)
{
    if (index < self->field_0x24) {
        char *mesh = (self->field_0x28)[index];
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

// ---- GetLine_7c354.cpp ----
__attribute__((visibility("hidden"))) extern const char g_getline_empty_7c428[];

extern "C" int paintcanvas_ext_gl_textwidth(void *self, unsigned int font, void *str,
                                            unsigned int begin, unsigned int end);

void GetLine(AbyssEngine::PaintCanvas *self, unsigned int param_1, void *param_3, int param_4,
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
        width += paintcanvas_ext_gl_textwidth(self, param_1, param_3, i, next);
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

// ---- DrawLine_794d8.cpp ----
__attribute__((visibility("hidden"))) extern char *const g_dl_flag_794ee;


void DrawLine(AbyssEngine::PaintCanvas *self, int param_1, int param_2, int param_3, int param_4)
{
    char *t = (char *)self;
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

    float *v = (float *)(t + 0x1d0);

    if (*g_dl_flag_794ee == 0) {
        paintcanvas_ext_dl_glLineWidth(1.0f);
        paintcanvas_ext_dl_glEnable(*(unsigned int *)(t + 0x34), true);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_setwvm(self, abuf);
        paintcanvas_ext_dl_glVertexPointer(2, 0x1406, 0, t + 0x1d0);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8074, 1);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8078, 0);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8075, 0);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8076, 0);
        paintcanvas_ext_dl_glDrawArrays(1, 0, 2);
        paintcanvas_ext_dl_glEnable(*(unsigned int *)(t + 0x34), true);
    } else {
        paintcanvas_ext_dl_setwvm(self, abuf);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_drawline2d(*(void **)(t + 0x34), (int)(t + 0x1d0), true);
    }
}

// ---- MeshChangeMaterial_7a540.cpp ----
void MeshChangeMaterial(AbyssEngine::PaintCanvas *self, unsigned int meshIndex, unsigned int matIndex)
{
    if (matIndex < self->field_0x174 &&
        meshIndex < self->field_0x24) {
        void *mesh = (self->field_0x28)[meshIndex];
        void *mat = (self->field_0x178)[matIndex];
        return paintcanvas_ext_change_mat(self, mesh, mat);
    }
}

// ---- CameraSetPerspective_7b99e.cpp ----
void CameraSetPerspective(AbyssEngine::PaintCanvas *self, unsigned int index, float a, float b, float c)
{
    if (index < self->field_0x164) {
        float w = (float)paintcanvas_ext_get_w(self);
        float h = (float)paintcanvas_ext_get_h(self);
        void *cam = (self->field_0x168)[index];
        paintcanvas_ext_cam_persp4(a, b, c, w, h, cam);
        if (self->field_0x170 == index) {
            return paintcanvas_ext_cam_setcur(self, index);
        }
    }
}

// ---- End3d_7d53c.cpp ----
// AbyssEngine::PaintCanvas::End3d() — ends the current 3D pass (counterpart to Begin3d).
// Ghidra mis-decoded this tiny tail-call; it simply forwards to the internal end-3D helper.

void End3d(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_end3d(self);
}

// ---- MeshCloneMaterial_7a380.cpp ----
void MeshCloneMaterial(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int *out)
{
    int result;
    if (index < self->field_0x24) {
        char *obj = (char *)paintcanvas_ext_alloc(0x74);
        char *mesh = (self->field_0x28)[index];
        paintcanvas_ext_material_clone(obj, *(void **)(mesh + 0x30));
        paintcanvas_ext_material_add(obj, (char *)self + 0x174);
        result = (int)self->field_0x174 - 1;
    } else {
        result = -1;
    }
    *out = result;
}

// ---- GetGravValue_7e7bc.cpp ----
void GetGravValue(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_grav(self->field_0x34);
}

// ---- BeginBG_7e1e0.cpp ----
#define HIDDEN __attribute__((visibility("hidden")))

extern char paintcanvas_g_bg_a;
extern char paintcanvas_g_bg_b;

void BeginBG(AbyssEngine::PaintCanvas *self)
{
    *(unsigned char *)(self->field_0x34 + 0xfd) = 0;
    paintcanvas_ext_gl_enable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_color(self->field_0x34, 1.0f, 1.0f, 1.0f, 1.0f);
    self->field_0x1f0 = paintcanvas_g_bg_a;
    char flag = paintcanvas_g_bg_b;
    paintcanvas_g_bg_a = 0;
    if (flag != 0) {
        return paintcanvas_ext_matgl_load(self->field_0x34, (char *)self + 0x38);
    }
    paintcanvas_ext_glMatrixMode(0x1702);
    paintcanvas_ext_gl_loadidentity();
    paintcanvas_ext_gl_ortho_persp(1.0f, 0.3f, 1.0f);
    paintcanvas_ext_glMatrixMode(0x1701);
    paintcanvas_ext_gl_loadmatrix((char *)self + 0x38);
    paintcanvas_ext_glMatrixMode(0x1700);
    return paintcanvas_ext_gl_done();
}

// ---- FontCreate_79b14.cpp ----
namespace AbyssEngine {
void ArrayAddImageFont(ImageFont *item, void *array);
}

void FontCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2,
                bool param_3)
{
    char *res = (char *)paintcanvas_ext_fc_findres(self, param_1);
    if (res == 0) {
        return;
    }
    unsigned short *info = *(unsigned short **)(res + 0xc);
    char *texres = (char *)paintcanvas_ext_fc_findres(self, *info);
    if (texres == 0) {
        return;
    }
    if (*(int *)(texres + 8) == -1) {
        paintcanvas_ext_fc_texcreate(self, *info, true);
    }
    if (*(int *)(res + 8) != -1) {
        *param_2 = *(unsigned int *)(res + 8);
        return;
    }
    void *font = 0;
    char *texpath = (char *)**(char ***)(texres + 0xc);
    int ok = paintcanvas_ext_fc_fontfromfile(self->field_0x34, texpath, info[1], &font);
    if (ok != 1) {
        return;
    }
    if (*(int *)(texres + 8) != -1) {
        *(int *)font = *(int *)(texres + 8);
    }
    AbyssEngine::ArrayAddImageFont((AbyssEngine::ImageFont *)font, (char *)self + 0x140);
    int idx = self->field_0x140 - 1;
    *(int *)(res + 8) = idx;
    *param_2 = idx;

    char *eng = self->field_0x34;
    int cur = *(int *)(eng + 0x78);
    if (cur == -1) {
        *(int *)(eng + 0x78) = idx;
    } else {
        char *curFont = (self->field_0x144)[cur];
        if (*(unsigned short *)curFont <= *(unsigned short *)font) {
            int curH = paintcanvas_ext_fc_fontheight(curFont);
            int newH = paintcanvas_ext_fc_fontheight(font);
            if (newH < curH) {
                *(int *)(self->field_0x34 + 0x78) = *param_2;
            }
        }
    }
}

// ---- SetResourceList_77f40.cpp ----
void SetResourceList(AbyssEngine::PaintCanvas *self, void **list, unsigned int count)
{
    return paintcanvas_ext_set_reslist(list, count, (char *)self + 0x134);
}

// ---- MaterialResourceChangeTexture_7c238.cpp ----
void MaterialResourceChangeTexture(AbyssEngine::PaintCanvas *self, unsigned short resId,
                                   void *texture, unsigned int slot)
{
    if (slot <= 7) {
        char *r = paintcanvas_ext_find_res(self, resId);
        if (r) {
            unsigned int matIdx = *(unsigned int *)(r + 0x8);
            if (matIdx + 1 != 0 && matIdx < self->field_0x174) {
                char *mat = (self->field_0x178)[matIdx];
                *(void **)(mat + slot * 4) = texture;
            }
        }
    }
}

// ---- TransformAddChild_7b2ee.cpp ----
void TransformAddChild(AbyssEngine::PaintCanvas *self, unsigned int parent, unsigned int child)
{
    unsigned int count = self->field_0x158;
    if (parent != child && child < count && parent < count) {
        char **arr = self->field_0x15c;
        char *p = arr[parent];
        char *c = arr[child];
        paintcanvas_ext_array_add(c, p + 0x4c);
        char **arr2 = self->field_0x15c;
        char *p2 = arr2[parent];
        char *c2 = arr2[child];
        paintcanvas_ext_child_link(p2 + 0xd4, c2, p2);
        char **arr3 = self->field_0x15c;
        return paintcanvas_ext_transform_dirty(arr3[parent]);
    }
}

// ---- TextureCreateGlobal_799b8.cpp ----
// AbyssEngine::PaintCanvas::TextureCreateGlobal(String name, unsigned unit) — loads a texture
// file into the given texture unit and, on success, leaves it unbound on that unit.
extern "C" int tcg_TextureCreateFromFile(void *engine, const char *path, void *cb, void *ud,
                                         unsigned *outId, bool b, float f);

__attribute__((visibility("hidden"))) extern int **g_tcg_canary;

void TextureCreateGlobal(AbyssEngine::PaintCanvas *self, AbyssEngine::String *name, int unit)
{
    int *canary = *g_tcg_canary;
    int saved = *canary;

    char *path = tcg_String_GetAEChar(name);
    unsigned outId;
    int rc = tcg_TextureCreateFromFile(self->field_0x34, path, 0, 0, &outId, false,
                                       0.0f);
    if (rc == 1) {
        tcg_glActiveTexture(unit + 0x84c0);
        tcg_glBindTexture(0xde1, 0);
        tcg_glActiveTexture(0x84c0);
    }
    tcg_operator_delete_arr(path);

    if (*canary != saved)
        tcg___stack_chk_fail();
}

// ---- Begin3d_7ce44.cpp ----
#define HIDDEN __attribute__((visibility("hidden")))

extern char paintcanvas_g_use_matgl;

void Begin3d(AbyssEngine::PaintCanvas *self)
{
    *(unsigned char *)(self->field_0x34 + 0xfd) = 0;
    paintcanvas_ext_gl_disable(0xb71);
    paintcanvas_ext_gl_depthmask(1);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_color(self->field_0x34, 1.0f, 1.0f, 1.0f, 1.0f);
    if (paintcanvas_g_use_matgl != 0) {
        return paintcanvas_ext_matgl_load(self->field_0x34, (char *)self + 0x38);
    }
    paintcanvas_ext_glMatrixMode(0x1702);
    paintcanvas_ext_gl_loadidentity();
    paintcanvas_ext_gl_ortho_persp(1.0f, 0.3f, 1.0f);
    paintcanvas_ext_glMatrixMode(0x1701);
    paintcanvas_ext_gl_loadmatrix((char *)self + 0x38);
    paintcanvas_ext_glMatrixMode(0x1700);
    return paintcanvas_ext_gl_done();
}

// ---- TransformAddMesh_7b0b0.cpp ----
// AbyssEngine::PaintCanvas::TransformAddMesh(unsigned transformIndex, unsigned short meshId, bool b)
// Adds a mesh (by global mesh id) to a transform. Ghidra mis-decoded this tiny wrapper; modeled
// on TransformAddMeshId — it creates/resolves the mesh then forwards to the internal add helper.
extern "C" void paintcanvas_ext_transform_addmesh(AbyssEngine::PaintCanvas *, void *,
                                                  unsigned short, bool);

void TransformAddMesh(AbyssEngine::PaintCanvas *self, unsigned int transformIndex,
                      unsigned short meshId, bool b)
{
    if (transformIndex < self->field_0x158) {
        void *t = (self->field_0x15c)[transformIndex];
        return paintcanvas_ext_transform_addmesh(self, t, meshId, b);
    }
}

// ---- GetReverseString_78104.cpp ----
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
        grs_String_ctor_copy(out, in, false);
    } else {
        grs_String_ctor_cstr(out, g_grs_empty, false);
        int i = in->field_0x8;
        while (--i >= 0) {
            String piece;
            grs_String_substr(&piece, in, (unsigned)i, (unsigned)(i + 1));
            grs_String_append(out, &piece);
            grs_String_dtor(&piece);
        }
    }

    if (*canary != saved)
        grs___stack_chk_fail();
}

// ---- SpriteSystemCreate_7a648.cpp ----
namespace AbyssEngine {
void ArrayAddSpriteSystem(SpriteSystem *item, void *array);
}

void SpriteSystemCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, bool param_2,
                        unsigned short param_3, unsigned int *param_4)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_ss2_sscreate(self->field_0x34, param_1, param_2, &ss);
    if (ok == 1) {
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_ss2_matcreate(self, param_3, &mat);
        if (mat <= self->field_0x174) {
            void *node = *(void **)((char *)ss + 0x10);
            node->field_0x30 =
                *(unsigned int *)(self->field_0x178 + mat * 4);
        }
        unsigned int i;
        for (i = 0; i < self->field_0x180; i++) {
            void **slot = (void **)(self->field_0x184 + i * 4);
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
        AbyssEngine::ArrayAddSpriteSystem((AbyssEngine::SpriteSystem *)ss,
                                          (char *)self + 0x180);
        result = self->field_0x180 - 1;
    } else {
        result = 0xffffffff;
    }
    *param_4 = result;
}

// ---- GetScreenPosition_7bfb4.cpp ----
void GetScreenPosition(AbyssEngine::PaintCanvas *self, char *param_1, char *param_2)
{
    char *t = (char *)self;
    if (*(unsigned int *)(t + 0x170) >= *(unsigned int *)(t + 0x164)) {
        return;
    }

    // copy {m[0xc], m[0x1c], m[0x2c]} into the output vector
    float src[3];
    src[0] = *(float *)(param_1 + 0xc);
    src[1] = *(float *)(param_1 + 0x1c);
    src[2] = *(float *)(param_1 + 0x2c);
    paintcanvas_ext_gsp_vec_assign(param_2, src);

    char *cam = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
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
    int w0 = paintcanvas_ext_gsp_getwidth(self);
    int w1 = paintcanvas_ext_gsp_getwidth(self);
    double fw = (double)paintcanvas_ext_gsp_signedtofloat(w0, 0);
    double termY = ((double)*(float *)(param_2 + 4) * 0.5) / (double)denomY;
    double halfW = (double)paintcanvas_ext_gsp_signedtofloat(w1 >> 1, 0);
    *(float *)param_2 = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp_getheight(self);
    double fh = (double)paintcanvas_ext_gsp_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp_getheight(self);
    double halfH = (double)paintcanvas_ext_gsp_signedtofloat(h1 >> 1, 0);
    *(float *)(param_2 + 4) = (float)(halfH + termY * fh);

    // remaining clamp/visibility checks have no observable side-effects on output
    // (they compute booleans only); reproduce the visible reads to keep behavior.
    char *cam2 = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    if (*(float *)(param_2 + 8) <= *(float *)(cam2 + 4)) {
        float fy = *(float *)(param_2 + 4);
        if (fy >= 0.0f) {
            float fx = *(float *)param_2;
            if (fx >= 0.0f) {
                int ww = paintcanvas_ext_gsp_getwidth(self);
                float fww = paintcanvas_ext_gsp_signedtofloat(ww, 0);
                if (fx < fww) {
                    int hh = paintcanvas_ext_gsp_getheight(self);
                    paintcanvas_ext_gsp_signedtofloat(hh, 0);
                }
            }
        }
    }
}

// ---- SpriteSystemCreate_7a71c.cpp ----
namespace AbyssEngine {
void ArrayAddSpriteSystem(SpriteSystem *item, void *array);
}

void SpriteSystemCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1,
                        bool param_2, unsigned int *param_3)
{
    void *ss = 0;
    unsigned int result;
    int ok = paintcanvas_ext_sscreate(self->field_0x34, param_1, param_2, &ss);
    if (ok == 1) {
        unsigned int i;
        for (i = 0; i < self->field_0x180; i++) {
            void **slot = (void **)(self->field_0x184 + i * 4);
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
        AbyssEngine::ArrayAddSpriteSystem((AbyssEngine::SpriteSystem *)ss,
                                          (char *)self + 0x180);
        result = self->field_0x180 - 1;
    } else {
        result = 0xffffffff;
    }
    *param_3 = result;
}

// ---- MaterialCreate_7c13c.cpp ----
void MaterialCreate(AbyssEngine::PaintCanvas *self, unsigned int *out, void *p2, void *p3)
{
    char *obj = (char *)paintcanvas_ext_alloc(0x74);
    paintcanvas_ext_material_ctor(obj);
    *(void **)(obj + 0x0) = p3;
    *(void **)(obj + 0x20) = p2;
    paintcanvas_ext_material_add(obj, (char *)self + 0x174);
    *out = self->field_0x174 - 1;
}

// ---- CameraIsSphereinViewFrustum_7bbb8.cpp ----
__attribute__((visibility("hidden"))) extern const double g_cisvf_gravscale_7bcd8;


int CameraIsSphereinViewFrustum(AbyssEngine::PaintCanvas *self, void *param_1, float param_2)
{
    char *t = (char *)self;
    if (param_2 == 0.0f ||
        *(unsigned int *)(t + 0x164) <= *(unsigned int *)(t + 0x170)) {
        return 1;
    }

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[14] = 1.0f;
    char scratch[60];
    paintcanvas_ext_cisvf_matidentity(scratch, m);

    void *grav = paintcanvas_ext_cisvf_getgrav(*(void **)(t + 0x34));
    double angle = *(double *)((char *)grav + 8) * g_cisvf_gravscale_7bcd8;
    float a = (float)angle;
    int orient = *(int *)(t + 0x30);
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cisvf_sinf(rot);
    float c = paintcanvas_ext_cisvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    return paintcanvas_ext_cisvf_inner(param_1, param_2, m, cam);
}

// ---- MeshChangeShaderAnimValue_7a2a4.cpp ----
void MeshChangeShaderAnimValue(AbyssEngine::PaintCanvas *self, char *mesh, float value, unsigned int mode)
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
    return paintcanvas_ext_shader_anim(self, *(void **)(mesh + 0x34));
}

// ---- Resume_7e828.cpp ----
__attribute__((visibility("hidden"))) extern int *const g_resume_curtex_7e828;

extern "C" int paintcanvas_ext_rs_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);

void Resume(AbyssEngine::PaintCanvas *self)
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < self->field_0x10; i++) {
        char *res = (self->field_0x14)[i];
        char *path = paintcanvas_ext_rs_getAEChar(res + 4);
        float f = *(float *)((self->field_0x14)[i] + 0x10);
        int ok = paintcanvas_ext_rs_texfromfile(self->field_0x34, path, 0, 0,
                                                &out, false, f);
        if (ok == 1) {
            *(int *)((self->field_0x14)[i]) = 0;
        }
        paintcanvas_ext_rs_deletearr(path);
    }
    int *cur = g_resume_curtex_7e828;
    if (*cur != 0) {
        paintcanvas_ext_rs_glActiveTexture(0x84c7);
        char *res = (self->field_0x14)[*cur];
        paintcanvas_ext_rs_glBindTexture(0x8513, *(unsigned int *)res);
        paintcanvas_ext_rs_glActiveTexture(0x84c0);
    }
}

// ---- TransformAddMeshId_7b1c6.cpp ----
namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
}


void TransformAddMeshId(AbyssEngine::PaintCanvas *self, unsigned int param_1, unsigned int param_2)
{
    if (param_1 >= self->field_0x158 ||
        param_2 >= self->field_0x24) {
        return;
    }
    char *tf = (self->field_0x15c)[param_1];
    char *mesh = (self->field_0x28)[param_2];
    AbyssEngine::ArrayAddMesh(*(AbyssEngine::Mesh **)(mesh), tf + 0x3c);
    paintcanvas_ext_tami_bsphere_merge(tf + 0xd4, mesh + 0x3c);

    void *res = *(void **)(mesh + 0x34);
    if (res != 0) {
        long long resLen = *(long long *)((char *)res + 0xf8);
        long long tfLen = *(long long *)(tf + 0xf8);
        if (tfLen < resLen) {
            paintcanvas_ext_tami_setanimlen(tf, (int)(resLen >> 32), (int)resLen);
            tf = (self->field_0x15c)[param_1];
        }
        long long tfStart = *(long long *)(tf + 0x100);
        void *res2 = *(void **)(mesh + 0x34);
        long long resStart = *(long long *)((char *)res2 + 0x100);
        if (tfStart == 0 || resStart < tfStart) {
            *(long long *)(tf + 0x100) = resStart;
        }
        paintcanvas_ext_tami_setanimstate(tf, 2, 0);
    }
    paintcanvas_ext_tami_finalize((self->field_0x15c)[param_1]);
}

// ---- GetReverseString_780a4.cpp ----
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
    grs2_String_ctor_copy(&tmp, in, false);
    grs2_GetReverseString_impl(out, 0, &tmp, *(char *)((char *)(unsigned long)param2 + 0x1c) == 0);
    grs2_String_dtor(&tmp);

    if (*canary != saved)
        grs2___stack_chk_fail();
}

// ---- GetAccelValue_7e7c2.cpp ----
void GetAccelValue(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_accel(self->field_0x34);
}

// ---- ResetPersMatrix_7d0b4.cpp ----
__attribute__((visibility("hidden"))) extern float *const g_rpm_fov_8d0dc;
__attribute__((visibility("hidden"))) extern const unsigned int g_rpm_const_8d234;
__attribute__((visibility("hidden"))) extern float *const g_rpm_near_8d196;
__attribute__((visibility("hidden"))) extern float *const g_rpm_far_8d1a4;
__attribute__((visibility("hidden"))) extern char *const g_rpm_flag_8d1b8;


void ResetPersMatrix(AbyssEngine::PaintCanvas *self)
{
    char *t = (char *)self;
    int w = paintcanvas_ext_rpm_dispwidth(*(void **)(t + 0x34));
    int h = paintcanvas_ext_rpm_dispheight(*(void **)(t + 0x34));
    float fov = *g_rpm_fov_8d0dc;
    float s = paintcanvas_ext_rpm_sinf(fov * 0.5f);
    float c = paintcanvas_ext_rpm_cosf(fov * 0.5f);
    float fw = paintcanvas_ext_rpm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_rpm_signedtofloat(h, 0);

    memset(t + 0x68, 0, 0x10);
    memset(t + 0x38, 0, 0x10);
    memset(t + 0x58, 0, 0x10);
    memset(t + 0x48, 0, 0x10);

    if (*(unsigned int *)(t + 0x30) <= 3) {
        float aspect = fw / fh;
        float f = 1.0f / (s / c);
        switch (*(unsigned int *)(t + 0x30)) {
        case 0:
            *(float *)(t + 0x48) = f / aspect;
            *(float *)(t + 0x3c) = -f;
            break;
        case 1:
            *(float *)(t + 0x48) = -(f / aspect);
            *(float *)(t + 0x3c) = -f;
            break;
        case 2:
            *(float *)(t + 0x4c) = -f;
            *(float *)(t + 0x38) = f / aspect;
            break;
        case 3:
            *(float *)(t + 0x4c) = -f;
            *(float *)(t + 0x38) = -(f / aspect);
            break;
        }
    }

    float n = *g_rpm_near_8d196;
    float fr = *g_rpm_far_8d1a4;
    *(unsigned int *)(t + 0x64) = g_rpm_const_8d234;
    *(float *)(t + 0x60) = (n + fr) / (fr - n);
    *(float *)(t + 0x70) = ((n + n) * fr) / (fr - n);

    if (*g_rpm_flag_8d1b8 == 0) {
        paintcanvas_ext_rpm_glMatrixMode(0x1702);
        paintcanvas_ext_rpm_glLoadIdentity();
        float sc;
        *(unsigned int *)&sc = g_rpm_const_8d234;
        paintcanvas_ext_rpm_glScalef(1.0f, sc, 1.0f);
        paintcanvas_ext_rpm_glMatrixMode(0x1701);
        paintcanvas_ext_rpm_glLoadMatrixf(t + 0x38);
        paintcanvas_ext_rpm_glMatrixMode(0x1700);
        paintcanvas_ext_rpm_glFinish();
    } else {
        paintcanvas_ext_rpm_loadproj(*(void **)(t + 0x34), t + 0x38);
    }
}

// ---- CameraIsPointinViewFrustum_7ba98.cpp ----
__attribute__((visibility("hidden"))) extern const double g_cipvf_gravscale_7bba8;


int CameraIsPointinViewFrustum(AbyssEngine::PaintCanvas *self, void *param_1)
{
    char *t = (char *)self;
    if (*(unsigned int *)(t + 0x170) >= *(unsigned int *)(t + 0x164)) {
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

    void *grav = paintcanvas_ext_cipvf_getgrav(*(void **)(t + 0x34));
    double angle = *(double *)((char *)grav + 8) * g_cipvf_gravscale_7bba8;
    float a = (float)angle;
    int orient = *(int *)(t + 0x30);
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cipvf_sinf(rot);
    float c = paintcanvas_ext_cipvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    return paintcanvas_ext_cipvf_inner(param_1, m, cam);
}

// ---- SetTexture_78212.cpp ----
void SetTexture(AbyssEngine::PaintCanvas *self, unsigned int, unsigned int)
{
    return paintcanvas_ext_set_texture(self->field_0x34);
}

// ---- _PaintCanvas_77a70.cpp ----
AbyssEngine::PaintCanvas *PaintCanvasDtor(AbyssEngine::PaintCanvas *self)
{
    char *t = (char *)self;
    paintcanvas_ext_dtor_releaseall(self);

    // Walk the resource list (count at 0x134, array at 0x138), releasing each.
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x134); i++) {
        void *res = (*(void ***)(t + 0x138))[i];
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
            void *cell = (*(void ***)(t + 0x138))[i];
            if (cell != 0) {
                paintcanvas_ext_dtor_op_delete(cell);
            }
            (*(void ***)(t + 0x138))[i] = 0;

            // glow scratch arrays released per-iteration in the original
            paintcanvas_ext_dtor_arr_release_mesh(t + 0x18c);
            paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x198);
            paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x1a4);
            paintcanvas_ext_dtor_arr_release_uint(t + 0x1b0);
            paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x1bc);
        }
    }

    paintcanvas_ext_dtor_meshrelease(*(void **)(t + 0x34), t + 8);
    paintcanvas_ext_dtor_meshrelease(*(void **)(t + 0x34), t + 0x1c8);

    // Release the loaded-texture name strings (count at 0x10, array at 0x14).
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x10); i++) {
        void *tex = (*(void ***)(t + 0x14))[i];
        if (tex != 0) {
            paintcanvas_ext_dtor_str_dtor((char *)tex + 4);
            paintcanvas_ext_dtor_op_delete(tex);
        }
        (*(void ***)(t + 0x14))[i] = 0;
    }

    paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x1bc);
    paintcanvas_ext_dtor_arr_dtor_uint(t + 0x1b0);
    paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x1a4);
    paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x198);
    paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x18c);
    paintcanvas_ext_dtor_arr_dtor_ss(t + 0x180);
    paintcanvas_ext_dtor_arr_dtor_mat(t + 0x174);
    paintcanvas_ext_dtor_arr_dtor_cam(t + 0x164);
    paintcanvas_ext_dtor_arr_dtor_tf(t + 0x158);
    paintcanvas_ext_dtor_arr_dtor_img(t + 0x14c);
    paintcanvas_ext_dtor_arr_dtor_font(t + 0x140);
    paintcanvas_ext_dtor_arr_dtor_res(t + 0x134);
    paintcanvas_ext_dtor_arr_dtor_mesh(t + 0x24);
    paintcanvas_ext_dtor_arr_dtor_tex(t + 0x10);
    return self;
}

// ---- Image2DCreate_79c78.cpp ----
namespace AbyssEngine {
void ArrayAddImage2D(Image2D *item, void *array);
}

void Image2DCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_i2d_findres(self, param_1);
    if (res == 0) {
        return;
    }
    unsigned short *info = *(unsigned short **)(res + 0xc);
    char *texres = (char *)paintcanvas_ext_i2d_findres(self, *info);
    if (texres == 0) {
        return;
    }
    if (*(int *)(texres + 8) == -1) {
        paintcanvas_ext_i2d_texcreate(self, *info, true);
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *img = (char *)operator new(0x18);
        memset(img, 0, 0x18);
        char *texpath = (char *)**(char ***)(texres + 0xc);
        int ok = paintcanvas_ext_i2d_imgregion(self->field_0x34, texpath,
                                               info[1], img);
        if (ok != 1) {
            return;
        }
        if (*(int *)(texres + 8) != -1) {
            *(int *)(img + 4) = *(int *)(texres + 8);
        }
        AbyssEngine::ArrayAddImage2D((AbyssEngine::Image2D *)img, (char *)self + 0x14c);
        idx = self->field_0x14c - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    *param_2 = idx;
}

// ---- GetScreenPosition_7bd58.cpp ----
__attribute__((visibility("hidden"))) extern const double g_gsp2_gravscale_8bfa8;


void GetScreenPosition(AbyssEngine::PaintCanvas *self, void *param_1, void *param_2, char *param_3)
{
    char *t = (char *)self;

    // first transform the input world position
    char transformed[16];
    paintcanvas_ext_gsp2_transformvec(transformed, param_1);

    if (*(unsigned int *)(t + 0x170) >= *(unsigned int *)(t + 0x164)) {
        return;
    }

    char invMat[60];
    void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    if (*t == 0) {
        paintcanvas_ext_gsp2_invtransformvec(invMat, (char *)cam + 0xc);
        paintcanvas_ext_gsp2_vec_assign(param_3, invMat);
    } else {
        float m[16];
        char scratch[60];
        memset(m, 0, sizeof(m));
        m[0] = 1.0f; m[5] = 1.0f; m[14] = 1.0f;
        paintcanvas_ext_gsp2_matidentity(scratch, m);

        void *grav = paintcanvas_ext_gsp2_getgrav(*(void **)(t + 0x34));
        double angle = *(double *)((char *)grav + 8) * g_gsp2_gravscale_8bfa8;
        float a = (float)angle;
        int orient = *(int *)(t + 0x30);
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
    char *cam2 = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
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
    int w0 = paintcanvas_ext_gsp2_getwidth(self);
    int w1 = paintcanvas_ext_gsp2_getwidth(self);
    float py = *(float *)(param_3 + 4);
    double fw = (double)paintcanvas_ext_gsp2_signedtofloat(w0, 0);
    double halfW = (double)paintcanvas_ext_gsp2_signedtofloat(w1 >> 1, 0);
    *(float *)param_3 = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp2_getheight(self);
    double fh = (double)paintcanvas_ext_gsp2_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp2_getheight(self);
    double halfH = (double)paintcanvas_ext_gsp2_signedtofloat(h1 >> 1, 0);
    *(float *)(param_3 + 4) = (float)(halfH + (((double)py * 0.5) / (double)denomY) * fh);

    // remaining bound checks compute booleans only (no observable output)
    float nx = *(float *)param_3;
    if (nx >= 0.0f) {
        float ny = *(float *)(param_3 + 4);
        if (ny >= 0.0f) {
            int ww = paintcanvas_ext_gsp2_getwidth(self);
            float fww = paintcanvas_ext_gsp2_signedtofloat(ww, 0);
            if (nx < fww) {
                int hh = paintcanvas_ext_gsp2_getheight(self);
                paintcanvas_ext_gsp2_signedtofloat(hh, 0);
            }
        }
    }
}

// ---- ResourceLoaded_7c7a4.cpp ----
int ResourceLoaded(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int type)
{
    unsigned int count;
    switch (type) {
    case 1: {
        char *res = (self->field_0x138)[index];
        if (*(int *)(res + 0x4) == 2) {
            int handle = *(int *)(res + 0x8);
            return handle + 1 != 0 ? 1 : 0;
        }
        return 0;
    }
    case 2:
        count = self->field_0x140;
        break;
    case 3:
        count = self->field_0x14c;
        break;
    case 4:
        count = self->field_0x24;
        break;
    case 5:
        count = self->field_0x158;
        break;
    case 6:
        count = self->field_0x174;
        break;
    default:
        return 0;
    }
    return index < count ? 1 : 0;
}

// ---- TransformGetTriCount_7b690.cpp ----
int TransformGetTriCount(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x158) {
        void *t = (self->field_0x15c)[index];
        return paintcanvas_ext_transform_tricount(self, t);
    }
    return 0;
}

// ---- SpriteSystemSetPosition_7a9d6.cpp ----
float SpriteSystemSetPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                              float x, float y, float z)
{
    if (index < self->field_0x180) {
        char *s = (self->field_0x184)[index];
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

// ---- DrawImage2D_78b68.cpp ----
void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int index, int x, int y)
{
    char abuf[60];
    if (index < self->field_0x14c) {
        char *img = (self->field_0x150)[index];
        if (*(unsigned char *)(img + 0x14)) {
            paintcanvas_ext_di_restore(*(unsigned char *)(img + 0x14), img);
            img = (self->field_0x150)[index];
        }
        paintcanvas_ext_di_settexture(self, *(unsigned int *)(img + 0x4), -1);

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

        paintcanvas_ext_di_setwvm(self, abuf);
        paintcanvas_ext_di_gldisable(0xb44);
        paintcanvas_ext_di_meshdraw(self->field_0x34,
                                    *(void **)((self->field_0x150)[index]));
        paintcanvas_ext_di_glenable(0xb44);
    }
}

// ---- PaintCanvas_77674.cpp ----
// Array/Matrix constructors (distinct callees grouped by template instantiation).
extern "C" void paintcanvas_ctor_arr_tex(void *);    // 0x6f88c  f10
extern "C" void paintcanvas_ctor_arr_mesh(void *);   // 0x6f724  f24, f18c
extern "C" void paintcanvas_ctor_matrix(void *);     // 0x6f118  f8
extern "C" void paintcanvas_ctor_arr_res(void *);    // 0x6f898  f134
extern "C" void paintcanvas_ctor_arr_font(void *);   // 0x6f8a4  f140
extern "C" void paintcanvas_ctor_arr_img(void *);    // 0x6f8b0  f14c
extern "C" void paintcanvas_ctor_arr_tf(void *);     // 0x6f730  f158
extern "C" void paintcanvas_ctor_arr_cam(void *);    // 0x6f8bc  f164
extern "C" void paintcanvas_ctor_arr_mat(void *);    // 0x6f8c8  f174
extern "C" void paintcanvas_ctor_arr_ss(void *);     // 0x6f8d4  f180
extern "C" void paintcanvas_ctor_arr_mtx(void *);    // 0x6f8e0  f198, f1a4, f1bc
extern "C" void paintcanvas_ctor_arr_uint(void *);   // 0x6f8ec  f1b0
extern "C" void paintcanvas_ext_meshcreate5(void *, unsigned short, unsigned short,
                                            signed char, void *);

AbyssEngine::PaintCanvas *PaintCanvasCtor(AbyssEngine::PaintCanvas *self, AbyssEngine::Engine *engine)
{
    char *t = (char *)self;
    paintcanvas_ctor_arr_tex(t + 0x10);
    paintcanvas_ctor_arr_mesh(t + 0x24);
    paintcanvas_ctor_matrix(t + 0xf8);
    paintcanvas_ctor_arr_res(t + 0x134);
    paintcanvas_ctor_arr_font(t + 0x140);
    paintcanvas_ctor_arr_img(t + 0x14c);
    paintcanvas_ctor_arr_tf(t + 0x158);
    paintcanvas_ctor_arr_cam(t + 0x164);
    paintcanvas_ctor_arr_mat(t + 0x174);
    paintcanvas_ctor_arr_ss(t + 0x180);
    paintcanvas_ctor_arr_mesh(t + 0x18c);
    paintcanvas_ctor_arr_mtx(t + 0x198);
    paintcanvas_ctor_arr_mtx(t + 0x1a4);
    paintcanvas_ctor_arr_uint(t + 0x1b0);
    paintcanvas_ctor_arr_mtx(t + 0x1bc);

    *(unsigned char *)(t + 0x1f1) = 0;
    *(unsigned int *)(t + 0x20) = 0;
    *(unsigned int *)(t + 0x1f4) = 1;
    *(unsigned int *)(t + 0x4) = 0;
    *(unsigned char *)(t + 0x0) = 0;
    *(AbyssEngine::Engine **)(t + 0x34) = engine;
    *(unsigned int *)(t + 0x170) = 0xffffffff;

    paintcanvas_ext_meshcreate5(engine, 4, 2, 0x11, t + 0x1c8);

    int *p = *(int **)(*(char **)(t + 0x1c8) + 0x2c);
    p[0] = 0x20000;
    p[1] = 1;
    p[2] = *(int *)0x87878;
    *(float *)(t + 0x1fc) = 1.0f;
    *(float *)(t + 0x200) = 1.0f;
    *(float *)(t + 0x204) = 1.0f;
    *(float *)(t + 0x208) = 1.0f;
    *(unsigned char *)(t + 0x1c) = 1;
    engine->field_0xfc = 1;

    paintcanvas_ext_meshcreate5(engine, 400, 200, 0x1b, t + 0x8);

    short *buf = *(short **)(*(char **)(t + 0x8) + 0x2c);
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
    *(unsigned char *)(t + 0x1f8) = 1;
    *(unsigned int *)(t + 0x1cc) = 0;
    return self;
}

// ---- TransformRemoveMesh_7b178.cpp ----
void TransformRemoveMesh(AbyssEngine::PaintCanvas *self, unsigned int transformIndex, void *mesh)
{
    if (self->field_0x158 <= transformIndex) {
        return;
    }
    char *x = paintcanvas_ext_find_mesh(self, mesh);
    if (x) {
        return paintcanvas_ext_remove_mesh(self, transformIndex, *(int *)(x + 0x8));
    }
}

// ---- ClearBuffer_7ca84.cpp ----
void ClearBuffer(AbyssEngine::PaintCanvas *self, unsigned int mask)
{
    paintcanvas_ext_enable(0xb71);
    paintcanvas_ext_depthmask(1);
    return paintcanvas_ext_clear2(self->field_0x34, mask);
}

// ---- TransformRemoveChild_7b34c.cpp ----
void TransformRemoveChild(AbyssEngine::PaintCanvas *self, unsigned int parent, unsigned int child)
{
    unsigned int count = self->field_0x158;
    if (parent != child && child < count && parent < count) {
        char **arr = self->field_0x15c;
        char *p = arr[parent];
        char *c = arr[child];
        paintcanvas_ext_array_remove(c, p + 0x4c);
        char **arr2 = self->field_0x15c;
        return paintcanvas_ext_transform_dirty(arr2[parent]);
    }
}

// ---- TextureCreate_79a34.cpp ----
extern "C" int paintcanvas_ext_tc_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);
extern "C" int paintcanvas_ext_tc_texfromfileintern(void *eng, char *path, void *cb, void *ud,
                                                    unsigned int *out, float f, void *lt, bool b);

void TextureCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, void *param_2,
                   void *param_3, unsigned int *param_4, bool param_5)
{
    void *eng = self->field_0x34;
    *(unsigned int *)((char *)eng + 0x7c) = 0xffffffff;
    *(unsigned int *)((char *)eng + 0x80) = 0xffffffff;

    char *res = (char *)paintcanvas_ext_tc_findres(self, param_1);
    if (res != 0) {
        unsigned int idx = *(unsigned int *)(res + 8);
        if (idx == 0xffffffff) {
            void **info = *(void ***)(res + 0xc);
            float f = (float)(int)(*(unsigned int *)((char *)info + 4));
            char *path = (char *)*info;
            int ok;
            if (!param_5) {
                ok = paintcanvas_ext_tc_texfromfileintern(self->field_0x34, path,
                                                          param_2, param_3, &idx, f, 0, false);
            } else {
                ok = paintcanvas_ext_tc_texfromfile(self->field_0x34, path,
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

// ---- StartDraw2FBO_7d088.cpp ----
void StartDraw2FBO(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_start_fbo(self->field_0x34);
}

// ---- Suspend_7e7c8.cpp ----
void Suspend(AbyssEngine::PaintCanvas *self)
{
    char texId[4];
    for (unsigned int i = 0; i < self->field_0x10; i++) {
        int *p = (self->field_0x14)[i];
        *(int *)texId = *p;
        if (*p != -1) {
            paintcanvas_ext_gl_deletetextures(1, texId);
            p = (self->field_0x14)[i];
        }
        *p = -1;
    }
}

// ---- DrawTransform_798e4.cpp ----
extern "C" void paintcanvas_ext_dt_drawmesh(void *self, void *mesh, void *m, void *m2,
                                            unsigned int flag, void *m3);

void DrawTransform(AbyssEngine::PaintCanvas *self, char *tf, void *m2, void *m3)
{
    char buf[60];
    if (tf && *(unsigned char *)(tf + 0xec)) {
        paintcanvas_ext_mtx_mul(buf, m2, tf);
        if (*(int *)(tf + 0x11c) != 0) {
            paintcanvas_ext_mtx_muleq(buf, tf + 0x5c);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(tf + 0x3c); i++) {
            void *mesh = (*(void ***)(tf + 0x40))[i];
            paintcanvas_ext_dt_drawmesh(self, mesh, buf, m3, *(unsigned int *)(tf + 0x48), tf + 0x98);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(tf + 0x4c); i++) {
            if (self->field_0x170 < self->field_0x164 &&
                paintcanvas_ext_dt_incamvf((*(void ***)(tf + 0x50))[i], buf,
                    (self->field_0x168)[self->field_0x170])) {
                paintcanvas_ext_dt_drawtransform_rec(self, (*(void ***)(tf + 0x50))[i], buf, m3);
            } else {
                self->field_0x4 += 1;
            }
        }
    }
}

// ---- DrawImage2D_78fc0.cpp ----
void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                 int param_4, int param_5, unsigned char param_6, unsigned char param_7,
                 unsigned char param_8)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di4_restore(*(unsigned char *)(img + 0x14), img);
        img = (*(char ***)(t + 0x150))[param_1];
    }
    paintcanvas_ext_di4_settexture(self, *(unsigned int *)(img + 4));

    // horizontal base placement (param_6 low bits)
    int baseX = param_2;
    int hOff;
    if ((param_6 & 7) == 4) {
        hOff = paintcanvas_ext_di4_getwidth(self) >> 1;
    } else if ((param_6 & 7) == 2) {
        baseX = -param_2;
        hOff = paintcanvas_ext_di4_getwidth(self);
    } else {
        hOff = 0;
    }

    // horizontal flip-span (param_6 bit 3)
    int spanW = param_4;
    if (param_6 & 8) {
        spanW = paintcanvas_ext_di4_getwidth(self) - (param_4 + param_2);
    }
    float fSpanW = paintcanvas_ext_di4_signedtofloat(spanW, 0);
    char *region = *(char **)(*(char **)((*(char ***)(t + 0x150))[param_1]) + 4);
    float regW = *(float *)(region + 0xc);

    // vertical flip-span (param_6 bit 7)
    int spanH = param_5;
    if (param_6 & 0x80) {
        spanH = paintcanvas_ext_di4_getheight(self) - (param_5 + param_3);
        region = *(char **)(*(char **)((*(char ***)(t + 0x150))[param_1]) + 4);
    }
    float fSpanH = paintcanvas_ext_di4_signedtofloat(spanH, 0);
    float regH = *(float *)(region + 0x1c);

    // vertical base placement (param_6 high nibble)
    int baseY = param_3;
    int vOff;
    if ((param_6 & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di4_getheight(self) >> 1;
    } else if ((param_6 & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di4_getheight(self);
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
            anchorX = ((param_4 + param_2) - paintcanvas_ext_di4_getwidth(self)) >> 1;
        }
    } else if ((param_7 & 7) == 2) {
        if ((param_6 & 8) == 0) {
            anchorX = -param_4;
        } else {
            anchorX = (param_4 + param_2) - paintcanvas_ext_di4_getwidth(self);
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
            anchorY = ((param_5 + param_3) - paintcanvas_ext_di4_getheight(self)) >> 1;
        }
    } else if ((param_7 & 0x70) == 0x20) {
        if ((param_6 & 0x80) == 0) {
            anchorY = -param_5;
        } else {
            anchorY = (param_5 + param_3) - paintcanvas_ext_di4_getheight(self);
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

    paintcanvas_ext_di4_setwvm(self, m);
    paintcanvas_ext_di4_gldisable(0xb44);
    paintcanvas_ext_di4_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_di4_glenable(0xb44);
}

// ---- CameraSetPerspective_7ba1e.cpp ----
void CameraSetPerspective(AbyssEngine::PaintCanvas *self, unsigned int index, float fov, float aspect)
{
    if (index < self->field_0x164) {
        float w = (float)paintcanvas_ext_get_w(self);
        float h = (float)paintcanvas_ext_get_h(self);
        void *cam = (self->field_0x168)[index];
        paintcanvas_ext_cam_persp(fov, aspect, w, h, cam);
        if (self->field_0x170 == index) {
            return paintcanvas_ext_cam_setcur(self, index);
        }
    }
}

// ---- DrawRegion2D_78818.cpp ----
void DrawRegion2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, float param_2, int param_3,
                  int param_4, int param_5, int param_6, float param_7, float param_8)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_dr2_restore(*(unsigned char *)(img + 0x14), img);
        img = (*(char ***)(t + 0x150))[param_1];
    }
    paintcanvas_ext_dr2_settexture(self, *(unsigned int *)(img + 4));

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

    paintcanvas_ext_dr2_setwvm(self, local138);
    paintcanvas_ext_dr2_gldisable(0xb44);
    paintcanvas_ext_dr2_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_dr2_glenable(0xb44);
}

// ---- MeshSetTriangle_7a1c6.cpp ----
void MeshSetTriangle(AbyssEngine::PaintCanvas *self, unsigned int meshIndex, unsigned short tri,
                     unsigned short v0, unsigned short v1, unsigned short v2)
{
    if (meshIndex < self->field_0x24) {
        char *mesh = (self->field_0x28)[meshIndex];
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

// ---- SetGameOrientation_7e3fc.cpp ----
__attribute__((visibility("hidden"))) extern const unsigned int g_sgo_const_8e6b4;
__attribute__((visibility("hidden"))) extern const unsigned int g_sgo_const_8e6b8;


static void zero16(char *p)
{
    memset(p, 0, 0x10);
}

void SetGameOrientation(AbyssEngine::PaintCanvas *self, int param_2)
{
    char *t = (char *)self;
    if (*(int *)(t + 0x30) == param_2) {
        return;
    }
    *(int *)(t + 0x30) = param_2;
    paintcanvas_ext_sgo_setorientation(*(void **)(t + 0x34), param_2);

    *(float *)(t + 0x3c) = -*(float *)(t + 0x3c);
    *(float *)(t + 0x48) = -*(float *)(t + 0x48);

    int w = paintcanvas_ext_sgo_dispwidth(*(void **)(t + 0x34));
    float fw = paintcanvas_ext_sgo_signedtofloat(w, 0);
    int h = paintcanvas_ext_sgo_dispheight(*(void **)(t + 0x34));
    float fh = paintcanvas_ext_sgo_signedtofloat(h, 0);

    if (param_2 == 3) {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xe4); zero16(t + 0xb8);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xec) = 0;
        *(unsigned int *)(t + 0xf0) = 0;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xb8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xbc) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fw;
        *(float *)(t + 0x8c) = -(2.0f / fh);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        int w2 = paintcanvas_ext_sgo_dispwidth(*(void **)(t + 0x34));
        *(float *)(t + 0xe8) = paintcanvas_ext_sgo_signedtofloat(w2, 0);
        int h2 = paintcanvas_ext_sgo_dispheight(*(void **)(t + 0x34));
        *(float *)(t + 0xec) = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (param_2 == 1) {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xe4); zero16(t + 0xbc);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xbc) = 0x3f800000;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = g_sgo_const_8e6b8;
        *(float *)(t + 0x78) = 2.0f / fh;
        *(float *)(t + 0x8c) = -(2.0f / fw);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        int h2 = paintcanvas_ext_sgo_dispheight(*(void **)(t + 0x34));
        *(float *)(t + 0xe8) = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (param_2 != 0) {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xbc);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xec) = 0;
        *(unsigned int *)(t + 0xf0) = 0;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xb8) = 0x3f800000;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xcc) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fw;
        *(float *)(t + 0x8c) = -(2.0f / fh);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
    } else {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xe4); zero16(t + 0xbc);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xbc) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fh;
        *(float *)(t + 0x8c) = -(2.0f / fw);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        int w2 = paintcanvas_ext_sgo_dispwidth(*(void **)(t + 0x34));
        *(float *)(t + 0xec) = paintcanvas_ext_sgo_signedtofloat(w2, 0);
    }

    if (*(int *)(t + 0x170) == -1) {
        return;
    }
    float *cam = (*(float ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    paintcanvas_ext_sgo_setpersp(self, cam[0], cam[1], cam[2]);
}

// ---- MeshCreate_79f2c.cpp ----
extern "C" int paintcanvas_ext_mc_meshcreate(void *eng, unsigned short a, unsigned short b,
                                             signed char c, void **out);

namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
}

void MeshCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned short param_2,
                signed char param_3, unsigned short param_4, unsigned int *param_6)
{
    int result = -1;
    unsigned int mat = 0xffffffff;
    void *mesh = 0;
    paintcanvas_ext_mc_matcreate(self, param_4, &mat);
    int ok = paintcanvas_ext_mc_meshcreate(self->field_0x34, param_1, param_2,
                                           param_3, &mesh);
    if (ok == 1) {
        if (0xfffffffe < self->field_0x174) {
            // material pointer back-reference: write last element ptr into mesh+0x30
            void *m = *(void **)(self->field_0x178 - 4);
            if (mesh) {
                mesh->field_0x30 = m;
            }
        }
        AbyssEngine::ArrayAddMesh((AbyssEngine::Mesh *)mesh, (char *)self + 0x24);
        result = self->field_0x24 - 1;
    }
    *param_6 = (unsigned int)result;
}

// ---- DrawMesh_7e978.cpp ----
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

void DrawMesh(AbyssEngine::PaintCanvas *self, char *param_1, const float *param_2,
              const float *param_3, unsigned int param_4, const float *param_5)
{
    char *t = (char *)self;

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
        int vis = paintcanvas_ext_dm_spherefrustum(self, uvM, r2 * *(float *)(param_1 + 0x4c));
        if (vis == 0) {
            *(int *)(t + 4) += 1;
            return;
        }
        if (*(short *)(param_1 + 2) != 0) {
            if (*(int *)(param_1 + 0x30) == 0) {
                float fr = paintcanvas_ext_dm_unsignedtofloat(color >> 24, 0);
                float fg = paintcanvas_ext_dm_unsignedtofloat((color >> 16) & 0xff, 0);
                paintcanvas_ext_dm_unsignedtofloat((color >> 8) & 0xff, 0);
                paintcanvas_ext_dm_unsignedtofloat(color & 0xff, 0);
                paintcanvas_ext_dm_setcolor(
                    *(void **)(t + 0x34),
                    (*(float *)(t + 0x1fc) * fr) / g_dm_255_8ee80, 0.0f,
                    (*(float *)(t + 0x200) * fg) / g_dm_255_8ee80, 0.0f);

                paintcanvas_ext_dm_mtx_muleq(worldM, param_3);
                paintcanvas_ext_dm_setwvm(self, worldM);
                paintcanvas_ext_dm_setmodelmatrix(*(void **)(t + 0x34));
                paintcanvas_ext_dm_setuvmatrix(*(void **)(t + 0x34), uvM);
                paintcanvas_ext_dm_meshdraw(*(void **)(t + 0x34), param_1);
                paintcanvas_ext_dm_resetuvmatrix(*(void **)(t + 0x34));
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
            DrawMesh(self, child, worldM, param_3, param_4, uvM);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(res + 0x4c); i++) {
            if (*(unsigned int *)(t + 0x170) < *(unsigned int *)(t + 0x164)) {
                void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
                char *tf = (*(char ***)(res + 0x50))[i];
                if (paintcanvas_ext_dm_incamvf(tf, (void *)param_2, cam)) {
                    paintcanvas_ext_dm_drawtransform(self, (*(void ***)(res + 0x50))[i],
                                                     worldM, (void *)param_3);
                }
            }
        }
    }
}

// ---- Begin2d_7e2a0.cpp ----
#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_b2d_flag HIDDEN;

void Begin2d(AbyssEngine::PaintCanvas *self)
{
    *(unsigned char *)(self->field_0x34 + 0xfd) = 1;
    paintcanvas_ext_gl_disable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_blendfunc(0x302, 0x303);
    paintcanvas_ext_setcolor(self->field_0x34, 1.0f, 1.0f, 1.0f, 1.0f);
    paintcanvas_ext_glenable2(self->field_0x34, 0xde1, true);
    if (*paintcanvas_g_b2d_flag == 0) {
        paintcanvas_ext_gl_texenvi(0x2300, 0x2200, 0x2100);
        paintcanvas_ext_glMatrixMode(0x1702);
        paintcanvas_ext_gl_loadidentity();
        paintcanvas_ext_gl_scalef(1.0f, 1.0f, 1.0f);
        paintcanvas_ext_glMatrixMode(0x1701);
        paintcanvas_ext_gl_loadmatrix((char *)self + 0x78);
        if (self->field_0x30 != 2) {
            paintcanvas_ext_gl_multmatrix((char *)self + 0xb8);
        }
        paintcanvas_ext_glMatrixMode(0x1700);
        paintcanvas_ext_gl_loadidentity();
    } else {
        paintcanvas_ext_setortho(self->field_0x34, (char *)self + 0x78,
                                 (char *)self + 0xb8, self->field_0x30 != 2);
    }
    self->field_0xc = 0;
}

// ---- DrawString_781c0.cpp ----
extern "C" void paintcanvas_ext_drawstring_raw(void *, const unsigned short *, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void DrawString(AbyssEngine::PaintCanvas *self, unsigned int index, const unsigned short *str,
                int x, int y, bool b)
{
    if (index < self->field_0x140) {
        char *font = (self->field_0x144)[index];
        paintcanvas_ext_string_prep(self, *(void **)(font + 0x8), -1);
        char *font2 = (self->field_0x144)[index];
        paintcanvas_ext_drawstring_raw(font2, str, x, y, self,
                                       self->field_0x34, b);
    }
}

// ---- DrawStringColor_78274.cpp ----
__attribute__((visibility("hidden"))) extern const char g_dsc_pipe_882c4[];
__attribute__((visibility("hidden"))) extern const char g_dsc_fmt_882ee[];

extern "C" void paintcanvas_ext_dsc_fontdraw(void *font, unsigned short *txt, unsigned int len,
                                             int x, int y, void *self, void *eng, bool b);

void DrawStringColor(AbyssEngine::PaintCanvas *self, unsigned int param_1, void *param_2,
                     int param_3, int param_4, bool param_5)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x140)) {
        return;
    }
    void *font0 = (*(char ***)(t + 0x144))[param_1];
    paintcanvas_ext_dsc_settexture(self, *(unsigned int *)((char *)font0 + 8));
    paintcanvas_ext_dsc_getcolor(self);

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
                void *font = (*(char ***)(t + 0x144))[param_1];
                unsigned short *txt = paintcanvas_ext_dsc_str_cast(part);
                paintcanvas_ext_dsc_fontdraw(font, txt, *(unsigned int *)(part + 8), param_3,
                                             param_4, self, *(void **)(t + 0x34), param_5);
                param_3 += paintcanvas_ext_dsc_textwidth(self, param_1, part);
            } else if (*(int *)(part + 8) == 0) {
                paintcanvas_ext_dsc_setcolor(self);
            } else {
                int color = 0;
                char *s = paintcanvas_ext_dsc_getAEChar(part);
                paintcanvas_ext_dsc_sscanf(s, g_dsc_fmt_882ee, &color);
                paintcanvas_ext_dsc_setcolor(self);
            }
            draw = !draw;
        }
        paintcanvas_ext_dsc_setcolor(self);
        paintcanvas_ext_dsc_releaseclasses(parts);
        void *p = paintcanvas_ext_dsc_arr_dtor(parts);
        paintcanvas_ext_dsc_op_delete(p);
    }
    paintcanvas_ext_dsc_str_dtor(str);
}

// ---- SetMatForGlow_7d24c.cpp ----
namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
void ArrayAddUint(unsigned int item, void *array);
}

// Adds a matrix (15 floats) plus the target Array pointer onto a glow-matrix list.

void SetMatForGlow(AbyssEngine::PaintCanvas *self, char *param_1)
{
    char *t = (char *)self;
    int off = 0x38;
    for (unsigned int i = 0; i < *(unsigned int *)(param_1 + 0x44); i++) {
        // meshes array at 0x48 -> list at this+0x18c
        AbyssEngine::ArrayAddMesh((*(AbyssEngine::Mesh ***)(param_1 + 0x48))[i], t + 0x18c);

        const float *m1 = (const float *)(*(char **)(param_1 + 0x30) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m1, t + 0x198);

        const float *m2 = (const float *)(*(char **)(param_1 + 0x3c) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m2, t + 0x1a4);

        AbyssEngine::ArrayAddUint((*(unsigned int **)(param_1 + 0x54))[i], t + 0x1b0);

        const float *m3 = (const float *)(*(char **)(param_1 + 0x60) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m3, t + 0x1bc);

        off += 0x3c;
    }
}

// ---- EnableClip_7cab0.cpp ----
void EnableClip(AbyssEngine::PaintCanvas *self, int param_1, int param_2, int param_3, int param_4)
{
    paintcanvas_ext_ec_glEnable(0xc11);
    int sx, sy, sw, sh;
    switch (self->field_0x30) {
    case 0:
        sx = param_1;
        sy = param_2;
        sw = param_3;
        sh = param_4;
        break;
    case 1: {
        int h = paintcanvas_ext_ec_getHeight(self);
        int w = paintcanvas_ext_ec_getWidth(self);
        sx = h - (param_2 + param_4);
        sy = w - (param_1 + param_3);
        sw = param_4;
        sh = param_3;
        break;
    }
    case 2: {
        int h = paintcanvas_ext_ec_getHeight(self);
        sx = h - (param_2 + param_4);
        sy = param_1;
        sw = param_3;
        sh = param_4;
        break;
    }
    case 3: {
        int w = paintcanvas_ext_ec_getWidth(self);
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

// ---- DrawImage2D_78dd0.cpp ----
void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                 unsigned char param_4, unsigned char param_5)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di3_restore(*(unsigned char *)(img + 0x14), img);
    }

    // Horizontal placement offset (param_5 low bits) + vertical (param_5 high nibble).
    int hOff;
    if ((param_5 & 7) == 4) {
        hOff = paintcanvas_ext_di3_getwidth(self) >> 1;
    } else if ((param_5 & 7) == 2) {
        hOff = paintcanvas_ext_di3_getwidth(self);
        param_3 = -param_3;
    } else {
        hOff = 0;
    }

    int vOff;
    unsigned int yShift = param_4;
    if ((param_5 & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di3_getheight(self) >> 1;
    } else if ((param_5 & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di3_getheight(self);
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
    char *region2 = *(char **)((*(char ***)(t + 0x150))[param_1] + 4);
    if ((param_4 & 0x70) == 0x20) {
        ry = -(int)*(float *)(region2 + 0x1c);
    } else if ((param_4 & 0x70) == 0x40) {
        double h = (double)paintcanvas_ext_di3_signedtofloat((int)*(float *)(region2 + 0x1c), 0);
        ry = (int)(long long)(h * -0.5);
    } else {
        ry = 0;
    }

    char *img2 = (*(char ***)(t + 0x150))[param_1];
    paintcanvas_ext_di3_settexture(self, *(unsigned int *)(img2 + 4));

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

    paintcanvas_ext_di3_setwvm(self, m);
    paintcanvas_ext_di3_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
}

// ---- MeshCreate_79fb4.cpp ----
void MeshCreate(AbyssEngine::PaintCanvas *self, unsigned short a, unsigned short b,
                signed char c, unsigned int *out)
{
    char mesh[4];
    *(void **)mesh = 0;
    int result = paintcanvas_ext_meshcreate(self->field_0x34, mesh);
    if (result == 1) {
        paintcanvas_ext_array_add_mesh(*(void **)mesh, (char *)self + 0x24);
        result = (int)self->field_0x24 - 1;
    } else {
        result = -1;
    }
    *out = result;
}

// ---- FontGetSpacing_79c30.cpp ----
int FontGetSpacing(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        return paintcanvas_ext_font_get_spacing(font);
    }
    return 0;
}

// ---- TransformGetTransform_7b67c.cpp ----
// AbyssEngine::PaintCanvas::TransformGetTransform(unsigned index)
// Returns the internal Transform pointer for a transform handle. (Ghidra mis-decoded this tiny
// accessor; modeled on its sibling TransformGetLocal/TransformGetTriCount which index the
// transform array at +0x158 count / +0x15c data.)
void *TransformGetTransform(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x158) {
        return (self->field_0x15c)[index];
    }
    return 0;
}

// ---- MeshCreate_79d5c.cpp ----
__attribute__((visibility("hidden"))) extern char *const g_meshcreate_vboflag_79d5c;


namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
}

void MeshCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2,
                bool param_3)
{
    char *res = (char *)paintcanvas_ext_mc2_findres(self, param_1);
    if (res == 0) {
        return;
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *info = *(char **)(res + 0xc);
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_mc2_matcreate(self, *(unsigned short *)(info + 4), &mat);
        void *matptr = 0;
        if (0xfffffffe < self->field_0x174) {
            matptr = *(void **)(self->field_0x178 - 4);
        }
        void *mesh = 0;
        int ok = paintcanvas_ext_mc2_meshfromfile(self->field_0x34,
                                                  *(char **)info, &mesh, matptr);
        if (ok != 1) {
            return;
        }
        if (*g_meshcreate_vboflag_79d5c != 0) {
            if (mesh) {
                mesh->field_0x84 = 1;
            }
            paintcanvas_ext_mc2_converttovbo(mesh);
        }
        AbyssEngine::ArrayAddMesh((AbyssEngine::Mesh *)mesh, (char *)self + 0x24);
        idx = self->field_0x24 - 1;
        *(unsigned int *)(res + 8) = idx;
    } else {
        char *meshes = self->field_0x28;
        char *existing = ((char **)meshes)[idx];
        if (*(int *)(existing + 0x34) != 0 || param_3) {
            void *clone = paintcanvas_ext_mc2_new_mesh_copy(
                ((void **)meshes)[*(unsigned int *)(res + 8)]);
            AbyssEngine::ArrayAddMesh((AbyssEngine::Mesh *)clone, (char *)self + 0x24);
            idx = self->field_0x24 - 1;
        }
    }
    *param_2 = idx;
}

// ---- FogSetParameter_7cf28.cpp ----
__attribute__((visibility("hidden"))) extern char *const g_fsp_flag_8cf40;
__attribute__((visibility("hidden"))) extern const double g_fsp_255d_8d070;
__attribute__((visibility("hidden"))) extern const float g_fsp_255f_8d078;


void FogSetParameter(AbyssEngine::PaintCanvas *self, int param_1, float param_2, float param_3,
                     float param_4, unsigned int param_5)
{
    char *t = (char *)self;
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
        char *eng = *(char **)(t + 0x34);
        g = g / g_fsp_255d_8d070;
        *(float *)(eng + 1000) = param_2;
        *(float *)(eng + 0x3ec) = param_3;
        col[0] = (float)r;
        col[1] = (float)g;
        paintcanvas_ext_fsp_vec_assign(eng + 0x3f0, col);
    }
}

// ---- DrawRegion2D_78538.cpp ----
__attribute__((visibility("hidden"))) extern const unsigned int g_dr3_const_88808;


void DrawRegion2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                  int param_4, int param_5, float param_6, int param_7, int param_8, int param_9,
                  int param_10)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    paintcanvas_ext_dr3_settexture(self, *(unsigned int *)(img + 4));
    char *mesh = (*(char ***)(t + 0x150))[param_1];
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

    paintcanvas_ext_dr3_setwvm(self, local130);
    paintcanvas_ext_dr3_gldisable(0xb44);
    paintcanvas_ext_dr3_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_dr3_glenable(0xb44);
}

// ---- RestoreImage2D_78a60.cpp ----
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

// ---- SpriteSystemGetPosition_7a8e6.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

void SpriteSystemGetPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                             const Matrix &m, Vector &out)
{
    if (index < self->field_0x180) {
        char *s = (self->field_0x184)[index];
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

// ---- MeshSet2DMask_78b30.cpp ----
void MeshSet2DMask(AbyssEngine::PaintCanvas *self, unsigned int index, int)
{
    unsigned int i = index;
    if (self->field_0x14c <= index) {
        return;
    }
    char **arr = self->field_0x150;
    char *img = arr[i];
    if (*(unsigned char *)(img + 0x14) != 0) {
        RestoreImage2D(arr, img);
        arr = self->field_0x150;
    }
    self->field_0x20 = arr[i];
}

// ---- ReleaseAllResources_77c84.cpp ----
__attribute__((visibility("hidden"))) extern int *const g_rar_curtex_87c98;
__attribute__((visibility("hidden"))) extern int *const g_rar_texcount_87cce;
__attribute__((visibility("hidden"))) extern int *const g_rar_tricount_87d96;


void ReleaseAllResources(AbyssEngine::PaintCanvas *self)
{
    char *t = (char *)self;
    *g_rar_curtex_87c98 = 0;

    // mark all resources as unloaded
    for (int i = 0; i < *(int *)(t + 0x134); i++) {
        char *res = (*(char ***)(t + 0x138))[i];
        *(int *)(res + 8) = -1;
    }

    // textures
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x10); i++) {
        int *tex = (*(int ***)(t + 0x14))[i];
        if (*tex != -1) {
            unsigned int id = (unsigned int)*tex;
            paintcanvas_ext_rar_gldeltex(1, &id);
            *g_rar_texcount_87cce = *g_rar_texcount_87cce - 1;
            char *eng = *(char **)(t + 0x34);
            char *texEntry = (*(char ***)(t + 0x14))[i];
            *(int *)(eng + 0x70) = *(int *)(eng + 0x70) - *(int *)(texEntry + 0x18);
            tex = (*(int ***)(t + 0x14))[i];
        }
        if (tex != 0) {
            paintcanvas_ext_rar_str_dtor((char *)tex + 4);
            paintcanvas_ext_rar_op_delete(tex);
        }
        (*(int ***)(t + 0x14))[i] = 0;
    }
    *(int *)(t + 0x10) = 0;

    // fonts
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x140); i++) {
        if ((*(void ***)(t + 0x144))[i] != 0) {
            paintcanvas_ext_rar_fontrelease(*(void **)(t + 0x34),
                                            &(*(void ***)(t + 0x144))[i]);
        }
    }
    paintcanvas_ext_rar_arr_removeall_font(t + 0x140);

    // image2D
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x14c); i++) {
        if ((*(void ***)(t + 0x150))[i] != 0) {
            paintcanvas_ext_rar_img2drelease(*(void **)(t + 0x34),
                                             &(*(void ***)(t + 0x150))[i]);
        }
    }
    paintcanvas_ext_rar_arr_removeall_img(t + 0x14c);

    // meshes
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x24); i++) {
        char *mesh = (*(char ***)(t + 0x28))[i];
        if (mesh != 0) {
            *g_rar_tricount_87d96 = *g_rar_tricount_87d96 - *(int *)(mesh + 0x7c);
            paintcanvas_ext_rar_meshrelease(*(void **)(t + 0x34),
                                            &(*(void ***)(t + 0x28))[i]);
        }
    }
    paintcanvas_ext_rar_arr_removeall_mesh(t + 0x24);

    // transforms
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x158); i++) {
        void *tf = (*(void ***)(t + 0x15c))[i];
        if (tf != 0) {
            paintcanvas_ext_rar_op_delete(paintcanvas_ext_rar_transform_dtor(tf));
            (*(void ***)(t + 0x15c))[i] = 0;
        }
    }
    paintcanvas_ext_rar_arr_removeall_tf(t + 0x158);

    // cameras
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x164); i++) {
        void *cam = (*(void ***)(t + 0x168))[i];
        if (cam != 0) {
            paintcanvas_ext_rar_op_delete(cam);
            (*(void ***)(t + 0x168))[i] = 0;
        }
    }
    paintcanvas_ext_rar_arr_removeall_cam(t + 0x164);
    *(int *)(t + 0x170) = -1;

    // materials
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x174); i++) {
        void *mat = (*(void ***)(t + 0x178))[i];
        if (mat != 0) {
            paintcanvas_ext_rar_op_delete(paintcanvas_ext_rar_material_dtor(mat));
            (*(void ***)(t + 0x178))[i] = 0;
        }
    }
    paintcanvas_ext_rar_arr_removeall_mat(t + 0x174);

    // sprite systems
    for (unsigned int i = 0; i < *(unsigned int *)(t + 0x180); i++) {
        if ((*(void ***)(t + 0x184))[i] != 0) {
            paintcanvas_ext_rar_ssrelease(*(void **)(t + 0x34),
                                          &(*(void ***)(t + 0x184))[i]);
        }
    }
    paintcanvas_ext_rar_arr_removeall_ss(t + 0x180);
    *(int *)(t + 0x1cc) = 0;
}

// ---- TransformGet2DPickedTextureRegion_7b590.cpp ----
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

// ---- CheckString_78194.cpp ----
void CheckString(AbyssEngine::PaintCanvas *self, unsigned int index, void *str)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        return paintcanvas_ext_check_string(font, len, *(unsigned int *)((char *)str + 0x8));
    }
}

// ---- ReloadTextures_7e8e4.cpp ----
extern "C" int paintcanvas_ext_rt_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);

void ReloadTextures(AbyssEngine::PaintCanvas *self)
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < self->field_0x10; i++) {
        char *res = (self->field_0x14)[i];
        if (*(int *)res == -1) {
            char *path = paintcanvas_ext_rt_getAEChar(res + 4);
            float f = *(float *)((self->field_0x14)[i] + 0x10);
            int ok = paintcanvas_ext_rt_texfromfile(self->field_0x34, path, 0, 0,
                                                    &out, false, f);
            if (ok == 1) {
                *(int *)((self->field_0x14)[i]) = 0;
            }
            paintcanvas_ext_rt_deletearr(path);
        }
    }
}

// ---- MaterialCreate_79e4c.cpp ----
namespace AbyssEngine {
void ArrayAddMaterial(Material *item, void *array);
}

void MaterialCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_matc_findres(self, param_1);
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
                char *tres = (char *)paintcanvas_ext_matc_findres(self, tid);
                if (tres == 0) {
                    break;
                }
                int t = *(int *)(tres + 8);
                if (t == -1) {
                    paintcanvas_ext_matc_texcreate(self, *(unsigned short *)(info + i * 2), true);
                    t = *(int *)(tres + 8);
                }
                *(int *)(mat + i * 4) = t;
            }
        }
        *(unsigned int *)(mat + 0x20) = *(unsigned int *)(info + 0x10);
        *(unsigned int *)(mat + 0x24) = *(unsigned int *)(info + 0x14);
        *(unsigned int *)(mat + 0x28) = *(unsigned int *)(info + 0x18);
        paintcanvas_ext_matc_vec_assign(mat + 0x68, info + 0x1c);
        AbyssEngine::ArrayAddMaterial((AbyssEngine::Material *)mat, (char *)self + 0x174);
        idx = self->field_0x174 - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    *param_2 = idx;
}

// ---- TransformGet2DPickedTextureRegion_7b424.cpp ----
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

// ---- DrawTransform_796a0.cpp ----
__attribute__((visibility("hidden"))) extern const double g_dt_gravscale_898d8;


void DrawTransform(AbyssEngine::PaintCanvas *self, unsigned int param_1, const float *param_2)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x158)) {
        return;
    }
    char *tf = (*(char ***)(t + 0x15c))[param_1];
    if (*(unsigned char *)(tf + 0xec) == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f; worldM[5] = 1.0f; worldM[14] = 1.0f;

    if (*(unsigned int *)(t + 0x170) < *(unsigned int *)(t + 0x164)) {
        void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
        if (*t == 0) {
            int vis = paintcanvas_ext_dt2_incamvf((*(void ***)(t + 0x15c))[param_1], 0, cam);
            if (vis == 0) {
                *(int *)(t + 4) += 1;
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

            void *grav = paintcanvas_ext_dt2_getgrav(*(void **)(t + 0x34));
            double angle = *(double *)((char *)grav + 8) * g_dt_gravscale_898d8;
            float a = (float)angle;
            int orient = *(int *)(t + 0x30);
            float ang = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dt2_sinf(ang);
            float c = paintcanvas_ext_dt2_cosf(ang);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            int vis = paintcanvas_ext_dt2_incamvf((*(void ***)(t + 0x15c))[param_1], rotM, cam);
            if (vis == 0) {
                *(int *)(t + 4) += 1;
                return;
            }

            float viewM[16];
            const float *src = param_2;
            if (src == 0) {
                src = (const float *)((*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)] + 0xc);
            }
            paintcanvas_ext_dt2_mtx_assign(viewM, src);
            paintcanvas_ext_dt2_mtx_muleq(viewM, rotM);
            paintcanvas_ext_dt2_mtx_getinv(scratch, viewM);
            paintcanvas_ext_dt2_mtx_assign(worldM, scratch);
        }
        void *cam2 = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
        paintcanvas_ext_dt2_seteye(*(void **)(t + 0x34),
                                   *(float *)((char *)cam2 + 0x18),
                                   *(float *)((char *)cam2 + 0x28),
                                   *(float *)((char *)cam2 + 0x38));
        tf = (*(char ***)(t + 0x15c))[param_1];
    }

    float ident[16];
    memset(ident, 0, sizeof(ident));
    ident[0] = 1.0f; ident[5] = 1.0f; ident[14] = 1.0f;
    paintcanvas_ext_dt2_drawrec(self, tf, ident, worldM);
}

// ---- FontSetSpacing_79c18.cpp ----
void FontSetSpacing(AbyssEngine::PaintCanvas *self, unsigned int index, short spacing)
{
    if (index < self->field_0x140) {
        void *font = (self->field_0x144)[index];
        return paintcanvas_ext_font_set_spacing(font, spacing);
    }
}

// ---- GetWidth_78fb4.cpp ----
void GetWidth(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_width(self->field_0x34);
}

// ---- CameraCreate_7b6a8.cpp ----
// AbyssEngine::PaintCanvas::CameraCreate(unsigned& out) — allocates a Camera sized to the
// current viewport and appends it to the camera array at +0x164, returning its index.

void CameraCreate(AbyssEngine::PaintCanvas *self, unsigned int *out)
{
    void *cam = operator new(0x5c);
    int w = pc_GetWidth(self);
    int h = pc_GetHeight(self);
    pc_Camera_ctor(cam, (float)h, (float)w);
    pc_ArrayAdd_Camera(cam, (char *)self + 0x164);
    *out = self->field_0x164 - 1;
}

// ---- DrawSpriteSystem_7ac20.cpp ----
__attribute__((visibility("hidden"))) extern const double g_dss_gravscale_8ada0;


void DrawSpriteSystem(AbyssEngine::PaintCanvas *self, unsigned int param_1, const float *mat)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x180)) {
        return;
    }
    void *ss = (*(void ***)(t + 0x184))[param_1];
    if (ss == 0) {
        return;
    }

    // local copy of the incoming matrix
    float local[15];
    memcpy(local, mat, sizeof(local));

    float identbuf[16];
    char scratch[60];
    float inv[16];

    if (*t == 0) {
        paintcanvas_ext_dss_mtx_getinv(inv, local);
        paintcanvas_ext_dss_mtx_assign(local, inv);
    } else {
        memset(identbuf, 0, sizeof(identbuf));
        identbuf[0] = 1.0f;
        identbuf[5] = 1.0f;
        identbuf[14] = 1.0f;
        paintcanvas_ext_dss_matidentity(scratch, identbuf);

        void *grav = paintcanvas_ext_dss_getgrav(*(void **)(t + 0x34));
        double angle = *(double *)((char *)grav + 8) * g_dss_gravscale_8ada0;
        float a = (float)angle;
        int orient = *(int *)(t + 0x30);
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
    paintcanvas_ext_dss_ssdraw(*(void **)(t + 0x34), ident2, local,
                               (*(void ***)(t + 0x184))[param_1]);
}

// ---- DrawString_78218.cpp ----
extern "C" void paintcanvas_ext_drawstring_str(void *, unsigned int, unsigned int, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void DrawString(AbyssEngine::PaintCanvas *self, unsigned int index, void *str,
                int x, int y, bool b)
{
    if (index < self->field_0x140) {
        char *font = (self->field_0x144)[index];
        paintcanvas_ext_string_prep(self, *(void **)(font + 0x8), -1);
        char *font2 = (self->field_0x144)[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        paintcanvas_ext_drawstring_str(font2, len, *(unsigned int *)((char *)str + 0x8), x, y,
                                       self, self->field_0x34, b);
    }
}

// ---- TransformGetTriCount_7a5f2.cpp ----
int TransformGetTriCount(AbyssEngine::PaintCanvas *self, char *transform)
{
    if (!transform) {
        return 0;
    }
    int total = 0;
    unsigned int n1 = *(unsigned int *)(transform + 0x3c);
    for (unsigned int i = 0; i != n1; ++i) {
        void *m = (*(void ***)(transform + 0x40))[i];
        total += paintcanvas_ext_mesh_tricount(self, m);
    }
    unsigned int n2 = *(unsigned int *)(transform + 0x4c);
    for (unsigned int i = 0; i != n2; ++i) {
        void *c = (*(void ***)(transform + 0x50))[i];
        total += paintcanvas_ext_transform_tricount(self, c);
    }
    return total;
}

// ---- MeshChangeShaderAnimValue_7a2d2.cpp ----
void MeshChangeShaderAnimValue(AbyssEngine::PaintCanvas *self, char *transform, float value, unsigned int mode)
{
    if (transform) {
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x3c); ++i) {
            void *m = (*(void ***)(transform + 0x40))[i];
            paintcanvas_ext_mesh_shaderanim(self, m, value, mode);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x4c); ++i) {
            void *c = (*(void ***)(transform + 0x50))[i];
            paintcanvas_ext_transform_shaderanim(self, c, value, mode);
        }
    }
}

// ---- ReleaseSpriteSystemResource_7c7f4.cpp ----
void ReleaseSpriteSystemResource(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < self->field_0x180) {
        void *ctx = self->field_0x34;
        char *arr = self->field_0x184;
        return paintcanvas_ext_release_sprite_res(ctx, arr + index * 4);
    }
}

// ---- DrawSpriteSystem_7adb0.cpp ----
__attribute__((visibility("hidden"))) extern const double g_dss2_gravscale_8af58;


void DrawSpriteSystem(AbyssEngine::PaintCanvas *self, unsigned int param_1,
                      const float *matA, const float *matB)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x180)) {
        return;
    }
    if ((*(void ***)(t + 0x184))[param_1] == 0) {
        return;
    }

    // local copies: matA -> world (local_a0), matB -> view (local_64)
    float world[15];
    float view[15];
    memcpy(world, matA, sizeof(world));
    memcpy(view, matB, sizeof(view));

    if (*t == 0) {
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

        void *grav = paintcanvas_ext_dss2_getgrav(*(void **)(t + 0x34));
        double angle = *(double *)((char *)grav + 8) * g_dss2_gravscale_8af58;
        int ia = (int)(long long)angle;
        if (*(int *)(t + 0x30) == 1) {
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

    paintcanvas_ext_dss2_ssdraw(*(void **)(t + 0x34), world, view,
                                (*(void ***)(t + 0x184))[param_1]);
}
