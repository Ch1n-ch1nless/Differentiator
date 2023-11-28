#include "tree.h"

Node* NodeCtor(error_t* error, tree_elem_t element)
{
    PTR_ASSERT(error);

    Node* new_node = nullptr;

    new_node = (Node*) calloc(1, sizeof(Node));
    if (new_node == nullptr)
    {
        *error = *error | MEM_ALLOC_ERR;
        return new_node;
    }

    new_node->data = element;

    new_node->left  = nullptr;

    new_node->right = nullptr;

    return new_node;
}

error_t TreeCtor(Tree* tree, const char* name, const char* file, const int line)
{
    PTR_ASSERT(tree)
    PTR_ASSERT(name)
    PTR_ASSERT(file)

    assert((line > 0) && "ERROR! Line is fewer then zero!!!\n");

    error_t error = NO_ERR;

    tree->root = NodeCtor(&error, TREE_POISON_VALUE);

    tree->size = 0;

    tree->name = name;

    tree->line = line;

    tree->file = file;

    return error;
}

error_t SubTreeDtor(Node** node)
{
    PTR_ASSERT(node)
    PTR_ASSERT(*node)

    error_t error = NO_ERR;

    if ((*node)->left != nullptr)
    {
        error |= SubTreeDtor(&((*node)->left));
    }

    if ((*node)->right != nullptr)
    {
        error |= SubTreeDtor(&((*node)->right));
    }

    free((*node)->data);

    free(*node);

    *node = nullptr;

    return error;
}

error_t TreeDtor(Tree* tree)
{
    PTR_ASSERT(tree)

    error_t error = NO_ERR;

    error |= SubTreeDtor(&(tree->root));

    tree->size = 0;

    tree->name = nullptr;

    tree->file = nullptr;

    tree->line = -1;

    return error;
}

error_t TreeInsert(Tree* tree, tree_elem_t string, Node* cur_node, AddNode indicator)
{
    PTR_ASSERT(tree)
    PTR_ASSERT(cur_node)

    error_t error = NO_ERR;

    Node* new_node  = nullptr;

    new_node = NodeCtor(&error, string);

    if (cur_node == nullptr)
    {
        tree->root = new_node;
    }
    else
    {
        if (indicator == ADD_LEFT_NODE)
        {
            cur_node->left = new_node;
        }
        else if (indicator == ADD_RIGHT_NODE)
        {
            cur_node->right = new_node;
        }
        else
        {
            error = UNKNOWN_INDICATOR_ERR;
        }
    }

    CHECK_TREE_ERROR(tree, error);

    return error;
}
