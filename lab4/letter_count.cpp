#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;


int main(int argc, char *argv[]) {
    if (argc != 1)
    {
        printf("Command requires no arguments\n");
        exit(1);
    }
    char *str1 = (char *) malloc(256);
    if (str1 == nullptr)
    {
        puts("Memory error\n");
        exit(2);
    }
    int b;
    int check = 256;
    int str1_len = 0;
    int letters = 0;
    for (b = getchar(); b != EOF; b = getchar(), str1_len++) {
        str1[str1_len] = b;

        if (str1_len >= 256) {
            check *= 2;
            str1 = (char *) realloc(str1, check);
        }
    }
    if (str1_len == 0) {
        printf("Entered string is empty\n");
        exit(1);
    }
    string currString;
    for (int j = 0; j < str1_len; j++) {
        if (isalpha(str1[j])) {
            letters += 1;
        }
        if (str1[j] != '\n') {
            currString += str1[j];
        }
        if (j + 1 < str1_len && str1[j + 1] == '\n') {
            printf("%d letters: %s\n", letters, currString.c_str());
            letters = 0;
            currString = "";
            j++;
        }
    }
    free(str1);
    return 0;
}
