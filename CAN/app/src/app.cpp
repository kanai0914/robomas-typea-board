#include "app/app.hpp"

#include "can.h"
#include "gpio.h"
/*
// モーターフィードバック格納
typedef struct {
    uint16_t angle;
    int16_t speed;
    int16_t current;
    uint8_t temp;
} MotorFeedback;

MotorFeedback motor[8];  // motor[0]〜motor[7]

// フィルター設定 + 起動
static void CAN_Start(CAN_HandleTypeDef* hcan)
{
    CAN_FilterTypeDef filter = {
        .FilterIdHigh         = 0x0000,
        .FilterIdLow          = 0x0000,
        .FilterMaskIdHigh     = 0x0000,
        .FilterMaskIdLow      = 0x0000,
        .FilterFIFOAssignment = CAN_RX_FIFO0,
        .FilterBank           = (hcan->Instance == CAN1),
        .FilterMode           = CAN_FILTERMODE_IDMASK,
        .FilterScale          = CAN_FILTERSCALE_32BIT,
        .FilterActivation     = ENABLE,
        .SlaveStartFilterBank = 14,
    };
    HAL_CAN_ConfigFilter(hcan, &filter);
    HAL_CAN_Start(hcan);
    HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

// 送信（モーター1〜4 or 5〜8）
static void CAN_Send(CAN_HandleTypeDef* hcan, uint32_t id, int16_t cur[4])
{
    CAN_TxHeaderTypeDef header = {
        .StdId = id,
        .IDE   = CAN_ID_STD,
        .RTR   = CAN_RTR_DATA,
        .DLC   = 8,
    };
    uint8_t data[8];
    for (int i = 0; i < 4; i++) {
        data[i * 2]     = (cur[i] >> 8) & 0xFF;
        data[i * 2 + 1] = cur[i] & 0xFF;
    }
    uint32_t mailbox;
    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        HAL_CAN_AddTxMessage(hcan, &header, data, &mailbox);
    }
}

// 受信コールバック
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data);

    if (header.StdId >= 0x201 && header.StdId <= 0x208) {
        uint8_t id        = header.StdId - 0x201;
        motor[id].angle   = (data[0] << 8) | data[1];
        motor[id].speed   = (int16_t)((data[2] << 8) | data[3]);
        motor[id].current = (int16_t)((data[4] << 8) | data[5]);
        motor[id].temp    = data[6];
    }
}
*/
void setup()
{
    /*
    CAN_Start(&hcan1);
    CAN_Start(&hcan2);
    */
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);  // LED_G 常時ON
}

void loop()
{
    /*
    int16_t cur[4] = {1000, 0, 0, 0};  // Motor1だけ動かす
    CAN_Send(&hcan1, 0x200, cur);
    */
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);  // LED_R 送信毎にトグル
    HAL_Delay(1);
}
