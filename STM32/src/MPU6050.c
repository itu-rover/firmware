#include "stm32f1xx_hal.h"
#include <MPU6050.h>

I2C_HandleTypeDef *imu_i2c_p;
imu_data_t *data_buffer;
uint8_t data_counter;
uint8_t data_size;
uint8_t imu_sensor_values[14];

static uint8_t read_register(uint8_t addr);
static void write_register(uint8_t addr, uint8_t data);
static imu_error_t read_register_it(uint8_t addr, uint8_t *data);
static void calculate_data(void);


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

static imu_error_t read_register_it(uint8_t addr, uint8_t *data)
{
	HAL_I2C_Mem_Read_IT(imu_i2c_p, IMU_SLAVE_ADD, addr, IMU_MEMORY_SIZE, data, IMU_DATA_SIZE);
	return IMU_ERROR_SUCCES;
}	

static void calculate_data()
{
	data_buffer->imu_data_accX = (imu_sensor_values[0] << 8u) | imu_sensor_values[1];
	data_buffer->imu_data_accY = (imu_sensor_values[2] << 8u) | imu_sensor_values[3];
	data_buffer->imu_data_accZ = (imu_sensor_values[4] << 8u) | imu_sensor_values[5];
	data_buffer->imu_data_gyroX = (imu_sensor_values[8] << 8u) | imu_sensor_values[9];
	data_buffer->imu_data_gyroY = (imu_sensor_values[10] << 8u) | imu_sensor_values[11];
	data_buffer->imu_data_gyroZ = (imu_sensor_values[12] << 8u) | imu_sensor_values[13];
	data_buffer->imu_data_temp = (imu_sensor_values[6] << 8u) | imu_sensor_values[7];
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
	
	//power on the imu
	write_register(IMU_PWR_MGMT_1, 0x00u);
	
	return IMU_ERROR_SUCCES;
}
	


imu_error_t imu_read_data(imu_data_t *data)
{
	data_buffer = data;
	data_counter = 0;
	data_size = 14;
	HAL_I2C_MemRxCpltCallback(imu_i2c_p);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(data_counter < data_size)
	{
		read_register_it(IMU_ACCEL_XOUT_H + data_counter, &imu_sensor_values[data_counter]);
		data_counter++;
	}
	else
	{
		calculate_data();
	}
}
