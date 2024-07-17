//program::FLASH driver cpp file
// @uthor::Nevzat DURMAZ
//   date::21.11.2022
// update::
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#ifndef INC_F1FLASH_H_
#define INC_F1FLASH_H_
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#define _WRT_ADR1			 		0x0801FC00 // last block begin
//#define _WRT_ADR2			 		0x0801FC00 // last block begin
//#define _WRT_ADR_END		 		0x0801FFFF // last block ends..
//#define _WRT_ADR1			 		0x0801FE00 // last block begin
//#define _WRT_ADR2			 		0x0801FF00 // last block begin
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
class F1FLASH {

	FLASH_EraseInitTypeDef EraseInitStruct;

public:
	F1FLASH() = default;
	inline virtual ~F1FLASH() {}

	uint32_t flashWriteData(uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords);
	void flashReadData(uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords);

	void write(uint32_t *_buf, unsigned short _num);
	void read(uint32_t *_buf, unsigned short _num);
//	void read2(uint32_t *_buf, unsigned short _num);
};
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
#endif /* INC_F1FLASH_H_ */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//