#include "operator.h"

operator_t get_operator_type(const char *operator_str)
{
    if (operator_str == NULL)
        return OP_NONE;

    if (strcmp(operator_str, "|") == 0)
        return OP_PIPE;
    else if (strcmp(operator_str, ";") == 0)
        return OP_SEQ;
    else if (strcmp(operator_str, "&&") == 0)
        return OP_AND;
    else if (strcmp(operator_str, "||") == 0)
        return OP_OR;
    else
        return OP_NONE;
}