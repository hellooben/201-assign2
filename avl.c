#include "avl.h"
#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct aval
{
    void *data;
    int freq;
    int height;
    int balance;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    // void (*swap)(BSTNODE *,BSTNODE *);     //swapper
    void (*free)(void *);
} AVAL;

AVAL *newAVAL(void*v,
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *));

AVAL *newAVAL(void*v,
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *)) {
    AVAL *new = malloc(sizeof(AVAL));
    new->data = v;
    new->freq = 1;
    new->height = 0;
    new->balance = 0;
    new->display = d;
    new->compare = c;
    new->free = f;
    return new;
}

struct avl
{
    int size;
    BST *bstree;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    // void (*swap)(BSTNODE *,BSTNODE *);    //swapper
    void (*free)(void *);                    //free
}

extern AVL *
newAVL (
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),            //comparator
void (*f)(void *)) {                //free
    AVL *tree = malloc(sizeof(AVL));
    tree->size = 0;
    tree->bstree = newBST()
}
