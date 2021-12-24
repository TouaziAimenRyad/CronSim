#include "../include/saturnd.h"
#include "../include/lib_deamon.h"
#include "./lib_deamon.c"
int main(int argc, char const *argv[])
{
   char *pathname_req = "./run/pipes/saturnd-request-pipe";
   int fd_req = open(pathname_req, O_RDONLY);

   /* Si la lecture échoute ( fichier inexistant ou autre ) :  */
   if(fd_req==-1)
    {
        /* Message d'erreur  : */
        perror("failed opening of request pipe");

        /* on arrete le programme :*/
        exit(EXIT_FAILURE);
    }

   deamon_read_req_creat_task(fd_req);

   close(fd_req);
}
