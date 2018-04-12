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
void addHeight(AVAL *, int);
int getBalance(AVAL *);
void setBalance(BSTNODE *);
void calculateHeights(BSTNODE *);
BSTNODE *findFavorite(BSTNODE *);
BSTNODE *getSibling(BSTNODE *);
int testLeft(BSTNODE *,BSTNODE *);
void insertionFixup(BST *, BSTNODE *);
void deleteFixup(BST *, BSTNODE *);
int isLinear(BSTNODE *, BSTNODE *, BSTNODE *);
void rotate(BST *,BSTNODE *,BSTNODE *);
void leftRotate(BST *,BSTNODE *,BSTNODE *);
void rightRotate(BST *,BSTNODE *,BSTNODE *);


void aDisplay(void *val, FILE *fp) {
    AVAL *temp = val;
    temp->display(getAVAL(temp), fp);
    // printf("{%d}", getHeight(temp));
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
        return val->freq;
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

void addHeight(AVAL *val, int num) {
    val->height += num;
    return;
}

void setHeight (AVAL *val, int num) {
    val->height = num;
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
        val->balance = getHeight(getBSTNODEvalue(l)) - getHeight(getBSTNODEvalue(r));
    }
    else if (l!=NULL && r==NULL) {
        val->balance = getHeight(getBSTNODEvalue(l)) + 1;
    }
    else if (r!= NULL && l == NULL) {
        val->balance = (-1) - getHeight(getBSTNODEvalue(r));
    }
    else {
        val->balance = 0;
    }
    return;
}

void calculateHeights(BSTNODE *node) {
    // printf("\n\ncalculating heights\nNODE: ");
    // AVAL *temp = getBSTNODEvalue(node);
    // temp->display(getAVAL(temp), stdout);
    // printf("\n");
    BSTNODE *l = getBSTNODEleft(node);
    BSTNODE *r = getBSTNODEright(node);

    if (node == NULL) {return;}
    if (l == NULL && r == NULL) {
        // printf("both null\n");
        setHeight(getBSTNODEvalue(node), 0);
        setBalance(node);
        return;
    }
    else if (l != NULL && r == NULL) {
        // printf("left isnt' null\n");
        // addHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(l))+1);
        setHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(l))+1);
    }
    else if (r != NULL && l == NULL) {
        // printf("right isnt' null\n");
        // addHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(r))+1);
        setHeight(getBSTNODEvalue(node), getHeight(getBSTNODEvalue(r))+1);
    }
    else {
        // printf("TWO CHILDREN\n");
        int numl = getHeight(getBSTNODEvalue(l));
        int numr = getHeight(getBSTNODEvalue(r));
        if (numl > numr) {
            // addHeight(getBSTNODEvalue(node), numl+1);
            setHeight(getBSTNODEvalue(node), numl+1);
        }
        else {
            // addHeight(getBSTNODEvalue(node), numr+1);
            setHeight(getBSTNODEvalue(node), numr+1);
        }
    }
    // printf("height is now %d\n\n\n", getHeight(getBSTNODEvalue(node)));
    setBalance(node);
    return;
}

BSTNODE *findFavorite(BSTNODE *node) {
    // setBalance(node);
    if (getBalance(getBSTNODEvalue(node)) >= 1) {
        return getBSTNODEleft(node);
    }
    else if (getBalance(getBSTNODEvalue(node)) <= -1) {
        return getBSTNODEright(node);
    }
    else {
        return NULL;
    }
}

BSTNODE *getSibling(BSTNODE *node) {
    BSTNODE *parent = getBSTNODEparent(node);
    if (parent == NULL) {return NULL;}
    if (getBSTNODEleft(parent) == node) {
        return getBSTNODEright(parent);
    }
    else if (getBSTNODEright(parent) == node) {
        return getBSTNODEleft(parent);
    }
    else {
        return NULL;
    }
}

int
testLeft(BSTNODE *node, BSTNODE *parent) {
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
    new->height = 1;
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
};

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
    printf("\nIN START OF INSERT WTIH ");
    a->display(value, stdout);
    printf("|AVL LOOKS LIKE :\n");
    displayAVL(a, stdout);
    printf("\n");
    AVAL *new = newAVAL(value, a->display, a->compare, a->free);
    BSTNODE *temp = findBST(a->bstree, new);

    if (temp != NULL) {
        printf("already in the tree\n");
        updateFREQ(getBSTNODEvalue(temp), 1);
        a->size ++;
        freeAVALalmost(new);
    }
    else {
        printf("not in the tree\n");
        BSTNODE *node = insertBST(a->bstree, new);
        a->size ++;
        printf("AFTER INSERT|AVL LOOKS LIKE :\n");
        displayAVL(a, stdout);
        printf("\n");

        insertionFixup(a->bstree, node);
    }
    printf("\nDONE INSERTING");
    printf("|AVL LOOKS LIKE :\n");
    displayAVL(a, stdout);
    printf("-----------------------------------\n");
    return;
}

void insertionFixup(BST *t, BSTNODE *node) {
    // printf("IN FIXUP\n");
    while (1) {
        AVAL *temp = getBSTNODEvalue(node);
        printf("IN THE WHILE LOOP\nnode: ");
        temp->display(getAVAL(temp), stdout);
        printf("\n");
        BSTNODE *parent = getBSTNODEparent(node);
        BSTNODE *fav = findFavorite(node);
        BSTNODE *sibling = getSibling(node);

        if (parent == NULL) {
            // printf("parent is null\n");
            setBalance(node);
            break;
        }
        else if (findFavorite(parent) != NULL && findFavorite(parent) == sibling) { //case 1
            printf("parent's favorite is the sibling, case 1\n");
            setBalance(parent);
            break;
        }
        else if (findFavorite(parent) == NULL) { // case 2
            printf("parent has no favorite, case 2\n");
            setBalance(parent);
            node = parent;
            continue;
        }
        else {
            if (fav != NULL && isLinear(node, parent, fav) == 0) { //case 3
                printf("in case 3\n");
                rotate(t, fav, node);
                printf("AVL after first rotation: \n");
                displayBSTdecorated(t, stdout);
                printf("\n");
                rotate(t, fav, parent);
                printf("AVL after second rotation: \n");
                displayBSTdecorated(t, stdout);
                printf("\n");
                setBalance(fav);
                setBalance(parent);
                setBalance(node);
            }
            else { //case 4
                printf("in case 4\n");
                printf("AVL BEFORE rotation: \n");
                displayBSTdecorated(t, stdout);
                printf("\n");
                rotate(t, node, parent);
                printf("AVL AFTER rotation: \n");
                displayBSTdecorated(t, stdout);
                printf("\n");
                setBalance(parent);
                setBalance(node);
            }
            break;
        }
    }
    return;
}

int isLinear(BSTNODE *node, BSTNODE *parent, BSTNODE *fav) {
    int total = 0;
    total += testLeft(node, parent);
    total += testLeft(fav, node);
    if (total == 2 || total == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void rotate(BST *t, BSTNODE *src, BSTNODE *dst) {
    if (testLeft(src, dst) == 0) {
        leftRotate(t, src, dst);
        return;
    }
    else {
        rightRotate(t, src, dst);
        return;
    }
}

void leftRotate(BST *t, BSTNODE *y, BSTNODE *x) {
    printf("left rotating!\n");
    AVAL *t1 = getBSTNODEvalue(y);
    AVAL *t2 = getBSTNODEvalue(x);
    t1->display(getAVAL(t1), stdout);
    printf(" and ");
    t1->display(getAVAL(t2), stdout);
    printf("\n");

    BSTNODE *yLeft = getBSTNODEleft(y);
    BSTNODE *xParent = getBSTNODEparent(x);

    setBSTNODEright(x, yLeft);

    if (yLeft != NULL) {
        setBSTNODEparent(yLeft, x);
    }

    setBSTNODEparent(y, xParent);

    if (xParent == NULL) {
        setBSTroot(t, y);
    }
    else if (testLeft(x, xParent) == 1) {
        setBSTNODEleft(xParent, y);
    }
    else {
        setBSTNODEright(xParent, y);
    }

    setBSTNODEleft(y, x);
    setBSTNODEparent(x, y);

    calculateHeights(x);
    calculateHeights(y);
    return;
}

void rightRotate(BST *t, BSTNODE *y, BSTNODE *x) {
    printf("right rotating!\n");
    AVAL *t1 = getBSTNODEvalue(y);
    AVAL *t2 = getBSTNODEvalue(x);
    t1->display(getAVAL(t1), stdout);
    printf(" and ");
    t1->display(getAVAL(t2), stdout);
    printf("\n");

    BSTNODE *yRight = getBSTNODEright(y);
    BSTNODE *xParent = getBSTNODEparent(x);

    setBSTNODEleft(x, yRight);

    if (yRight != NULL) {
        setBSTNODEparent(yRight, x);
    }

    setBSTNODEparent(y, xParent);

    if (xParent == NULL) {
        setBSTroot(t, y);
    }
    else if (testLeft(x, xParent) == 0) {
        setBSTNODEright(xParent, y);
    }
    else {
        setBSTNODEleft(xParent, y);
    }

    setBSTNODEright(y, x);
    setBSTNODEparent(x, y);

    calculateHeights(x);
    calculateHeights(y);

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
            // printf("in the else\n");
            temp = swapToLeafBST(a->bstree, temp);
            BSTNODE *nud = getBSTNODEparent(temp);
            pruneLeafBST(a->bstree, temp);

            int b = 0;
            while (nud) {
                calculateHeights(nud);
                b = getBalance(getBSTNODEvalue(nud));
                if (b > 1 || b < -1) {
                    deleteFixup(a->bstree, temp);
                }
                nud = getBSTNODEparent(nud);
            }

            // printf("out the while\n");
            // pruneLeafBST(a->bstree, temp);

            setBSTsize(a->bstree, sizeBST(a->bstree)-1);
            a->size --;

            freeAVALalmost(new);
            freeBSTNODE(temp, a->free);

            return returnable;
        }
    }
    else {
        updateFREQ(getBSTNODEvalue(temp), 1);
        freeAVALalmost(new);
        return NULL;
    }
}

void deleteFixup(BST *t, BSTNODE *node) {
    printf("\nIN DELETE FIXUP\n");

    // int h = getHeight(getBSTNODEvalue(node));
    // printf("HEIGHT: %d\n", h);
    setHeight(getBSTNODEvalue(node), 0);
    BSTNODE *parent = getBSTNODEparent(node);
    BSTNODE *sibling = getSibling(node);

    while (1) {
        printf("NODE: ");
        AVAL *temp = getBSTNODEvalue(node);
        temp->display(getAVAL(temp), stdout);
        printf("\n");
        if (getBSTroot(t) == node) {
            printf("root is null\n");
            break;
        }
        else if (findFavorite(parent) == node) { //case 1
            printf("parent's favorite is the node\n");
            setBalance(parent);
            node = parent;
            continue;
        }
        else if (findFavorite(parent) == NULL) { //case 2
            printf("parent has no favorite\n");
            setBalance(parent);
            break;
        }
        else {
            BSTNODE *fav = findFavorite(sibling);
            if (fav != sibling && isLinear(node, parent, fav) == 0) { // case 3
                rotate(t, fav, sibling);
                rotate(t, fav, parent);
                setBalance(parent);
                setBalance(sibling);
                setBalance(fav);
                node = fav;
                continue;
            }
            else { //case 4
                rotate(t, sibling, parent);
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
    return sizeBST(a->bstree);
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
