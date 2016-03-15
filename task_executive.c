/* 
* FILE : task_executive.c
* PROJECT : INFO8110 -Assignment #2
* PROGRAMMER : A. Jamil Aryanm
* FIRST VERSION : 2016-29-02
* DESCRIPTION :
* This file implements a simple task scheduler for the board
*/

#include <stdio.h>
#include <stdint.h>
#include "task_executive.h"

#define MAX_TASKS 20

// 
typedef struct Task_s {
   void (*f)(void *data); /* Task function */
   void *data;  /* Private data pointer for this task */
} Task_t;

/* Process list */
Task_t  tasks[MAX_TASKS];
int32_t currentTask; 

void TaskInit()
{
  for(int i=0;i<MAX_TASKS; i++)
  {
    tasks[i].f = NULL;
    tasks[i].data = NULL;
  }
  printf("task is initialized\n");
}


int32_t TaskAdd(void (*f)(void *data), void *data)
{
  /* Try to find an empty slot */
  int i;
  for(i=0; i<MAX_TASKS; i++) 
  {
     if(tasks[i].f == NULL)
     {
        tasks[i].f    = f;
        tasks[i].data = data;
        return i;
     }
   }
  /* No slots available, return -1 */
    return -1;
}


int32_t TaskKill(int32_t id)
{
   if(tasks[id].f==NULL)
   {
      puts("NULL id");
      return -1;
   }
   tasks[id].f = NULL;
   tasks[id].data = NULL;
   printf("Task[%d] was killed!\n",(int)id);
   return 0;   
}


/* Find the next task to run */
static int32_t TaskNext(void)
{
   int32_t i;
   uint32_t count=0;
   i = currentTask;
   do {
      i = (i + 1) % MAX_TASKS;
      count++;
   } while((tasks[i].f == NULL)&& (count <= MAX_TASKS));
  return (count <= MAX_TASKS) ? i : -1;
}


int32_t TaskSwitcher(void)
{
   currentTask = TaskNext();
   if(currentTask<0){
      puts("vault empty!\n");
      return -1;
   }
   tasks[currentTask].f(tasks[currentTask].data);
  return 1;
}
