#include "differentiator_output.h"

#define ARRAY differentiator->name_table.array
#define SIZE  differentiator->name_table.size
#define TREE  differentiator->tree
#define VALUE ((Argument*)node->data)->value
#define TYPE  ((Argument*)node->data)->type
#define LEFT  node->left
#define RIGHT node->right

//=================================================================================================

static void PrintNodeInTex(FILE* output, Differentiator* differentiator, Node* node)
{
    PTR_ASSERT(output)
    PTR_ASSERT(differentiator)
    PTR_ASSERT(node)  

    int node_priority = GetPriority(node); 

    switch (TYPE)
    {
        case TYPE_NUMBER:
            fprintf(output, "%d", VALUE);
            break;

        case TYPE_VARIABLE:
        {
            int index = VALUE;
            fprintf(output, "%s", ARRAY[index]);
            break;
        }

        case TYPE_OPERATION:
        {
            // dangerous
            // 0x5 != 5.0
            Operations oper = (Operations) VALUE;

            switch (oper)
            {
                case OPERATION_DIV:
                {
                    fprintf(output, " \\frac{");
                    PrintNodeInTex(output, differentiator, LEFT );
                    fprintf(output, "}{");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, "} ");
                    break;
                }

                case OPERATION_POW:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTex(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTex(output, differentiator, LEFT);
                    }

                    fprintf(output, "^{");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, "} ");
                    break;
                }

                case OPERATION_MUL:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTex(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTex(output, differentiator, LEFT);
                    }

                    fprintf(output, " * ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTex(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTex(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_PLUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTex(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTex(output, differentiator, LEFT);
                    }

                    fprintf(output, " + ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTex(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTex(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_MINUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTex(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTex(output, differentiator, LEFT);
                    }

                    fprintf(output, " - ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTex(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTex(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_COS:
                {
                    fprintf(output, "cos(");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_CTG:
                {
                    fprintf(output, "ctg(");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_LN:
                {
                    fprintf(output, "\\ln(");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_SIN:
                {
                    fprintf(output, "sin(");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_SQRT:
                {
                    fprintf(output, "\\sqrt{");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, "} ");
                    break;
                }

                case OPERATION_TG:
                {
                    fprintf(output, "tg(");
                    PrintNodeInTex(output, differentiator, RIGHT);
                    fprintf(output, ") ");
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

//=================================================================================================

static void DifferentiatorTextDump(FILE* output, Differentiator* differentiator)
{
    PTR_ASSERT(output)
    PTR_ASSERT(differentiator)

    fprintf(output, "$ ");

    PrintNodeInTex(output, differentiator, TREE.root);

    fprintf(output, " $\n");
}

//=================================================================================================

static void ShowIntro(FILE* tex_file)
{
    fprintf(tex_file, BEGIN_MSG);
}

//=================================================================================================

static void ShowOutro(FILE* tex_file)
{
    fprintf(tex_file, FINAL_MSG);
}

//=================================================================================================

void PrintExpressionInTeX(FILE* tex_file, Differentiator* differentiator, Differentiator* derivative)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(tex_file)

    ShowIntro(tex_file);

    fprintf(tex_file, "Исходная функция f(x) = ");

    DifferentiatorTextDump(tex_file, differentiator);

    fprintf(tex_file, "Производная f'(x) = ");

    DifferentiatorTextDump(tex_file, derivative);

    ShowOutro(tex_file);
}

//=================================================================================================

static void NodeGraphDump(Node* node, FILE* dot_file, Differentiator* differentiator)
{
    if (node == nullptr)
        return;

    fprintf(dot_file, "node_%p[label = \"{", node);

    switch (TYPE)
    {
        case TYPE_NUMBER:
        {
            fprintf(dot_file, "%d | NUM | {<left> left | <right> right}}\", fillcolor = \"blue\"];\n", VALUE);
            break;
        }
        case TYPE_VARIABLE:
        {
            fprintf(dot_file, "%s | VAR | {<left> left | <right> right}}\", fillcolor = \"green\"];\n", ARRAY[(int)VALUE]);
            break;
        }
        case TYPE_OPERATION:
        {
            fprintf(dot_file, "%s | OPER | {<left> left | <right> right}}\", fillcolor = \"red\"];\n", (OPERATION_TABLE[VALUE]).string);
            break;
        }
        case TYPE_UNDEFINED:
        default:
            break;
    }

    NodeGraphDump(node->left,  dot_file, differentiator);

    NodeGraphDump(node->right, dot_file, differentiator);
}

//=================================================================================================

static void EdgeGraphDump(Node* node, FILE* dot_file)
{
    if (node->left != nullptr)
    {
        fprintf(dot_file, "node_%p:<left> -> node_%p;\n",
                           node, node->left);

        EdgeGraphDump(node->left, dot_file);
    }

    if (node->right != nullptr)
    {
        fprintf(dot_file, "node_%p:<right> -> node_%p;\n",
                           node, node->right);

        EdgeGraphDump(node->right, dot_file);
    }
}

//=================================================================================================

void TreeGraphDump(Differentiator* differentiator)
{
    PTR_ASSERT(differentiator)

    error_t error = NO_ERR;

    FILE* dot_file = fopen(DOT_FILE, "w");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    fprintf(dot_file, "digraph G\n"
                      "{\n"
                      "\tgraph [dpi = 100];\n\n"
                      "\trankdir = TB;\n\n"
                      "\tedge[minlen = 3, arrowsize = 2, penwidth = 1.5];\n"
                      "\tnode[shape = Mrecord, style = \"rounded, filled\", "
                      "fillcolor = \"yellow\", fontsize = 20, "
                      "penwidth = 3];\n\n");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    NodeGraphDump(differentiator->tree.root, dot_file, differentiator);
    fprintf(dot_file, "\n");

    EdgeGraphDump(differentiator->tree.root, dot_file);
    fprintf(dot_file, "\n");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    fprintf(dot_file, "}\n");

    fclose(dot_file);

    char command[MAX_SIZE_OF_COMMAND] = {};
    sprintf(command, "dot %s -T png -o %s", DOT_FILE, PICTURE_FILE);
    system(command);
    //Do not check system!!! I need to fix it!!! >:(
}

//=================================================================================================
