#ifndef CEC_DEVICE_H__
#define CEC_DEVICE_H__

#include "CEC.h"

class CEC_Device : public CEC_LogicalDevice
{
public:
  CEC_Device(int physicalAddress, int in_line, int out_line);
  
  void Initialize(CEC_DEVICE_TYPE type);
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
  bool _isrTriggered;
  bool _lastLineState2;
  int  _in_line, _out_line;
};

#endif // CEC_DEVICE_H__
