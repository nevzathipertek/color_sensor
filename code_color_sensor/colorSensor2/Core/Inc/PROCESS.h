//program::process of iz-1881 control header file
// @uthor::Nevzat DURMAZ
//   date::13.09.2023
// update::14.09.2023
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef SRC_PROCESS_H_
#define SRC_PROCESS_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#include <F1FLASH.h>
#include <OLED.h>
#include <CANBUS.h>
#include <PWMOUT.h>
#include <TCS3472.h>
#include <BUTTONS.h>
#include <array>
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
using namespace hiper;
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c2;
extern IWDG_HandleTypeDef hiwdg;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//FUNCTION DEFINATIONS FOR F.POINTER//
void global_led1(bool _state);
void global_OLED_SDA(bool _sta);
void global_OLED_SCL(bool _sta);
bool global_INT_COL(void);
bool global_SELECT_BUTTON(void);
bool global_ACTIVATE_BUTTON(void);
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef ADJUST_MENUSIZE
#define ADJUST_MENUSIZE 		6 //16
#endif
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define 	PWM_MIN_DUTYVALUE	 	40
#define 	PWM_MAX_DUTYVALUE	 	360 //450 //360
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class PROCESS final {

	typedef enum{
		FUNCTION_PAGE,
		ADJUST_PAGE,
		CALIBRATION_PAGE,
		NONE_OF_THEM,
	}MENU_PAGES_t;

	struct adj_menu_main_t{
		const char* name{};
	};

	struct adj_menu_selection_str_t{
		const char* name{};
	};


	std::array<adj_menu_main_t, ADJUST_MENUSIZE> arAdjMenu;
	std::array<adj_menu_selection_str_t, 5> arBaudSelection;

	const char* strMenuList{"MENU LIST"};
	const char* strSave{"  Save?"};
	const char* strExit{"  Exit?"};
	const char* strCalib{"Calibra"};
	const char* strNoSample{"no Samp"};
	const char* strHiper{"ALP*"};

	inline int getMax(int &r, int &g, int &b){
		return (r > g) ? (r > b ? r : b) : (g > b ? g : b);
	}

public:

	F1FLASH fl{};
	OLED ol{ global_OLED_SDA, global_OLED_SCL, false, 10 };

	CANBUS cn{ &hcan, &htim4, nullptr };

	PWMOUT ld{ &htim2, 1, 36'000'000 };
	TCS3472 col{ &hi2c2, global_INT_COL };
	BUTTONS bt{global_SELECT_BUTTON, global_ACTIVATE_BUTTON, 20};

	MENU_PAGES_t _mp{FUNCTION_PAGE};

	int stepOsDelay{0}, funcPageStep{0}, adjPageStep{0};

	std::array<int, 32> aRed;
	std::array<int, 32> aGreen;
	std::array<int, 32> aBlue;
	std::array<int, 32> aCct;
	std::array<int, 32> aCct_dn40;
	std::array<int, 32> aIlluminance;

	int red{0}, green{0}, blue{0}, cct{0}, cct_dn40{0}, illuminance{0};

	unsigned int ldDuty{50};

	uint8_t coAddress{14}, coBaudSelect{0}, sampleSize{0}, //isSavingValues{0},
			sampleStep{0};

	bool isAwake{true}, isResultSampling{false}, isScreenSaverOn{true}, isSavingValues{false};



	inline PROCESS() = delete;

	inline explicit PROCESS( void (*_CanInit_)(void) ) :

		fl{}, ol{ global_OLED_SDA, global_OLED_SCL, false, 10 },
		cn{ &hcan, &htim4, _CanInit_ }, ld{ &htim2, 1, 36'000'000 },
		col{ &hi2c2, global_INT_COL }, bt{global_SELECT_BUTTON, global_ACTIVATE_BUTTON, 20},
		stepOsDelay{0}, funcPageStep{0}, adjPageStep{0}, ldDuty{50},
		coAddress{14}, coBaudSelect{0}, sampleSize{0}, //isSavingValues{0},
		isAwake{true}, isResultSampling{false}, isScreenSaverOn{true}, isSavingValues{false}
		{}


	inline virtual ~PROCESS() {}


	void init(void);
	//	void canPollTask(void);

	void processing(void);
	void valuesProcessing(void);
	void getValues(void);
	void setValues(void);

	void WriteScreenSequence(void);
	void setNewPage(void);
	void backToFuncPage(void);
	void backToCalibrationPage(void);

	void functionPage(void);
	void adjustPage(void);

	void sleep(void);
	void wakeUp(void);
	void readingValues(void);
	void savingValues(void);


//	void calibrationPage(void);


	inline uint8_t getCoAddress(void)const { return coAddress; 	}

	inline uint8_t getCoBaud(void) {
		uint8_t baud;
		switch(coBaudSelect){
		case 0: 	baud = 5; 	break;
		case 1: 	baud = 12; 	break;
		case 2: 	baud = 25; 	break;
		case 3: 	baud = 50; 	break;
		case 4: 	baud = 100; break;
		default: 	baud = 0; 	break;
		}
		return baud;
	}

};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//	// printing arrays
//	template <typename Accu, typename T, size_t size>
//	inline void print(Accu& ol, const std::array<T, size>& ar){
//		ol << '[';
//		for (size_t i=0; i<ar.size()-1; ++i) ol << ar[i] << ',';
//		ol << ar.back() << ']';
//	}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* SRC_PROCESS_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
