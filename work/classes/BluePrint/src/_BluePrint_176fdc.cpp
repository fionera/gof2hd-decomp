#include "class.h"

extern "C" void ArrayReleaseInt(Array<int> *a);   // ArrayRelease<int>
extern "C" void *ArrayInt_dtor(Array<int> *a);    // Array<int>::~Array
extern "C" void operator_delete(void *p);
extern "C" void String_dtor(void *p);             // AbyssEngine::String::~String

// BluePrint::~BluePrint() (D2). Returns this.
extern "C" void *_ZN9BluePrintD2Ev(BluePrint *self)
{
    Array<int> *a = F<Array<int> *>(self, 0x0);
    if (a != 0) {
        ArrayReleaseInt(a);
        Array<int> *a2 = F<Array<int> *>(self, 0x0);
        if (a2 != 0)
            operator_delete(ArrayInt_dtor(a2));
    }
    F<Array<int> *>(self, 0x0) = 0;
    String_dtor((char *)self + 0x14);
    return self;
}
