#include "../include/includes.h"
#include "../include/lib_deamon.h"


//response functions  
void deamon_write_res_create(int fd_res,uint64_t taskid)
{
    uint16_t reply_code=be16toh(SERVER_REPLY_OK);
    void * reply=malloc(sizeof(uint64_t)+sizeof(uint16_t));
    *((uint16_t *)reply)=reply_code;
    *((uint64_t *)(reply+16))=taskid;
    write(fd_res,&reply,sizeof(uint64_t)+sizeof(uint16_t));



}

void deamon_write_res_remove(int fd_res,uint16_t reply_code) //reply code depends on the execution of the server it could be err or ok check server_reply.h
{
  
    if(reply_code==SERVER_REPLY_OK)
    {
       uint16_t reply_ok=be16toh(reply_code);
       write(fd_res,&reply_ok,sizeof(uint16_t));

    }
    else
    {
       uint16_t reply_err=be16toh(reply_code);
       uint16_t err_code=be16toh(reply_code);
       void *reply=malloc(2*sizeof(uint16_t));
       *((uint16_t *)reply)=reply_err;
       *((uint16_t *)(reply+16))=err_code;
       write(fd_res,&reply,sizeof(uint16_t));

    }
    


}

void deamon_write_res_list(int fd_res)
{

}




//request functions 