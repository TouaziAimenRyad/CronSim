
#include "../include/task.h"


//switch case in the sat file is what's causing pblm

void printList(struct TASK **head) 
{
   struct TASK *temp;
   //temp=*head;
  
	//while (temp != NULL) {
		
             printf("in prin taskid %lu\n",(*head)->task_id);
             printf("in prin time %s\n",(*head)->time);
             
             printf(" inprin  argv %s\n",(*head)->ARGV);
		//temp= temp->next;
	//}
   //free(temp);temp=NULL;
}

/*this was working 
void append_task(struct TASK** head, struct TASK *new_data)
{
   struct TASK *last = *head; 
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
 */
int delet_task(struct TASK **head, uint64_t id)//problm with argc of the structure
{
   printf("in taskid %lu\n",(*head)->task_id);
             printf("in  time %s\n",(*head)->time);
             
             printf(" in  argv %s\n",(*head)->ARGV);
   
   
   
    return 0;
}