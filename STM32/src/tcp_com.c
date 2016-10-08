#include <stdint.h>
#include "stm32f1xx_hal.h"
#include <MPU6050.h>
#include <tcp_com.h>

UART_HandleTypeDef *tcp_uart_p;
imu_data_t *tcp_imu_data_p;

void tcp_rx_data_callback(void)
{
	uint8_t tx_buf[2];
	tx_buf[0] = (*tcp_imu_data_p).imu_data_accX >> 8;
	tx_buf[1] = (*tcp_imu_data_p).imu_data_accX;
	HAL_UART_Transmit(tcp_uart_p, tx_buf, 2, 100);
}

void tcp_begin(UART_HandleTypeDef *tcp_uart, imu_data_t *imu_data)
{
	tcp_uart_p = tcp_uart;
	tcp_imu_data_p = imu_data;
}






