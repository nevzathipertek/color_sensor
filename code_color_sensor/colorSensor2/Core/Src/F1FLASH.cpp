//program::FLASH driver cpp file
// @uthor::Nevzat DURMAZ
//   date::21.11.2022
// update::
//version::01.01
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
extern "C"{
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_flash.h"
}
#include "F1FLASH.h"
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
uint32_t F1FLASH::flashWriteData (uint32_t StartPageAddress, uint32_t *Data,
		uint16_t numberofwords) {

	uint32_t PAGEError;
	uint16_t writtenwords = 0;

	  /* Unlock the Flash to enable the flash control register access *************/
	   HAL_FLASH_Unlock();

	   /* Erase the user Flash area*/
	  uint32_t StartPage = StartPageAddress;
	  uint32_t EndPageAdress = StartPageAddress + numberofwords*4;
	  uint32_t EndPage = EndPageAdress;

	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	   EraseInitStruct.PageAddress = StartPage;
	   EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;

	   if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	   {
	     /*Error occurred while page erase.*/
		  return HAL_FLASH_GetError ();
	   }

	   /* Program the user Flash area word by word*/

	   while (writtenwords < numberofwords){
		   if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data[writtenwords]) == HAL_OK){
			   StartPageAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
			   writtenwords++;
		   }
		   else{/* Error occurred while writing data in Flash memory*/
			   return HAL_FLASH_GetError ();
		   }
	   }

	   /* Lock the Flash to disable the flash control register access (recommended
	      to protect the FLASH memory against possible unwanted operation) *********/
	   HAL_FLASH_Lock();

	   return 0;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void F1FLASH::flashReadData (uint32_t StartPageAddress, uint32_t *RxBuf,
		uint16_t numberofwords) {

	do{
		*RxBuf = *(__IO uint32_t *)StartPageAddress;
		StartPageAddress += 4;
		RxBuf++;
	} while (--numberofwords);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void F1FLASH::write(uint32_t *_buf, unsigned short _num){

	flashWriteData(_WRT_ADR1, _buf, _num);

	HAL_Delay(50);
	HAL_NVIC_SystemReset();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void F1FLASH::read(uint32_t *_buf, unsigned short _num){
	flashReadData(_WRT_ADR1, _buf, _num);
}
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
