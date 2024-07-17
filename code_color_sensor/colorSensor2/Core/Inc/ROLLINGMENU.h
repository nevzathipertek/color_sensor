//program::rolling menu header file
// @uthor::Nevzat DURMAZ
//   date::26.05.2024
// update::26.05.2024
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef SRC_ROLLINGMENU_H_
#define SRC_ROLLINGMENU_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#include <array>
#include <cstring>
#include <algorithm>
#include <OLED.h>
#include <F1FLASH.h>
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef ADJUST_MENUSIZE
#define ADJUST_MENUSIZE 		16
#endif
//--------------------------------------------------------------------//
#ifndef FUNC_MENUSIZE
#define FUNC_MENUSIZE 			10
#endif
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef ADJUST_SELECTION_SIZE0
#define ADJUST_SELECTION_SIZE0 			1 //exit
#endif
//--------------------------------------------------------------------//
#ifndef ADJUST_SELECTION_SIZE1
#define ADJUST_SELECTION_SIZE1 			5 //baud
#endif
//--------------------------------------------------------------------//
#ifndef ADJUST_SELECTION_SIZE2
#define ADJUST_SELECTION_SIZE2 			1 //sample
#endif
//--------------------------------------------------------------------//
#ifndef ADJUST_SELECTION_SIZE3
#define ADJUST_SELECTION_SIZE3 			2 //screen saver
#endif
//--------------------------------------------------------------------//
#ifndef ADJUST_SELECTION_SIZE4
#define ADJUST_SELECTION_SIZE4 			1 //save
#endif
//--------------------------------------------------------------------//
//#ifndef ADJUST_SELECTION_SIZE5
//#define ADJUST_SELECTION_SIZE5 			32
//#endif
////--------------------------------------------------------------------//
//#ifndef ADJUST_SELECTION_SIZE6
//#define ADJUST_SELECTION_SIZE6 			32
//#endif
////--------------------------------------------------------------------//
//#ifndef ADJUST_SELECTION_SIZE7
//#define ADJUST_SELECTION_SIZE7 			32
//#endif
////--------------------------------------------------------------------//
//#ifndef ADJUST_SELECTION_SIZE8
//#define ADJUST_SELECTION_SIZE8 			32
//#endif
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class ROLLINGMENU {

	typedef enum{
		FUNCTION_PAGE,
		ADJUST_PAGE,
		CALIBRATION_PAGE,
		NONE_OF_THEM,
	}MENU_PAGES_t;

	struct adj_menu_selection_str_t{
		const char* name{};
	};

	struct adj_menu_main_t{
		const char* name{};
	};


	struct func_menu_t{
		int* val{};
		const char* name{};
		short nZeros{};
		short x{}, y{};
	};

	OLED& ol;
	F1FLASH& fl;
	volatile bool& sBut, & fBut;

//	std::size_t adjustMainSize{0}, adjustSelectionSize{0};

	int _funcSize{0}, _menuSize{0}, _selectSize1{0},_selectSize2{0},_selectSize3{0},
		_selectSize4{0},_selectSize5{0},_selectSize6{0},_selectSize7{0},_selectSize8{0};

	std::array<adj_menu_main_t, ADJUST_MENUSIZE> arAdjMenu;
	std::array<func_menu_t, FUNC_MENUSIZE> arFuncMenu;

	std::array<adj_menu_selection_str_t, ADJUST_SELECTION_SIZE0> arAdjSlc0;
	std::array<adj_menu_selection_str_t, ADJUST_SELECTION_SIZE1> arAdjSlc1;
	std::array<adj_menu_selection_str_t, ADJUST_SELECTION_SIZE2> arAdjSlc2;
	std::array<adj_menu_selection_str_t, ADJUST_SELECTION_SIZE3> arAdjSlc3;
	std::array<adj_menu_selection_str_t, ADJUST_SELECTION_SIZE4> arAdjSlc4;


	MENU_PAGES_t _mp{FUNCTION_PAGE};


	int stepOsDelay{0}, funcPageStep{0}, adjPageStep{0}, adjSelection1Baud{0},
		adjSelectionStep4{0},adjSelectionStep5{0},adjSelectionStep6{0};

	mutable int shortOsDelay{0};
	mutable int longOsDelay{0};

	uint8_t coAddress{14}, sampleSize{0};

	bool isAwake{true}, isResultSampling{false}, isScreenSaverOn{true}, isSavingValues{false};

	void sleep(void);
	void wakeUp(void);
	void readingValues(void);
	void savingValues(void);
	void adjustPage(void);
	void functionPage(void);
	void calibrationPage(void);
	void backToFuncPage(void);

	void createFuncPage( std::size_t n_size );

public:

	inline ROLLINGMENU() = delete;

	explicit inline ROLLINGMENU(OLED& _oled, F1FLASH& _flash,
			volatile bool& _selectButon, volatile bool& _functionButon, int waitForOsDelay) :
			ol{_oled}, fl{_flash}, sBut{_selectButon}, fBut{_functionButon},
			_mp{FUNCTION_PAGE}, stepOsDelay{0},
			shortOsDelay{ 5'000 / waitForOsDelay }, longOsDelay{ 20'000 / waitForOsDelay }, // longOsDelay{ 30'000 / waitForOsDelay },
			coAddress{14}, sampleSize{0},
			isAwake{true},  isResultSampling{false}, isScreenSaverOn{true}, isSavingValues{false}
			{
				ol.clear();
				ol.setTextSize(true);
				readingValues();
			}

	inline virtual ~ROLLINGMENU(){}

	void show(void);
	void addAdjPage(const char* name);

	void addAdjSelectionStr(unsigned char funcNumSelection,	const char* name);

	void addFuncPage(const char* name,  int* val, short x, short y, short n_size = 4);

	inline uint8_t getCoAddress(void)const { return coAddress; 	}

	inline uint8_t getCoBaud(void) {
		uint8_t baud;
		switch(adjSelection1Baud){
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
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* SRC_ROLLINGMENU_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
