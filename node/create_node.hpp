#ifndef CREATE_NODE_HPP
#define CREATE_NODE_HPP

#include "math_node.hpp"


/// Return dynamically created MathNode or nullptr if given range is whitespace.
MathNode* CreateNode(const char* begin, const char* end);


#endif // CREATE_NODE_HPP

