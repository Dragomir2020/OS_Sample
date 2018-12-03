
//Libraries
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> //mmap
#include <unistd.h>
#include <fcntl.h>
#include <fstream> // Filestream for reading text file

//#define MAX_LINE 100
//#define FILE_LENGTH 0x100

//const int SHM_SIZE = 1024;  /* make it a 1K shared memory segment */
const char FILE_NAME[] = "sample.txt";

// Main program
int main(int argc, char* argv[]){
    
    const char *filepath = "/tmp/mmapped.bin";
    
    int fd = open(filepath, O_RDONLY, (mode_t)0600);
    
    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    char *map = (char *)"";
    size_t textsize;
    while(strcmp(map, (char *)"STOP") != 0){
        struct stat fileInfo = {0};

        if (fstat(fd, &fileInfo) == -1)
        {
            perror("Error getting the file size");
            exit(EXIT_FAILURE);
        }
        
        textsize = fileInfo.st_size;
        if (textsize == 0)
        {
            fprintf(stderr, "Error: File is empty, nothing to do\n");
            exit(EXIT_FAILURE);
        }
        
        map = (char *)mmap(0, textsize, PROT_READ, MAP_SHARED, fd, 0);
        if (map == MAP_FAILED)
        {
            close(fd);
            perror("Error mmapping the file");
            exit(EXIT_FAILURE);
        }
        
        printf("%s\n", map);
        
        sleep(1);
    }
    
    // Don't forget to free the mmapped memory
    if (munmap(map, textsize) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);
    
    return 0;
}


