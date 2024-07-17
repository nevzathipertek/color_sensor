//program::BUTTONS driver header file
// @uthor::Nevzat DURMAZ
//   date::06.06.2020
// update::30.05.2024
//version::04.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef BUTTONS_H_
#define BUTTONS_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class BUTTONS {

	bool (*_selectfp)(void);
	bool (*_activatefp)(void);


	mutable int shortOsDelay;

	int __select_count{0}, __activate_count{0};

public:
	volatile bool __select{false};
	volatile bool __activate{false};

	inline BUTTONS() = delete;

	inline explicit BUTTONS( bool (*__butSelect__)(void), bool (*__butActivate__)(void), int waitForOsDelay ) :
	 _selectfp{__butSelect__}, _activatefp{__butActivate__},
	shortOsDelay{ 120 / waitForOsDelay },
	__select_count{0}, __activate_count{0},
	__select{false}, __activate{false} {}

	inline virtual ~BUTTONS() {}

	void catchPressedButton(void);

	inline bool getSelect(void)const 	{ return __select;		}
	inline bool getActivate(void)const 	{ return __activate;	}

};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* BUTTONS_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
