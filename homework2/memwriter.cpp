/** Compilation: gcc -o memwriter memwriter.cpp -lrt -lpthread **/
#include <cstdlib>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdio>
#include <cstring>
#include "shmem.h"

void report_and_exit(const char *msg) {
    perror(msg);
    exit(-1);
}

int main() {
    int fd = open(BackingFile, O_RDWR | O_CREAT, AccessPerms);
    if (fd < 0) report_and_exit("Can't open shared mem segment...");

    ftruncate(fd, ByteSize);

    char *memptr = static_cast<caddr_t>(mmap(nullptr, ByteSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    if ((caddr_t) -1 == memptr) report_and_exit("Can't get segment...");

    fprintf(stderr, "Shared mem address: %p [0..%d]\n", memptr, ByteSize - 1);
    fprintf(stderr, "backing file:       %s\n", BackingFile);


    sem_t *semptr = sem_open(SemaphoreName, O_CREAT, AccessPerms, 0);
    if (semptr == (void *) -1) report_and_exit("sem_open");

    strcpy(memptr, MemContents);

    if (sem_post(semptr) < 0) report_and_exit("sem_post");

    sleep(12);

    munmap(memptr, ByteSize);
    close(fd);
    sem_close(semptr);
    shm_unlink(BackingFile);
    return 0;
}
