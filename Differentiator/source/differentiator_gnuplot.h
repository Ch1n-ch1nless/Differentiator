#ifndef DIFFERENTIATOR_GNUPLOT_H_INCLUDED
#define DIFFERENTIATOR_GNUPLOT_H_INCLUDED

#include "differentiator_output.h"
#include "differentiator_derivative.h"
#include <unistd.h>

void MakeGraphic(FILE* pdf_file, Differentiator* differentiator);
void MakeGraphicWithTheilorSeries(FILE* pdf_file, Differentiator* differentiator, Differentiator* theilor);

#endif //DIFERENTIATOR_GNUPLOT_H_INCLUDED