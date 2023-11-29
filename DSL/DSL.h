#ifndef DSL_H_INCLUDED
#define DSL_H_INCLUDED

#define NUM(value)                  NewNode(TYPE_NUMBER,    {.num_value  = value}, nullptr, nullptr)
#define OPER(value, left, right)    NewNode(TYPE_OPERATION, {.oper_index = value}, left,    right )

#define dL DiffNode(node->left,  index)
#define dR DiffNode(node->right, index)
#define cL CopyNode(node->left)  
#define cR CopyNode(node->right) 

#define ADD(left, right)    OPER(OPERATION_PLUS,  left, right)
#define SUB(left, right)    OPER(OPERATION_MINUS, left, right)
#define MUL(left, right)    OPER(OPERATION_MUL,   left, right)
#define DIV(left, right)    OPER(OPERATION_DIV,   left, right)
#define POW(left, right)    OPER(OPERATION_POW,   left, right)

#define SIN(left)    OPER(OPERATION_SIN,   left, nullptr)
#define COS(left)    OPER(OPERATION_COS,   left, nullptr)
#define TAN(left)    OPER(OPERATION_TG,    left, nullptr)
#define CTAN(left)   OPER(OPERATION_CTG,   left, nullptr)
#define LN(left)     OPER(OPERATION_LN,    left, nullptr)
#define SQRT(left)   OPER(OPERATION_SQRT,  left, nullptr)

#endif //DSL_H_INCLUDED