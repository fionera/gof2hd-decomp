#include "class.h"

// FileInterfaceAndroid::~FileInterfaceAndroid() [deleting destructor] — runs the complete-object
// dtor (external), then tail-calls operator delete:
//   blx <complete dtor> ; pop {r7,lr} ; b.w <operator delete>
extern "C" void *FileInterfaceAndroid_completeDtor(FileInterfaceAndroid *self);
extern "C" void operator_delete(void *p);

FileInterfaceAndroid::~FileInterfaceAndroid()
{
    operator_delete(FileInterfaceAndroid_completeDtor(this));
}
