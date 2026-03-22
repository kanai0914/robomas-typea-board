#include "app/robomas_can.hpp"

#include <cstring>

void robomas_can::init(CAN_HandleTypeDef* hcan)
{
    RxFilter.FilterScale          = CAN_FILTERSCALE_32BIT;  // Idtype FDCAN_STANDAER_ID
    RxFilter.FilterBank           = 0;
    RxFilter.FilterMode           = CAN_FILTERMODE_IDMASK;
    RxFilter.FilterFIFOAssignment = CAN_RX_FIFO0;
    RxFilter.FilterIdHigh         = 0;
    RxFilter.FilterIdLow          = 0;
    RxFilter.FilterMaskIdHigh     = 0;
    RxFilter.FilterMaskIdLow      = 0;
    if (HAL_CAN_ConfigFilter(hcan, &RxFilter) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_Start(hcan) != HAL_OK) {
        Error_Handler();
    }
    // 割り込み有効
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }
}

void robomas_can::send_data(CAN_HandleTypeDef* hcan, uint16_t can_id, uint8_t* data, uint8_t len)
{
    uint32_t mailbox;
    TxHeader.StdId = can_id;
    TxHeader.DLC   = len;
    TxHeader.IDE   = CAN_ID_STD;
    TxHeader.RTR   = CAN_RTR_DATA;
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0);
    if (HAL_CAN_AddTxMessage(hcan, &TxHeader, data, &mailbox) != HAL_OK) {
        Error_Handler();
    }
}

void robomas_can::receive_data(uint16_t can_id, uint8_t data[8])
{
    switch (can_id) {
        case motor_1:
            memcpy(&feedback[0], data, 8);
            process_data(&feedback[0]);
            break;
        case motor_2:
            memcpy(&feedback[1], data, 8);
            process_data(&feedback[1]);
            break;
        case motor_3:
            memcpy(&feedback[2], data, 8);
            process_data(&feedback[2]);
            break;
        case motor_4:
            memcpy(&feedback[3], data, 8);
            process_data(&feedback[3]);
            break;
        case motor_5:
            memcpy(&feedback[4], data, 8);
            process_data(&feedback[4]);
            break;
        case motor_6:
            memcpy(&feedback[5], data, 8);
            process_data(&feedback[5]);
            break;
        case motor_7:
            memcpy(&feedback[6], data, 8);
            process_data(&feedback[6]);
            break;
        case motor_8:
            memcpy(&feedback[7], data, 8);
            process_data(&feedback[7]);
            break;
        default:
            break;
    }
}

void robomas_can::process_data(MotorFeedback* feedback)
{
    // 後ほど実装
}

// bxCANは割り込みフラグが一つしかない。なので引数は一つ
void robomas_can::can_callback(CAN_HandleTypeDef* hcan)
{
    if ((hcan->Instance == hcan1.Instance) || (hcan->Instance == hcan2.Instance)) {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
            Error_Handler();
        }
        receive_data(RxHeader.StdId, RxData);
    }
}

void robomas_can::can1_current(
    int16_t motor1_current, int16_t motor2_current, int16_t motor3_current, int16_t motor4_current
)
{
    current_1->motor1_current = motor1_current;
    current_1->motor2_current = motor2_current;
    current_1->motor3_current = motor3_current;
    current_1->motor4_current = motor4_current;
}

void robomas_can::can2_current(
    int16_t motor5_current, int16_t motor6_current, int16_t motor7_current, int16_t motor8_current
)
{
    current_2->motor5_current = motor5_current;
    current_2->motor6_current = motor6_current;
    current_2->motor7_current = motor7_current;
    current_2->motor8_current = motor8_current;
}