#ifndef TAYLOR_SERIES_H_INCLUDED
#define TAYLOR_SERIES_H_INCLUDED

#include "differentiator_gnuplot.h"
#include "differentiator_derivative.h"
#include "differentiator_optimizate.h"

void ExpanseInSeriesTheilor(FILE* tex_file, Differentiator differentiator, Differentiator* theilor, int degree);

#endif // TAYLOR_SERIES_H_INCLUDED