/*
 * HP20xSTM32.c
 *
 *  Created on: Apr 28, 2021
 *      Author: tmatko
 */


// https://wiki.seeedstudio.com/Grove-Barometer-High-Accuracy/#features
// https://github.com/Seeed-Studio/Grove_Barometer_HP20x/blob/master/HP20x_dev.cpp


#include "HP20xSTM32.h"





HAL_StatusTypeDef readFromReg(uint8_t regPointer, int outBytesSize, uint8_t* result)
{
	HAL_StatusTypeDef status;

	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(HP20X_I2C_DEV_ID), &regPointer, 1, OSR_ConvertTime);
	if(status != HAL_OK)
		return status;

	status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(HP20X_I2C_DEV_ID), result, outBytesSize, OSR_ConvertTime);
	return status;
}

HAL_StatusTypeDef writeToReg(uint8_t regPointer, uint8_t register_value)
{
    HAL_StatusTypeDef status = HAL_OK;

    uint8_t data[2];  // 2 bytes - from documentation
    data[0] = regPointer;
    data[1] = register_value;

    status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(HP20X_I2C_DEV_ID), data, 2, OSR_ConvertTime);

    return status;
}






HAL_StatusTypeDef enableCompensation()
{
	HAL_StatusTypeDef status;

	uint8_t regPtr = HP20X_RD_REG_MODE | REG_PARA;
	uint8_t regValue[1];
	status = readFromReg(regPtr,1, regValue); // not necessary
	if(status != HAL_OK)
		return status;

	uint8_t reg_prt_write = HP20X_WR_REG_MODE | REG_PARA;
	uint8_t reg_value_write = OK_HP20X_DEV;
	status = writeToReg(reg_prt_write, reg_value_write);

	return status;
}


HAL_StatusTypeDef HP20x_begin()
{
    /* Reset HP20x */
	HAL_StatusTypeDef status;

	uint8_t regPointer = HP20X_SOFT_RST;
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(HP20X_I2C_DEV_ID), &regPointer, 1, OSR_ConvertTime);
	HAL_Delay(100);

	if(status != HAL_OK)
		return status;

	status = enableCompensation();

	return status;
}







HAL_StatusTypeDef HP20x_readTemp(float *dataWrite)
{
	HAL_StatusTypeDef status;

	int bytes = 3;
	uint8_t sensorData[bytes];
	uint8_t regPointer = HP20X_READ_T;

	uint8_t cmd = HP20X_WR_CONVERT_CMD | OSR_CFG;
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(HP20X_I2C_DEV_ID), &cmd, 1, OSR_ConvertTime);
	if(status != HAL_OK)
		return status;

	status = readFromReg(regPointer, bytes, sensorData);
	if(status != HAL_OK)
		return status;

	*dataWrite = ( (sensorData[0] << 16) | (sensorData[1] << 8) | (sensorData[2]));

	return status;
}





HAL_StatusTypeDef HP20x_readPressure(float *dataWrite)
{
	HAL_StatusTypeDef status;

	int bytes = 3;
	uint8_t sensorData[bytes];
	uint8_t regPointer = HP20X_READ_P;

	uint8_t cmd = HP20X_WR_CONVERT_CMD | OSR_CFG;
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(HP20X_I2C_DEV_ID), &cmd, 1, OSR_ConvertTime);
	if(status != HAL_OK)
		return status;

	status = readFromReg(regPointer, bytes, sensorData);
	if(status != HAL_OK)
		return status;

	*dataWrite = ( (sensorData[0] << 16) | (sensorData[1] << 8) | (sensorData[2]));

	return status;
}


HAL_StatusTypeDef HP20x_readAltitude(float *dataWrite)
{
	HAL_StatusTypeDef status;

	int bytes = 3;
	uint8_t sensorData[bytes];
	uint8_t regPointer = HP20X_READ_A;
/*
	uint8_t cmd = HP20X_WR_CONVERT_CMD | OSR_CFG;
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(HP20X_I2C_DEV_ID), &cmd, 1, OSR_ConvertTime);
	if(status != HAL_OK)
		return status;
*/
	status = readFromReg(regPointer, bytes, sensorData);
	if(status != HAL_OK)
		return status;

	*dataWrite = ( (sensorData[0] << 16) | (sensorData[1] << 8) | (sensorData[2]));

	return status;
}
