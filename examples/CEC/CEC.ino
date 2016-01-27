#include "CEC_Device.h"

#define IN_LINE 2
#define OUT_LINE 3

CEC_Device device(0x1000);

bool XX_GetLineState()
{
  int state = digitalRead(IN_LINE);
  return state == LOW;
}

void XX_SetLineState(CEC_Device* device, bool state)
{
  digitalWrite(OUT_LINE, state?LOW:HIGH);
  // give enough time for the line to settle before sampling
  // it
  delayMicroseconds(50);
  device->_lastLineState2 = XX_GetLineState();
}

void setup()
{
  pinMode(OUT_LINE, OUTPUT);
  pinMode(IN_LINE, INPUT);

  digitalWrite(OUT_LINE, LOW);
  delay(200);

  Serial.begin(115200);
  //device.MonitorMode = true;
  device.Promiscuous = true;
  device.Initialize(CEC_LogicalDevice::CDT_PLAYBACK_DEVICE);
}

void loop()
{
  if (Serial.available())
  {
    unsigned char c = Serial.read();
    unsigned char buffer[3];
    
    switch (c)
    {
      case '1':
        buffer[0] = 0x36;
        device.TransmitFrame(4, buffer, 1);
        break;
    }
  }
  device.Run();
}


