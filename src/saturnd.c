#include "../include/saturnd.h"


//TODO: make functions in lib_satrund.c for the treatment of req and replying with res  
int main(int argc, char * argv[]) {
    
  
    char *req_opcode=malloc(sizeof(uint16_t));
    uint64_t req_task_id;
    char* pathname_req="./requetes";
    char* pathname_res="./reponse";
    if (mkfifo(pathname_req,0777)==-1)
    {
        if(errno!=EEXIST){
            perror("problem creating fifo_req");
            return 1;
        }
    }
    if(mkfifo(pathname_res,0777)==-1)
    {
        if(errno!=EEXIST){
            perror("problem creating fifo_res");
            return 2;
        }
    }
     
     while(1){
    
        int fd_req=open(pathname_req,O_RDONLY);
        read(fd_req,req_opcode,sizeof(uint16_t));
        read(fd_req,&req_task_id,sizeof(uint64_t));
        close(fd_req);
        //write(1,req,sizeof(uint16_t));
        printf("opcode = %s \n",req_opcode);
        printf("taskid = %lu \n",req_task_id);


}