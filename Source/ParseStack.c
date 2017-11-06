/* ParseStack.c: Implements various functions defined in ParseStack.h */

#include "ParseStack.h"
#include "Memory.h"

/* Initializes an empty ParseStack */
/* Returns: A newly allocated ParseStack */
ParseStack * BuildParseStack()
{
    /* Allocate memory for the ParseStack */
    ParseStack * stack = NewMem(sizeof(ParseStack));

    /* Initialize the ParseStack information */
    stack->node = NULL;
    stack->next = NULL;

    return stack;
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
    RemoveMem(stack);
}
