#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

void setup() {
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"receiver1"); // 标记本地名，用于接收
  Mirf.payload = 5;
  Mirf.channel = 3;
  Mirf.config();
}

void loop() {
  byte data[Mirf.payload];
  if(!Mirf.isSending() && Mirf.dataReady()) { 
    Mirf.getData(data);
    int i;
    String Temp;
    for (i = 0; i < Mirf.payload; i++) //把收到的信息拼起来，到一个串里面
    {
      Temp += char(data[i]);
    }
    Serial.print("Get:");
    Serial.print(Mirf.payload);
    Serial.print(" ");
    Serial.println(Temp);
  }
