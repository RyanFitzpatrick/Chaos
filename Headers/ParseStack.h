#ifndef _CH_PARSESTACK_
#define _CH_PARSESTACK_

#include "ParseTree.h"

typedef struct ParseStack {
    ParseTree * node;
    struct ParseStack * next;
} ParseStack;

ParseStack * BuildParseStack();
void EndParseStack(ParseStack *);

#endif
