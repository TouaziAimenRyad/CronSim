#include "../include/includes.h"
#include "../include/lib_deamon.h"


//aux functions
// We need a function that reads from the tasks storage and return a table of task structure and another one to do the oposit 
void save_tasks( struct TASK  *task_table ,uint32_t nbtask){
  struct stat st ;
  char * directory="/home/don/DataStore";
  char  filename[500];
  
  uint64_t taskid;
  uint64_t min;
  uint32_t hr;
  uint8_t dy;
  uint32_t argc;
  int done;
  char * argv;
  if (stat(directory,&st)==-1){
    if (mkdir(directory,0700)==-1){
      perror("couldn't creat data store ");
      exit(1);
    }
  }
  sprintf(filename,"%s/%s",directory,"TASK_STORE");
  int fd=open(filename,O_WRONLY | O_CREAT,0700);
  if(fd==-1)
  {
    perror("couldn't open file");
    exit(1);
  }
  
  write(fd,&nbtask,sizeof(uint32_t));
  for (int i = 0; i < nbtask; i++)
  {
    taskid=task_table[i].task_id;
    done=task_table[i].done;
    argc=task_table[i].ARGC;
    argv=task_table[i].ARGV;
    min=task_table[i].time.minutes;
    hr=task_table[i].time.hours;
    dy=task_table[i].time.daysofweek;
    uint64_t tasksize=2*sizeof(uint64_t)+2*sizeof(uint32_t)+sizeof(uint8_t)+strlen(argv)+sizeof(int);
    void *task =malloc(tasksize+sizeof(uint64_t));
    int offest=0;
    *((uint64_t *) (task+offest))=tasksize;
    offest+=sizeof(uint64_t);
    *((int*) (task+offest))=done;
    offest+=sizeof(int);
    *((uint64_t *) (task+offest))=taskid;
     offest+=sizeof(uint64_t);
    *((uint64_t *) (task+offest))=min;
     offest+=sizeof(uint64_t);
    *((uint32_t *) (task+offest))=hr;
     offest+=sizeof(uint32_t);
    *((uint8_t *) (task+offest))=dy;
     offest+=sizeof(uint8_t);
    *((uint32_t *) (task+offest))=argc;
     offest+=sizeof(uint32_t);
    strcpy((char *)(task + offest), argv);

    write(fd,task,(sizeof(uint64_t)+tasksize));
    
    free(task);task=NULL;

   
  }

  close(fd);


}

void read_saved_tasks(struct TASK  *task_table ,int *nbtask)
{
  struct stat st ;
  char * directory="/home/don/DataStore";
  char  filename[500];
  uint32_t nb;
  uint64_t taskid;
  int done;
  struct timing time ;
  char* str_time=malloc(sizeof(uint64_t)+sizeof(uint8_t)+sizeof(uint32_t));
  uint64_t min;
  uint32_t hr;
  uint8_t dy;
  uint32_t argc;
  char * argv;
  long tasksize;

  if (stat(directory,&st)==-1){
      perror("couldn't find data store ");
      exit(1);
  }
  sprintf(filename,"%s/%s",directory,"TASK_STORE");
  int fd=open(filename,O_RDONLY | O_APPEND | O_CREAT,0700);
  if(fd==-1)
  {
    perror("couldn't open file");
    exit(1);
  } 

  if(read(fd,&nb,sizeof(uint32_t))!=0){
    
     *nbtask=(int)nb;
     for (int i = 0; i < nb; i++)
     {
       read(fd,&tasksize,sizeof(uint64_t));
       void *task=malloc((long)tasksize);
       read(fd,task,(long )tasksize);
       done=*((int *)task);
       taskid=*((uint64_t *)(task+sizeof(int)));
       min=*((uint64_t *)(task+sizeof(int)+sizeof(uint64_t)));
       hr=*((uint32_t *)(task+sizeof(int)+sizeof(uint64_t)+sizeof(uint64_t)));
       dy=*((uint8_t *)(task+sizeof(int)+sizeof(uint64_t)+sizeof(uint64_t)+sizeof(uint32_t)));
       argc=*((uint32_t *)(task+sizeof(int)+sizeof(uint64_t)+sizeof(uint64_t)+sizeof(uint32_t)+sizeof(uint8_t)));
       argv=((char *)(task+sizeof(int)+sizeof(uint64_t)+sizeof(uint64_t)+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)));
       time.minutes=min;
       time.hours=hr;
       time.daysofweek=dy;
       task_table[i].task_id=taskid;
       task_table[i].time=time;
       task_table[i].ARGC=argc;
       strcpy(task_table[i].ARGV,argv);
       timing_string_from_timing(str_time,&time);
       
     
       free(task);task=NULL;
     
     }


  }
  
  free(str_time);str_time=NULL;
  close(fd);

}



int checktime(int current_time ,char* time_str)
{  
    int ntime [20];
    char m[20];
    char buff[20][20];
    char buff2[20][20];
    int i =0;
    int z=0;
    int trouv=0;
    strcpy(m,time_str);
    if(strcmp(m,"*")==0)
    {
        return 0;
    }
    else
    {  
        //break the time_str on the ,
        char *token;
        token = strtok(m, ",");
        while( token != NULL ) 
        {
         strcpy(buff[i],token);
         i++;
         token = strtok(NULL, ",");
        } 
        
       
        //tranform the string table into int tabl with interperate the x-y cases
        for(int j=0;j<i;j++)
        {
            if(strlen(buff[j])>2)
            {
               char *token2;
               int o=0;
               token2 = strtok(buff[j], "-");
               while( token2 != NULL ) 
               {
                 strcpy(buff2[o],token2);
                 o++;
                 token2 = strtok(NULL, "-");
               } 
               int min =atoi(buff2[0]);
               int max =atoi(buff2[1]);
               for(int t=min;t<=max;t++){
                   ntime[z]=t;
                   z++;
               }
                
            }
            else
            {
             ntime[z]=atoi(buff[j]);
             z++;
            }
                
        }
        //check if current_time exist in this function
        int u=0;
        while((u<z)&&(trouv==0))
        {
            if (ntime[u]==current_time) trouv=1;
            u++;
        }
         
        return trouv;
    }
    
}


void execute_task(char * argv){
  char cmd[20][50];
  char argv2[20][50];
  char buff[80];
  int i=0;
  strcpy(buff,argv);
  char *token;
  token = strtok(buff, " ");
  while( token != NULL ) 
  {
    strcpy(cmd[i],token);
    i++;
    token = strtok(NULL, ",");
  }
  for (int j = 0; j < i; j++)
  {
    strcpy(argv2[j],cmd[j+1]);
  }
 
}


//------------------------------------------------------------------------------------
// Response functions :

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

// Les valeurs possibles pour ERRCODE sont :
//  - 0x4e46 ('NF') : il n'existe aucune tâche avec cet identifiant
//  - 0x4e52 ('NR') : la tâche n'a pas encore été exécutée au moins une fois

// Stdout :


void demon_write_res_stdout(int fd_res, uint16_t reponse_code, uint32_t output, uint16_t erreur_code){
// Si la réponse envoyée par le serveur est ok alors :
if (reponse_code == SERVER_REPLY_OK){
uint16_t reponse_ok = htobe32(reponse_code);
unit32_t outoupt2 = htobe32(output);
// On écrit dans notre fifo :
write(fd_res, &reponse_ok, sizeof(uint16_t));
write(fd_res, &outoupt2, sizeof(uint32_t));

}else{
// Sinon : ( la réponse envoyée par le serveur est erreur alors ) :
uint16_t reponse_erreur = htobe16(0x4552);
uint16_t error_code = htobe16(reponse_code);
void *reponse = malloc(sizeof(uint16_t));
*((uint16_t *)reponse) = reponse_erreur;
*((uint16_t *)(reponse + 16)) = error_code;
write(fd_res, &reponse, 2*sizeof(uint16_t));
}
}


// Stderr :


void demon_write_res_stderr(int fd_res, uint16_t reponse_code, uint32_t output, uint16_t erreur_code){
// Si la réponse envoyée par le serveur est ok alors :
if (reponse_code == SERVER_REPLY_OK){
uint16_t reponse_ok = htobe32(reponse_code);
unit32_t outoupt2 = htobe32(output);
// On écrit dans notre fifo :
write(fd_res, &reponse_ok, sizeof(uint16_t));
write(fd_res, &outoupt2, sizeof(uint32_t));

}else{
// Sinon : ( la réponse envoyée par le serveur est erreur alors ) :
uint16_t reponse_erreur = htobe16(0x4552);
uint16_t error_code = htobe16(reponse_code);
void *reponse = malloc(sizeof(uint16_t));
*((uint16_t *)reponse) = reponse_erreur;
*((uint16_t *)(reponse + 16)) = error_code;
write(fd_res, &reponse, 2*sizeof(uint16_t));
}
}

//--------------------------------------------------------------------------------------------

// Request functions 

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
  hr=*((uint32_t *)(time+sizeof(uint64_t)));
  dy=*((uint8_t *)(time+sizeof(uint64_t)+sizeof(uint32_t)));
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
  strcpy(new_task.ARGV,data);//new_task.ARGV=data;
  new_task.done=0;
  
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


// Read stdout :

void demon_read_request_stdout_task(int fd_req,int fd_res){
uint64_t task_id;
read(fd_req,&task_id,sizeof(uint64_t));
task_id = be64toh(task_id);
printf("%ld",task_id);

demon_write_res_stdout(fd_res,0x4f4b,11,0x4552);

}

// Read sterr : 

void demon_read_request_stderr_task(int fd_req,int fd_res){
uint64_t task_id;
read(fd_req,&task_id,sizeof(uint64_t));
task_id = be64toh(task_id);
printf("%ld",task_id);

demon_write_res_stderr(fd_res,0x4f4b,11,0x4552);
}
