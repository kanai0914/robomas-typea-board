#include "app/pid_controller.hpp"

void pid_controller::set_gain(float p_gain, float i_gain, float d_gain)
{
    gain.kp = p_gain;
    gain.ki = i_gain;
    gain.kd = d_gain;
}

float pid_controller::pid_controll(MotorFeedback* speed, int16_t target_speed)
{
    prev_error = error;
    error      = target_speed - speed->speed;
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