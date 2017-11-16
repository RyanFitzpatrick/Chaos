/* ParseStack.c: Implements various functions defined in ParseStack.h */

#include "ParseStack.h"
#include "Memory.h"

/* Initializes an empty ParseStack */
/* NOTE: It's recommended to use the BuildParseStack macro instead of calling this directly */
/* Returns: A newly allocated ParseStack */
ParseStack * _BuildParseStack()
{
    /* Allocate memory for the ParseStack */
    ParseStack * stack = NULL;
    NewMem(stack, sizeof(ParseStack));

    /* Initialize the ParseStack information */
    stack->node = NULL;
    stack->next = NULL;

    return stack;

    FAIL:
        /* Return NULL on error */
        return NULL;
}

ParseStack * _PushToStack(ParseStack * stack, ParseTree * tree)
{
    ParseStack * head = NULL;

    if (stack->node != NULL)
    {
        NewMem(head, sizeof(ParseStack));

        head->node = tree;
        head->next = stack;

        return head;
    }

    stack->node = tree;
    return stack;

    FAIL:
        return NULL;
}

/* Releases all memory used by a ParseStack */
/* Param (stack) ParseStack *: The ParseStack to be released */
void EndParseStack(ParseStack * stack)
{
    /* If stack is NULL then we stop since there's nothing we can do */
    if (stack == NULL)
        return;

    /* Release all memory used by the ParseStack and then release the ParseStack itself */
    EndParseTree(stack->node);
    EndParseStack(stack->next);
    DiscardMem(stack);
}
