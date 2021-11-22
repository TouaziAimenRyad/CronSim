#include "../include/includes.h"
#include "../include/lib_client.h"




void client_request_list_tasks(uint16_t opcode ){
    int fd;
    // FIFO file path
    
    char * myfifo = "./requetes";
    fd=open(myfifo,O_WRONLY);
    char * req=malloc(sizeof(uint16_t));
    sprintf(req,"%u",opcode);
    write(fd,req,sizeof(uint16_t));
    close(fd);
    

}



struct TASK generate_task(uint64_t task_id, char * timing,char * command_line){  //takes the argument got from the cmd and then transform them into task


}




void client_req_creat_task(uint16_t opcode){
   


}
