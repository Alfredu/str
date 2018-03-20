#ifndef ALTIMU_H
#define ALTIMU_H

#include <LSM6.h>
#include <Wire.h>

/*
 * This class measures the inclination of the y axis in altIMU sensor
 *
*/

class altIMU { 
public:

	altIMU(int x);
  altIMU();
	~altIMU();
  int readValue();

private:
  LSM6 imu;
  


};

#endif
