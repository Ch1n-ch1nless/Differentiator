#ifndef TREE_CONFIG_H_INCLUDED
#define TREE_CONFIG_H_INCLUDED

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../../CommonFiles/config_errors.h"

typedef void* tree_elem_t;

#define tree_format "%p"

const tree_elem_t TREE_POISON_VALUE = nullptr;

struct Node
{
    tree_elem_t data  = TREE_POISON_VALUE;
    Node*       left  = nullptr;
    Node*       right = nullptr;
};

struct Tree
{
    Node*       root     = nullptr;
    size_t      size     = 0;
    const char* name     = nullptr;
    const char* file     = nullptr;
    int         line     = 0;
};

#endif // TREE_CONFIG_H_INCLUDED
