
//Libraries
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> //mmap
#include <unistd.h>
#include <fcntl.h>
#include <fstream> // Filestream for reading text file

#define MAX_LINE 100
#define FILE_LENGTH 0x100

const int SHM_SIZE = 1024;  /* make it a 1K shared memory segment */
const char FILE_NAME[] = "sample.txt";
int fd;
char *text;
size_t textsize;
char *map;

void mmap(std::string str){
    // actually write out the data and close the pipe
    std::cout << str << std::endl;
    
    // Stretch the file size to the size of the (mmapped) array of char
    
    text = (char *)str.c_str();
    
    textsize = strlen(text) + 1; // + \0 null character
    
    if (lseek(fd, textsize-1, SEEK_SET) == -1)
    {
        close(fd);
        perror("Error calling lseek() to 'stretch' the file");
        exit(EXIT_FAILURE);
    }
    
    if (write(fd, "", 1) == -1)
    {
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }
    
    
    // Now the file is ready to be mmapped.
    map = (char*)mmap(0, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = 0; i < textsize; i++)
    {
        map[i] = text[i];
    }
    
    // Write it now to disk
    if (msync(map, textsize, MS_SYNC) == -1)
    {
        perror("Could not sync the file to disk");
    }
}

// Main program
int main(int argc, char* argv[]){
    
    // Open a file for writing.
    
    const char *filepath = "/tmp/mmapped.bin";
    
    fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    
    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    
    std::ifstream file(FILE_NAME);
    char buffer[MAX_LINE];
    std::string str;
    while (std::getline(file, str))
    {
        mmap(str);
        sleep(1);
    }
    
    char end[] = "STOP";
    for (size_t i = 0; i < 4; i++)
    {
        map[i] = end[i];
    }
    sleep(1);
    
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


