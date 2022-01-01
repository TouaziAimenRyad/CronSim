#include "./includes.h"
#ifndef TASK_H
#define TASK_H

struct TASK
{
   // struct TASK *next;   // i literarlly have no idea but apparently when i call for the attribute inside the delete  some weeird stuff happens the only solution is to sacrifce taskid and argc  or turnthemm into strings due to uint problems i guess 
    char *ARGV;
    struct timing time;
    uint64_t task_id;
    uint32_t ARGC;
    
    
    
  
};


void append_task(struct TASK** head_ref, struct TASK *new_data);
void printList(struct TASK **node);
int  delet_task(struct TASK** head, uint64_t taskid); //delet task on success returns 1 on fail returns 0

#endif 
