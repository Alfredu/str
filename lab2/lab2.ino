#include "altIMU.h"
#include "hcsr04.h"

/* This programm uses both classes we have made for HCSR
 *  
 */

altIMU acc;
HCSR04 diss;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  acc = altIMU(1);
  diss._setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  float resacc = acc.getAngleAroundYAxis();
  float val = diss.createTrigger();
  Serial.println(val);
  Serial.println(resacc);
  
}
