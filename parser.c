#include "shell.h"
#include "parser.h"

int parsePipe(char *input, char **left, char **right) {
    char *pipePos = strchr(input, '|');
    if (!pipePos) return 0;

    *pipePos = '\0';
    pipePos++;
    while (*pipePos == ' ') pipePos++;

    *left  = input;
    *right = pipePos;
    return 1;
}

void parseInput(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}
