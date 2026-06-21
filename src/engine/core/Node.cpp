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

template<class T>
void ArrayRelease(Array<T> &a)
{
    if (a.data_) ::operator delete[](a.data_);
    a.data_ = nullptr;
}

// The original library exports this instantiation out-of-line.
template void ArrayRelease<Node*>(Array<Node*> &);
