#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"
#include "avl.h"
#include "gst.h"
#include "bst.h"
#include "string.h"


char *processString(int, FILE *);
char *cleanString(char *);
void processInsertGST(char *, GST *);
void processInsertAVL(char *, AVL *);
void processCommandsAVL(AVL *, char *, FILE *fp);
void processCommandsGST(GST *, char *, FILE *fp);

int main (int argc, char **argv) {
    int tree = 0;
    FILE *data;
    FILE *commands;
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        printf("Author: Ben Bailey\n");
        return 0;
    }
    else {
        // printf("TREE: %d\n", argv[1][1]);
        switch (argv[1][1]) {
            case 103:
                // printf("gst!\n");
                tree = 1;
                break;
            case 114:
                // printf("avl!\n");
                tree = 0;
                break;
        }
        commands = fopen(argv[argc-1], "r");
        data = fopen(argv[argc-2], "r");
    }

    if (tree == 0) { //AVL tree
        AVL *avl = newAVL(displaySTRING, compareSTRING, freeSTRING);
        // printf("AVL!\n");

        int str = stringPending(data);
        char *temp = processString(str, data);
        processInsertAVL(temp, avl);

        str = stringPending(data);
        while (!feof(data)) {
            temp = processString(str, data);
            processInsertAVL(temp, avl);
            str = stringPending(data);
        }

        str = stringPending(commands);
        temp = processString(str, commands);
        if (temp [0] != 32) {
            processCommandsAVL(avl, temp, commands);
        }
        str = stringPending(commands);
        while (!feof(commands)) {
            temp = processString(str, commands);
            if (temp [0] != 32) {
                processCommandsAVL(avl, temp, commands);
            }
            str = stringPending(commands);
        }
    }
    else { //Green tree
        GST *gst = newGST(displaySTRING, compareSTRING, freeSTRING);
        // printf("GST!\n");

        int str = stringPending(data);
        char *temp = processString(str, data);
        // printf("TEMP after cleaning: %s\n", temp);
        processInsertGST(temp, gst);

        str = stringPending(data);
        while (!feof(data)) {
            // STRING *new = newSTRING(temp);
            // insertGST(gst, new);

            temp = processString(str, data);
            // if (temp) {
            //     printf("TEMP after cleaning: %s\n", temp);
            // }
            processInsertGST(temp, gst);
            str = stringPending(data);
        }

        // printf("out of the first while\ntree looks like: \n");
        // displayGST(gst, stdout);
        // printf("\n\n");

        str = stringPending(commands);
        temp = processString(str, commands);
        // printf("TEMP after cleaning: %s\n", temp);
        if (temp [0] != 32) {
            // printf("going to process commands\n");
            processCommandsGST(gst, temp, commands);
        }

        str = stringPending(commands);
        while (!feof(commands)) {
            temp = processString(str, commands);
            if (temp [0] != 32) {
                // printf("going to process commands\n");
                processCommandsGST(gst, temp, commands);
            }
            str = stringPending(commands);
        }
    }

    return 0;
}

/********************************************/

void processInsertGST(char *str, GST *tree) {
    if (isalpha(str[0]) == 1) {
        // printf("inserting %s\n\n", str);
        STRING *new = newSTRING(str);
        insertGST(tree, new);
        return;
    }
    else {
        // printf("NOT INSERTING %s\n\n", str);
        return;
    }
}

void processInsertAVL(char *str, AVL *tree) {
    if (isalpha(str[0]) == 1) {
        // printf("inserting %s\n\n", str);
        STRING *new = newSTRING(str);
        insertAVL(tree, new);
        return;
    }
    else {
        // printf("NOT INSERTING %s\n\n", str);
        return;
    }
}

char *processString(int x, FILE *fp) {
    char *str;
    if (x == 0) {
        // printf("not a double quote\n");
        str = readToken(fp);
        str = cleanString(str);
        return str;
    }
    else {
        // printf("double quote\n");
        str = readString(fp);
        str = cleanString(str);
        return str;
    }
}

char * cleanString(char *str) {
    char *returnable = str;


    int size = sizeof(str)/sizeof(char *);
    int pos = 0;
    // printf("STR SIZE : %d\nSTRING: %s\n", size, str);
    // if (str[0] < 32) {
    //     printf("RETURNING NULL\n");
    //     return NULL;
    // }

    for (int i=0; i<(size); i++) {
        if (str[i] >= 97 && str[i] <= 122) {
            // printf("lowercase\n");
            returnable[pos] = str[i];
            pos ++;
        }
        else if (str[i] >= 65 && str[i] <= 90) {
            // printf("uppercase\n");
            returnable[pos] = str[i] + 32;
            pos ++;
        }
        else if (str[i] == 32) {
            // printf("space\n");
            returnable[pos] = str[i];
            pos ++;
        }
        else if (isalpha(str[i]) == 0) {
            // printf("not alphabetical\n");
            continue;
        }
    }
    return returnable;
}

void processCommandsAVL(AVL *tree, char *temp, FILE *fp) {
    int str = 0;
    switch (temp[0]) {
        case 105: //insert
            str = stringPending(fp);
            temp = processString(str, fp);
            STRING *insert = newSTRING(temp);
            insertAVL(tree, insert);
            break;
        case 100: //deleteAVL
            str = stringPending(fp);
            temp = processString(str, fp);
            STRING *delete = newSTRING(temp);
            deleteAVL(tree, delete);
            break;
        case 102: //frequency
            str = stringPending(fp);
            temp = processString(str, fp);
            STRING *freq = newSTRING(temp);
            int count = findAVLcount(tree, freq);
            printf("Frequency of ");
            displaySTRING(freq, stdout);
            printf(" is %d\n", count);
            break;
        case 115: //show
        if (sizeAVL(tree) == 0) {
            printf("EMPTY\n");
            break;
        }
        else {
            displayAVL(tree, stdout);
            break;
        }
        case 114: //statistics
            statisticsAVL(tree, stdout);
            break;
    }
    return;
}

void processCommandsGST(GST *tree, char *temp, FILE *fp) {
    // printf("PROCESSING COMMANDS GST with %s\n", temp);
    int str = 0;
    switch (temp[0]) {
        case 105: //insert
            str = stringPending(fp);
            temp = processString(str, fp);
            STRING *insert = newSTRING(temp);
            insertGST(tree, insert);
            break;
        case 100: //deleteAVL
            // printf("delete\n");
            str = stringPending(fp);
            // printf("result of pending: %d\n", str);
            temp = processString(str, fp);
            // printf("TEMP after cleaning: %s\n", temp);
            STRING *delete = newSTRING(temp);
            deleteGST(tree, delete);
            break;
        case 102: //frequency
            str = stringPending(fp);
            temp = processString(str, fp);
            STRING *freq = newSTRING(temp);
            int count = findGSTcount(tree, freq);
            printf("Frequency of ");
            displaySTRING(freq, stdout);
            printf(" is %d\n", count);
            break;
        case 115: //show
            if (sizeGST(tree) == 0) {
                printf("EMPTY\n");
                break;
            }
            else {
                displayGST(tree, stdout);
                break;
            }
        case 114: //statistics
            statisticsGST(tree, stdout);
            break;
    }
    return;
}
