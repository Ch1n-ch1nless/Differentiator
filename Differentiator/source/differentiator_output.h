#ifndef DIFFERENTIATOR_OUTPUT_H_INCLUDED
#define DIFFERENTIATOR_OUTPUT_H_INCLUDED

#include "differentiator.h"
#include "differentiator_input.h"
#include "../../Tree/source/tree_dump.h"

const char* const DOT_FILE      = "tree.dot";
const char* const PATH_TO_IMG  = "C:\\Users\\OlegA\\Desktop\\Differentiator\\Images\\";

const char BEGIN_MSG[] = R"(\documentclass [a4paper, 12x `pt]{article}
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

const int NUMBER_OF_QUOTES  = 16;
const int MAX_SIZE_OF_QUOTE = 300;

const char FUNNY_MATH_QUOTES[NUMBER_OF_QUOTES][MAX_SIZE_OF_QUOTE] = {
    {"\"АААаааАААА!!! n-ая Теорема Кантора! Вы не шокированы?? Вы точно поняли?\" - Дашков Е.В.\n"},
    {"Что это такое? А! Так это очевидно!!!\n"},
    {"Доказательство тривиально!!! Оставим читателю в качестве домашнего упражнения!\n"},
    {"Вспоминаем метод Алекса Эдуардовича Султанова!!!\n"},
    {"\"Мне вообще на эти все множители наплевать, делаем. Шлёп, шлёп, шлёп-шлёп!!!\" - Райгородский А.М.\n"},
    {"По миллионной теореме Коши для всех случаев в жизни!\n"},
    {"\"ДИРИРХЛЕЕЕЕ!!! ДИРИХЛЕЕЕ!!!\" - Савватеев А.В.\n"},
    {"Методом пристального взгляда заметим, что!\n"},
    {"Такую задачку решали ещё в советских яслях... Помню, в 1953 году решал её!\n"},
    {"Наносим 10 Сталинских ударов по этому выражению!!!\n"},
    {"Заметим, что ...\n"},
    {"Для решения этой задачи переместимся в n-мерное пр-во!!!\n"},
    {"Сейчас наступит катарсис!!!\n"},
    {"Вас ещё не кокнуло? Продолжаем!\n"},
    {"Ну даже ёжику понятно, как посчитать эту производную!!!\n"},
    {"Получим вот такое выражение! Мы упустили часть доказательств равносильных переходов! Поэтому я хочу, чтобы ВЫ САМИ ИХ ДОКАЗАЛИ!\n"}
};

const size_t MAX_SIZE_OF_COMMAND = 1 << 10;

void DifferentiatorTextDump(FILE* output, Differentiator* differentiator, Node* node, const char* word);

void DerivativeTextDump(FILE* output, Differentiator* differentiator, Node* node, Node* diff_node, const char* word);

void PrintExpressionInTeX(FILE* tex_file, Differentiator* differentiator, const char* word);

void TreeGraphDump(Differentiator* differentiator);

void ShowIntro(FILE* tex_file);

void ShowOutro(FILE* tex_file);

#define DIF_HTML_DUMP(html_file, differentiator) DifferentiatorHtmlDump(html_file, differentiator, __FILE__, __LINE__, \
                                                                                                   __FUNCTION__)
 
void DifferentiatorHtmlDump(FILE* html_file, Differentiator* differentiator, const char* file, 
                                                                             const int   line,
                                                                             const char* func );

#endif //DIFFERENTIATOR_OUTPUT_H_INCLUDED