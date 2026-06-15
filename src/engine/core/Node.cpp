#include "gof2/engine/core/Node.h"

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
