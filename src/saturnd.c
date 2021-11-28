#include "../include/saturnd.h"
#include "../include/command.h"
#include "command.c"

//TODO: make functions in lib_satrund.c for the treatment of req and replying with res
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
        read(fd_req, req_opcode, sizeof(uint16_t));
        uint16_t opcode2 = convert_char_to_uint16(req_opcode);
        uint16_t opcode = htobe16(opcode2); //needs fixing
        switch (opcode)
        {
        case (0x4c53):
            close(fd_req);
            printf("opcode = %s \n", req_opcode);
            break;

        case (0x524d):
            read(fd_req, &req_task_id, sizeof(uint64_t));
            close(fd_req);
            printf("opcode = %s \n", req_opcode);
            printf("taskid = %lu \n", req_task_id);
            break;

        case (0x5458):
            read(fd_req, &req_task_id, sizeof(uint64_t));
            close(fd_req);
            printf("opcode = %s \n", req_opcode);
            printf("taskid = %lu \n", req_task_id);
            break;

        case (0x544d):
            close(fd_req);
            printf("opcode = %s \n", req_opcode);
            break;

        case (0x4352):
            read(fd_req, req_cmd, sizeof(uint32_t));
            read(fd_req, req_min, sizeof(uint64_t));
            read(fd_req, req_hr, sizeof(uint32_t));
            read(fd_req, req_dy, sizeof(uint8_t));

            close(fd_req);
            //write(1,req,sizeof(uint16_t));
            printf("opcode = %s \n", req_opcode);
            printf("command = %s \n", req_cmd);
            printf("miniut = %s \n", req_min);
            printf("hour = %s \n", req_hr);
            printf("day= %s \n", req_dy);
            break;

        case (0x534f):
            read(fd_req, &req_task_id, sizeof(uint64_t));
            close(fd_req);
            printf("opcode = %s \n", req_opcode);
            printf("taskid = %lu \n", req_task_id);
            break;

        case (0x5345):
            read(fd_req, &req_task_id, sizeof(uint64_t));
            close(fd_req);
            printf("opcode = %s \n", req_opcode);
            printf("taskid = %lu \n", req_task_id);
            break;
        }
    }
}

