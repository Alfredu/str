#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Servo.h>
#include "PIDball.h"
#include "hcsr04.h"

Servo myservo;  // create servo object to control a servo
HCSR04 distSensor;
PIDball controller;

//shared resources
double distance;
double oldPosition;
double newPosition;

SemaphoreHandle_t xDistanceSemaphore;
SemaphoreHandle_t xPositionSemaphore;
SemaphoreHandle_t xNewPositionSemaphore;

void TaskReadDistance( void *pvParameters );
void TaskReadServoPosition( void *pvParameters );
void TaskCalculate(void *pvParameters );
void TaskSetServoPosition(void *pvParameters);

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
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
    
  xTaskCreate(
    TaskReadServoPosition
    ,  (const portCHAR *)"readServoPosition"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskCalculate
    ,  (const portCHAR *)"readServoPosition"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskSetServoPosition
    ,  (const portCHAR *)"readServoPosition"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
}

void loop() {
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskReadDistance( void *pvParameters ) {
  (void) pvParameters;

  for(;;) {
    if ( xSemaphoreTake( xDistanceSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      distance = distSensor.getDistance();
      xSemaphoreGive( xDistanceSemaphore ); //Now we free xDistanceSemaphore so it can be used by other tasks
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void TaskReadServoPosition( void *pvParameters ){
  (void) pvParameters;
  for(;;) {
    if ( xSemaphoreTake( xPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      oldPosition = myservo.read();
      xSemaphoreGive( xPositionSemaphore ); //Now we free xPositionSemaphore so it can be used by other tasks
  
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void TaskCalculate(void *pvParameters ){
  (void) pvParameters;
  for (;;) {
    if ( xSemaphoreTake( xDistanceSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      if ( xSemaphoreTake( xPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
         if ( xSemaphoreTake( xNewPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
           newPosition = controller.compute(distance, oldPosition);
           xSemaphoreGive( xNewPositionSemaphore );
         }
         xSemaphoreGive( xPositionSemaphore );
      }
      xSemaphoreGive( xDistanceSemaphore ); //Now we free xDistanceSemaphore so it can be used by other tasks
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void TaskSetServoPosition(void *pvParameters){
  (void) pvParameters;
  for(;;) {
    if ( xSemaphoreTake( xNewPositionSemaphore, ( TickType_t ) 1 ) == pdTRUE ) {
      myservo.write(newPosition);
      xSemaphoreGive( xNewPositionSemaphore );
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

