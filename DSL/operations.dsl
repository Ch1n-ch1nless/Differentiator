DEF_OPER(0,  PLUS,  "+",    1,  {
                                    Node* left_node  = dL;
                                    Node* right_node = dR;
                                    return OPER(0, left_node, right_node);
                                })

DEF_OPER(1,  MINUS, "-",    1,  {
                                    Node* left_node  = dL;
                                    Node* right_node = dR;
                                    return OPER(1, left_node, right_node);
                                })
DEF_OPER(2,  MUL,   "*",    2,  {
                                    return OPER(0, OPER(2, dL, cR), OPER(2, dR, cL));
                                })
/*DEF_OPER(3,  DIV,   "/",    2, {OPER(DIV, OPER(MINUS, OPER(MUL, dL, cR), OPER(MUL, dR, cL)), OPER(POW, cR, NUM(2)))})
DEF_OPER(4,  POW,   "^",    3, {OPER(MUL, OPER(POW, cL, cR), OPER(PLUS, OPER(MUL, dR, OPER(LN, cL, nullptr)), OPER(DIV, OPER(MUL, cR, dL), cL)))})
DEF_OPER(5,  SIN,   "sin",  5, {OPER(MUL, dL, OPER(COS, cL, nullptr))})
DEF_OPER(6,  COS,   "cos",  5, {OPER(MUL, dL, OPER(MUL, NUM(-1), OPER(SIN, cL, nullptr)))})
DEF_OPER(7,  TG,    "tg",   5, {OPER(DIV, dL, OPER(POW, OPER(COS, cL, nullptr), NUM(2)))})
DEF_OPER(8,  CTG,   "ctg",  5, {OPER(DIV, dL, OPER(MUL, NUM(-1), OPER(POW, OPER(SIN, cL, nullptr), NUM(2))))})
DEF_OPER(9,  LN,    "ln",   5, {OPER(DIV, dL, cL)})
DEF_OPER(10, SQRT,  "sqrt", 5, {OPER(DIV, dL, OPER(MUL, NUM(2), OPER(SQRT, cL, nullptr)))})*/