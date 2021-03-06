/* 
* FILE : task_executive.c
* PROJECT : INFO8110 -Assignment #3
* PROGRAMMER : A. Jamil Aryanm
* FIRST VERSION : 2016-22-03
* DESCRIPTION :
* This file implements a cooperative task scheduler for stm32
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "task_executive.h"

#define MAX_TASKS 20
#define TASK_STACK_SIZE 0x100

/* Space for process stacks. */
uint32_t stacks[MAX_TASKS][TASK_STACK_SIZE];
__attribute__((naked)) static void TaskShell(void);

/*Process Control Block to for saving/restoring contexts */
typedef struct PCB_s {
  /* Storage for r4-r14 */
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t sl;
  uint32_t fp;
  uint32_t ip;
  uint32_t lr;
  uint32_t sp;  /* Note location...*/
} PCB_t;


/*  struct for pointing to Task's functions */
typedef struct Task_s {
  void (*f)(void *data);
  /* Task function */
  void *data;
  /* Private data pointerfor this task */
  uint32_t *stack;        
  /* Pointer to top oftask's stack */
  PCB_t pcb;  
 /* Process control block */
} Task_t;

/* Process list */
Task_t  tasks[MAX_TASKS];
int32_t currentTask; 

/* initializes the task's struct */
void TaskInit()
{
  for(int i=0;i<MAX_TASKS; i++)
  {
    tasks[i].f = NULL;
    tasks[i].data = NULL;
  }
  printf("task is initialized\n");
}

/* adds Task function from function list*/
int32_t TaskAdd(void (*f)(void *data), void *data)
{

  /* Fill in initial PCB */
  int i;
  for(i=0; i<MAX_TASKS; i++) 
  {
     memset(&(tasks[i].pcb),0,sizeof(tasks[i].pcb));
     tasks[i].pcb.sp=(uint32_t)(&(stacks[i][TASK_STACK_SIZE-4]));
     tasks[i].pcb.fp=(uint32_t)(&(stacks[i][TASK_STACK_SIZE-4]));
     tasks[i].pcb.lr =(uint32_t)TaskShell;
  /* Put an initial stack frame on too */
     stacks[i][TASK_STACK_SIZE-1]=(uint32_t)(TaskShell);
  /* Try to find an empty slot */
     if(tasks[i].f == NULL)
     {
        tasks[i].f    = f;
        tasks[i].data = data;
        tasks[i].stack = stacks[i];
        return i;
     }
   }
  /* No slots available, return -1 */
    return -1;
}

/* kills a particular function based on id */
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
        /* NOTE: for this expression to work correctly, MAX_TASKS must be an EVEN number */
        i = ((i + 1) % (MAX_TASKS-1))+1;
        count++;
      } while((tasks[i].f == NULL) && (count <= MAX_TASKS));

      return (count <= MAX_TASKS) ? i : -1;
                         /*
                        int32_t i;
                        uint32_t count=0;
                        i = currentTask;
                        do {
                             i = (i + 1) % MAX_TASKS;
                              count++;
                        } 
                        while((tasks[i].f == NULL)
                        && (count <= MAX_TASKS));
                        return (count <= MAX_TASKS) ? i : -1;
                         */
}

/* the main scheduler function that implements context switching */
int32_t TaskSwitcher(void)
{

      int32_t nextTask;
      register uint32_t *currentPCB asm("r0");
      register uint32_t *nextPCB asm("r1");
      //printf("Task switcher called\n\n");
      nextTask = TaskNext();
      if(nextTask < 0)
      {
      /* In the case of no tasks to run,return to the original thread */
        printf("TaskSwitcher(): No tasks to run!\n");
        nextTask = 0;
      }
      /* If the current task is the only one  to be running, just return */
      if(nextTask == currentTask)
        return 0;
      currentPCB = &(tasks[currentTask].pcb.r4);
      nextPCB = &(tasks[nextTask].pcb.r4);
      currentTask = nextTask;
      asm volatile (
        "stm %[current]!,{r4-r12,r14}\n\t"
        "str sp, [%[current]]\n\t"
        "ldm %[next]!,{r4-r12,r14}\n\t"
        "ldr sp, [%[next]]\n\t"
        : /* No Outputs */
        : [current] "r" (currentPCB), [next] "r"
        (nextPCB) /* Inputs */: /* 'No' Clobbers */);
        /* We are now on the other context */
        return 0;
    

                           /*  int32_t currentTask = TaskNext();
                          tasks[currentTask].f(tasks[currentTask].data);
                        return 1;*/
}

/* stub function that does not allow stack access by compiler */
__attribute__((naked)) static void TaskShell(void) 
    {
    /* call the appropriate Task Function */
    tasks[currentTask].f(tasks[currentTask].data);
    /* Terminate this task */
    TaskKill(currentTask);
    /* Call scheduler, Never returns */
    TaskSwitcher();
    }
