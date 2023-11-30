#include "differentiator.h"
#include "differentiator_input.h"
#include "differentiator_output.h"
#include "differentiator_derivative.h"

int main()
{
    error_t error = NO_ERR;

    Differentiator differentiator = {};
    error = DifferentiatorCtor(&differentiator);

    Differentiator derivative = {};
    error = DifferentiatorCtor(&derivative);

    /*=== Reads tree ==*/

    error = CreateBufferFromFile(&differentiator, "data_base.txt");
    error = ReadTreeFromBuffer(&differentiator);

    /*=== Creates file.tex ===*/ 

    FILE* tex_file = nullptr;
    error = OpenFile("tree.tex", &tex_file, "w");
    if (error != NO_ERR)
        return error;

    /*=== Prints begin of document ===*/

    ShowIntro(tex_file);

    /*=== Prints math expression ===*/

    PrintExpressionInTeX(tex_file, &differentiator, "Наше выражение: \n");

    DifferentiatorSimplify(tex_file, &differentiator);

    /*=== Takes derivative ===*/

    TakeXDerivative(tex_file, &differentiator, &derivative, "x");

    /*=== Prints derivative of math expression ===*/

    PrintExpressionInTeX(tex_file, &derivative, "Производная выражения: \n");

    DifferentiatorSimplify(tex_file, &derivative);

    /*=== Creates Picture ===*/

    TreeGraphDump(&derivative);

    DifferentiatorDtor(&differentiator);
    DifferentiatorDtor(&derivative);

    /*=== Prints end of document ===*/

    ShowOutro(tex_file);

    return error;
}