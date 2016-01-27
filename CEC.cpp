#include "CEC.h"

int CEC_LogicalDevice::_validLogicalAddresses[6][5] = 
	{	{CLA_TV,					CLA_FREE_USE,			CLA_UNREGISTERED,		CLA_UNREGISTERED,	CLA_UNREGISTERED,	},
		{CLA_RECORDING_DEVICE_1,	CLA_RECORDING_DEVICE_2,	CLA_RECORDING_DEVICE_3,	CLA_UNREGISTERED,	CLA_UNREGISTERED,	},
		{CLA_PLAYBACK_DEVICE_1,		CLA_PLAYBACK_DEVICE_2,	CLA_PLAYBACK_DEVICE_3,	CLA_UNREGISTERED,	CLA_UNREGISTERED,	},
		{CLA_TUNER_1,				CLA_TUNER_2,			CLA_TUNER_3,			CLA_TUNER_4,		CLA_UNREGISTERED,	},
		{CLA_AUDIO_SYSTEM,			CLA_UNREGISTERED,		CLA_UNREGISTERED,		CLA_UNREGISTERED,	CLA_UNREGISTERED,	},
		{CLA_UNREGISTERED,			CLA_UNREGISTERED,		CLA_UNREGISTERED,		CLA_UNREGISTERED,	CLA_UNREGISTERED,	},
		};

#define MAKE_ADDRESS(s,d) ((((s) & 0xf) << 4) | ((d) & 0xf))


CEC_LogicalDevice::CEC_LogicalDevice(int physicalAddress)
: CEC_Electrical(CLA_UNREGISTERED)
, _physicalAddress(physicalAddress)
, _logicalAddress(CLA_UNREGISTERED)
, _done(false)
, _waitTime(0)
, _primaryState(CEC_ALLOCATE_LOGICAL_ADDRESS)
, _deviceType(CDT_OTHER)
{
	_secondaryState = CEC_XMIT_POLLING_MESSAGE;
	_tertiaryState = 0;
}

void CEC_LogicalDevice::Initialize(CEC_DEVICE_TYPE type)
{
	CEC_Electrical::Initialize();
	_deviceType = type;

        if (MonitorMode)
        {
            _primaryState = CEC_READY;
        }
}

bool CEC_LogicalDevice::ProcessStateMachine(bool* success)
{
	unsigned char buffer[1];
	bool wait = false;

	switch (_primaryState)
	{
	case CEC_ALLOCATE_LOGICAL_ADDRESS:
		switch (_secondaryState)
		{
		case CEC_XMIT_POLLING_MESSAGE:
			// Section 6.1.3 specifies that <Polling Message> while allocating a Logical Address
			// will have the same initiator and destination address
			buffer[0] = MAKE_ADDRESS(_validLogicalAddresses[_deviceType][_tertiaryState], _validLogicalAddresses[_deviceType][_tertiaryState]);
			ClearTransmitBuffer();
			Transmit(buffer, 1);
			
			_secondaryState = CEC_RCV_POLLING_MESSAGE;
			wait = true;
			break;

		case CEC_RCV_POLLING_MESSAGE:
			if (success)
			{
				if (*success)
				{
					// Someone is there, try the next
					_tertiaryState++;
					if (_validLogicalAddresses[_deviceType][_tertiaryState] != CLA_UNREGISTERED)
						_secondaryState = CEC_XMIT_POLLING_MESSAGE;
					else
					{
						_logicalAddress = CLA_UNREGISTERED;
						DbgPrint("Logical address assigned: %d\n", _logicalAddress);
						_primaryState = CEC_READY;
					}
				}
				else
				{
					// We hereby claim this as our logical address!
					_logicalAddress = _validLogicalAddresses[_deviceType][_tertiaryState];
					SetAddress(_logicalAddress);
					DbgPrint("Logical address assigned: %d\n", _logicalAddress);
					_primaryState = CEC_READY;
				}
			}
			else
				wait = true;
			break;
		}
		break;

	case CEC_READY:
		_primaryState = CEC_IDLE;
		OnReady();
		wait = true;
		break;

	case CEC_IDLE:
		wait = true;
		break;
	}

	return wait;
}

void CEC_LogicalDevice::OnReceiveComplete(unsigned char* buffer, int count)
{
	ASSERT(count >= 1);
	int sourceAddress = (buffer[0] >> 4) & 0x0f;
	int targetAddress = buffer[0] & 0x0f;
	OnReceive(sourceAddress, targetAddress, buffer + 1, count - 1);
}

bool CEC_LogicalDevice::TransmitFrame(int targetAddress, unsigned char* buffer, int count)
{
	if (_primaryState != CEC_IDLE)
		return false;

	unsigned char addr[1];

	addr[0] = MAKE_ADDRESS(_logicalAddress, targetAddress);
        ClearTransmitBuffer();
	if (!TransmitPartial(addr, 1))
		return false;
	return Transmit(buffer, count);
}

void CEC_LogicalDevice::OnTransmitComplete(bool success)
{
	if (_primaryState == CEC_ALLOCATE_LOGICAL_ADDRESS &&
		_secondaryState == CEC_RCV_POLLING_MESSAGE &&
		_logicalAddress == CLA_UNREGISTERED)
	{
		while (!ProcessStateMachine(&success))
			;
	}
        else
          DbgPrint("Transmit: %d\n", success);
}

void CEC_LogicalDevice::Run()
{
	// Initial pump for the state machine (this will cause a transmit to occur)
	while (!ProcessStateMachine(NULL))
		;

	if (((_waitTime == (unsigned long)-1 && !TransmitPending()) || (_waitTime != (unsigned long)-1 && _waitTime > micros())) && !IsISRTriggered())
		return;

        unsigned long wait = Process();
        if (wait != (unsigned long)-2)
	        _waitTime = wait;
	return;
}


