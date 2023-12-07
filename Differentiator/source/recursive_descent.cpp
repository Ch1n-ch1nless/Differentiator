#include "recursive_descent.h"

#define sys_assert(condition) assert(condition && "Syntax Error!!!\n");

static Node* GetGeneral(            Differentiator* differentiator, int* index);
static Node* GetExpression(         Differentiator* differentiator, int* index);
static Node* GetTerm(               Differentiator* differentiator, int* index);
static Node* GetPow(                Differentiator* differentiator, int* index);
static Node* GetUnaryOperator(      Differentiator* differentiator, int* index);
static Node* GetPrimaryExpression(  Differentiator* differentiator, int* index);
static Node* GetNumber(             Differentiator* differentiator, int* index);


static Node* GetNumber(Differentiator* differentiator, int* index)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(index)

    Node* node = nullptr;

    if (differentiator->lexem_array.array[*index].type == TYPE_NUMBER)
    {
        node = NewNode(TYPE_NUMBER,   differentiator->lexem_array.array[*index].value, nullptr, nullptr);
    }
    else
    {
        node = NewNode(TYPE_VARIABLE, differentiator->lexem_array.array[*index].value, nullptr, nullptr);
    }

    (*index)++;

    return node;
}

static Node* GetPrimaryExpression(Differentiator* differentiator, int* index)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(index)

    Node* node = nullptr;

    if ( *index < differentiator->lexem_array.size                                    &&
         differentiator->lexem_array.array[*index].type             == TYPE_OPERATION &&
         differentiator->lexem_array.array[*index].value.oper_index == OPERATION_LEFT   )
    {
        (*index)++;

        node = GetExpression(differentiator, index);

        (*index)++;

        return node;
    }

    return GetNumber(differentiator, index);
}

static Node* GetUnaryOperator(Differentiator* differentiator, int* index)
{
    Node* node = nullptr;

    if ( *index < differentiator->lexem_array.size                                    &&
         differentiator->lexem_array.array[*index].type             == TYPE_OPERATION && 
        (differentiator->lexem_array.array[*index].value.oper_index == OPERATION_SIN  ||
         differentiator->lexem_array.array[*index].value.oper_index == OPERATION_COS  ||
         differentiator->lexem_array.array[*index].value.oper_index == OPERATION_TG   ||
         differentiator->lexem_array.array[*index].value.oper_index == OPERATION_CTG  ||
         differentiator->lexem_array.array[*index].value.oper_index == OPERATION_LN   || 
         differentiator->lexem_array.array[*index].value.oper_index == OPERATION_SQRT    ))
    {
        int old_index = *index;

        (*index)++;
         
        node = GetPrimaryExpression(differentiator, index);
        
        node = NewNode(TYPE_OPERATION, differentiator->lexem_array.array[old_index].value, node, nullptr);

        return node;
    }

    return GetPrimaryExpression(differentiator, index);
    
}

static Node* GetPow(Differentiator* differentiator, int* index)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(index)

    Node* node      = nullptr;
    Node* left_node = GetUnaryOperator(differentiator, index);
    while ( *index < differentiator->lexem_array.size                                    &&
            differentiator->lexem_array.array[*index].type             == TYPE_OPERATION && 
            differentiator->lexem_array.array[*index].value.oper_index == OPERATION_POW     )
    {
        int old_index = *index;

        (*index)++;

        Node* right_node = GetUnaryOperator(differentiator, index);
        
        node = NewNode(TYPE_OPERATION, differentiator->lexem_array.array[old_index].value, left_node, right_node);

        left_node = node;
    }
    
    return left_node;
}

static Node* GetTerm(Differentiator* differentiator, int* index)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(index)

    Node* node      = nullptr;
    Node* left_node = GetPow(differentiator, index);

    while (  *index < differentiator->lexem_array.size                                    &&
             differentiator->lexem_array.array[*index].type             == TYPE_OPERATION && 
            (differentiator->lexem_array.array[*index].value.oper_index == OPERATION_DIV  ||
             differentiator->lexem_array.array[*index].value.oper_index == OPERATION_MUL    ))
    {
        int old_index = *index;

        (*index)++;

        Node* right_node = GetPow(differentiator, index);
        
        node = NewNode(TYPE_OPERATION, differentiator->lexem_array.array[old_index].value, left_node, right_node);

        left_node = node;
    }

    return left_node;
}

static Node* GetExpression(Differentiator* differentiator, int* index)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(index)

    Node* node      = nullptr;
    Node* left_node = GetTerm(differentiator, index);
    while (  *index < differentiator->lexem_array.size                                     && 
             differentiator->lexem_array.array[*index].type             == TYPE_OPERATION  && 
            (differentiator->lexem_array.array[*index].value.oper_index == OPERATION_PLUS  ||
             differentiator->lexem_array.array[*index].value.oper_index == OPERATION_MINUS   ))
    {
        int old_index = *index;

        (*index)++;

        Node* right_node = GetTerm(differentiator, index);
        
        node = NewNode(TYPE_OPERATION, differentiator->lexem_array.array[old_index].value, left_node, right_node);

        left_node = node;
    }

    return left_node;
}

static Node* GetGeneral(Differentiator* differentiator, int* index)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(index)

    Node* node = GetExpression(differentiator, index);

    return node;
}

error_t ReadTreeWithRecursiveDescent(Differentiator* differentiator)
{
    PTR_ASSERT(differentiator)

    error_t error = NO_ERR;

    int index     = 0;

    differentiator->tree.root = GetGeneral(differentiator, &index);

    return error;
}