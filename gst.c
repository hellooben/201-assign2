#include "gst.h"
#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

GSTVAL *newGSTVAL(void*,GST*);
void displayGSTVAL(void *,FILE *);
int compareGSTVAL(void *, void *);
void updateFREQ(GSTVAL *,int);
int getFREQ(GSTVAL *);
void *getGSTVAL(GSTVAL *);

typdef struct gstval
{
    void *data;
    int freq;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    // void (*swap)(BSTNODE *,BSTNODE *);     //swapper
    void (*free)(void *);                  //free
} GSTVAL;

GSTVAL *
newGSTVAL(void *v, GST *g) {
    GSTVAL *new = malloc(sizeof(GSTVAL));
    new->data = v;
    new->freq = 1;
    new->display = g->display;
    new->compare = g->compare;
    new->free = g->free;
    return new;
}

void displayGSTVAL(void *val, FILE *fp) {
    val->display(getGSTVAL(val),fp);
    if (getFREQ(val) > 1) {
        fprintf(fp, "[%d]", getFREQ(val));
    }
    return;
}

int compareGSTVAL(void *v1, void *v2) {
    return v1->compare(getGSTVAL(v1),getGSTVAL(v2));
}

void updateFREQ(GSTVAL *val, int num) {
    val->freq += num;
    return;
}

int getFREQ(GSTVAL *val) {
    if (val != NULL) {
        return val->freq;
    }
    else {
        return 0;
    }
}

void *getGSTVAL(GSTVAL *val) {
    return val->data;
}


/*----------------------------------------------------------------------*/


struct gst
{
    int size;
    BST *bstree;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    // void (*swap)(BSTNODE *,BSTNODE *);     //swapper
    void (*free)(void *);                  //free
};

extern GST *
newGST(void (*d)(void *,FILE *),           //display
int (*c)(void *,void *)),           //comparator
void (*f)(void *)) {                 //free
    GST *tree = malloc(sizeof(GST));
    assert(tree!=0);

    tree->size = 0;
    tree->bstree = newBST(displayGSTVAL, compareGSTVAL, 0, f);
    tree->display = d;
    tree->compare = c;
    tree->free = f;
    return tree;
}

extern void
insertGST(GST *g,void *value) {
    BSTNODE *temp = findBST(g->bstree, value);
    if (temp != NULL) {
        updateFREQ(getBSTNODEvalue(temp),1);
        g->size ++;
    }
    else {
        GSTVAL *new = newGSTVAL(value, g);
        insertBST(g->bstree, new);
        g->size ++;
    }
    return;
}

extern int
findGSTcount(GST *g,void *value) {
    BSTNODE *temp = findBST(g->bstree, value);
    if (temp != NULL) {
        return getFREQ(getBSTNODEvalue(temp));
    }
    else {
        return 0;
    }
}

extern void *
findGST(GST *g,void *value) {
    BSTNODE *temp = findBST(g->bstree, value);
    if (temp != NULL) {
        return getGSTVAL(getBSTNODEvalue(temp));
    }
    else {
        printf("Value ");
        g->display(value, stdout);
        printf(" not found\n");
        return NULL;
    }
}

extern void *
deleteGST(GST *g,void *value) {
    BSTNODE *temp = findBST(g->bstree, value);
    if (temp != NULL) {
        if (getFREQ(getBSTNODEvalue(temp)) > 1) {
            updateFREQ(temp, -1);
            g->size --;
            return getGSTVAL(getBSTNODEvalue(temp));
        }
        else {
            temp = swapToLeafBST(g->bstree, temp);
            pruneLeafBST(h->bstree, temp);
            setBSTsize(h->bstree, sizeBST(h->bstree)-1);
            g->size --;
            return getGSTVAL(getBSTNODEvalue(temp));
        }
    }
    else {
        printf("Value ");
        g->display(value, stdout);
        printf(" not found\n");
        return NULL;
    }
}

extern int
sizeGST(GST *g) {
    return g->size;
}

extern int
duplicates(GST *g) {
    return sizeGST(g) - sizeBST(g->bstree);
}

extern void
statisticsGST(GST *g,FILE *fp) {
    fprintf(fp, "Duplicates: %d\n", duplicates(g));
    statisticsBST(g, fp);
    return;
}

extern void
displayGST(GST *g,FILE *fp) {
    displayBSTdecorated(g->bstree, fp);
    return;
}

extern void
displayGSTdebug(GST *g,FILE *fp) {
    displayBST(g->bstree, fp);
    return;
}
