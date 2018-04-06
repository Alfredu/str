#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include "altIMU.h"
#include "hcsr04.h"

// UDP library
byte myMAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
IPAddress myIP(192, 168, 1, 4);
IPAddress destIP(192, 168, 1, 2);
unsigned int localPort = 8889;
 // local port to listen on
char sendBuffer[] = "DUPA";
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
 // a string to send back
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
  float angle = accelerometer.getAngleAroundYAxis();
  float dist = distSensor.getDistance();
  dtostrf(angle, 4,2, sendBuffer);
  Udp.beginPacket(destIP, 8888);
  Udp.write(sendBuffer);
  Udp.endPacket();
  delay(100);

  int packetSize = Udp.parsePacket(); // if there’s data available, read a packet
  if(packetSize)
  {

    IPAddress remote = Udp.remoteIP();
    if (remote[3]=2) {
      Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE); // read the packet into packet
      Serial.println(packetBuffer);
    }
  }
  delay(100);
}

