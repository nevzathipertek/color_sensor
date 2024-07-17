//program::CAN bus protocol with SN65HVD230 driver header file
// @uthor::Nevzat DURMAZ
//   date::02.01.2021
// update::18.09.2023
//version::01.03
//--------------------------------------------------------------------//
#ifndef CANBUS_H_
#define CANBUS_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#include "CO_app_STM32.h"
#include "storage/CO_storage.h"
#include "OD.h" //Object Dictionary for Can Open Work Protocol
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//#define ODT_ARRAY   ODT_ARR
//#define ODT_RECORD  ODT_REC
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class CANBUS {

	unsigned short CounterOfCommunication{0};

	inline void incCounterOfCom(void) { CounterOfCommunication++; }

public:

	CAN_HandleTypeDef* pCanUsed;
	TIM_HandleTypeDef* pTimUsed;
	void (*pCanInit)(void);

	inline CANBUS() = delete;

	inline explicit CANBUS( CAN_HandleTypeDef* pUSEDhcan,
			TIM_HandleTypeDef* pUSEDhtim, void (*_CanInitFunc_)(void) ) :
			pCanUsed{pUSEDhcan}, pTimUsed{pUSEDhtim}, pCanInit{_CanInitFunc_} {}

	inline virtual ~CANBUS() {}

	// node ID must be selected, baud is useless here,
	// it takes the configuration of STM32 as baud...
	void init(uint8_t _nodeID_, uint8_t _baudKHz_);
	void sendRequest(void);

	uint8_t getNodeIDactive(void)const;
	uint8_t getNodeIDdesired(void)const;
	bool getRedLed(void)const;
	bool getGreenLed(void)const;


	static inline void DelayUs(unsigned int _delTime){ for(unsigned int i=0; i<_delTime; i++); }

};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* CANBUS_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
