
#ifndef MPU6050_H_
#define MPU6050_H_

#include <CONFIG.h>

#ifdef CONF_IMU 

#ifdef __cplusplus
extern "C" {
#endif
	
//MPU slave address for AD0 = gnd
//Slave address must be 8 bits 
//First bit is R/W sign and must leave 0	
#define IMU_SLAVE_ADD 0xD0

	
//MPU6050 internal memory addresses
#define IMU_GYRO_CONFIG 0x1B
#define IMU_ACCEL_CONFIG 0x1C
#define IMU_ACCEL_XOUT_H 0x3B
#define IMU_ACCEL_XOUT_L 0x3C
#define IMU_PWR_MGMT_1 0x6B
#define IMU_WHO_AM_I 0x75
	
//MPU6050 register masks	
#define IMU_MASK_GYRO_CONFIG_RATE 3
#define IMU_MASK_ACCEL_CONFIG_RATE 3

//IMU data structure includes sensor values	
typedef struct{
	double imu_data_accX;
	double imu_data_accY;
	double imu_data_accZ;
	double imu_data_gyroX;
	double imu_data_gyroY;
	double imu_data_gyroZ;
	double imu_data_temp;	
}imu_data_raw_t;

typedef struct{
	double imu_data_Ayaw;
	double imu_data_Apitch;
	double imu_data_Aroll;
	double imu_data_Gyaw;
	double imu_data_Gpitch;
	double imu_data_Groll;
}imu_data_t;
	
//IMU system status
typedef enum{
	IMU_ERROR_COMM, //Sensor not responding
	IMU_ERROR_SUCCES
}imu_error_t;

//IMU gyro range selection
//Default value is 250 deg/sec
//This value can be changed with imu_begin() function as parameter
typedef enum{
	IMU_GYRO_RANGE_250, // +-250 deg/s
	IMU_GYRO_RANGE_500,	// +-500 deg/s
	IMU_GYRO_RANGE_1000,	// +- 1000 deg/s
	IMU_GYRO_RANGE_2000	// +- 2000 deg/s
}imu_gyro_range_t;

//IMU accelerometer range selection
//Default value is 2G
//This value can be changed with imu_begin() function as parameter
typedef enum{
	IMU_ACC_RANGE_2,	//+- 2G
	IMU_ACC_RANGE_4,	//+- 4G
	IMU_ACC_RANGE_8,	//+- 8G
	IMU_ACC_RANGE_16	//+- 16G
}imu_acc_range_t;

//call this function in HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) otherwise malfunctioning may be occur
void imu_rx_cplt_callback(void);

//this function have to be called before reading sensor data for waking
imu_error_t imu_begin(I2C_HandleTypeDef *imu_i2c, uint32_t rate_us, imu_gyro_range_t gyro_range, imu_acc_range_t acc_range);

//function starts reading data, for sensor values refer to address which will be declareted in callback as parameter
imu_error_t imu_read_data(void);

//callback function that indicates end of receiving  
void imu_data_ready_callback(imu_data_raw_t data_raw, imu_data_t data);

#ifdef __cplusplus
}
#endif

#endif	//CONF_IMU
#endif	//MPU6050_H_
