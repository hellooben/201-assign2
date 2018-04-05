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

/*-------------PRIVATE FUNCTIONS-------------*/
AVAL *newAVAL(void*v,
void (*d)(void *,FILE *),           //display
int (*c)(void *,void *),           //comparator
void (*f)(void *));
void aDisplay(void *,FILE *);
int compareAVAL(void *,void *);
void swapper(BSTNODE *,BSTNODE *);
void freeAVAL(void *);
void freeAVALalmost(AVAL *);
void *getAVAL(AVAL *);
int getFREQ(AVAL *);
void updateFREQ(AVAL *, int);
int getHeight(AVAL *);
void setHeight(AVAL *, int);
int getBalance(AVAL *);
void setBalance(BSTNODE *);
void calculateHeights(BSTNODE *);
BSTNODE *findFavorite(BSTNODE *);
BSTNODE *getSibling(BSTNODE *);
int isLeft(BSTNODE *,BSTNODE *);
void insertionFixup(BST *, BSTNODE *);
void deleteFixup(BST *, BSTNODE *);
void rotate(BST *,BSTNODE *,BSTNODE *);
void leftRotate(BST *,BSTNODE *,BSTNODE *);
void rightRotate(BST *,BSTNODE *,BSTNODE *);


void aDisplay(void *val, FILE *fp) {
    AVAL *temp = val;
    temp->display(getAVAL(temp), fp);
    if (getFREQ(temp) > 1) {
        fprintf(fp, "[%d]", getFREQ(temp));
    }
    if (getBalance(temp) > 0) {
        fprintf(fp, "+");
    }
    if (getBalance(temp) < 0) {
        fprintf(fp, "-");
    }
}

int compareAVAL(void *v1, void *v2) {
    AVAL *temp = v1;
    AVAL *temp2 = v2;
    return temp->compare(getAVAL(temp),getAVAL(temp2));
}

void swapper(BSTNODE *a, BSTNODE *b) {
    AVAL *ta = getBSTNODEvalue(a);
    AVAL *tb = getBSTNODEvalue(b);

    void *temp = ta->data;
    ta->data = tb->data;
    tb->data = temp;

    int count = ta->freq;
    ta->freq = tb->freq;
    tb->freq = count;

    return;
}

void freeAVAL(void *val) {
    AVAL *temp = val;
    if (temp->free) {
        temp->free(getAVAL(temp));
        free(val);
    }
    return;
}

void freeAVALalmost(AVAL *val) {
    free(val);
    return;
}

void *getAVAL(AVAL *val) {
    return val->data;
}

int getFREQ(AVAL *val) {
    if (val != NULL) {
        return val->data;
    }
    else {return 0;}
}

void updateFREQ(AVAL *val, int count) {
    val->freq += count;
    return;
}

int getHeight(AVAL *val) {
    return val->height;
}

void setHeight(AVAL *val, int num) {
    val->height += num;
    return;
}

int getBalance(AVAL *val) {
    return val->balance;
}

void setBalance(BSTNODE *node) {
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    AVAL *val = getBSTNODEvalue(node);
    if (l!=NULL && r!=NULL) {
        val->balance = getBalance(getBSTNODEvalue(l)) - getBalance(getBSTNODEvalue(r));
    }
    else if (l!=NULL && r==NULL) {
        val->balance = getBalance(getBSTNODEvalue(l)) + 1;
    }
    else {
        val->balance = getBalance(getBSTNODEvalue(r)) - 1;
    }
    return;
}

void calculateHeights(BSTNODE *node) {
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    if (node == NULL) {return;}
    if (l == NULL || r == NULL) {
        return;
    }
    else if (l != NULL && r == NULL) {
        setHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(l)));
    }
    else if (r != NULL && l == NULL) {
        setHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(r)));
    }
    else {
        int numl = getHeight(getBSTNODEvalue(l));
        int numr = getHeight(getBSTNODEvalue(r));
        if (numl > numr) {
            setHeight(getBSTNODEvalue(node), numl);
        }
        else {
            setHeight(getBSTNODEvalue(node), numr);
        }
    }
    return;
}

BSTNODE *findFavorite(BSTNODE *node) {
    if (getBSTNODEleft(node) == NULL && getBSTNODEright(node) == NULL) {
        return node;
    }
    else if (getBSTNODEleft(node) == NULL) {
        return getBSTNODEright(node);
    }
    else if (getBSTNODEright(node) == NULL) {
        return getBSTNODEleft(node);
    }
    else {
        int h1 = getHeight(getBSTNODEvalue(getBSTNODEleft(node)));
        int h2 = getHeight(getBSTNODEvalue(getBSTNODEright(node)));
        if (h1 > h2) {
            return getBSTNODEleft(node);
        }
        else if (h2 > h1) {
            return getBSTNODEright(node);
        }
        else {
            return node;
        }
    }
}

BSTNODE *getSibling(BSTNODE *node) {
    BSTNODE *parent = getBSTNODEparent(node);
    if (getBSTNODEleft(parent) == node) {
        return getBSTNODEright(parent);
    }
    else {
        return getBSTNODEleft(parent);
    }
}

int
isLeft(BSTNODE *node, BSTNODE *parent) {
    BSTNODE *maybe = getBSTNODEleft(parent);
    if (maybe != NULL && maybe == node) {
        return 1;
    }
    else {
        return 0;
    }
}

/*------------------------------------------*/

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
    tree->bstree = newBST(aDisplay, compareAVAL, swapper, freeAVAL);
    tree->display = d;
    tree->compare = c;
    tree->free = f;
    return tree;
}

extern void
insertAVL(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);

    if (temp != NULL) {
        updateFREQ(getBSTNODEvalue(temp), 1);
        a->size ++;
        freeAVALalmost(new);
    }
    else {
        BSTNODE *node = insertBST(a->bstree, new);
        BSTNODE *heights = node;
        a->size ++;
        while (heights != NULL) {
            calculateHeights(heights);
            heights = getBSTNODEparent(heights);
        }
        insertionFixup(a->bstree, node);
    }
    return;
}

void insertionFixup(BST *t, BSTNODE *node) {
    while (1) {
        BSTNODE *parent = getBSTNODEparent(node);
        BSTNODE *left = getBSTNODEleft(node);
        BSTNODE *right = getBSTNODEright(node);
        BSTNODE *sibling = getSibling(node);

        if (parent == NULL) {
            break;
        }
        else if (findFavorite(parent) == sibling) { //case 1
            setBalance(parent);
            break;
        }
        else if (findFavorite(parent) == parent) { // case 2
            setBalance(parent);
            node = parent
            continue;
        }
        else {
            BSTNODE *fav = findFavorite(node);
            if (fav != node && isLinear(node, parent, fav) == 0) { //case 3
                rotate(t, fav, node);
                rotate(t, fav, parent);
                setBalance(fav);
                setBalance(parent);
                setBalance(node);
            }
            else { //case 4
                rotate(t, node, parent);
                setBalance(parent);
                setBalance(x);
            }
            break;
        }
    }
    return;
}

int isLinear(BSTNODE *node, BSTNODE *parent, BSTNODE *fav) {
    int total = 0;
    total += isLeft(node, parent);
    total += isLeft(fav, node);
    if (total == 2 || total == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void rotate(BST *t, BSTNODE *src, BSTNODE *dst) {
    if (isLeft(src, dst) == 0) {
        leftRotate(t, src, dst);
        return;
    }
    else {
        rightRotate(t, srt, dst);
        return;
    }
}

void leftRotate(BST *t, BSTNODE *src, BSTNODE *dst) {
    BSTNODE *srcLeft = getBSTNODEleft(src);
    BSTNODE *dstParent = getBSTNODEparent(dst);
    setBSTNODEright(dst, srcLeft);
    if (srcLeft != NULL) {
        setBSTNODEparent(srcLeft, dst);
    }
    setBSTNODEparent(src, dstParent);
    if (dstParent == NULL) {
        setBSTroot(t, src);
    }
    else if (isLeft(dst, dstParent) == 1) {
        setBSTNODEleft(dstParent, src);
    }
    else {
        setBSTNODEright(dstParent, src);
    }
    setBSTNODEleft(src, dst);
    setBSTNODEparent(dst, src);
    calculateHeights(dst);
    calculateHeights(src);
    return;
}

void rightRotate(BST *t, BSTNODE *src, BSTNODE *dst) {
    BSTNODE *srcRight = getBSTNODEright(src);
    BSTNODE *dstParent = getBSTNODEparent(dst);
    setBSTNODEleft(dst, srcRight);
    if (srcRight != NULL) {
        setBSTNODEparent(srcRight, dst);
    }
    setBSTNODEparent(src, dstParent);
    if (dstParent == NULL) {
        setBSTroot(t, src);
    }
    else if (isLeft(dst, dstParent) == 0) {
        setBSTNODEright(dstParent, src);
    }
    else {
        setBSTNODEleft(dstParent, src);
    }
    setBSTNODEright(src, dst);
    setBSTNODEparent(dst, src);
    calculateHeights(dst);
    calculateHeights(src);
    return;
}

extern void *
deleteAVL(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);
    if (temp != NULL) {
        void *returnable = getAVAL(getBSTNODEvalue(temp));
        if (getFREQ(getBSTNODEvalue(temp)) > 1) {
            updateFREQ(getBSTNODEvalue(temp), -1);
            a->size --;
            freeAVALalmost(new);
            return returnable;
        }
        else {
            temp = swapToLeafBST(a->bstree, temp);
            BSTNODE *heights = temp;
            while (heights != NULL) {
                calculateHeights(heights);
                heights = getBSTNODEparent(heights);
            }
            deleteFixup(a->bstree, temp);
            pruneLeafBST(a->bstree, temp);
            setBSTsize(a->bstree, sizeBST(a->bstree)-1);
            a->size --;
            freeGSTVALalmost(new);
            freeBSTNODE(temp, a->free);
            return returnable;
        }
    }
    else {
        freeGSTVALalmost(new);
        return NULL;
    }
}

void deleteFixup(BST *t, BSTNODE *node) {
    int h = getHeight(getBSTNODEvalue(node));
    setHeight(node, -h);
    BSTNODE *parent = getBSTNODEparent(node);
    // BSTNODE *left = getBSTNODEleft(node);
    // BSTNODE *right = getBSTNODEright(node);
    BSTNODE *sibling = getSibling(node);

    while (1) {
        if (getBSTroot(t) == node) {
            break;
        }
        else if (findFavorite(parent) == node) { //case 1
            setBalance(parent);
            node = parent;
            continue;
        }
        else if (findFavorite(parent) == parent) { //case 2
            setBalance(parent);
            break;
        }
        else {
            BSTNODE *fav = findFavorite(sibling);
            if (fav != sibling && isLinear(node, parent, fav) == 0) { // case 3
                rotate(fav, sibling);
                rotate(fav, parent);
                setBalance(parent);
                setBalance(sibling);
                setBalance(fav);
                node = fav;
                continue;
            }
            else { //case 4
                rotate(sibling, parent);
                setBalance(parent);
                setBalance(sibling);
                if (fav == sibling) {
                    break;
                }
                node = sibling;
                continue;
            }
        }
    }
    return;
}

extern int
findAVLcount(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);
    if (temp != NULL) {
        freeAVALalmost(new);
        return getFREQ(getBSTNODEvalue(temp));
    }
    else {
        freeAVALalmost(new);
        return 0;
    }
}

extern void *
findAVL(AVL *a,void *value) {
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);
    if (temp != NULL) {
        void *returnable = getAVAL(getBSTNODEvalue(temp));
        freeAVALalmost(new);
        return returnable;
    }
    else {
        freeAVALalmost(new);
        return NULL;
    }
}

extern int
sizeAVL(AVL *a) {
    return a->size;
}

extern int
duplicatesAVL(AVL *a) {
    return sizeAVL(a) - sizeBST(a->bstree);
}

extern void
statisticsAVL(AVL *a,FILE *fp) {
    fprintf(fp, "Duplicates: %d\n", duplicatesAVL(a));
    statisticsBST(a->bstree, fp);
    return;
}

extern void
displayAVL(AVL *a,FILE *fp) {
    displayBSTdecorated(a->bstree, fp);
    return;
}

extern void
displayAVLdebug(AVL *a,FILE *fp) {
    displayBST(a->bstree, fp);
    return;
}

extern void
freeAVL(AVL *a) {
    freeBST(a->bstree);
    free(a);
    return;
}
