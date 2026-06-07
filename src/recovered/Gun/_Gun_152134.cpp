#include "class.h"

// Gun::~Gun() — real C++ destructor so the demangled symbol contains "~Gun".

extern "C" __attribute__((nothrow)) void Gun_operator_delete(void *p);      // operator delete(void*)
extern "C" __attribute__((nothrow)) void Gun_operator_delete_arr(void *p);  // operator delete[](void*)

struct VecArray;
extern "C" void Gun_ArrayReleaseClasses(VecArray *a);     // ArrayReleaseClasses<Vector*>(Array*)
extern "C" void *Gun_ArrayDtor(VecArray *a);              // Array<Vector*>::~Array(Array*)

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gGunStringDtor __attribute__((visibility("hidden")));

struct Gun {
    void *f_0;
    ~Gun() noexcept(false);
};

__attribute__((minsize)) Gun::~Gun() noexcept(false)
{
    char *self = (char *)this;

    if (*(void **)(self + 0x3c) != 0)
        Gun_operator_delete_arr(*(void **)(self + 0x3c));
    *(void **)(self + 0x3c) = 0;

    Gun_operator_delete_arr(*(void **)(self + 0x40));
    *(void **)(self + 0x40) = 0;

    if (*(void **)(self + 0x10c) != 0)
        Gun_operator_delete_arr(*(void **)(self + 0x10c));
    *(void **)(self + 0x10c) = 0;

    Gun_operator_delete_arr(*(void **)(self + 0x110));
    *(void **)(self + 0x110) = 0;

    VecArray *arr = *(VecArray **)(self + 0xac);
    if (arr != 0) {
        Gun_ArrayReleaseClasses(arr);
        VecArray *arr2 = *(VecArray **)(self + 0xac);
        if (arr2 != 0) {
            void *p = Gun_ArrayDtor(arr2);
            Gun_operator_delete(p);
        }
    }
    *(void **)(self + 0xac) = 0;

    dtor_fn d = gGunStringDtor;
    d(self + 0x2c);
    d(self + 0x20);
    d(self + 0x14);
    d(self + 0x8);
}
