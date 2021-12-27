#include "./includes.h"
#ifndef TASK_H
#define TASK_H

struct TASK
{
    uint64_t task_id;
    struct timing time;
    struct command_line *command;
    struct TASK *next;
  
};

void append_task(struct TASK** head_ref, struct TASK *new_data);


#endif 
