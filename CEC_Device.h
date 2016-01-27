#ifndef CEC_DEVICE_H__
#define CEC_DEVICE_H__

#include "CEC.h"

class CEC_Device : public CEC_LogicalDevice
{
public:
  CEC_Device(int physicalAddress);
  virtual ~CEC_Device();
  
  virtual void Run();
  
protected:
  virtual bool LineState();
  virtual void SetLineState(bool);
  virtual void SignalIRQ();
  virtual bool IsISRTriggered();
  virtual bool IsISRTriggered2() { return _isrTriggered; }

  virtual void OnReady();
  virtual void OnReceive(int source, int dest, unsigned char* buffer, int count);
  
private:
friend void XX_SetLineState(CEC_Device* device, bool state);
friend bool XX_GetLineState();
  bool _isrTriggered;
  bool _lastLineState2;
};

#endif // CEC_DEVICE_H__
