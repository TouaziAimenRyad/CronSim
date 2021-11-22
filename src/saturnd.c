#include "../include/saturnd.h"


//TODO: make functions in lib_satrund.c for the treatment of req and replying with res  
int main(int argc, char * argv[]) {
    
    char *req=malloc(sizeof(uint16_t));
    char* pathname_req="./requetes";
    char* pathname_res="./reponse";
    mkfifo(pathname_req,0666);
    mkfifo(pathname_res,0666);
    
        int fd_req=open(pathname_req,O_RDONLY);
        read(fd_req,req,sizeof(uint16_t));
        close(fd_req);
        uint16_t req2=atoi(req);
        printf("%u\n",req2);




}