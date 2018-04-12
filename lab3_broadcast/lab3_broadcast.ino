#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include "altIMU.h"
#include "hcsr04.h"
#include <stdio.h>

/* 
 *  In this program we send and receive a date (pair of values: distance + angle)
 *  through broadcast using EthernetUDP
 */
 
byte myMAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
IPAddress myIP(192, 168, 1, 4);
IPAddress destIP(192, 168, 1, 255); //broadcast address
unsigned int localPort = 8889; // local port to listen on
char sendBuffer[10];                 // a string to send back
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
                                            
EthernetUDP Udp; // An EthernetUDP instance to let us send packets over UDP
altIMU accelerometer;
HCSR04 distSensor;

void setup() {
  Ethernet.begin(myMAC,myIP); // start the Ethernet and UDP:
  Udp.begin(localPort);
  Serial.begin(9600);
  accelerometer.initiate();
  distSensor.initiate();
}

void loop() {
  //we read current values from sensors
  float angle = accelerometer.getAngleAroundYAxis();
  float dist = distSensor.getDistance();
  char angle_ch[4];
  char dist_ch[4];
  char coma[] = ", ";
  //we transform those values to char[] and copy it to sendBuffer
  dtostrf(angle, 4,2, angle_ch);
  dtostrf(dist, 4, 2, dist_ch);
  strcat(sendBuffer,angle_ch);
  strcat(sendBuffer,coma);
  strcat(sendBuffer,dist_ch);
  //we send a pair of values to our neighbour
  Udp.beginPacket(destIP, 8888);
  Udp.write(sendBuffer);
  Udp.endPacket();
  delay(100);

  int packetSize = Udp.parsePacket(); // if there’s data available, read a packet
  if(packetSize)
  {
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE); // read the packet into packetBuffer
    Serial.println(packetBuffer); //we plot the values we have obtained
  }
  delay(100);
}

