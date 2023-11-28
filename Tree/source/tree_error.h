#ifndef TREE_ERROR_H_INCLUDED
#define TREE_ERROR_H_INCLUDED

#include "tree_config.h"

#define CHECK_TREE_ERROR(tree, error)                                       \
        error |= TreeVerify(tree);                                          \
        if (error != NO_ERR)                                                \
        {                                                                   \
            PrintTreeErrors(tree, error, __FILE__, __LINE__, __FUNCTION__); \
        }

error_t TreeVerify(Tree* tree);

void    PrintTreeErrors(Tree* tree, error_t error, const char* file,
                                                   const int   line,
                                                   const char* function);

#endif // TREE_ERROR_H_INCLUDED
