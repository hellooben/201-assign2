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
void cleanString(char *);
void cleanToken(char *);
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
        switch (argv[1][1]) {
            case 103:
                tree = 1;
                break;
            case 114:
                tree = 0;
                break;
        }
        commands = fopen(argv[argc-1], "r");
        data = fopen(argv[argc-2], "r");
    }

    if (tree == 0) { //AVL tree
        AVL *avl = newAVL(displaySTRING, compareSTRING, freeSTRING);

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

        int str = stringPending(data);
        char *temp = processString(str, data);
        processInsertGST(temp, gst);

        str = stringPending(data);
        while (!feof(data)) {
            temp = processString(str, data);
            processInsertGST(temp, gst);
            str = stringPending(data);
        }

        str = stringPending(commands);
        temp = processString(str, commands);
        processCommandsGST(gst, temp, commands);

        str = stringPending(commands);
        while (!feof(commands)) {
            temp = processString(str, commands);
            processCommandsGST(gst, temp, commands);
            str = stringPending(commands);
        }
    }
    return 0;
}

/********************************************/

void processInsertGST(char *str, GST *tree) {
    if (isalpha(str[0])) {
        STRING *new = newSTRING(str);
        insertGST(tree, new);
        return;
    }
    else {
        return;
    }
}

void processInsertAVL(char *str, AVL *tree) {
    if (isalpha(str[0])) {
        STRING *new = newSTRING(str);
        insertAVL(tree, new);
        return;
    }
    else {
        return;
    }
}

char *processString(int x, FILE *fp) {
    char *str;
    if (x == 0) {
        str = readToken(fp);
        cleanToken(str);
        return str;
    }
    else {
        str = readString(fp);
        cleanString(str);
        return str;
    }
}

void cleanToken(char *str) {
    int size = strlen(str);
    int pos = 0;
    for (int i=0; i<size; i++) {
        if (isalpha(str[i])) {
            if (isupper(str[i])) {
                str[pos] = str[i] + 32;
            }
            else {
                str[pos] = str[i];
            }
            pos ++;
        }
        else continue;
    }
    str[pos] = '\0';
    return;
}

void cleanString(char *str) {
    int size = strlen(str);
    int pos = 0;
    int space = 0;
    for (int i=0; i<size; i++) {
        if (str[i] == 32) {
            if (space != 0) {
                space = 0;
                str[pos] = str[i];
                pos ++;
            }
            else continue;
        }
        else if (isalpha(str[i])) {
            if (isupper(str[i])) {
                str[pos] = str[i] + 32;
            }
            else {
                str[pos] = str[i];
            }
            space = 1;
            pos ++;
        }
        else continue;
    }

    if (isspace(str[pos-1])) {
        str[pos-1] = '\0';
    }
    else {
        str[pos] = '\0';
    }
    return;
}

void processCommandsAVL(AVL *tree, char *temp, FILE *fp) {
    int str = 0;
    int num = temp[0];

    if (num == 105) {
        str = stringPending(fp);
        temp = processString(str, fp);
        processInsertAVL(temp, tree);
    }
    else if (num == 100) {
        str = stringPending(fp);
        temp = processString(str, fp);
        STRING *delete = newSTRING(temp);
        void *del = findAVL(tree, delete);
        if (del == NULL && isalpha(temp[0])) {
            printf("Value ");
            displaySTRING(delete, stdout);
            printf(" not found.\n");
        }
        else {
            del = deleteAVL(tree, delete);
        }
    }
    else if (num == 102) {
        str = stringPending(fp);
        temp = processString(str, fp);
        STRING *freq = newSTRING(temp);
        int count = findAVLcount(tree, freq);
        printf("Frequency of ");
        displaySTRING(freq, stdout);
        printf(": %d\n", count);
    }
    else if (num == 115) {
        displayAVL(tree, stdout);
    }
    else if (num == 114) {
        statisticsAVL(tree, stdout);
    }
    return;
}

void processCommandsGST(GST *tree, char *temp, FILE *fp) {
    int str = 0;
    int num = temp[0];

    if (num == 105) {
        str = stringPending(fp);
        temp = processString(str, fp);
        processInsertGST(temp, tree);
    }
    else if (num == 100) {
        str = stringPending(fp);
        temp = processString(str, fp);
        STRING *delete = newSTRING(temp);
        void *del = findGST(tree, delete);
        if (del == NULL && isalpha(temp[0])) {
            printf("Value ");
            displaySTRING(delete, stdout);
            printf(" not found.\n");
        }
        else {
            del = deleteGST(tree, delete);
        }
    }
    else if (num == 102) {
        str = stringPending(fp);
        temp = processString(str, fp);
        STRING *freq = newSTRING(temp);
        int count = findGSTcount(tree, freq);
        printf("Frequency of ");
        displaySTRING(freq, stdout);
        printf(": %d\n", count);
    }
    else if (num == 115) {
        displayGST(tree, stdout);
    }
    else if (num == 114) {
        statisticsGST(tree, stdout);
    }
    return;
}
