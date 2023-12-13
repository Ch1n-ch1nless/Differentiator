#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include "../../Tree/source/tree.h"
#include "../../Stack/source/stack.h"
#include "../../DSL/DSL.h"
#include <math.h>


/*============================================== STRUCTS =================================================*/

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

union Value
{
    double num_value;
    int    oper_index; //Operations 
    int    var_index;
};

struct Lexem
{
    Types  type  = TYPE_UNDEFINED;
    Value  value;
};

struct Variable
{
    const char* name  = nullptr;
    double      value = 0;
};

struct LexemArray
{   
    Lexem* array    = nullptr;
    size_t size     = 0;
    size_t capacity = 0;
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
    LexemArray  lexem_array = {};
    NameTable   name_table  = {};
};

#define DEF_OPER(number, name, operation_in_file, ...) {operation_in_file, OPERATION_##name},

const OperationInfo OPERATION_TABLE[END_OF_OPERATION_ENUM] = 
{
    #include "../../DSL/operations.dsl"
};

#undef DEF_OPER

const size_t NAME_TABLE_CAPACITY_INIT  = 16;
const size_t LEXEM_ARRAY_CAPACITY_INIT = 16;
const int    VARIABLE_WAS_NOT_FOUND    = -1;
const int    MAX_LEN_OF_EXPRESSION     = 1 << 12;

const double EPS = 1e-6;

/*======================================================================================================*/

bool    IsZero(double number);
bool    IsOne( double number);

error_t DifferentiatorCtor(Differentiator* differentiator);
error_t DifferentiatorDtor(Differentiator* differentiator);

error_t DifferentiatorVerify(Differentiator* differentiator);
void    PrintDifferentiatorErrors(Differentiator* differentiator, error_t error);

size_t  FindVariableInNameTable(NameTable* name_table, const char* name_of_variable, error_t* error);
error_t AddVariableToNameTable( NameTable* name_table, const char* name_of_variable, double value_of_variable);
error_t AddLexemToArray(Differentiator* differentiator, Lexem* new_node_data);

int     GetPriority(Node* node);

Node*   GetNodeFromStack(Stack* stk, Tree* tree, error_t* error);

double  DifferentiatorCalculate(Node* node, NameTable* name_table);

int     CalculateSizeOfSubtree(Node* node);

#endif //DIFFERENTIATOR_H_INCLUDED