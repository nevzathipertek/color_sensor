//program::IR PWM Source driver cpp file
// @uthor::Nevzat DURMAZ
//   date::24.11.2018
// update::15.05.2024
//version::01.04
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
extern "C"{
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_tim.h"
}
#include "PWMOUT.h"
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PWMOUT::init( unsigned int _dutyCycle, unsigned int _desiredFreq,
		unsigned int _period, unsigned int _maxPeriodPercentage ){

	__maxDuty = ( (_period * _maxPeriodPercentage) / 100) + 1;
	__period = _period;

	if(_desiredFreq) setFrequency(_desiredFreq );
	setPeriod(__period-1);
	setDuty(_dutyCycle);

	switch (__channel){
	case 1:	if(HAL_TIM_PWM_Start(__pTimUsed, TIM_CHANNEL_1)!= HAL_OK){}	break;
	case 2:	if(HAL_TIM_PWM_Start(__pTimUsed, TIM_CHANNEL_2)!= HAL_OK){}	break;
	case 3:	if(HAL_TIM_PWM_Start(__pTimUsed, TIM_CHANNEL_3)!= HAL_OK){}	break;
	case 4:	if(HAL_TIM_PWM_Start(__pTimUsed, TIM_CHANNEL_4)!= HAL_OK){}	break;
	default: break;
	}

}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
// the period is always set to 99 for duty.
void PWMOUT::setPeriod( unsigned int period ){
	__pTimUsed->Instance->ARR = period;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
// we change frequency by configuring the prescaler...
// the period is always set to 99 for duty.
// TIMERCLOCKSOURCE / 100 = frequency;
void PWMOUT::setFrequency( unsigned int _desiredFreq ){

	unsigned int tempPrescaler, tempFreq, tempClockSource{ ( __clockSource*10 ) / __period };

	if ( (_desiredFreq > 20) && (_desiredFreq < 100'001) ) {

		tempPrescaler = tempClockSource / _desiredFreq;

		if(tempPrescaler > 9) {

			if ( (tempPrescaler  % 10) > 5) { tempPrescaler /= 10; 						}
			else 							{ tempPrescaler /= 10;  tempPrescaler--; 	}
		}
		else tempPrescaler = 0;

		tempFreq = ( tempClockSource / (tempPrescaler+1) );

		if ( (tempFreq  % 10) > 5) 	{ tempFreq /= 10; tempFreq++; 	}
		else 						{ tempFreq /= 10; 				}
	}
	else return;

	if(__prescaler != tempPrescaler){
		__prescaler = tempPrescaler;
		__pTimUsed->Instance->PSC = __prescaler;
		__freq = tempFreq;
	}


}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PWMOUT::setDuty( unsigned int dutyCycle ){

	if(dutyCycle < __maxDuty){ // PWMMAXDUTYVALUE){

		if(__duty != dutyCycle){ __duty = dutyCycle;

			switch (__channel){
			case 1:	__pTimUsed->Instance->CCR1 = __duty;	break;
			case 2:	__pTimUsed->Instance->CCR2 = __duty;	break;
			case 3:	__pTimUsed->Instance->CCR3 = __duty;	break;
			case 4:	__pTimUsed->Instance->CCR4 = __duty;	break;
			default: break;
			}
		}
	}
	else return;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
