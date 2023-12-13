#ifndef DERIVATIVE_H_INCLUDED
#define DERIVATIVE_H_INCLUDED

#include "differentiator.h"
#include "differentiator_optimizate.h"
#include "differentiator_output.h"

void    CopyVariables(NameTable* new_name_table, NameTable* name_table);
error_t TakeXDerivative(FILE* tex_file, Differentiator* differentiator, Differentiator* new_differentiator, const char* name_of_variable);
Node*   DiffNode(FILE* tex_file, Differentiator* differentiator, Node* node, const int index);
Node*   NewNode(Types type, Value value, Node* left, Node* right);
bool    IsVariableInSubTree(Node* const begin_of_subtree, const char* variable, Differentiator* differentiator);
Node*   DiffPow(Node* node, Differentiator* differentiator, const char* variable, FILE* tex_file);

#endif //DERIVATIVE_H_INCLUDED