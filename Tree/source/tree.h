#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "tree_error.h"

#define TREE_CTOR(tree) TreeCtor(tree, #tree, __FILE__, __LINE__);

enum AddNode : int
{
    ADD_LEFT_NODE  = -1,
    ADD_RIGHT_NODE =  1
};

error_t  TreeCtor(Tree* tree, const char* name, const char* file, const int line);
Node*    NodeCtor(error_t* error, tree_elem_t element);

error_t  TreeDtor(Tree* tree);
error_t  SubTreeDtor(Node** node);

error_t TreeInsert(Tree* tree, tree_elem_t element, Node* new_node, AddNode indicator);

#endif // TREE_H_INCLUDED
