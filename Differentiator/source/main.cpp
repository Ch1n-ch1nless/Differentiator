#include "differentiator.h"
#include "differentiator_input.h"
#include "differentiator_output.h"
#include "differentiator_derivative.h"
#include "differentiator_gnuplot.h"

#include "recursive_descent.h"

int main()
{
    error_t error = NO_ERR;

    /*Create trees, buffers and name tables*/
    Differentiator differentiator = {};
    error = DifferentiatorCtor(&differentiator);

    Differentiator derivative = {};
    error = DifferentiatorCtor(&derivative);

    /*Read buffer*/
    error = CreateBufferFromFile(&differentiator, "data_base.txt");
    /*Create Array of lexems from buffer*/
    error = TokenizeBuffer(&differentiator);
    
    /*Create Tree from array of lexems*/
    error = ReadTreeWithRecursiveDescent(&differentiator);

    /*File with tex*/
    FILE* tex_file  = nullptr;
    error = OpenFile("tree.tex",  &tex_file,  "w");

    /*Print libraries and other things*/
    ShowIntro(tex_file);

    /*Print expression*/
    PrintExpressionInTeX(tex_file, &differentiator, "Дано f(x) = ");

    /*Simplify expression*/
    DifferentiatorSimplify(tex_file, &differentiator);

    /*Draw graphic of expression*/
    MakeGraphic(tex_file, &differentiator);

    /*Take derivative of expression*/
    error = TakeXDerivative(tex_file, &differentiator, &derivative, "x");

    /*Simplify derivative*/
    DifferentiatorSimplify(tex_file, &derivative);

    /*Print Derivavtive to tex*/
    PrintExpressionInTeX(tex_file, &derivative, "В итоге производная f'(x) = ");

    /*Draw Graphic of derivavtive*/
    MakeGraphic(tex_file, &derivative);

    /*Delete structs*/
    DifferentiatorDtor(&differentiator);
    DifferentiatorDtor(&derivative);

    /*Print end of file and close the tex_file*/
    ShowOutro(tex_file);
    CloseFile(tex_file);

    return 0;
}