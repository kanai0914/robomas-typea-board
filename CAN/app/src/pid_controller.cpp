#include "app/pid_controller.hpp"

float pid_controller::pid_controll(MotorFeedback* speed, int16_t target_speed)
{
    prev_error = error;
    error      = target_speed - speed->speed;
    gain.kp    = 2.0f;
    gain.ki    = 0.03f;
    gain.kd    = 0.3f;
    integral   = integral + error;
    P          = gain.kp * error;
    I          = gain.ki * integral;
    D          = gain.kd * (error - prev_error);

    if ((P + I + D) > 3000) {
        integral = integral - error;
        output   = 3000;
    } else if ((P + I + D) < -3000) {
        integral = integral - error;
        output   = -3000;

    } else {
        integral = integral;
        output   = P + I + D;
    }
    return output;
};