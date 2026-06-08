#include "class.h"

// AbyssEngine::PaintCanvas::TransformGetTransform(unsigned index)
// Returns the internal Transform pointer for a transform handle. (Ghidra mis-decoded this tiny
// accessor; modeled on its sibling TransformGetLocal/TransformGetTriCount which index the
// transform array at +0x158 count / +0x15c data.)
void *TransformGetTransform(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x158)) {
        return (*(void ***)((char *)self + 0x15c))[index];
    }
    return 0;
}
