//program::TCS3472 color sensor source file
// @uthor::Nevzat DURMAZ
//   date::22.03.2024
// update::22.03.2024
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
extern "C"{
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal_exti.h"
#include "stm32f1xx_hal_cortex.h"
}
#include "TCS3472.h"
#include "math.h"
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
///* USER CODE BEGIN 4 */     ==> if using EXT pin PASTE TO MAIN ==
//extern "C"
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	if ( GPIO_Pin == COLOR_INT_B0_Pin ) {
//		pr.col.updateDataInt();
//	}
//}
///* USER CODE END 4 */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
float powf(const float x, const float y) {
  return (float)(pow((double)x, (double)y));
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::updateDataInt(void) {
	getRawData();
	clearStatus();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::updateData(void) {

	if( getRGB() ){
		colorTemp();
		colorTemp_dn40();
		calculateLux();
	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
// returning the id if(id==0) means DID NOT INITILAZED
// integrationTime = (256 - _tcs34725IntegrationTime) * 12 / 5 + 1
uint8_t TCS3472::init( uint8_t integration_time, uint8_t control_gain, uint8_t int_persistence,
		uint8_t wait_long, uint8_t wait_time ){

//	ID Register (0x12)
//	The ID Register provides the value for the part number. The ID register is a read-only register.
//	FIELD 	BITS 		DESCRIPTION					//0x44 = TCS34721 and TCS34725
//	ID 		7:0 		Part number identification  //0x4D = TCS34723 and TCS34727
	uint8_t id = readByte(TCS34725_ID);

	if ((id != 0x4d) && (id != 0x44) && (id != 0x10)) {
		return _id;
	}
	else _id = id;

//	disable();

//	RGBC Timing Register (0x01)
//	The RGBC timing register controls the internal integration time of the RGBC clear and IR channel ADCs in
//	2.4-ms increments. Max RGBC Count = (256 − ATIME) × 1024 up to a maximum of 65535.
//	VALUE 		INTEG_CYCLES 		TIME 			MAX COUNT
//	0xFF 		1 					2.4 ms 			1024
//	0xF6 		10 					24 ms 			10240
//	0xD5 		42 					101 ms 			43008
//	0xC0 		64 					154 ms 			65535
//	0x00 		256 				700 ms 			65535
	// ATIME = 256 − Integration Time / 2.4 ms
	// Integration Time = 2.4 ms × (256 − ATIME)
	//for 20ms integration => // ATIME = 256 - (20/2.4) = 248 (0xF8)
	writeByte(TCS34725_ATIME,  integration_time);  //2.4ms

//	Configuration Register (0x0D)
//	The configuration register sets the wait long time.  0x0D: b1
//	WLONG 1 Wait Long. When asserted, the wait cycles are increased by a factor 12×
//	from that programmed in the WTIME register.
	writeByte(TCS34725_CONFIG,  wait_long);


//	Wait Time Register (0x03)
//	Wait time is set 2.4 ms increments unless the WLONG bit is asserted,
//	in which case the wait times are 12× longer. WTIME is programmed as a 2’s complement number.
//	REGISTER VALUE 		WAIT TIME 		TIME (WLONG = 0) 		TIME (WLONG = 1)
//	0xFF 				1 				2.4 ms 					0.029 sec
//	0xAB 				85 				204 ms 					2.45 sec
//	0x00 				256 			614 ms 					7.4 sec
	writeByte(TCS34725_WTIME,  wait_time); //2.4ms

//	Control Register (0x0F)
//	The Control register provides eight bits of miscellaneous control to the analog block.
//	These bits typically control functions such as gain settings and/or diode selection.
//	FIELD 		BITS 		DESCRIPTION
//	Reserved 	7:2 		Reserved. Write bits as 0
//	AGAIN 		1:0 		RGBC Gain Control.
//							FIELD VALUE 		RGBC GAIN VALUE
//							00 					1× gain
//							01 					4× gain
//							10 					16× gain
//							11 					60× gain
	writeByte(TCS34725_CONTROL,  control_gain); //1x

//	Persistence Register (0x0C)
//	The persistence register controls the filtering interrupt capabilities of the device. Configurable filtering is
//	provided to allow interrupts to be generated after each integration cycle or if the integration has produced a result
//	that is outside of the values specified by the threshold register for some specified amount of time.
//	FIELD 	BITS 		DESCRIPTION
//	PPERS 	7:4 		Reserved
//	APERS 	3:0 		Interrupt persistence. Controls rate of interrupt to the host processor.
//						FIELD VALUE 	MEANING 		INTERRUPT PERSISTENCE FUNCTION
//						0000 			Every 			Every RGBC cycle generates an interrupt
//						0001 			1 				1 clear channel value outside of threshold range
//						0010 			2 				2 clear channel consecutive values out of range
//						0011 			3 				3 clear channel consecutive values out of range
//						0100 			5 				5 clear channel consecutive values out of range
//						0101 			10 				10 clear channel consecutive values out of range
//						0110 			15 				15 clear channel consecutive values out of range
//						0111 			20 				20 clear channel consecutive values out of range
//						1000 			25 				25 clear channel consecutive values out of range
//						1001 			30 				30 clear channel consecutive values out of range
//						1010 			35 				35 clear channel consecutive values out of range
//						1011 			40 				40 clear channel consecutive values out of range
//						1100 			45 				45 clear channel consecutive values out of range
//						1101 			50 				50 clear channel consecutive values out of range
//						1110 			55 				55 clear channel consecutive values out of range
//						1111 			60 				60 clear channel consecutive values out of range
	writeByte(TCS34725_PERS, int_persistence);

//	RGBC Interrupt Threshold Registers (0x04 − 0x07)
//	The RGBC interrupt threshold registers provides the values to be used as the high and low trigger points for
//	the comparison function for interrupt generation. If the value generated by the clear channel crosses below the
//	lower threshold specified, or above the higher threshold, an interrupt is asserted on the interrupt pin.
//	REGISTER 	ADDRESS 	BITS 		DESCRIPTION
//	AILTL 		0x04 		7:0 		RGBC clear channel low threshold lower byte
//	AILTH 		0x05 		7:0 		RGBC clear channel low threshold upper byte
//	AIHTL 		0x06 		7:0 		RGBC clear channel high threshold lower byte
//	AIHTH 		0x07 		7:0 		RGBC clear channel high threshold upper byte
	writeByte(TCS34725_AILTL,  0x00);
	writeByte(TCS34725_AILTH,  0x20);
	writeByte(TCS34725_AIHTL,  0x00);
	writeByte(TCS34725_AIHTH,  0x20);


	clearStatus();
	enable();

	return _id;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//The Status Register provides the internal status of the device. This register is read only.
//FIELD  		BIT 		DESCRIPTION
//Reserved 		7:5 		Reserved.
//AINT 			4 			RGBC clear channel Interrupt.
//Reserved 		3:1 		Reserved.
//AVALID 		0 			RGBC Valid. Indicates that the RGBC channels have completed an integration cycle.
void TCS3472::getStatus(void) {

	devStatus = readByte(TCS34725_STATUS);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::clearStatus(void) {

	writeByte(TCS34725_COMMAND_CLEARINT_AUTOINCREMENT, 0x00);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::getRawData(void) {

	uint8_t _data[8];
	HAL_I2C_Mem_Read(__pI2cUsed, TCS34725_ADDRESS, TCS34725_GETALLRAWDATA, 1, _data, 8, 100);

	rawClear 	= 	static_cast<int>(( _data[1] << 8 ) + _data[0]);
	rawRed 		= 	static_cast<int>(( _data[3] << 8 ) + _data[2]);
	rawGreen 	= 	static_cast<int>(( _data[5] << 8 ) + _data[4]);
	rawBlue 	= 	static_cast<int>(( _data[7] << 8 ) + _data[6]);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//           7 6 5      4    3       2       1    0
// ENABLE : Reserved  AIEN  WEN  Reserved   AEN  PON   //Address 0x00
//--------------------------------------------------------------------//
// Reserved 7:5 	Reserved. Write as 0.
// AIEN 	4 		RGBC interrupt enable. When asserted, permits RGBC interrupts to be generated.
// WEN 		3 		Wait enable. This bit activates the wait feature. Writing a 1 activates the wait timer. Writing a 0 disables the wait timer.
// Reserved 2 		Reserved. Write as 0.
// AEN 		1 		RGBC enable. This bit actives the two-channel ADC. Writing a 1 activates the RGBC. Writing a 0 disables the RGBC.
// PON 		1 		0 Power ON. This bit activates the internal oscillator to permit the timers and ADC channels to operate.
// 					Writing a 1 activates the oscillator. Writing a 0 disables the oscillator.
//					A minimum interval of 2.4 ms must pass after PON is asserted before an RGBC can be initiated.
//--------------------------------------------------------------------//
void TCS3472::enable(void) {
	writeByte(TCS34725_ENABLE, TCS34725_ENABLE_PON);
	HAL_Delay(3);
//	writeByte(TCS34725_ENABLE, TCS34725_ENABLE_AEN | TCS34725_ENABLE_PON );
	writeByte(TCS34725_ENABLE, TCS34725_ENABLE_AIEN_AEN_PON);
	HAL_Delay(24);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::disable(void) {
	uint8_t reg = 0;
	reg = readByte(TCS34725_ENABLE);
//	writeByte(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_AEN | TCS34725_ENABLE_PON ));
	writeByte(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_AIEN_AEN_PON) );
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
// 	            7     6 5     4 3 2 1 0
//	COMMAND :  CMD   TYPE     ADDR/SF
//--------------------------------------------------------------------//
// TYPE : FIELD VALUE INTEGRATION TIME
//  00    Repeated byte protocol transaction
//  01    Auto-increment protocol transaction
//  10    Reserved — Do not use
//  11    Special function — See description below
//--------------------------------------------------------------------//
//  ADDR/SF : 00110 Clear channel interrupt clear
//--------------------------------------------------------------------//
void TCS3472::writeByte(uint8_t reg, uint8_t value){
	HAL_I2C_Mem_Write(__pI2cUsed, TCS34725_ADDRESS, (TCS34725_COMMAND_NOINCREMET | reg), 1, &value, 1, 100);
}
//--------------------------------------------------------------------//
uint8_t TCS3472::readByte(uint8_t reg){
	uint8_t _data;
	HAL_I2C_Mem_Read(__pI2cUsed, TCS34725_ADDRESS, (TCS34725_COMMAND_NOINCREMET | reg), 1, &_data, 1, 100);
	return _data;
}
//--------------------------------------------------------------------//
//uint16_t TCS3472::readShort(uint8_t reg){
//	uint8_t _data[2];
//	HAL_I2C_Mem_Read(__pI2cUsed, TCS34725_ADDRESS, (TCS34725_COMMAND_AUTOINCREMET | reg), 1, _data, 2, 100);
//	return ( ( _data[1] << 8 ) + _data[0] );
//}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
bool TCS3472::getRGB(void){

	if (rawClear == 0) { return false; }
	else{
		red 	= rawRed / 32; 		// (rawRed * 255) / rawClear ;
		green 	= rawGreen / 32; 	// (rawGreen * 255) / rawClear;
		blue 	= rawBlue / 32; 	// (rawBlue * 255) / rawClear;

	    if( ++processCount > 9999) processCount = 0;
	}

    return true;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::colorTemp(void){

	float X, Y, Z, xc, yc, n;

	if (rawRed == 0 && rawGreen == 0 && rawBlue == 0) { return; }

	// Map RGB values to their XYZ counterparts.
	X = (-0.14282f * rawRed) + (1.54924f * rawGreen) + (-0.95641f * rawBlue);
	Y = (-0.32466f * rawRed) + (1.57837f * rawGreen) + (-0.73191f * rawBlue);
	Z = (-0.68202f * rawRed) + (0.77073f * rawGreen) + ( 0.56332f * rawBlue);

	// Calculate the chromaticity co-ordinates.
	xc = (X) / (X + Y + Z);
	yc = (Y) / (X + Y + Z);

	//3. Use McCamy's formula to determine the CCT.
	n = (xc - 0.3320F) / (0.1858F - yc);

	//	Calculate the final CCT. The results in degrees Kelvin.
	cct = static_cast<int>( (449.0f * powf(n, 3)) + (3525.0f * powf(n, 2)) + (6823.3f * n) + 5520.33f );
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::colorTemp_dn40(void){
	int r2, b2, ir, temp;
//	uint16_t sat = 8192;

	if (rawClear == 0	) 	{ return; }
//	if (rawClear >= sat	) 	{ return; }

	temp = rawRed + rawGreen + rawBlue;
	ir = ( temp > rawClear) ? (temp - rawClear) / 2 : 0;

	r2 = rawRed - ir;
	b2 = rawBlue - ir;

	if (r2 == 0) { return; }

	cct_dn40 = (3810 * b2) / r2 + 1391;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void TCS3472::calculateLux(void) {

  illuminance = static_cast<int>( (-0.32466f * rawRed) + (1.57837f * rawGreen) + (-0.73191f * rawBlue) );
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
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
