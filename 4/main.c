#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_FILENAME_SIZE 255
#define LOCK_TIME_IN_SECONDS 10
#define LOCK_FILE_EXTENSION ".lck"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Pass the fileName as first argument.\n");
        return 1;
    }

    char fileName[MAX_FILENAME_SIZE];
    char lockFileName[MAX_FILENAME_SIZE];
    strcpy(fileName, argv[1]);
    strcpy(lockFileName, argv[1]);
    strcat(lockFileName, LOCK_FILE_EXTENSION);

    waitWhileFileIsLocked(fileName, lockFileName);

    createLockFile(lockFileName);

    printFileIsLocked(fileName);
    sleep(LOCK_TIME_IN_SECONDS);

    deleteLockFile(lockFileName);

    return 0;
}

void waitWhileFileIsLocked(char* fileName, char* lockFileName){
    struct stat st;
    while (stat(lockFileName, &st) == 0) {
        printFileIsLocked(fileName);
        sleep(3);
    }
}

void createLockFile(char *lockFileName) {
    FILE *lockFile = fopen(lockFileName, "w");
    fprintf(lockFile, "%d", getpid());
    fclose(lockFile);
    return 0;
}

void deleteLockFile(char *lockFileName) {
    remove(lockFileName);
}

void printFileIsLocked(char* fileName){
    printf("The file %s is locked. Waiting..\n", fileName);
}

