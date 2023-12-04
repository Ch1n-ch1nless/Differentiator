#include "differentiator.h"
#include "differentiator_input.h"
#include "differentiator_output.h"
#include "differentiator_derivative.h"
#include "differentiator_gnuplot.h"

#include "rec_spusk.h"

int main()
{
    Buffer equation = {};
    scanf("%s", equation.buffer);

    Differentiator differentiator = {};
    DifferentiatorCtor(&differentiator);

    differentiator.tree.root = GetG(&equation);

    FILE* html_file = fopen("tree.html", "w");

    DIF_HTML_DUMP(html_file, &differentiator);

    MakeGraphic(html_file, &differentiator);

    fclose(html_file);

    return 0;
}