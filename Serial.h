#ifndef SERIAL_H__
#define SERIAL_H__

#include "Common.h"

#define SERIAL_BUFFER_SIZE 16

class SerialLine
{
public:
	SerialLine();

	void ClearTransmitBuffer();
	virtual bool Transmit(unsigned char* buffer, int count);
	virtual bool TransmitPartial(unsigned char* buffer, int count);
	void RegisterReceiveHandler();

protected:
	unsigned char _transmitBuffer[SERIAL_BUFFER_SIZE];
	unsigned char _receiveBuffer[SERIAL_BUFFER_SIZE];
	int _transmitBufferCount;
	int _transmitBufferBit;
	int _transmitBufferByte;
	int _receiveBufferCount;
	int _receiveBufferBit;
	int _receiveBufferByte;

	virtual void OnTransmitBegin() {;}
	virtual void OnReceiveComplete(unsigned char* buffer, int count);

	int PopTransmitBit();
	int RemainingTransmitBytes();
	int TransmitSize();
	void ResetTransmitBuffer();

	void PushReceiveBit(int);
	int ReceivedBytes();
	void ResetReceiveBuffer();
};

#endif // SERIAL_H__
