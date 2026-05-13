#include "shell.h"
#include "builtin.h"
#include "raspberry.h"

int handleBuiltIn(char **args) {
    if (args[0] == NULL) return 1;

    if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL)
            printf("Expected argument to \"cd\"\n");
        else if (chdir(args[1]) != 0)
            perror("cd failed");
        return 1;
    }

    if (strcmp(args[0], "help") == 0) {
        printf("\nSimple Shell Help\n");
        printf("Built-in commands:\n");
        printf("  cd <dir>              change directory\n");
        printf("  exit                  exit shell\n");
        printf("  help                  show this menu\n");
        printf("  start chatbot         start housebot service\n");
        printf("  restart chatbot       restart housebot service\n");
        printf("  status chatbot        show housebot status\n");
        printf("  stop chatbot          stop housebot service\n");
        printf("Supports pipes: ls | grep txt\n");
        return 1;
    }

    if (strcmp(args[0], "start")   == 0 && args[1] && strcmp(args[1], "chatbot") == 0) { startChatBot();   return 1; }
    if (strcmp(args[0], "restart") == 0 && args[1] && strcmp(args[1], "chatbot") == 0) { restartChatBot(); return 1; }
    if (strcmp(args[0], "status")  == 0 && args[1] && strcmp(args[1], "chatbot") == 0) { chatBotStatus();  return 1; }
    if (strcmp(args[0], "stop")    == 0 && args[1] && strcmp(args[1], "chatbot") == 0) { stopChatBot();    return 1; }

    return 0;
}
