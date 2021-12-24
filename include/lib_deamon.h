#include "saturnd.h"
#ifndef LIB_D_H
#define LIB_D_H

//response headers
void deamon_write_res_create();
void deamon_write_res_remove();
void deamon_write_res_list();

//request headers


uint16_t deamon_read_req_opcode();

void deamon_read_req_creat_task();

void deamon_read_req_remove_task();




#endif /* LIB_D_H*/
