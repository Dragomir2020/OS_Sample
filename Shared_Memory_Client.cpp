
//Libraries
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream> // Filestream for reading text file
#include <sys/shm.h> // Shared memory segment
#include <sys/ipc.h> // Generates unique key
#define MAX_LINE 100

const int SHM_SIZE = 1024;  /* make it a 1K shared memory segment */
const char FILE_NAME[] = "sample.txt";

// Returns with info needed to clean up mapped memory
struct MappedMemory{
    void* data;
    int shmid;
};

// Creates mapped memory
MappedMemory create_shared_memory(const char fileName[], int fTokIndex){
    key_t key;
    int shmid;
    char *data;
    
    /* make the key: */
    if ((key = ftok(fileName, fTokIndex)) == -1) {
        perror("ftok");
        exit(1);
    }
    
    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }
    
    /* attach to the segment to get a pointer to it: */
    void *hold;
    hold = shmat(shmid, (void *)0, 0);
    data =(char *)hold;
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    // Save data to struct
    MappedMemory mm;
    mm.data = hold;
    mm.shmid = shmid;
    return mm;
    
}

void clean_up_mapped_memory(MappedMemory mm){
    /* detach from the segment: */
    if (shmdt(mm.data) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    /* delete the IPC structure */
    shmctl(mm.shmid, IPC_RMID, NULL);
}

// Main program
int main(int argc, char* argv[]){

    MappedMemory mm1 = create_shared_memory(FILE_NAME, 1);
    char* data = (char *)mm1.data;
    
    MappedMemory mm2 = create_shared_memory(FILE_NAME, 2);
    int * arr = (int *)mm2.data;
    
    std::ifstream file(FILE_NAME);
    char buffer[MAX_LINE];
    std::string str;
    while (std::getline(file, str))
    {
        // actually write out the data and close the pipe
        std::cout << str << std::endl;
        strncpy(data, str.c_str(), SHM_SIZE);
        arr[0] = arr[0] + 1;
        sleep(1);
    }
    
    strncpy(data, "STOP", SHM_SIZE);
    
    clean_up_mapped_memory(mm1);
    clean_up_mapped_memory(mm2);
    
    return 0;
    

}


