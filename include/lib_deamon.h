#include "saturnd.h"
#ifndef LIB_D_H
#define LIB_D_H

//response headers
void deamon_write_res_create(int fd_res,uint64_t taskid);
void deamon_write_res_remove(int fd_res,uint16_t reply_code);
void deamon_write_res_list(int fd_res);



//request headers



#endif /* LIB_D_H*/
