#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void setup();
void loop();
void can_callback(CAN_HandleTypeDef* hcan);
#ifdef __cplusplus
}
#endif