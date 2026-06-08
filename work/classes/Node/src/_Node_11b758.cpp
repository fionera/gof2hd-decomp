#include "class.h"

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
