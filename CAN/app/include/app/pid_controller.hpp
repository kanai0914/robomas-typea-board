#pragma once
#include "app/robomas_peripheral.hpp"
class pid_controller
{
private:
    struct gain {
        float kp;
        float ki;
        float kd;
    };
    gain gain;
    float integral;    // 誤差の合計
    float prev_error;  // 一個前の誤差を覚える変数
    float error = 0;
    float P;
    float I;
    float D;
    float output;

public:
    float pid_controll(MotorFeedback* speed, int16_t target_speed);
};
