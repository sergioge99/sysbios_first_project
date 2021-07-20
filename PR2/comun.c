
/******************************************************************************/
/*                                                                            */
/* project  : PRACTICAS SE-II UNIZAR                                          */
/* filename : comun.c                                                         */
/* version  : 2                                                               */
/* date     : 28/09/2020                                                      */
/* author   : Jose Luis Villarroel                                            */
/* description : Comun con servidor esporadico. PR2                           */
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/*                        Defines                                             */
/******************************************************************************/

#define TARGET_IS_TM4C123_RB1

/******************************************************************************/
/*                        Used modules                                        */
/******************************************************************************/

#include <xdc/std.h>
#include <stdbool.h>
#include <stdint.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <xdc/runtime/Log.h>
#include <ti/uia/events/UIABenchmark.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Swi.h>

#include "computos.h"
#include "servidores.h"
#include "Event.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/******************************************************************************/
/*                        Function prototypes                                 */
/******************************************************************************/

void task1_release (void) ;
void task2_release (void) ;
void task3_release (void) ;

Void task1(UArg arg0, UArg arg1);
Void task2(UArg arg0, UArg arg1);
Void task3(UArg arg0, UArg arg1);

Void Sporadic_Server(UArg arg0, UArg arg1);
Void swiSS(UArg arg0, UArg arg1);

Void Ev(UArg arg0, UArg arg1);

/******************************************************************************/
/*                        Global variables                                    */
/******************************************************************************/

Task_Params taskParams;
Clock_Params clockParams;
Semaphore_Params semaphoreParams ;

Hwi_Params hwiParams;
Swi_Params swiParams;

Mailbox_Params MbxParams ;

Clock_Handle tsk1Clock;
Clock_Handle tsk2Clock;
Clock_Handle tsk3Clock;

Semaphore_Handle task1_sem ;
Semaphore_Handle task2_sem ;
Semaphore_Handle task3_sem ;

Task_Handle tsk1;
Task_Handle tsk2;
Task_Handle tsk3;
Task_Handle tskSS;

Hwi_Handle myHwi ;
Swi_Handle mySwi ;

Mailbox_Handle Event_Queue ;

Void myIsr(UArg arg) ;


/******************************************************************************/
/*                        main                                                */
/******************************************************************************/

Void main()
{

    /* GPIO Int */

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3) ;

    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_INT_PIN_3);
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    IntPrioritySet(INT_GPIOD, 0);
    IntEnable(INT_GPIOD);

    /* Hwi */

    Hwi_Params_init(&hwiParams);
    hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    myHwi = Hwi_create(19, myIsr, &hwiParams, NULL);

    /* Mail Box */

    Mailbox_Params_init(&MbxParams) ;
    Event_Queue = Mailbox_create (4, 100, &MbxParams, NULL) ;

    /* Swi */

    Swi_Params_init(&swiParams);
    swiParams.priority = 1 ;
    mySwi = Swi_create(swiSS, &swiParams, NULL);

    /* Create three independent tasks */

    Semaphore_Params_init(&semaphoreParams);

    Clock_Params_init(&clockParams);
    clockParams.period = 100 ;
    clockParams.startFlag = TRUE;
    tsk1Clock = Clock_create((Clock_FuncPtr)task1_release, 100, &clockParams, NULL);

    task1_sem = Semaphore_create(0, &semaphoreParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 5;
    tsk1 = Task_create (task1, &taskParams, NULL);

    Clock_Params_init(&clockParams);
    clockParams.period = 200 ;
    clockParams.startFlag = TRUE;
    tsk2Clock = Clock_create((Clock_FuncPtr)task2_release, 200, &clockParams, NULL);

    task2_sem = Semaphore_create(0, &semaphoreParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 3;
    tsk2 = Task_create (task2, &taskParams, NULL);

    Clock_Params_init(&clockParams);
    clockParams.period = 400 ;
    clockParams.startFlag = TRUE;
    tsk3Clock = Clock_create((Clock_FuncPtr)task3_release, 400, &clockParams, NULL);

    task3_sem = Semaphore_create(0, &semaphoreParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 1;
    tsk3 = Task_create (task3, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 4;
    tskSS = Task_create (Sporadic_Server, &taskParams, NULL);

    Crear_Servidores () ;
    Init_Events (50) ;

    Hwi_enable();
    BIOS_start();
}

/******************************************************************************/
/*                        Tasks                                               */
/******************************************************************************/
/*
 *  ======== task1 ========
 */

void task1_release (void)
{
   Semaphore_post(task1_sem);
}

Void task1(UArg arg0, UArg arg1)
{
    for (;;) {
        Semaphore_pend (task1_sem, BIOS_WAIT_FOREVER);
        CS (10) ;
        S11 () ;
    }
}

/*
 *  ======== task2 ========
 */

void task2_release (void)
{
   Semaphore_post(task2_sem);
}

Void task2(UArg arg0, UArg arg1)
{
    for (;;) {
        Semaphore_pend (task2_sem, BIOS_WAIT_FOREVER);
        CS (50) ;
        S21 () ;
    }
}


/*
 *  ======== task3 ========
 */

void task3_release (void)
{
   Semaphore_post(task3_sem);
}

Void task3(UArg arg0, UArg arg1)
{
    for (;;) {
        Semaphore_pend (task3_sem, BIOS_WAIT_FOREVER);
        S22 () ;
    	CS (30) ;
    	S12 () ;
    }
}

/******************************************************************************/
/*                        Aperiodic event management                          */
/******************************************************************************/


Void myIsr(UArg arg)
{
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_INT_PIN_3);

    Swi_post (mySwi) ;
}


 Void swiSS(UArg arg0, UArg arg1) {

//	UInt32 Time_Stamp ;

	  Log_write1(UIABenchmark_start, (xdc_IArg)"Ext_Event");
      CS(15) ;
	  Log_write1(UIABenchmark_stop, (xdc_IArg)"Ext_Event");
//    Time_Stamp = Clock_getTicks();
//    Mailbox_post(Event_Queue, &Time_Stamp, BIOS_NO_WAIT);
}


void Sporadic_Server (UArg arg0, UArg arg1) {

	UInt32 Replenishment_Period = 120 ;
	UInt32 Next_Start, Time_Stamp, Activation_Time ;
	long d ;

	Next_Start = Clock_getTicks();

	while (TRUE){
	  Mailbox_pend (Event_Queue, &Time_Stamp, BIOS_WAIT_FOREVER);

	  Log_write1(UIABenchmark_start, (xdc_IArg)"Ext_Event");
      CS(15) ;
	  Log_write1(UIABenchmark_stop, (xdc_IArg)"Ext_Event");

	  if (Time_Stamp > Next_Start) Activation_Time = Time_Stamp ;
	  else Activation_Time = Next_Start ;
	  Next_Start = Activation_Time + Replenishment_Period ;
	  d = (long)Next_Start - (long)Clock_getTicks() ;
	  if (d < 0) d = 0 ;
	  Task_sleep((UInt32)d);
	  }

}


