#include <stdint.h>
#include "stm32f1xx_hal.h"
#include <MPU6050.h>

//Interrupt activity status
#define THERE_IS_NO_ACTIVE_READING 0x00u
#define THERE_IS_ACTIVE_READING 0x01u

typedef struct
{
	uint8_t data_counter;
	uint8_t data_size;
	uint8_t active_reading;
}imu_it_handler_t;

//public variables
I2C_HandleTypeDef *imu_i2c_p;
imu_data_t data_buffer;
int8_t imu_sensor_values[14];
imu_it_handler_t imu_it_handler;

//static function prototypes
static uint8_t read_register(uint8_t addr);
static void write_register(uint8_t addr, uint8_t data);
static imu_error_t read_register_it(uint8_t addr, uint8_t *data);
static void calculate_data(void);

//static functions
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

//SENSOR DATA
imu_error_t imu_read_data(void)
{
	imu_it_handler.data_counter = 0;
	imu_it_handler.data_size = IMU_DATA_REGISTER_COUNTER;
	imu_it_handler.active_reading = THERE_IS_ACTIVE_READING;
	HAL_I2C_MemRxCpltCallback(imu_i2c_p);
	return IMU_ERROR_SUCCES;
}

static void calculate_data(void)
{
	data_buffer.imu_data_accX = ((double)((imu_sensor_values[0] << 8u) | imu_sensor_values[1]) / 16384);
	data_buffer.imu_data_accY = ((double)((imu_sensor_values[2] << 8u) | imu_sensor_values[3]) / 16384);
	data_buffer.imu_data_accZ = ((double)((imu_sensor_values[4] << 8u) | imu_sensor_values[5]) / 16384);
	data_buffer.imu_data_gyroX = ((double)((imu_sensor_values[8] << 8u) | imu_sensor_values[9]) * 250 / 32768);
	data_buffer.imu_data_gyroY = ((double)((imu_sensor_values[10] << 8u) | imu_sensor_values[11]) * 250 / 32768);
	data_buffer.imu_data_gyroZ = ((double)((imu_sensor_values[12] << 8u) | imu_sensor_values[13]) * 250 / 32768);
	data_buffer.imu_data_temp = (imu_sensor_values[6] << 8u) | imu_sensor_values[7];
	
	imu_data_ready_callback(data_buffer);
}

void imu_rx_cplt_callback(void)
{
	if(imu_it_handler.active_reading == THERE_IS_ACTIVE_READING)
	{
		if(imu_it_handler.data_counter < imu_it_handler.data_size)
		{
			read_register_it(IMU_ACCEL_XOUT_H + imu_it_handler.data_counter, &imu_sensor_values[imu_it_handler.data_counter]);
			imu_it_handler.data_counter++;
		}
		else
		{
			imu_it_handler.active_reading = THERE_IS_NO_ACTIVE_READING;
			calculate_data();
		}	
	}	
}
//END OF SENSOR DATA



//INIT
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
//END OF INIT




