#include "differentiator.h"
#include "differentiator_input.h"
#include "differentiator_output.h"
#include "differentiator_derivative.h"
#include "differentiator_gnuplot.h"

#include "recursive_descent.h"

int main()
{
    error_t error = NO_ERR;

    Differentiator differentiator = {};
    error = DifferentiatorCtor(&differentiator);

    error = CreateBufferFromFile(&differentiator, "data_base.txt");
    error = TokenizeBuffer(&differentiator);
    
    error = ReadTreeWithRecursiveDescent(&differentiator);

    FILE* html_file = nullptr;
    error = OpenFile("tree.html", &html_file, "w");

    DIF_HTML_DUMP(html_file, &differentiator);

    return 0;
}