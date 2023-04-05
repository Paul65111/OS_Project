#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 256
#define MAX_OPTIONS_LENGTH 64

// Function to print access rights
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
            char options[MAX_OPTIONS_LENGTH];
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
           
