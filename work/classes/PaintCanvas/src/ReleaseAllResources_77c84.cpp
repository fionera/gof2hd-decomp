#include "class.h"

__attribute__((visibility("hidden"))) extern int *const g_rar_curtex_87c98;
__attribute__((visibility("hidden"))) extern int *const g_rar_texcount_87cce;
__attribute__((visibility("hidden"))) extern int *const g_rar_tricount_87d96;

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
