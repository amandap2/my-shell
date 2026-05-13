#include "shell.h"
#include "executor.h"

void executeCommand(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) < 0)
            perror("command failed");
        exit(1);
    } else {
        wait(NULL);
    }
}

void executePipe(char **args1, char **args2) {
    int pipefd[2];
    pipe(pipefd);

    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args1[0], args1);
        perror("pipe cmd1 failed");
        exit(1);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        execvp(args2[0], args2);
        perror("pipe cmd2 failed");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);
}
