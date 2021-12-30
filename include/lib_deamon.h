#include "saturnd.h"
#ifndef LIB_D_H
#define LIB_D_H

// Responses headers :

void deamon_write_res_create(int fd_res,uint64_t taskid);

void deamon_write_res_remove(int fd_res,uint16_t reply_code);

void deamon_write_res_list(int fd_res);

void demon_write_res_stdout(int fd, uint32_t reponse_code);

void demon_write_res_stderr(int fd, uint32_t reponse_code);



// Request headers :

uint16_t deamon_read_req_opcode(int fd_req);

void deamon_read_req_creat_task(int fd_req);

void deamon_read_req_remove_task(int fd_req);

void demon_read_request_stdout_task(int fd);

void demon_read_request_stderr_task(int fd);


#endif /* LIB_D_H*/
