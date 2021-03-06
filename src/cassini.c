#include "../include/cassini.h"
#include "../include/lib_client.h"
#include "lib_client.c"
const char usage_info[] = "\
   usage: cassini [OPTIONS] -l -> list all tasks\n\
      or: cassini [OPTIONS]    -> same\n\
      or: cassini [OPTIONS] -q -> terminate the daemon\n\
      or: cassini [OPTIONS] -c [-m MINUTES] [-H HOURS] [-d DAYSOFWEEK] COMMAND_NAME [ARG_1] ... [ARG_N]\n\
          -> add a new task and print its TASKID\n\
             format & semantics of the \"timing\" fields defined here:\n\
             https://pubs.opengroup.org/onlinepubs/9699919799/utilities/crontab.html\n\
             default value for each field is \"*\"\n\
      or: cassini [OPTIONS] -r TASKID -> remove a task\n\
      or: cassini [OPTIONS] -x TASKID -> get info (time + exit code) on all the past runs of a task\n\
      or: cassini [OPTIONS] -o TASKID -> get the standard output of the last run of a task\n\
      or: cassini [OPTIONS] -e TASKID -> get the standard error\n\
      or: cassini -h -> display this message\n\
\n\
   options:\n\
     -p PIPES_DIR -> look for the pipes in PIPES_DIR (default: /tmp/<USERNAME>/saturnd/pipes)\n\
";

int main(int argc, char *argv[])
{
  errno = 0;

  char *minutes_str = "*";
  char *hours_str = "*";
  char *daysofweek_str = "*";
  char *pipes_directory = NULL;
  char *command_str;

  uint16_t operation = CLIENT_REQUEST_LIST_TASKS;
  uint64_t taskid;

  int opt;
  char *strtoull_endp;
  while ((opt = getopt(argc, argv, "hlcqm:H:d:p:r:x:o:e:")) != -1)
  {
    switch (opt)
    {
    case 'm':
      minutes_str = optarg;
      break;

    case 'H':
      hours_str = optarg;
      break;

    case 'd':
      daysofweek_str = optarg;
      break;

    case 'p':
      pipes_directory = strdup(optarg);
      if (pipes_directory == NULL)
        goto error;
      break;

    case 'l':
      operation = CLIENT_REQUEST_LIST_TASKS;

      client_request_list_tasks(operation);
      client_get_res_list();

      break;

    case 'c':
      operation = CLIENT_REQUEST_CREATE_TASK;
      // struct task=generate_task();
      // client_req_creat_task(operation,minutes_str,hours_str,daysofweek_str,command_str);
      break;

    case 'q':
      operation = CLIENT_REQUEST_TERMINATE;
      client_request_terminate(operation);
      client_get_res_terminate();
      break;

    case 'r':
      operation = CLIENT_REQUEST_REMOVE_TASK;
      taskid = strtoull(optarg, &strtoull_endp, 10);

      if (strtoull_endp == optarg || strtoull_endp[0] != '\0'){
        goto error;
      }
      client_req_remove_task(operation, taskid);
      client_get_res_remove();

      break;

    case 'x':
      operation = CLIENT_REQUEST_GET_TIMES_AND_EXITCODES;
      taskid = strtoull(optarg, &strtoull_endp, 10);

      if (strtoull_endp == optarg || strtoull_endp[0] != '\0'){
        goto error;
      }
      client_request_get_times_and_exitcodes(operation, taskid);
      client_get_res_time_and_exitcodes();

      break;

    case 'o':
      operation = CLIENT_REQUEST_GET_STDOUT;
      taskid = strtoull(optarg, &strtoull_endp, 10);
     
      if (strtoull_endp == optarg || strtoull_endp[0] != '\0'){
        goto error;
      }
      client_request_get_stdout(operation, taskid);
      client_get_reply_stdout();

      break;

    case 'e':
      operation = CLIENT_REQUEST_GET_STDERR;
      taskid = strtoull(optarg, &strtoull_endp, 10);

      if (strtoull_endp == optarg || strtoull_endp[0] != '\0'){
        goto error;
      }
      client_request_get_stderr(operation, taskid);
      client_get_reply_stderr();

      break;

    case 'h':
      printf("%s", usage_info);
      return 0;

    case '?':
      fprintf(stderr, "%s", usage_info);
      goto error;
    }
  }

  if (operation == CLIENT_REQUEST_CREATE_TASK)
  {

    char *cmd[argc - optind];
    int j = 0;
    int i = optind;
    while (i <= argc - 1)
    {

      cmd[j] = argv[i];
      i++;
      j++;
    }
    //struct command_line command;
    client_req_creat_task(operation, minutes_str, hours_str, daysofweek_str, cmd, argc - optind);
    client_get_res_create();
  }

  return EXIT_SUCCESS;

error:
  if (errno != 0)
    perror("main");
  free(pipes_directory);
  pipes_directory = NULL;
  return EXIT_FAILURE;
}
