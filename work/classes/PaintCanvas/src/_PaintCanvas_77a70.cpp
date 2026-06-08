#include "class.h"

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
