#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
void setup()
{
  Serial.begin(9600);

  Mirf.cePin = 9;                //设置CE引脚为D9
  Mirf.csnPin = 10;        //设置CE引脚为D10
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"sen1");
  Mirf.payload = sizeof(unsigned int);
  Mirf.channel = 3;
  Mirf.config();
  Serial.println("I'm Sender...");
}

unsigned int adata = 0;
void loop()
{
  //读取A0值到adata
  adata = 1232;
  byte data[Mirf.payload];

  data[0] = adata & 0xFF;                //低八位给data[0]，
  data[1] = adata >> 8;                //高八位给data[1]。
  Mirf.setTADDR((byte *)"rec1");
  Mirf.send(data);
  //while死循环等待发送完毕，才能进行下一步操作。
  while (Mirf.isSending()) {}
  Serial.println(adata);

  delay(1000);
}
