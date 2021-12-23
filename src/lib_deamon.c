#include "../include/includes.h"
#include "../include/lib_deamon.h"


//response functions  


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