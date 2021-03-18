/* Copyright Instigate Robotics CJSC 2019 */

#ifndef ASSERT_RTOS_H
#define ASSERT_RTOS_H

#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/queue.h>

#ifdef  USE_FULL_ASSERT
void rtos_assert_failed(const char * const file, uint32_t line )
{
        (void)file;
        (void)line;
        static portBASE_TYPE xPrinted = pdFALSE;
        (void)xPrinted;
        volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
        taskENTER_CRITICAL();
        {
                /* You can step out of this function to debug the assertion by using
                 *         the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
                 *                 value. */
                while( ulSetToNonZeroInDebuggerToContinue == 0 )
                {
                }
        }
        taskEXIT_CRITICAL();
}

void vApplicationMallocFailedHook( void )
{
        /* The malloc failed hook is enabled by setting
        configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

        Called if a call to pvPortMalloc() fails because there is insufficient
        free memory available in the FreeRTOS heap.  pvPortMalloc() is called
        internally by FreeRTOS API functions that create tasks, queues, software
        timers, and semaphores.  The size of the FreeRTOS heap is set by the
        configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
        for( ;; );
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
        ( void ) pcTaskName;
        ( void ) xTask;
        /* Run time stack overflow checking is performed if
           configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
           function is called if a stack overflow is detected.  pxCurrentTCB can be
           inspected in the debugger if the task name passed into this function is
           corrupt. */
        for( ;; );
}	

#endif

#endif // ASSERT_RTOS_H
