#include "differentiator_output.h"

#define ARRAY differentiator->name_table.array
#define SIZE  differentiator->name_table.size
#define TREE  differentiator->tree
#define VALUE ((Lexem*)node->data)->value
#define TYPE  ((Lexem*)node->data)->type
#define LEFT  node->left
#define RIGHT node->right

//=================================================================================================

static void PrintNodeInTeX(FILE* output, Differentiator* differentiator, Node* node)
{
    PTR_ASSERT(output)
    PTR_ASSERT(differentiator)
    PTR_ASSERT(node)

    int node_priority = GetPriority(node); 

    switch (TYPE)
    {
        case TYPE_NUMBER:
            fprintf(output, "%lg", VALUE.num_value);
            break;

        case TYPE_VARIABLE:
        {
            int index = VALUE.var_index;
            if (node->left != nullptr)
            {
                fprintf(output, "%s", REPLACE_ARRAY[index]);
            }   
            else
            {
                fprintf(output, "%s", ARRAY[index]);
            }
            break;
        }

        case TYPE_OPERATION:
        {
            Operations oper = (Operations) VALUE.oper_index;

            switch (oper)
            {
                case OPERATION_DIV:
                {
                    fprintf(output, " \\frac{");
                    PrintNodeInTeX(output, differentiator, LEFT );
                    fprintf(output, "}{");
                    PrintNodeInTeX(output, differentiator, RIGHT);
                    fprintf(output, "} ");
                    break;
                }

                case OPERATION_POW:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTeX(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTeX(output, differentiator, LEFT);
                    }

                    fprintf(output, "^{");
                    PrintNodeInTeX(output, differentiator, RIGHT);
                    fprintf(output, "} ");
                    break;
                }

                case OPERATION_MUL:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTeX(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTeX(output, differentiator, LEFT);
                    }

                    fprintf(output, " \\cdot ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTeX(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTeX(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_PLUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTeX(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTeX(output, differentiator, LEFT);
                    }

                    fprintf(output, " + ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTeX(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTeX(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_MINUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTeX(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTeX(output, differentiator, LEFT);
                    }

                    fprintf(output, " - ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintNodeInTeX(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintNodeInTeX(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_COS:
                {
                    fprintf(output, "\\cos(");
                    PrintNodeInTeX(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_CTG:
                {
                    fprintf(output, "\\ctg(");
                    PrintNodeInTeX(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_LN:
                {
                    fprintf(output, "\\ln(");
                    PrintNodeInTeX(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_SIN:
                {
                    fprintf(output, "\\sin(");
                    PrintNodeInTeX(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_SQRT:
                {
                    fprintf(output, "\\sqrt{");
                    PrintNodeInTeX(output, differentiator, LEFT);
                    fprintf(output, "} ");
                    break;
                }

                case OPERATION_TG:
                {
                    fprintf(output, "\\tg(");
                    PrintNodeInTeX(output, differentiator, LEFT);
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

static int ReplaceLongExpression(Differentiator* differentiator, Node* node, int rep_array_index)
{
    PTR_ASSERT(differentiator)
    
    if (node == nullptr)
        return rep_array_index;

    error_t error = NO_ERR;

    rep_array_index = ReplaceLongExpression(differentiator, node->left,  rep_array_index);
    rep_array_index = ReplaceLongExpression(differentiator, node->right, rep_array_index);

    if (CalculateSizeOfSubtree(node) > MAX_LEN_OF_EXPRESSION) 
    {
        if (CalculateSizeOfSubtree(node->left) > CalculateSizeOfSubtree(node->right))
        {
            Lexem* new_lexem = (Lexem*) calloc(1, sizeof(Lexem));
            new_lexem->type             = TYPE_VARIABLE;
            new_lexem->value.var_index  = rep_array_index;
            rep_array_index++;

            Node* rep_node = NodeCtor(&error, new_lexem);

            Node* tmp_node = node->left;
            node->left     = rep_node;
            rep_node->left = tmp_node;
        }
        else
        {
            Lexem* new_lexem = (Lexem*) calloc(1, sizeof(Lexem));
            new_lexem->type             = TYPE_VARIABLE;
            new_lexem->value.var_index  = rep_array_index;
            rep_array_index++;

            Node* rep_node = NodeCtor(&error, new_lexem);

            Node* tmp_node = node->right;
            node->right    = rep_node;
            rep_node->left = tmp_node;
        }
    }

    return rep_array_index;
}

//=================================================================================================

static Node* FindRepVariableInExpression(Node* node, int index)
{
    if (node == nullptr)
        return nullptr;

    Node* replace_variable = node;

    if (((Lexem*)replace_variable->data)->type == TYPE_VARIABLE)
    {
        if (((Lexem*)replace_variable->data)->value.var_index == index)
        {
            if (replace_variable->left != nullptr)
            {
                return replace_variable;
            }
        }
    }

    replace_variable = FindRepVariableInExpression(node->left, index);
    if (replace_variable != nullptr)
        return replace_variable;

    return FindRepVariableInExpression(node->right, index);
}

//=================================================================================================

void PrintReplacesNodes(FILE* output, Differentiator* differentiator, Node* node)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(node)

    int number_of_rep_vars = ReplaceLongExpression(differentiator, node, 0);
    FILE* html_file  = nullptr;

    if (number_of_rep_vars != 0)
    {
        fprintf(output," где значения переменных равны: \\\n");

        Node* rep_node = nullptr;

        for (int i = 0; i < number_of_rep_vars; i++)
        {
            rep_node = FindRepVariableInExpression(node, i);
            fprintf(output, "$$ %s = ", REPLACE_ARRAY[i]);
            PrintNodeInTeX(output, differentiator, rep_node->left);
            fprintf(output, " $$\\\n");
        }
    }
}

//=================================================================================================

void DifferentiatorTextDump(FILE* output, Differentiator* differentiator, Node* node, const char* word)
{
    PTR_ASSERT(output)
    PTR_ASSERT(differentiator)
    PTR_ASSERT(word)

    fprintf(output, "%s", word);

    fprintf(output, "$$ ");

    PrintNodeInTeX(output, differentiator, node);

    fprintf(output, " $$\n");
}

//=================================================================================================

void DerivativeTextDump(FILE* output, Differentiator* differentiator, Node* node, Node* diff_node, const char* word)
{
    PTR_ASSERT(output)
    PTR_ASSERT(differentiator)
    PTR_ASSERT(word)
    
    fprintf(output, "%s", word);

    fprintf(output, "$$ \\frac{d}{dx}(");

    PrintNodeInTeX(output, differentiator, node);

    fprintf(output, ") = ");

    PrintNodeInTeX(output, differentiator, diff_node);

    fprintf(output, " $$\\\n");
}

//=================================================================================================

void ShowIntro(FILE* tex_file)
{
    fprintf(tex_file, BEGIN_MSG);
}

//=================================================================================================

void ShowOutro(FILE* tex_file)
{
    fprintf(tex_file, FINAL_MSG);
}

//=================================================================================================

void PrintExpressionInTeX(FILE* tex_file, Differentiator* differentiator, const char* word)
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(tex_file)

    DifferentiatorTextDump(tex_file, differentiator, TREE.root, word);

    PrintReplacesNodes(tex_file, differentiator, differentiator->tree.root);
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
            fprintf(dot_file, "%lg | NUM | {<left> left | <right> right}}\", fillcolor = \"blue\"];\n", VALUE.num_value);
            break;
        }
        case TYPE_VARIABLE:
        {
            fprintf(dot_file, "%s | VAR | {<left> left | <right> right}}\", fillcolor = \"green\"];\n", 
                    differentiator->name_table.array[VALUE.var_index]);
            break;
        }
        case TYPE_OPERATION:
        {
            fprintf(dot_file, "%s | OPER | {<left> left | <right> right}}\", fillcolor = \"red\"];\n", 
                    OPERATION_TABLE[VALUE.oper_index].string);
            break;
        }
        case TYPE_UNDEFINED:
        default:
            fprintf(dot_file, "}\"];\n");
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
}

//=================================================================================================

static void PrintHtmlIntro(FILE* html_file)
{
    fprintf(html_file, "<html>\n"
                        "\t<head>\n"
                        "\t\t<title>\n"
                        "\t\t\tList log\n"
                        "\t\t</title>\n"
                        "\t</head>\n"
                        "\t<body>\n"      
                        "\t\t<pre>\n");
}

//=================================================================================================

static void PrintHtmlOutro(FILE* html_file)
{
    fprintf(html_file,  "\t\t</pre>\n"
                        "\t</body>\n"
                        "</html>");
}

//=================================================================================================

static void PrintEquationToHtmlDump(FILE* output, Differentiator* differentiator, Node* node)
{
    PTR_ASSERT(output)
    PTR_ASSERT(differentiator)
    PTR_ASSERT(node)

    int node_priority = GetPriority(node); 

    switch (TYPE)
    {
        case TYPE_NUMBER:
            fprintf(output, "%lg", VALUE.num_value);
            break;

        case TYPE_VARIABLE:
        {
            int index = VALUE.var_index;
            fprintf(output, "%s", ARRAY[index].name);
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
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                    }

                    fprintf(output, " / ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_POW:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                    }

                    fprintf(output, " ^ ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_MUL:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                    }

                    fprintf(output, " * ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_PLUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                    }

                    fprintf(output, " + ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_MINUS:
                {
                    if (node_priority > GetPriority(LEFT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, LEFT);
                    }

                    fprintf(output, " - ");

                    if (node_priority > GetPriority(RIGHT))
                    {
                        fprintf(output, "\\left(");
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                        fprintf(output, "\\right)");
                    } 
                    else
                    {
                        PrintEquationToHtmlDump(output, differentiator, RIGHT);
                    }
                    break;
                }

                case OPERATION_COS:
                {
                    fprintf(output, "cos(");
                    PrintEquationToHtmlDump(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_CTG:
                {
                    fprintf(output, "ctg(");
                    PrintEquationToHtmlDump(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_LN:
                {
                    fprintf(output, "ln(");
                    PrintEquationToHtmlDump(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_SIN:
                {
                    fprintf(output, "sin(");
                    PrintEquationToHtmlDump(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_SQRT:
                {
                    fprintf(output, "sqrt(");
                    PrintEquationToHtmlDump(output, differentiator, LEFT);
                    fprintf(output, ") ");
                    break;
                }

                case OPERATION_TG:
                {
                    fprintf(output, "tg(");
                    PrintEquationToHtmlDump(output, differentiator, LEFT);
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

void DifferentiatorHtmlDump(FILE* html_file, Differentiator* differentiator, const char* file, 
                                                                             const int   line,
                                                                             const char* func )
{
    PTR_ASSERT(differentiator)
    PTR_ASSERT(html_file)

    static int dump_iter = 0;

    PrintHtmlIntro(html_file);

    fprintf(html_file, "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n\n");

    PrintEquationToHtmlDump(html_file, differentiator, differentiator->tree.root);

    TreeGraphDump(differentiator);

    char command[MAX_SIZE_OF_COMMAND] = {};
    sprintf(command, "dot %s -T png -o %s%d.png", DOT_FILE, PATH_TO_IMG, dump_iter);
    system(command);

    fprintf(html_file, "<img src = %s%d.png height = 400>\n", PATH_TO_IMG, dump_iter);

    fprintf(html_file, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");

    dump_iter++;

    PrintHtmlOutro(html_file);
}

//=================================================================================================
