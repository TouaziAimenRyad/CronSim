#include "saturnd.h"
#ifndef LIB_D_H
#define LIB_D_H

//aux functions 




//response headers
void deamon_write_res_create(int fd_res,uint64_t taskid);
void deamon_write_res_remove(int fd_res,uint16_t reply_code);
void deamon_write_res_list(int fd_res);




//request headers

uint16_t deamon_read_req_opcode(int fd_req);

void deamon_read_req_creat_task(int fd_req,int fd_res,uint64_t taskid , struct TASK  **task_table );

void deamon_read_req_remove_task(int fd_req,int fd_res, struct TASK  **task_table);




#endif /* LIB_D_H*/
