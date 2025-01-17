#pragma once
#include "esp_log.h"
#include "mqtt_client.h"
#include "mpu6050.h"

#define I2C_MASTER_SCL_IO    22 // SCL pin
#define I2C_MASTER_SDA_IO    21 // SDA pin
#define I2C_MASTER_FREQ_HZ   260000
#define I2C_MASTER_NUM       I2C_NUM_0
#define ESP_INTR_FLAG_DEFAULT 0

void mpu6050_init(void);

void mpu6050_read(mpu6050_acce_value_t* acce, mpu6050_gyro_value_t* gyro, mpu6050_temp_value_t* temp);

void mpu6050_monitor_task(void* params);