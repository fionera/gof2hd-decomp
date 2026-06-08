#include "class.h"

// Node::Node(int)
//   field_04 = 0; field_08 = param_1; children = new Array<Node*>();
struct Node { Node(int param_1); };
Node::Node(int param_1)
{
    i32(this, 0x04) = 0;
    i32(this, 0x08) = param_1;
    void *arr = operator new(0xc);
    Node_ArrayNode_ctor(arr);
    pp(this, 0x00) = arr;
}
