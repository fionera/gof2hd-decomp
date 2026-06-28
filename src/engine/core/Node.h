#ifndef GOF2_NODE_H
#define GOF2_NODE_H
#include "engine/core/Array.h"
#include "AEString.h"

class Node {
public:
    Array<Node *> *children;
    Node *parent;
    int value;
    uint8_t reserved_0x0c[0x24];

    unsigned int spriteMaterial;

    Node(int value);

    ~Node();
};

#endif
