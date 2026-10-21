#ifndef GOF2_NODE_H
#define GOF2_NODE_H
#include "engine/core/Array.h"
#include "AEString.h"

class Node {
public:
    Array<Node *> *children;
    Node *parent;
    int value;

    Node(int value);

    ~Node();
};

#endif
