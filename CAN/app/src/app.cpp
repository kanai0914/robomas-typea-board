#include "app/app.hpp"

#include "app/pid_controller.hpp"
#include "app/robomas_can.hpp"
#include "can.h"
#include "gpio.h"

robomas_can can;
pid_controller pid;
MotorCurrent_can1 current_1_data;
MotorCurrent_can2 current_2_data;
MotorCurrent_can1* current_1 = &current_1_data;
MotorCurrent_can2* current_2 = &current_2_data;

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    can.can_callback(hcan);
}

void packet_current_data()
{
    can.can1_current(16000, 0, 0, 0);
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
    can.can1_current(pid.pid_controll(&can.feedback[0], 10000), 0, 0, 0);
    can.send_data(&hcan1, M_SEND_CANID, (uint8_t*)current_1, 8);
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);

    HAL_Delay(100);
}