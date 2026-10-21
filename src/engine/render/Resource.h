#ifndef GOF2_RESOURCE_H
#define GOF2_RESOURCE_H

namespace AbyssEngine {
    // Resource kinds as stored in Resource::type (values from the original resource tables).
    enum ResourceKind {
        RESOURCE_FONT = 1,
        RESOURCE_TEXTURE = 2,
        RESOURCE_IMAGE = 3,
        RESOURCE_MESH = 4,
        RESOURCE_MATERIAL = 6
    };

    // One entry of the PaintCanvas resource list (16 bytes). `data` points at the
    // kind-specific descriptor (ResourceTexture / ResourceImage / ResourceMesh /
    // ResourceMaterial); `index` is the loaded-object slot, -1 until loaded.
    struct Resource {
        unsigned short id;
        int type;
        int index;
        void *data; // lint: void_ptr (heterogeneous resource payload dispatched by kind; no common base)

        Resource(unsigned short resId, int kind, void *payload) // lint: void_ptr (heterogeneous resource payload dispatched by kind; no common base)
            : id(resId), type(kind), index(-1), data(payload) {}
    };
}

#endif
