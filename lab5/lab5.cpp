#include<cstdio>
#include<unistd.h>
#include <cstdlib>
#include <cstring>
#define R 0
#define W 1
using namespace std;

int main() {
    int p[2], q[2];
    int returnStatus1, returnStatus2;
    int pid;
    char readMessage[100];
    FILE  *fp;

    returnStatus1 = pipe(p);
    if (returnStatus1 == -1) {
        printf("Unable to create pipe 1 \n");
        return 1;

    }

    returnStatus2 = pipe(q);
    if (returnStatus2 == -1) {
        printf("Unable to create pipe 2 \n");
        return 1;
    }

    pid = fork();
    // Parent process
    if (pid < 0) {
        printf("Fork error\n");
        exit(1);
    }
    if (pid > 0) {
        char *str1 = (char *) malloc(256);

        if (str1 == nullptr)
        {
            puts("Memory error\n");
            exit(2);
        }

        int b;
        int check = 256;
        int str1_len = 0;
        int numberOfSequentialSymbols = 0;
        for (b = getchar(); b != EOF; b = getchar(), str1_len++) {

            if (b != '\n') {
                numberOfSequentialSymbols++;
                str1[str1_len] = b;

                if (numberOfSequentialSymbols == 3) {
                    numberOfSequentialSymbols = 0;
                    str1[++str1_len] = '*';
                }
            } else {
                numberOfSequentialSymbols = 0;
                str1[str1_len] = b;
            }

            if (str1_len >= 256) {
                check *= 2;
                str1 = (char *) realloc(str1, check);
            }
        }

        close(p[0]);
        close(q[1]);

        if (strcmp(str1, "") == 0 || strcmp(str1, "\n") == 0) {
            printf("Entered empty string\n");
            exit(-1);
        }

        printf("In Parent: entered string with first filter: %s", str1);
        fp = ::fdopen(p[W], "w");
        fputs(str1, fp);
        fclose(fp);

        read(q[0], readMessage, sizeof(readMessage) - 1);
        printf("In Parent: child returned result: %s", readMessage);

    } else {
        // Child process
        close(p[1]);
        close(q[0]);

        dup2(p[0], STDIN_FILENO);
        dup2(q[1], STDOUT_FILENO);
        close(p[0]);
        close(q[1]);
        execl("letter_count", "letter_count", (char *) nullptr);
        printf("Error with letter_count filter\n");
        exit(1);
    }
    return 0;
}
