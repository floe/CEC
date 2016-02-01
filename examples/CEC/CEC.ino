#include "CEC_Device.h"

#define IN_LINE 2
#define OUT_LINE 3

class MyCEC: public CEC_Device {
  public:
    MyCEC(int physAddr): CEC_Device(physAddr,IN_LINE,OUT_LINE) { }
    void OnReceive(int source, int dest, unsigned char* buffer, int count) {
      unsigned char frame86[3] = { 0x82, 0x10, 0x00 };
      unsigned char frame83[4] = { 0x84, 0x10, 0x00, 0x04 }; // FIXME: physical address hardcoded
      unsigned char frame8F[2] = { 0x90, 0x00 };
      unsigned char frame9F[2] = { 0x9E, 0x04 };
      unsigned char frame46[5] = { 0x47, 'H','T','P','C' }; // FIXME: name hardcoded
      CEC_Device::OnReceive(source,dest,buffer,count);
      if (count == 0) return;
      switch (buffer[0]) {
        case 0x36: DbgPrint("standby\n"); break;
        case 0x83: DbgPrint("report phys addr\n"); TransmitFrame(0x0F,frame83,sizeof(frame83)); break; // report physical address
        case 0x86: DbgPrint("report stream status\n"); if (buffer[1] == 0x10 && buffer[2] == 0x00) TransmitFrame(0x0F,frame86, sizeof(frame86)); break; // report stream status
        case 0x8F: DbgPrint("report power status\n"); TransmitFrame(0x00,frame8F,sizeof(frame8F)); break; // report power status  
        case 0x9F: DbgPrint("report CEC version\n"); TransmitFrame(0x00,frame9F,sizeof(frame9F)); break; // report CEC version 
        case 0x46: DbgPrint("report OSD name\n"); TransmitFrame(0x00,frame46,sizeof(frame46)); break; // report OSD name
        default:   break;
      }
    }
};

MyCEC device(0x1000);

void setup()
{
  Serial.begin(115200);
  device.Init();
  //device.MonitorMode = true;
  //device.Promiscuous = true;
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
        // request vendor ID
        buffer[0] = 0x8C;
        device.TransmitFrame(0, buffer, 1);
        break;
      case '2':
        // ping
        device.TransmitFrame(0, buffer, 0);
      case '3':
        buffer[0] = 0x8E;
        buffer[1] = 0x01;
        device.TransmitFrame(0,buffer,2);
    }
  }
  device.Run();
}

