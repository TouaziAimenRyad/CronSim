#include "../include/includes.h"
#include "../include/lib_client.h"

// struct TASK generate_task(uint64_t task_id, char *timing, char **command_line)
// { //takes the argument got from the cmd and then transform them into task
// }

// List :
void client_request_list_tasks(uint16_t opcode)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opcode2 = be16toh(opcode);

    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opcode2, sizeof(opcode2));
    close(fd);
}

// Creat :

void client_req_creat_task(uint16_t opcode ,char * min, char * hr,char * day ,char * command_line[],int argc)
{

    int fd;
    struct timing time;
    
    char *str=malloc(sizeof(uint32_t));

    uint16_t opcode2 = be16toh(opcode);
     u_int32_t aux_argc = htobe32(  (u_int32_t) (argc-1) );
    

    if(timing_from_strings(&time,min,hr,day)==-1){
        perror("error while making time structure");
        exit(EXIT_FAILURE);
    };
    u_int64_t minutes = le64toh(time.minutes);
    u_int32_t hours = le32toh(time.hours);
    u_int8_t daysofweek =le16toh(time.daysofweek);

    char *myfifo = "./run/pipes/saturnd-request-pipe";
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opcode2, sizeof(opcode2));
    write(fd, &aux_argc, sizeof(aux_argc));
    write(fd, &minutes, sizeof(minutes));
    write(fd, &hours, sizeof(hours));
    write(fd, &daysofweek, sizeof(daysofweek));
    int size=0;
   for (int i = 0; i < argc; i++)
    { 
         uint32_t aux_argv_len = htobe32((uint32_t) strlen(command_line[i]));
         write(fd, &aux_argv_len, sizeof(aux_argv_len));
         write(fd, command_line[i], strlen(command_line[i]));
        
    }
   
    
    close(fd);
}

// Remove

void client_req_remove_task(uint16_t opcode, uint64_t task_id)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opc = be16toh(opcode);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
       perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opc, sizeof(opc));
    write(fd, &task_id, sizeof(uint64_t));
    close(fd);
}

// ('OUT') : STDOUT -- Cette fonction affichera la sortie standard de la dernière exécution de la tâche :

void client_request_get_stdout(uint16_t opcode, u_int64_t task_id)
{

    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opc = be16toh(opcode);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opc, sizeof(opc));
    write(fd, &task_id, sizeof(uint64_t));

    close(fd);
}

// ('SE') : STDERR -- Cette fonction affichera la sortie erreur standard de la dernière exécution de
// la tâche :

void client_request_get_stderr(uint16_t opcode, u_int64_t task_id)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opc = be16toh(opcode);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opc, sizeof(opc));
    write(fd, &task_id, sizeof(uint64_t));
}

// Terminate :

void client_request_terminate(uint16_t opcode)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opcode2 = be16toh(opcode);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opcode2, sizeof(opcode2));
    close(fd);
}

// Get_times_and_exitcodes :

void client_request_get_times_and_exitcodes(uint16_t opcode, uint64_t taskid)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opcode2 = be16toh(opcode);
    //uint64_t task_id= be64toh(taskid);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opcode2, sizeof(opcode2));
    write(fd, &taskid, sizeof(uint64_t));
    close(fd);
}

