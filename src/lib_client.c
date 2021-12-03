#include "../include/includes.h"
#include "../include/lib_client.h"

// struct TASK generate_task(uint64_t task_id, char *timing, char **command_line)
// { //takes the argument got from the cmd and then transform them into task
// }

/**
 * Cette fonction envoie une requet du client au démon pour lui demander de ister toutes les tâches
 * @param opcode
 *
 */
void client_request_list_tasks(uint16_t opcode)
{
    /* Déclaration du descripteur de notre fichier : */
    int fd;

    /* Création d'un pointeur myfifo vers un char " pipe "  */
    char *myfifo = "./run/pipes/saturnd-request-pipe";

    /* Caster le opcode en uint16_t et le task_id en uint64_t : */
    uint16_t opcode2 = be16toh(opcode);

    /* On ouvre notre pipe en écriture :  */
    fd = open(myfifo, O_WRONLY);

    /* Si l'ouverture échoue : */
    if (fd == -1)
    {

        /* Message d'erreur */
        perror("failed opening of request pipe");

        /* On arrete notre programme */
        exit(EXIT_FAILURE);
    }

    /* On écrit notre content dans notre fichier :  */
    write(fd, &opcode2, sizeof(opcode2));

    /* On ferme nle descripteur */
    close(fd);
}

// Creat :

void client_req_creat_task(uint16_t opcode, char *min, char *hr, char *day, char *command_line[], int argc)
{

    int fd;
    struct timing time;

    int command_line_size = 0;
    for (int j = 0; j < argc; j++)
    {
        command_line_size += strlen(command_line[j]);
    }
    command_line_size = command_line_size * sizeof(uint8_t);

    int size = sizeof(uint16_t) + sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint32_t) + argc * sizeof(uint32_t) + command_line_size;
    void *content = malloc(size);
    int offset = 0;

    uint16_t opcode2 = be16toh(opcode);
    uint32_t aux_argc = htobe32((uint32_t)(argc));

    if (timing_from_strings(&time, min, hr, day) == -1)
    {
        perror("error while making time structure");
        exit(EXIT_FAILURE);
    };
    uint64_t minutes = htobe64(time.minutes);
    uint32_t hours = htobe32(time.hours);
    uint8_t daysofweek = (time.daysofweek);

    char *myfifo = "./run/pipes/saturnd-request-pipe";
    fd = open(myfifo, O_WRONLY);
    if (fd == -1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    *((uint16_t *)content) = opcode2;
    offset = offset + sizeof(uint16_t);
    *((uint64_t *)(content + offset)) = minutes;
    offset = offset + sizeof(uint64_t);
    *((uint32_t *)(content + offset)) = hours;
    offset = offset + sizeof(uint32_t);
    *((uint8_t *)(content + offset)) = daysofweek;
    offset = offset + sizeof(uint8_t);
    *((uint32_t *)(content + offset)) = aux_argc;
    offset = offset + sizeof(uint32_t);
    for (int i = 0; i < argc; i++)
    {
        int argv_len = strlen(command_line[i]);
        uint32_t aux_argv_len = htobe32((uint32_t)argv_len);
        *((uint32_t *)(content + offset)) = aux_argv_len;
        offset = offset + sizeof(uint32_t);

        uint32_t argv = convert_char_to_uint32(command_line[i]);
        strcpy((char *)(content + offset), command_line[i]);
        //*((uint32_t *)(content+offset))=htobe32(argv);
        offset = offset + sizeof(uint8_t) * argv_len;
    }

    write(fd, content, size);
    close(fd);

    client_get_res_create(opcode);
}

/**
 * Cette fonction envoie une requete du client au démon pour lui demander de supprimer une tâche :
 * @param opcode
 * @param task_id
 *
 */

void client_req_remove_task(uint16_t opcode, uint64_t task_id)
{
    /* Déclaration du descripteur de notre fichier : */
    int fd;

    /* Création d'un pointeur myfifo vers un char " pipe "  */
    char *myfifo = "./run/pipes/saturnd-request-pipe";

    /* Caster le opcode en uint16_t et le task_id en uint64_t : */
    uint16_t opc = be16toh(opcode);
    uint64_t taskid = htobe64(task_id);

    /* Déclaration d'un pointeur générique : */
    void *content = malloc(sizeof(uint16_t) + sizeof(uint64_t));

    /* On met notre opc dans content ( 16 premiers uint avec le opcode), et à partir de 16 et pour 64 autres uint ( le task_id) */
    *((uint16_t *)content) = opc;
    *((uint64_t *)(content + sizeof(uint16_t))) = taskid;

    /* On ouvre notre pipe en écriture :  */
    fd = open(myfifo, O_WRONLY);

    /* Si l'ouverture échoue : */
    if (fd == -1)
    {
        /* Message d'erreur */
        perror("failed opening of request pipe");

        /* On arrete notre programme */
        exit(EXIT_FAILURE);
    }

    /* On écrit notre content dans notre fichier :  */
    write(fd, content, sizeof(uint16_t) + sizeof(uint64_t));

    /* On ferme     le descripteur */
    close(fd);
}

/**
 * Cette fonction envooe une requete du client au démon pour lui demander d'afficher
 * la sortie standard de la dernière exécution de la tâche .
 * @param opcode
 * @param task_id
 *
 */

void client_request_get_stdout(uint16_t opcode, uint64_t task_id)
{
    /* Déclaration du descripteur de notre fichier : */
    int fd;

    /* Création d'un pointeur myfifo vers un char " pipe "  */
    char *myfifo = "./run/pipes/saturnd-request-pipe";

    /* Caster le opcode en uint16_t et le task_id en uint64_t : */
    uint16_t opc = be16toh(opcode);
    uint64_t taskid = htobe64(task_id);

    /* Déclaration d'un pointeur générique : */
    void *content = malloc(sizeof(uint16_t) + sizeof(uint64_t));

    /* On met notre opc dans content ( 16 premiers uint avec le opcode), et à partir de 16 et pour 64 autres uint ( le task_id) */
    *((uint16_t *)content) = opc;
    *((uint64_t *)(content + sizeof(uint16_t))) = taskid;

    /* On ouvre notre pipe en écriture :  */
    fd = open(myfifo, O_WRONLY);

    /* Si l'ouverture échoue : */

    if (fd == -1)
    {
        /* Message d'erreur : */
        perror("failed opening of request pipe");

        /* On arrete notre programme : */
        exit(EXIT_FAILURE);
    }

    /* On écrit notre content dans notre fichier :  */
    write(fd, content, sizeof(uint16_t) + sizeof(uint64_t));

    /* On ferme le descripteur :  */
    close(fd);
}

/**
 * Cette fonction envoie une requet du client au démon pour lui demander d'afficher la sortie erreur
 * standard de la dernière exécution de la tâche :
 * @param opcode
 * @param task_id
 *
 */

void client_request_get_stderr(uint16_t opcode, uint64_t task_id)
{
    /* Déclaration du descripteur de notre fichier : */
    int fd;

    /* Création d'un pointeur myfifo vers un char " pipe "  */
    char *myfifo = "./run/pipes/saturnd-request-pipe";

    /* Caster le opcode en uint16_t et le task_id en uint64_t : */
    uint16_t op = be16toh(opcode);
    uint64_t taskid = htobe64(task_id);

    /* Déclaration d'un pointeur générique : */
    void *content = malloc(sizeof(uint16_t) + sizeof(uint64_t));

    /* On met notre opc dans content ( 16 premiers uint avec le opcode), et à partir de 16 et pour 64 autres uint ( le task_id) */
    *((uint16_t *)content) = op;
    *((uint64_t *)(content + sizeof(uint16_t))) = taskid;

    /* On ouvre notre pipe en écriture :  */
    fd = open(myfifo, O_WRONLY);

    /* Si l'ouverture échoue : */
    if (fd == -1)
    {
        /* Message d'erreur : */
        perror("failed opening of request pipe");

        /* On arrete notre programme : */
        exit(EXIT_FAILURE);
    }

    /* On écrit notre content dans notre fichier :  */
    write(fd, content, sizeof(uint16_t) + sizeof(uint64_t));

    /* On ferme le descipteur : */
    close(fd);
}

// Terminate :

void client_request_terminate(uint16_t opcode)
{
    int fd;
    char *myfifo = "./run/pipes/saturnd-request-pipe";
    uint16_t opcode2 = be16toh(opcode);
    fd = open(myfifo, O_WRONLY);
    if (fd == -1)
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
    uint64_t taskid = htobe64(task_id);
    void *content = malloc(sizeof(uint16_t) + sizeof(uint64_t));

    *((uint16_t *)content) = opcode2;
    *((uint64_t *)(content + sizeof(uint16_t))) = taskid;
    fd = open(myfifo, O_WRONLY);
    if (fd == -1)
    {
        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    write(fd, content, sizeof(uint16_t) + sizeof(uint64_t));
    close(fd);
}

/**
 * Cette fonction traite la réponse à la requete stdout :
 *
 */

void client_get_reply_stdout()
{

    /* On récupère la réponse du fichier saturnd-reply-pipe */
    char *chemin = "./run/pipes/saturnd-reply-pipe";

    /* Pointeur générique : */
    uint16_t *opcode = malloc(sizeof(uint16_t));

    /* On ouvre le fichier de réponse : */
    int fd = open(chemin, O_RDONLY);

    /* Si la lecture échoute ( fichier inexistant ou autre ) : */
    if (fd == -1)
    {

        perror("erreur");
        exit(EXIT_FAILURE);
    }

    /* On va lire notre fichier : */
    read(fd, opcode, sizeof(uint16_t));

    if (*opcode == be16toh(SERVER_REPLY_ERROR))
    {
        uint16_t erreur_code;
        read(fd, &erreur_code, sizeof(uint16_t));
        erreur_code = be16toh(erreur_code);
        if ((erreur_code == SERVER_REPLY_ERROR_NOT_FOUND) || (erreur_code == SERVER_REPLY_ERROR_NEVER_RUN))
        {
            printf("%x", erreur_code);
            exit(1);
        }
         
    } else {

        uint32_t size_output;
        read(fd, &size_output, sizeof(uint32_t));
        size_output = be32toh(size_output);
        char *output = malloc(size_output * sizeof(uint8_t));
        read(fd, output, size_output);
        printf("%s", output);
        free(output);
        exit(0);
    }

    /* Fermeture du descripteur : */
    close(fd);
}

/**
 * Cette fonction traite la réponse du serveur à la requete stderr :
 *
 */

void client_get_reply_stderr()
{

    /* On récupère la réponse du fichier saturnd-reply-pipe */
    char *chemin = "./run/pipes/saturnd-reply-pipe";

    /* Pointeur générique : */
    uint16_t *opcode = malloc(sizeof(uint16_t));

    /* On vérifie si l'allocation en mémoire a été faite correctement : */
    if (opcode == NULL)
    {
        exit(1);
    }

    /* On ouvre le fichier de réponse : */
    int fd = open(chemin, O_RDONLY);

    /* Si la lecture échoute ( fichier inexistant ou autre ) :  */
    if (fd == -1)
    {

        /* Affiche une erreur  : */
        perror("erreur");

        /* On arrete notre programme : */
        exit(EXIT_FAILURE);
    }

    /* On va lire notre fichier : */
    read(fd, opcode,sizeof(uint16_t));

    if (*opcode == be32toh(SERVER_REPLY_ERROR))
    {
        uint16_t erreur_code;
        read(fd, &erreur_code, sizeof(uint16_t));
        erreur_code = be16toh(erreur_code);
        if ((erreur_code == SERVER_REPLY_ERROR_NOT_FOUND) || (erreur_code == SERVER_REPLY_ERROR_NEVER_RUN))
        {
            printf("%x", erreur_code);
            perror(" Erreur ! ");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        uint32_t size_output;
        read(fd, &size_output,sizeof(uint32_t));
        size_output=be32toh(size_output);
        char *output = malloc(size_output * sizeof(uint8_t));
        read(fd, output, size_output);
        printf("%s", output);
        free(output);
        exit(0);
    }

    // Fermeture du descripteur :
    close(fd);
}


void client_get_res_create(){
  char *pathname_res = "./run/pipes/saturnd-reply-pipe";
  int size=sizeof(uint16_t)+sizeof(uint64_t);
  uint16_t restype;
  uint64_t taskid;
  int fd_res = open(pathname_res, O_RDONLY);
   if(fd_res==-1)
    {
       
        perror("failed opening of request pipe");

       
        exit(EXIT_FAILURE);
    }

    read(fd_res,&restype,sizeof(uint16_t));
    if (restype==be16toh(SERVER_REPLY_OK))
    {
        read(fd_res,&taskid,sizeof(uint64_t));
        printf("%ld",be64toh(taskid));
        exit(0);
    }
    
    close(fd_res);

   
    
    
    
    
    
}


void client_get_res_remove(){
  char *pathname_res = "./run/pipes/saturnd-reply-pipe";
 uint16_t error;
 uint16_t restype;
  int fd_res = open(pathname_res, O_RDONLY);
   if(fd_res==-1)
    {
       
        perror("failed opening of request pipe");

       
        exit(EXIT_FAILURE);
    }

    read(fd_res,&restype,sizeof(uint16_t));
    close(fd_res);

   
    if (restype==be16toh(SERVER_REPLY_ERROR))
    {
        read(fd_res,&error,sizeof(uint16_t));
        error=be16toh(error);
        printf("%x",error);
        exit(0);
    }
    
   
    
}


void client_get_res_list(){
  char *pathname_res = "./run/pipes/saturnd-reply-pipe";
  
  int size_timing=sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint8_t);
  struct timing *time=malloc(size_timing);
  char* time_str=malloc(size_timing);
  char* task_str=malloc(sizeof(uint64_t)+size_timing);
  uint64_t min;
  uint32_t hr;
  uint8_t dy;
  uint16_t reptype;
  uint32_t nbtask;
  uint64_t taskid;
  uint32_t argc;
  uint32_t arglen;
  
  int fd_res = open(pathname_res, O_RDONLY);
   if(fd_res==-1)
    {
       
        perror("failed opening of request pipe");

       
        exit(EXIT_FAILURE);
    }

    read(fd_res,&reptype,sizeof(uint16_t));
    if (reptype==be16toh(SERVER_REPLY_OK))
    {
        read(fd_res,&nbtask,sizeof(uint32_t));
        nbtask=be32toh(nbtask);
        if(nbtask>0)
        {
            for (int i = 0; i < nbtask; i++)
            {
                read(fd_res,&taskid,sizeof(uint64_t));
                taskid=be64toh(taskid);
                read(fd_res,&min,sizeof(uint64_t));
                read(fd_res,&hr,sizeof(uint32_t));
                read(fd_res,&dy,sizeof(uint8_t));
                time->minutes=be64toh(min);
                time->hours=be32toh(hr);
                time->daysofweek=dy;

                timing_string_from_timing(time_str,time);

                sprintf(task_str,"%ld: %s",taskid,time_str);
                read(fd_res,&argc,sizeof(uint32_t));
                for(int j=0;j<be32toh(argc);j++)
                {
                   read(fd_res,&arglen,sizeof(uint32_t));
                   arglen=be32toh(arglen);
                   char * argv=malloc(arglen*sizeof(uint8_t));
                   read(fd_res,argv,arglen*sizeof(uint8_t));
                   strcat(task_str," ");
                   strcat(task_str,argv);
                   
                   
                }

                printf("%s\n",task_str);

            }
            
        }
        


    }
    
   
    
    close(fd_res);
    exit(0);
    
}
void client_get_res_terminate(){
    char *pathname_res = "./run/pipes/saturnd-reply-pipe";
    void *res = malloc(sizeof(uint16_t));
    int fd_res = open(pathname_res, O_RDONLY);
    if (fd_res == -1)
    {

        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }

    read(fd_res, res,sizeof(uint16_t));


}
void client_get_res_time_and_exitcodes()
{
    char *pathname_res = "./run/pipes/saturnd-reply-pipe";
    void *reptype = malloc(sizeof(uint16_t));
    uint32_t nb_run;
    int64_t time;
    uint16_t exitcode;
    uint16_t errcode;

    int fd_res = open(pathname_res, O_RDONLY);
    if (fd_res == -1)
    {

        perror("failed opening of request pipe");
        exit(EXIT_FAILURE);
    }
    read(fd_res,reptype,sizeof(uint16_t));
     if (*((uint16_t *)reptype)== be16toh(SERVER_REPLY_OK))
     {
        read(fd_res, &nb_run, sizeof(uint32_t));
        nb_run =(int) be32toh(nb_run);

        for(int i=0;i<nb_run;i++)
        {
          read(fd_res, &time, sizeof(int64_t));
          time = be64toh(time);

          read(fd_res, &exitcode, sizeof(uint16_t));
          exitcode = be16toh(exitcode);
          
          time = (time_t)time;
          struct tm *timeInfo = localtime(&time);

          if (timeInfo == NULL)
          {
            perror("failed reading of request pipe");
            exit(EXIT_FAILURE);
          }
          printf("%02d-%02d-%02d %02d:%02d:%02d %d\n",
                 timeInfo->tm_year + 1900, timeInfo->tm_mon + 1,
                 timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min,
                 timeInfo->tm_sec, exitcode);

          
        }
      }
      else if(*((uint16_t *)reptype) == be16toh(SERVER_REPLY_ERROR))
      {
        read(fd_res, &errcode, sizeof(uint16_t));
        if ((errcode == SERVER_REPLY_ERROR_NOT_FOUND))
        {
          printf("%u", errcode);
          exit(1);
        }
        else
          exit(1);
      }

      }
