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
    name_table->array    = (Variable*) calloc(name_table->capacity, sizeof(Variable));
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

    Variable* temp_pointer = nullptr;
    temp_pointer = (Variable*) realloc(name_table->array, name_table->capacity);
    if (temp_pointer == nullptr)
    {
        name_table->capacity /= 2;
        return error | MEM_ALLOC_ERR;
    }

    name_table->array = temp_pointer;

    return error;
}

//=======================================================================================

static error_t LexemArrayCtor(LexemArray* lexem_array)
{
    PTR_ASSERT(lexem_array)

    error_t error = NO_ERR;

    lexem_array->array = (Lexem*) calloc(LEXEM_ARRAY_CAPACITY_INIT, sizeof(Lexem));
    if (lexem_array->array == nullptr)
    {
        return error | MEM_ALLOC_ERR;
    }

    lexem_array->size     = 0;
    lexem_array->capacity = LEXEM_ARRAY_CAPACITY_INIT;

    return error;
}

//=======================================================================================

static error_t LexemArrayDtor(LexemArray* lexem_array)
{
    PTR_ASSERT(lexem_array)

    error_t error = NO_ERR;

    free(lexem_array->array);

    lexem_array->array    = nullptr;
    lexem_array->capacity = 0;
    lexem_array->size     = 0;
}

//=======================================================================================

static error_t LexemArrayResize(LexemArray* lexem_array)
{
    PTR_ASSERT(lexem_array)

    error_t error = NO_ERR;

    Lexem* temp_ptr = lexem_array->array;

    lexem_array->capacity *= 2;

    temp_ptr = (Lexem*) calloc(lexem_array->capacity, sizeof(Lexem));
    if (temp_ptr == nullptr)
    {
        lexem_array->capacity /= 2;
        return error | MEM_ALLOC_ERR;
    }

    for (int i = 0; i < lexem_array->size; i++)
    {
        temp_ptr[i].type = lexem_array->array[i].type;
        temp_ptr[i].value = lexem_array->array[i].value;
    }
    for (int i = lexem_array->size; i < lexem_array->capacity; i++)
    {
        temp_ptr[i].type  = TYPE_UNDEFINED;
        temp_ptr[i].value = {.num_value = NAN};
    }

    free(lexem_array->array);
    lexem_array->array = temp_ptr;

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

    error |= LexemArrayCtor(&(differentiator->lexem_array));

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

    error  = NameTableDtor(&(differentiator->name_table));

    error |= LexemArrayDtor(&(differentiator->lexem_array));

    return error;
}

//=======================================================================================

error_t DifferentiatorVerify(Differentiator* differentiator)
{
    PTR_ASSERT(differentiator)

    error_t error = NO_ERR;

    error = TreeVerify(&(differentiator->tree));

    if (differentiator->buf_size <= 0)
    {
        error |= BUF_SIZE_IS_FEWER_ZERO_ERR;
    }

    if (differentiator->buffer == nullptr)
    {
        error |= BUFFER_IS_NULL_ERR;
    }

    if (differentiator->name_table.array == nullptr)
    {
        error |= NULL_ARR_ERR;
    }

    return error;
}

//=======================================================================================

void PrintDifferentiatorErrors(Differentiator* differentiator, error_t error)
{
    PrintTreeErrors(&(differentiator->tree), error, __FILE__, __LINE__, __FUNCTION__);

    if (error & BUF_SIZE_IS_FEWER_ZERO_ERR)
    {
        printf("Error! Size of buffer is fewer NULL!!!\n");
    }

    if (error & BUFFER_IS_NULL_ERR)
    {
        printf("Error! Buffer is NULL!!!\n");
    }

    if (error & NULL_ARR_ERR)
    {
        printf("Error! Name table is NULL!!\n");
    }

}

//=======================================================================================

size_t FindVariableInNameTable(NameTable* name_table, const char* name, error_t* error)
{
    PTR_ASSERT(name_table)
    PTR_ASSERT(name)
    PTR_ASSERT(error)

    for (size_t i = 0; i < name_table->size; i++)
    {
        if (strcmp(name, name_table->array[i].name) == 0)
        {
            return i;
        }
    }

    return VARIABLE_WAS_NOT_FOUND;
}

//=======================================================================================

error_t AddVariableToNameTable(NameTable* name_table, const char* name_of_variable, double value_of_variable)
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

    name_table->array[name_table->size].name  = name_of_variable;
    name_table->array[name_table->size].value = value_of_variable;
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

    Lexem* arg = (Lexem*) node->data;

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

double DifferentiatorCalculate(Node* node, NameTable* name_table)
{
    if (node == nullptr)
        return 0;

    switch (((Lexem*)node->data)->type)
    {
        case TYPE_NUMBER:
            return ((Lexem*)node->data)->value.num_value;

        case TYPE_OPERATION:
        {
            double left  = DifferentiatorCalculate(node->left,  name_table);
            double right = DifferentiatorCalculate(node->right, name_table);

            switch (((Lexem*)node->data)->value.oper_index)
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
        {
            size_t index = ((Lexem*)node->data)->value.var_index;
            //return name_table->array[index].value;
            return 0;
        }
        case TYPE_UNDEFINED:
        default:
            return NAN;
    }
}

//=======================================================================================

error_t AddLexemToArray(Differentiator* differentiator, Lexem* new_node_data)
{
    PTR_ASSERT(differentiator)

    error_t error = NO_ERR;
    
    if (differentiator->lexem_array.size > differentiator->lexem_array.capacity)
    {
        return error | CAPACITY_FEWER_SIZE_ERR;
    }
    else if (differentiator->lexem_array.size == differentiator->lexem_array.capacity)
    {
        error = LexemArrayResize(&(differentiator->lexem_array));
        if (error != NO_ERR)
            return error;
    }

    int i = differentiator->lexem_array.size;

    differentiator->lexem_array.array[differentiator->lexem_array.size].type  = new_node_data->type;
    differentiator->lexem_array.array[differentiator->lexem_array.size].value = new_node_data->value;
    differentiator->lexem_array.size++;

    return error;
}

//=================================================================================================

int CalculateSizeOfSubtree(Node* node)
{
    if (node == nullptr)
        return 0;
    
    int size_of_subtree = 0;

    switch (((Lexem*)node->data)->type)
    {
        case TYPE_NUMBER:
        {
            size_of_subtree = 1;
            break;
        }

        case TYPE_VARIABLE:
        {
            size_of_subtree = 1;
            break;
        }

        case TYPE_OPERATION:
        {
            #define DEF_OPER(number, name, operator_in_file, priority, size, ...)       \
                case (OPERATION_##name):                                                \
                {                                                                       \
                    size_of_subtree =  size;                                            \
                    break;                                                              \
                }

            int oper_index = ((Lexem*)node->data)->value.oper_index;

            switch (oper_index)
            {
                #include "../../DSL/operations.dsl"
            }
        }

        case TYPE_UNDEFINED:
        default:
        {
            break;
        }
    }

    return size_of_subtree;
}

//=================================================================================================