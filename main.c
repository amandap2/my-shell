#include "shell.h"
#include "ui.h"
#include "parser.h"
#include "executor.h"
#include "builtin.h"

int main() {
    char input[MAX_INPUT];
    char *args1[MAX_ARGS], *args2[MAX_ARGS];

    printBanner();

    while (1) {
        printDir();
        takeInput(input);

        if (strlen(input) == 0) continue;

        char *left, *right;

        if (parsePipe(input, &left, &right)) {
            parseInput(left,  args1);
            parseInput(right, args2);
            executePipe(args1, args2);
        } else {
            parseInput(input, args1);
            if (handleBuiltIn(args1)) continue;
            executeCommand(args1);
        }
    }

    return 0;
}