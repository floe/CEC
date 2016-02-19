#include "CEC_Device.h"

#define IN_LINE 2
#define OUT_LINE 3
#define HPD_LINE 10

// ugly macro to do debug printing in the OnReceive method
#define report(X) do { DbgPrint("report " #X "\n"); report ## X (); } while (0)

#define phy1 ((_physicalAddress >> 8) & 0xFF)
#define phy2 ((_physicalAddress >> 0) & 0xFF)

class MyCEC: public CEC_Device {
  public:
    MyCEC(int physAddr): CEC_Device(physAddr,IN_LINE,OUT_LINE) { }
    
    void reportPhysAddr()    { unsigned char frame[4] = { 0x84, phy1, phy2, 0x04 }; TransmitFrame(0x0F,frame,sizeof(frame)); } // report physical address
    void reportStreamState() { unsigned char frame[3] = { 0x82, phy1, phy2 };       TransmitFrame(0x0F,frame,sizeof(frame)); } // report stream state (playing)
    
    void reportPowerState()  { unsigned char frame[2] = { 0x90, 0x00 };             TransmitFrame(0x00,frame,sizeof(frame)); } // report power state (on)
    void reportCECVersion()  { unsigned char frame[2] = { 0x9E, 0x04 };             TransmitFrame(0x00,frame,sizeof(frame)); } // report CEC version (v1.3a)
    
    void reportOSDName()     { unsigned char frame[5] = { 0x47, 'H','T','P','C' };  TransmitFrame(0x00,frame,sizeof(frame)); } // FIXME: name hardcoded
    void reportVendorID()    { unsigned char frame[4] = { 0x87, 0x00, 0xF1, 0x0E }; TransmitFrame(0x00,frame,sizeof(frame)); } // report fake vendor ID
    // TODO: implement menu status query (0x8D) and report (0x8E,0x00)
    
    void handleKey(unsigned char key) {
      switch (key) {
        case 0x00: Keyboard.press(KEY_RETURN); break;
        case 0x01: Keyboard.press(KEY_UP_ARROW); break;
        case 0x02: Keyboard.press(KEY_DOWN_ARROW); break;
        case 0x03: Keyboard.press(KEY_LEFT_ARROW); break;
        case 0x04: Keyboard.press(KEY_RIGHT_ARROW); break;
        case 0x0D: Keyboard.press(KEY_ESC); break;
        case 0x4B: Keyboard.press(KEY_PAGE_DOWN); break;
        case 0x4C: Keyboard.press(KEY_PAGE_UP); break;
        case 0x53: Keyboard.press(KEY_HOME); break;
      }
    }
        
    void OnReceive(int source, int dest, unsigned char* buffer, int count) {
      if (count == 0) return;
      switch (buffer[0]) {
        
        case 0x36: DbgPrint("standby\n"); break;
        
        case 0x83: report(PhysAddr); break;
        case 0x86: if (buffer[1] == phy1 && buffer[2] == phy2)
                   report(StreamState); break;
        
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

// TODO: set physical address via serial (or even DDC?)

// Note: this does not need to correspond to the physical address (i.e. port number)
// where the Arduino is connected - in fact, it _should_ be a different port, namely
// the one where the PC to be controlled is connected. Basically, it is the address
// of the port where the CEC-less source device is plugged in.
MyCEC device(0x1000);

void setup()
{
  // setup Hotplug pin
  pinMode(HPD_LINE,INPUT);
  
  Serial.begin(115200);
  Keyboard.begin();
  
  //device.MonitorMode = true;
  //device.Promiscuous = true;
  device.Initialize(CEC_LogicalDevice::CDT_PLAYBACK_DEVICE);
}

void loop()
{
  // FIXME: does it still work without serial connected?
  // try to toggle LED e.g.
  if (Serial.available())
  {
    unsigned char c = Serial.read();
    unsigned char buffer[2] = { c, 0 };
    Serial.print("Command: "); Serial.println((const char*)buffer);
    
    switch (c)
    {
      case 'v':
        // request vendor ID
        buffer[0] = 0x8C;
        device.TransmitFrame(0, buffer, 1);
        break;
      case 'p':
        // toggle promiscuous mode
        device.Promiscuous = !(device.Promiscuous);
        break;
      case 'h':
        // query Hotplug pin
        Serial.print("Hotplug state: "); Serial.println(digitalRead(HPD_LINE));
        break;
    }
  }
  device.Run();
}

