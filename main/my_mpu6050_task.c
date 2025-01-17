#include <string.h>
#include "my_mpu6050_task.h"
#include "my_mqtt_client.h"

static const char *TAG = "my_mpu6050_task";

static const char *g_acc_topic = "topic/accelerometer_data";
static const char *g_gyro_topic = "topic/gyroscope_data";
static const char *g_temp_topic = "topic/temperature_data";

static mpu6050_handle_t mpu6050_dev = NULL;

void mpu6050_init(void)
{
    mpu6050_dev = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);
    if(!mpu6050_dev)
    {
        ESP_LOGI(TAG, "Error initializing the MPU6050\n");
    }
    mpu6050_config(mpu6050_dev, ACCE_FS_4G, GYRO_FS_500DPS);
    mpu6050_wake_up(mpu6050_dev);
}

void mpu6050_read(mpu6050_acce_value_t* acce, mpu6050_gyro_value_t* gyro, mpu6050_temp_value_t* temp)
{
    mpu6050_get_acce(mpu6050_dev, acce);
    mpu6050_get_gyro(mpu6050_dev, gyro);
    mpu6050_get_temp(mpu6050_dev, temp);
}


void mpu6050_monitor_task(void* params)
{
while(1)
    {
    mpu6050_acce_value_t acce = {0};
    mpu6050_gyro_value_t gyro = {0};
    mpu6050_temp_value_t temp = {0};

    char acc_message[64] = { 0 };
    char gyro_message[64] = { 0 };
    char temp_message[10] = { 0 };

    mpu6050_read(&acce, &gyro, &temp);

    sprintf(acc_message, "%.2f %.2f %.2f", acce.acce_x, acce.acce_y, acce.acce_z);
    sprintf(gyro_message, "%.2f %.2f %.2f", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
    sprintf(temp_message, "%.2f", temp.temp);
    
    mqtt_publish_sensor_data(g_acc_topic, acc_message);
    mqtt_publish_sensor_data(g_gyro_topic, gyro_message);
    mqtt_publish_sensor_data(g_temp_topic, temp_message);

    vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}