#ifndef CONFIG_ERRORS_H_INCLUDED
#define CONFIG_ERRORS_H_INCLUDED

#include <assert.h>

#define PTR_ASSERT(pointer)     assert((pointer != nullptr) && ("ERROR! Pointer to " #pointer "is NULL!!!\n"));

typedef unsigned long long error_t;

enum Errors : error_t
{
    NO_ERR                              = 0,       //!< constant which shows that no errors
    NULL_STK_ERR                        = 1 << 0,  //!< constant which shows that pointer to stack is null
    NULL_ARR_ERR                        = 1 << 1,  //!< constant which shows that pointer to array is null
    MINUS_CAPACITY_ERR                  = 1 << 2,  //!< constant which shows that capacity of array is fewer 0
    MINUS_SIZE_ERR                      = 1 << 3,  //!< constant which shows that size of array is fewer 0
    CAPACITY_FEWER_SIZE_ERR             = 1 << 4,  //!< constant which shows that capacity of array is fewer size
    OPEN_FILE_ERR                       = 1 << 5,  //!< constant which shows that program can not open file
    MEM_ALLOC_ERR                       = 1 << 6,  //!< constant which shows that program can not allocate memory
    NULL_NAME_ERR                       = 1 << 7,  //!< constant which shows that pointer to name of stack is null
    NULL_FILE_ERR                       = 1 << 8,  //!< constant which shows that pointer to file of stack is null
    MINUS_LINE_ERR                      = 1 << 9,  //!< constant which shows that value of stack's line is fewer 0
    RIGHT_CANARY_DIED                   = 1 << 10, //!< constant which shows that right canary of stack was changed
    LEFT_CANARY_DIED                    = 1 << 11, //!< constant which shows that left canary of stack was changed
    WRONG_INTRO_ERR                     = 1 << 12, //!< constant which shows that left canary of array was changed
    WRONG_OUTRO_ERR                     = 1 << 13, //!< constant which shows that right canary of array was changed
    WRONG_HASH                          = 1 << 14, //!< constant which shows that hash is wrong
    STK_KILL_ERR                        = 1 << 15, //!< constant which shows that status of stack is KILLED_STACK
    WRONG_STK_STATUS_ERR                = 1 << 16, //!< constant which shows that status of stack was changed to strange value
    ADD_ARG_ERR                         = 1 << 17,
    ROOT_IS_NULL_ERR                    = 1 << 18,
    TREE_NAME_IS_NULL_ERR               = 1 << 19,
    TREE_FILE_IS_NULL_ERR               = 1 << 20,
    TREE_LINE_IS_FEWER_ZERO_ERR         = 1 << 21,
    CLOSE_FILE_ERR                      = 1 << 22,
    LEN_OF_WORD_IS_BIGGER_MAX_SIZE_ERR  = 1 << 23,
    UNKNOWN_INDICATOR_ERR               = 1 << 24,
    WRONG_BUFFER_SYNTAX_ERR             = 1 << 25,
    INPUT_ERR                           = 1 << 26,
    EMPTY_FILE_ERR                      = 1 << 27,
    FREAD_ERR                           = 1 << 28,
    BUFFER_IS_NULL_ERR                  = 1 << 29,
    BUF_SIZE_IS_FEWER_ZERO_ERR          = 1 << 30,
};

#endif //CONFIG_ERRORS_H_INCLUDED
