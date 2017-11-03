#include "Type.h"
#include "Memory.h"
#include <string.h>

Type * BuildType(char * name, Type * parent)
{
    Type * type = NewMem(sizeof(Type));

    type->fields = BuildMap(32);
    type->parent = parent;
    type->name = NewMem(sizeof(char) * (strlen(name) + 1));
    strcpy(type->name, name);

    return type;
}

void EndType(Type * type)
{
    if (type == NULL)
        return;

    EndMap(type->fields);
    RemoveMem(type->name);
    RemoveMem(type);
}
