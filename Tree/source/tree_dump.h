#ifndef TREE_DUMP_H_INCLUDED
#define TREE_DUMP_H_INCLUDED

#include "tree.h"

#define TREE_TXT_DUMP(tree, output, mode) \
        TreeTextDump(tree, output, __FILE__, __LINE__, __FUNCTION__, mode);

const char* const TXT_DUMP_FILE = "tree.dmp";

enum Dump_mode : unsigned int
{
    PRE_ORDER   = 0,
    POST_ORDER  = 1,
    IN_ORDER    = 2,
};

void TreeTextDump(Tree* tree, FILE* output, const char* file, const int  line,
                                            const char* func, Dump_mode  mode );

void PrintInPreOrder(Node* node, FILE* output);

void PrintInPostOrder(Node* node, FILE* output);

void PrintInInOrder(Node* node, FILE* output);

#endif // TREE_DUMP_H_INCLUDED
