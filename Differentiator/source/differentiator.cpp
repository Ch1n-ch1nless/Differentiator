#include "differentiator.h"

//=======================================================================================

bool IsZero(double number)
{
    if (fabs(number) < EPS)
        return true;

    return false;
}

bool IsOne( double number)
{
    if (fabs(number - 1.0) < EPS)
        return true;

    return false;
}

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

    for (size_t i = 0; i < name_table->size; i++)
    {
        free(name_table->array[i]);
    }

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

double DifferentiatorCalculate(Node* node)
{
    if (node == nullptr)
        return 0;

    switch (((NodeData*)node->data)->type)
    {
        case TYPE_NUMBER:
            return ((NodeData*)node->data)->value.num_value;

        case TYPE_OPERATION:
        {
            double left  = DifferentiatorCalculate(node->left );
            double right = DifferentiatorCalculate(node->right);

            switch (((NodeData*)node->data)->value.oper_index)
            {
                case OPERATION_PLUS:
                    return left + right;

                case OPERATION_MINUS:
                    return left - right;

                case OPERATION_MUL:
                    return left * right;

                case OPERATION_DIV:
                {
                    if (IsZero(right))
                    {
                        assert(false && ("ERROR! The denominators of fractions are zero!!!\n"));
                    }

                    return left / right;
                }

                case OPERATION_POW:
                    return pow(left, right);

                case OPERATION_SIN:
                    return sin(left);

                case OPERATION_COS:
                    return cos(left);

                case OPERATION_TG:
                    return tan(left);

                case OPERATION_CTG:
                    if (IsZero(left))
                    {
                        assert(false && ("ERROR! Ctg can not be calculated!!!\n"));
                    }
                    return 1 / tan(left);

                case OPERATION_LN:
                    return log(left);

                case OPERATION_SQRT:
                    return sqrt(left);
                
                default:
                    break;
            }
            break;
        }

        case TYPE_VARIABLE:
        case TYPE_UNDEFINED:
        default:
            return NAN;
    }
}

//=======================================================================================