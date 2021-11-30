#include "../include/cassini.h"
#ifndef LIB_H_ //if you change the file name change the guards
#define LIB_H_

// struct TASK generate_task(uint64_t task_id, char * timing,char * command_line);

void client_request_list_tasks(uint16_t opcode);

void client_req_creat_task(uint16_t opcode,char * min, char * hr,char * day ,char * command_line[],int argc);

void client_req_remove_task(uint16_t opcode, uint64_t task_id);

void client_request_terminate(uint16_t opcode);

void client_request_get_times_and_exitcodes(uint16_t opcode, uint64_t taskid);

void client_request_get_stdout(uint16_t opcode, uint64_t taskid);

void client_request_get_stderr(uint16_t opcode, uint64_t taskid);


//---------------------------------------response
void client_get_res_create();
void client_get_res_remove();
void client_get_res_list();


#endif