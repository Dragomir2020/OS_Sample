
//Libraries
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 80
#define oops(m,x)    { perror(m); exit(x); }


// Main program
int main(int argc, char* argv[]){
    
    char line[MAX_LINE];
    int pipe;
    // open a named pipe
    pipe = open("/tmp/myFIFO", O_RDONLY);
    // read the data from the pipe
    while(strcmp((char*)line, (char*)"STOP") != 0){
        read(pipe, line, MAX_LINE-1);
        printf("%s\n", line);
        sleep(1);
    }
    
    // close the pipe
    close(pipe);
    return 0;
}


