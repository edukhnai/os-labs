#include <sys/stat.h> // to work with directories
#include <dirent.h> // to work with directories
#include <cstdio>
#include <ctime>
#include <pwd.h> // struct password
#include <grp.h>

int main(int argc, char *argv[]) {
    // current directory
    DIR *currDir;
    // iterating directory for the while loop
    struct dirent *theFile;
    // status associated with the current file
    struct stat theStat;
    //Creating a placeholder for the string
    char buf[1024];
    // assign user input to the current directory
    currDir = opendir(argv[1]);
    if (argv[1] == nullptr) {
        printf("Directory name is null\n");
    } else if (argv[2] != nullptr) {
        printf("More than one argument\n");
    } else {
        while ((theFile = readdir(currDir)) != NULL) {
            if ((*theFile).d_name[0] != '.') { // remove if . and .. also need to be printed
                sprintf(buf, "%s/%s", argv[1], (*theFile).d_name);
                // use stat function to get information about the file
                lstat(buf, &theStat);
                // establish the file type
                switch (theStat.st_mode & S_IFMT) {
                    case S_IFDIR:
                        printf(" %s\n", (*theFile).d_name);
                        break;
                    default:
                        break;
                }
            }
        }
        closedir(currDir);
    }
}
