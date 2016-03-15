/* 
* FILE : task_executive.c
* PROJECT : INFO8110 -Assignment #2
* PROGRAMMER : A. Jamil Aryan
* FIRST VERSION : 2016-29-02
* DESCRIPTION :
* This is the headerfile listing the common functions
*/

void TaskInit();
int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);
int32_t TaskKill(int32_t id);
