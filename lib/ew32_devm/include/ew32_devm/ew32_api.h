#pragma once

#include <stdio.h>
#include "ew32_config.h"


#define EW32_TASK_PRIORITY_IDLE       EW32_TASK_IDLE_PRIORITY           /*!< Priority for no real time operations - idle task */
#define EW32_TASK_PRIORITY_LOWEST     ( EW32_TASK_IDLE_PRIORITY + 1U )  /*!< Priority for lowest operation  */
#define EW32_TASK_PRIORITY_LOW        ( EW32_TASK_IDLE_PRIORITY + 2U )  /*!< Priority for low operation  */
#define EW32_TASK_PRIORITY_NORMAL     ( EW32_TASK_IDLE_PRIORITY + 3U )  /*!< Priority for tasks for normal operatins - user interfaces for example */
#define EW32_TASK_PRIORITY_HALFCRT    ( EW32_TASK_IDLE_PRIORITY + 4U )  /*!< Priority for tasks with normal deadlines and not a lot of processings*/
#define EW32_TASK_PRIORITY_URGENT     ( EW32_TASK_IDLE_PRIORITY + 5U )  /*!< priority for tasks with short deadlines and a lot of processings */
#define EW32_TASK_PRIORITY_CRITICAL   ( EW32_TASK_IDLE_PRIORITY + 6U )  /*!< Priority for critical tasks - the highest priority  */


#ifdef __cplusplus
extern "C" {
#endif

typedef void (* ew32_task_handle)( void * );
typedef void* ew32_task_t;

#if EW32_TASKAPI_PLATFORM == EW32_PLATFORM_TAKSAPI_ASYNC
extern void ew32_main_task(void *data);
#endif
void ew32_tasksystem_init();

uint8_t ew32_task_create(ew32_task_handle task, uint16_t stackSize, uint8_t prio, const char* pcName, 
                         void* userdata, uint8_t core, ew32_task_t** tk);

void ew32_task_delay(uint16_t delay);
void ew32_task_suspend(ew32_task_t task);
void ew32_task_resume(ew32_task_t task);


typedef struct ew32_semaphore_type *ew32_semaphore_t;

#define EW32_SEMAPHORE_MUTEX            1
#define EW32_SEMAPHORE_BIN_MUTEX        2
#define EW32_SEMAPHORE_RECV_MUTEX       3

#define EW32_INFINITY                   -1

uint8_t ew32_semaphore_create(uint32_t type, ew32_semaphore_t* lock);
uint8_t ew32_semaphore_lock(ew32_semaphore_t handle, int32_t timeout);
void ew32_semaphore_unlock(ew32_semaphore_t handle);
uint8_t ew32_semaphore_destroy_ex(ew32_semaphore_t handle, uint8_t unlockByheld);

#define ew32_semaphore_trylock( HANDLE ) ew32_semaphore_lock ( (HANDLE), 1)
#define ew32_semaphore_destroy( HANDLE ) ew32_semaphore_destroy_ex ( (HANDLE), 0)

#ifdef __cplusplus
}
#endif