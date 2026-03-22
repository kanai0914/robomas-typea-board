#pragma once

#include <can.h>
#include <stdint.h>

#include "app/robomas_peripheral.hpp"
#include "main.h"

class robomas_can
{
private:
    CAN_RxHeaderTypeDef RxHeader;
    CAN_FilterTypeDef RxFilter;
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t RxData_1[4];
    uint8_t RxData_2[4];

    uint8_t RxData[8];

public:
    MotorFeedback feedback[8];
    void init(CAN_HandleTypeDef* hcan);
    void send_data(CAN_HandleTypeDef* hcan, uint16_t can_id, uint8_t data[8], uint8_t len);
    void can_callback(CAN_HandleTypeDef* hdcan);
    void receive_data(uint16_t can_id, uint8_t data[8]);
    void process_data(MotorFeedback* feedback);
    int16_t get_motor_speed(uint8_t motor_num);
    // 可読性を上げるために独立
    void can1_current(
        int16_t motor1_current,
        int16_t motor2_current,
        int16_t motor3_current,
        int16_t motor4_current
    );

    void can2_current(
        int16_t motor5_current,
        int16_t motor6_current,
        int16_t motor7_current,
        int16_t motor8_current
    );
};
