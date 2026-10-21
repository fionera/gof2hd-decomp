#ifndef GOF2_NODE_H
#define GOF2_NODE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- Node: a scene-graph / path-finder node holding a list of
// child nodes, a back-pointer to its parent, and an integer value/identifier.
// The same class is reused by the engine sprite system, which stows a material
// handle in the slot at +0x30.
class Node {
public:
    Array<Node*>* children;          // +0x00  child / neighbour nodes
    Node*         parent;            // +0x04  back-pointer to parent
    int           value;            // +0x08  node value / system index
    uint8_t       reserved_0x0c[0x24]; // +0x0c  unused padding up to +0x30
    // Sprite-system material handle. Other TUs (PaintCanvas) still read it as
    // field_0x30, so keep that name working via the union alias.
    union {
        unsigned int spriteMaterial; // +0x30  material handle (sprite system)
        unsigned int field_0x30;
    };

    Node(int value);
    ~Node();
};

#endif
