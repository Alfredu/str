#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

// UDP library
byte myMAC[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x48, 0x4};
IPAddress myIP(192, 168, 1, 4);
unsigned int localPort = 8889;
 // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
EthernetUDP Udp; // EthernetUDP instance to let us receive packets over UDP
void setup() {
Ethernet.begin(myMAC,myIP);
 // start the Ethernet and UDP:
Udp.begin(localPort);
Serial.begin(9600);
}
void loop() {
int packetSize = Udp.parsePacket(); // if there’s data available, read a packet
  if(packetSize)
  {
    Serial.print("Received packet of size ");Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
    Serial.print(remote[i], DEC);
    if (i < 3) {Serial.print(".");}
    }
    Serial.print(", port "); Serial.println(Udp.remotePort());
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE); // read the packet into packet
    Serial.println("Contents:"); Serial.println(packetBuffer);
  }

  else{
    Serial.println("NO PACKET");
    
  }
  delay(100);
}

