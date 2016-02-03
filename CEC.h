#ifndef CEC_H__
#define CEC_H__

#include "CEC_Electrical.h"

class CEC_LogicalDevice : public CEC_Electrical
{
public:
	typedef enum {
		CDT_TV,
		CDT_RECORDING_DEVICE,
		CDT_PLAYBACK_DEVICE,
		CDT_TUNER,
		CDT_AUDIO_SYSTEM,
		CDT_OTHER,				// Not a real CEC type..
	} CEC_DEVICE_TYPE;

public:
	CEC_LogicalDevice(int physicalAddress);
	void Initialize(CEC_DEVICE_TYPE type);

	virtual void Run();
	virtual bool TransmitFrame(int targetAddress, unsigned char* buffer, int count);

protected:
	virtual bool IsISRTriggered() = 0;

	bool ProcessStateMachine(bool* success);

	virtual void OnReceiveComplete(unsigned char* buffer, int count);
	virtual void OnTransmitComplete(bool);

	virtual void OnReady() {;}
	virtual void OnReceive(int sourceAddress, int targetAddress, unsigned char* buffer, int count) = 0;

private:
	typedef enum {
		CLA_TV = 0,
		CLA_RECORDING_DEVICE_1,
		CLA_RECORDING_DEVICE_2,
		CLA_TUNER_1,
		CLA_PLAYBACK_DEVICE_1,
		CLA_AUDIO_SYSTEM,
		CLA_TUNER_2,
		CLA_TUNER_3,
		CLA_PLAYBACK_DEVICE_2,
		CLA_RECORDING_DEVICE_3,
		CLA_TUNER_4,
		CLA_PLAYBACK_DEVICE_3,
		CLA_RESERVED_1,
		CLA_RESERVED_2,
		CLA_FREE_USE,
		CLA_UNREGISTERED,
	} CEC_LOGICAL_ADDRESS;

	typedef enum {
		CEC_IDLE,
		CEC_READY,
		CEC_ALLOCATE_LOGICAL_ADDRESS,
	} CEC_PRIMARY_STATE;

	typedef enum {
		CEC_XMIT_POLLING_MESSAGE,
		CEC_RCV_POLLING_MESSAGE,
	} CEC_SECONDARY_STATE;

	typedef enum {
	} CEC_TERTIARY_STATE;

protected:
	static int _validLogicalAddresses[6][5];
	int _logicalAddress;
	int _physicalAddress;
	unsigned long _waitTime;
	bool _done;

	CEC_DEVICE_TYPE _deviceType;
	CEC_PRIMARY_STATE _primaryState;
	CEC_SECONDARY_STATE _secondaryState;
	int _tertiaryState;

};

#endif // CEC_H__
