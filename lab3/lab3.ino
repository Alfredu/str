#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include "altIMU.h"

// UDP library
byte myMAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
IPAddress myIP(192, 168, 1, 4);
IPAddress destIP(192, 168, 1, 1);
unsigned int localPort = 8888;
 // local port to listen on
char sendBuffer[] = "HOLI";
 // a string to send back
EthernetUDP Udp; // An EthernetUDP instance to let us send packets over UDP
altIMU accelerometer;

void setup() {
  Ethernet.begin(myMAC,myIP); // start the Ethernet and UDP:
  Udp.begin(localPort);
  Serial.begin(9600);
  accelerometer.initiate();
}

void loop() {
  float b = accelerometer.getPitch();
  float angle = accelerometer.getAngleAroundYAxis();
  //dtostrf(b, 4,2, sendBuffer);
  Serial.println(sendBuffer);
  Udp.beginPacket(destIP, 8889);
  Udp.write(sendBuffer);
  Udp.endPacket();
  delay(300);
}

