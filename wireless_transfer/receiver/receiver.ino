#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

unsigned int adata = 0, oldadata = 0;

void setup()
{
  Serial.begin(9600);
  Mirf.cePin = 9;     //设置CE引脚为D9
  Mirf.csnPin = 10;   //设置CE引脚为D10
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();  //初始化nRF24L01

  Mirf.setRADDR((byte *)"rec1");
  Mirf.payload = sizeof(unsigned int);
  Mirf.channel = 3;
  Mirf.config();
  Serial.println("I'm Receiver...");
}


void loop()
{
  byte data[Mirf.payload];
  if (Mirf.dataReady())   //等待接收数据准备好
  {
    Mirf.getData(data);    //接收数据到data数组
    adata = (unsigned int)((data[1] << 8) | data[0]);

    //与上一次结果比较，避免相同结果刷屏,降低串口流量
    if (adata != oldadata)
    {
      oldadata = adata; //本次结果作为历史结果。
      //Serial.print输出数据
      Serial.print("receive from rec1 =");
      Serial.println(adata);
    }

  }
}
