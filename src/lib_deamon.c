#include "../include/includes.h"
#include "../include/lib_deamon.h"


//aux functions
//we need a function that reads from the tasks storage and return a table of task structure and another one to do the oposit 


//----------------------------------------------------------------------------------------------------------------------------------------------
//response functions  
void deamon_write_res_create(int fd_res,uint64_t taskid)
{
    uint16_t reply_code=be16toh(SERVER_REPLY_OK);
    taskid=be64toh(taskid);
   // write(1,&reply_code,sizeof(uint16_t)); rani naktab ok tssama ki na9raha nal9a ko
    write(fd_res,&reply_code,sizeof(uint16_t));
    write(fd_res,&taskid,sizeof(uint64_t));
    //this is not working and i 'dont know why s
   /* void * reply=malloc(sizeof(uint64_t)+sizeof(uint16_t));
    *((uint16_t *)reply)=reply_code;
    *((uint64_t *)(reply+sizeof(uint16_t)))=taskid;
    write(fd_res,&reply,sizeof(uint64_t)+sizeof(uint16_t));*/


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
       uint16_t err_code=be16toh(SERVER_REPLY_ERROR_NOT_FOUND);
       write(fd_res,&reply_err,sizeof(uint16_t));
       write(fd_res,&err_code,sizeof(uint16_t));
       /*void *reply=malloc(2*sizeof(uint16_t));
       *((uint16_t *)reply)=reply_err;
       *((uint16_t *)(reply+16))=err_code;
       write(fd_res,&reply,sizeof(uint16_t));*/

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
   opcode=be16toh(opcode);
   return (opcode);

}



void deamon_read_req_creat_task( int fd_req ,int fd_res,uint64_t taskid , struct TASK  *task_table ,int *nbtask)
{
  struct TASK new_task;
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
  timing_string_from_timing( time_str, time_struct);
  
  
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
    
    
    
    //after we are done reading we execute what needs to be executed from the server 
    //after that we send the response 
  


  new_task.time=time_str;
  new_task.task_id=taskid;
  new_task.ARGC=argc;
  new_task.ARGV=data;
  //new_task.next = NULL;

  //append_task(task_table,&new_task);

  task_table[*nbtask]=new_task;
 
   
  deamon_write_res_create(fd_res,taskid);

}

void deamon_read_req_remove_task(int fd_req ,int fd_res, struct TASK  *task_table)
{
    uint64_t taskid;
    read(fd_req,&taskid,sizeof(uint64_t));
    taskid=be64toh(taskid);
    printf(" id to remove   %lu\n",taskid);
    //printf("test in %s \n",(char *)(((*task_table).command)->ARGV));
    //int delet_flag = delet_task(task_table,taskid);
    /*if (delet_flag==1)
    {
      deamon_write_res_remove(fd_res,SERVER_REPLY_OK);
    }
    else
    {
      deamon_write_res_remove(fd_res,SERVER_REPLY_ERROR);
    }*/
    

    //after we are done reading we execute what needs to be executed from the server 
    //after that we send the response by calling for funct that handle 

}