//
// Created by kate on 11.11.2019.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "server.h"

using namespace std;

int main(int argc, char **argv) {
    struct message msg;
    int n, fdpub, fdpriv;
    char line[LINESIZE];

    if (argc != 2)
    {
        printf("Command requires one argument\n");
        exit(1);
    }

    sprintf(msg.privfifo, "Fifo%d", getpid());
    if (mkfifo(msg.privfifo, S_IFIFO | 0666) == -1) {
        perror(msg.privfifo);
        exit(1);
    }

    if ((fdpub = open(PUBLIC, O_WRONLY)) == -1) {
        perror(PUBLIC);
        exit(2);
    }

    strcpy(msg.filename, argv[1]);
    FILE *file = fopen(msg.filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", msg.filename);
        return -1;
    }
    write(fdpub, (char *) &msg, sizeof(msg));

    if ((fdpriv = open(msg.privfifo, O_RDONLY)) == -1) {
        perror(msg.privfifo);
        exit(3);
    }

    while ((n = read(fdpriv, line, LINESIZE)) > 0) {
        int letters = 0;
        string currString;
        for (int i = 0; i < n; i++) {
            if (isalpha(line[i])) {
                letters += 1;
            }
            if (line[i] != '\n') {
                currString += line[i];
            }
            if (i + 1 < n && line[i + 1] == '\n') {
                string newLine = to_string(letters) + " letters: " + currString + '\n';
                write(1, newLine.c_str(), sizeof(newLine) + 1);
                letters = 0;
                currString = "";
                i++;
            }
        }
    }

    close(fdpriv);
    unlink(msg.privfifo);
    exit(0);
}
