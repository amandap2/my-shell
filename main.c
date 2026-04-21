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
            // if(handleBuiltIn(args1)) continue;

            // executeCommand(args1); //execute external command
        }
    }

    return 0;
}