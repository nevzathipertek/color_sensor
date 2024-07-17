//program::IR PWM Source driver header file
// @uthor::Nevzat DURMAZ
//   date::24.11.2018
// update::15.05.2024
//version::01.04
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef SRC_PWMOUT_H_
#define SRC_PWMOUT_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class PWMOUT {

	TIM_HandleTypeDef* __pTimUsed{nullptr};
	uint32_t __channel{0}, __prescaler{0xFFFF}, __clockSource{0};
	volatile uint32_t __duty{0}, __freq{0}, __period{0}, __maxDuty{0};

public:
	inline PWMOUT() = delete;

	inline PWMOUT(TIM_HandleTypeDef* _pTim_, uint32_t _CHANNEL_, uint32_t _TIMERCLOCKSOURCE_ ) :
		__pTimUsed{ _pTim_ }, __channel{ _CHANNEL_}, __clockSource{ _TIMERCLOCKSOURCE_ },
		__duty{0}, __freq{0}, __maxDuty{0} { }

	inline virtual ~PWMOUT() {}

	inline PWMOUT& setParams(TIM_HandleTypeDef* _pTim_, uint32_t _CHANNEL_, uint32_t _TIMERCLOCKSOURCE_) {
		__pTimUsed = _pTim_; __channel = _CHANNEL_; __clockSource = _TIMERCLOCKSOURCE_;
		return *this;
	}
	void init( unsigned int _dutyCycle = 0, unsigned int _desiredFreq = 68'000,
			unsigned int _period = 100, unsigned int _maxPeriodPercentage = 90 );
//--------------------------------------------------------------------//
	void setPeriod( unsigned int period = 99 );
	inline unsigned int getFrequency(void)const { return __freq; }
	inline unsigned int getDuty(void)const 		{ return __duty; }
//--------------------------------------------------------------------//
	void setFrequency( unsigned int _desiredFreq = 68'000 );
	void setDuty(unsigned int dutyCycle = 0);
//--------------------------------------------------------------------//
};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* SRC_PWMOUT_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
