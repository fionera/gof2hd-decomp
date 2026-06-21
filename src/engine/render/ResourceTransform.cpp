#include "engine/render/ResourceTransform.h"

namespace AbyssEngine {

// ResourceTransform is only ever destroyed as a base subobject, so the binary
// carries a single out-of-line destructor variant — the base-object dtor (D2),
// taken by address from the engine's object-destruction tables. It has no
// complete-object (D1) or deleting (D0) counterpart. A plain out-of-line C++
// destructor would force clang to also outline D1, which the original lacks, so
// we emit only D2 directly under its mangled name (same asm-label idiom as
// String.cpp's D0). Body: release the two owned heap arrays and null the slots.
// Returns `self`: the Itanium ABI base-object destructor yields `this` (the
// trailing `mov r0,r4` in the original), so we mirror that to byte-match.
ResourceTransform *ResourceTransform_base_dtor(ResourceTransform *self) asm("_ZN11AbyssEngine17ResourceTransformD2Ev");
[[gnu::used]] ResourceTransform *ResourceTransform_base_dtor(ResourceTransform *self)
{
    if (self->dataA != nullptr) {
        ::operator delete[](self->dataA);
    }
    self->dataA = nullptr;

    if (self->dataB != nullptr) {
        ::operator delete[](self->dataB);
    }
    self->dataB = nullptr;

    return self;
}

} // namespace AbyssEngine
