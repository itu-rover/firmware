
#ifndef MPU6050_H_
#define MPU6050_H_

#include <CONFIG.h>

#ifdef CONF_IMU 

#ifdef __cplusplus
extern "C" {
#endif
	
typedef enum{
	IMU_STATE_
}imu_state_t;
	
typedef enum{
	IMU_ERROR_COM,
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


imu_error_t imu_self_test(void);
imu_error_t imu_begin();
	
#ifdef __cplusplus
}
#endif

#endif	//CONF_IMU
#endif	//MPU6050_H_
