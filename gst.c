#include "gst.h"
#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


typedef struct gstval
{
    void *data;
    int freq;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*free)(void *);                  //free
} GSTVAL;

GSTVAL *
newGSTVAL(void *v,
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *)) {
    GSTVAL *new = malloc(sizeof(GSTVAL));
    new->data = v;
    new->freq = 1;
    new->display = d;
    new->compare = c;
    new->free = f;
    return new;
}

GSTVAL *newGSTVAL(void*,
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *));
void displayGSTVAL(void *,FILE *);
int compareGSTVAL(void *, void *);
void freeGSTVAL(void *);
void freeGSTVALalmost(GSTVAL *);
void updateFreqGST(GSTVAL *,int);
int getFreqGST(GSTVAL *);
void *getGSTVAL(GSTVAL *);

void displayGSTVAL(void *val, FILE *fp) {
    GSTVAL *temp = val;
    temp->display(getGSTVAL(temp),fp);
    if (getFreqGST(val) > 1) {
        fprintf(fp, "[%d]", getFreqGST(temp));
    }
    return;
}

int compareGSTVAL(void *v1, void *v2) {
    GSTVAL *temp = v1;
    GSTVAL *temp2 = v2;
    return temp->compare(getGSTVAL(temp),getGSTVAL(temp2));
}

void freeGSTVAL(void *val) {
    GSTVAL *temp = val;
    if (temp->free) {
        temp->free(getGSTVAL(temp));
        free(val);
    }
    return;
}

void freeGSTVALalmost(GSTVAL *val) {
    free(val);
    return;
}

void updateFreqGST(GSTVAL *val, int num) {
    val->freq += num;
    return;
}

int getFreqGST(GSTVAL *val) {
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
    void (*free)(void *);                  //free
};

extern GST *
newGST(void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *)) {                 //free
    GST *tree = malloc(sizeof(GST));
    assert(tree!=0);

    tree->size = 0;
    tree->bstree = newBST(displayGSTVAL, compareGSTVAL, 0, freeGSTVAL);
    tree->display = d;
    tree->compare = c;
    tree->free = f;
    return tree;
}

extern void
insertGST(GST *g,void *value) {
    GSTVAL *new = newGSTVAL(value, g->display, g->compare, g->free);
    BSTNODE *temp = findBST(g->bstree, new);
    if (temp != NULL) {
        updateFreqGST(getBSTNODEvalue(temp),1);
        g->size ++;
        freeGSTVALalmost(new);
    }
    else {
        insertBST(g->bstree, new);
        g->size ++;
    }
    return;
}

extern int
findGSTcount(GST *g,void *value) {
    GSTVAL *new = newGSTVAL(value, g->display, g->compare, g->free);
    BSTNODE *temp = findBST(g->bstree, new);
    if (temp != NULL) {
        freeGSTVALalmost(new);
        return getFreqGST(getBSTNODEvalue(temp));
    }
    else {
        freeGSTVALalmost(new);
        return 0;
    }
}

extern void *
findGST(GST *g,void *value) {
    GSTVAL *new = newGSTVAL(value, g->display, g->compare, g->free);
    BSTNODE *temp = findBST(g->bstree, new);
    if (temp != NULL) {
        void *returnable = getGSTVAL(getBSTNODEvalue(temp));
        freeGSTVALalmost(new);
        return returnable;
    }
    else {
        freeGSTVALalmost(new);
        return NULL;
    }
}

extern void *
deleteGST(GST *g,void *value) {
    GSTVAL *new = newGSTVAL(value, g->display, g->compare, g->free);
    BSTNODE *temp = findBST(g->bstree, new);
    if (temp != NULL) {
        void *returnable = getGSTVAL(getBSTNODEvalue(temp));
        if (getFreqGST(getBSTNODEvalue(temp)) > 1) {
            updateFreqGST(getBSTNODEvalue(temp), -1);
            g->size --;
            freeGSTVALalmost(new);
            return returnable;
        }
        else {
            temp = swapToLeafBST(g->bstree, temp);
            pruneLeafBST(g->bstree, temp);
            setBSTsize(g->bstree, sizeBST(g->bstree)-1);
            g->size --;
            freeGSTVALalmost(new);
            freeBSTNODE(temp, g->free);
            return returnable;
        }
    }
    else {
        freeGSTVALalmost(new);
        return NULL;
    }
}

extern int
sizeGST(GST *g) {
    return sizeBST(g->bstree);
}

extern int
duplicates(GST *g) {
    return g->size - sizeBST(g->bstree);
}

extern void
statisticsGST(GST *g,FILE *fp) {
    fprintf(fp, "Duplicates: %d\n", duplicates(g));
    statisticsBST(g->bstree, fp);
    return;
}

extern void
displayGST(GST *g,FILE *fp) {
    if (getBSTroot(g->bstree) == NULL) {
        fprintf(fp, "EMPTY\n");
    }
    else {
        displayBSTdecorated(g->bstree, fp);
    }
    return;
}

extern void
displayGSTdebug(GST *g,FILE *fp) {
    displayBST(g->bstree, fp);
    return;
}

extern void
freeGST(GST *g) {
    freeBST(g->bstree);
    free(g);
    return;
}
