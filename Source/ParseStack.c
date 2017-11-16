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

/* Pushes a ParseTree node to the ParseStack */
/* NOTE: It's recommended to use the PushToStack macro instead of calling this directly */
/* Param (stack) ParseStack *: The ParseStack to push the node onto */
/* Param (tree) ParseTree *: The ParseTree node to push onto the stack */
/* Returns: The modified stack with the new node on top */
ParseStack * _PushToStack(ParseStack * stack, ParseTree * tree)
{
    ParseStack * head = NULL;

    /* If the stack already has nodes then create a new node a push it on top of the stack */
    if (stack->node != NULL)
    {
        /* Allocate memory for the new ParseStack node */
        NewMem(head, sizeof(ParseStack));

        /* Initialize the ParseStack information and add it to the top of the stack */
        head->node = tree;
        head->next = stack;

        return head;
    }

    /* If the stack has no nodes then add the tree to the base and return */
    stack->node = tree;
    return stack;

    FAIL:
        /* Return NULL on error */
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
