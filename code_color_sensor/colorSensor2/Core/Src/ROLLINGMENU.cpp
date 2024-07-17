//program::rolling menu header file
// @uthor::Nevzat DURMAZ
//   date::26.05.2024
// update::26.05.2024
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
extern "C"{
#include "stm32f1xx_hal.h"
}
#include "ROLLINGMENU.h"
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
const char hyphen[] = "-------";
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::addAdjPage(const char* name){

	if ( _menuSize == ADJUST_MENUSIZE-1 ) return;
	arAdjMenu[_menuSize].name = name;
	++_menuSize;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::addAdjSelectionStr(unsigned char funcNumSelection,
		const char* name){

	switch( funcNumSelection ){

	case 0:
		arAdjSlc0[0].name = name;
		break;

	case 1:
		if ( _selectSize1 == ADJUST_SELECTION_SIZE1 ) return;
		arAdjSlc1[_selectSize1].name = name;
		++_selectSize1;
		break;

	case 2:
		if ( _selectSize2 == ADJUST_SELECTION_SIZE2 ) return;
		arAdjSlc2[_selectSize2].name = name;
		++_selectSize2;
		break;

	case 3:
		if ( _selectSize3 == ADJUST_SELECTION_SIZE3 ) return;
		arAdjSlc3[_selectSize3].name = name;
		++_selectSize3;
		break;

	case 4:
		if ( _selectSize4 == ADJUST_SELECTION_SIZE4 ) return;
		arAdjSlc4[_selectSize4].name = name;
		++_selectSize4;
		break;

//	case 5:
//		if ( _selectSize5 == ADJUST_SELECTION_SIZE5 ) return;
//		arAdjSlc5[_selectSize5].name = name;
//		++_selectSize5;
//		break;

	default: break;

	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::addFuncPage(const char* name, int* val, short x, short y,
		short n_size){

	if ( _funcSize == FUNC_MENUSIZE-1 ) return;

	arFuncMenu[_funcSize].name = name;
	arFuncMenu[_funcSize].val = val;
	arFuncMenu[_funcSize].x = x;
	arFuncMenu[_funcSize].y = y;
	arFuncMenu[_funcSize].nZeros = n_size;
	++_funcSize;

}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::show(void){

	if ( isAwake ){

		switch(_mp){
		case FUNCTION_PAGE:
			functionPage();
			break;
		case ADJUST_PAGE:
			adjustPage();
			break;
		case CALIBRATION_PAGE:
			calibrationPage();
			break;
		default:break;
		}

		if ( fBut ) { stepOsDelay = 0; }

		if ( sBut ) {
			stepOsDelay = 0;
			ol.clear(); _mp = ADJUST_PAGE;
		}

	}
	else{

		if ( fBut || sBut ) {
			if( !isAwake ) { wakeUp();	_mp = FUNCTION_PAGE; }
		}
	}

	if (isScreenSaverOn ) {
		if ( ++stepOsDelay > longOsDelay ) { if( isAwake ) sleep(); }
	}

}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::sleep(void){
	ol.clear();
	ol.oled_off();
	isAwake = false;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::wakeUp(void){
	ol.oled_on();
	ol.setTextSize(true);

	isAwake = true;
	stepOsDelay = 0;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::readingValues(void){

	uint32_t buf[5]{0}; // = ;

	fl.read(buf, 5);

	if( buf[0] == reinterpret_cast<uint32_t>("ALP") ){

		adjSelection1Baud = static_cast<int>(buf[1]);
		coAddress = static_cast<uint8_t>(buf[2]);
		sampleSize = static_cast<uint8_t>(buf[3]);
		isScreenSaverOn = static_cast<bool>(buf[4]);
	}
	else{
		adjSelection1Baud = 2;
		coAddress = 32;
		sampleSize = 0;
		isScreenSaverOn = true;
	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::savingValues(void){

	uint32_t buf[5]; // = {0};

	buf[0] = reinterpret_cast<uint32_t>("ALP");
	buf[1] = static_cast<uint32_t>(adjSelection1Baud);
	buf[2] = static_cast<uint32_t>(coAddress);
	buf[3] = static_cast<uint32_t>(sampleSize);
	buf[4] = static_cast<uint32_t>(isScreenSaverOn);
	fl.write(buf, 5);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::adjustPage(void){

	ol(0, 0)[false] << ((adjPageStep > 1) ? arAdjMenu[adjPageStep-2].name : "MENU LIST" );
	ol(0, 1)		<< ((adjPageStep > 0) ? arAdjMenu[adjPageStep-1].name : arAdjMenu[_menuSize-1].name );
					 //"---------------"
	ol(0, 2)		<< hyphen;
	ol(0, 3)[true]	<< arAdjMenu[adjPageStep].name;
	ol(0, 5)[false]	<< hyphen;
	ol(0, 6) 		<< ((adjPageStep+1 < _menuSize) ? arAdjMenu[adjPageStep+1].name : arAdjMenu[0].name );
	ol(0, 7) 		<< ((adjPageStep+2 < _menuSize) ? arAdjMenu[adjPageStep+2].name : arAdjMenu[1].name );


	switch(adjPageStep){ //arAdjMenu[adjPageStep].funcNum){

	// setAdjMainShow  //setAdjSelectionsShow //21 small 15 big
	case 0:				  //"---------------------"  //"---------------"
//		ol(0, 0)[false]  << "you can adjust settng";
//		ol(0, 1)		 << "push(left)sw continue";

		ol(64, 2)[false]	<< hyphen;
		ol(64, 3)[true] 	<< arAdjSlc0[0].name;
		ol(64, 5)[false]	<< hyphen;

//		ol(0, 6)         << "or exit without save!";
//		ol(0, 7)		 << "push(right)thumb EXIT";

		if ( fBut ) backToFuncPage();
		if ( sBut ) { ol.clear(); ++adjPageStep; }
		break;

	case 1:
		ol(64, 0)[false] 	<< ((adjSelection1Baud > 1) ? arAdjSlc1[adjSelection1Baud-2].name : arAdjSlc1[_selectSize1-2].name );
		ol(64, 1)			<< ((adjSelection1Baud > 0) ? arAdjSlc1[adjSelection1Baud-1].name : arAdjSlc1[_selectSize1-1].name );
		ol(64, 2)			<< hyphen;
		ol(64, 3)[true]		<< arAdjSlc1[adjSelection1Baud].name;
		ol(64, 5)[false]	<< hyphen;
		ol(64, 6) 			<< ((adjSelection1Baud+1 < _selectSize1) ? arAdjSlc1[adjSelection1Baud+1].name : arAdjSlc1[0].name );
		ol(64, 7) 			<< ((adjSelection1Baud+2 < _selectSize1) ? arAdjSlc1[adjSelection1Baud+2].name : arAdjSlc1[1].name );

		if ( fBut ) if(++adjSelection1Baud == _selectSize1) adjSelection1Baud=0;
		if ( sBut ) { ol.clear(); ++adjPageStep; }
		break;

	case 2:
		ol(64, 1)[false]	<< coAddress-1;
		ol(64, 2)			<< hyphen;
		ol(64, 5)  	        << hyphen;
		ol(64, 6)			<< coAddress+1;
		ol(64, 3)[true]		<< coAddress;

		if ( fBut ) if(++coAddress > 99) coAddress = 14;
		if ( sBut ) { ol.clear();  ++adjPageStep; }
		break;

	case 3:

		(sampleSize < 4 ? ol(64, 1)[false]	<< arAdjSlc2[0].name :
				 ol(64, 1)[false]	<< sampleSize-4);
		ol(64, 2)			<< hyphen;
		ol(64, 5)  	        << hyphen;
		ol(64, 6)			<< sampleSize+4;

		if(isResultSampling){ ol(64, 3)[true]	>> 2  << sampleSize; 			}
		else				{ ol(64, 3)[true]	>> 2  << arAdjSlc2[0].name; 	}

		if ( fBut ) {
			sampleSize+=4;
			if(sampleSize > 64) {
				isResultSampling = false; sampleSize = 0;
			}
			else isResultSampling = true;
		}
		if ( sBut ) { ol.clear();  ++adjPageStep; } //

		break;

	case 4:

		ol(64, 2)[false]	<< hyphen;
		ol(64, 5)  	        << hyphen;
		ol(64, 3)[true]		<< ( isScreenSaverOn ? arAdjSlc3[0].name : arAdjSlc3[1].name );

		if ( fBut ) {
			if(isScreenSaverOn) isScreenSaverOn = false;
			else isScreenSaverOn = true;
		}
		if ( sBut ) { ol.clear(); ++adjPageStep; } // adjPageStep=0;
		break;

	case 5:

		ol(64, 2)[false]	<< hyphen;
		ol(64, 3)[true] 	<< (isSavingValues ? arAdjSlc4[0].name : arAdjSlc0[0].name);
		ol(64, 5)[false]	<< hyphen;

		if ( fBut ) { isSavingValues ? isSavingValues = false : isSavingValues = true; }
		if ( sBut ) {
			if (isSavingValues) {
				//save : baud, sample, screen saver, max light value, calibration parameter
				savingValues();
			}
			adjPageStep = 0;
			backToFuncPage();
		}
		break;
//	case 6:
//		break;
//	case 7:
//		break;
//	case 8:
//		break;

	default: adjPageStep = 0; break;
	}

}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::calibrationPage(void){




}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::backToFuncPage(void){
	ol.setTextSize(true);
	ol.clear();
	_mp = FUNCTION_PAGE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void ROLLINGMENU::functionPage(void){

	switch(funcPageStep){

	case 0:
		ol(arFuncMenu[0].x, arFuncMenu[0].y) << arFuncMenu[0].name >>
								arFuncMenu[0].nZeros << *arFuncMenu[0].val;
		break;

	case 1:
		ol(arFuncMenu[1].x, arFuncMenu[1].y) << arFuncMenu[1].name >>
								arFuncMenu[1].nZeros << *arFuncMenu[1].val;
		break;

	case 2:
		ol(arFuncMenu[2].x, arFuncMenu[2].y) << arFuncMenu[2].name >>
								arFuncMenu[2].nZeros << *arFuncMenu[2].val;
		break;

	case 3:
		ol(arFuncMenu[3].x, arFuncMenu[3].y) << arFuncMenu[3].name >>
								arFuncMenu[3].nZeros << *arFuncMenu[3].val;
		break;

	case 4:
		ol(arFuncMenu[4].x, arFuncMenu[4].y) << arFuncMenu[4].name >>
								arFuncMenu[4].nZeros << *arFuncMenu[4].val;
		break;

	case 5:
		ol(arFuncMenu[5].x, arFuncMenu[5].y) << arFuncMenu[5].name >>
								arFuncMenu[5].nZeros << *arFuncMenu[5].val;
		break;

	case 6:
		ol(arFuncMenu[6].x, arFuncMenu[6].y) << arFuncMenu[6].name >>
								arFuncMenu[6].nZeros << *arFuncMenu[6].val;
		break;

	case 7:
		ol(arFuncMenu[7].x, arFuncMenu[7].y) << arFuncMenu[7].name >>
								arFuncMenu[7].nZeros << *arFuncMenu[7].val;
		break;
	case 8:
		ol(arFuncMenu[8].x, arFuncMenu[8].y) << arFuncMenu[8].name >>
								arFuncMenu[8].nZeros << *arFuncMenu[8].val;
		break;

	case 9:
		ol(arFuncMenu[9].x, arFuncMenu[9].y) << arFuncMenu[9].name >>
								arFuncMenu[9].nZeros << *arFuncMenu[9].val;
		break;

	case 10:
		ol(arFuncMenu[10].x, arFuncMenu[10].y) << arFuncMenu[10].name >>
								arFuncMenu[10].nZeros << *arFuncMenu[10].val;
		break;
	default:break;
	}
	if(++funcPageStep > _funcSize-1 ) funcPageStep = 0;
}
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
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
