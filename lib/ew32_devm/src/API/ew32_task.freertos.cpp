#include "ew32_devm/ew32_config.h"

#if EW32_TASKAPI_PLATFORM == EW32_PLATFORM_TAKSAPI_RTOS
#include "ew32_devm/ew32_api.h"
#include "ew32_base/ew32_logging.h"

#if EW32_PLATFORM == EW32_PLATFORM_RPPICO
#include <pico/time.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#endif

#if EW32_PLATFORM == EW32_PLATFORM_ESP
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#endif

#if EW32_PLATFORM == EW32_PLATFORM_RPPICO 
extern "C" {

    void idle_task (void*) {
        for(;;) {
            vPortYield();
        }
    }
}
#endif

void ew32_tasksystem_init() {
    #if EW32_PLATFORM == EW32_PLATFORM_RPPICO 
        TaskHandle_t _task;
        xTaskCreate(idle_task, "idle_task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &_task);


        #if configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1
            vTaskCoreAffinitySet(_task, EW32_FREERTOS_ON_CORE);
        #else
            (void)_task;
        #endif

        vTaskStartScheduler ();
    #endif
}

uint8_t ew32_task_create(ew32_task_handle task, uint16_t stackSize, uint8_t prio, const char* pcName, 
                        void* userdata, uint8_t core, ew32_task_t** tk) {
    
    BaseType_t _ret = 1;
    #if EW32_PLATFORM == EW32_PLATFORM_RPPICO
        _ret = xTaskCreate(task, pcName, stackSize, userdata, prio, (TaskHandle_t*)tk);
        #if configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1
            vTaskCoreAffinitySet((TaskHandle_t)*tk, core);
        #endif
    #else
        _ret = xTaskCreatePinnedToCore(task, pcName, stackSize, userdata, 
            prio, (TaskHandle_t*)tk, core);
    #endif

    return _ret;
}

void ew32_task_delay(uint16_t delay) {

    #if EW32_PLATFORM == EW32_PLATFORM_RPPICO
        sleep_ms(delay);
    #else
        vTaskDelay(delay / portTICK_PERIOD_MS);
    #endif
}
void ew32_task_suspend(ew32_task_t task) {
    vTaskSuspend( (TaskHandle_t)task );
}
void ew32_task_resume(ew32_task_t task) {
    vTaskResume( (TaskHandle_t)task );
}

struct ew32_semaphore_type {
    QueueHandle_t lock;
    uint8_t locked;
};
uint8_t ew32_semaphore_destroy_ex(ew32_semaphore_t handle, uint8_t unlockByLock) {
    if(handle == NULL) return 1;
    if(handle->locked == 1) {
        if(unlockByLock) ew32_semaphore_unlock(handle);
        else return 2;
    }

    vSemaphoreDelete( handle->lock );

    return 0;
}

uint8_t ew32_semaphore_create(uint32_t type, ew32_semaphore_t* lock) {
    if(lock == NULL) return 1;

    if(type == EW32_SEMAPHORE_MUTEX) (*lock)->lock = xSemaphoreCreateMutex();
    if(type == EW32_SEMAPHORE_RECV_MUTEX) (*lock)->lock = xSemaphoreCreateRecursiveMutex();
    (*lock)->lock = xSemaphoreCreateBinary();

    return 0;
}
uint8_t ew32_semaphore_lock(ew32_semaphore_t lock, int32_t timeout) {
    if(lock == NULL) return 1;

    if( xSemaphoreTake( lock->lock,  timeout == EW32_INFINITY ? portMAX_DELAY : timeout) == pdTRUE ) {
        lock->locked = 1;
        return 0;
    }
    return 2;
}
void ew32_semaphore_unlock(ew32_semaphore_t lock) {
    if(lock == NULL) return ;

    if(xSemaphoreGive( lock->lock)  == pdTRUE ) {
        lock->locked = 0;
    }
}


#endif