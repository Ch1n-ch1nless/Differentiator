#include "DSL.h"

DEF_OPER(0,  PLUS,  "+",    1,  {
                                    ADD(dL, dR)
                                })

DEF_OPER(1,  MINUS, "-",    1,  {
                                    SUB(dL, dR)
                                })

DEF_OPER(2,  MUL,   "*",    2,  {
                                    ADD(MUL(dL, cR), MUL(cL, dR))
                                })
DEF_OPER(3,  DIV,   "/",    2,  {
                                    DIV(SUB(MUL(dL, cR), MUL(cL, dR)), POW(cR, NUM(2)))
                                })
DEF_OPER(4,  POW,   "^",    3,  {
                                    MUL(POW(cL, cR), ADD(MUL(dR, LN(cL)), DIV(MUL(cR, dL), cL)))
                                })
DEF_OPER(5,  SIN,   "sin",  5,  {
                                    MUL(dL, COS(cL))
                                })
DEF_OPER(6,  COS,   "cos",  5,  {
                                    MUL(NUM(-1), MUL(dL, SIN(dL)))
                                })
DEF_OPER(7,  TG,    "tg",   5,  {
                                    DIV(dL, POW(COS(cL), NUM(2)))
                                })
DEF_OPER(8,  CTG,   "ctg",  5,  {
                                    MUL(NUM(-1), DIV(dL, POW(SIN(cL), NUM(2))))
                                })
DEF_OPER(9,  LN,    "ln",   5,  {
                                    DIV(dL, cL)
                                })
DEF_OPER(10, SQRT,  "sqrt", 5,  {
                                    DIV(dL, MUL(NUM(2), SQRT(cL)))
                                })