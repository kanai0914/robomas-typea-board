#include "app/app.hpp"

#include "app/robomas_can.hpp"
#include "can.h"
#include "gpio.h"

robomas_can can;
// app.cpp
MotorCurrent_can1 current_1_data;
MotorCurrent_can2 current_2_data;
MotorCurrent_can1* current_1 = &current_1_data;
MotorCurrent_can2* current_2 = &current_2_data;

void can_callback(CAN_HandleTypeDef* hcan)
{
    can.can_callback(hcan);
}

void packet_current_data()
{
    can.can1_current(12000, 0, 0, 0);
    can.can2_current(0, 0, 0, 0);
}

void setup()
{
    can.init(&hcan1);
    can.init(&hcan2);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);  // LED_G 常時ON
}

void loop()
{
    // 電流値を更新
    packet_current_data();

    // CAN1の制御フレーム送信（ID: 0x200）
    can.send_data(&hcan1, M_SEND_CANID, (uint8_t*)&current_1_data, sizeof(MotorCurrent_can1));

    // CAN2の制御フレーム送信（ID: 0x200）
    can.send_data(&hcan2, M_SEND_CANID, (uint8_t*)&current_2_data, sizeof(MotorCurrent_can2));

    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);  // LED_R 送信毎にトグル
    HAL_Delay(1);
}