
#include "FreeRTOSConfig.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Servo.h>
#include "PIDball.h"
#include "hcsr04.h"

Servo myservo;  // create servo object to control a servo
HCSR04 distSensor;
PIDball controller;

int taskTrace[512] = {-1};
int timingTrace[512] = {0};
int priorityTrace[512];

int index = 0;
int nextCPriority = 2;
boolean increasing = true;
//shared resources
double distance;
double oldPosition;
double newPosition;

SemaphoreHandle_t xDistanceSemaphore;
SemaphoreHandle_t xPositionSemaphore;
SemaphoreHandle_t xNewPositionSemaphore;
TaskHandle_t xTaskAHandle,xTaskBHandle,xTaskCHandle;

void TaskReadDistance( void *pvParameters );
void TaskReadServoPosition( void *pvParameters );
void TaskCalculate(void *pvParameters );
void TaskSetServoPosition(void *pvParameters);
void TaskA(void *pvParameters);
void TaskB(void *pvParameters);
void TaskC(void *pvParameters);
void TaskChangePriority(void *pvParameters);
extern void vTaskSetApplicationTaskTag(
                        TaskHandle_t xTask,
                        TaskHookFunction_t pxTagValue );


void setup() {
  distSensor.initiate();
  myservo.attach(9, 800,2350);  // attaches the servo on pin 9 to the servo object
  myservo.write(73);
  delay(1000);
  Serial.begin(9600);
  controller.setGoal(17.0);
  controller.setTunings(6,0,0.001);

  if ( xDistanceSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xDistanceSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xDistanceSemaphore ) != NULL )
      xSemaphoreGive( ( xDistanceSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  if ( xPositionSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xPositionSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xPositionSemaphore ) != NULL )
      xSemaphoreGive( ( xPositionSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  if ( xNewPositionSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xNewPositionSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xNewPositionSemaphore ) != NULL )
      xSemaphoreGive( ( xNewPositionSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
  
  xTaskCreate(
    TaskReadDistance
    ,  (const portCHAR *)"readDistance"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  6  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
    
  xTaskCreate(
    TaskReadServoPosition
    ,  (const portCHAR *)"readServoPosition"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  6  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskCalculate
    ,  (const portCHAR *)"calculate"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  5  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskSetServoPosition
    ,  (const portCHAR *)"setServoPosition"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  4  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    xTaskCreate(
      TaskPrintTrace
      , (const portCHAR *)"printTrace"
      , 128
      , NULL
      , 7
      , NULL);

    xTaskCreate(
      TaskA
      , (const portCHAR *)"A"
      , 128
      , NULL
      , 3
      , &xTaskAHandle);

    xTaskCreate(
      TaskB
      , (const portCHAR *)"B"
      , 128
      , NULL
      , 2
      , &xTaskBHandle);

    xTaskCreate(
      TaskC
      , (const portCHAR *)"C"
      , 128
      , NULL
      , 1
      , &xTaskCHandle);

    xTaskCreate(
      TaskChangePriority
      , (const portCHAR *)"ChangePriority"
      , 128
      , NULL
      , 8
      , NULL);
}

void loop() {
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskReadDistance( void *pvParameters ) {
  //We set a task tag to identify the task in the tracer.
  vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t) 1);
 
  for(;;) {
    if ( xSemaphoreTake( xDistanceSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      
    logTaskSwitch(1, 6);
      distance = distSensor.getDistance();
      xSemaphoreGive( xDistanceSemaphore ); //Now we free xDistanceSemaphore so it can be used by other tasks
    }
    vTaskDelay(PERIOD / portTICK_PERIOD_MS);
  }
}

void TaskReadServoPosition( void *pvParameters ){
  vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t) 2);
  for(;;) {
    if ( xSemaphoreTake( xPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      
    logTaskSwitch(2, 6);
      oldPosition = myservo.read();
      xSemaphoreGive( xPositionSemaphore ); //Now we free xPositionSemaphore so it can be used by other tasks
  
    }
    vTaskDelay(PERIOD / portTICK_PERIOD_MS);
  }
}

void TaskCalculate(void *pvParameters ){
  vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t) 3);
  for (;;) {
    if ( xSemaphoreTake( xDistanceSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      if ( xSemaphoreTake( xPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
         if ( xSemaphoreTake( xNewPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
          
    logTaskSwitch(3, 5);
           newPosition = controller.compute(distance, oldPosition);
           xSemaphoreGive( xNewPositionSemaphore );
         }
         xSemaphoreGive( xPositionSemaphore );
      }
      xSemaphoreGive( xDistanceSemaphore ); //Now we free xDistanceSemaphore so it can be used by other tasks
    }
    vTaskDelay(PERIOD / portTICK_PERIOD_MS);
  }
}

void TaskSetServoPosition(void *pvParameters){
  vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t) 4);
  for(;;) {
    if ( xSemaphoreTake( xNewPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      
    logTaskSwitch(4, 4);
      myservo.write(newPosition);
      xSemaphoreGive( xNewPositionSemaphore );
    }
    vTaskDelay(PERIOD / portTICK_PERIOD_MS);
  }
}

void TaskPrintTrace(void *pvParameters){
  (void) pvParameters;

  for(;;){
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    for(int i = 0; i<index-1; i++){
      char buff[32];
      sprintf(buff, "TAG: %d, %d, PRIORITY: %d", taskTrace[i], timingTrace[i] - timingTrace[0], priorityTrace[i]);
      Serial.println(buff);
    }
    while(1);
  }
}

void TaskA(void *pvParameters){
  (void) pvParameters;

  for(;;) {
    logTaskSwitch(5, uxTaskPriorityGet(NULL));
    double t0 = millis();
    while(millis()-t0 < 25) ;
    vTaskDelay(PERIOD / portTICK_PERIOD_MS);
  }
}

void TaskB(void *pvParameters){
  (void) pvParameters;

  for(;;) {
    logTaskSwitch(6, uxTaskPriorityGet(NULL));
    double t0 = millis();
    while(millis()-t0 < 25) ;
    vTaskDelay(PERIOD / portTICK_PERIOD_MS);
  }
}

void TaskC(void *pvParameters){
  (void) pvParameters;

  for(;;) {
    logTaskSwitch(7, uxTaskPriorityGet(NULL));
    double t0 = millis();
    while(millis()-t0 < 25);
    vTaskDelay(PERIOD / portTICK_PERIOD_MS);
  }
}

void TaskChangePriority(void *pvParameters){
  (void) pvParameters;

  for(;;) {
    vTaskDelay(200 / portTICK_PERIOD_MS);
    vTaskPrioritySet(xTaskCHandle, nextCPriority);

    if(nextCPriority == 2 && increasing){
      vTaskPrioritySet(xTaskBHandle, 1);
    }
    else if(nextCPriority == 1 && !increasing){
      vTaskPrioritySet(xTaskBHandle, 2);
    }
    else if(nextCPriority == 3 && increasing){
      vTaskPrioritySet(xTaskAHandle, 2);
    }
    else if(nextCPriority == 2 && !increasing ){
      vTaskPrioritySet(xTaskAHandle, 3);
    }

    if(nextCPriority == 3){
      increasing = false;
    }
    else if(nextCPriority == 1){
      increasing = true;
    }
    
    if(increasing){
      nextCPriority++;
    }
    else{
      nextCPriority--;
    }
  }
}

void logTaskSwitch(int tag, int priority){
  taskTrace[index] = tag;
  timingTrace[index] = millis();
  priorityTrace[index] = priority;
  index++;
}

