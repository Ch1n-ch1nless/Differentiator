#ifndef DIFFERENTIATOR_GNUPLOT_H_INCLUDED
#define DIFFERENTIATOR_GNUPLOT_H_INCLUDED

#include "differentiator_output.h"
#include "differentiator_derivative.h"
#include <unistd.h>

void MakeGraphic(FILE* pdf_file, Differentiator* differentiator);

#endif //DIFERENTIATOR_GNUPLOT_H_INCLUDED