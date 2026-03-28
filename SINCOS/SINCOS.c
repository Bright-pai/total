#include "BUJIN.h"
#include "math.h"
#include "SINCOS.h"
#include "arm_acle.h"
#include "UART.h"

//Yaw = Num * OneStep / MicorDivide
//Hudu = Yaw * pai /180
//Num = Yaw * MicroDivide /Onestep

// 将XY坐标转换为两个电机的角度(度)
void ConvertXYToAngles(float x, float y, float* angle_x, float* angle_y) {
    *angle_x = atan2f(Distance, x) * (180.0f / M_PI);
    *angle_y = atan2f(Distance, y) * (180.0f / M_PI);
}

// 将角度转换为步进电机步数
uint32_t AngleToSteps(float angle) {
    return (uint32_t)(angle * MicorDivide /OneStep);
}

uint32_t DistanceToSteps(float distance) {
    float angle_rad = atanf(distance / Distance);
    float angle_deg = angle_rad * (180.0f / M_PI);
    return AngleToSteps(angle_deg);
}

void Draw_Line(float x1, float y1, float x2, float y2, float speed) 
{
    float XDDIR;
    float YDDIR;
    float Xspeed;
    float Yspeed;
    float start_angle_x, start_angle_y;
    float end_angle_x, end_angle_y;
    ConvertXYToAngles(x1, y1, &start_angle_x, &start_angle_y);
    ConvertXYToAngles(x2, y2, &end_angle_x, &end_angle_y);
    float delta_angle_x = end_angle_x - start_angle_x;
    float delta_angle_y = end_angle_y - start_angle_y;

//    Serial_Printf("%.2f,%.2f",delta_angle_x,delta_angle_y);

    if (delta_angle_x<0) {
        XDDIR = -1;
    }
    else {
        XDDIR = 1;
    }
    if (delta_angle_y<0) {
        YDDIR = -1;
    }
    else {
        YDDIR = 1;
    }
    uint32_t total_steps_x = AngleToSteps(fabsf(delta_angle_x));
    uint32_t total_steps_y = AngleToSteps(fabsf(delta_angle_y));
 
 //   Serial_Printf("%d,%d",total_steps_x,total_steps_y);
    
    if (total_steps_x == 0 && total_steps_y == 0) {
    // 起点和终点相同，无需运动
    Stop_X_Bujin();  // 假设存在停止电机的函数
    Stop_Y_Bujin();
    }
    else {
    if (total_steps_x>=total_steps_y) {
        Xspeed = XDDIR * speed;
        Yspeed = YDDIR * ((float)total_steps_y/(float)total_steps_x) * speed;
    }
    else {
        Yspeed = YDDIR * speed;
        Xspeed = XDDIR * ((float)total_steps_x/(float)total_steps_y) * speed;
    }

    Set_X_Bujin_Speed(Xspeed);
    Set_Y_Bujin_Speed(Yspeed);
    Serial_Printf("%.2f,%.2f",Xspeed,Yspeed);
    Bujin_Set_X_Num(total_steps_x);
    Bujin_Set_Y_Num(total_steps_y);

    Start_X_Bujin();
    Start_Y_Bujin();
   // while (X_NUM != 0 || Y_NUM != 0);
    }
}

void Draw_Rectan(float cx, float cy, float width, float height, float angle, float speed) 
{
    float angle_rad = angle * M_PI / 180.0f;
    float cos_theta = cosf(angle_rad);  // 旋转矩阵的cos值
    float sin_theta = sinf(angle_rad);  // 旋转矩阵的sin值

    float half_w = width / 2.0f;
    float half_h = height / 2.0f;
    float x_offset[4] = { half_w,  -half_w,  -half_w,   half_w };  // 右、左、左、右
    float y_offset[4] = { half_h,   half_h,   -half_h,  -half_h };  // 上、上、下、下

    float x[4], y[4];
    for (int i = 0; i < 4; i++) {
        // 旋转矩阵公式：
        // x' = x0*cosθ - y0*sinθ  （x0,y0为相对中心的偏移量）
        // y' = x0*sinθ + y0*cosθ
        // 加上中心点坐标得到绝对坐标
        x[i] = cx + (x_offset[i] * cos_theta - y_offset[i] * sin_theta);
        y[i] = cy + (x_offset[i] * sin_theta + y_offset[i] * cos_theta);
    }
    Draw_Line(cx, cy, x[0],  y[0], speed);
    while (X_NUM != 0 || Y_NUM != 0);
    //右上
    Draw_Line(x[0], y[0], x[1], y[1], speed);
    while (X_NUM != 0 || Y_NUM != 0);

    //左上
    Draw_Line(x[1], y[1], x[2], y[2], speed);
    while (X_NUM != 0 || Y_NUM != 0);

    //左下
    Draw_Line(x[2], y[2], x[3], y[3], speed);
    while (X_NUM != 0 || Y_NUM != 0);

    //右下
    Draw_Line(x[3], y[3], x[0], y[0], speed);
    while (X_NUM != 0 || Y_NUM != 0);

    Draw_Line(x[0], y[0],cx, cy, speed);
    while (X_NUM != 0 || Y_NUM != 0);
}

void Draw_Circle(float cx, float cy, float radius, float angle, float speed, uint16_t segments)
{
    if (radius <= 0.0f || segments < 4) {
        return;
    }
    if (segments == 0) {
        segments = CIRCLE_DEFAULT_SEGMENTS;
    }
    float total_angle_rad = 2.0f * M_PI;  // 整个圆的弧度（360度）
    float step_angle_rad = total_angle_rad / segments;  // 每段的角度

    float rotate_rad = angle * M_PI / 180.0f;

    float x_prev, y_prev;  // 上一个点的坐标
    float x_curr, y_curr;  // 当前点的坐标

    float theta = 0.0f + rotate_rad;  // 初始角度（加上整体旋转角度）
    x_prev = cx + radius * cosf(theta);
    y_prev = cy + radius * sinf(theta);

    float x_ini = x_prev;
    float y_ini = y_prev;

   Draw_Line(cx, cy, x_prev, y_prev, speed);
   while (X_NUM != 0 || Y_NUM != 0)
   {
    
   }

    for (uint16_t i = 1; i <= segments; i++) {
        // 计算当前点的角度（累加每段角度）
        theta = (float)i * step_angle_rad + rotate_rad;

        x_curr = cx + radius * cosf(theta);
        y_curr = cy + radius * sinf(theta);

        // 绘制从上个点到当前点的短直线
        Draw_Line(x_prev, y_prev, x_curr, y_curr, speed);

        // 阻塞等待当前线段绘制完成（确保连续）
        while (X_NUM != 0 || Y_NUM != 0);

        // 更新“上一个点”为当前点，准备下一段
        x_prev = x_curr;
        y_prev = y_curr;
    }
   Draw_Line(x_ini, y_ini, cx, cy, speed);
   while (X_NUM != 0 || Y_NUM != 0);

}