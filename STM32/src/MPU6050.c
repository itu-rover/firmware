#include "stm32f1xx_hal.h"
#include <MPU6050.h>

I2C_HandleTypeDef *imu_i2c_p;

static uint8_t read_register(uint8_t addr);
static void write_register(uint8_t addr, uint8_t data);
static imu_error_t read_register_it(uint8_t addr, uint16_t data_size, uint8_t *data);

static uint8_t read_register(uint8_t addr)
{
	uint8_t regval;
	HAL_I2C_Mem_Read(imu_i2c_p, IMU_SLAVE_ADD, addr, IMU_MEMORY_SIZE, &regval, IMU_MEMORY_SIZE, IMU_TIMEOUT);
	return regval;
}

static void write_register(uint8_t addr, uint8_t data)
{
	HAL_I2C_Mem_Write(imu_i2c_p, IMU_SLAVE_ADD, addr, IMU_MEMORY_SIZE, &data, IMU_MEMORY_SIZE, IMU_TIMEOUT);
}

static imu_error_t read_register_it(uint8_t addr, uint16_t data_size, uint8_t *data)
{
	HAL_I2C_Mem_Read_IT(imu_i2c_p, IMU_SLAVE_ADD, addr, IMU_MEMORY_SIZE, data, data_size);
}	

imu_error_t imu_begin(I2C_HandleTypeDef *imu_i2c, imu_gyro_range_t gyro_range, imu_acc_range_t acc_range)
{
	imu_i2c_p = imu_i2c;
	uint8_t regval;
	
	//Checks communication 
	if(read_register(IMU_WHO_AM_I) != (IMU_SLAVE_ADD >> 1u))
	{
		return IMU_ERROR_COMM;
	}
	
	//set up gyro range
	regval = read_register(IMU_GYRO_CONFIG);
	regval |= gyro_range << IMU_MASK_GYRO_CONFIG_RATE;
	write_register(IMU_GYRO_CONFIG, regval);
	
	//set up accelerometer range
	regval = read_register(IMU_ACCEL_CONFIG);
	regval |= acc_range << IMU_MASK_ACCEL_CONFIG_RATE;
	write_register(IMU_ACCEL_CONFIG, regval);
	
	return IMU_ERROR_SUCCES;
}
	


imu_error_t imu_start_reading(void)
{
	uint8_t data;
	data = read_register(IMU_ACCEL_XOUT_H);

}
