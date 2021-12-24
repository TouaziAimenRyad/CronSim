#include "../include/includes.h"
#include "../include/lib_deamon.h"


//response functions  
void deamon_write_res_create(uint64_t taskid)
{
   char *pathname_res = "./run/pipes/saturnd-reply-pipe";
   int fd_res = open(pathname_res, O_RDONLY);
   if(fd_res==-1)
    {
       
        perror("failed opening of request pipe");

       
        exit(EXIT_FAILURE);
    }

    uint16_t reply_code=be16toh(SERVER_REPLY_OK);
    void * reply=malloc(sizeof(uint64_t)+sizeof(uint16_t));
    *((uint16_t *)reply)=reply_code;
    *((uint64_t *)(reply+16))=taskid;
    write(fd_res,&reply,sizeof(uint64_t)+sizeof(uint16_t));



}

void deamon_write_res_remove(uint16_t reply_code) //reply code depends on the execution of the server it could be err or ok check server_reply.h
{
   char *pathname_res = "./run/pipes/saturnd-reply-pipe";
   int fd_res = open(pathname_res, O_RDONLY);
   if(fd_res==-1)
    {
       
        perror("failed opening of request pipe");

       
        exit(EXIT_FAILURE);
    }
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

void deamon_write_res_list()
{

}

//---------------------------------------------------------------------------------------------------------------


//request functions 

uint16_t deamon_read_req_opcode()
{
   char *pathname_req = "./run/pipes/saturnd-request-pipe";
   uint16_t opcode;
   
   /* On ouvre le fichier de réponse en lecture : */
   int fd_req = open(pathname_req, O_RDONLY);

   /* Si la lecture échoute ( fichier inexistant ou autre ) :  */
   if(fd_req==-1)
    {
        /* Message d'erreur  : */
        perror("failed opening of request pipe");

        /* on arrete le programme :*/
        exit(EXIT_FAILURE);
    }

    read(fd_req,&opcode,sizeof(uint16_t));


    return be16toh(opcode);

}

void deamon_read_req_creat_task()
{
  char *pathname_req = "./run/pipes/saturnd-request-pipe";

  int size_timing=sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint8_t);
  void * time_req = malloc(size_timing);
  struct timing *time=malloc(size_timing);
  char* time_str=malloc(size_timing);
  uint64_t min;
  uint32_t hr;
  uint8_t dy;

  uint32_t argc;
  uint32_t arglen;
  char * data = "";

  /* On ouvre le fichier de réponse en lecture : */
  int fd_req = open(pathname_req, O_RDONLY);

  /* Si la lecture échoute ( fichier inexistant ou autre ) :  */
  if(fd_req==-1)
    {
        /* Message d'erreur  : */
        perror("failed opening of request pipe");

        /* on arrete le programme :*/
        exit(EXIT_FAILURE);
    }

    read(fd_req,time_req,sizeof(size_timing));
    min = *(( uint64_t *)time_req);;
    hr = *(( uint32_t *)(time_req+64));
    dy = *(( uint8_t *)(time_req+32));

    time->minutes=be64toh(min);
    time->hours=be32toh(hr);
    time->daysofweek=dy;
    timing_string_from_timing(time_str,time);

    read(fd_req,&argc,sizeof(uint32_t));
    argc=be32toh(argc);
    
    for(int i =0 ; i<argc;i++)
    {
       read(fd_req,&arglen,sizeof(uint32_t));
       arglen=be32toh(arglen);
       char * argv=malloc(arglen*sizeof(uint8_t));
       read(fd_req,argv,arglen*sizeof(uint8_t));
       strcat(data,argv);
       strcat(data," ");

    }  

    //after we are done reading we execute what needs to be executed from the server 
    //after that we send the response 


}

void deamon_read_req_remove_task()
{
   char *pathname_req = "./run/pipes/saturnd-request-pipe";
   uint64_t taskid;

   int fd_req = open(pathname_req, O_RDONLY);

  /* Si la lecture échoute ( fichier inexistant ou autre ) :  */
  if(fd_req==-1)
    {
        /* Message d'erreur  : */
        perror("failed opening of request pipe");

        /* on arrete le programme :*/
        exit(EXIT_FAILURE);
    }

    read(fd_req,&taskid,sizeof(uint64_t));

    //after we are done reading we execute what needs to be executed from the server 
    //after that we send the response by calling for funct that handle 

}