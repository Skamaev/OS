#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUMBERS_COUNT 1024

const int compareNumbers (const void *a, const void *b) {
    const double *da = (const double *) a;
    const double *db = (const double *) b;
    return (*da > *db) - (*da < *db);
}

int main(int argc, char *argv[])
{
    double numbersArray[1000];
    int numbersCount=0;

    int i;
    for (i=1; i<argc-1; ++i) {
        numbersCount = handleFile(argv[i], numbersArray, numbersCount);
    }

    qsort(numbersArray, numbersCount, sizeof(double), compareNumbers);

    FILE *outputFile = fopen(argv[argc-1], "w");
    if ( outputFile == NULL ) {
        printf("%s %s\n", "Error. Cannot open output file", argv[argc-1]);
        return 1;
    }

    for (i=0; i<numbersCount; ++i) {
        fprintf(outputFile, "%f\n", numbersArray[i]);
    }

    int stat = fclose(outputFile);

    if (stat == -1) {
        printf("Error. Can't close output file: %s");
    }

    return 0;
}

int handleFile(char* fileName, double* numbersArray, int numbersCount) {
    FILE* file = fopen(fileName, "r");

    if ( file == NULL ) {
        printf("%s:%s\n", "Error. Cannot open input file", fileName);
        return 0;
    }

    int n;
    double currentNumber;

    while((n = fscanf(file, "%lf", &currentNumber)) != -1) {
        if (n == 0) {
            int charNumber;
            charNumber = fgetc(file);
            while (!isspace(charNumber) && charNumber != EOF) {
                charNumber = fgetc(file);
            }
            continue;
        }
        if (numbersCount >= MAX_NUMBERS_COUNT) {
            printf("Error. Not enough memory.");
            return 0;
        }
        numbersArray[numbersCount++] = currentNumber;
    }
    return numbersCount;
}


