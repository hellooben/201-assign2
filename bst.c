#include "bst.h"
#include "sll.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

BSTNODE *binaryInsert(BST *t, BSTNODE *node, BSTNODE *prev, void *value);
BSTNODE *binaryFind(BST *t, BSTNODE *node, void *value);
void displayRecursive(BST *t, BSTNODE *root, FILE *fp);
void freeRecursive(BSTNODE *node, BST *t);
void levelOrder(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq);
int BSTmaximum(BSTNODE *root);
int BSTminimum(BSTNODE *root);
void levelOrderDecorated(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq);
int isLeft(BST *t, BSTNODE *node, BSTNODE *parent);

struct bstnode
{
    void *data;
    struct bstnode *parent;
    struct bstnode *left;
    struct bstnode *right;
};

extern BSTNODE *
newBSTNODE(void *v) {
    BSTNODE *new = malloc(sizeof(BSTNODE));
    new->data = v;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

extern void *
getBSTNODEvalue(BSTNODE *n) {
    return n->data;
}

extern void
setBSTNODEvalue(BSTNODE *n,void *value) {
    n->data = value;
    return;
}

extern BSTNODE *
getBSTNODEleft(BSTNODE *n) {
    if (n->left != NULL) {
        return n->left;
    }
    else {
        return NULL;
    }
}

extern void
setBSTNODEleft(BSTNODE *n,BSTNODE *replacement) {
    n->left = replacement;
        //replacement->parent = n;
        //replacement->right = getBSTNODEright(curr);
        //replacement->left = getBSTNODEleft(curr);
    return;
}

extern BSTNODE *
getBSTNODEright(BSTNODE *n) {
    if (n->right != NULL) {
        return n->right;
    }
    else {
        return NULL;
    }
}

extern void
setBSTNODEright(BSTNODE *n,BSTNODE *replacement) {
    n->right = replacement;
        //replacement->parent = n;
        //replacement->right = getBSTNODEright(curr);
        //replacement->left = getBSTNODEleft(curr);
    return;
}

extern BSTNODE *
getBSTNODEparent(BSTNODE *n) {
    // if (n->parent != NULL) {
    //     return n->parent;
    // }
    // else {
    //     return NULL;
    // }
    return n->parent;
}

extern void
setBSTNODEparent(BSTNODE *n,BSTNODE *replacement) {
    n->parent = replacement;
        //replacement->parent = getBSTNODEparent(curr);
        //replacement->right = getBSTNODEright(curr);
        //replacement->left = getBSTNODEleft(curr);
    return;
}

extern void
freeBSTNODE(BSTNODE *n,void (*f)(void *)) {
    if (f != NULL) {
        f(n->data);
        free(n);
    }
    return;
}


/*----------------------------------------------------------------------*/


struct bst
{
    //QUEUE *list;
    int size;
    BSTNODE *root;
    void (*display)(void *,FILE *);          //display
    int (*compare)(void *,void *);           //comparator
    void (*swap)(BSTNODE *,BSTNODE *);     //swapper
    void (*free)(void *);                  //free
};

extern BST *
newBST(void (*d)(void *,FILE *),
int (*c)(void *,void *),
void (*s)(BSTNODE *,BSTNODE *),
void (*f)(void *)) {
    BST *tree = malloc(sizeof(BST));
    assert(tree!=0);
    //tree->list = newQUEUE(d, f);
    tree->size = 0;
    tree->root = NULL;
    tree->display = d;
    tree->compare = c;
    tree->swap = s;
    tree->free = f;
    return tree;
}

extern BSTNODE *
getBSTroot(BST *t) {
    if (t->root != NULL) {
        return t->root;
    }
    else {
        return NULL;
    }
}

extern void
setBSTroot(BST *t,BSTNODE *replacement) {
    t->root = replacement;
    return;
}

extern void
setBSTsize(BST *t,int s) {
    t->size = s;
    return;
}

extern BSTNODE *
insertBST(BST *t,void *value) {
    // printf("in insert\n");
    if (t->root == NULL) {
        // printf("root is null. SETTING ROOT\n");
        BSTNODE *new = newBSTNODE(value);
        assert(new!=0);
        t->root = new;
        setBSTNODEparent(t->root, new);
        t->size ++;
        //enqueue(t->list, new);
        return new;
    }
    else {
        // printf("root is not null!\n");
        //BSTNODE *curr = t->root;

        BSTNODE *returnable = binaryInsert(t, t->root, t->root, value);
        assert(returnable!=0);
        t->size ++;
        //enqueue(t->list, returnable);
        // printf("BST INSRT returning this: ");
        // t->display(getBSTNODEvalue(returnable), stdout);
        // printf("\n");
        return returnable;
    }
}

BSTNODE *
binaryInsert(BST *t, BSTNODE *node, BSTNODE *prev, void *value) {
    // BSTNODE *new = newBSTNODE(value);
    // printf("IN BINARY INSERT\n");
    if (node == NULL) {
        BSTNODE *new = newBSTNODE(value);
        new->parent = prev;
        if (t->compare(prev->data, value) < 0) {
            // printf("setting right\n");
            prev->right = new;
        }
        else {
            // printf("setting left\n");
            prev->left = new;
        }
        // printf("NULL NODE, setting the parent\nreturning this: ");
        // t->display(getBSTNODEvalue(new), stdout);
        // printf("\n");
        return new;
    }
    if (t->compare(node->data, value) < 0) {
        // printf("going right!\n");
        return binaryInsert(t, node->right, node, value);
        // node->right = binaryInsert(t, node->right, node, value);
        // return node;
        // return node->right;
    }
    else if (t->compare(node->data, value) > 0) {
        // printf("going left!\n");
        return binaryInsert(t, node->left, node, value);
        // node->left = binaryInsert(t, node->left, node, value);
        // return node;
        // return node->left;
    }
    else {
        // printf("returning outside\n");

        return node;
    }
    // printf("returning outside\n");
    // else {
    //     printf("inside of the last else somehow\n");
    //     return node;}

}

extern BSTNODE *
findBST(BST *t,void *value) {
    if (t->root == NULL) {
        return NULL;
    }
    else {
        BSTNODE *curr = t->root;
        // printf("about to binary find\n");
        BSTNODE *returnable = binaryFind(t, curr, value);
        return returnable;
    }
}

BSTNODE *
binaryFind(BST *t, BSTNODE *node, void *value) {
    // printf("IN BINARY FIND\n");
    if (node == NULL) {
        // printf("NODE IS NULL!\n");
        return node;
    }
    // printf("LOOKING FOR : ");
    // t->display(value, stdout);
    // printf("\n");
    // printf("COMPARING : ");
    // t->display(getBSTNODEvalue(node), stdout);
    // printf(" and \n");
    // t->display(value, stdout);
    // printf("\n");
    // printf("COMPARE RESULT: %d\n", t->compare(node->data, value));
    if (t->compare(node->data, value) == 0) {
        return node;
    }
    else if (t->compare(node->data, value) > 0) {
        // printf("looking at: ");
        // t->display(node->data, stdout);
        // printf(". went left\n");
        return binaryFind(t, node->left, value);
    }
    else if (t->compare(node->data, value) < 0) {
        // printf("looking at: ");
        // t->display(node->data, stdout);
        // printf(". went right\n");
        return binaryFind(t, node->right, value);
    }
    else {
        return NULL;
    }
}

extern BSTNODE *
deleteBST(BST *t,void *value) {
    BSTNODE *current = findBST(t, value);
    //printf("found\n");
    //printf("OLD BST: \n");
    //displayBSTdebug(t, stdout);
    current = swapToLeafBST(t, current);
    //printf("after swapping ");
    //t->display(current->data, stdout);
    //printf(" to a leaf\n");
    //printf("NEW BST: \n");
    //displayBSTdebug(t, stdout);

    //printf("swapped\n");

    pruneLeafBST(t, current);
    t->size --;
    //printf("BST after ");
    // t->display(current->data, stdout);
    // printf(" was pruned: \n");
    //displayBSTdebug(t, stdout);

    return current;
}

extern BSTNODE *
swapToLeafBST(BST *t,BSTNODE *node) {
    //printf("in swap\n");
    if (t->size == 1) {
        return node;
    }
    else if (node->left == NULL && node->right == NULL) {
        // t->display(node->data, stdout);
        // printf(" was swapped to a leaf\n");
        return node;
    }
    else {
        BSTNODE *temp = node;
        if (node->right != NULL) {
            //void *data = getBSTNODEvalue(node);
            //setBSTNODEvalue(node, getBSTNODEvalue(node->right));
            //setBSTNODEvalue(node->right, data);
            node = node->right;
            while (node->left != NULL) {
                //void *data = getBSTNODEvalue(node);
                //setBSTNODEvalue(node, getBSTNODEvalue(node->left));
                //setBSTNODEvalue(node->left, data);
                node = node->left;
            }
        }
        else {
            //void *data = getBSTNODEvalue(node);
            //setBSTNODEvalue(node, getBSTNODEvalue(node->left));
            //setBSTNODEvalue(node->left, data);
            node = node->left;
            while (node->right != NULL) {
                //void *data = getBSTNODEvalue(node);
                //setBSTNODEvalue(node, getBSTNODEvalue(node->right));
                //setBSTNODEvalue(node->right, data);
                node = node->right;
            }
        }
        if (t->swap) {
            t->swap(temp, node);
        }
        else {
            void *data = getBSTNODEvalue(temp);
            setBSTNODEvalue(temp, getBSTNODEvalue(node));
            setBSTNODEvalue(node, data);
        }
        return swapToLeafBST(t, node);
    }
}

extern void
pruneLeafBST(BST *t,BSTNODE *leaf) {
    // printf("in prune\n");
    if (getBSTNODEleft(leaf) || getBSTNODEright(leaf)) {
        // printf("this isnt a leaf. you fucked up\n");
        return;
    }
    else {
        // printf("it's a leaf\n");
        if (leaf->parent == leaf) {
            // printf("NULLED OUT ROOT\n");
            t->root = NULL;
            return;
        }
        // printf("hello\n");
        // t->display(leaf->data, stdout);
        // printf("'s parent isnt null\n");
        BSTNODE *par = getBSTNODEparent(leaf);
        //printf("got parent: ");
        //t->display(par->data, stdout);
        //printf("\n");
        if (par->left != NULL) {
            //printf("left isnt null\n");
            if (t->compare(getBSTNODEvalue(par->left), getBSTNODEvalue(leaf)) == 0) {
                //printf("its a left leaf\n");
                par->left = NULL;
                leaf->parent = NULL;
                return;
            }
        }
        if (par->right != NULL) {
            //printf("it's a right leaf\n");
            par->right = NULL;
            leaf->parent = NULL;
            return;
        }
        //leaf->parent = NULL;
        return;
    }
}

extern int
sizeBST(BST *t) {
    return t->size;
}

extern void
statisticsBST(BST *t,FILE *fp) {
    BSTNODE *current = t->root;
    if (current == NULL) {
        fprintf(fp, "Nodes: %d\nMinimum depth: %d\nMaximum depth: %d\n", 0, -1, -1);
        return;
    }
    else {
        // printf("gonna find the min and max\n");
        int min = BSTminimum(t->root);
        int max = BSTmaximum(t->root);
        // printf("found the min and max\n");
        fprintf(fp, "Nodes: %d\nMinimum depth: %d\nMaximum depth: %d\n", sizeBST(t), min, max);

        return;
    }
}

int
BSTminimum(BSTNODE *root) {
    if (root == NULL) {
        return -1;
    }
    else {
        int leeft = BSTminimum(root->left);
        int reet = BSTminimum(root->right);

        if (leeft < reet) {
            return leeft + 1;
        }
        else {
            return reet + 1;
        }
    }
}

int
BSTmaximum(BSTNODE *root) {
    if (root == NULL) {
        return -1;
    }
    else {
        int leeft = BSTmaximum(root->left);
        int reet = BSTmaximum(root->right);

        if (leeft > reet) {
            return leeft + 1;
        }
        else {
            return reet + 1;
        }
    }
}

extern void
displayBST(BST *t,FILE *fp) {
    if (t->root == NULL) {
        printf("[]");
        return;
    }
    else {
        displayRecursive(t, t->root, fp);
        return;
    }
}

extern void
displayBSTdebug(BST *t,FILE *fp) {
    if (t->root == NULL) {
        return;
    }
    else {
        QUEUE *newq = newQUEUE(t->display, t->free);
        QUEUE *oldq = newQUEUE(t->display, t->free);
        levelOrder(t, t->root, fp, newq, oldq);
        freeQUEUE(newq);
        freeQUEUE(oldq);
        return;
    }
}

void
levelOrder(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq) {
    int j = 0;
    t->display(node->data, fp);
    printf("\n");
    j ++;
    enqueue(oldq, node);

    while (j<sizeBST(t)) {
        int oldsize = sizeQUEUE(oldq);
        for (int i=0; i<oldsize; i++) {
            node = dequeue(oldq);
            if (node->left != NULL) {
                enqueue(newq, node->left);
            }
            if (node->right != NULL) {
                enqueue(newq, node->right);
            }
        }
        int newsize = sizeQUEUE(newq);
        for (int x=0; x<newsize; x++) {
            node = dequeue(newq);
            t->display(node->data, fp);
            enqueue(oldq, node);
            if (x < newsize-1) {printf(" ");}
            j ++;
        }
        printf("\n");
    }
    while (sizeQUEUE(oldq) > 0) {
    // for (int i=0; i<=sizeQUEUE(oldq); i++) {
        // printf("soize: %d\n", sizeQUEUE(oldq));
        dequeue(oldq);
    }
    return;
}

extern void
displayBSTdecorated(BST *t,FILE *fp) {
    if (t->root == NULL) {
        return;
    }
    else {
        QUEUE *newq = newQUEUE(t->display, t->free);
        QUEUE *oldq = newQUEUE(t->display, t->free);
        levelOrderDecorated(t, t->root, fp, newq, oldq);
        // printf("SIZES: %d, %d\n", sizeQUEUE(newq),sizeQUEUE(oldq));
        freeQUEUE(newq);
        freeQUEUE(oldq);
        return;
    }
}

void
levelOrderDecorated(BST *t, BSTNODE *node, FILE *fp, QUEUE *newq, QUEUE *oldq) {
    int j = 0;
    int lines = 0;
    fprintf(fp, "%d: ", lines);
    t->display(node->data, fp);
    fprintf(fp, "(");
    t->display(getBSTNODEparent(node)->data, fp);
    fprintf(fp, ")");
    fprintf(fp, "X");
    fprintf(fp, "\n");
    j ++;
    lines ++;
    enqueue(oldq, node);

    while (j<sizeBST(t)) {
        int oldsize = sizeQUEUE(oldq);
        for (int i=0; i<oldsize; i++) {
            node = dequeue(oldq);
            if (node->left != NULL) {
                enqueue(newq, node->left);
            }
            if (node->right != NULL) {
                enqueue(newq, node->right);
            }
        }
        int newsize = sizeQUEUE(newq);
        fprintf(fp, "%d: ", lines);
        for (int x=0; x<newsize; x++) {
            node = dequeue(newq);
            if (getBSTNODEleft(node)==NULL && getBSTNODEright(node)==NULL) {
                fprintf(fp, "=");
            }
            t->display(node->data, fp);
            fprintf(fp, "(");
            t->display(getBSTNODEvalue(getBSTNODEparent(node)), fp);
            fprintf(fp, ")");
            if (isLeft(t, node, getBSTNODEparent(node)) == 1) {
                fprintf(fp, "L");
            }
            else {
                fprintf(fp, "R");
            }
            enqueue(oldq, node);
            if (x < newsize-1) {fprintf(fp, " ");}
            j ++;
        }
        fprintf(fp, "\n");
        lines ++;
    }
    while (sizeQUEUE(oldq) > 0) {
    // for (int i=0; i<=sizeQUEUE(oldq); i++) {
        // printf("soize: %d\n", sizeQUEUE(oldq));
        dequeue(oldq);
    }
    return;

}

int
isLeft(BST *t, BSTNODE *node, BSTNODE *parent) {
    BSTNODE *maybe = getBSTNODEleft(parent);
    if (maybe != NULL && t->compare(getBSTNODEvalue(node),getBSTNODEvalue(maybe)) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

extern void freeBST(BST *t) {
    if (t->root == NULL) {
        free(t);
        return;
    }
    else {
        // printf("in free, queue size is %d, tree size is %d\n", sizeQUEUE(t->list), sizeBST(t));
        // displayQUEUE(t->list, stdout);
        // printf("\n");
        // printf("IN FREE\n");
        BSTNODE *curr = t->root;
        freeRecursive(curr, t);
        //freeQUEUE(t->list);
        //printf("freed queue\n");
        free(t);
        return;
    }
}

void
freeRecursive(BSTNODE *node, BST *t) {
    if (node == NULL) {
        return;
    }
    BSTNODE *temp = node;
    if (temp->right != NULL && t->free != NULL) {
        // t->display(getBSTNODEvalue(temp), stdout);
        // printf("'s right is present'\n");
        freeRecursive(getBSTNODEright(temp), t);
    }
    if (temp->left != NULL && t->free != NULL) {
        // t->display(getBSTNODEvalue(temp), stdout);
        // printf("'s left is present'\n");
        freeRecursive(getBSTNODEleft(temp), t);
    }
    if (t->free != NULL) {
        // t->display(getBSTNODEvalue(temp), stdout);
        // printf("\n");
        freeBSTNODE(temp, t->free);
    }
}

void
displayRecursive(BST *t, BSTNODE *root, FILE *fp) {
    if (root == NULL) {
        printf("[]");
        return;
    }

    printf("[");
    t->display(root->data, fp);

    if (root->left != NULL) {
        printf(" ");
        displayRecursive(t, root->left, fp);
    }
    if (root->right != NULL) {
        printf(" ");
        displayRecursive(t, root->right, fp);
    }
    printf("]");
}
