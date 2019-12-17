#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cctype>
#include "server.h"
#include "stdlib.h"
#include "stdio.h"
#include <cstring>

int main(int argc, char **argv) {
    int fdpub, fdpriv, fd;
    struct message msg;
    int n;
    char line[LINESIZE];
    loop:

    if ((fdpub = open(PUBLIC, O_RDONLY)) == -1) {
        perror(PUBLIC);
        exit(1);
    }

    while (read(fdpub, (char *) &msg, sizeof(msg)) > 0) {
        if ((fd = open(msg.filename, O_RDONLY)) == -1) {
            perror(msg.filename);
            break;
        }

        if ((fdpriv = open(msg.privfifo, O_WRONLY)) == -1) {
            perror(msg.privfifo);
            break;
        }

        while ((n = read(fd, line, LINESIZE)) > 0) {
            int h = 0;
            char *str1 = (char *) malloc(256);
            for (int j = 0; j < n; j++) {
                if (j == n - 1 && str1[j] == '\n') {
                    // does not allow to append ending \n to the new string
                }
                if (j + 1 < n && j + 2 < n && line[j] != '\n' && line[j + 1] != '\n' && line[j + 2] != '\n') {
                    int j1 = j;
                    int j2 = j + 1;
                    int j3 = j + 2;
                    str1[h] = line[j1];
                    str1[++h] = line[j2];
                    str1[++h] = line[j3];
                    str1[++h] = '*';
                    h = h + 1;
                    j = j + 2;
                } else {
                    str1[h] = line[j];
                    h = h + 1;
                }
            }

            write(fdpriv, str1, h + 1);
        }

        close(fd);
        close(fdpriv);
    }

    close(fdpub);
    goto loop;
}
