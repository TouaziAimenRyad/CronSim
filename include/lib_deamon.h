#include "saturnd.h"
#ifndef LIB_D_H
#define LIB_D_H

// Fonctions auxilliaires :  

void save_tasks( struct TASK  *task_table ,uint32_t nbtask);

void read_saved_tasks(struct TASK  *task_table ,int *nbtask);

int checktime(int current_time ,char* time_str);

void execute_task(char * argv);

// Response headers

void deamon_write_res_create(int fd_res,uint64_t taskid);

void deamon_write_res_remove(int fd_res,uint16_t reply_code);

void deamon_write_res_list(int fd_res,struct TASK  *task_table ,uint32_t nbtask);

void deamon_write_res_terminate(int fd_req,int fd_res,int pid1,int pid2);

void deamon_write_res_time_ans_exitcodes(int fd_res, uint16_t reply_code, uint32_t nb_run, int64_t time,uint16_t exitcode);



void demon_write_res_stderr(int fd_res, uint16_t reponse_code, uint32_t output, uint16_t erreur_code);

void demon_write_res_stdout(int fd_res, uint16_t reponse_code, uint32_t output, uint16_t erreur_code);




// Request headers

uint16_t deamon_read_req_opcode(int fd_req);

void deamon_read_req_creat_task(int fd_req,int fd_res,uint64_t taskid , struct TASK  *task_table ,int *nbtask);

void deamon_read_req_remove_task(int fd_req,int fd_res, struct TASK  *task_table,int* nbtask);

<<<<<<< HEAD
void demon_read_request_stderr_task(int fd);
=======
void demon_read_request_stdout_task(int fd_req,int fd_res);

void demon_read_request_stderr_task(int fd_req,int fd_res);
>>>>>>> functionstdoutstderr

void demon_read_request_time_and_exitcodes(int fd_req,int fd_res);



#endif /* LIB_D_H*/
