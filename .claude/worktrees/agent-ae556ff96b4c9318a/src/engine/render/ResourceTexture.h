#ifndef GOF2_RESOURCETEXTURE_H
#define GOF2_RESOURCETEXTURE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {

class String;

// AbyssEngine::ResourceTexture — pairs a heap-owned texture resource name with a
// scalar value (e.g. an LOD bias or weighting factor).
class ResourceTexture {
public:
    char* name;    // heap copy of the resource name C-string
    float value;   // associated scalar value

    ResourceTexture(const char *name, float value);
    ResourceTexture(const String &name, float value);
    ~ResourceTexture();
};

} // namespace AbyssEngine

#endif
