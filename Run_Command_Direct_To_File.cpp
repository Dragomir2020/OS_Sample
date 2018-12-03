
//Libraries
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

// Main program used to update the user can get the direct the control flow
int main(int argc, char* argv[]){
    //std::cout << "Number of parameters: " << argv[0] << std::endl;
    
    // First parameter is path and filename
    if(argc == 3){
        int pid = fork();
        if(pid == 0){
            // This is a child process
            int file = open(argv[2], O_APPEND | O_WRONLY); // Try to open file
            if(file < 0) return 1;
            //Now we redirect standard output to the file using dup2
            if(dup2(file,1) < 0)    {
                close(file);
                return 1; // this can return 0 normally
            }
            execl( argv[1], argv[1], "-l", (char*)0 );
            close(file);
            exit(0);
        } else {
           // This is a parent process
        }
    } else {
        // Enter only two paramters
        std::cout << "Please enter 1 parameter that is a filename." << std::endl;
    }
    
    return 0;
}


