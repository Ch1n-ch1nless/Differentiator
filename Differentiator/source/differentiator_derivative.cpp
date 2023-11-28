#include "differentiator_derivative.h"

#define TYPE    ((Argument*)node->data)->type
#define VALUE   ((Argument*)node->data)->value
#define LEFT    node->left
#define RIGHT   node->right

static Node* NewNode(Types type, double value, Node* left, Node* right)
{
    error_t error = NO_ERR;

    Argument* arg = (Argument*) calloc(1, sizeof(Argument));

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

#define NUM(value)                  NewNode(TYPE_NUMBER,    value, nullptr, nullptr)
#define OPER(value, left, right)    NewNode(TYPE_OPERATION, value, left,    right)

#define dL DiffNode(node->left)
#define dR DiffNode(node->right)
#define cL CopyNode(node->left)  
#define cR CopyNode(node->right) 

static bool FindX(Node* node)  //search index --
{
    if (node == nullptr)
        return false;

    if (TYPE == TYPE_VARIABLE)
        return true;

    return FindX(LEFT) || FindX(RIGHT);
}

static Node* DiffPow(Node* node)
{
    PTR_ASSERT(node)

    bool var_in_base    = FindX(node->left);
    bool var_in_degree  = FindX(node->right);

    if (!var_in_base && !var_in_degree)
    {
        return NUM(0);
    }
    else if (var_in_base && !var_in_degree)
    {
        return OPER(OPERATION_MUL, cR, OPER(OPERATION_POW, cL, OPER(OPERATION_MINUS, cR, NUM(1))));
    }
    else if (!var_in_base && var_in_degree)
    {
        return OPER(OPERATION_MUL, OPER(OPERATION_LN, NUM(0), cL), OPER(OPERATION_MUL, OPER(OPERATION_POW, cL, cR), dR));
    }
    else
    {
        printf("Sorry, but program can not derive from this term!!\n"); //assert()
        return nullptr;
    }
}

Node* DiffNode(Node* node)
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
            return NUM(1);

        case TYPE_OPERATION:
        {                  
            switch ((Operations)VALUE)
            {
                case OPERATION_PLUS:
                    return OPER(OPERATION_PLUS, dL, dR);

                case OPERATION_MINUS:
                    return OPER(OPERATION_MINUS, dL, dR);

                case OPERATION_MUL:
                    return OPER(OPERATION_PLUS, OPER(OPERATION_MUL, dL, cR), OPER(OPERATION_MUL, dR, cL));

                case OPERATION_DIV:
                    return OPER(OPERATION_DIV, OPER(OPERATION_MINUS, OPER(OPERATION_MUL, dL, cR), OPER(OPERATION_MUL, dR, cL)), 
                                               OPER(OPERATION_MUL, cR, cR));

                case OPERATION_POW:
                    return DiffPow(node);
                    break;

                case OPERATION_SIN:
                    return OPER(OPERATION_MUL, dR, OPER(OPERATION_COS, NUM(0), cR));

                case OPERATION_COS:
                    return OPER(OPERATION_MUL, dR, OPER(OPERATION_MINUS, NUM(0), OPER(OPERATION_SIN, NUM(0), cR)));

                case OPERATION_TG:
                    return OPER(OPERATION_DIV, dR, OPER(OPERATION_POW, OPER(OPERATION_COS, NUM(0), cR), NUM(2)));

                case OPERATION_CTG:
                    return OPER(OPERATION_DIV, OPER(OPERATION_MUL, NUM(-1), dR), OPER(OPERATION_POW, OPER(OPERATION_SIN, NUM(0), cR), NUM(2)));

                case OPERATION_LN:
                    return OPER(OPERATION_DIV, dR, cR);

                case OPERATION_SQRT:
                    return OPER(OPERATION_DIV, dR, OPER(OPERATION_MUL, NUM(0.5), OPER(OPERATION_SQRT, NUM(0), cR)));

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

error_t TakeXDerivative(Differentiator* differentiator, Differentiator* new_differentiator)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(new_differentiator)

    error_t error = NO_ERR;

    new_differentiator->tree.root = DiffNode(differentiator->tree.root);

    //new_differentiator->name_table = differentiator->name_table;
    CopyVariables(&(new_differentiator->name_table), &(differentiator->name_table));

    return error;
}

