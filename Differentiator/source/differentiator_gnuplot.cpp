#include "differentiator_gnuplot.h"

#define ARRAY differentiator->name_table.array
#define SIZE  differentiator->name_table.size
#define TREE  differentiator->tree
#define VALUE ((NodeData*)node->data)->value
#define TYPE  ((NodeData*)node->data)->type
#define LEFT  node->left
#define RIGHT node->right

static void PrintEquationToPlot(FILE* gp, Differentiator* differentiator, Node* node)
{
    PTR_ASSERT(gp)
    PTR_ASSERT(differentiator)
    PTR_ASSERT(node)

    int node_priority = GetPriority(node); 

    switch (TYPE)
    {
        case TYPE_NUMBER:
            fprintf(gp, "%lg", VALUE.num_value);
            break;

        case TYPE_VARIABLE:
        {
            int index = VALUE.var_index;
            fprintf(gp, "%s", ARRAY[index].name);
            break;
        }

        case TYPE_OPERATION:
        {
            Operations oper = (Operations) VALUE.oper_index;

            switch (oper)
            {
                case OPERATION_DIV:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, LEFT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, LEFT);
                    }

                    fprintf(gp, " / ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_POW:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, LEFT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, LEFT);
                    }

                    fprintf(gp, " ** ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_MUL:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, LEFT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, LEFT);
                    }

                    fprintf(gp, " * ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_PLUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, LEFT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, LEFT);
                    }

                    fprintf(gp, " + ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_MINUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, LEFT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, LEFT);
                    }

                    fprintf(gp, " - ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_COS:
                {
                    fprintf(gp, "cos(");
                    PrintEquationToPlot(gp, differentiator, LEFT);
                    fprintf(gp, ") ");
                    break;
                }

                case OPERATION_CTG:
                {
                    fprintf(gp, "1 / tg(");
                    PrintEquationToPlot(gp, differentiator, LEFT);
                    fprintf(gp, ") ");
                    break;
                }

                case OPERATION_LN:
                {
                    fprintf(gp, "log(");
                    PrintEquationToPlot(gp, differentiator, LEFT);
                    fprintf(gp, ") ");
                    break;
                }

                case OPERATION_SIN:
                {
                    fprintf(gp, "sin(");
                    PrintEquationToPlot(gp, differentiator, LEFT);
                    fprintf(gp, ") ");
                    break;
                }

                case OPERATION_SQRT:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(gp, "\\left(");
                        PrintEquationToPlot(gp, differentiator, LEFT);
                        fprintf(gp, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToPlot(gp, differentiator, LEFT);
                    }

                    fprintf(gp, " ** 0.5 ");
                    break;
                }

                case OPERATION_TG:
                {
                    fprintf(gp, "tg(");
                    PrintEquationToPlot(gp, differentiator, LEFT);
                    fprintf(gp, ") ");
                    break;
                }
                
                case OPERATION_UNDEFINED:
                default:
                    break;
            }
            break;
        }

        case TYPE_UNDEFINED:
        default:
            printf("ERROR!!! Undefined type of argument!!!\n");
            break;
    }
}

void MakeGraphic(FILE* pdf_file, Differentiator* differentiator)
{
    PTR_ASSERT(pdf_file)
    PTR_ASSERT(differentiator)

    FILE* gp = popen("C:\\\"Program Files\"\\gnuplot\\bin\\pgnuplot -persist", "w");
    if (gp == nullptr)
    {
        printf("ERROR! Program can not open file!!\n");
    }

    fprintf(gp, "set terminal win\n");
    fprintf(gp, "plot ");

    PrintEquationToPlot(gp, differentiator, differentiator->tree.root);

    fprintf(gp, "\n");

    fprintf(gp, "pause -1 \"hit enter to exit\"\n");
    fprintf(gp, "exit\n");
    pclose(gp);

}