#include "../include/saturnd.h"


//TODO: make functions in lib_saturnd.c for the treatment of req and replying with res
//this saturnd.c was used for tests during our work 

int main(int argc, char *argv[])
{

    char *req_opcode = malloc(sizeof(uint16_t));
    uint64_t req_task_id;
    char *req_min = malloc(sizeof(uint64_t));
    char *req_hr = malloc(sizeof(uint32_t));
    char *req_dy = malloc(sizeof(uint8_t));
    char *req_cmd = malloc(sizeof(uint32_t));

    char *pathname_req = "./run/pipes/saturnd-request-pipe";
    char *pathname_res = "./run/pipes/saturnd-response-pipe";
    if (mkfifo(pathname_req, 0777) == -1)
    {
        if (errno != EEXIST)
        {
            perror("problem creating fifo_req");
            return 1;
        }
    }
    if (mkfifo(pathname_res, 0777) == -1)
    {
        if (errno != EEXIST)
        {
            perror("problem creating fifo_res");
            return 2;
        }
    }

    while (1)
    {

        int fd_req = open(pathname_req, O_RDONLY);
        int fd_res = open(pathname_res, O_WRONLY);
        read(fd_req, req_opcode, sizeof(uint16_t));
        uint16_t opcode2 = strtol(req_opcode,NULL,16);
        uint16_t opcode = htobe16(opcode2); //needs fixing
        uint16_t reply=be16toh(SERVER_REPLY_OK);
        char* res=malloc(16);
        sprintf(res,"%u",reply);
            close(fd_req);
            write(fd_res,&reply,sizeof(uint16_t));
            printf("reply = %s \n", res);
            break;



        
        
    }
}

