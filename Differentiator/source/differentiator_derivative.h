#ifndef DERIVATIVE_H_INCLUDED
#define DERIVATIVE_H_INCLUDED

#include "differentiator.h"
#include "differentiator_optimizate.h"
#include "differentiator_output.h"

error_t TakeXDerivative(FILE* tex_file, Differentiator* differentiator, Differentiator* new_differentiator, const char* name_of_variable);
Node*   DiffNode(FILE* tex_file, Differentiator* differentiator, Node* node, const int index);
Node*   NewNode(Types type, Value value, Node* left, Node* right);

#endif //DERIVATIVE_H_INCLUDED