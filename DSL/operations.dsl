#include "DSL.h"

DEF_OPER(0,  PLUS,  "+",    1,  {S(L) + S(R)},      {
                                                        ADD(dL, dR)
                                                    })

DEF_OPER(1,  MINUS, "-",    1,  {S(L) + S(R)},      {
                                                        SUB(dL, dR)
                                                    })

DEF_OPER(2,  MUL,   "*",    2,  {S(L) + S(R)},      {
                                                        ADD(MUL(dL, cR), MUL(cL, dR))
                                                    })
DEF_OPER(3,  DIV,   "/",    2,  {M(S(L), S(R))},    {
                                                        DIV(SUB(MUL(cR, dL), MUL(cL, dR)), POW(cR, NUM(2)))
                                                    })
DEF_OPER(4,  POW,   "^",    3,  {S(L) + S(R) /2},   {
                                                        (isVariable(R)) 
                                                        ? 
                                                        (MUL(POW(cL, cR), ADD(MUL(dR, LN(cL)), DIV(MUL(cR, dL), cL))))
                                                        :
                                                        (MUL(MUL(cR, POW(cL, SUB(cR, NUM(1)))), dL))
                                                    })
DEF_OPER(5,  SIN,   "sin",  5,  {3 + S(L)},         {
                                                        MUL(COS(cL), dL)
                                                    })
DEF_OPER(6,  COS,   "cos",  5,  {3 + S(L)},         {
                                                        MUL(NUM(-1), MUL(SIN(cL), dL))
                                                    })
DEF_OPER(7,  TG,    "tg",   5,  {3 + S(L)},         {
                                                        DIV(dL, POW(COS(cL), NUM(2)))
                                                    })
DEF_OPER(8,  CTG,   "ctg",  5,  {3 + S(L)},         {
                                                        MUL(NUM(-1), DIV(dL, POW(SIN(cL), NUM(2))))
                                                    })
DEF_OPER(9,  LN,    "ln",   5,  {3 + S(L)},         {
                                                        DIV(dL, cL)
                                                    })
DEF_OPER(10, SQRT,  "sqrt", 5,  {3 + S(L)},         {
                                                        DIV(dL, MUL(NUM(2), SQRT(cL)))
                                                    })
DEF_OPER(11, LEFT,  "(",    6,  1,                  {
                                                        dL
                                                    })
DEF_OPER(12, RIGHT, ")",    6,  1,                  {
                                                        dR
                                                    })