#include "ParseTree.h"
#include "Memory.h"

ParseTree * BuildParseTree(Symbol * symbol)
{
    /* Allocate memory for the ParseTree */
    ParseTree * tree = NewMem(sizeof(ParseTree));

    /* Initialize the ParseTree information */
    tree->symbol = symbol;
    tree->left = NULL;
    tree->center = NULL;
    tree->right = NULL;

    return tree;
}

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
    RemoveMem(tree);
}
