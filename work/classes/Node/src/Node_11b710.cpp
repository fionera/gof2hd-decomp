#include "class.h"

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
