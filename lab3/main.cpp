#include <sys/stat.h>
#include <cstdio>
#include <dirent.h>
#include <ctime>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[]) {
    // current directory
    DIR *currDirectory;
    // file for iterating through directory, will be used in the while loop
    struct dirent *currFile;
    // stats associated to the current file
    struct stat currStat;
    // info about owner of file
    struct passwd *tf;
    // info about group to which owner belongs
    struct group *gf;

    //Creating a placeholder for the string.
    char infoPlaceholder[1024];

    // assigning current directory to user input
    currDirectory = opendir(argv[1]);

    //If a file is found, readdir returns a NOT NULL value
    while ((currFile = readdir(currDirectory)) != NULL) {
        // disable printing hidden files
        if ((*currFile).d_name[0] != '.') {
            //We sprint "currDirectory/currFile" which defines the path to our file
            sprintf(infoPlaceholder, "%s/%s", argv[1], (*currFile).d_name);
            //Then we use stat function in order to retrieve information about the file
            lstat(infoPlaceholder, &currStat);

            // index of file
            printf("%ld ", (long) currStat.st_ino);

            // determines file type
            switch (currStat.st_mode & S_IFMT) {
                case S_IFBLK:
                    printf("b ");
                    break;
                case S_IFCHR:
                    printf("c ");
                    break;
                case S_IFDIR:
                    printf("d ");
                    break; //It's a (sub)directory
                case S_IFIFO:
                    printf("p ");
                    break; //fifo
                case S_IFLNK:
                    printf("l ");
                    break; //Sym link
                case S_IFSOCK:
                    printf("s ");
                    break;
                    //Filetype isn't identified
                default:
                    printf("- ");
                    break;
            }
            // permissions
            printf((currStat.st_mode & S_IRUSR) ? " r" : " -");
            printf((currStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((currStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((currStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((currStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((currStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((currStat.st_mode & S_IROTH) ? "r" : "-");
            printf((currStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((currStat.st_mode & S_IXOTH) ? "x" : "-");

            // number of links to file
            printf("%ld ", currStat.st_nlink);

            // file owner
            tf = getpwuid(currStat.st_uid);
            printf("\t%s ", (*tf).pw_name);

            // group
            gf = getgrgid(currStat.st_gid);
            printf("\t%s ", (*gf).gr_name);

            //size of file in bytes
            printf("%zu", currStat.st_size);

            // date in format 14 Oct 2019 09:18
            char mBuf[80];
            struct tm *mytm = localtime(&currStat.st_mtime);
            localtime(&currStat.st_mtime);
            strftime(mBuf, 18, "%d %b %Y %H:%M", mytm);
            printf(" %s", mBuf);

            // name
            printf(" %s\n", (*currFile).d_name);
        }
    }
    closedir(currDirectory);
}
