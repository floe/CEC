#ifndef CECWIRE_H__
#define CECWIRE_H__

#include "Serial.h"

#define CEC_MAX_RETRANSMIT 5

class CEC_Electrical : public SerialLine
{
public:
	CEC_Electrical(int address);
	void Initialize();
	void SetAddress(int address);

	unsigned long Process();
	bool TransmitPending() { return _primaryState == CEC_TRANSMIT && _secondaryState == CEC_IDLE_WAIT; }

	int Promiscuous;
	int MonitorMode;

protected:
	virtual bool LineState() = 0;
	virtual void SetLineState(bool) = 0;

private:
	typedef enum {
		CEC_IDLE,
		CEC_TRANSMIT,
		CEC_RECEIVE,
	} CEC_PRIMARY_STATE;

	typedef enum {
		CEC_RCV_STARTBIT1,
		CEC_RCV_STARTBIT2,
		CEC_RCV_DATABIT1,
		CEC_RCV_DATABIT2,
		CEC_RCV_ACK_SENT,
		CEC_RCV_ACK1,
		CEC_RCV_ACK2,
		CEC_RCV_LINEERROR,

		CEC_IDLE_WAIT,
		CEC_XMIT_STARTBIT1,
		CEC_XMIT_STARTBIT2,
		CEC_XMIT_DATABIT1,
		CEC_XMIT_DATABIT2,
		CEC_XMIT_ACK,
		CEC_XMIT_ACK2,
		CEC_XMIT_ACK3,
		CEC_XMIT_ACK_TEST,
	} CEC_SECONDARY_STATE;

	typedef enum {
		CEC_RCV_BIT0,
		CEC_RCV_BIT1,
		CEC_RCV_BIT2,
		CEC_RCV_BIT3,
		CEC_RCV_BIT4,
		CEC_RCV_BIT5,
		CEC_RCV_BIT6,
		CEC_RCV_BIT7,
		CEC_RCV_BIT_EOM,
		CEC_RCV_BIT_ACK,

		CEC_ACK,
		CEC_NAK,

		CEC_XMIT_BIT0,
		CEC_XMIT_BIT1,
		CEC_XMIT_BIT2,
		CEC_XMIT_BIT3,
		CEC_XMIT_BIT4,
		CEC_XMIT_BIT5,
		CEC_XMIT_BIT6,
		CEC_XMIT_BIT7,
		CEC_XMIT_BIT_EOM,
		CEC_XMIT_BIT_ACK,


		CEC_IDLE_RETRANSMIT_FRAME,
		CEC_IDLE_NEW_FRAME,
		CEC_IDLE_SUBSEQUENT_FRAME,
	} CEC_TERTIARY_STATE;

private:
	bool ResetState();
	void ResetTransmit(bool retransmit);
	virtual void OnTransmitBegin();
	virtual void OnTransmitComplete(bool);

	void ProcessFrame();

	// Helper functions
	bool Raise();
	bool Lower();
	void HasRaised(unsigned long);
	void HasLowered(unsigned long);
	bool CheckAddress();
	void ReceivedBit(bool);
	unsigned long LineError();

	int _address;

	bool _lastLineState;
	unsigned long _lastStateChangeTime;
	unsigned long _bitStartTime;

	int _xmitretry;

	bool _eom;
	bool _follower;
	bool _broadcast;
	bool _amLastTransmittor;
	bool _transmitPending;

	CEC_PRIMARY_STATE _primaryState;
	CEC_SECONDARY_STATE _secondaryState;
	CEC_TERTIARY_STATE _tertiaryState;
};



#endif // CECWIRE_H__
