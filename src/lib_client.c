#include "../include/includes.h"
#include "../include/lib_client.h"




void client_request_list_tasks(uint16_t opcode ){
    int fd;
    char * myfifo = "./requetes";
    uint16_t opcode2=be16toh(opcode);

    fd=open(myfifo,O_WRONLY);
<<<<<<< HEAD
    write(fd,&opcode2,sizeof(opcode2));
=======
    write(fd,&opcode,sizeof(opcode));
>>>>>>> 8bd63a37ba1ddf524f3f9d99f3a0371af380b924
    close(fd);


}



struct TASK generate_task(uint64_t task_id, char * timing,char * command_line){  //takes the argument got from the cmd and then transform them into task


}




void client_req_creat_task(uint16_t opcode){
   


}
