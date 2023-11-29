#include "differentiator.h"

//=======================================================================================

static error_t NameTableCtor(NameTable* name_table)
{
    PTR_ASSERT(name_table)

    error_t error = NO_ERR;

    name_table->capacity = NAME_TABLE_CAPACITY_INIT;
    name_table->array    = (const char**) calloc(name_table->capacity, sizeof(char*));
    if (name_table->array == nullptr)
        error |= MEM_ALLOC_ERR;

    return error;
}

//=======================================================================================

static error_t NameTableDtor(NameTable* name_table)
{
    PTR_ASSERT(name_table)

    error_t error = NO_ERR;

    free(name_table->array);
    name_table->array    = nullptr;
    name_table->capacity = 0;
    name_table->size     = 0;

    return error;
}

//=======================================================================================

static error_t NameTableResize(NameTable* name_table)
{
    PTR_ASSERT(name_table)

    error_t error = NO_ERR;

    name_table->capacity *= 2;

    const char** temp_pointer = nullptr;
    temp_pointer = (const char**) realloc(name_table->array, name_table->capacity);
    if (temp_pointer == nullptr)
    {
        name_table->capacity /= 2;
        return error | MEM_ALLOC_ERR;
    }

    name_table->array = temp_pointer;

    return error;
}

//=======================================================================================

error_t DifferentiatorCtor(Differentiator* differentiator)
{
    PTR_ASSERT(differentiator)

    error_t error = NO_ERR;

    error = TREE_CTOR(&(differentiator->tree));
    if (error != NO_ERR)
        return error;

    differentiator->buffer   = nullptr;
    differentiator->buf_size = 0;

    error |= NameTableCtor(&(differentiator->name_table));

    return error;
}

//=======================================================================================

error_t DifferentiatorDtor(Differentiator* differentiator)
{
    PTR_ASSERT(differentiator)

    error_t error = NO_ERR;

    error = TreeDtor(&(differentiator->tree));
    if (error != NO_ERR)
        return error;

    free(differentiator->buffer);
    differentiator->buffer   = nullptr;
    differentiator->buf_size = 0;

    error = NameTableDtor(&(differentiator->name_table));

    return error;
}

//=======================================================================================

size_t FindVariableInNameTable(NameTable* name_table, const char* name, error_t* error)
{
    PTR_ASSERT(name_table)
    PTR_ASSERT(name)
    PTR_ASSERT(error)

    for (size_t i = 0; i < name_table->size; i++)
    {
        if (strcmp(name, name_table->array[i]) == 0)
        {
            return i;
        }
    }

    return VARIABLE_WAS_NOT_FOUND;
}

//=======================================================================================

error_t AddVariableToNameTable(NameTable* name_table, const char* name_of_variable)
{
    PTR_ASSERT(name_table)

    error_t error = NO_ERR;

    if (name_table->size > name_table->capacity)
    {
        return error | CAPACITY_FEWER_SIZE_ERR;
    }
    else if (name_table->size == name_table->capacity)
    {
        error = NameTableResize(name_table);
        if (error != NO_ERR)
            return error;
    }

    name_table->array[name_table->size] = name_of_variable;
    name_table->size += 1;

    return error;
}

//=======================================================================================

#define DEF_OPER(number, name, str, priority, ...)  \
    case OPERATION_##name:                          \
        return priority;

int GetPriority(Node* node)
{
    if (node == nullptr)
        return 4;

    NodeData* arg = (NodeData*) node->data;

    if (arg->type != TYPE_OPERATION)
    {
        return 4;
    }

    switch((int) arg->value.oper_index)
    {
        #include "../../DSL/operations.dsl"

        case OPERATION_UNDEFINED:
        default:
            assert(false && ("ERROR! Undefined operation!\n"));
            break;
    }
}

#undef DEF_OPER

//=======================================================================================

Node* GetNodeFromStack(Stack* stk, Tree* tree, error_t* error)
{
    PTR_ASSERT(stk)
    PTR_ASSERT(tree)
    PTR_ASSERT(error)

    Node* node = tree->root;

    for (size_t i = 0; i < (size_t) stk->size; i++)
    {
        switch(GetStkDataElemT(stk, i))
        {
            case ADD_LEFT_NODE:
                node = node->left;
                break;

            case ADD_RIGHT_NODE:
                node = node->right;
                break;

            default:
                *error |= UNKNOWN_INDICATOR_ERR;
                return nullptr;
        }
    }

    return node;
}

//=======================================================================================