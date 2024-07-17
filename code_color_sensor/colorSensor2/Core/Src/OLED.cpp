//program::OLED lcd driver cpp file
// @uthor::Nevzat DURMAZ
//   date::09.08.2016
// update::16.05.2024
//version::01.09
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
extern "C"{
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
}
#include "OLED.h"
#include "OLED_font.h"
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::init(void){
	HAL_Delay(50);
	oledInit();
	clear();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::oledInit(void){

	writeCommand(SSD1306_SET_MULTIPLEX_RATIO); //Set MUX Ratio 	A8h, 3Fh
	writeCommand(0x3F);
	writeCommand(SSD1306_SET_DISPLAY_OFFSET); //Set Display Offset D3h, 00h
	writeCommand(0x00);
	writeCommand(SSD1306_SET_START_LINE); //Set Display Start Line 40h
	writeCommand(0xA1);//Set Segment re-map A0h/A1h
	writeCommand(SSD1306_COM_SCAN_DIR_DEC); //Set COM Output Scan Direction C0h/C8h
	writeCommand(SSD1306_SET_COM_PINS);	//Set COM Pins hardware configuration DAh, 02
	writeCommand(0x12); //0x02
	writeCommand(SSD1306_SET_CONTRAST_CONTROL);	//Set Contrast Control 81h, 7Fh
	writeCommand(0x7F);
	writeCommand(SSD1306_DISPLAY_ALL_ON_RESUME); //Disable Entire Display On A4h
	writeCommand(SSD1306_NORMAL_DISPLAY); //Set Normal Display A6h
	writeCommand(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO); //Set Osc Frequency D5h, 80h
	writeCommand(0x80);
	writeCommand(SSD1306_CHARGE_PUMP); //Enable charge pump regulator 8Dh, 14h
	writeCommand(0x14);
	writeCommand(SSD1306_DISPLAY_ON); //Display On AFh
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::start(void){
	__sda(true);
	__scl(true);
	ol_del();
	__sda(false);
	__scl(false);
	ol_del();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::stop(void){
	__sda(false);
	__scl(true);
	ol_del();
	__sda(true);
	ol_del();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::write_byte(unsigned char data){

	for(short i=0;i<8;++i){

		if( (data<<i) & 0x80) __sda(true);
		else __sda(false);

		__scl(true);
		ol_del();
		__scl(false);
	}

	ol_del();
	__scl(true);
	ol_del();
	__scl(false);
	ol_del();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::writeByte(unsigned char addr, unsigned char data){

//	HAL_I2C_Mem_Write(__pI2C, oled_adrr, addr, 1, &data, 1, 100);
	start();
	write_byte(oled_adrr);
	write_byte(addr);
	write_byte(data);
	stop();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::writeCommand(unsigned char I2C_Command){
	writeByte(0x00, I2C_Command);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::writeData(unsigned char I2C_Data){
	writeByte(0x40, I2C_Data);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::fill(unsigned char _start, unsigned char _end, \
		unsigned char fill_data){

	if(_start > 8) _start = 0;
	if(_end > 8) _end = 8;

	for(short m = _start; m < _end; m++){

		writeCommand(0xB0 + m);  //page0-page1
		writeCommand(0x00);      //low column start address
		writeCommand(0x10);      //high column start address

		for(short n=0; n<128; n++) writeData(fill_data);
	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::oled_on(void){

	writeCommand(0X8D);
	writeCommand(0X14);
	writeCommand(0XAF);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::oled_off(void){

	writeCommand(0X8D);
	writeCommand(0X10);
	writeCommand(0XAE);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::delBox(unsigned char x){

	setpos(x-1, 6);
	for(short i=0; i<3; ++i) writeData(0x00);

	setpos(x-1, 7);
	for(short i=0; i<3; ++i) writeData(0x00);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::putBox(unsigned char x){

	setpos(x-1, 6);
	for(short i=0; i<3; ++i) writeData(0xFF);

	setpos(x-1, 7);
	for(short i=0; i<3; ++i) writeData(0xFF);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::put_pixel_blue(unsigned char x, unsigned char y){

	unsigned char temp,fill;
	if(y>40) y = 40;
	y = 64-y;

	temp = y%8;
	fill = 0x01<<temp;

	setpos(x, y/8);
	writeData(fill);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::del_and_put_pixel_blue(unsigned char x = 0, unsigned char yOld = 0, unsigned char yNew = 0){

	unsigned char temp, fill, yPosOld, yPosNew;

	(yOld > 40) ? yPosOld = 24 : yPosOld = 64-yOld;
	(yNew > 40) ? yPosNew = 24 : yPosNew = 64-yNew;

	temp = yPosNew % 8;
	fill = 0x01<<temp;

	yPosOld /= 8;
	yPosNew /= 8;

//	for(unsigned char i=2; i<8; ++i){
//		setpos(x, i);
//		if(i == yPosNew)writeData(fill);
//		else writeData(0x00);
//	}

	if(yPosOld == yPosNew){
		setpos(x, yPosOld);
		writeData(fill);
	}
	else{
		setpos(x, yPosOld);
		writeData(0x00);

		setpos(x, yPosNew);
		writeData(fill);
	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//void OLED::del_and_put_pixel_blue(unsigned char x, unsigned char y_old, unsigned char y_new){
//
//	unsigned char temp,fill;
//
//	if(y_old>40) y_old = 40;
//	y_old = 64-y_old;
//
//	if(y_new>40) y_new = 40;
//	y_new = 64-y_new;
//
//	y_old /= 8;
//
//	if(y_old == (y_new/8)){
//
//		temp = y_new % 8;
//		fill = 0x01<<temp;
//
//		setpos(x, y_old);
//		writeData(fill);
//	}
//	else{
//
//		setpos(x, y_old);
//		writeData(0x00);
//
//		temp = y_new % 8;
//		fill = 0x01<<temp;
//
//		setpos(x, y_new/8);
//		writeData(fill);
//	}
//}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::putChar(const char c){

	unsigned char temp=c-32;

	if(c == '\n'){
		oled_x=0;
		oled_bigText ? oled_y+=2 : oled_y+=1;
	}
	else if(c == '\r'){
		oled_x = 0;
	}
	else if(c == '\t'){
		oled_bigText ? oled_x+=16 : oled_x+=12;
	}
	else{

		if (oled_bigText) {
			if(oled_x > 119){
				oled_x = 0;
				oled_y += 2;
			}
			if(oled_y > 7) oled_y=0;

			setpos(oled_x, oled_y);
			for(unsigned char i=0;i<8;++i) writeData(F8X16[temp*16+i]);

			setpos(oled_x, oled_y+1);
			for(unsigned char i=0;i<8;++i) writeData(F8X16[temp*16+i+8]);

			oled_y -= 1;
			oled_x += 8;
		}
		else{
			if(oled_x > 121){
				oled_x = 0;
				oled_y++;
			}
			if(oled_y > 8) oled_y=0;

			setpos(oled_x,oled_y);
			for(unsigned char i=0;i<6;++i) writeData(F6x8[temp][i]);

			oled_x += 6;
		}

	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
OLED& OLED::operator<<(const char *pCdata){

	const char *p = pCdata;

	if(nSizeSetted){
		while(nSize--){
			putChar(*p);
			p++;
		}
		nSizeSetted = false;
	}
	else{
		while(*p != '\0'){
			putChar(*p);
			p++;
		}
	}

	return *this;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
OLED& OLED::operator<<(int Number){

	int ulTmp, ulTmp2;
    char cBuf[16]{'\0'};
    short ii{0}, zero{nSize};


    if(oled_numberformat == 16){
    	zero-=2;
    	putChar('0'); putChar('x');
    	ulTmp = Number;
    }

    if(Number == 0){

    	if (nSizeSetted) {
    		while(zero--) putChar('0');
    		nSizeSetted = false;
    	}
    	else putChar('0');

    	return *this;
    }
    else if(Number < 0){
    	zero-=1;
    	putChar('-');
        ulTmp = -Number;
    }
    else ulTmp = Number;

    if (nSizeSetted){

        for(short i = 0; i < zero; ++i){

        	ulTmp2 = ulTmp % oled_numberformat;
        	if(ulTmp2 > 9){
        		cBuf[i] = (char)(ulTmp2 - 10 + 65); //chr(ulTmp2 - 10 + ord('A'))
            }
            else{
            	cBuf[i] = (char)(ulTmp2 + 48); //chr(ulTmp2 + ord('0'))
            }

            ulTmp /= oled_numberformat;
        }

    	ii = zero - 1;
    	nSizeSetted = false;
    }
    else{

        for(short i = 0; i < 16; ++i){

        	ulTmp2 = ulTmp % oled_numberformat;
        	if(ulTmp2 > 9){
        		cBuf[i] = (char)(ulTmp2 - 10 + 65); //chr(ulTmp2 - 10 + ord('A'))
            }
            else{
            	cBuf[i] = (char)(ulTmp2 + 48); //chr(ulTmp2 + ord('0'))
            }

            ulTmp /= oled_numberformat;
            if(ulTmp == 0) {
            	ii = i;
            	break;
            }
        }

    }

    for(short i = ii; i > -1; --i){
    	putChar(cBuf[i]);
    }

    return *this;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
OLED& OLED::operator<<(float Number){

	int ulTmp;
	char cBuf[16]{'\0'};
	short j{0}, after_dot{nSize};

	if (after_dot > 4) after_dot = 4;

	if(Number < 0.0){
		Number = -Number;
		putChar('-');
	}
	else if(!nSizeSetted) putChar(' ');

	if(Number == 0.0){
		if(nSizeSetted) {
			nSizeSetted = false;
			putChar('0');
			putChar('.');
			putChar('0');
		}
		else {
			putChar('0');putChar('0');
			putChar('.');
			putChar('0');putChar('0');
		}
		return *this;
	}


	if(nSizeSetted) {
		for(short i=0; i<after_dot+1; ++i) Number = Number*10;
		ulTmp=(int)Number;

		if((ulTmp % 10) > 4) ulTmp+=10;
		ulTmp/=10;

		for(short i=0; i<after_dot; ++i){
			cBuf[j] = (char)((ulTmp%10) + 48);
			ulTmp /= 10;
			j++;
		}

		cBuf[j]='.';
		j++;

		if(ulTmp == 0){
			cBuf[j] = '0';
			j++;
		}

		while (ulTmp > 0){
			cBuf[j] = (char)((ulTmp%10) + 48);
			ulTmp /= 10;
			j++;
		}

		nSizeSetted = false;
	}
	else{
		ulTmp=(int)(Number * 1000.0);

		if((ulTmp % 10) > 4) ulTmp += 10;
		ulTmp /= 10;

		cBuf[j] = (char)((ulTmp%10) + 48);
		ulTmp /= 10;
		j++;

		cBuf[j] = (char)((ulTmp%10) + 48);
		ulTmp /= 10;
		j++;

		cBuf[j]='.';
		j++;

		if(ulTmp == 0){
			cBuf[j] = '0';
			j++;
			cBuf[j] = '0';
			j++;
		}
		else{
			if (ulTmp > 0){
				cBuf[j] = (char)((ulTmp%10) + 48);
				ulTmp /= 10;
				j++;
			}
			if (ulTmp > 0){
				cBuf[j] = (char)((ulTmp%10) + 48);
				ulTmp /= 10;
				j++;
			}
			else{
				cBuf[j] = '0';
				j++;
			}
		}

	}

	while(j != 0){
		j -= 1;
		putChar(cBuf[j]);
	}

	return *this;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void OLED::draw_bmp(unsigned char x0, unsigned char y0, unsigned char x1,\
		unsigned char y1, const unsigned char BMP[]){

	unsigned int j=0;

	for(unsigned char y = (y0/8); y < (y1/8); ++y){

		setpos(x0,y);
		for(unsigned char x = x0; x < x1; ++x) writeData(BMP[j++]);
	}
}
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
