//program::TCS3472 color sensor header file
// @uthor::Nevzat DURMAZ
//   date::22.03.2024
// update::22.03.2024
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef SRC_TCS3472_H_
#define SRC_TCS3472_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//TCS34721† 0x39 FN−6 I2C Vbus = VDD Interface TCS34721FN
//TCS34723† 0x39 FN−6 I2C Vbus = 1.8 V Interface TCS34723FN
//TCS34725 0x29 FN−6 I2C Vbus = VDD Interface TCS34725FN
//TCS34727 0x29 FN−6 I2C Vbus = 1.8 V Interface TCS34727FN
// the address 0x29 so we shift one for 7 byte address value ==> 0x29 << 1 = 0x52
#define TCS34725_ADDRESS 			0x52 //
//--------------------------------------------------------------------//
#define TCS34725_ID    				0x12
//--------------------------------------------------------------------//
// Integration time settings for TCS34725
#define TCS34725_ATIME 					0x01 //Integration time
// 60-Hz period: 16.67ms, 50-Hz period: 20ms
// 100ms is evenly divisible by 50Hz periods and by 60Hz periods
#define TCS34725_INTEGRATIONTIME_2_4MS  0xFF // 2.4ms - 1 cycle - Max Count: 1024//
#define TCS34725_INTEGRATIONTIME_24MS 	0xF6 // 24.0ms - 10 cycles - Max Count: 10240//
#define TCS34725_INTEGRATIONTIME_50MS  	0xEB // 50.4ms - 21 cycles - Max Count: 21504//
#define TCS34725_INTEGRATIONTIME_60MS 	0xE7 // 60.0ms - 25 cycles - Max Count: 25700//
#define TCS34725_INTEGRATIONTIME_101MS 	0xD6 // 100.8ms - 42 cycles - Max Count: 43008//
#define TCS34725_INTEGRATIONTIME_120MS 	0xCE // 120.0ms - 50 cycles - Max Count: 51200//
#define TCS34725_INTEGRATIONTIME_154MS 	0xC0 // 153.6ms - 64 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_180MS 	0xB5 // 180.0ms - 75 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_199MS 	0xAD // 199.2ms - 83 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_240MS 	0x9C // 240.0ms - 100 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_300MS 	0x83 // 300.0ms - 125 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_360MS 	0x6A // 360.0ms - 150 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_401MS 	0x59 // 400.8ms - 167 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_420MS 	0x51 // 420.0ms - 175 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_480MS 	0x38 // 480.0ms - 200 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_499MS 	0x30 // 499.2ms - 208 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_540MS 	0x1F // 540.0ms - 225 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_600MS 	0x06 // 600.0ms - 250 cycles - Max Count: 65535//
#define TCS34725_INTEGRATIONTIME_614MS 	0x00 // 614.4ms - 256 cycles - Max Count: 65535//
//--------------------------------------------------------------------//
#define TCS34725_CONFIG 					0x0D // Configuration
#define TCS34725_CONFIG_WAITLONG_ON  		0x02
#define TCS34725_CONFIG_WAITLONG_OFF 		0x00
//--------------------------------------------------------------------//
#define TCS34725_WTIME 				0x03 // Wait time (if TCS34725_ENABLE_WEN is asserted)
#define TCS34725_WTIME_2_4MS 		0xFF // WLONG0 = 2.4ms   WLONG1 = 0.029s
#define TCS34725_WTIME_204MS 		0xAB // WLONG0 = 204ms   WLONG1 = 2.45s
#define TCS34725_WTIME_614MS 		0x00 // WLONG0 = 614ms   WLONG1 = 7.4s
//--------------------------------------------------------------------//
#define TCS34725_CONTROL 			0x0F //Set the gain level for the sensor
// Gain settings for TCS34725
#define TCS34725_GAIN_1X 				0x00 //No gain
#define TCS34725_GAIN_4X 				0x01 //4x gain
#define TCS34725_GAIN_16X 				0x02 //16x gain
#define TCS34725_GAIN_60X 				0x03 //60x gain
//--------------------------------------------------------------------//
#define TCS34725_PERS  				0x0C // Persistence register - basic SW filtering mechanism for interrupts//
#define TCS34725_PERS_NONE 			0b0000 // Every RGBC cycle generates an interrupt//
#define TCS34725_PERS_1_CYCLE 		0b0001 // 1 clean channel value outside threshold range generates an interrupt//
#define TCS34725_PERS_2_CYCLE  		0b0010 // 2 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_3_CYCLE 		0b0011 // 3 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_5_CYCLE  		0b0100 // 5 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_10_CYCLE 		0b0101 // 10 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_15_CYCLE 		0b0110 // 15 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_20_CYCLE 		0b0111 // 20 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_25_CYCLE 		0b1000 // 25 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_30_CYCLE 		0b1001 // 30 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_35_CYCLE  	0b1010 // 35 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_40_CYCLE 		0b1011 // 40 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_45_CYCLE 		0b1100 // 45 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_50_CYCLE 		0b1101 // 50 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_55_CYCLE  	0b1110 // 55 clean channel values outside threshold range generates an interrupt//
#define TCS34725_PERS_60_CYCLE 		0b1111 // 60 clean channel values outside threshold range generates an interrupt//
//--------------------------------------------------------------------//
#define TCS34725_AILTL 				0x04 // Clear channel lower interrupt threshold (lower byte)//
#define TCS34725_AILTH 				0x05 // Clear channel lower interrupt threshold (higher byte)//
#define TCS34725_AIHTL 				0x06 // Clear channel upper interrupt threshold (lower byte)//
#define TCS34725_AIHTH  			0x07 // Clear channel upper interrupt threshold (higher byte)//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define TCS34725_STATUS 			0x13 // Device status //
//--------------------------------------------------------------------//
#define TCS34725_STATUS_AINT 		0x10 // RGBC Clean channel interrupt//
#define TCS34725_STATUS_AVALID 		0x01 // Indicates that the RGBC channels have completed an integration cycle//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define TCS34725_GETALLRAWDATA 			0b10110100
#define TCS34725_CDATAL 				0x14 // Clear channel data low byte//
#define TCS34725_CDATAH 				0x15 // Clear channel data high byte//
#define TCS34725_RDATAL 				0x16 // Red channel data low byte//
#define TCS34725_RDATAH 				0x17 // Red channel data high byte//
#define TCS34725_GDATAL 				0x18 // Green channel data low byte//
#define TCS34725_GDATAH 				0x19 // Green channel data high byte//
#define TCS34725_BDATAL 				0x1A // Blue channel data low byte//
#define TCS34725_BDATAH 				0x1B // Blue channel data high byte//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
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
#define TCS34725_COMMAND_NOINCREMET 					0b10000000
#define TCS34725_COMMAND_AUTOINCREMET 					0b10100000
#define TCS34725_COMMAND_CLEARINT_AUTOINCREMENT 		0b11100110
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define TCS34725_ENABLE 					0x00 // Enable Register
#define TCS34725_ENABLE_AIEN_AEN_PON 		0x13
//--------------------------------------------------------------------//
#define TCS34725_ENABLE_AIEN 				0x10 // RGBC interrupt enable. When asserted, permits RGBC interrupts to be generated.
#define TCS34725_ENABLE_WEN  				0x08 // Wait Enable - Writing 1 activates the wait timer
#define TCS34725_ENABLE_AEN  				0x02 // RGBC Enable - Writing 1 actives the ADC, 0 disables it
#define TCS34725_ENABLE_PON  				0x01 // Power on - Writing 1 activates the internal oscillator, 0 disables it
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define TCS34725_R_COEF       0.136
#define TCS34725_G_COEF       1.000
#define TCS34725_B_COEF       -0.444
#define TCS34725_DF           310.0 	// Device factor
#define TCS34725_CT_COEF      3810.0 	// Color temp coef for CT calculation
#define TCS34725_CT_OFFSET    1391.0 	// Color temp offset for CT calculation
#define TCS34725_GA           1.0 		// Glass attenuation factor; put 1.08 if behind clear glass
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class TCS3472 {

	I2C_HandleTypeDef* __pI2cUsed{nullptr};
	bool (*__int)(void);

	uint8_t _id{0};
	uint8_t devStatus{0};

	void writeByte(uint8_t reg, uint8_t value);
	uint8_t readByte(uint8_t reg);


public:

	int rawRed{0}, rawGreen{0}, rawBlue{0}, rawClear{0}, ir{0};

	int processCount{0}, red{0}, green{0}, blue{0},
			cct{0}, cct_dn40{0}, illuminance{0};


	inline TCS3472() = delete;

	explicit inline TCS3472(I2C_HandleTypeDef* _pI2c_, bool (*_INT_)(void) ) :
			__pI2cUsed{_pI2c_},	__int{_INT_},
			_id{0}, devStatus{0},
			rawRed{0}, rawGreen{0}, rawBlue{0}, rawClear{0},
			processCount{0}, red{0}, green{0}, blue{0},
			cct{0}, cct_dn40{0}, illuminance{0} {}

	inline virtual ~TCS3472() {}
											//FC for 10 ms.//F8 (248) for 20ms. //TCS34725_INTEGRATIONTIME_2_4MS,
	uint8_t init( uint8_t integration_time = 0xF8, uint8_t control_gain = TCS34725_GAIN_60X,
			uint8_t int_persistence = TCS34725_PERS_NONE, uint8_t wait_long = TCS34725_CONFIG_WAITLONG_OFF,
			uint8_t wait_time = TCS34725_WTIME_2_4MS );

	void updateData(void);
	void updateDataInt(void);

	void getStatus(void);
	void clearStatus(void);

	void getRawData(void);
	void enable(void);
	void disable(void);

	bool getRGB(void);

	void colorTemp(void);
	void colorTemp_dn40(void);
	void calculateLux(void);

	inline uint8_t getId(void)const 			{ return _id; 				}
	inline bool getIntPos(void)const 			{ return __int(); 			}
	inline int getProcessCount(void)const 		{ return processCount; 		}
	inline int getColorTemp(void)const 			{ return cct; 				}
};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* SRC_TCS3472_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
