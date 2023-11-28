#ifndef DIFFERENTIATOR_OUTPUT_H_INCLUDED
#define DIFFERENTIATOR_OUTPUT_H_INCLUDED

#include "differentiator.h"
#include "differentiator_input.h"

const char* const DOT_FILE      = "tree.dot";
const char* const PICTURE_FILE  = "tree.png";

const char BEGIN_MSG[] = R"(
    \documentclass [a4paper, 12x `pt]{article}
    \usepackage [utf8] {inputenc}
    \usepackage [T2A] {fontenc}
    \usepackage [russian] {babel}
    \usepackage {amsmath, amsfonts, amssymb, amsthm, mathtools, textcomp, stmaryrd}

    \title{Начало Математического безумия!}
    \author{Александров Олег}

    \begin{document}
    \maketitle

)";

const char FINAL_MSG[] = R"(

    \end{document}
)";

const size_t MAX_SIZE_OF_COMMAND = 1 << 10;

void PrintExpressionInTeX(FILE* tex_file, Differentiator* differentiator, Differentiator* derivative);

void TreeGraphDump(Differentiator* differentiator);

#endif //DIFFERENTIATOR_OUTPUT_H_INCLUDED