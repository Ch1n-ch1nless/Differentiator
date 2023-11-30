#include "differentiator_optimizate.h"

//=================================================================================================

static bool FindVariable(Node* node)
{
    if (node == nullptr)
        return false;

    if (((NodeData*)node->data)->type == TYPE_VARIABLE)
        return true;

    return FindVariable(node->left) || FindVariable(node->right);
}

//=================================================================================================

static bool RemoveConstants(Node* node)
{
    bool flag = false;
    
    if (node->left != nullptr)
        flag |= RemoveConstants(node->left);

    if (node->right != nullptr)
        flag |= RemoveConstants(node->right);

    if (node->left != nullptr && node->right != nullptr &&  ((NodeData*)node->data)->type == TYPE_OPERATION)
    {
        if (FindVariable(node->left) == false && FindVariable(node->right) == false)
        {
            double new_value = DifferentiatorCalculate(node);
            ((NodeData*)node->data)->type = TYPE_NUMBER;
            ((NodeData*)node->data)->value.num_value = new_value;

            free(node->left);
            node->left = nullptr;

            free(node->right);
            node->right = nullptr;

            return true;
        }
    }

    return flag;
}

//=================================================================================================

static bool DeleteNeutralElements(Node* node)
{
    if (node == nullptr)
        return false;

    bool flag = false;

    if (node->left != nullptr)
        flag |= DeleteNeutralElements(node->left);

    if (node->right != nullptr)
        flag |= DeleteNeutralElements(node->right);

    if (((NodeData*)node->data)->type == TYPE_OPERATION)
    {
        switch (((NodeData*)node->data)->value.oper_index)
        {
            case OPERATION_PLUS:
            {
                if (((NodeData*)node->left->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->left->data)->value.num_value))
                {
                    SubTreeDtor(&(node->left));
                    *node = *node->right;
                    return true;
                }
                else if (((NodeData*)node->right->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->right->data)->value.num_value))
                {
                    SubTreeDtor(&(node->right));
                    *node = *node->left;
                    return true;
                }

                return flag;
            }

            case OPERATION_MUL:
            {

                if ((((NodeData*)node->left->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->left->data)->value.num_value)) ||
                    (((NodeData*)node->right->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->right->data)->value.num_value)))
                {
                    SubTreeDtor(&(node->left));
                    SubTreeDtor(&(node->right));
                    ((NodeData*)node->data)->type  = TYPE_NUMBER;
                    ((NodeData*)node->data)->value.num_value = 0.0;

                    return true;
                }

                if (((NodeData*)node->left->data)->type == TYPE_NUMBER && IsOne(((NodeData*)node->left->data)->value.num_value))
                {
                    SubTreeDtor(&(node->left));
                    *node = *node->right;
                    return true;
                }
                else if (((NodeData*)node->right->data)->type == TYPE_NUMBER && IsOne(((NodeData*)node->right->data)->value.num_value))
                {
                    SubTreeDtor(&(node->right));
                    *node = *node->left;
                    return true;
                }

                return flag;
            }

            case OPERATION_MINUS:
            {
                if (((NodeData*)node->right->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->right->data)->value.num_value))
                {
                    SubTreeDtor(&(node->right));
                    *node = *node->left;
                    return true;
                }

                return flag;
            }

            case OPERATION_DIV:
            {
                if (((NodeData*)node->right->data)->type == TYPE_NUMBER && IsOne(((NodeData*)node->right->data)->value.num_value))
                {
                    SubTreeDtor(&(node->right));
                    *node = *node->left;
                    return true;
                }

                if (((NodeData*)node->left->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->left->data)->value.num_value))
                {
                    SubTreeDtor(&(node->left));
                    SubTreeDtor(&(node->right));
                    ((NodeData*)node->data)->type  = TYPE_NUMBER;
                    ((NodeData*)node->data)->value.num_value = 0.0;

                    return true;
                }

                return flag;
            }

            case OPERATION_POW:
            {
                if (((NodeData*)node->right->data)->type == TYPE_NUMBER && IsOne(((NodeData*)node->right->data)->value.num_value))
                {
                    SubTreeDtor(&(node->right));
                    *node = *node->left;

                    return true;
                }
                else if (((NodeData*)node->right->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->right->data)->value.num_value))
                {
                    SubTreeDtor(&(node->left));
                    SubTreeDtor(&(node->right));
                    ((NodeData*)node->data)->type            = TYPE_NUMBER;
                    ((NodeData*)node->data)->value.num_value = 1.0;

                    return true;
                }

                if (((NodeData*)node->left->data)->type == TYPE_NUMBER && IsOne(((NodeData*)node->left->data)->value.num_value))
                {
                    SubTreeDtor(&(node->right));
                    *node = *node->left;

                    return true;
                }
                else if (((NodeData*)node->left->data)->type == TYPE_NUMBER && IsZero(((NodeData*)node->left->data)->value.num_value))
                {
                    SubTreeDtor(&(node->left));
                    SubTreeDtor(&(node->right));
                    ((NodeData*)node->data)->type            = TYPE_NUMBER;
                    ((NodeData*)node->data)->value.num_value = 0.0;

                    return true;
                }

                return flag;
            }

            case OPERATION_SIN:
            case OPERATION_COS:
            case OPERATION_TG:
            case OPERATION_CTG:
            case OPERATION_LN:
            case OPERATION_SQRT:
            case OPERATION_UNDEFINED:
            default:
                break;
        }
    }
}

//=================================================================================================

void DifferentiatorSimplify(FILE* tex_file, Differentiator* differentiator)
{
    PTR_ASSERT(differentiator)

    bool is_tree_changed = false;

    do
    {
        is_tree_changed = false;
        is_tree_changed = DeleteNeutralElements(differentiator->tree.root);
        is_tree_changed = RemoveConstants(differentiator->tree.root);
    } 
    while (is_tree_changed);

    DeleteNeutralElements(differentiator->tree.root);
    is_tree_changed = RemoveConstants(differentiator->tree.root);

    int index = rand() % NUMBER_OF_QUOTES;

    PrintExpressionInTeX(tex_file, differentiator, FUNNY_MATH_QUOTES[index]);
}

//=================================================================================================