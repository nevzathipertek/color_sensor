//program::OLED lcd driver header file
// @uthor::Nevzat DURMAZ
//   date::09.08.2016
// update::16.05.2024
//version::01.09
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef OLED_H_
#define OLED_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//#include <type_traits>
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define SSD1306_COMMAND			0x00
#define SSD1306_DATA			0xC0
#define SSD1306_DATA_CONTINUE	0x40
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define RST_NOT_IN_USE			255
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
// SSD1306 Command set
// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL					0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME					0xA4
#define SSD1306_DISPLAY_ALL_ON							0xA5
#define SSD1306_NORMAL_DISPLAY							0xA6
#define SSD1306_INVERT_DISPLAY							0xA7
#define SSD1306_DISPLAY_OFF								0xAE
#define SSD1306_DISPLAY_ON								0xAF
#define SSD1306_NOP										0xE3
// Scrolling Commands
#define SSD1306_HORIZONTAL_SCROLL_RIGHT					0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT					0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT	0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT		0x2A
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3
// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN						0x00
#define SSD1306_SET_HIGHER_COLUMN						0x10
#define SSD1306_MEMORY_ADDR_MODE						0x20
#define SSD1306_SET_COLUMN_ADDR							0x21
#define SSD1306_SET_PAGE_ADDR							0x22
// Hardware Configuration Commands
#define SSD1306_SET_START_LINE							0x40
#define SSD1306_SET_SEGMENT_REMAP						0xA0
#define SSD1306_SET_MULTIPLEX_RATIO						0xA8
#define SSD1306_COM_SCAN_DIR_INC						0xC0
#define SSD1306_COM_SCAN_DIR_DEC						0xC8
#define SSD1306_SET_DISPLAY_OFFSET						0xD3
#define SSD1306_SET_COM_PINS							0xDA
#define SSD1306_CHARGE_PUMP								0x8D
// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO				0xD5
#define SSD1306_SET_PRECHARGE_PERIOD					0xD9
#define SSD1306_SET_VCOM_DESELECT						0xDB
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define I2C_DMA_DATALENGHT 		1024
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class OLED{
	const unsigned char oled_adrr{0x78};
	unsigned char oled_x{0},  oled_y{0};

	bool oled_bigText{true}, nSizeSetted{false};
	short oled_numberformat{10}, nSize{0};
//--------------------------------------------------------------------//
//	I2C_HandleTypeDef* __pI2C{nullptr};
//	unsigned char __i2c_data[8][128][2]{0x40}, __i2cDMAStep{1};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
	void (*__sda)(bool);
	void (*__scl)(bool);

	void oledInit(void);
	void start(void);
	void stop(void);
	void write_byte(unsigned char);
//--------------------------------------------------------------------//
	inline void ol_del(void){ for(unsigned short i=0; i<4; ++i); }
//--------------------------------------------------------------------//
	void writeByte(unsigned char, unsigned char);
	void writeCommand(unsigned char);
	void writeData(unsigned char);
//--------------------------------------------------------------------//
	void fill(unsigned char, unsigned char,	unsigned char);

	inline void setpos(unsigned char x, unsigned char y){
		if( x < 128 ) oled_x = x;
		if( y < 8 ) oled_y = y;
		writeCommand( 0xB0 + oled_y );
		writeCommand( ( (oled_x&0xF0) >>4) | 0x10 );
		writeCommand( (oled_x&0x0F) | 0x01);
	}
//--------------------------------------------------------------------//
public:

	inline OLED() = delete;

	//	explicit inline OLED( I2C_HandleTypeDef* __pI2C__,
	explicit inline OLED( void (*_SDA_)(bool), void (*_SCL_)(bool),
			bool bigText = true, short numberFormat = 10) :
			oled_x{0}, oled_y{0},
			oled_bigText{bigText}, nSizeSetted{false},
			oled_numberformat{numberFormat}, nSize{0},
			__sda{_SDA_}, __scl{_SCL_} {}

	inline virtual ~OLED() {}
//--------------------------------------------------------------------//
	void init(void);
	void showScreenPicture(void);
//--------------------------------------------------------------------//

	// clear all
	inline void clear(void) { fill(0, 8, 0x00); }

	// for clear or fill all line...
	inline void operator!=(bool __fill){
		__fill ? fill(oled_y, oled_bigText ? oled_y+2 : oled_y+1, 0xFF) :
		fill(oled_y, oled_bigText ? oled_y+2 : oled_y+1, 0x00);
	}

	// for clear all or just blue side
	inline void operator==(bool _all){
		if(_all) fill(0, 8, 0x00);
		else fill(2, 8, 0x00);
	}

	// for set text size
	inline void setTextSize(bool bigText){ oled_bigText = bigText; 					}
	inline OLED& operator[](bool bigText){ oled_bigText = bigText; return *this; 	}

	// for set position
	inline void setPosition(unsigned char x, unsigned char y){ setpos(x, y); }
	inline OLED& operator()(unsigned char x, unsigned char y){ setpos(x, y); return *this; }

	// for set n_size zero or after dot
	inline void setNSize(short n_size){
		if((n_size > 0) && (n_size < 32)) { nSize = n_size; nSizeSetted = true; }
	}
	inline OLED& operator>>(short n_size){
		if((n_size > 0) && (n_size < 32)) { nSize = n_size; nSizeSetted = true; }
		return *this;
	}

//	template <typename T>
//	auto getDataForOut(T t){
//		if constexpr (std::is_pointer<T>::value) 	{ 	return *t;	}
//		else 										{	return t;	}
//	}

	// for outputs
	OLED& operator<<(const char *pCdata);
	OLED& operator<<(const int Number);
	OLED& operator<<(const float Number);

	inline OLED& operator<<(const double Number)			{ return operator<<( static_cast<float>(Number) ); 	}
	inline OLED& operator<<(const long int Number)			{ return operator<<( static_cast<int>(Number) ); 	}
	inline OLED& operator<<(const long unsigned int Number)	{ return operator<<( static_cast<int>(Number) ); 	}
	inline OLED& operator<<(const unsigned int Number)		{ return operator<<( static_cast<int>(Number) ); 	}
	inline OLED& operator<<(const short Number)				{ return operator<<( static_cast<int>(Number) ); 	}
	inline OLED& operator<<(const unsigned short Number)	{ return operator<<( static_cast<int>(Number) ); 	}
	inline OLED& operator<<(const unsigned char Number)		{ return operator<<( static_cast<int>(Number) ); 	}
	inline OLED& operator<<(const bool _sta)				{ _sta ? putChar('T') : putChar('F'); return *this; }
	inline OLED& operator<<(const char _ch)					{ putChar(_ch); return *this;						}
//--------------------------------------------------------------------//
	void oled_on(void);
	void oled_off(void);
//--------------------------------------------------------------------//
	void delBox(unsigned char x);
	void putBox(unsigned char x);
	void put_pixel_blue(unsigned char, unsigned char);
	void del_and_put_pixel_blue(unsigned char, unsigned char, unsigned char);
//--------------------------------------------------------------------//
	void putChar(const char c);
//--------------------------------------------------------------------//
	inline void setNumberFormat(char numform) { oled_numberformat=numform; }
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
	void draw_bmp(unsigned char, unsigned char, unsigned char,\
			unsigned char, const unsigned char*);
//--------------------------------------------------------------------//
	inline void oledClearError(void){ oledInit(); }
//--------------------------------------------------------------------//
};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//template <typename U, typename T, typename ...Args>
//inline void print(U& ol, const T &t, Args ...params){
//	if constexpr (sizeof...(params) == 0) {
//		ol << t;
//	}
//	else{
//		ol << t << ", ";
//		print(ol, params...);
//	}
//
//}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
	template <typename U, typename T>
	inline void print(U& ol, const T &t){ ol << t; }
//--------------------------------------------------------------------//
	// completing with the generic "print" function with oled...
	template <typename U, typename T, typename ...Args>
	inline void print(U& ol, const T &t, Args ...params){
		ol << t;
		print(ol, params...);
	}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* OLED_H_ */
//--------------------------------------------------------------------//
