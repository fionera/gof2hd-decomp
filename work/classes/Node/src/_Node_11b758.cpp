#include "class.h"

// Node::~Node()
//   ArrayRelease<Node*>(this->children);
struct Node { ~Node(); };
Node::~Node()
{
    Node_ArrayNode_release(pp(this, 0x00));
}
