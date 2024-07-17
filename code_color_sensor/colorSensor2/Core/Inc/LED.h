//program::LED driver cpp/header file
// @uthor::Nevzat DURMAZ
//   date::04.10.2016
// update::05.09.2023
//version::03.08
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef LED_H_
#define LED_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class LED {

	bool led_pos{false};
	int led_count{0};

	volatile int __times{1}, __duration{10}, __wait{0},
				 __times2{0}, __duration2{0}, __wait2{0},
				 ledTotalCount{999};

	void (*fpTurn)(bool);

	inline void turnLed(bool _state){
		if(led_pos != _state){ led_pos = _state; fpTurn(led_pos); }
	}

public:

	inline LED() = delete;
	inline virtual ~LED() {}

	explicit inline LED( void (*_ledTurn_)(bool), int waitForOsDelay = 1, int nTimes = 2, int nDurationLedOnStatus = 20, int nWaitBetweenTimes = 100 ) :
			led_pos{false}, led_count{0}, __times{nTimes}, __duration{nDurationLedOnStatus < waitForOsDelay ? 1 : nDurationLedOnStatus / waitForOsDelay},
			__wait{nWaitBetweenTimes < waitForOsDelay ? 1 : waitForOsDelay / waitForOsDelay},
			__times2{0}, __duration2{0}, __wait2{0}, ledTotalCount{( ((nTimes * (nDurationLedOnStatus + nWaitBetweenTimes)) / waitForOsDelay) < 999) ? 999 :
					((nTimes * (nDurationLedOnStatus + nWaitBetweenTimes)) / waitForOsDelay) },
			fpTurn{_ledTurn_} {}

	inline void setLedValues(int waitForOsDelay = 1, int nTimes = 2, int nDurationLedOnStatus = 20, int nWaitBetweenTimes = 100){
		int temp = ((nTimes * (nDurationLedOnStatus + nWaitBetweenTimes)) / waitForOsDelay);
		__times = nTimes;
		__duration = nDurationLedOnStatus < waitForOsDelay ? 1 : nDurationLedOnStatus / waitForOsDelay;
		__wait = nWaitBetweenTimes < waitForOsDelay ? 1 : waitForOsDelay / waitForOsDelay;
		(temp > 999) ? (ledTotalCount = temp) : (ledTotalCount = 999);
	}

	inline void operator=(bool _state)	{ turnLed(_state); 	}
	inline void operator++(int)			{ turnLed(true); 	}
	inline void operator--(int)			{ turnLed(false); 	}
	inline bool getLedState(void)const	{ return led_pos;	}

	// use tick with a RTOS function or near HAL_IncTick...
	inline void tick(void){
		if(++led_count > ledTotalCount) {
			led_count = 0;
			__times2 = __times;
			__duration2 = __duration;
			__wait2 = 0;
		}
		if(__wait2)	{ __wait2--; }
		else if(__times2) {
			turnLed(true);
			if(--__duration2 == 0){
				__times2--;
				__duration2 = __duration;
				__wait2 = __wait;
				turnLed(false);
			}
		}
	}

//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* LED_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
