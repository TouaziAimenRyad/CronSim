#include "../include/includes.h"
#include "../include/lib_deamon.h"


//aux functions
//we need a function that reads from the tasks storage and return a table of task structure and another one to do the oposit 


//----------------------------------------------------------------------------------------------------------------------------------------------
//response functions  
void deamon_write_res_create(int fd_res,uint64_t taskid)
{
    uint16_t reply_code=be16toh(SERVER_REPLY_OK);
    void * reply=malloc(sizeof(uint64_t)+sizeof(uint16_t));
    *((uint16_t *)reply)=reply_code;
    *((uint64_t *)(reply+16))=taskid;
    write(fd_res,&reply,sizeof(uint64_t)+sizeof(uint16_t));


}

void deamon_write_res_remove(int fd_res,uint16_t reply_code) //reply code depends on the execution of the server it could be err or ok check server_reply.h
{
  
    if(reply_code==SERVER_REPLY_OK)
    {
       uint16_t reply_ok=be16toh(reply_code);
       write(fd_res,&reply_ok,sizeof(uint16_t));

    }
    else
    {
       uint16_t reply_err=be16toh(reply_code);
       uint16_t err_code=be16toh(reply_code);
       void *reply=malloc(2*sizeof(uint16_t));
       *((uint16_t *)reply)=reply_err;
       *((uint16_t *)(reply+16))=err_code;
       write(fd_res,&reply,sizeof(uint16_t));

    }
    


}

void deamon_write_res_list(int fd_res)
{

}




//-------------------------------------------------------------------------------------------------------------------------------------------------



//request functions 

uint16_t deamon_read_req_opcode( int fd_req)
{
   uint16_t opcode;
   read(fd_req,&opcode,sizeof(uint16_t));
   return (opcode);

}



void deamon_read_req_creat_task( int fd_req ,uint64_t taskid , struct TASK  **task_table )
{
  struct TASK new_task;
  struct command_line new_command_ligne;
  int size_timing=sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint8_t);
  void * time =malloc(size_timing);
  uint64_t min;
  uint32_t hr;
  uint8_t dy;
  struct timing *time_struct=malloc(size_timing);
  char* time_str=malloc(size_timing);

  uint32_t argc;
  uint32_t arglen;
  char * argv;
  char * data = malloc(sizeof(uint8_t));
  
  read(fd_req,time,size_timing);
  min=*((uint64_t *)time);
  hr=*((uint32_t *)(time+64));
  hr=*((uint8_t *)(time+64+32));
  min =be64toh(min);
  hr=be32toh(hr);

  time_struct->minutes=min;
  time_struct->hours=hr;
  time_struct->daysofweek=dy;

  printf("test %ld  %d  %d\n",min,hr,dy);
  
  read(fd_req,&argc,sizeof(uint32_t)); 
  argc=be32toh(argc);
  for(int i =0 ; i<argc;i++)
  {
    read(fd_req,&arglen,sizeof(uint32_t));
    arglen=be32toh(arglen);
    argv=malloc(arglen*sizeof(char));
    read(fd_req,argv,arglen*sizeof(char));
    strcat(data,argv);
    strcat(data," ");   

  }  
    
    
    printf("test2 %s \n",data);
    //after we are done reading we execute what needs to be executed from the server 
    //after that we send the response 


  new_task.time=*time_struct;
  new_task.task_id=taskid;
  new_command_ligne.ARGC=argc;
  new_command_ligne.ARGV=data;
  new_task.command=&new_command_ligne;
  
  append_task(task_table,&new_task);
  printf("%lu  %u %u %lu %s\n",new_task.time.minutes,new_task.time.hours,new_task.time.daysofweek,new_task.task_id ,(char*)new_task.command->ARGV);

}

void deamon_read_req_remove_task(int fd_req)
{
    uint64_t taskid;
    read(fd_req,&taskid,sizeof(uint64_t));
    taskid=be64toh(taskid);
    printf("%ld",taskid);
    //after we are done reading we execute what needs to be executed from the server 
    //after that we send the response by calling for funct that handle 

}