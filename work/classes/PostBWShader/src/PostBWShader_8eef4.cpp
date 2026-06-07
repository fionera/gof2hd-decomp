#include "class.h"

extern "C" uint32_t PostBWShader_ctor_copy_src;
extern "C" uint32_t PostBWShader_ctor_copy_dst;

namespace AbyssEngine {

PostBWShader::PostBWShader()
{
    uint32_t copied = PostBWShader_ctor_copy_src;
    PostBWShader_ctor_copy_dst = copied;

    uint32_t storage[4];
    String *name = new (storage) String("PostBWShader", false);
    *reinterpret_cast<String *>(bytes(this) + 0xc) = *name;
    name->~String();
}

} // namespace AbyssEngine
