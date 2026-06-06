// AbyssEngine Array<T> container — free-function template primitives.
// Array<T> layout = { u32 size; T* data; u32 size2(capacity); }  (== AEArray, validated).
// Recovered from libgof2hdaa.so:
//   ArrayAdd       @0x805b4  (matched 15/15)
//   ArraySetLength @0xb721c
//   ArrayRemoveAll @0x8fb3c
// All pointer instantiations compile to identical code, so this template matches every
// Array<T*> instantiation in the binary.
extern "C" void* realloc(void* p, unsigned int n);
extern "C" void* memset(void* p, int c, unsigned int n);

template <class T>
struct Array { unsigned int size; T* data; unsigned int size2; };

template <class T>
void ArrayAdd(T item, Array<T>& a) {
    a.size2 = a.size + 1;
    a.data  = (T*)realloc(a.data, a.size2 * sizeof(T));
    a.data[a.size] = item;
    a.size  = a.size2;
}

template <class T>
void ArraySetLength(unsigned int n, Array<T>& a) {
    T* ptr;
    unsigned int cap;
    if (a.size2 == n) {
        ptr = a.data;
        cap = n;
    } else {
        cap = n ? n : 1;
        a.size2 = cap;
        ptr = (T*)realloc(a.data, cap * sizeof(T));
        a.data = ptr;
        cap = a.size2;
    }
    memset(ptr, 0, cap * sizeof(T));   // __aeabi_memclr4
    a.size = n;
}

template <class T>
void ArrayRemoveAll(Array<T>& a) {
    a.size2 = 1;
    T* d = a.data;        // load data before zeroing size, matching target schedule
    a.size  = 0;
    a.data  = (T*)realloc(d, sizeof(T));
    memset(a.data, 0, a.size2 * sizeof(T));
}

// Force emission of the pointer instantiations (identical code for all T*).
template void ArrayAdd<void*>(void*, Array<void*>&);
template void ArraySetLength<void*>(unsigned int, Array<void*>&);
template void ArrayRemoveAll<void*>(Array<void*>&);
