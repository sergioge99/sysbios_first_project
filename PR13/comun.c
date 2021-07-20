/******************************************************************************/
/*                                                                            */
/* project  : PRACTICAS SE-II UNIZAR                                          */
/* filename : comun.c                                                         */
/* version  : 2                                                               */
/* date     : 28/09/2020                                                      */
/* author   : Jose Luis Villarroel                                            */
/* description : Tareas con recursos compartidos. PR13                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                        Used modules                                        */
/******************************************************************************/

#define TARGET_IS_TM4C123_RB1

#include <xdc/std.h>
#include <stdbool.h>
#include <stdint.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <xdc/runtime/Log.h>
#include <ti/uia/events/UIABenchmark.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/arm/a8/TimestampProvider.h>

#include "computos.h"
#include "servidores.h"


/******************************************************************************/
/*                        Defines                                             */
/******************************************************************************/


/******************************************************************************/
/*                        Global variables                                    */
/******************************************************************************/

Task_Handle tsk1;
Task_Handle tsk2;
Task_Handle tsk3;

Clock_Handle clocktask1 ;
Clock_Handle clocktask2 ;
Clock_Handle clocktask3 ;

Semaphore_Handle task1_sem ;
Semaphore_Handle task2_sem ;
Semaphore_Handle task3_sem ;

/******************************************************************************/
/*                        Function Prototypes                                 */
/******************************************************************************/

Void task1(UArg arg0, UArg arg1);
Void task2(UArg arg0, UArg arg1);
Void task3(UArg arg0, UArg arg1);

Void task1_release (void);
Void task2_release (void);
Void task3_release (void);


/******************************************************************************/
/*                        Main                                                */
/******************************************************************************/

Void main()
{
 	Clock_Params clockParams;
    Task_Params taskParams;

    /* Create three independent tasks */
    Task_Params_init(&taskParams);
    taskParams.priority = 5;
    tsk1 = Task_create (task1, &taskParams, NULL);

	Clock_Params_init(&clockParams);
	clockParams.period = 100 ;
	clockParams.startFlag = TRUE;
	clocktask1 = Clock_create((Clock_FuncPtr)task1_release, 10, &clockParams, NULL);

    task1_sem = Semaphore_create(0, NULL, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 3;
    tsk2 = Task_create (task2, &taskParams, NULL);

	Clock_Params_init(&clockParams);
	clockParams.period = 200 ;
	clockParams.startFlag = TRUE;
	clocktask2 = Clock_create((Clock_FuncPtr)task2_release, 10, &clockParams, NULL);

    task2_sem = Semaphore_create(0, NULL, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 1;
    tsk3 = Task_create (task3, &taskParams, NULL);

	Clock_Params_init(&clockParams);
	clockParams.period = 400 ;
	clockParams.startFlag = TRUE;
	clocktask3 = Clock_create((Clock_FuncPtr)task3_release, 5, &clockParams, NULL);

    task3_sem = Semaphore_create(0, NULL, NULL);

    Crear_Servidores () ;

    BIOS_start();
}

/******************************************************************************/
/*                        Tasks                                               */
/******************************************************************************/
/*
 *  ======== task1 ========
 */

void task1_release (void) {
	  Semaphore_post(task1_sem);
}

Void task1(UArg arg0, UArg arg1)
{
    for (;;) {

    	Semaphore_pend (task1_sem, BIOS_WAIT_FOREVER) ;

        CS (10) ;
        S11 () ;
    }
}

/*
 *  ======== task2 ========
 */

void task2_release (void) {
	  Semaphore_post(task2_sem);
}

Void task2(UArg arg0, UArg arg1)
{
    for (;;) {

    	Semaphore_pend (task2_sem, BIOS_WAIT_FOREVER) ;

        CS (60) ;
        S21 () ;
    }
}


/*
 *  ======== task3 ========
 */

void task3_release (void) {
	  Semaphore_post(task3_sem);
}

Void task3(UArg arg0, UArg arg1)
{
    for (;;) {

    	Semaphore_pend (task3_sem, BIOS_WAIT_FOREVER) ;

        S12 () ;
    	CS (30) ;
    	S22 () ;
    }
}


