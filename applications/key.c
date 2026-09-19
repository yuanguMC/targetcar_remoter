#include "key.h"
#include "main.h"
#include "nrf24l01_recive.h"

static volatile uint8_t key_buffer[KEY_COUNT]={0};

//数据数组
uint8_t key_data[KEY_COUNT] = {0};
KeyInfo keyinfo={0};
static int8_t key_index_from_pin(uint16_t gpio_pin)
{
	switch (gpio_pin) {
	case K1_Pin:  return 0;
	case K2_Pin:  return 1;
	case K3_Pin:  return 2;
	case K4_Pin:  return 3;
	case K5_Pin:  return 4;
	case K6_Pin:  return 5;
	case K7_Pin:  return 6;
	case K8_Pin:  return 7;
	case K9_Pin:  return 8;
	// case K10_Pin: return 9;
	// case K11_Pin: return 10;
	default:     return -1;
	}
}

static  GPIO_TypeDef * key_sys_from_index(uint16_t gpio_pin)
{

	switch (gpio_pin) {
	case K1_Pin:  return GPIOA;
	case K2_Pin:  return GPIOA;
	case K3_Pin:  return GPIOB;
	case K4_Pin:  return GPIOB;
	case K5_Pin:  return GPIOB;
	case K6_Pin:  return GPIOB;
	case K7_Pin:  return GPIOA;
	case K8_Pin:  return GPIOA;
	case K9_Pin:  return GPIOB;

	default:     return NULL;
	}
}

uint8_t Key_GPIO_Get(uint16_t gpio_pin)		//由按键编号获得按键电平，数组位置信息
{
	keyinfo.key_id=key_index_from_pin(gpio_pin);

	if (keyinfo.key_id < 0 || keyinfo.key_id >= KEY_COUNT) {
		keyinfo.key_id=0;
		return 0;
	}

	keyinfo.key_sys=key_sys_from_index(gpio_pin);

	if (keyinfo.key_sys == NULL) {
		keyinfo.key_id=0;
		return 0;
	}
	return 1;
}

uint8_t Key_GPIO_Read(uint16_t gpio_pin)
{
	GPIO_PinState state;				//读取电平状态

	if(Key_GPIO_Get(gpio_pin)==0) return 0;

	state=HAL_GPIO_ReadPin(keyinfo.key_sys, keyinfo.key_id);

	return (state == GPIO_PIN_SET) ? 1 : 0;
}


//按键消抖：读取两次电平，间隔 10 ms 后比较，若相同则判定为有效按键
uint8_t key_remove_jitter(uint16_t gpio_pin)
{
    uint32_t start_tick;
    uint8_t first_state;
    uint8_t second_state;

    start_tick = HAL_GetTick();
    first_state = Key_GPIO_Read(gpio_pin);

	//for (uint8_t i = 0; i < 10; i++);

    second_state = Key_GPIO_Read(gpio_pin);

    if (first_state == second_state) {
        return 1U;
    }

    return 0U;
}

void Key_Init(void)
{
	keyinfo.key_id=0;
	keyinfo.key_id=0;
}

uint8_t Key_Get(uint8_t key_code)
{
	if (key_code < 1 || key_code >= KEY_COUNT) {
		return 0;
	}

	return key_buffer[key_code - 1];
}

void Key_Clear(uint8_t key_code)
{
	if (key_code >= 1 && key_code < KEY_COUNT) {
		key_buffer[key_code - 1] = 0;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	int8_t key_index;

	if (GPIO_Pin == IRQ_Pin) {
		nrf24l01_irq_callback();
		return;
	}

	if (key_remove_jitter(GPIO_Pin) == 0U) {
		return;
	}

	key_index = key_index_from_pin(GPIO_Pin);
	if (key_index < 0 || key_index >= KEY_COUNT) {
		return;
	}

	key_buffer[(uint8_t)key_index] ++;

}


//按键数据处理，将数据存入数据数组中
void Key_process(void)
{

    key_data[0] = key_buffer[move]%2;
    key_data[1] = key_buffer[inmove]%2;
    key_data[2] = key_buffer[Clockwise]%2;
    key_data[3] = key_buffer[counterclockwise]%2;


    key_data[4] = key_buffer[SPEED_DOWN];
    key_data[5] = key_buffer[SPEED_UP];
    key_data[6] = key_buffer[WSPEED_UP];
    key_data[7] = key_buffer[WSPEED_DOWN];

    // /* 加减速按键按次处理，避免每次发送都重复消费历史按键次数。 */
    // key_buffer[SPEED_DOWN] = 0;
    // key_buffer[SPEED_UP] = 0;
    // key_buffer[WSPEED_UP] = 0;
    // key_buffer[WSPEED_DOWN] = 0;
}




