#include "ParseStack.h"
#include "Memory.h"

ParseStack * BuildParseStack() {
    /* Allocate memory for the ParseStack */
    ParseStack * stack = NewMem(sizeof(ParseStack));

    /* Initialize the ParseStack information */
    stack->node = NULL;
    stack->next = NULL;

    return stack;
}

void EndParseStack(ParseStack * stack) {
    /* If stack is NULL then we stop since there's nothing we can do */
    if (stack == NULL)
        return;

    /* Release all memory used by the ParseStack and then release the ParseStack itself */
    EndParseTree(stack->node);
    EndParseStack(stack->next);
    RemoveMem(stack);
}
