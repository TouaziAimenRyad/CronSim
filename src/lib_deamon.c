#include "../include/includes.h"
#include "../include/lib_deamon.h"


//aux functions
//we need a function that reads from the tasks storage and return a table of task structure and another one to do the oposit 

void get_argvs(char* argv, char * list[])
{ 
  int len=strlen(argv);
  int i=0;
  int j=0;
  int t=0;
  while (i<len)
  {
    if(*(argv+i)!=' ')
    {
      list[j][t]=*(argv+i);
      t++;
    }
    else
    {
      t=0;
      j++;
    }
    i++;
  }
  

}


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

void deamon_write_res_list(int fd_res ,struct TASK  *task_table,uint32_t nbtask)
{
  
  uint64_t min; 
  uint32_t hr;
  uint8_t dy ;

  struct timing time_struct;

  uint16_t reptype=htobe16(SERVER_REPLY_OK);//
  uint32_t nb_task= htobe32(nbtask);//
  uint64_t taskid;
  uint32_t argc;
  char * argv="";
  char * argv_list="";
  
  //printf("%x    %u\n",reptype,nb_task);

  write(fd_res,&reptype,sizeof(uint16_t));
  write(fd_res,&nb_task,sizeof(uint32_t)); //this pblm
  /*size_t offset=0;
  *((uint16_t * )task)=reptype;
  offset+=sizeof(uint16_t);
  *((uint32_t * )task+offset)=nb_task;
  offset+=sizeof(uint32_t);*/

  for (int i = 0; i < nbtask; i++)
  {
    taskid=htobe64(task_table[i].task_id);//
    write(fd_res,&taskid,sizeof(uint64_t));//this pblm
   /* *((uint64_t * )task+offset)=taskid;
    offset+=sizeof(uint64_t);*/

    time_struct=task_table[i].time;
    min=htobe64(time_struct.minutes);//
    write(fd_res,&min,sizeof(uint64_t));//this pblm
    /**((uint64_t * )task+offset)=min;
    offset+=sizeof(uint64_t);*/

    hr=htobe32( time_struct.hours);//
    write(fd_res,&hr,sizeof(uint32_t));//this pblm
    /* *((uint32_t * )task+offset)=hr;
    offset+=sizeof(uint32_t);*/

    dy=time_struct.daysofweek;//
    write(fd_res,&dy,sizeof(uint8_t));//this pblm
    /**((uint8_t * )task+offset)=dy;
    offset+=sizeof(uint8_t);*/

    argc=htobe32( task_table[i].ARGC);//
    write(fd_res,&argc,sizeof(uint32_t));//this pblm
    /**((uint32_t * )task+offset)=argc;
    offset+=sizeof(uint32_t);*/
    
    //printf("%lu   %u   %u    %u\n",min,hr,dy,argc);
    char *argv_temp=malloc(strlen(task_table[i].ARGV));
    strcpy(argv_temp,task_table[i].ARGV);
    char * token=strtok(argv_temp," "); //strtok is modifying the argument
    while (token!=NULL)
    {
      char *arg=token; 
      uint32_t arglen=(uint32_t)strlen(arg); 
      uint32_t arglen2=htobe32(arglen);
      write(fd_res,&arglen2,sizeof(uint32_t));
      write(fd_res,arg,arglen*sizeof(uint8_t));
    /* *((uint32_t * )task+offset)=arglen;
     offset+=sizeof(uint32_t);
     strcat((char *)task,arg);*/
      
      //strcat(argv,arg);

      //strcat(argv_list,argv);
      //printf(" %s   \n",argv);
      token=strtok(NULL," ");
    }
    
    free(argv_temp);
    argv_temp=NULL;
  

    
  }
  
  


 
 
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





void deamon_read_req_creat_task( int fd_req ,int fd_res,uint64_t taskid , struct TASK  *task_table ,int *nbtask) //this works
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
  char * data = malloc(sizeof(uint8_t)); //this cant be freed cause we saving the adress pointer in the stask structure 
  
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
    free(argv);
    argv=NULL; 

  }  
    
    
  new_task.time=*time_struct;
  new_task.task_id=taskid;
  new_task.ARGC=argc;
  new_task.ARGV=data;
  
  int pos=*nbtask;
  task_table[pos]=new_task;
  (*nbtask)++;
 
 free(time);time=NULL;
 free(time_str);time_str=NULL;
 free(time_struct);time_struct=NULL;
   
  deamon_write_res_create(fd_res,taskid);

}




void deamon_read_req_remove_task(int fd_req ,int fd_res, struct TASK  *task_table ,int * nbtask)  //this works
{
    uint64_t taskid;
    read(fd_req,&taskid,sizeof(uint64_t));
    taskid=be64toh(taskid);
    printf(" id to remove   %lu\n",taskid);
    int i=0;
    int trouv=0;
    while (i<*nbtask && trouv==0)
    {
      if (task_table[i].task_id==taskid)
      {
        trouv=1;
      }else
      {
        i++;
      }
      
    }
    if(trouv==1)
    {
      for (int j = i; j < *nbtask-1; j++)
      {
        task_table[i]=task_table[i+1];
      }

      *nbtask=*nbtask-1;
      deamon_write_res_remove(fd_res,SERVER_REPLY_OK);
      
    }
    else
    {
      deamon_write_res_remove(fd_res,SERVER_REPLY_ERROR);
    }
    

}