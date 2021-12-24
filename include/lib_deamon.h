#include "saturnd.h"
#ifndef LIB_D_H
#define LIB_D_H

//response headers

//-----------------------------------------------------

//request headers


uint16_t deamon_read_req_opcode(int fd_req);

void deamon_read_req_creat_task(int fd_req);

void deamon_read_req_remove_task(int fd_req);




#endif /* LIB_D_H*/
