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



struct TASK generate_task(uint64_t task_id, char * timing,char * *command_line){  //takes the argument got from the cmd and then transform them into task
    


}




void client_req_creat_task(uint16_t opcode/*, struct TASK task*/){

    int fd;
   /* struct timing time=(task->time);
    struct command_line command=(task->command);*/
   
    uint64_t minutes=124355;     //(time->minutes);
    uint32_t hours= 154144 ;      //(time->hours);
    uint8_t daysofweek= 64;   //(time->daysofweek);

    uint32_t ARGC= 172341 ;       //(command->ARGC);
    //uint32_t *ARGV= (command->ARGV);



    char * myfifo = "./requetes";
    fd=open(myfifo,O_WRONLY);
    write(fd,&opcode,sizeof(opcode));
    write(fd,&ARGC,sizeof(uint32_t));
    write(fd,&minutes,sizeof(minutes));
    write(fd,&hours,sizeof(hours));
    write(fd,&daysofweek,sizeof(daysofweek));
    close(fd);


    
   


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