/* 
* FILE : simpleTask.c
* PROJECT : INFO8110 -Assignment #1
* PROGRAMMER : A. Jamil Aryan
* FIRST VERSION : 2016-02-01
* DESCRIPTION :
* This file implements a simple task counter which represents an 
* an intercommunication process in ARM processor's library software
* by counting a passed value as a parameter to "count" command on 
* minicom client.
*/

#include <stdio.h>
#include <string.h>
#include "stm32f3_discovery.h"
#include "common.h"


static uint32_t counter;

// This function decrements the global counter variable
void TaskCounter(void)
{
   if (counter)
   {
      printf("Now counter value is: %d \n",(int)counter);
      counter--;
   }
}

// This function handles the command interface between minicom and stm32 library
void CmdCount(int mode)
{
   uint32_t val = 0;
   int filter;
   filter = fetch_uint32_arg(&val);
   if(filter)
   {
     printf("pass in number of counts\n");
     return;
   }
   
   printf("Entered value for counter is : %d \n",(int)val);
   counter = val;
   
}
ADD_CMD("count", CmdCount,"<number> 	Implements a simple task intercommunication");

