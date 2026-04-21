/*
stdlib.h: responsible for memory allocation/deallocation manipulation
Example: NULL: indicates that the pointer its not poiting to any place in the memory

string.h: string manipulation
*/


#include <stdio.h> //standard input-output header
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024 // MAX length user input
#define MAX_ARGS 64    // MAX number of arguments

void printBanner(){
    printf(
        "\n************************************************\n\n"
    );

    printf(
        "  **************** My Shell ****************\n\n"
    );

    printf(
        "  \"Focus beats talent when talent doesn't focus.\"  \n\n"
    );
}

void printDir(){
    char cwd[1024];

    //get current working directory
    //if it fails, print error and return
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd failed"); //print system error
        return; //exit function safely
    }

    //display current directory path
    printf("\nDir: %s", cwd);
}

void takeInput(char *input){
    printf("\n>>> ");  //Prompt

    if(fgets(input, MAX_INPUT, stdin) == NULL){
        //if input fails (e.g., )ctrl + D), exit shell
        printf("\nExiting...\n");
        exit(0);
    }

    //Remove trailing newline character
    input[strcspn(input, "\n")] = 0;
}

int parsePipe(char *input, char **left, char **rigth){
    char *pipePos = strchr(input, '|'); //find pipe symbol

    if(!pipePos) return 0; //no pipe found;

    *pipePos = '\0'; //split string at pipe
    pipePos++; // move to right side

    while(*pipePos == ' ') pipePos++;

    *left = input; //left command
    *rigth = pipePos; //rigth command    *left = input; //left command


    return 1; //pipe exists
}

void parseInput(char *input, char **args){
    int i = 0;

    //tokenize input based on space
    args[i] = strtok(input, " ");

    //continue splitting until null or max args reached
    while (args[i] != NULL && i < MAX_ARGS - 1)
    {
        i++;

        args[i] = strtok(NULL, " ");
    }
    
}

void executePipe(char **args1, char **args2){
    int pipefd[2];
    pipe(pipefd); //create pipe

    pid_t p1 = fork();

    if(p1 == 0){
        //first child process (left command)
        dup2(pipefd[1], STDOUT_FILENO); //redirect stdout to pipe write end
        close(pipefd[0]); // close unused read end
        close(pipefd[1]);

        execvp(args1[0], args1); //execute first command
        perror("pipe cmd1 failed");
        exit(1);
    }

    pid_t p2 = fork();

    if(p2 == 0){
        //second child process (right command)
        dup2(pipefd[0], STDIN_FILENO); //redirect stdin to pipe read end
        close(pipefd[1]); //close unused write end
        close(pipefd[0]);

        execvp(args2[0], args2); //execute second command
        perror("Pipe cmd2 failed");
        exit(1);
    }

    //parent process closes both ends of pipe
    close(pipefd[0]);
    close(pipefd[1]);

    //wait for both child processes to complete
    wait(NULL);
    wait(NULL);
}

int handleBuiltIn(char **args){
    if (args[0] == NULL) return 1; //empty command

    //exit command
    if(strcmp(args[0], "exit") == 0){
        printf("Exiting shell...\n");
        exit(0);
    }

    //change directory command
    if(strcmp(args[0], "cd") == 0){
        if(args[1] == NULL){
            printf("Expected argument to \"cd\"\n");
        } else{
            //change directory and handle error
            if(chdir(args[1]) != 0){
                perror("cd failed");
            }
        }
        return 1;
    }

    //help command
    if(strcmp(args[0], "help") == 0){
        printf("\nSimple Shell Help\n");
        printf("Built-in commands:\n");
        printf(" cd <dir>\n exit\n help\n");
        printf("Supports pipes: ls | grep txt\n");
        return 1;
    }

    return 0; //not a built-in command
}

void executeCommand(char **args){
    pid_t pid = fork(); //create a child process

    if(pid == 0){
        //child process executes command
        if(execvp(args[0], args) < 0){
            perror("command failed"); //if execution fails
        }
        exit(1);
    } else{
        //parent process waits for child to finish
        wait(NULL);
    }
}

int main(){
    char input[MAX_INPUT]; //variable char array that reserves the max_input on the memory
    char *args1[MAX_ARGS], *args2[MAX_ARGS]; // two pointers
    
    printBanner();

    while(1){
        printDir(); //show current directory
        takeInput(input); //get user input
        
        if(strlen(input) == 0) continue; //ignore empty input
        
        char *left, *rigth;

        //check if input contains a pipe
        if(parsePipe(input, &left, &rigth)){
            parseInput(left, args1); //parse left command
            
            parseInput(rigth, args2); //parse rigth command

            executePipe(args1, args2); // execute piped commands
        } else{
            parseInput(input, args1); //parse normal command

            //handle built-in commands first
            if(handleBuiltIn(args1)) continue;

            executeCommand(args1); //execute external command
        }
    }

    return 0;
}