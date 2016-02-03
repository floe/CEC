#include "CEC_Device.h"
#include <Arduino.h>

CEC_Device::CEC_Device(int physicalAddress, int in_line, int out_line)
: CEC_LogicalDevice(physicalAddress)
, _isrTriggered(false)
, _lastLineState2(true)
, _in_line(in_line)
, _out_line(out_line)
{
}

void CEC_Device::Initialize(CEC_DEVICE_TYPE type)
{
  pinMode(_out_line, OUTPUT);
  pinMode( _in_line,  INPUT);

  digitalWrite(_out_line, LOW);
  delay(200);

  CEC_LogicalDevice::Initialize(type);
}

void CEC_Device::OnReady()
{
  // This is called after the logical address has been
  // allocated
  DbgPrint("Device ready\n");
}

void CEC_Device::OnReceive(int source, int dest, unsigned char* buffer, int count)
{
  // This is called when a frame is received.  To transmit
  // a frame call TransmitFrame.  To receive all frames, even
  // those not addressed to this device, set Promiscuous to true.
  DbgPrint("Packet received at %ld: %02d -> %02d: %02X", millis(), source, dest, ((source&0x0f)<<4)|(dest&0x0f));
  for (int i = 0; i < count; i++)
    DbgPrint(":%02X", buffer[i]);
  DbgPrint("\n");
}

bool CEC_Device::LineState()
{
  int state = digitalRead(_in_line);
  return state == LOW;
}

void CEC_Device::SetLineState(bool state)
{
  digitalWrite(_out_line, state?LOW:HIGH);
  // give enough time for the line to settle before sampling
  // it
  delayMicroseconds(50);
  _lastLineState2 = LineState();
}

void CEC_Device::SignalIRQ()
{
  // This is called when the line has changed state
  _isrTriggered = true;
}

bool CEC_Device::IsISRTriggered()
{
  if (_isrTriggered)
  {
    _isrTriggered = false;
    return true;
  }
  return false;
}

void CEC_Device::Run()
{
  bool state = LineState();
  if (_lastLineState2 != state)
  {
    _lastLineState2 = state;
    SignalIRQ();
  }
  CEC_LogicalDevice::Run();
}
