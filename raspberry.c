#include "shell.h"
#include "raspberry.h"

void sshRaspberry(char *remoteCmd) {
    char *args[] = {"ssh", "raspberry", remoteCmd, NULL};
    pid_t pid = fork();
    if (pid == 0) {
        execvp("ssh", args);
        perror("ssh failed");
        exit(1);
    } else {
        wait(NULL);
    }
}

void startChatBot()   { sshRaspberry("sudo systemctl start housebot.service"); }
void restartChatBot() { sshRaspberry("sudo systemctl restart housebot.service"); }
void chatBotStatus()  { sshRaspberry("sudo systemctl status housebot.service --no-pager"); }
void stopChatBot()    { sshRaspberry("sudo systemctl stop housebot.service"); }
