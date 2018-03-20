#include "altIMU.h" //include the declaration for this class

altIMU::altIMU(int x) {
   Wire.begin();
  
  imu.init();
  imu.enableDefault();
}
altIMU::altIMU() {
}
altIMU::~altIMU()  {};

int altIMU::readValue() {
  imu.read();
  char report[80];
  snprintf(report, sizeof(report), "A: %6d %6d %6d    G: %6d %6d %6d",
    imu.a.x, imu.a.y, imu.a.z,
    imu.g.x, imu.g.y, imu.g.z);

  Serial.println(report);
  return imu.a.x;
}

