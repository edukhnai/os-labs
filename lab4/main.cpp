#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 1)
    {
        printf("Command requires no arguments\n");
        exit(1);
    }
    pid_t pid1, pid2;
    int fd[2];
    int status, dead;
    switch (pid1 = fork()) {
        case -1:
            printf("Error with fork() #1 \n");
            exit(1);

        case 0:
            pipe(fd);
            switch(pid2 = fork()) {
                case -1:
                    printf("Error with fork() #2 \n");
                    exit(2);

                case 0:
                    close(0);
                    dup(fd[0]);
                    close(fd[0]);
                    close(fd[1]);
                    execl("letter_count", "letter_count", (char*)nullptr);
                    puts("Error with counting words filter \n");
                    exit(0);

                default:
                    close(1);
                    dup(fd[1]);
                    close(fd[1]);
                    close(fd[0]);
                    execl("insert_asterisk", "insert_asterisk", (char*)nullptr);
                    puts("Error with adding asterisk filter\n");
                    exit(0);
            }

        default:
            dead = wait(&status);
            exit(0);
    }
}
