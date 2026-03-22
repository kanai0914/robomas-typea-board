/**
 * @file robomas_address.hpp
 * @author Kanai Ayu
 * @brief
 * robomasのCAN通信において、アドレスを定義するためのヘッダファイル
 * @version 0.1.0
 * @date 2026-03-21
 *
 * @copyright Copyright (c) 2026 Ayu Kanai
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <stdint.h>

// M2006　M3508の制御フレーム（送信）
#define M_SEND_CANID 0x200

// GM6020の制御フレーム（送信）
#define GM_SEND_CANID 0x1FF

// ESCからの受信ID
#define motor_1 0x201
#define motor_2 0x202
#define motor_3 0x203
#define motor_4 0x204
#define motor_5 0x205
#define motor_6 0x206
#define motor_7 0x207
#define motor_8 0x208

struct MotorFeedback {
    uint16_t angle;
    int16_t speed;
    int16_t current;
    uint8_t temp;
    uint8_t spare;
} __attribute__((__packed__));

struct MotorCurrent_can1 {
    int16_t motor1_current = 0;
    int16_t motor2_current = 0;
    int16_t motor3_current = 0;
    int16_t motor4_current = 0;
} __attribute__((__packed__));

struct MotorCurrent_can2 {
    int16_t motor5_current = 0;
    int16_t motor6_current = 0;
    int16_t motor7_current = 0;
    int16_t motor8_current = 0;
} __attribute__((__packed__));

extern MotorCurrent_can1* current_1;
extern MotorCurrent_can2* current_2;