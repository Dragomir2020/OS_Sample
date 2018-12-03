
//Libraries
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <array>

// Main program used to update the user can get the direct the control flow
int main(int argc, char* argv[]){
    //std::cout << "Number of parameters: " << argv[0] << std::endl;
    
    // First parameter is path and filename
    if(argc == 2){
        const int MAXLINE = 80;
        char line[MAXLINE];
        
        // Find number of files of type c
        FILE *fp = popen("ls *.cpp", "r");
        int i = 0;
        while (fgets(line, MAXLINE, fp) != NULL) {
            //copy line into the array;
            i++;
        }
        pclose(fp);
        std::string st = "find . -type f -name \"*.cpp\" | mailx -s \"" + std::to_string(i) + "\" " + argv[1];
        char * functions = (char *)st.c_str();
        
        //std::cout << "functions " << functions << std::endl;
        FILE * fileParameter = popen(functions, "r");
    } else {
        // Enter only two paramters
        std::cout << "Please enter 1 parameter that is an email address." << std::endl;
    }
    
    return 0;
}


