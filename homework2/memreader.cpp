/** Compilation: gcc -o memreader memreader.cpp -lrt -lpthread **/
#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include "shmem.h"

void report_and_exit(const char* msg) {
    perror(msg);
    exit(-1);
}

int main() {
    int fd = open(BackingFile, O_RDWR, AccessPerms);
    if (fd < 0) report_and_exit("Can't get file descriptor...");

    char* memptr = static_cast<char *>(mmap(nullptr,
                                            ByteSize,
                                            PROT_READ | PROT_WRITE,
                                            MAP_SHARED,
                                            fd,
                                            0));
    if ((caddr_t) -1 == memptr) report_and_exit("Can't access segment...");


    sem_t* semptr = sem_open(SemaphoreName,
                             O_CREAT,
                             AccessPerms,
                             0);
    if (semptr == (void*) -1) report_and_exit("sem_open");

    if (!sem_wait(semptr)) {
        int i;
        for (i = 0; i < strlen(MemContents); i++)
            write(STDOUT_FILENO, memptr + i, 1);
        sem_post(semptr);
    }

    munmap(memptr, ByteSize);
    close(fd);
    sem_close(semptr);
    unlink(BackingFile);
    return 0;
}
