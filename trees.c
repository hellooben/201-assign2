#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"
#include "integer.h"
#include "bst.h"
#include "heap.h"
#include "scanner.h"
#include "real.h"
#include "string.h"
#include "options.c"

int main (int argc, char **argv) {
    int tree = 0;
    FILE *data;
    FILE *commands;
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        printf("Author: Ben Bailey\n");
        return 0;
    }
    else {
        switch (argv[1]) {
            case '-g':
                tree = 1;
            case '-r':
                tree = 0;
        }
        commands = fopen(argv[argc-1], "r");
        data = fopen(argv[argc-2], "r");
    }

    if (tree == 0) { //AVL tree

    }
    else { //Green tree
        
    }







    return 0;
}
