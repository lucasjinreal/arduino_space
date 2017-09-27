#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

void setup() {
  Serial.begin(9600);

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  // this is client, so here set client1
  Mirf.setRADDR((byte *)"client1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();

  Serial.println("Beginning ... ");
}

void loop() {

  // send data to server1
  Mirf.setTADDR((byte *)"server1");

  // this is data to be sent, send 1314 to server forever!!!
  unsigned int data = 1714;
  Mirf.send((byte *)&data);

  while (Mirf.isSending()) {
  }
  Serial.println("Finished sending");
  delay(1000);
}



