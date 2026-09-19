#ifndef NRF24L01_RECEIVE_H
#define NRF24L01_RECEIVE_H
#include "struct_typedef.h"
#include "nrf24l01.h"


extern uint8_t cmd_move             ;  // bit 0 : 前进
extern uint8_t cmd_stop_move        ;  // bit 1 : 后退
extern uint8_t cmd_clockwise        ;  // bit 2 : 顺时针旋转
extern uint8_t cmd_counterclockwise ;  // bit 3 : 逆时针旋转

extern int16_t lr_joystick          ;  // short : 左右移动拨杆值

extern float   move_speed           ;  // float 0 : 移动速度
extern float   rotation_speed       ;  // float 1 : 旋转速度
extern float   lr_move_sensitivity  ;  // float 2 : 左右移动灵敏度
extern uint8_t SendFlag							;	 //发送成功标志位

void nrf24l01_send_init(void);
void nrf24l01_irq_callback(void);


#endif
