#ifndef GOF2_NODE_H
#define GOF2_NODE_H
#include "gof2/common.h"

// Galaxy on Fire 2 -- Node: a scene-graph node holding a list of child nodes,
// a back-pointer to its parent, and an integer value/identifier.
class Node {
public:
    Array<Node*>* children;
    Node*         parent;
    int           value;
    unsigned int  field_0x30;

    Node(int value);
    ~Node();
};

#endif
