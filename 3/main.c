#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define fd = "fdf"
int main(int argc, char *argv[]) {
    static const char CANT_OPEN_FILE_ERROR_MESSAGE [] = "Cannot open file\n";
    static const char CANT_CLOSE_FILE_ERROR_MESSAGE [] = "Cannot close file\n";
    char currentByte;
    int currentStepNullsCount=0;
    int fileDescriptor;
    char *fileName = argv[1];
    fileDescriptor = open(fileName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
    if ( fileDescriptor == -1 ) {
        perror(CANT_OPEN_FILE_ERROR_MESSAGE);
        exit(1);
    }
    while (read(STDIN_FILENO, &currentByte, 1) == 1) {
        if(currentByte != 0){
            if (currentStepNullsCount == 0) {
                write(fileDescriptor, &currentByte, 1);
            }
            else {
                lseek(fileDescriptor, currentStepNullsCount, SEEK_CUR);
                write(fileDescriptor, &currentByte, 1);
                currentStepNullsCount = 0;
            }
        }
        else {
            ++currentStepNullsCount;
        }
    }

    int closeFileResult = close(fileDescriptor);

    if (closeFileResult == -1) {
        perror(CANT_CLOSE_FILE_ERROR_MESSAGE);
        exit(1);
    }
    return 0;
}
