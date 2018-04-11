/*** AVL binary search tree class ***/

#ifndef __AVL_INCLUDED__
#define __AVL_INCLUDED__

#include <stdio.h>

typedef struct avl AVL;

extern AVL *newAVL(
    void (*)(void *,FILE *),           //display
    int (*)(void *,void *),            //comparator
    void (*)(void *));                 //freeing function
extern void insertAVL(AVL *,void *);
extern int findAVLcount(AVL *,void *);
extern void *findAVL(AVL *,void *);
extern void *deleteAVL(AVL *,void *);
extern int sizeAVL(AVL *);
extern int duplicatesAVL(AVL *);
extern void statisticsAVL(AVL *,FILE *);
extern void displayAVL(AVL *,FILE *);
extern void displayAVLdebug(AVL *,FILE *);
extern void freeAVL(AVL *);

#endif
/*
AVAL *temp = getBSTNODEvalue(node);
temp->display(getAVAL(temp), stdout);
printf("\n");


function deleteFixup(x)
        {
        set the height of x to zero //since it will be deleted
        loop
            {
            if (x is the root)
                exit the loop
            else if (parent favors x)                  //case 1
                {
                set the balance of parent
                x = parent
                //continue looping
                }
            else if (parent has no favorite)           //case 2
                {
                set the balance of parent
                exit the loop
                }
            else
                {
                p = parent of x
                z = the sibling of x
                y = favorite of z
                if (y exists and y,z,p are not linear) //case 3
                    {
                    rotate y to z
                    rotate y to p
                    set the balance of p
                    set the balance of z
                    set the balance of y
                    x = y
                    //continue looping
                    }
                else
                    {
                    rotate z to p                      //case 4
                    set the balance of p
                    set the balance of z
                    if (y does not exist)
                        exit the loop
                    x = z
                    //continue looping
                    }
                }
            }
        }

function insertionFixup(x)
    {
    loop
        {
        if (x is the root)
            exit the loop
        else if (parent favors the sibling)        //case 1
            {
            set the balance of parent
            exit the loop
            }
        else if (parent has no favorite)           //case 2
            {
            set the balance of parent
            x = parent
            //continue looping
            }
        else //parent favors x
            {
            y = the favorite child of x
            p = parent of x
            if (y exists and y,x,p are not linear) //case 3
                {
                rotate y to x
                rotate y to p
                set the balance of x
                set the balance of p
                set the balance of y
                }
            else                                   //case 4
                {
                rotate x to p
                set the balance of p
                set the balance of x
                }
            exit the loop
            }
        }
    }
*/
