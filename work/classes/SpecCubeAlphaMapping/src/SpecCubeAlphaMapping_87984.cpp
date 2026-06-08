#include "class.h"

// AbyssEngine::SpecCubeAlphaMapping::SpecCubeAlphaMapping()
// Chains the ShaderBaseStruct ctor, installs the vtable, registers the instance in a global,
// and stores the shader's resource-name String at +0xc.
extern "C" {
extern void **g_SCAM_globalSlotSrc;   // *(undefined4**)(DAT_00097a00...)
extern void **g_SCAM_globalSlotDst;   // *(undefined4**)(DAT_00097a08...)
extern const char g_SCAM_name[];      // resource-name string literal
}

namespace AbyssEngine {

SpecCubeAlphaMapping::SpecCubeAlphaMapping()
{
    ShaderBaseStruct_ctor(this);

    // install vtable (target adds +8 to the table base) and register self in the global slot
    i32(this, 0x00) = (int)(SpecCubeAlphaMapping_vtable + 8);
    *g_SCAM_globalSlotDst = *g_SCAM_globalSlotSrc;

    String name(g_SCAM_name, false);
    *(String *)((char *)this + 0xc) = name;
}

} // namespace AbyssEngine
