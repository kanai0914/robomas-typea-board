#include "app/robomas_can.hpp"

#include <cstring>
extern MotorCurrent_can1* current_1;
extern MotorCurrent_can2* current_2;
void robomas_can::init(CAN_HandleTypeDef* hcan)
{
    // クラスメンバではなく、ローカルで毎回クリーンに初期化する方が安全です
    CAN_FilterTypeDef filter = {0};

    filter.FilterScale          = CAN_FILTERSCALE_32BIT;
    filter.FilterMode           = CAN_FILTERMODE_IDMASK;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterIdHigh         = 0;
    filter.FilterIdLow          = 0;
    filter.FilterMaskIdHigh     = 0;
    filter.FilterMaskIdLow      = 0;

    // ★追加1：フィルターを「有効化」する（これがないと受信できません）
    filter.FilterActivation = ENABLE;

    // ★追加2：デュアルCAN（CAN1/CAN2）のためのバンク振り分け
    if (hcan->Instance == CAN1) {
        filter.FilterBank = 0;
    } else {
        filter.FilterBank = 14;
    }
    filter.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK) {
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
    CAN_TxHeaderTypeDef header = {0};  // ローカルで初期化
    uint32_t mailbox;

    header.StdId = can_id;
    header.DLC   = len;
    header.IDE   = CAN_ID_STD;
    header.RTR   = CAN_RTR_DATA;

    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0);

    if (HAL_CAN_AddTxMessage(hcan, &header, data, &mailbox) != HAL_OK) {
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
    feedback->angle   = __builtin_bswap16(feedback->angle);
    feedback->speed   = __builtin_bswap16(feedback->speed);
    feedback->current = __builtin_bswap16(feedback->current);
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
    current_1->motor1_current = __builtin_bswap16(motor1_current);
    current_1->motor2_current = __builtin_bswap16(motor2_current);
    current_1->motor3_current = __builtin_bswap16(motor3_current);
    current_1->motor4_current = __builtin_bswap16(motor4_current);
}

void robomas_can::can2_current(
    int16_t motor5_current, int16_t motor6_current, int16_t motor7_current, int16_t motor8_current
)
{
    current_2->motor5_current = __builtin_bswap16(motor5_current);
    current_2->motor6_current = __builtin_bswap16(motor6_current);
    current_2->motor7_current = __builtin_bswap16(motor7_current);
    current_2->motor8_current = __builtin_bswap16(motor8_current);
}

int16_t robomas_can::get_motor_speed(uint8_t motor_num)
{
    // 範囲外アクセス防止の安全対策
    if (motor_num >= 1 && motor_num <= 8) {
        return feedback[motor_num - 1].speed;
    }
    return 0;  // エラー時や範囲外の時はとりあえず0を返す
}