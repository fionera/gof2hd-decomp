#include "class.h"

namespace AbyssEngine {

// AbyssEngine::SpecCubeMapping::SpecCubeMapping()
//   Runs the ShaderBaseStruct base ctor, installs the SpecCubeMapping vtable,
//   publishes its shader index, and stores the shader resource name.
SpecCubeMapping::SpecCubeMapping()
{
    new ((void *)this) ShaderBaseStruct();

    // Install vtable (the +8 skips the RTTI/offset-to-top header words).
    *(void **)this = (void *)(_ZTVN11AbyssEngine15SpecCubeMappingE + 8);

    // SpecCubeMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern.
    SpecCubeMapping::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;

    String name("SpecCubeMapping", false);
    *(String *)((char *)this + 0xc) = name;
}

} // namespace AbyssEngine
