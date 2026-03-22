#include "app/app.hpp"

#include "app/pid_controller.hpp"
#include "app/robomas_can.hpp"
#include "can.h"
#include "gpio.h"

MotorCurrent_can1 current_1_data;
MotorCurrent_can2 current_2_data;
MotorCurrent_can1* current_1 = &current_1_data;
MotorCurrent_can2* current_2 = &current_2_data;

robomas_can can;
pid_controller pid_1;
pid_controller pid_2;

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    can.can_callback(hcan);
}

void setup()
{
    can.init(&hcan1);
    can.init(&hcan2);
    pid_1.set_gain(2.0f, 0.03f, 0.3f);
}
void loop()
{
    can.can1_current(pid_1.pid_controll(&can.feedback[0], 10000), 0, 0, 0);
    can.send_data(&hcan1, M_SEND_CANID, (uint8_t*)current_1, 8);
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);

    HAL_Delay(100);
}