#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILENAME "in.conf"
#define MAX_PROGRAMS_NUMBER 20
#define MAX_CONFIG_STRING_LENGTH 70
#define TEMP_DIRECTORY_NAME "/tmp/"

int main() {
    FILE *configFile = fopen(CONFIG_FILENAME, "r");
    pid_t processesIds[MAX_PROGRAMS_NUMBER];
    int restartProgramsIds[MAX_PROGRAMS_NUMBER];
    char programItems[MAX_PROGRAMS_NUMBER][MAX_CONFIG_STRING_LENGTH];


    char pidFilePath[MAX_PROGRAMS_NUMBER];
    char processNumber[2];
    char *programArgs[MAX_CONFIG_STRING_LENGTH];

    int programItemIndex = 0;
    pid_t processId;
    char configString[MAX_CONFIG_STRING_LENGTH];

    while (fgets(configString, sizeof(configString), configFile) != NULL) {
            strcpy(programItems[programItemIndex], configString);

            createConfigStringContext(configString, programItemIndex, programItems, programArgs, restartProgramsIds);

            processId = forkWithValidation(programArgs[0], programArgs);
            processesIds[programItemIndex] = processId;

            char *temp = strtok(programArgs[0],"/");
            while (temp != NULL) {
                programArgs[0] = temp;
                temp = strtok(NULL,"/");
            }
            strcpy(pidFilePath, TEMP_DIRECTORY_NAME);
            strcat(pidFilePath, programArgs[0]);
            sprintf(processNumber,"%d", programItemIndex++);
            strcat(pidFilePath, processNumber);
            strcat(pidFilePath, ".pid");

            FILE *of = fopen(pidFilePath,"w");
            fprintf(of, "%d", processId);
            fclose(of);

    }
    fclose(configFile);
    int runningProcessCount = programItemIndex;

    while (runningProcessCount) {
        processId = wait(NULL);
        int j = 0;
        for (; j < programItemIndex; j++) {
            if (processesIds[j] == processId) {
                strcpy(configString, programItems[j]);
                programArgs[0] = strtok(configString, " ");

                char *temp = strtok(programArgs[0],"/");
                while (temp != NULL) {
                    programArgs[0] = temp;
                    temp = strtok(NULL,"/");
                }
                strcpy(pidFilePath, TEMP_DIRECTORY_NAME);
                strcat(pidFilePath, programArgs[0]);
                sprintf(processNumber,"%d", j);
                strcat(pidFilePath, processNumber);
                strcat(pidFilePath, ".pid");
                if (restartProgramsIds[j] == 'r') {
                    restartProgramsIds[j] = 0;
                    temp = strtok(programItems[j], " ");
                    int i = 0;
                    while (temp != NULL) {
                        programArgs[i++] = temp;
                        temp = strtok(NULL, " ");
                    }
                    programArgs[--i] = NULL;
                    processId = forkWithValidation(programArgs[0],programArgs);
                    processesIds[j] = processId;
                    FILE *of = fopen(pidFilePath,"w");
                    fprintf(of, "%d", processId);
                    fclose(of);

                } else {
                    processesIds[j] = 0;
                    remove(pidFilePath);
                    runningProcessCount--;
                }
            }
        }
    }
    return 0;
}


void createConfigStringContext(char configString[], int stringNumber, char *progs[MAX_PROGRAMS_NUMBER][MAX_CONFIG_STRING_LENGTH], char *args[], int respawn[]){
    strcpy(progs[stringNumber], configString);
    char *tok = strtok(configString, " ");
    int i = 0;
    while (tok != NULL) {
        args[i++] = tok;
        tok = strtok(NULL, " ");
    }
    if (args[--i][0] == 'r')
        respawn[stringNumber] = 'r';

    args[i]=0;
}

void handleForkError(){
    printf("Error:Fork failed.\n");
    exit(-1);
}

void executeProgramWithArgs(char *programName, char *args[]){
    execvp(programName, args);
    exit(0);
}

int forkWithValidation(char *programName, char *args[]){
    int processId = fork();
    switch (processId) {
    case -1:
        handleForkError();
    case 0:
        executeProgramWithArgs(programName,args);
    return processId;

    }
}

