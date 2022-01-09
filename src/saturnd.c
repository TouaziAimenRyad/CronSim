#include "../include/saturnd.h"
#include "../include/lib_deamon.h"
#include "./lib_deamon.c"


//TODO: make functions in lib_saturnd.c for the treatment of req and replying with res

int main(int argc, char *argv[])
{
 pid_t pid;
 int x_fd;
 //step 1 fork the parent
 pid=fork();
 if(pid<0){ //in case of err
   exit(EXIT_FAILURE);
 }
 if(pid>0){// terminate the parrent 
     exit(EXIT_SUCCESS);
 }
 //step 2 the child process becomes the session leader and processs group leader 
 if(setsid()<0){
     exit(EXIT_FAILURE);
 }
 //step3 catch ignore and handle signals 
 signal(SIGCHLD,SIG_IGN);
 signal(SIGHUP,SIG_IGN);
 //step5 set the new file permissions that is created by deamon
 umask(077);
 //final step 8 execute the tasks ??
 struct TASK table_tasks_head [100] ;
 int nbtask=0;
 char *pathname_req = "./run/pipes/saturnd-request-pipe";
 char *pathname_res = "./run/pipes/saturnd-reply-pipe";
 int fd_req;
 int fd_res; 
 read_saved_tasks(table_tasks_head,&nbtask);
 uint16_t opcode=0;
 int taskid=nbtask;
 while(1){
       fd_req = open(pathname_req, O_RDONLY);
       if(fd_req==-1){
          perror("failed opening of request pipe"); 
          exit(EXIT_FAILURE);
        } 
       
       fd_res = open(pathname_res, O_WRONLY);
       if(fd_res==-1){
          perror("failed opening of request pipe");
          exit(EXIT_FAILURE);
        }
         
       opcode =deamon_read_req_opcode(fd_req);
       if(opcode==CLIENT_REQUEST_CREATE_TASK){
         deamon_read_req_creat_task(fd_req,fd_res,(uint64_t)taskid,table_tasks_head,&nbtask);
         taskid++;
         close(fd_res);
         close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_REMOVE_TASK){
          deamon_read_req_remove_task(fd_req,fd_res,table_tasks_head,&nbtask);
          close(fd_res);
          close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_LIST_TASKS)
        {
         deamon_write_res_list(fd_res,table_tasks_head,nbtask);
         close(fd_res);
         close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_GET_TIMES_AND_EXITCODES)
        {
         //req func here or res 
         close(fd_res);
         close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_GET_STDOUT)
        {
         //req func here or res here 
         close(fd_res);
         close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_GET_STDERR)
        {
         //req func here or res here 
         close(fd_res);
         close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_TERMINATE)
        {
         //req func here or res here 
         save_tasks(table_tasks_head,(uint32_t)nbtask);
         //save other structures for other files 
         close(fd_res);
         close(fd_req);
        }
  
         
 
 }
 
 
    
    return 1;

    
}

