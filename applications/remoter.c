/////////////////////////////////////////////遥控器主任务////////////////////////////////////////////
                                   
//2026.9.12 v1.00                   Han          //通过中断读取按键状态,在循环任务处理后发送
//2026.9.13 v1.01                   Han          //添加oled输出目标速度

//2026.9.13 v1.10                   Han          //遥控器可正常发送数据包
//2026.9.20 v1.20                   Han          //由终端改为轮询，保留中断读取接口，按键检测稳定
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "remoter.h"
#include "main.h"
#include <string.h>
#include "cmsis_os.h"


void Remote_Init(void);         //遥控器初始化
void Remote_Send(void);         //发送数据包
void date_pack_process(void);   //打包数据包

/////////////////////全局变量////////////////////////////
uint8_t tx_buffer[NRF24L01_TX_PACKET_WIDTH]={0};


#define MOVE_SPEED_STEP       0.05f
#define ROTATION_SPEED_STEP   5.0f
#define MOVE_SPEED_MAX        1.0f
#define ROTATION_SPEED_MAX    100.0f

static float clamp_float(float value, float min_value, float max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

static void put_int16_le(uint8_t *buffer, int16_t value)
{
    uint16_t unsigned_value = (uint16_t)value;

    buffer[0] = (uint8_t)(unsigned_value & 0xFFU);
    buffer[1] = (uint8_t)(unsigned_value >> 8);
}
void Remote_Init(void){
    nrf24l01_send_init();
    //Key_Init();
}

void date_pack_process(void)
{
    uint16_t move_up_count;
    uint16_t move_down_count;
    uint16_t rotation_up_count;
    uint16_t rotation_down_count;

    Key_process();

    move_up_count = key_data[5];
    move_down_count = key_data[4];
    rotation_up_count = key_data[6];
    rotation_down_count = key_data[7];

    move_speed = ((float)move_up_count - (float)move_down_count) * MOVE_SPEED_STEP;
    rotation_speed = ((float)rotation_up_count - (float)rotation_down_count) * ROTATION_SPEED_STEP;
    move_speed = clamp_float(move_speed, 0.0f, MOVE_SPEED_MAX);
    rotation_speed = clamp_float(rotation_speed, 0.0f, ROTATION_SPEED_MAX);

    memset(tx_buffer, 0, sizeof(tx_buffer));
    tx_buffer[0] = 0xA5;
    tx_buffer[1] = (uint8_t)((key_data[0] << 0) |
                             (key_data[1] << 1) |
                             (key_data[2] << 2) |
                             (key_data[3] << 3));
    put_int16_le(&tx_buffer[2], lr_joystick);
    memcpy(&tx_buffer[4], &move_speed, sizeof(move_speed));
    memcpy(&tx_buffer[8], &rotation_speed, sizeof(rotation_speed));
    memcpy(&tx_buffer[12], &lr_move_sensitivity, sizeof(lr_move_sensitivity));
    tx_buffer[17] = 0x5A;
}

void Remote_Send(void)
{
    memcpy(NRF24L01_TxPacket, tx_buffer, NRF24L01_TX_PACKET_WIDTH);
    SendFlag=NRF24L01_Send();
}

void remote_task(void const * argument)
{
    Remote_Init();
    osDelay(1000);
    while(1)
    {
        date_pack_process();   
        Remote_Send();
        osDelay(10);
    }
}
