#include "app/app.hpp"

#include <cmath>

struct ImuData {
    volatile float ax, ay, az;
    volatile float gx, gy, gz;
    volatile float roll, pitch, yaw;
    volatile float mx, my, mz;  // 地磁気データ追加！
};
ImuData imu;

void ist8310_write_reg(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(IST8310_I2C, IST8310_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

uint8_t ist8310_read_reg(uint8_t reg)
{
    uint8_t data = 0;
    HAL_I2C_Mem_Read(IST8310_I2C, IST8310_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    return data;
}

void ist8310_init()
{
    IST8310_RST_LOW();
    HAL_Delay(50);
    IST8310_RST_HIGH();
    HAL_Delay(50);

    ist8310_write_reg(0x0B, 0x08);
    HAL_Delay(10);
    ist8310_write_reg(0x41, 0x09);
    HAL_Delay(10);
    ist8310_write_reg(0x42, 0xC0);
    HAL_Delay(10);
    ist8310_write_reg(0x0A, 0x0B);
    HAL_Delay(10);
}

void ist8310_update()
{
    uint8_t buf[6];
    HAL_I2C_Mem_Read(IST8310_I2C, IST8310_ADDR, IST8310_DATAXL, I2C_MEMADD_SIZE_8BIT, buf, 6, 10);

    imu.mx = (int16_t)(buf[1] << 8 | buf[0]);
    imu.my = (int16_t)(buf[3] << 8 | buf[2]);
    imu.mz = (int16_t)(buf[5] << 8 | buf[4]);
}
const float alpha = 0.98f;

void mpu6500_write_reg(uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = {reg, data};
    MPU6500_CS_LOW();
    HAL_SPI_Transmit(&hspi5, buf, 2, 10);
    MPU6500_CS_HIGH();
}

uint8_t mpu6500_read_reg(uint8_t reg)
{
    uint8_t tx = reg | MPU6500_READ_FLAG;
    uint8_t rx = 0;
    MPU6500_CS_LOW();
    HAL_SPI_Transmit(&hspi5, &tx, 1, 10);
    HAL_SPI_Receive(&hspi5, &rx, 1, 10);
    MPU6500_CS_HIGH();
    return rx;
}

void mpu6500_read_regs(uint8_t reg, uint8_t* buf, uint8_t len)
{
    uint8_t tx = reg | MPU6500_READ_FLAG;
    MPU6500_CS_LOW();
    HAL_SPI_Transmit(&hspi5, &tx, 1, 10);
    HAL_SPI_Receive(&hspi5, buf, len, 10);
    MPU6500_CS_HIGH();
}

float g_offset_x = 0, g_offset_y = 0, g_offset_z = 0;

void calibrate_gyro()
{
    float sum_x = 0, sum_y = 0, sum_z = 0;
    const int samples = 500;
    for (int i = 0; i < samples; i++) {
        uint8_t buf[6];
        mpu6500_read_regs(MPU6500_GYRO_XOUT_H, buf, 6);
        sum_x += (int16_t)(buf[0] << 8 | buf[1]) / 32.8f;
        sum_y += (int16_t)(buf[2] << 8 | buf[3]) / 32.8f;
        sum_z += (int16_t)(buf[4] << 8 | buf[5]) / 32.8f;
        HAL_Delay(1);
    }
    g_offset_x = sum_x / samples;
    g_offset_y = sum_y / samples;
    g_offset_z = sum_z / samples;
}

float accel_roll_offset  = 0;
float accel_pitch_offset = 0;

void calibrate_accel()
{
    float sum_roll = 0, sum_pitch = 0;
    const int samples = 500;
    for (int i = 0; i < samples; i++) {
        uint8_t buf[6];
        mpu6500_read_regs(MPU6500_ACCEL_XOUT_H, buf, 6);
        float ax = (int16_t)(buf[0] << 8 | buf[1]) / 16384.0f;
        float ay = (int16_t)(buf[2] << 8 | buf[3]) / 16384.0f;
        float az = (int16_t)(buf[4] << 8 | buf[5]) / 16384.0f;
        sum_roll += atan2f(ay, az) * 57.2957795f;
        sum_pitch += atan2f(-ax, sqrtf(ay * ay + az * az)) * 57.2957795f;
        HAL_Delay(1);
    }
    accel_roll_offset  = sum_roll / samples;
    accel_pitch_offset = sum_pitch / samples;
}

static uint32_t last_tick = 0;

void imu_update()
{
    // 正確なdtをHAL_GetTickで計測
    uint32_t now = HAL_GetTick();
    float dt     = (now - last_tick) / 1000.0f;  // msをsに変換
    last_tick    = now;

    // dtが異常な値のときはスキップ
    if (dt <= 0.0f || dt > 1.0f) return;

    uint8_t buf[14];
    mpu6500_read_regs(MPU6500_ACCEL_XOUT_H, buf, 14);

    int16_t ax_raw = (int16_t)(buf[0] << 8 | buf[1]);
    int16_t ay_raw = (int16_t)(buf[2] << 8 | buf[3]);
    int16_t az_raw = (int16_t)(buf[4] << 8 | buf[5]);
    int16_t gx_raw = (int16_t)(buf[8] << 8 | buf[9]);
    int16_t gy_raw = (int16_t)(buf[10] << 8 | buf[11]);
    int16_t gz_raw = (int16_t)(buf[12] << 8 | buf[13]);

    imu.ax = ax_raw / 16384.0f;
    imu.ay = ay_raw / 16384.0f;
    imu.az = az_raw / 16384.0f;

    imu.gx = (gx_raw / 32.8f) - g_offset_x;
    imu.gy = (gy_raw / 32.8f) - g_offset_y;
    imu.gz = (gz_raw / 32.8f) - g_offset_z;

    float accel_roll  = atan2f(imu.ay, imu.az) * 57.2957795f - accel_roll_offset;
    float accel_pitch = atan2f(-imu.ax, sqrtf(imu.ay * imu.ay + imu.az * imu.az)) * 57.2957795f -
                        accel_pitch_offset;

    imu.roll  = alpha * (imu.roll + imu.gx * dt) + (1.0f - alpha) * accel_roll;
    imu.pitch = alpha * (imu.pitch + imu.gy * dt) + (1.0f - alpha) * accel_pitch;
    imu.yaw += imu.gz * dt;
}

void setup()
{
    mpu6500_write_reg(0x6B, 0x80);
    HAL_Delay(100);
    mpu6500_write_reg(0x6B, 0x00);
    HAL_Delay(100);
    mpu6500_write_reg(0x1C, 0x00);
    mpu6500_write_reg(0x1B, 0x10);

    uint8_t id = mpu6500_read_reg(MPU6500_WHO_AM_I);

    last_tick = HAL_GetTick();  // 初期化！
    calibrate_gyro();
    calibrate_accel();                        // ← 追加
    ist8310_init();                           // ← 追加
    uint8_t ist_id = ist8310_read_reg(0x00);  // WHO_AM_I → 0x10が返れば通信成功
}

void loop()
{
    imu_update();
    ist8310_update();  // ← 追加
    HAL_Delay(10);
}