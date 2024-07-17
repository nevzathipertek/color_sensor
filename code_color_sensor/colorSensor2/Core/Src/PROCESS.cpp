//program::process of color sensor source file
// @uthor::Nevzat DURMAZ
//   date::13.09.2023
// update::14.09.2023
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#include "main.h"
#include "PROCESS.h"
// #include "301/CO_ODinterface.h"
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
const int longOsDelay = 1'000;  // 20sn
const char hyphen[] = "-------";
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
// call every 20ms
void PROCESS::processing(void){

	valuesProcessing();
	getValues();
	setValues();
	if ( HAL_IWDG_Refresh(&hiwdg) != HAL_OK ){}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::valuesProcessing(void){

	col.updateData();

	if ( col.rawClear < 8192 )	{
		if ( ++ldDuty > PWM_MAX_DUTYVALUE ) ldDuty = PWM_MAX_DUTYVALUE;
	}
//	else if ( (col.rawClear == 8192) && (getMax(col.rawRed, col.rawGreen, col.rawBlue) > 8110) )  	{ if ( --ld_duty < 40 ) ld_duty = 40; 	}
	else if ( getMax(col.rawRed, col.rawGreen, col.rawBlue) > 7372 )  	{
		if ( --ldDuty < PWM_MIN_DUTYVALUE ) ldDuty = PWM_MIN_DUTYVALUE;
	}
	else if ( getMax(col.rawRed, col.rawGreen, col.rawBlue) < 7208 ) 	{
		if ( ++ldDuty > PWM_MAX_DUTYVALUE ) ldDuty = PWM_MAX_DUTYVALUE;
	}

	ld.setDuty(ldDuty);

}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::getValues(void){

	int tempRed{0}, tempGreen{0}, tempBlue{0}, tempCct{0}, tempCctdn40{0}, tempIlluminance{0};

	if( sampleSize > 0 ){

		aRed[sampleStep] = col.red;
		aGreen[sampleStep] = col.green;
		aBlue[sampleStep] = col.blue;
		aCct[sampleStep] = col.cct;
		aCct_dn40[sampleStep] = col.cct_dn40;
		aIlluminance[sampleStep] = col.illuminance;

		if( ++sampleStep >= sampleSize ) sampleStep = 0;

		for ( uint8_t i = 0; i < sampleSize; ++i ){
			tempRed += aRed[i];
			tempGreen += aGreen[i];
			tempBlue += aBlue[i];
			tempCct += aCct[i];
			tempCctdn40 += aCct_dn40[i];
			tempIlluminance += aIlluminance[i];
		}

		red = tempRed / sampleSize;
		green = tempGreen / sampleSize;
		blue = tempBlue / sampleSize;
		cct = tempCct / sampleSize;
		cct_dn40 = tempCctdn40 / sampleSize;
		illuminance = tempIlluminance / sampleSize;
	}
	else{

		red = col.red;
		green = col.green;
		blue = col.blue;
		cct = col.cct;
		cct_dn40 = col.cct_dn40;
		illuminance = col.illuminance;
	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::setValues(void){

	OD_RAM.x6400_red_RGB888 = static_cast<uint8_t>(red);
	OD_RAM.x6401_green_RGB888 = static_cast<uint8_t>(green);
	OD_RAM.x6402_blue_RGB888 = static_cast<uint8_t>(blue);

	OD_RAM.x6403_cct_ColorTemperature = static_cast<uint16_t>(cct);
	OD_RAM.x6404_illimunate = static_cast<uint16_t>(illuminance);

	cn.sendRequest();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
// call every 20ms
void PROCESS::WriteScreenSequence(void){

	if ( isAwake ){

		switch(_mp){

		case FUNCTION_PAGE:

			functionPage();

			if (isScreenSaverOn ) {
				if ( ++stepOsDelay > longOsDelay ) { if( isAwake ) sleep(); }
			}

			// if both first pressed ACTIVATE and then SELECT ==> go to reset
			if ( bt.getActivate() ){

				if ( bt.getSelect() ) HAL_NVIC_SystemReset(); // while(true); //WDT reset!..
			}

			// if SELECT ==> go to adjust
			if ( bt.getSelect() ) {
				stepOsDelay = 0;
				ol.clear(); _mp = ADJUST_PAGE;
			}

			break;

		case ADJUST_PAGE:

			adjustPage();

			if ( ++stepOsDelay > longOsDelay ) { backToFuncPage(); }
			break;

//		case CALIBRATION_PAGE:
//			calibrationPage();
//			break;

		default: break;
		}

		// stop counting for sleep
		if ( bt.getActivate() || bt.getSelect() ) stepOsDelay = 0;

	}
	else{
		// if pressed any button ==> wake up
		if ( bt.getActivate() || bt.getSelect() ) {
			if( !isAwake ) { wakeUp();	_mp = FUNCTION_PAGE; }
		}
	}

}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::setNewPage(void)				{ funcPageStep = 0; ol.setTextSize(true); ol.clear(); 	}
//--------------------------------------------------------------------//
void PROCESS::backToFuncPage(void)			{ _mp = FUNCTION_PAGE; setNewPage(); 					}
//--------------------------------------------------------------------//
void PROCESS::backToCalibrationPage(void)	{ _mp = CALIBRATION_PAGE; setNewPage();					}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::functionPage(void){

	switch(funcPageStep){

	case 0: ol(0, 0)  		<< "#"   >> 4 << col.processCount; 	break;
	case 1: ol(48, 0) 		<< "Ld"  >> 3 << ldDuty; 			break;

	case 2: ol(96, 0)[false] << (cn.getGreenLed() ? '|' : '}') >> 3 << getCoBaud();
			ol(96, 1) 		 << (cn.getRedLed()   ? '|' : '}') >> 2 << getCoAddress();  break;

	case 3: ol(0, 2)[true]  << "R:"   >> 3 << red; 			break;
	case 4: ol(48, 2) 		<< "cct:" >> 5 << cct; 			break;
	case 5: ol(0, 4)  		<< "G:"   >> 3 << green; 		break;
	case 6: ol(48, 4) 		<< "dn4:" >> 5 << cct_dn40; 	break;
	case 7: ol(0, 6)  		<< "B:"   >> 3 << blue; 		break;
	case 8: ol(48, 6) 		<< "lux:" >> 5 << illuminance; 	break;
	default: break;
	}
	if(++funcPageStep > 8 ) funcPageStep = 0;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::adjustPage(void){

	ol(0, 0)[false] << ((adjPageStep > 1) ? arAdjMenu[adjPageStep-2].name : strMenuList );
	ol(0, 1)		<< ((adjPageStep > 0) ? arAdjMenu[adjPageStep-1].name : arAdjMenu[5].name );
					 //"---------------"
	ol(0, 2)		<< hyphen;
	ol(0, 3)[true]	<< arAdjMenu[adjPageStep].name;
	ol(0, 5)[false]	<< hyphen;
	ol(0, 6) 		<< ((adjPageStep+1 < 6) ? arAdjMenu[adjPageStep+1].name : arAdjMenu[0].name );
	ol(0, 7) 		<< ((adjPageStep+2 < 6) ? arAdjMenu[adjPageStep+2].name : arAdjMenu[1].name );


	switch(adjPageStep){  //arAdjMenu[adjPageStep].funcNum){
	// setAdjMainShow     //setAdjSelectionsShow     //21 small 15 big
	case 0:				  //"---------------------"  //"---------------"

		ol(64, 2)[false]	<< hyphen;
		ol(64, 3)[true] 	<< strExit;
		ol(64, 5)[false]	<< hyphen;

		if ( bt.getActivate() ) backToFuncPage();
		if ( bt.getSelect() ) { ol.clear(); ++adjPageStep; }
		break;

	case 1:
		ol(64, 0)[false] 	<< ((coBaudSelect > 1) ? arBaudSelection[coBaudSelect-2].name : arBaudSelection[3].name );
		ol(64, 1)			<< ((coBaudSelect > 0) ? arBaudSelection[coBaudSelect-1].name : arBaudSelection[4].name );
		ol(64, 2)			<< hyphen;
		ol(64, 3)[true]		<< arBaudSelection[coBaudSelect].name;
		ol(64, 5)[false]	<< hyphen;
		ol(64, 6) 			<< ((coBaudSelect+1 < 5) ? arBaudSelection[coBaudSelect+1].name : arBaudSelection[0].name );
		ol(64, 7) 			<< ((coBaudSelect+2 < 5) ? arBaudSelection[coBaudSelect+2].name : arBaudSelection[1].name );

		if ( bt.getActivate() ) if(++coBaudSelect > 4) coBaudSelect=0;
		if ( bt.getSelect() ) { ol.clear(); ++adjPageStep; }
		break;

	case 2:
		ol(64, 1)[false]	<< coAddress-1;
		ol(64, 2)			<< hyphen;
		ol(64, 5)  	        << hyphen;
		ol(64, 6)			<< coAddress+1;
		ol(64, 3)[true]		<< coAddress;

		if ( bt.getActivate() ) if(++coAddress > 64) coAddress = 14;
		if ( bt.getSelect() ) { ol.clear();  ++adjPageStep; }
		break;

	case 3:

		(sampleSize < 4 ? ol(64, 1)[false]	<< strNoSample :
				 ol(64, 1)[false]	<< sampleSize-8);
		ol(64, 2)			<< hyphen;
		ol(64, 5)  	        << hyphen;
		ol(64, 6)			<< sampleSize+8;

		if(isResultSampling){ ol(64, 3)[true]	>> 2  << sampleSize; 	}
		else				{ ol(64, 3)[true]	>> 2  << strNoSample; 	}

		if ( bt.getActivate() ) {
			sampleSize += 8;
			if(sampleSize > 32) {
				isResultSampling = false; sampleSize = 0;
			}
			else isResultSampling = true;
		}
		if ( bt.getSelect() ) { ol.clear();  ++adjPageStep; } //

		break;

	case 4:

		ol(64, 2)[false]	<< hyphen;
		ol(64, 5)  	        << hyphen;
		ol(64, 3)[true]		<< ( isScreenSaverOn ? "saverON" : "sav.OFF" );

		if ( bt.getActivate() ) {
			if(isScreenSaverOn) isScreenSaverOn = false;
			else isScreenSaverOn = true;
		}
		if ( bt.getSelect() ) { ol.clear(); ++adjPageStep; } // adjPageStep=0;
		break;

	case 5:

		ol(64, 2)[false]	<< hyphen;

		if ( isSavingValues ) ol(64, 3)[true] << strSave;
//		else if ( isSavingValues == 2 ) ol(64, 3)[true] << strCalib;
		else ol(64, 3)[true] << strExit;

		ol(64, 5)[false]	<< hyphen;

		if ( bt.getActivate() ) { isSavingValues ? isSavingValues = false : isSavingValues = true; } //{ if ( ++isSavingValues > 2 ) isSavingValues = 0; }
		if ( bt.getSelect() ) {

			adjPageStep = 0;
			//save : baud, sample, screen saver, max light value, calibration parameter
			if (isSavingValues ) 		{ savingValues(); 			}       	//			else if ( isSavingValues == 2 )	{ backToCalibrationPage(); 	}
			else						{ backToFuncPage(); 		}
		}
		break;



	default: adjPageStep = 0; break;
	}

}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//void PROCESS::calibrationPage(void){
//
//	switch(funcPageStep){
//
//	case 0: ol(0, 0)[true]  <<  "r"  >> 3 << col.red; 	break;
//	case 1: ol 		  		<< "/g"  >> 3 << col.green; break;
//	case 2: ol   	  		<< "/b"  >> 3 << col.blue; 	break;
//
//	case 3: ol(0, 2)[false]  <<  "R:"  >> 4 << col.rawRed;  	break;
//	case 4: ol   			 << " G:"  >> 4 << col.rawGreen;  	break;
//	case 5: ol   			 << " B:"  >> 4 << col.rawBlue;  	break;
//	case 6: ol(0, 3)   		 <<  "C:"  >> 4 << col.rawClear;  	break;
//	case 7:  break;
//	default: break;
//	}
//	if( ++funcPageStep > 7 ) funcPageStep = 0;
//
//	if ( bt.getActivate() ) {
//
//	}
//
//	if ( bt.getSelect() ) {
//		backToFuncPage();
//	}
//}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::sleep(void){
	ol.clear();
	ol.oled_off();
	isAwake = false;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::wakeUp(void){
	ol.oled_on();
	ol.setTextSize(true);

	isAwake = true;
	stepOsDelay = 0;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::init(void) {

	//fisrt set infrared light 68KHz and %20 duty
	ld.init( 100, 68'000, 500, 80 );

	readingValues();

	ol.init();
//	HAL_GPIO_EXTI_IRQHandler(COLOR_INT_B0_Pin);
//	ol(0, 0)[true] << "Hiper Teknoloji";

	ol(0, 2) >> 5 << col.init();

	cn.init(getCoAddress(), getCoBaud());

	ol.clear();
	ol.setTextSize(true);

	arAdjMenu[0].name = "Adjust :";
	arAdjMenu[1].name = "CmSpeed:";
	arAdjMenu[2].name = "Address:";
	arAdjMenu[3].name = "Sample :";
	arAdjMenu[4].name = "ScreenS:";
	arAdjMenu[5].name = "SaveExt:";

	arBaudSelection[0].name = "50K  bd";
	arBaudSelection[1].name = "125K bd";
	arBaudSelection[2].name = "250K bd";
	arBaudSelection[3].name = "500K bd";
	arBaudSelection[4].name = "1000Kbd";


}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::readingValues(void){

	uint32_t buf[5]{0}; // = ;

	fl.read(buf, 5);

	if( buf[0] == reinterpret_cast<uint32_t>(strHiper) ){

		coBaudSelect = static_cast<int>(buf[1]);
		coAddress = static_cast<uint8_t>(buf[2]);
		sampleSize = static_cast<uint8_t>(buf[3]);
		isScreenSaverOn = static_cast<bool>(buf[4]);
	}
	else{
		coBaudSelect = 2;
		coAddress = 14;
		sampleSize = 0;
		isScreenSaverOn = true;
	}

	if ( sampleSize != 0 ) isResultSampling = true;
	else isResultSampling = false;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void PROCESS::savingValues(void){

	uint32_t buf[5]; // = {0};

	buf[0] = reinterpret_cast<uint32_t>(strHiper);
	buf[1] = static_cast<uint32_t>(coBaudSelect);
	buf[2] = static_cast<uint32_t>(coAddress);
	buf[3] = static_cast<uint32_t>(sampleSize);
	buf[4] = static_cast<uint32_t>(isScreenSaverOn);
	fl.write(buf, 5);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void global_OLED_SDA(bool _sta){
	HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, _sta ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void global_OLED_SCL(bool _sta){
	HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, _sta ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
bool global_INT_COL(void){
	return ( HAL_GPIO_ReadPin(COLOR_INT_B0_GPIO_Port, COLOR_INT_B0_Pin) == GPIO_PIN_SET ) ? true : false;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
bool global_ACTIVATE_BUTTON(void){
	return ( HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_SET ) ? false : true;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
bool global_SELECT_BUTTON(void){
	return ( HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin) == GPIO_PIN_SET ) ? false : true;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
