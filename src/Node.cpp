#include "gof2/Node.h"




// ---- Node_11b710.cpp ----
// Node::Node(int)
//   field_04 = 0; field_08 = param_1; children = new Array<Node*>();
Node::Node(int param_1)
{
    this->field_0x4 = 0;
    this->value = param_1;
    void *arr = operator new(0xc);
    Node_ArrayNode_ctor(arr);
    this->children = (Array<Node*>*)arr;
}

// ---- _Node_11b758.cpp ----
// Node::~Node()
//   ArrayRelease<Node*>(this->children);
Node::~Node()
{
    Node_ArrayNode_release(this->children);
}
