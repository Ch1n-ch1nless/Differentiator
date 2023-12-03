#include "differentiator_input.h"

#define BUFFER   differentiator->buffer
#define BUF_SIZE differentiator->buf_size
#define TREE     differentiator->tree

//=================================================================================================

static error_t CountBufferSize(const char* const filename, size_t* const buf_size)
{
    PTR_ASSERT(filename)
    PTR_ASSERT(buf_size)

    struct stat st = {};

    stat(filename, &st);
    
    if (st.st_size == 0)
    {
        return EMPTY_FILE_ERR;
    }

    *buf_size = st.st_size;

    return NO_ERR;
}

//=================================================================================================

static error_t ReadBufferFromFile(FILE* const file_pointer, char** const buffer, const size_t buf_size)
{
    PTR_ASSERT(buffer)
    PTR_ASSERT(file_pointer)
    
    *buffer = (char*) calloc(buf_size + 1, sizeof(char));

    if (*buffer == nullptr)
    {
        return MEM_ALLOC_ERR;
    }

    size_t number_of_symbols = fread(*buffer, sizeof(char), buf_size, file_pointer);

    if (number_of_symbols != buf_size) 
    {
        if (feof(file_pointer)) 
        {
            printf("Error reading %s: unexpected end of file\n", "<STRING>");
            return FREAD_ERR;
        } 
        else if (ferror(file_pointer)) 
        {
            printf("Error reading %s", "<STRING>");
            return FREAD_ERR;
        }
        
        if (number_of_symbols > buf_size) 
        {
            printf("Error! Symbols in file  more then buf_size!");
            assert(false);
        }
    }

    return NO_ERR;
}

//=================================================================================================

error_t CreateBufferFromFile(Differentiator* const differentiator, const char* const input_file_name)
{
    PTR_ASSERT(differentiator)

    error_t error = NO_ERR;

    FILE* input = nullptr;

    error = OpenFile(input_file_name, &input, "rb");
    if (error == OPEN_FILE_ERR)
        return error;

    error |= CountBufferSize(input_file_name, &BUF_SIZE);

    error |= ReadBufferFromFile(input, &BUFFER, BUF_SIZE);

    error |= CloseFile(input);

    return error;
}

//=================================================================================================

error_t OpenFile(const char* const file_name, FILE** const file_ptr, const char* const mode)
{
    PTR_ASSERT(file_name)
    PTR_ASSERT(file_ptr)
    PTR_ASSERT(mode)

    *file_ptr = fopen(file_name, mode);

    if (*file_ptr == nullptr)
        return OPEN_FILE_ERR;

    return NO_ERR;
}

//=================================================================================================

error_t CloseFile(FILE* const file_ptr)
{
    PTR_ASSERT(file_ptr)

    if (fclose(file_ptr) != 0)
        return CLOSE_FILE_ERR;

    return NO_ERR;
}

//=================================================================================================

error_t SkipWhiteSpaces(char** const buffer)
{
    PTR_ASSERT( buffer)
    PTR_ASSERT(*buffer)

    size_t count_of_extra_char = 0;                 //Count of extra spaces, tabs before command
    char temp_array[MAX_LEN_OF_WORD] = {};          //Temporary array of symbols

    sscanf(*buffer, "%[ \t\n\r]%n", temp_array, &count_of_extra_char);
    
    *buffer = *buffer + count_of_extra_char;

    return NO_ERR;
}

//=================================================================================================

void ClearBuffer(void)
{
    int c = EOF;
    do
    {
        c = getchar();
    } 
    while (c != EOF && c != '\n');
}

//=================================================================================================

static Operations FindOperationInTable(char* string)
{
    PTR_ASSERT(string)

    for (size_t i = 0; i < END_OF_OPERATION_ENUM; i++)
    {
        if (strcmp(string, (OPERATION_TABLE[i]).string) == 0)
        {   
            return OPERATION_TABLE[i].oper;
        }
    }

    return OPERATION_UNDEFINED;
}

//=================================================================================================

static error_t IsArgumentNumber(char** cur_symbol, NodeData* arg)
{
    PTR_ASSERT( cur_symbol)
    PTR_ASSERT(*cur_symbol)
    PTR_ASSERT(arg)

    error_t error = NO_ERR;

    int    number_of_symbols = 0;
    double temp_value     = NAN;

    if (arg->type == TYPE_UNDEFINED)
    {
        if (sscanf(*cur_symbol, "%lg%n", &temp_value, &number_of_symbols) == 1)
        {
            arg->type            = TYPE_NUMBER;
            arg->value.num_value = temp_value;
            *cur_symbol          = *cur_symbol + number_of_symbols;
            return error;
        }

        return error | WRONG_BUFFER_SYNTAX_ERR;
    }

    return error;
}

//=================================================================================================

static error_t IsArgumentOperation(char** cur_symbol, NodeData* arg)
{
    PTR_ASSERT( cur_symbol)
    PTR_ASSERT(*cur_symbol)
    PTR_ASSERT(arg)

    error_t error = NO_ERR;

    int number_of_symbols             = 0;
    char temp_string[MAX_LEN_OF_WORD] = {};

    if (arg->type == TYPE_UNDEFINED)
    {
        if (sscanf(*cur_symbol, "%s%n", temp_string, &number_of_symbols) == 1)
        {
            Operations oper = FindOperationInTable(temp_string);

            if (oper != OPERATION_UNDEFINED)
            {
                arg->type             = TYPE_OPERATION;
                arg->value.oper_index = oper;
                *cur_symbol           = *cur_symbol + number_of_symbols;
            }

            return error;
        }

        return error | WRONG_BUFFER_SYNTAX_ERR;
    }

    return error;
}

//=================================================================================================

static error_t IsArgumentVariable(char** cur_symbol, NodeData* arg, NameTable* name_table)
{
    PTR_ASSERT( cur_symbol)
    PTR_ASSERT(*cur_symbol)
    PTR_ASSERT(arg)
    PTR_ASSERT(name_table)

    error_t error = NO_ERR;

    int number_of_symbols                  = 0;
    char* name_of_variable = (char*) calloc(MAX_LEN_OF_WORD, sizeof(char));

    if (arg->type == TYPE_UNDEFINED)
    {
        if (sscanf(*cur_symbol, "%s%n", name_of_variable, &number_of_symbols) == 1)
        {
            int index = FindVariableInNameTable(name_table, name_of_variable, &error);

            arg->type = TYPE_VARIABLE;
            *cur_symbol = *cur_symbol + number_of_symbols;

            if (index != VARIABLE_WAS_NOT_FOUND)
            {
                arg->value.var_index = index;
                return error;
            }

            index = name_table->size;
            error |= AddVariableToNameTable(name_table, name_of_variable, NAN);
            arg->value.var_index = index;

            return error;
        }

        return error | WRONG_BUFFER_SYNTAX_ERR;
    }

    return error;
}

//=================================================================================================

static NodeData* ReadArgument(char** const cur_symbol, error_t* const error, NameTable* name_table)
{
    PTR_ASSERT( cur_symbol)
    PTR_ASSERT(*cur_symbol)
    PTR_ASSERT(error)
    PTR_ASSERT(name_table)

    NodeData* arg = (NodeData*) calloc(1, sizeof(NodeData));
    if (arg == nullptr)
    {
        *error |= MEM_ALLOC_ERR;
        return nullptr;
    }

    arg->type            = TYPE_UNDEFINED;
    arg->value.num_value = NAN;

    *error |= IsArgumentNumber(cur_symbol, arg);
    *error |= IsArgumentOperation(cur_symbol, arg);
    *error |= IsArgumentVariable(cur_symbol, arg, name_table);

    return arg;
}

//=================================================================================================

error_t ReadTreeFromBuffer(Differentiator* const differentiator)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(BUFFER)

    error_t error = NO_ERR;

    Node* node = TREE.root;

    char* cur_symbol = BUFFER;

    Stack stk = {};
    error |= STACK_CTOR(&stk);
    
    while ((size_t)(cur_symbol - BUFFER) < BUF_SIZE)
    {
        error |= SkipWhiteSpaces(&cur_symbol);

        if (*cur_symbol == OPEN_NODE_BRACKET)
        {
            if (node->left == nullptr)
                {
                    error |= StackPush(&stk, ADD_LEFT_NODE);
                    node->left = NodeCtor(&error, nullptr);
                    node = node->left;
                }
                else
                {
                    if (node->right == nullptr)
                    {
                        error |= StackPush(&stk, ADD_RIGHT_NODE);
                        node->right = NodeCtor(&error, nullptr);
                        node = node->right;
                    }
                    else
                    {
                        error |= WRONG_BUFFER_SYNTAX_ERR;
                        return error;
                    }
                }
                cur_symbol++;
        }
        else if (*cur_symbol == CLOSE_NODE_BRACKET)
        {
            if (stk.size == 0 && ((size_t)(cur_symbol - BUFFER) != BUF_SIZE - 1))
            {
                error |= WRONG_BUFFER_SYNTAX_ERR;
                return error;
            }

                stk_elem_t ret_value = STK_POISON_VALUE;
                error |= StackPop(&stk, &ret_value);

                node = GetNodeFromStack(&stk, &TREE, &error);

                cur_symbol++;
        }
        else
        {
            node->data = ReadArgument(&cur_symbol, &error, &(differentiator->name_table));
        }
    }

    return error;
}

//=================================================================================================

#undef BUFFER
#undef BUF_SIZE
#undef TREE