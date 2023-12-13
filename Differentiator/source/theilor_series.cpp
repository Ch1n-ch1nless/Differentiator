#include "theilor_series.h"

static double Factorial(int number)
{
    double result = 1;
    for (int i = 1; i <= number; i++)
    {
        result *= i;
    }

    return result;
}

void ExpanseInSeriesTheilor(FILE* tex_file, Differentiator differentiator, Differentiator* theilor, int degree)
{
    PTR_ASSERT(tex_file)
    PTR_ASSERT(theilor)
    assert((degree > 0) && "ERROR!!! Degree <= 0!");

    double value = DifferentiatorCalculate(differentiator.tree.root, &(differentiator.name_table));

    theilor->tree.root = NewNode(TYPE_NUMBER, {.num_value = value}, nullptr, nullptr);

    CopyVariables(&(theilor->name_table), &(differentiator.name_table));

    for (int i = 1; i < degree; i++)
    {
        differentiator.tree.root = DiffNode(tex_file, &differentiator, differentiator.tree.root, 0);

        DifferentiatorSimplify(tex_file, &differentiator);

        value = DifferentiatorCalculate(differentiator.tree.root, &(differentiator.name_table));
        printf("%lg\n", value);

        theilor->tree.root = OPER(OPERATION_PLUS, MUL(DIV(NUM(value), NUM(Factorial(i))), 
                                                      POW(NewNode(TYPE_VARIABLE, {.var_index = 0}, nullptr, nullptr), NUM(i))), 
                                                  theilor->tree.root);

        DifferentiatorSimplify(tex_file, theilor);

        FILE* html_file = fopen("tree.html", "a");
        DifferentiatorHtmlDump(html_file, &differentiator, __FILE__, __LINE__, __FUNCTION__);
    }
}