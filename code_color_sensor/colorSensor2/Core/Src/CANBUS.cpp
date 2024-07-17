//program::CAN bus protocol with SN65HVD230 driver cpp file
// @uthor::Nevzat DURMAZ
//   date::02.01.2021
// update::18.09.2023
//version::01.03
//--------------------------------------------------------------------//
extern "C"{
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_can.h"
#include "stm32f1xx_hal_cortex.h"
//--------------------------------------------------------------------//
#include "301/CO_SDOclient.h"
#include "301/CO_Emergency.h"
}
#include "CANBUS.h"
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
namespace hiper {
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
CANopenNodeSTM32 canOpen;
//--------------------------------------------------------------------//
//CAN_HandleTypeDef* CANBUS::pCanUsed;
//TIM_HandleTypeDef* CANBUS::pTimUsed;
//void (*CANBUS::pCanInit)(void);
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void CANBUS::init(uint8_t _nodeID_, uint8_t _baudKHz_){

	canOpen.CANHandle = pCanUsed;
	canOpen.HWInitFunction = pCanInit;
	canOpen.timerHandle = pTimUsed;
	canOpen.desiredNodeID = _nodeID_;
	canOpen.baudrate = _baudKHz_;
	canopen_app_init(&canOpen);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
uint8_t CANBUS::getNodeIDactive(void)const	{ return canOpen.activeNodeID;							}
uint8_t CANBUS::getNodeIDdesired(void)const	{ return canOpen.desiredNodeID;							}
bool CANBUS::getRedLed(void)const			{ return static_cast<bool>(canOpen.outStatusLEDRed);	}
bool CANBUS::getGreenLed(void)const			{ return static_cast<bool>(canOpen.outStatusLEDGreen);	}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void CANBUS::sendRequest(void){
	CO_TPDOsendRequest(&canOpen.canOpenStack->TPDO[0]);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
CO_SDO_abortCode_t global_readSDO(CO_SDOclient_t *SDO_C, uint8_t nodeId,
                            uint16_t index, uint8_t subIndex,
                            uint8_t *buf, size_t bufSize, size_t *readSize)
{
    CO_SDO_return_t SDO_ret;

    // setup client (this can be skipped, if remote device don't change)
    SDO_ret = CO_SDOclient_setup(SDO_C,
                                 CO_CAN_ID_SDO_CLI + nodeId,
                                 CO_CAN_ID_SDO_SRV + nodeId,
                                 nodeId);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
        return CO_SDO_AB_GENERAL;
    }

    // initiate upload
    SDO_ret = CO_SDOclientUploadInitiate(SDO_C, index, subIndex, 1000, false);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
        return CO_SDO_AB_GENERAL;
    }

    // upload data
    do {
        uint32_t timeDifference_us = 10000;
        CO_SDO_abortCode_t abortCode = CO_SDO_AB_NONE;

        SDO_ret = CO_SDOclientUpload(SDO_C,
                                     timeDifference_us,
                                     false,
                                     &abortCode,
                                     NULL, NULL, NULL);
        if (SDO_ret < 0) {
            return abortCode;
        }

        CANBUS::DelayUs(timeDifference_us);
//        HAL_Delay(1);
//        sleep_us(timeDifference_us);
    } while(SDO_ret > 0);

    // copy data to the user buffer (for long data function must be called
    // several times inside the loop)
    *readSize = CO_SDOclientUploadBufRead(SDO_C, buf, bufSize);

    return CO_SDO_AB_NONE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
CO_SDO_abortCode_t global_writeSDO(CO_SDOclient_t *SDO_C, uint8_t nodeId,
                             uint16_t index, uint8_t subIndex,
                             uint8_t *data, size_t dataSize)
{
    CO_SDO_return_t SDO_ret;
    bool_t bufferPartial = false;

    // setup client (this can be skipped, if remote device is the same)
    SDO_ret = CO_SDOclient_setup(SDO_C,
                                 CO_CAN_ID_SDO_CLI + nodeId,
                                 CO_CAN_ID_SDO_SRV + nodeId,
                                 nodeId);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
    	return CO_SDO_AB_UNSUPPORTED_ACCESS;
//        return -1;
    }

    // initiate download
    SDO_ret = CO_SDOclientDownloadInitiate(SDO_C, index, subIndex,
                                           dataSize, 1000, false);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
    	return CO_SDO_AB_UNSUPPORTED_ACCESS;
//        return -1;
    }

    // fill data
    size_t nWritten = CO_SDOclientDownloadBufWrite(SDO_C, data, dataSize);
    if (nWritten < dataSize) {
        bufferPartial = true;
        // If SDO Fifo buffer is too small, data can be refilled in the loop.
    }

    //download data
    do {
        uint32_t timeDifference_us = 10000;
        CO_SDO_abortCode_t abortCode = CO_SDO_AB_NONE;

        SDO_ret = CO_SDOclientDownload(SDO_C,
                                       timeDifference_us,
                                       false,
                                       bufferPartial,
                                       &abortCode,
                                       NULL, NULL);
        if (SDO_ret < 0) {
            return abortCode;
        }

        CANBUS::DelayUs(timeDifference_us);
//        HAL_Delay(1);
//        sleep_us(timeDifference_us);
    } while(SDO_ret > 0);

    return CO_SDO_AB_NONE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//  usage samples...
//	OD_set_u32(OD_find(OD, 0x6404), 0x00, valueOfInf, false);
//	OD_set_u16(OD_find(OD, 0x6051), 0x00, CounterOfCommunication, false);
//	OD_RAM.x6050_statusOfSensor = 16;
//	CO_TPDOsendRequest(&canOpen.canOpenStack->TPDO[0]);
//	CO_TPDOsendRequest(&canOpen.canOpenStack->TPDO[1]);
//	write_SDO(canOpen.canOpenStack->SDOclient, CANtransmitID3, 0x6005, 0, TxData1, 4);
//	read_SDO(canOpen1.canOpenStack->SDOclient, CANtransmitID2, 0x6041, 0, &RxData2[3], 8, (size_t *)4);
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
} /* namespace hiper */
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
