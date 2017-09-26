#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

unsigned int adata = 0;
void setup() {
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  // this is server, so here set server1
  Mirf.setRADDR((byte *)"server1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();

  Serial.println("Listening from client...");
}

void loop() {
  // a buffer to catch data
  byte data[Mirf.payload];

  if (!Mirf.isSending() && Mirf.dataReady()) {
    Serial.println("Got packet from client");
    Mirf.getData(data);
    adata = (unsigned int)((data[1] << 8) | data[0]);
  }
}
