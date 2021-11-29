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
    
    int command_line_size=0;
    for(int j=0;j<argc;j++){
        command_line_size+=strlen(command_line[j]);

    }
    command_line_size=command_line_size*sizeof(uint8_t);

    int size=sizeof(uint16_t)+sizeof(uint64_t)+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+argc*sizeof(uint32_t)+command_line_size;
    void * content=malloc(size);
    int offset=0;


    uint16_t opcode2 = be16toh(opcode);
    uint32_t aux_argc = htobe32(  (uint32_t) (argc) );
    

    if(timing_from_strings(&time,min,hr,day)==-1){
        perror("error while making time structure");
        exit(EXIT_FAILURE);
    };
    uint64_t minutes = htobe64(time.minutes);
    uint32_t hours =  htobe32(time.hours);
    uint8_t daysofweek =(time.daysofweek);

    char *myfifo = "./run/pipes/saturnd-request-pipe";
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    *((uint16_t *)content)=opcode2;
    offset=offset+sizeof(uint16_t);
    *((uint64_t *)(content+offset))=minutes;
    offset=offset+sizeof(uint64_t);
    *((uint32_t *)(content+offset))=hours;
    offset=offset+sizeof(uint32_t);
    *((uint8_t *)(content+offset))=daysofweek;
     offset=offset+sizeof(uint8_t);
    *((uint32_t *)(content+offset))=aux_argc;
    offset=offset+sizeof(uint32_t);
   for (int i = 0; i < argc; i++)
    { 
         int argv_len=strlen(command_line[i]);
         uint32_t aux_argv_len = htobe32((uint32_t) argv_len );
         *((uint32_t *)(content+offset))=aux_argv_len;
         offset=offset+sizeof(uint32_t);

         uint32_t argv=convert_char_to_uint32(command_line[i]);
          strcpy((char *)(content+offset),command_line[i]);
         //*((uint32_t *)(content+offset))=htobe32(argv);
         offset=offset+sizeof(uint8_t)*argv_len;
         
         
    }
     
    write(fd,content,size);

    
    close(fd);
}

// Remove

void client_req_remove_task(uint16_t opcode, uint64_t task_id)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opc = be16toh(opcode);
    uint64_t taskid=htobe64(task_id);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
       perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opc, sizeof(opc));
    write(fd, &taskid, sizeof(uint64_t));
    close(fd);
}

// ('OUT') : STDOUT -- Cette fonction affichera la sortie standard de la dernière exécution de la tâche :

void client_request_get_stdout(uint16_t opcode, uint64_t task_id)
{

    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opc = be16toh(opcode);
    uint64_t taskid=htobe64(task_id);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opc, sizeof(opc));
    write(fd, &taskid, sizeof(uint64_t));

    close(fd);
}

// ('SE') : STDERR -- Cette fonction affichera la sortie erreur standard de la dernière exécution de
// la tâche :

void client_request_get_stderr(uint16_t opcode, uint64_t task_id)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opc = be16toh(opcode);
    uint64_t taskid=htobe64(task_id);
    fd = open(myfifo, O_WRONLY);
    if(fd==-1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, &opc, sizeof(opc));
    write(fd, &taskid, sizeof(uint64_t));
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

void client_request_get_times_and_exitcodes(uint16_t opcode, uint64_t task_id)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opcode2 = be16toh(opcode);
    uint64_t taskid=htobe64(task_id);
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

