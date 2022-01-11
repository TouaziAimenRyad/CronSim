#include "../include/includes.h"
#include "../include/lib_deamon.h"

// Responses functions :

// Creat : 

void deamon_write_res_create(int fd_res, uint64_t taskid)
{
  uint16_t reply_code = be16toh(SERVER_REPLY_OK);
  void *reply = malloc(sizeof(uint64_t) + sizeof(uint16_t));
  *((uint16_t *)reply) = reply_code;
  *((uint64_t *)(reply + 16)) = taskid;
  write(fd_res, &reply, sizeof(uint64_t) + sizeof(uint16_t));
}

// Remove : 

// reply code depends on the execution of the server it could be err or ok check server_reply.h

void deamon_write_res_remove(int fd_res, uint16_t reply_code) 
{

  if (reply_code == SERVER_REPLY_OK)
  {
    uint16_t reply_ok = be16toh(reply_code);
    write(fd_res, &reply_ok, sizeof(uint16_t));
  }
  else
  {
    uint16_t reply_err = be16toh(reply_code);
    uint16_t err_code = be16toh(reply_code);
    void *reply = malloc(2 * sizeof(uint16_t));
    *((uint16_t *)reply) = reply_err;
    *((uint16_t *)(reply + 16)) = err_code;
    write(fd_res, &reply, sizeof(uint16_t));
  }
}


//  List : 

void deamon_write_res_list(int fd_res)
{
}

/ Stdout :

void demon_write_res_stdout(int fd, uint32_t reponse_code)
{
  // Si la réponse envoyée par le serveur est ok alors 

  if (reponse_code == SERVER_REPLY_OK)
  {
    uint16_t reponse_ok = be32toh(reponse_code);
    // On écrit dans notre fifo : 
    write(fd, &reponse_ok, sizeof(uint32_t));
  }
  else
  {
    // Sinon : ( la réponse vnoyée par le serveur est erreur alors )
    uint16_t reponse_erreur = be16toh(reponse_code);
    uint16_t error_code = be16toh(reponse_code);
    void *reponse = malloc(sizeof(uint16_t));
    *((uint16_t *)reponse) = reponse_erreur;
    *((uint16_t *)(reponse + 16)) = error_code;
    write(fd, &reponse, sizeof(uint16_t));
  }
}

// Stderr :

void demon_write_res_stderr(int fd, uint32_t reponse_code)
{
  
  // Si la réponse envoyée par le serveur est ok alors 
  if (reponse_code == SERVER_REPLY_OK)
  {
    uint16_t reponse_ok = be32toh(reponse_code);
    // On écrit dans notre fifo  la réponse envoyée par le serveur :
    write(fd, &reponse_ok, sizeof(uint32_t));
  }
  else
  {
    // Sinon : ( la réponse envoyée est erreur  ) :
    uint16_t reponse_erreur = be16toh(reponse_code);
    uint16_t error_code = be16toh(reponse_code);
    void *reponse = malloc(sizeof(uint16_t));
    *((uint16_t *)reponse) = reponse_erreur;
    *((uint16_t *)(reponse + 16)) = error_code;
    write(fd, &reponse, sizeof(uint16_t));
  }
}

//  ----------------------------------------------------------------------------------------------

// Request functions :


// Read opcode :

uint16_t deamon_read_req_opcode(int fd_req)
{
  uint16_t opcode;
  read(fd_req, &opcode, sizeof(uint16_t));
  return (opcode);
}


// Read creat : 

void deamon_read_req_creat_task(int fd_req)
{

  int size_timing = sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint8_t);
  void *time = malloc(size_timing);
  uint64_t min;
  uint32_t hr;
  uint8_t dy;
  struct timing *time_struct = malloc(size_timing);
  //char *time_str = malloc(size_timing);

  uint32_t argc;
  uint32_t arglen;
  char *argv;
  char *data = malloc(sizeof(uint8_t));

  read(fd_req, time, size_timing);
  min = *((uint64_t *)time);
  hr = *((uint32_t *)(time + 64));
  hr = *((uint8_t *)(time + 64 + 32));
  min = be64toh(min);
  hr = be32toh(hr);

  time_struct->minutes = min;
  time_struct->hours = hr;
  time_struct->daysofweek = dy;

  printf("%ld  %d  %d\n", min, hr, dy);

  read(fd_req, &argc, sizeof(uint32_t));
  argc = be32toh(argc);
  for (int i = 0; i < argc; i++)
  {
    read(fd_req, &arglen, sizeof(uint32_t));
    arglen = be32toh(arglen);
    argv = malloc(arglen * sizeof(char));
    read(fd_req, argv, arglen * sizeof(char));
    strcat(data, argv);
    strcat(data, " ");
  }

  printf("%s ", data);
  // after we are done reading we execute what needs to be executed from the server
  // after that we send the response
}

// Read remove : 

void deamon_read_req_remove_task(int fd_req)
{
  uint64_t taskid;
  read(fd_req, &taskid, sizeof(uint64_t));
  taskid = be64toh(taskid);
  printf("%ld", taskid);
  // after we are done reading we execute what needs to be executed from the server
  // after that we send the response by calling for funct that handle
}


// Read stdout :

void demon_read_request_stdout_task(int fd){
  uint64_t task_id;
  read(fd,&task_id,sizeof(uint64_t));
  task_id = be64toh(task_id);
  printf("%ld",task_id);
}

// Read sterr : 

void demon_read_request_stderr_task(int fd){
  uint64_t task_id;
  read(fd,&task_id,sizeof(uint64_t));
  task_id = be64toh(task_id);
  printf("%ld",task_id);
}