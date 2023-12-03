#include "differentiator_derivative.h"

#define TYPE    ((NodeData*)node->data)->type
#define VALUE   ((NodeData*)node->data)->value
#define LEFT    node->left
#define RIGHT   node->right

static Node* NewNode(Types type, Value value, Node* left, Node* right)
{
    error_t error = NO_ERR;

    NodeData* arg = (NodeData*) calloc(1, sizeof(NodeData));

    arg->type  = type;
    arg->value = value;

    Node* node = NodeCtor(&error, arg);
    LEFT  = left;
    RIGHT = right;

    return node;
}

static Node* CopyNode(Node* node)
{
    if (node == nullptr)
        return nullptr;

    Node* new_node = NewNode(TYPE, VALUE, CopyNode(LEFT), CopyNode(RIGHT));

    return new_node;
}

Node* DiffNode(FILE* tex_file, Differentiator* differentiator, Node* node, const int index)
{
    Node* new_node = nullptr;

    #define DEF_OPER(number, name, string, priority, diff_action)                       \
                case OPERATION_##name:                                                  \
                {                                                                       \
                    new_node = diff_action;                                             \
                    break;                                                              \
                }

    if (node == nullptr)
    {
        return nullptr;
    }

    switch (TYPE)
    {
        case TYPE_NUMBER:
            new_node = NUM(0);
            break;

        case TYPE_VARIABLE:
            new_node = NUM(VALUE.var_index == index);
            break;

        case TYPE_OPERATION:
        {    
            switch (VALUE.oper_index)
            {
                #include "../../DSL/operations.dsl"

                case OPERATION_UNDEFINED:
                default:
                    break;
            }
        }
    }

    DerivativeTextDump(tex_file, differentiator, node, new_node, FUNNY_MATH_QUOTES[rand() % NUMBER_OF_QUOTES]);

    return new_node;

    #undef DEF_OPER
}

static void CopyVariables(NameTable* new_name_table, NameTable* name_table)
{
    PTR_ASSERT(new_name_table)
    PTR_ASSERT(name_table)

    for (size_t i = 0; i < name_table->size; i++)
    {
        AddVariableToNameTable(new_name_table, name_table->array[i].name, name_table->array[i].value);
    }
}

error_t TakeXDerivative(FILE* tex_file, Differentiator* differentiator, Differentiator* new_differentiator, const char* name_of_variable)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(new_differentiator)

    error_t error = NO_ERR;

    CopyVariables(&(new_differentiator->name_table), &(differentiator->name_table));

    int index = FindVariableInNameTable(&(new_differentiator->name_table), name_of_variable, &error);

    new_differentiator->tree.root = DiffNode(tex_file, differentiator, differentiator->tree.root, index);

    return error;
}

