#include "tree_dump.h"

void TreeTextDump(Tree* tree, FILE* output, const char* file, const int line,
                                            const char* func, Dump_mode mode )
{
    PTR_ASSERT(tree);
    PTR_ASSERT(output);
    PTR_ASSERT(file);
    PTR_ASSERT(func);

    assert((line > 0) && "ERROR! Line is fewer then zero!!!\n");

    Node* start_node = tree->root;

    fprintf(output, "Tree \"%s\": [%p] from %s(%d)\n", tree->name, tree, tree->file, tree->line);
    fprintf(output, "called from file: %s(%d) in function: %s\n{\n", file, line, func);

    fprintf(output, "\tRoot of tree:   [%p]\n\n", tree->root);

    fprintf(output, "\tNumber of nodes: %u\n\n\t\t", tree->size);

    if (mode == PRE_ORDER)
    {
        PrintInPreOrder(start_node, output);
    }
    else if (mode == POST_ORDER)
    {
        PrintInPostOrder(start_node, output);
    }
    else if (mode == IN_ORDER)
    {
        PrintInInOrder(start_node, output);
    }
    else
    {
        fprintf(output, "Wrong mode!!!\n");
    }

    fprintf(output, "\n}\n");
}

void PrintInPreOrder(Node* node, FILE* output)
{
    assert((output != nullptr) && "Pointer to output is NULL!!!\n");

    if (node == nullptr)
    {
        return;
    }

    fprintf(output, "( \"" tree_format "\" ", node->data);

    PrintInPreOrder(node->left,  output);

    fprintf(output, " ");

    PrintInPreOrder(node->right, output);

    fprintf(output, " ) ");

    return;
}

void PrintInPostOrder(Node* node, FILE* output)
{
    assert((output != nullptr) && "Pointer to output is NULL!!!\n");

    if (node == nullptr)
    {
        fprintf(output, "\"nil\"");
        return;
    }

    fprintf(output, "( ");

    PrintInPostOrder(node->left,  output);

    fprintf(output, " ");

    PrintInPostOrder(node->right, output);

    fprintf(output, " \"" tree_format "\" ) ", node->data);

    return;
}

void PrintInInOrder(Node* node, FILE* output)
{
    assert((output != nullptr) && "Pointer to output is NULL!!!\n");

    if (node == nullptr)
    {
        fprintf(output, "\"nil\"");
        return;
    }

    fprintf(output, "( ");

    PrintInInOrder(node->left,  output);

    fprintf(output, " \"" tree_format "\" ", node->data);

    PrintInInOrder(node->right, output);

    fprintf(output, ") ");

    return;
}