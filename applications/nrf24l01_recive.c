#include "nrf24l01_recive.h"
#include <string.h>

// ==================== 全局变量 ====================
uint8_t cmd_move             = 0;  // bit 0 : 前进
uint8_t cmd_stop_move        = 0;  // bit 1 : 停止移动
uint8_t cmd_clockwise        = 0;  // bit 2 : 顺时针旋转
uint8_t cmd_counterclockwise = 0;  // bit 3 : 逆时针旋转

int16_t lr_joystick     = 0;       // short : 左右移动拨杆值

float   move_speed      = 0.0f;    // float 0 : 移动速度
float   rotation_speed  = 0.0f;    // float 1 : 旋转速度
float   lr_move_sensitivity  = 0.0f;    // float 2 : 左右移动灵敏度

uint8_t SendFlag;

// 发送缓冲
//static uint8_t tx_buffer[NRF24L01_RX_PACKET_WIDTH];
static volatile uint8_t tx_pending = 0;
//static uint32_t last_valid_tx_tick = 0;

void nrf24l01_send_init(void) { 
    NRF24L01_Init();
		SendFlag=0;
}

void nrf24l01_irq_callback(void)
{
    tx_pending = 1U;
}
