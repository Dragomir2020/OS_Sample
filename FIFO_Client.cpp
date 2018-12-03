
//Libraries
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include<cstring>
#include <ctype.h>
#define MAX_LINE 80
#define oops(m,x)    { perror(m); exit(x); }


// Main program
int main(int argc, char* argv[]){
    
    char line[MAX_LINE];
    int pipe;
    
    char * myfifo = (char *)"/tmp/myfifo";
    mkfifo(myfifo, 0600);
    
    // open a named pipe
    pipe = open(myfifo, O_WRONLY); // | O_NONBLOCK
    
    std::ifstream file("sample.txt");
    //char buffer[MAX_LINE];
    std::string str;
    while (std::getline(file, str))
    {
        // actually write out the data and close the pipe
        std::cout << str << std::endl;
        transform(str.begin(), str.end(), str.begin(), toupper);
        write(pipe, (const char *)str.c_str(), strlen(str.c_str()) + 1);
        sleep(1);
    }
    write(pipe, "STOP", 4);
    // close the pipe
    close(pipe);
    return 0;
    
}


