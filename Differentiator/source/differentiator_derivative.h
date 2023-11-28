#ifndef DERIVATIVE_H_INCLUDED
#define DERIVATIVE_H_INCLUDED

#include "differentiator.h"

error_t TakeXDerivative(Differentiator* differentiator, Differentiator* new_differentiator);
Node*   DiffNode(Node* node);


#endif //DERIVATIVE_H_INCLUDED