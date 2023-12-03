#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include "../../Tree/source/tree.h"
#include "../../Stack/source/stack.h"
#include "../../DSL/DSL.h"
#include <math.h>


/*============================================== STRUCTS =================================================*/

struct Variable
{
    const char* name  = nullptr;
    double      value = NAN;
};

struct NameTable
{
    Variable* array    = nullptr;
    size_t    size     = 0;
    size_t    capacity = 0;
};

struct Differentiator
{
    Tree        tree        = {};
    char*       buffer      = nullptr;
    size_t      buf_size    = 0;
    NameTable   name_table  = {};
};

enum Types : int
{
    TYPE_UNDEFINED  = -1,
    TYPE_NUMBER     =  0,
    TYPE_OPERATION  =  1,
    TYPE_VARIABLE   =  2,   
};

#define DEF_OPER(number, name, ...) OPERATION_##name = number,

enum Operations : int
{
    OPERATION_UNDEFINED = -1,
    #include "../../DSL/operations.dsl"

    END_OF_OPERATION_ENUM
};

#undef DEF_OPER

struct OperationInfo
{
    const char* string;
    Operations  oper;
};

#define DEF_OPER(number, name, operation_in_file, ...) {operation_in_file, OPERATION_##name},

const OperationInfo OPERATION_TABLE[END_OF_OPERATION_ENUM] = 
{
    #include "../../DSL/operations.dsl"
};

#undef DEF_OPER

union Value
{
    double num_value;
    size_t oper_index;
    size_t var_index;
};

struct NodeData
{
    Types  type  = TYPE_UNDEFINED;
    Value  value;
};

const size_t NAME_TABLE_CAPACITY_INIT = 16;
const int    VARIABLE_WAS_NOT_FOUND   = -1;

const double EPS = 1e-6;

/*======================================================================================================*/

bool    IsZero(double number);
bool    IsOne( double number);

error_t DifferentiatorCtor(Differentiator* differentiator);
error_t DifferentiatorDtor(Differentiator* differentiator);

size_t  FindVariableInNameTable(NameTable* name_table, const char* name_of_variable, error_t* error);
error_t AddVariableToNameTable( NameTable* name_table, const char* name_of_variable, double value_of_variable);

int     GetPriority(Node* node);

Node*   GetNodeFromStack(Stack* stk, Tree* tree, error_t* error);

double  DifferentiatorCalculate(Node* node, NameTable* name_table);

#endif //DIFFERENTIATOR_H_INCLUDED