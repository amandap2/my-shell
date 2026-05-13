#include "shell.h"
#include "ui.h"

void printBanner() {
    printf("\n************************************************\n\n");
    printf("  **************** My Shell ****************\n\n");
    printf("  \"Focus beats talent when talent doesn't focus.\"  \n\n");
}

void printDir() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        return;
    }
    printf("\nDir: %s", cwd);
}

void takeInput(char *input) {
    printf("\n>>> ");
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        printf("\nExiting...\n");
        exit(0);
    }
    input[strcspn(input, "\n")] = 0;
}