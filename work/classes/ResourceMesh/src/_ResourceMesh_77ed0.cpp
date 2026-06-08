#include "class.h"

namespace AbyssEngine {

// AbyssEngine::ResourceMesh::~ResourceMesh()
ResourceMesh::~ResourceMesh()
{
    operator_delete__(*(void **)this);
    *(void **)this = 0;
}

} // namespace AbyssEngine
