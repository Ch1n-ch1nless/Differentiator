#ifndef REC_SPUSK_H_INCLUDED
#define REC_SPUSK_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include "differentiator.h"
#include "differentiator_derivative.h"

const size_t MAX_SIZE_OF_BUFFER = 1 << 11;

struct Buffer
{
    char buffer[MAX_SIZE_OF_BUFFER] = {};
    int  pointer                    = 0;
};

Node* GetG(Buffer* equation);
Node* GetE(Buffer* equation);
Node* GetT(Buffer* equation);
Node* GetP(Buffer* equation);
Node* GetN(Buffer* equation);

#endif //REC_SPUSK_H_INCLUDED