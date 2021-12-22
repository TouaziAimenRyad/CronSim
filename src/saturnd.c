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

    //step4 fork for the second time cause we dont want for session id to be same as process id
    pid=fork();
    if (pid<0){
        exit(EXIT_FAILURE);
    }
    if (pid>0){// terminate parent 
        printf("Parent id %d \n",getpid());
        exit(EXIT_SUCCESS);
    }


    //step5 set the new file permissions that is created by deamon
    umask(077);


    //step6 change the working directory to the root directory if the curent directory is on some mounted file system so deamon process will not let the mounted file system to unmount 
    chdir("/");


    //step 7 close all open file discreptors 
    for (x_fd=sys_conf(_SC_OPEN_MAX);x_fd>=0;x_fd--){
        close(x_fd);
    }

    //final step 8 execute the tasks ??
    while (1)
    {
        sleep(1);
        //execute what you execute 
    }
    
    return 1;

    
}

