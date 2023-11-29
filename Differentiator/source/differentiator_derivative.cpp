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
    PTR_ASSERT(node)

    Node* new_node = NewNode(TYPE, VALUE, LEFT, RIGHT);

    return new_node;
}

#define NUM(value)                  NewNode(TYPE_NUMBER,    {value}, nullptr, nullptr)
#define OPER(value, left, right)     NewNode(TYPE_OPERATION, {value}, left,    right  )

#define dL DiffNode(node->left, index)
#define dR DiffNode(node->right, index)
#define cL CopyNode(node->left)  
#define cR CopyNode(node->right) 

Node* DiffNode(Node* node, const int index)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    switch (TYPE)
    {
        case TYPE_NUMBER:
            return NUM(0);

        case TYPE_VARIABLE:
            if (VALUE.var_index == index)
            {
                return NUM(1);
            }
            return NUM(0);

        case TYPE_OPERATION:
        {    
            #define DEF_OPER(number, name, string, priority, diff_action, ...)  \
                case OPERATION_##name:                                          \
                {                                                               \
                    diff_action;                                         \
                }

            switch (VALUE.oper_index)
            {
                #include "../../DSL/operations.dsl"

                case OPERATION_UNDEFINED:
                default:
                    break;
            }
        }
    }
}

static void CopyVariables(NameTable* new_name_table, NameTable* name_table)
{
    PTR_ASSERT(new_name_table)
    PTR_ASSERT(name_table)

    for (size_t i = 0; i < name_table->size; i++)
    {
        AddVariableToNameTable(new_name_table, name_table->array[i]);
    }
}

error_t TakeXDerivative(Differentiator* differentiator, Differentiator* new_differentiator, const char* name_of_variable)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(new_differentiator)

    error_t error = NO_ERR;

    CopyVariables(&(new_differentiator->name_table), &(differentiator->name_table));

    int index = FindVariableInNameTable(&(new_differentiator->name_table), name_of_variable, &error);

    new_differentiator->tree.root = DiffNode(differentiator->tree.root, index);

    return error;
}

