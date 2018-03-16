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
  return imu.a.x;
}

