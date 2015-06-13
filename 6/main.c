#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Pass the directoryName as first argument.\n");
        exit(1);
    }
    handleDirectoryRecursively (argv[1]);
    return 0;
}

void handleDirectoryRecursively (const char * directoryName) {
    DIR * directoryStream;
    struct stat statStructure;
    directoryStream = opendir (directoryName);

    if (!directoryStream) {
        printf ("Error. Failed to open directory '%s'", directoryName);
        exit (0);
    }
    while (1) {
        const char * fileName;
        char filePath[PATH_MAX];

        struct dirent * currentDirentStructure = readdir (directoryStream);;

        if (!currentDirentStructure) {
            break;
        }

        fileName = currentDirentStructure -> d_name;

        snprintf (filePath, PATH_MAX,"%s/%s", directoryName, fileName);

        if (stat(filePath, &statStructure) == -1) {
           exit(0);
        }

        int linksCount = statStructure.st_nlink;
        printPathWithLinksCount(filePath, linksCount);

        if (currentDirentStructure -> d_type == DT_DIR &&
                strcmp (fileName, "..") != 0 &&
                strcmp (fileName, ".") != 0) {
                printPathWithLinksCount(filePath, linksCount);
                handleDirectoryRecursively (filePath);
        }
    }
    if (closedir (directoryStream)) {
        printf ("Error. Failed to close directory '%s': %s\n", directoryName);
        exit (0);
    }
}

void printPathWithLinksCount(char * path, int linkCount) {
    printf("%s\n%d\n", path, linkCount);
}

