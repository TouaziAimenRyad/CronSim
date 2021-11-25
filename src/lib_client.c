#include "../include/includes.h"
#include "../include/lib_client.h"




void client_request_list_tasks(uint16_t opcode ){
    int fd;
    char * myfifo = "./requetes";
    uint16_t opcode2=be16toh(opcode);

    fd=open(myfifo,O_WRONLY);
    write(fd,&opcode2,sizeof(opcode2));
    close(fd);


}



struct TASK generate_task(uint64_t task_id, char * timing,char * command_line){  //takes the argument got from the cmd and then transform them into task


}




void client_req_creat_task(uint16_t opcode){
   


}


void client_req_remove_task(uint16_t opcode,uint64_t task_id){

    int fd;
    char * myfifo = "./requetes";
    uint16_t opcode2=be16toh(opcode);
    fd=open(myfifo,O_WRONLY);
    write(fd,&opcode2,sizeof(opcode2));
    write(fd,&task_id,sizeof(uint64_t));
    close(fd);


}