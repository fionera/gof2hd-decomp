#include "class.h"

extern "C" void ParticleSystemManager_render3d(int mgr);
extern "C" void Level_render_tail(int sys);

void Level::render(int ctx) {
    unsigned int *a;
    a = *(unsigned int **)((char *)this + 0xe4);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x14))(o);
            a = *(unsigned int **)((char *)this + 0xe4);
        }
    }
    a = *(unsigned int **)((char *)this + 0xe8);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x14))(o);
            a = *(unsigned int **)((char *)this + 0xe8);
        }
    }
    a = *(unsigned int **)((char *)this + 0xf8);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
            int *o2 = ((int **)(*(int *)(*(int *)((char *)this + 0xf8) + 4)))[i];
            (*(void (**)(int *))(*o2 + 0x24))(o2);
            a = *(unsigned int **)((char *)this + 0xf8);
        }
    }
    a = *(unsigned int **)((char *)this + 0xfc);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
            int *o2 = ((int **)(*(int *)(*(int *)((char *)this + 0xfc) + 4)))[i];
            (*(void (**)(int *))(*o2 + 0x24))(o2);
            a = *(unsigned int **)((char *)this + 0xfc);
        }
    }
    a = *(unsigned int **)((char *)this + 0xf4);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
            int *o2 = ((int **)(*(int *)(*(int *)((char *)this + 0xf4) + 4)))[i];
            (*(void (**)(int *))(*o2 + 0x24))(o2);
            a = *(unsigned int **)((char *)this + 0xf4);
        }
    }
    a = *(unsigned int **)((char *)this + 0x100);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            if (o != 0) {
                (*(void (**)(int *, int))(*o + 0x34))(o, ctx);
                int *o2 = ((int **)(*(int *)(*(int *)((char *)this + 0x100) + 4)))[i];
                (*(void (**)(int *))(*o2 + 0x24))(o2);
                a = *(unsigned int **)((char *)this + 0x100);
            }
        }
    }
    if (*(int *)((char *)this + 0x80) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x80));
    }
    if (*(int *)((char *)this + 0x74) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x74));
    }
    if (*(int *)((char *)this + 0x78) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x78));
    }
    if (*(int *)((char *)this + 0x7c) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x7c));
    }
    if (*(int *)((char *)this + 0x88) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x88));
    }
    if (*(int *)((char *)this + 0x84) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x84));
    }
    if (*(int *)((char *)this + 0x8c) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x8c));
    }
    if (*(int *)((char *)this + 0x98) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x98));
    }
    if (*(int *)((char *)this + 0x94) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x94));
    }
    if (*(int *)((char *)this + 0x9c) != 0) {
        ParticleSystemManager_render3d(*(int *)((char *)this + 0x9c));
    }
    return Level_render_tail(starSystem);
}
