#include "../include/cassini.h"
#ifndef LIB_H_ //if you change the file name change the guards 
#define LIB_H_


void client_request_list_tasks(uint16_t opcode);

struct TASK generate_task(uint64_t task_id, char * timing,char * command_line);

void client_req_creat_task(uint16_t opcode);










#endif