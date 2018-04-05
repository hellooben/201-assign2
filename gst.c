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
    // void (*swap)(BSTNODE *,BSTNODE *);     //swapper
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
void updateFREQ(GSTVAL *,int);
int getFREQ(GSTVAL *);
void *getGSTVAL(GSTVAL *);

void displayGSTVAL(void *val, FILE *fp) {
    GSTVAL *temp = val;
    temp->display(getGSTVAL(temp),fp);
    if (getFREQ(val) > 1) {
        fprintf(fp, "[%d]", getFREQ(temp));
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
    // printf("IN INSERTGST\n");
    GSTVAL *new = newGSTVAL(value, g->display, g->compare, g->free);
    BSTNODE *temp = findBST(g->bstree, new);
    // printf("result of findBST awaits!!!!!!!:\n");
    if (temp != NULL) {
        // printf("found it!\n");
        updateFREQ(getBSTNODEvalue(temp),1);
        g->size ++;
        freeGSTVALalmost(new);
    }
    else {
        // printf("didnt find it!\n");
        // printf("Made the new GSTVAL!\n");
        insertBST(g->bstree, new);
        g->size ++;
    }
    // freeGSTVALalmost(new);
    return;
}

extern int
findGSTcount(GST *g,void *value) {
    GSTVAL *new = newGSTVAL(value, g->display, g->compare, g->free);
    BSTNODE *temp = findBST(g->bstree, new);
    if (temp != NULL) {
        // freeGSTVAL(new);
        freeGSTVALalmost(new);
        return getFREQ(getBSTNODEvalue(temp));
    }
    else {
        // freeGSTVAL(new);
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
        // freeGSTVAL(new);
        freeGSTVALalmost(new);
        return returnable;
    }
    else {
        // printf("Value ");
        // g->display(value, stdout);
        // printf(" not found\n");
        // freeGSTVAL(new);
        freeGSTVALalmost(new);
        return NULL;
    }
}

extern void *
deleteGST(GST *g,void *value) {
    GSTVAL *new = newGSTVAL(value, g->display, g->compare, g->free);
    BSTNODE *temp = findBST(g->bstree, new);
    if (temp != NULL) {
        // printf("found\n");
        // void *returnable = getGSTVAL(getBSTNODEvalue(temp));
        void *returnable = getGSTVAL(getBSTNODEvalue(temp));
        if (getFREQ(getBSTNODEvalue(temp)) > 1) {
            updateFREQ(getBSTNODEvalue(temp), -1);
            g->size --;
            // freeGSTVAL(new);
            // printf("RETURNING: \n");
            // g->display(returnable, stdout);
            // printf("\n");
            freeGSTVALalmost(new);
            return returnable;
            // return getGSTVAL(getBSTNODEvalue(temp));
        }
        else {
            temp = swapToLeafBST(g->bstree, temp);
            pruneLeafBST(g->bstree, temp);
            setBSTsize(g->bstree, sizeBST(g->bstree)-1);
            g->size --;
            // returnable = freeGSTVALalmost(new);
            // printf("RETURNING after swap and prune: \n");
            // g->display(returnable, stdout);
            // printf("\n");
            freeGSTVALalmost(new);
            freeBSTNODE(temp, g->free);
            return returnable;
            // return getGSTVAL(getBSTNODEvalue(temp));
        }
    }
    else {
        // printf("Value ");
        // new->display(value, stdout);
        // printf(" not found!!!!\n");
        // freeGSTVAL(new);
        freeGSTVALalmost(new);
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
    statisticsBST(g->bstree, fp);
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

extern void
freeGST(GST *g) {
    freeBST(g->bstree);
    free(g);
    return;
}
