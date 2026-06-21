#include "engine/core/Node.h"

Node::Node(int value)
{
    this->parent = nullptr;
    this->value = value;
    this->children = new Array<Node*>();
}

Node::~Node()
{
    delete this->children;
}

// The original emits ArrayRelease<Node*> out-of-line (it frees a Node tree's
// child list without deleting the child Nodes). Pin the instantiation here, in
// the TU that owns the Node type.
template void ArrayRelease<Node *>(Array<Node *> &);
