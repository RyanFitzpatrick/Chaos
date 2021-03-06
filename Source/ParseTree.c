/* ParseTree.c: Implements various functions defined in ParseTree.h */

#include "ParseTree.h"
#include "Memory.h"

/* Initializes a ParseTree node with the given Symbol */
/* NOTE: It's recommended to use the BuildParseTree macro instead of calling this directly */
/* Param (symbol) Symbol *: The Symbol representing the ParseTree node */
/* Returns: A newly allocated ParseTree node */
ParseTree * _BuildParseTree(Symbol * symbol)
{
    /* Allocate memory for the ParseTree */
    ParseTree * tree = NULL;
    NewMem(tree, sizeof(ParseTree));

    /* Initialize the ParseTree information */
    tree->symbol = symbol;
    tree->left = NULL;
    tree->center = NULL;
    tree->right = NULL;

    return tree;

    FAIL:
        /* Return NULL on error */
        return NULL;
}

/* Releases all memory used by a ParseTree */
/* Param (tree) ParseTree *: The ParseTree to be released */
void EndParseTree(ParseTree * tree)
{
    /* If tree is NULL then we stop since there's nothing we can do */
    if (tree == NULL)
        return;

    /* Release all memory used by the ParseTree and then release the ParseTree itself */
    EndSymbol(tree->symbol);
    EndParseTree(tree->left);
    EndParseTree(tree->center);
    EndParseTree(tree->right);
    DiscardMem(tree);
}
