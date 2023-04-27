#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_PATH_LENGTH 256


void counter_l(char *filename) {
    FILE *fp;
    char c;
    int lines = 0;
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }
    
    while ((c = getc(fp)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }
    
    printf("File %s has %d lines\n", filename, lines);
    fclose(fp);
}
void printAccessRights(mode_t mode) {
    printf("User:\n");
    printf("Read - %s\n", (mode & S_IRUSR) ? "yes" : "no");
    printf("Write - %s\n", (mode & S_IWUSR) ? "yes" : "no");
    printf("Exec - %s\n", (mode & S_IXUSR) ? "yes" : "no");
    printf("Group:\n");
    printf("Read - %s\n", (mode & S_IRGRP) ? "yes" : "no");
    printf("Write - %s\n", (mode & S_IWGRP) ? "yes" : "no");
    printf("Exec - %s\n", (mode & S_IXGRP) ? "yes" : "no");
    printf("Others:\n");
    printf("Read - %s\n", (mode & S_IROTH) ? "yes" : "no");
    printf("Write - %s\n", (mode & S_IWOTH) ? "yes" : "no");
    printf("Exec - %s\n", (mode & S_IXOTH) ? "yes" : "no");
}

// Function to create symbolic link
void createSymbolicLink(char *filepath) {
    char linkname[MAX_PATH_LENGTH];
    printf("Please give the link name: ");
    scanf("%s", linkname);
    if (symlink(filepath, linkname) == -1) {
        perror("Error creating symbolic link");
    } else {
        printf("Symbolic link created successfully.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        char *filepath = argv[i];

        struct stat filestat;
        if (lstat(filepath, &filestat) == -1) {
            perror("Error getting file information");
            continue;
        }

        printf("%s - ", filepath);
        if (S_ISREG(filestat.st_mode)) {
            printf("REGULAR FILE\n");
            printf("MENU\n");
            printf("• n: name\n");
            printf("• d: size\n");
            printf("• h: hard link count\n");
            printf("• m: time of last modification\n");
            printf("• a: access rights\n");
            printf("• 1: create symbolic link\n");
            printf("Please enter your options: ");
            char options[64];
            scanf("%s", options);

            printf("Name of file: %s\n", filepath);
            if (strchr(options, 'n') != NULL) {
                // Display name of file
            }
            if (strchr(options, 'd') != NULL) {
                printf("Size of file: %ld bytes\n", filestat.st_size);
            }
            if (strchr(options, 'h') != NULL) {
                printf("Hard link count: %ld\n", filestat.st_nlink);
            }
            if (strchr(options, 'm') != NULL) {
                printf("Time of last modification: %s", ctime(&filestat.st_mtime));
            }
            if(strchr(options, 'a')!= NULL){
                printAccessRights(filestat.st_uid);
            }
            if(strchr(options, 'l')!= NULL){
                createSymbolicLink(filepath);
            }
        }
    }
    pid_t pid=fork();
    if (pid == -1) {
        printf("Failed fork\n");
        return 1;
    } else if (pid == 0) {
        for (int i = 1; i <= atoi(argv[1]); i++) {
            printf("Child with PID %d going to %d\n", getpid(), i);
        }
        return 0;
    }
    for (int i = 2; i < argc; i++) {
        pid = fork();
        if (pid == -1) {
            printf("Failed fork\n");
            return 1;
        } else if (pid == 0) {
            counter_l(argv[i]);
            char *arg=argv[i];
            if(strstr(arg,".c")!=NULL){
                char *aux = (char *)malloc(sizeof(char)*(strlen(arg)+6));
                sprintf(aux,"gcc %s -o %s.out && ./%s.out",arg,arg,arg);
                system(aux);
                free(aux);
            }else{
                char *aux = (char *)malloc(sizeof(char)*(strlen(arg)+3));
                sprintf(aux,"./%s",arg);
                system(aux);
                free(aux);
            }
            return 0;
        }
    }
    for (int i = 0; i < argc - 2; i++) {
        wait(NULL);
    }
}   
