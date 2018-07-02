#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#ifndef BUF_SIZE 
#define BUF_SIZE 1024
#endif
void usage(const char *bin) {
    printf("%s [-a filename|filename]\n", bin);
    exit(EXIT_FAILURE);
}
void errorHandle(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
int main(int argc, char **argv) {
    char *fileName = NULL;
    int flags = O_WRONLY | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    int opt;
    while((opt = getopt(argc, argv, "+:a:")) != -1)
        switch(opt) {
            case 'a':
                fileName = optarg;
                flags |= O_APPEND; 
                break;
            case '?':
            case ':': usage(argv[0]); break;
        }
    if(optind < argc) fileName = argv[optind]; 
    int fd = -1;
    if(fileName) { 
        if((flags & O_APPEND) != O_APPEND) flags |= O_TRUNC;

        fd = open(fileName, flags, mode);
        if(fd == -1) errorHandle("open");
    }
    char buffer[BUF_SIZE];
    ssize_t numRead = -1;
    while((numRead = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) { 
        if(write(STDOUT_FILENO, buffer, numRead) != numRead)errorHandle("");
        if(fd != -1)
            if(write(fd, buffer, numRead) != numRead) errorHandle("Err Buff");
    }
    if(numRead == -1) errorHandle("read");
    if(fd != -1 && close(fd) == -1) errorHandle("close");
    return 0;
}
