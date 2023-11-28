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

    error = CreateBufferFromFile(&differentiator, "data_base.txt");
    error = ReadTreeFromBuffer(&differentiator);

    TreeGraphDump(&differentiator);
    
    TakeXDerivative(&differentiator, &derivative);

    FILE* tex_file = nullptr;
    error = OpenFile("tree.tex", &tex_file, "w");
    if (error != NO_ERR)
        return error;

    PrintExpressionInTeX(tex_file, &differentiator, &derivative);

    return error;
}