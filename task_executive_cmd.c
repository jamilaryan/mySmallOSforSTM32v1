/* 
* FILE : task_executive_cmd.c
* PROJECT : INFO8110 -Assignment #2
* PROGRAMMER : A. Jamil Aryan
* FIRST VERSION : 2016-22-03
* DESCRIPTION :
* This file implements the individual tasks and commands
* to be executed from minicom
*/

#include <stdio.h>
#include <stdint.h>
#include "common.h"
#include "task_executive.h"
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "stdlib.h"
#include "stm32f3_discovery_accelerometer.h"
#include "stm32f3_discovery_gyroscope.h"


uint32_t  r;      // random number
int16_t xyz[3];   // variable for polling sensors

/* turns leds on randomly */
void TaskLedOn(void* data)
{
  r = rand() % 8;

  printf("Hello from TaskLedOn\n");
  
  BSP_LED_On(r);
  TaskSwitcher();
  
}

/*turns leds off randomly */
void TaskLedOff(void* data)
{
  TaskSwitcher();
  r = rand() % 8;

  BSP_LED_Off(r);
  printf("Hello from TaskLedOff\n");
  
}

/* implements accelerometer */
void TaskAccel(void* data)
{
  TaskSwitcher();
  BSP_ACCELERO_GetXYZ(xyz);

  printf("Accelerometer returns:\n"
	 "   X: %d\n"
	 "   Y: %d\n"
	 "   Z: %d\n",
	 xyz[0],xyz[1],xyz[2]);

  TaskSwitcher();
}

void TaskPB(void* data)
{
  uint32_t button;
  button = BSP_PB_GetState(BUTTON_USER);

  if(button)
  {
    printf("\n button was pressed!!\n");
    for(int i=0;i<=7;i++)
    {
      BSP_LED_On(i);
    }
  }

  TaskSwitcher();
}

void TaskGyro(void* data)
{
  float xyz[3];

  BSP_GYRO_GetXYZ(xyz);

  printf("Gyroscope returns:\n"
	 "   X: %d\n"
	 "   Y: %d\n"
	 "   Z: %d\n",
	 (int)(xyz[0]*256),
	 (int)(xyz[1]*256),
	 (int)(xyz[2]*256));

  TaskSwitcher();
}

void CmdTasks(int mode)
{
  if(mode != CMD_INTERACTIVE) {
    return;
  }
  printf("Starting testing task switch\n");

  TaskInit();
  
  printf("TaskPB added, id = %d\n",(int)TaskAdd(TaskPB,0));  
  printf("TaskLedOff added, id = %d\n",(int) TaskAdd(TaskLedOff,0));
  printf("TaskLedOn added, id = %d\n",(int)TaskAdd(TaskLedOn,0));
  printf("TaskGyro added, id = %d\n",(int)TaskAdd(TaskGyro,0));
  printf("TaskAccel added, id = %d\n",(int)TaskAdd(TaskAccel,0));
  
  TaskSwitcher();

/*
  
  for (int i=0; i<100; i++)
  {   
     if(TaskSwitcher() == -1)
        break;
  }
*/
} 
ADD_CMD("tasktest",CmdTasks,"<index> <state> Implements a basic scheduler")


void CmdTaskAdd(int mode)
{
  if(mode != CMD_INTERACTIVE) {
    return;
  }

  TaskInit();
  printf("TaskPB added, id = %d\n",(int)TaskAdd(TaskPB,0));
  printf("TaskAccel added, id = %d\n",(int)TaskAdd(TaskAccel,0));  
  printf("TaskLedOff added, id = %d\n",(int) TaskAdd(TaskLedOff,0));
  printf("TaskLedOn added, id = %d\n",(int)TaskAdd(TaskLedOn,0));
  printf("TaskGyro added, id = %d\n",(int)TaskAdd(TaskGyro,0));
 
} 
ADD_CMD("addtask",CmdTaskAdd,"              Adds a Task to Scheduler")


void CmdTaskKill(int mode)
{

  uint32_t id;
  int rc;

  if(mode != CMD_INTERACTIVE)
  {
    return;
  }

  rc = fetch_uint32_arg(&id);
  if(rc) 
  {
    printf("missing task id\n");
    return;
  }
  
  if(id >= 0 && id <10)
  {
    TaskKill(id);
  } else {
    puts("no action was taken");
  }
} 
ADD_CMD("killtask",CmdTaskKill,"<task id>     Kills a Task from Scheduler")

