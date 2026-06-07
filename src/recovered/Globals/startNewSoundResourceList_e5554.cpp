#include "class.h"

extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" void ArrayInt_release(Array<int> *a);
extern "C" void *ArrayInt_dtor(Array<int> *a);
extern "C" void ArrayInt_ctor(Array<int> *a);
extern "C" void ArrayInt_add(int val, Array<int> *a);
extern "C" void Globals_startNewSoundResourceList_tail(int val, Array<int> *a);

extern "C" void Globals_startNewSoundResourceList(void *self)
{
    if (F<Array<int> *>(self, 4) != 0) {
        ArrayInt_release(F<Array<int> *>(self, 4));
        if (F<Array<int> *>(self, 4) != 0) {
            operator_delete(ArrayInt_dtor(F<Array<int> *>(self, 4)));
        }
    }
    F<Array<int> *>(self, 4) = 0;
    Array<int> *a = (Array<int> *)operator_new(0xc);
    ArrayInt_ctor(a);
    F<Array<int> *>(self, 4) = a;
    ArrayInt_add(0x7c, a);
    return Globals_startNewSoundResourceList_tail(0x7b, F<Array<int> *>(self, 4));
}
