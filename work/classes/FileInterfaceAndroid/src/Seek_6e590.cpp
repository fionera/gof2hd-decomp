#include "class.h"

extern "C" void *malloc(unsigned int n);
extern "C" void free(void *p);
extern "C" unsigned int zip_fread(void *zf, void *buf, unsigned int n);
extern "C" int fseek(void *f, long off, int whence);

// FileInterfaceAndroid::Seek(unsigned int) — skip `n` bytes forward. For a zip handle this means
// reading (and discarding) into a scratch buffer; for a stdio handle a relative fseek.
// NOTE: near-match (33/33 instruction multiset identical; ~2 instrs differ in scheduling slot —
// where `n` and the malloc result are spilled to callee-saved regs relative to their cbz checks).
extern "C" bool FileInterfaceAndroid_Seek(FileInterfaceAndroid *self, unsigned int n)
{
    if (n == 0)
        return true;
    void *zf = self->f_c;
    int delta;
    if (zf != 0) {
        void *tmp = malloc(n);
        if (tmp == 0)
            return false;
        unsigned int got = zip_fread(zf, tmp, n);
        free(tmp);
        delta = got - n;
    } else {
        void *f = self->f_8;
        if (f == 0)
            return false;
        delta = fseek(f, n, 1);
    }
    return delta == 0;
}
