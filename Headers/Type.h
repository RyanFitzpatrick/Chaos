#ifndef _CH_TYPE
#define _CH_TYPE

#include "HashMap.h"

typedef struct Type
{
    char * name;
    HashMap * fields;
    struct Type * parent;
} Type;

Type * BuildType(char *, Type *);
void EndType(Type *);

#endif
