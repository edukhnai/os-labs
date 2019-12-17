#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc != 1)
    {
        printf("Command requires no arguments\n");
        exit(1);
    }
    char *str1 = (char *) malloc(256);
    char *str2 = (char *) malloc(256);
    if (str1 == nullptr)
    {
        puts("Memory error\n");
        exit(2);
    }
    int b;
    int check = 256;
    int h = 0;
    int str1_len = 0;
    for (b = getchar(); b != EOF; b = getchar(), str1_len++) {
        str1[str1_len] = b;

        if (str1_len >= 256) {
            check *= 2;
            str1 = (char *) realloc(str1, check);
            str2 = (char *) realloc(str2, check);
        }
    }
    if (str1_len == 0) {
        printf("Entered string is empty\n");
        exit(1);
    }
    for (int j = 0; j < str1_len; j++) {
        if (j == str1_len - 1 && str1[j] == '\n') {
            // prevents understanding '\n' as symbol
        }
        if (j + 1 < str1_len && j + 2 < str1_len && str1[j] != '\n' && str1[j + 1] != '\n' && str1[j + 2] != '\n') {
            int j1 = j;
            int j2 = j + 1;
            int j3 = j + 2;
            str2[h] = str1[j1];
            str2[++h] = str1[j2];
            str2[++h] = str1[j3];
            str2[++h] = '*';
            h = h + 1;
            j = j + 2;
        } else {
            str2[h] = str1[j];
            h = h + 1;
        }
    }

    for (int i = 0; i < h; i++)
        putchar(str2[i]);
    free(str1);
    free(str2);
    return 0;
}
