#define PERIOD 25
#define traceTASK_SWITCHED_IN() logTaskSwitch((int)pxCurrentTCB->pxTaskTag)
#define configUSE_APPLICATION_TASK_TAG 1
