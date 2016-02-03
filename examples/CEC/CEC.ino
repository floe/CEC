#include "CEC_Device.h"

#define IN_LINE 2
#define OUT_LINE 3

#define report(X) do { DbgPrint("report " #X "\n"); report ## X (); } while (0)

class MyCEC: public CEC_Device {
  public:
    MyCEC(int physAddr): CEC_Device(physAddr,IN_LINE,OUT_LINE) { }
    
    void reportPhysAddr()    { unsigned char frame[4] = { 0x84, 0x10, 0x00, 0x04 }; TransmitFrame(0x0F,frame,sizeof(frame)); } // FIXME: physical address hardcoded}
    void reportStreamState() { unsigned char frame[3] = { 0x82, 0x10, 0x00 };       TransmitFrame(0x0F,frame,sizeof(frame)); } // FIXME: physical address hardcoded}
    void reportPowerState()  { unsigned char frame[2] = { 0x90, 0x00 };             TransmitFrame(0x00,frame,sizeof(frame)); } // report power on
    void reportCECVersion()  { unsigned char frame[2] = { 0x9E, 0x04 };             TransmitFrame(0x00,frame,sizeof(frame)); } // report CEC v1.3a
    void reportOSDName()     { unsigned char frame[5] = { 0x47, 'H','T','P','C' };  TransmitFrame(0x00,frame,sizeof(frame)); } // FIXME: name hardcoded
    void reportVendorID()    { unsigned char frame[4] = { 0x87, 0x00, 0xF1, 0x0E }; TransmitFrame(0x00,frame,sizeof(frame)); } // report fake vendor ID
    
    void handleKey(unsigned char key) {
      switch (key) {
        case 0x00: Keyboard.press(KEY_RETURN); break;
        case 0x01: Keyboard.press(KEY_UP_ARROW); break;
        case 0x02: Keyboard.press(KEY_DOWN_ARROW); break;
        case 0x03: Keyboard.press(KEY_LEFT_ARROW); break;
        case 0x04: Keyboard.press(KEY_RIGHT_ARROW); break;
        case 0x0D: Keyboard.press(KEY_ESC); break;
        case 0x4C: Keyboard.press(KEY_PAGE_DOWN); break;
        case 0x4B: Keyboard.press(KEY_PAGE_UP); break;
        case 0x53: Keyboard.press(KEY_HOME); break;
      }
    }
        
    void OnReceive(int source, int dest, unsigned char* buffer, int count) {
      if (count == 0) return;
      switch (buffer[0]) {
        case 0x36: DbgPrint("standby\n"); break;
        case 0x83: report(PhysAddr); break;
        case 0x86: if (buffer[1] == 0x10 && buffer[2] == 0x00) report(StreamState); break; // report stream status
        case 0x8F: report(PowerState); break;
        case 0x9F: report(CECVersion); break;  
        case 0x46: report(OSDName);    break;
        case 0x8C: report(VendorID);   break;
        
        case 0x44: handleKey(buffer[1]); break;
        case 0x45: Keyboard.releaseAll(); break;
        
        default: CEC_Device::OnReceive(source,dest,buffer,count); break;
      }
    }
};

MyCEC device(0x1000);

void setup()
{
  Serial.begin(115200);
  Keyboard.begin();
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
        break;
    }
  }
  device.Run();
}

