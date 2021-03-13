#include <os_cpu.h>
#include <os_cfg.h>
#include <app_cfg.h>
#include <ucos_ii.h>

#include "nucleoboard.h"
#include "hw_init.h"
#include "print.h"

#define BUFSIZE 256

typedef INT8S HANDLE;

// Prototype for startup task
void StartupTask(void* pdata);
void Mp3DemoTask(void* pdata);
void LcdTouchDemoTask(void* pdata);

// Allocate a stack for the startup task
static OS_STK StartupStk[APP_CFG_TASK_START_STK_SIZE];
static OS_STK LcdTouchDemoTaskStk[APP_CFG_TASK_START_STK_SIZE];
static OS_STK Mp3DemoTaskStk[APP_CFG_TASK_START_STK_SIZE];

// Allocate the print buffer
PRINT_DEFINEBUFFER();

int main()
{
	INT8U err;
	char buf[BUFSIZE];
	
    Hw_init();
	
	//RETAILMSG(1, ("MP3 Player Demo: Built %s %s.\r\n\r\n",
	//	__DATE__,
	//	__TIME__));  
	PrintWithBuf(buf, BUFSIZE, "MP3 Player Demo: Built %s %s.\r\n\r\n", __DATE__, __TIME__);
		
    // Initialize the OS
    //DEBUGMSG(1, ("main: Running OSInit()...\n"));
	PrintWithBuf(buf, BUFSIZE, "main: Running OSInit()...\n");
    OSInit();
    
    // Initialize driver framework after initializing uCOS since the framework uses uCOS services
    //DEBUGMSG(1, ("Initializing PJDF driver framework...\n"));
	PrintWithBuf(buf, BUFSIZE, "Initializing PJDF driver framework...\n");

    // Create the startup task
    //DEBUGMSG(1, ("main: Creating start up task.\n"));
	PrintWithBuf(buf, BUFSIZE, "main: Creating start up task.\n");

    err = OSTaskCreate(
        StartupTask,
        (void*)0,
        &StartupStk[APP_CFG_TASK_START_STK_SIZE-1],
        APP_TASK_START_PRIO);

    if (err != OS_ERR_NONE) {
        //DEBUGMSG(1, ("main: failed creating start up task: %d\n", err));
		PrintWithBuf(buf, BUFSIZE, "main: failed creating start up task: %d\n", err);
        while(OS_TRUE);  //park on error
    }

    //DEBUGMSG(1, ("Starting multi-tasking.\n"));
	PrintWithBuf(buf, BUFSIZE, "Starting multi-tasking.\n");

    // start the OS
    OSStart();

    while (1);
}



/************************************************************************************

   This task is the initial task running, started by main(). It starts
   the system tick timer and creates all the other tasks. Then it deletes itself.

************************************************************************************/
void StartupTask(void* pdata)
{
	char buf[BUFSIZE];

    //INT32U length;
    //static HANDLE hSD = 0;
    //static HANDLE hSPI = 0;

	PrintWithBuf(buf, BUFSIZE, "StartupTask: Begin\n");
	PrintWithBuf(buf, BUFSIZE, "StartupTask: Starting timer tick\n");

    // Start the system tick
    OS_CPU_SysTickInit(OS_TICKS_PER_SEC);    // Create the test tasks
    PrintWithBuf(buf, BUFSIZE, "StartupTask: Creating the application tasks\n");

    // The maximum number of tasks the application can have is defined by OS_MAX_TASKS in os_cfg.h
    OSTaskCreate(Mp3DemoTask, (void*)0, &Mp3DemoTaskStk[APP_CFG_TASK_START_STK_SIZE-1], APP_TASK_TEST1_PRIO);
    OSTaskCreate(LcdTouchDemoTask, (void*)0, &LcdTouchDemoTaskStk[APP_CFG_TASK_START_STK_SIZE-1], APP_TASK_TEST2_PRIO);

    // Delete ourselves, letting the work be done in the new tasks.
    PrintWithBuf(buf, BUFSIZE, "StartupTask: deleting self\n");
	OSTaskDel(OS_PRIO_SELF);
}


void Mp3DemoTask(void* pdata)
{
    //INT32U length;

    OSTimeDly(2000); // Allow other task to initialize LCD before we use it.
    
	char buf[BUFSIZE];
	PrintWithBuf(buf, BUFSIZE, "Mp3DemoTask: starting\n");
	PrintWithBuf(buf, BUFSIZE, "Starting MP3 device test\n");
    
	int count = 0;
	
    while (1)
    {
        OSTimeDly(500);
        PrintWithBuf(buf, BUFSIZE, "Begin streaming sound file  count=%d\n", ++count);
        PrintWithBuf(buf, BUFSIZE, "Done streaming sound file  count=%d\n", count);
    }
}


void LcdTouchDemoTask(void* pdata)
{
    //INT32U length;

	char buf[BUFSIZE];
	PrintWithBuf(buf, BUFSIZE, "LcdTouchDemoTask: starting\n");
	PrintWithBuf(buf, BUFSIZE, "Initializing LCD controller\n");

    while (1) { 
		OSTimeDly(500);
		PrintWithBuf(buf, BUFSIZE, "Initializing LCD controller\n");
    }
}











