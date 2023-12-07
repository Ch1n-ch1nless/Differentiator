#ifndef DIFFERENTIATOR_INPUT_H_INCLUDED
#define DIFFERENTIATOR_INPUT_H_INCLUDED

#include "differentiator.h"
#include <ctype.h>
#include <sys/stat.h>

const char   OPEN_NODE_BRACKET  = '(';
const char   CLOSE_NODE_BRACKET = ')';
const size_t MAX_LEN_OF_WORD    = 1 << 10;

error_t CreateBufferFromFile(Differentiator* const differentiator, const char* const input_file_name);

error_t TokenizeBuffer(Differentiator* const differentiator);

error_t OpenFile(const char* const file_name, FILE** const file_ptr, const char* const mode);
error_t CloseFile(FILE* const file_ptr);

error_t SkipWhiteSpaces(char** const buffer);
void    ClearBuffer(void);

#endif //DIFFERENTIATOR_INPUT_H_INCLUDED