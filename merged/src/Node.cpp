#include "Node.h"




// ---- Node_11b710.cpp ----
// Node::Node(int)
//   field_04 = 0; field_08 = param_1; children = new Array<Node*>();
struct Node {
    // @portable-fields
    void* f_0; // 0x0
    int f_4; // 0x4
    int f_8; // 0x8
 Node(int param_1); };
Node::Node(int param_1)
{
    this->f_4 = 0;
    this->f_8 = param_1;
    void *arr = operator new(0xc);
    Node_ArrayNode_ctor(arr);
    this->f_0 = arr;
}

// ---- _Node_11b758.cpp ----
// Node::~Node()
//   ArrayRelease<Node*>(this->children);
struct Node {
    // @portable-fields
    void* f_0; // 0x0
    int f_4; // 0x4
    int f_8; // 0x8
 ~Node(); };
Node::~Node()
{
    Node_ArrayNode_release(this->f_0);
}
