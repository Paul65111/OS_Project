#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_PATH_LENGTH 256
int nrproc=0;
char filepath[MAX_PATH_LENGTH];
void counter_l(char *filename) {
    FILE *fp;
    char c;
    int lines = 0;
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open file %s\n", filename);
        exit(-1);
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
void computescore(int erorrs,int warnings,char* file){
    int score;
  
   if(erorrs == 0 && warnings == 0){
    printf("The score for the file is 10\n");
   }
   if(erorrs >= 1){
    printf("The score for the file is 1\n");
   }
   if(erorrs == 0 && warnings >= 10){
    printf("The score for the file is 2\n");
   }
   if(erorrs == 0 && warnings <=10)
   {
    score = 2+8*((10-warnings)/10);
    printf("The score for the file is %d\n",score);
   }
    
}
void getLinkname(char* file){

    char buffer[1024];
    readlink(file,buffer,sizeof(buffer)-1);

    printf("The link name of %s is %s\n ",file,buffer);

}
void getLinksize(char* file){

    char buf[1024];

    readlink(file,buf,sizeof(buf)-1);

    struct stat Target;
    lstat(buf,&Target);

    printf("The target size of %s is %ld\n ",file,Target.st_size);

}
void check_c_files(char* dir_name){

    DIR *directory;
    struct dirent *ent;
    int count = 0;
    int len;
    directory = opendir(dir_name);

    if(directory!=NULL )
    {
        while((ent = readdir(directory)) !=NULL){
            len = strlen(ent->d_name);
                if(strcmp(ent->d_name+len -2,".c") == 0)
                    count++;

        }
        closedir(directory);
    }else{
        printf("Error opening directory\n");
    }

    printf("The number of c files is %d\n",count);

}
int check_regularfile(char* file_name){ 

    int len = strlen(file_name);
            if(strcmp(file_name+len - 2,".c") == 0)
                    return 1;

    return 0;

}
int main(int argc, char *argv[]) {
    int pipefd[2];
    int pid;
    struct stat filestat;
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        exit(1);
    }
    for( int  i = 1; i < argc ;i++){
        lstat(argv[i],&filestat);
    }
    pid  = fork();
    
    if(pid  < 0 ){
        perror("Process for regular file didn t start");
    }else if( pid == 0){
        for(int j=1;j<argc;j++){
            char *filepath = argv[j];
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
                if (strchr(options, 'n') != NULL) {
                    printf("The name of the file is: %s\n",filepath);
                }
                if (strchr(options, 'd') != NULL) {
                    printf("Size of file: %ld bytes\n", filestat.st_size);
                }
                if (strchr(options, 'h') != NULL) {
                    printf("Hard link count: %ld\n", filestat.st_nlink);
                }
                if (strchr(options, 'm') != NULL) {
                    printf("Time of last modification: %s\n", ctime(&filestat.st_mtime));
                }
                if(strchr(options, 'a')!= NULL){
                    printAccessRights(filestat.st_uid);
                }
                if(strchr(options, 'l')!= NULL){
                    createSymbolicLink(filepath);
                }
            }else if(S_ISLNK(filestat.st_mode)){
                printf("The file ' %s ' a symbolic link\n",argv[j]);
                char c[10];
                printf("MENU\n");
                printf("\n 'n'->Link name \n'l'->Delete link \n 'd'->Size of link \n'z'->Size of target \n'a'->Acces rights \n");
                scanf(" %s",c);
                if (strchr(c, 'n') != NULL) {
                    getLinkname(argv[j]);
                }
                else if (strchr(c, 'l') != NULL) {
                    unlink(argv[j]);
                    printf("The link was deleted\n");
                }
                else if (strchr(c, 'd') != NULL) {
                    printf("Size of link is %ld \n",filestat.st_size);
                }
                else if (strchr(c, 'z') != NULL) {
                    getLinksize(argv[j]);
                }
                else if (strchr(c, 'a') != NULL) {
                    printAccessRights(filestat.st_uid);
                }
                else{
                printf("Error command");
                }
            }else{
                printf("The %s is not a regular file or a symbolic link\n",argv[j]);
                char c[10];
                printf("\n 'n'->Name \n 'd'->Size \n 'a'->Access rights \n 'c'->Total number of .c files \n");
                scanf(" %s",c);
                if (strchr(c, 'n') != NULL) {
                    printf("File name is %s\n",argv[j]);
                }
                else if (strchr(c, 'd') != NULL) {
                    printf("Size of the file is %ld\n",filestat.st_size);
                }
                else if (strchr(c, 'c') != NULL) {
                    check_c_files(argv[j]);
                }
                else if (strchr(c, 'a') != NULL) {
                    printAccessRights(filestat.st_uid);
                }
                else{
                printf("Error command\n");
                }
            }
        }
        exit(0);
    }
    for(int j=1;j<argc;j++){
        char *filepath = argv[j];
        printf("%s - ", filepath);
        if(S_ISREG(filestat.st_mode)){
            if(pipe(pipefd) == -1){
                perror("Error with pipe\n");
                exit(EXIT_FAILURE);
            }
            nrproc++;
            pid = fork();
            int warnings=0,erorrs=0;   
            if(pid < 0){
                perror("Cannot start\n");
            }else if(pid == 0){
                if(check_regularfile(argv[j] ) == 1){    
                        close(pipefd[0]);
                        dup2(pipefd[1], STDOUT_FILENO);
                        execlp("./script.sh","./script.sh",argv[j],NULL);
                        exit(0);
                }else{  
                        execlp("wc","wc","-l",argv[j],NULL);
                        exit(0);
                }
                exit(0);
            }else{            
                close(pipefd[1]);
                FILE * FD = fdopen(pipefd[0],"r");
                if(FD == NULL){
                    perror("Cannot open the file for the pipe\n");
                    exit(EXIT_FAILURE);
                }
                fscanf(FD,"%d",&warnings);
                fscanf(FD,"%d", &erorrs);
                fclose(FD);
                close(pipefd[0]);
                if(check_regularfile(argv[j]) == 1){
                    computescore(warnings,erorrs,argv[j]);        
                }
            }
        }
        if(S_ISDIR(filestat.st_mode)){
                pid = fork();
                nrproc++;
                if(pid < 0){
                    perror("Cannot start\n");
                }else if(pid == 0){
                    char director_name[256];
                    strcat(director_name,argv[j]);
                    strcat(director_name,"_file.txt");
                    if(execlp("./filescript.sh","./filescript.sh",director_name,NULL) == -1){
                        printf("Failed to create the directory\n");
                    }
                exit(0);
                }
        }
        if(S_ISLNK(filestat.st_mode)){
            nrproc++;
            pid = fork();
            if(pid < 0){
                perror("Didn t start\n");
            }else if(pid == 0){
                execlp("chmod","chmod","760",argv[j],NULL);
            }
        }
        for(int i = 0;i<nrproc;i++){
            int status;
            wait(&status);             
        }
    }
    return 0;
}
