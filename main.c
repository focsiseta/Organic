#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int checkExtension(const char *,const char *);
int tellSize(int fd);


int main(int argc, char const *argv[])
{
    if(argc < 5){
        printf("Usage :organic [username] [source] [type of file] [destination folder name]");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    int x = 0;
    DIR *dir = NULL;
    struct dirent *entry = alloca(sizeof(struct dirent));
    char path[255] = {'\0'};
    char file_list[255][255] = {'\0'};
    char file_name[255][255] = {'\0'};
    char file_remove[255][255] = {'\0'};
    int fdarray[255] = {0};
    sprintf(path,"/home/%s/%s",argv[1],argv[2]);
    dir = opendir(path);
    while((entry = readdir(dir)) != NULL && i < 255){
        sprintf(file_list[i],"%s/%s",path,entry->d_name);
        if(checkExtension(file_list[i],argv[3]) == 0){
            sprintf(file_remove[x],"%s",file_list[i]);
            fdarray[x] = open(file_list[i],O_RDWR);
            sprintf(file_name[x],"%s",entry->d_name);
            x++;
        }
        i++;
    }
    char destination[255] = {'\0'};
    sprintf(destination,"%s/%s/",path,argv[4]);
    mkdir(destination,0777);
    i = 0;
    while(i < x){
        char oof[255] = {'\0'};
        int size = tellSize(fdarray[i]);
        char *buffer = malloc(sizeof(char) * size);
        read(fdarray[i],buffer,size);
        sprintf(oof,"%s/%s",destination,file_name[i]);
        int newfd = creat(oof,0777);
        write(newfd,buffer,size);
        remove(file_remove[i]);
        free(buffer);
        i++;
    }
    
    return 0;
}


int checkExtension(const char *file_name,const char *extension){
    const char *p = file_name;
    for(;*p != '\0';p++);
    p -= strlen(extension);
    return strcmp(extension,p);
}

int tellSize(int fd){
    int size = lseek(fd,0L,SEEK_END);
    lseek(fd,0L,SEEK_SET);
    return size;
}