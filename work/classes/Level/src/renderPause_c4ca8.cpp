#include "class.h"

void Level::renderPause() {
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
            (*(void (**)(int *))(*o + 0x24))(o);
            a = *(unsigned int **)((char *)this + 0xf8);
        }
    }
    a = *(unsigned int **)((char *)this + 0xfc);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x24))(o);
            a = *(unsigned int **)((char *)this + 0xfc);
        }
    }
    a = *(unsigned int **)((char *)this + 0xf4);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            (*(void (**)(int *))(*o + 0x24))(o);
            a = *(unsigned int **)((char *)this + 0xf4);
        }
    }
    a = *(unsigned int **)((char *)this + 0x100);
    if (a != 0) {
        for (unsigned int i = 0; i < *a; i = i + 1) {
            int *o = ((int **)a[1])[i];
            if (o != 0) {
                (*(void (**)(int *))(*o + 0x24))(o);
                a = *(unsigned int **)((char *)this + 0x100);
            }
        }
    }
}
