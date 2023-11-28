#include "getter_setter.h"
#include "stk_config.h"

canary_t GetStkDataIntro(const Stack* stk)
{
    assert(stk);

    canary_t* temp_ptr = (canary_t*) stk->data;
    return temp_ptr[0];
}

canary_t GetStkDataOutro(const Stack* stk)
{
    assert(stk);

    char* char_ptr = stk->data + sizeof(canary_t) + stk->capacity * sizeof(stk_elem_t);
    canary_t* canary_ptr = (canary_t*) char_ptr;
    return canary_ptr[0];
}

void SetStkDataIntro(Stack* stk, const canary_t intro_value)
{
    assert(stk);

    canary_t* temp_ptr = (canary_t*) stk->data;
    temp_ptr[0] = intro_value;
}

void SetStkDataOutro(Stack* stk, const canary_t outro_value)
{
    assert(stk);

    char* char_ptr = stk->data + sizeof(canary_t) + stk->capacity * sizeof(stk_elem_t);
    canary_t* canary_ptr = (canary_t*) char_ptr;
    canary_ptr[0] = outro_value;
}

stk_elem_t GetStkDataElemT(const Stack* stk, const size_t index)
{
    assert(stk);

    #if defined WITH_CANARY
        canary_t* temp_ptr1 = (canary_t*) stk->data;
        temp_ptr1++;
        stk_elem_t* temp_ptr = (stk_elem_t*) temp_ptr1;
        return temp_ptr[index];
    #else /*WITHOUT CANARY*/
        stk_elem_t* temp_ptr = (stk_elem_t*) stk->data;
        return temp_ptr[index];
    #endif
}

void SetStkDataElemT(Stack* stk, const size_t index, stk_elem_t new_value)
{
    assert(stk);

    #if defined WITH_CANARY
        canary_t* temp_ptr1 = (canary_t*) stk->data;
        temp_ptr1++;
        stk_elem_t* temp_ptr = (stk_elem_t*) temp_ptr1;
        temp_ptr[index] = new_value;
    #else /*WITHOUT CANARY*/
        *((stk_elem_t*)(stk->data + index)) = new_value;
    #endif
    return;
}
