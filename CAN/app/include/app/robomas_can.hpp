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
    MotorFeedback feedback[8];
    uint8_t RxData[8];

public:
    void init(CAN_HandleTypeDef* hcan);
    void send_data(CAN_HandleTypeDef* hcan, uint16_t can_id, uint8_t* data, uint8_t len);
    void can_callback(CAN_HandleTypeDef* hdcan, uint32_t RxFifo0ITs);
    void receive_data(uint16_t can_id, uint8_t data[8]);
    void process_data(MotorFeedback* feedback);
};
