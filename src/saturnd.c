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

 
 struct TASK *table_tasks_head =mmap(table_tasks_head, 100*sizeof (struct TASK), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0); //creating a shared memory between a child a parent  needs to check err 
 int* nbtask= mmap(NULL, sizeof (int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
 *nbtask=0;
 char *pathname_req = "./run/pipes/saturnd-request-pipe";
 char *pathname_res = "./run/pipes/saturnd-reply-pipe";
 int fd_req;
 int fd_res; 

 read_saved_tasks(table_tasks_head,nbtask); //needs to modify if it's empty or doesnt exist so the server doesnt' crash 

 uint16_t opcode=0;
 int taskid=*nbtask;

 pid_t server_pid;
 server_pid = fork();
 if (server_pid<0)
 {
     perror("error while forking the server ");
 }
 if(server_pid==0)
 {
     int fd =open("/home/don/test.txt",O_RDONLY | O_CREAT,0700);
     int status;
     while (1)
     {
         
         char buff[100];
         time_t now;   
         time(&now); //current time t seconds since 1jnv1970
         struct tm now_t = *localtime(&now);

        
         int hours=now_t.tm_hour;
         int min=now_t.tm_min;
         int dy=now_t.tm_wday;
         
         for (int i = 0; i < *nbtask; i++)
         {
           char  min_str[50];
           char  hr_str[50];
           char  dy_str[50];
           char cmd[100];
           strcpy(cmd,table_tasks_head[i].ARGV);
           timing_string_from_field(min_str,0,59,table_tasks_head[i].time.minutes);
           timing_string_from_field(hr_str,0,23,table_tasks_head[i].time.hours);
           timing_string_from_field(dy_str,0,6,table_tasks_head[i].time.daysofweek);
           if((checktime(hours,hr_str)==1)||(checktime(dy,dy_str)==1)||(checktime(min,min_str)==1)&&(table_tasks_head[i].done==0))
           {
            table_tasks_head[i].done=1;
            if(fork()==0)
            {
              execute_task(cmd); 
            }
            //wait(NULL);
            
           }
           
         }
        sleep(60);
     }
     
    
 }
 
 
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
       kill(server_pid,SIGSTOP);
       if(opcode==CLIENT_REQUEST_CREATE_TASK){
         deamon_read_req_creat_task(fd_req,fd_res,(uint64_t)taskid,table_tasks_head,nbtask);
         taskid++;
         close(fd_res);
         close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_REMOVE_TASK){
          deamon_read_req_remove_task(fd_req,fd_res,table_tasks_head,nbtask);
          close(fd_res);
          close(fd_req);
        }
        if(opcode==CLIENT_REQUEST_LIST_TASKS)
        {
         deamon_write_res_list(fd_res,table_tasks_head,*nbtask);
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
         save_tasks(table_tasks_head,(uint32_t)*nbtask);
         deamon_write_res_terminate(fd_req,fd_res,getpid(),server_pid);
         //save other structures for other files 
         //kill the server_pid process and the getpid
        
        }
        kill(server_pid,SIGCONT);
  
   }
 
 
 
 
 
    
    return 1;

    
}

