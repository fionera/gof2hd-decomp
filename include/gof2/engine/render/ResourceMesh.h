#ifndef GOF2_RESOURCEMESH_H
#define GOF2_RESOURCEMESH_H
#include <cstdint>

namespace AbyssEngine {

// A named mesh resource: owns a heap copy of its name plus a resource id and flag.
class ResourceMesh {
public:
    char *name;             // heap copy of the mesh name
    uint16_t wResourceId;   // resource id
    uint8_t flag;

    ResourceMesh(const char *name, unsigned short id, bool flag);
    ~ResourceMesh();
};

} // namespace AbyssEngine

#endif
