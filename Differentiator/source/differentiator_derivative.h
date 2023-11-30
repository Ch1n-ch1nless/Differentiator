#ifndef DERIVATIVE_H_INCLUDED
#define DERIVATIVE_H_INCLUDED

#include "differentiator.h"
#include "differentiator_optimizate.h"

error_t TakeXDerivative(Differentiator* differentiator, Differentiator* new_differentiator, const char* name_of_variable);
Node*   DiffNode(Node* node, const int index);


#endif //DERIVATIVE_H_INCLUDED