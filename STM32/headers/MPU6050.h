
#ifndef MPU6050_H_
#define MPU6050_H_

#include <CONFIG.h>

#ifdef CONF_IMU 

#ifdef __cplusplus
extern "C" {
#endif
	
#define IMU_SLAVE_ADD 0xD0

	

#define IMU_GYRO_CONFIG 0x1B
#define IMU_ACCEL_CONFIG 0x1C
#define IMU_ACCEL_XOUT_H 0x3B
#define IMU_ACCEL_XOUT_L 0x3C
#define IMU_PWR_MGMT_1 0x6B
#define IMU_WHO_AM_I 0x75
	
#define IMU_MASK_GYRO_CONFIG_RATE 3
#define IMU_MASK_ACCEL_CONFIG_RATE 3
	
typedef struct{
	int16_t imu_data_accX;
	int16_t imu_data_accY;
	int16_t imu_data_accZ;
	int16_t imu_data_gyroX;
	int16_t imu_data_gyroY;
	int16_t imu_data_gyroZ;
	int16_t imu_data_temp;	
}imu_data_t;
	
typedef enum{
	IMU_ERROR_COMM, //Sensor not responding
	IMU_ERROR_SUCCES
}imu_error_t;

typedef enum{
	IMU_GYRO_RANGE_250, // +-250 deg/s
	IMU_GYRO_RANGE_500,	// +-500 deg/s
	IMU_GYRO_RANGE_1000,	// +- 1000 deg/s
	IMU_GYRO_RANGE_2000	// +- 2000 deg/s
}imu_gyro_range_t;

typedef enum{
	IMU_ACC_RANGE_2,	//+- 2G
	IMU_ACC_RANGE_4,	//+- 4G
	IMU_ACC_RANGE_8,	//+- 8G
	IMU_ACC_RANGE_16	//+- 16G
}imu_acc_range_t;

imu_error_t imu_begin(I2C_HandleTypeDef *imu_i2c, imu_gyro_range_t gyro_range, imu_acc_range_t acc_range);
imu_error_t imu_read_data(imu_data_t *data);

	
#ifdef __cplusplus
}
#endif

#endif	//CONF_IMU
#endif	//MPU6050_H_
