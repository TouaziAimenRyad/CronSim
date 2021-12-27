
#include "../include/task.h"


void append_task(struct TASK** head, struct TASK *new_data)
{
   struct TASK *last = *head; 
   new_data->next = NULL;
   if (*head == NULL)
    {
       *head = new_data;
       return;
    } 
   while (last->next != NULL)
        last = last->next;
    last->next = new_data;

    return;


}