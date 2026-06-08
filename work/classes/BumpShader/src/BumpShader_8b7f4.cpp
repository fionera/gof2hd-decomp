#include "class.h"

namespace AbyssEngine {

// AbyssEngine::BumpShader::BumpShader()
BumpShader::BumpShader()
{
    ShaderBaseStruct_ctor(this);

    // install vtable (+8 past RTTI/offset slots) and copy the type-info index static.
    *(int *)this = (int)(BumpShader_vtable + 8);
    BumpShader_typeInfoDest = BumpShader_typeInfoSource;

    // name = String("BumpShader", false); this->name(0xc) = name; ~name.
    String name(BumpShader_name, false);
    String_assign((String *)((char *)this + 0xc), &name);
    String_dtor(&name);
}

} // namespace AbyssEngine
