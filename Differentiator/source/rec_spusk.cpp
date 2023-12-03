#include "rec_spusk.h"

#define sys_assert(condition) assert(condition && "Syntax Error!!!\n");

Node* GetG(Buffer* equation)
{
    Node* node = GetE(equation);
    sys_assert(equation->buffer[equation->pointer] == '\0');
    return node;
}

Node* GetE(Buffer* equation)
{
    Node* node      = nullptr;
    Node* left_node = GetT(equation);
    while ( equation->buffer[equation->pointer] == '+' || 
            equation->buffer[equation->pointer] == '-'      )
    {
        char oper = equation->buffer[equation->pointer];
        equation->pointer++;
        Node* right_node = GetT(equation);
        switch (oper)
        {
            case '+':
                node = NewNode(TYPE_OPERATION, {.oper_index = OPERATION_PLUS}, left_node, right_node);
                break;
            case '-':
                node = NewNode(TYPE_OPERATION, {.oper_index = OPERATION_MINUS}, left_node, right_node);
                break;
            default:
                sys_assert(false);
        }

        left_node = node;
    }

    return left_node;
}

Node* GetT(Buffer* equation)
{
    Node* node      = nullptr;
    Node* left_node = GetP(equation);
    while ( equation->buffer[equation->pointer] == '*' ||
            equation->buffer[equation->pointer] == '/'      )
    {
        char oper = equation->buffer[equation->pointer];
        equation->pointer++;
        Node* right_node = GetP(equation);
        switch(oper)
        {
            case '*':
                node = NewNode(TYPE_OPERATION, {.oper_index = OPERATION_MUL}, left_node, right_node);
                break;

            case '/':
                node = NewNode(TYPE_OPERATION, {.oper_index = OPERATION_DIV}, left_node, right_node);
                break;

            default:
                sys_assert(false);
        }

        left_node = node;
    }
    return left_node;
}

Node* GetP(Buffer* equation)
{
    Node* node = nullptr;
    if (equation->buffer[equation->pointer] == '(')
    {
        equation->pointer += 1;
        node = GetE(equation);
        sys_assert(equation->buffer[equation->pointer] == ')');
        equation->pointer += 1;
        return node;
    }

    return GetN(equation);
}

Node* GetN(Buffer* equation)
{
    Node*  node  = NewNode(TYPE_NUMBER, {.num_value = 0.5}, nullptr, nullptr);
    int    old_p = equation->pointer;
    size_t val   = 0;

    while ('0' <= equation->buffer[equation->pointer] && equation->buffer[equation->pointer] <= '9')
    {
        val = val * 10 + (equation->buffer[equation->pointer] - '0');
        equation->pointer += 1;
    }

    sys_assert(old_p != equation->pointer);
    ((NodeData*)node->data)->value.num_value = val + .0;

    return node;
}