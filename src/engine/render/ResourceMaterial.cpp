#include "engine/render/ResourceMaterial.h"

namespace AbyssEngine {

// ResourceMaterial is only ever constructed as a complete object whose
// constructor is identical to the base-object form, so the binary carries a
// single out-of-line constructor variant per signature — the base-object
// constructor (C2). It has no complete-object (C1) counterpart: the original
// toolchain folded C1 into C2 and kept only C2, so every `new ResourceMaterial`
// binds to the C2 symbol. A plain out-of-line C++ constructor would force clang
// to also outline C1, which the original lacks, so we emit only C2 directly
// under its mangled name (same asm-label idiom as ResourceTransform's D2).
//
// field_1c is initialised to 0x3f800000, the IEEE-754 bit pattern of 1.0f.

// ResourceMaterial::ResourceMaterial(uint16_t texId, uint16_t texId2, BlendMode blend)
void ResourceMaterial_base_ctor(ResourceMaterial *self, uint16_t texId, uint16_t texId2,
                                BlendMode blend) asm("_ZN11AbyssEngine16ResourceMaterialC2EttNS_9BlendModeE");
[[gnu::used]] void ResourceMaterial_base_ctor(ResourceMaterial *self, uint16_t texId,
                                              uint16_t texId2, BlendMode blend)
{
    self->blendMode = static_cast<int>(blend);
    self->field_14  = 0;
    self->field_18  = 0;
    self->field_1c  = 0x3f800000;
    self->field_20  = 0;
    self->field_24  = 0;
    for (int i = 0; i != 8; ++i)
        self->texIndices[i] = 0xffff;
    self->texIndices[1] = texId2;
    self->texIndices[0] = texId;
}

// ResourceMaterial::ResourceMaterial(uint16_t texId, BlendMode blend)
void ResourceMaterial_base_ctor(ResourceMaterial *self, uint16_t texId,
                                BlendMode blend) asm("_ZN11AbyssEngine16ResourceMaterialC2EtNS_9BlendModeE");
[[gnu::used]] void ResourceMaterial_base_ctor(ResourceMaterial *self, uint16_t texId, BlendMode blend)
{
    self->blendMode = static_cast<int>(blend);
    self->field_14  = 0;
    self->field_18  = 0;
    self->field_1c  = 0x3f800000;
    self->field_20  = 0;
    self->field_24  = 0;
    for (int i = 0; i != 8; ++i)
        self->texIndices[i] = 0xffff;
    self->texIndices[0] = texId;
}

} // namespace AbyssEngine
