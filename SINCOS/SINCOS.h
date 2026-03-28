#ifndef _SINCOS_H
#define _SINCOS_H

#include "ti_msp_dl_config.h"

#define Distance 30
#define OneStep 1.8
#define MicorDivide 38
#define CIRCLE_DEFAULT_SEGMENTS 36 //圆细分

void ConvertXYToAngles(float x, float y, float* angle_x, float* angle_y);
uint32_t AngleToSteps(float angle) ;
uint32_t DistanceToSteps(float distance);
void Draw_Line(float x1, float y1, float x2, float y2, float speed);
void Draw_Rectan(float cx, float cy, float width, float height, float angle, float speed);
void Draw_Circle(float cx, float cy, float radius, float angle, float speed, uint16_t segments);

#endif