#include "key.h"
#include "main.h"
#include "nrf24l01_recive.h"
#include "cmsis_os.h"

uint8_t key_data[KEY_COUNT] = {0};
KeyInfo keyinfo={0};
static volatile uint8_t key_state[KEY_COUNT] = {0};
static volatile uint8_t key_count[4] = {0};

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState pressed_state;
} KeyPinConfig;

static const KeyPinConfig key_pin_config[KEY_COUNT] = {
    {GPIOA, K1_Pin, GPIO_PIN_RESET},
    {GPIOA, K2_Pin, GPIO_PIN_RESET},
    {GPIOB, K3_Pin, GPIO_PIN_RESET},
    {GPIOB, K4_Pin, GPIO_PIN_RESET},
    {GPIOB, K5_Pin, GPIO_PIN_SET},
    {GPIOB, K6_Pin, GPIO_PIN_SET},
    {GPIOA, K7_Pin, GPIO_PIN_SET},
    {GPIOA, K8_Pin, GPIO_PIN_SET},
    {GPIOB, K9_Pin, GPIO_PIN_RESET}
};

static int8_t key_index_from_pin(uint16_t gpio_pin)
{
    uint8_t index;

    for (index = 0; index < KEY_COUNT; index++) {
        if (key_pin_config[index].pin == gpio_pin) {
            return (int8_t)index;
        }
	}

    return -1;
}

static const KeyPinConfig *key_config_from_pin(uint16_t gpio_pin)
{
    int8_t key_index = key_index_from_pin(gpio_pin);

    if (key_index < 0) {
        return NULL;
    }

    return &key_pin_config[(uint8_t)key_index];
}


uint8_t Key_GPIO_Read(uint16_t gpio_pin)
{
    const KeyPinConfig *config = key_config_from_pin(gpio_pin);

    if (config == NULL) {
        return 0;
    }

    return (HAL_GPIO_ReadPin(config->port, config->pin) == config->pressed_state) ? 1U : 0U;
}


void Key_Init(void)
{
	keyinfo.key_id=0;
}


void readkey (void const * argument)
{
    uint8_t counter_state[KEY_COUNT] = {0};
    uint8_t last_state[KEY_COUNT] = {0};

    while (1) {

        uint8_t index;
        uint8_t key_state_first;
        uint8_t key_state_second;

        for (index = 0; index < KEY_COUNT; index++) {
            key_state_first =  Key_GPIO_Read(key_pin_config[index].pin);
            osDelay(5);
            key_state_second = Key_GPIO_Read(key_pin_config[index].pin);

            if (key_state_first == key_state_second) {
                counter_state[index] = key_state_first;
            } else {
                counter_state[index] = last_state[index];
            }
        }


        for (index = 0; index < KEY_COUNT ; index++) {
            if (counter_state[index] != last_state[index]) {
                if (counter_state[index] == GPIO_PIN_SET) {
                    if (index < 4U) {
                        key_count[index]++;
                    }
                    key_state[index] = GPIO_PIN_RESET;
                } else {
                    key_state[index] = GPIO_PIN_SET;
                }
            }

            last_state[index] = counter_state[index];
        }

        osDelay(5);
    }
}




//按键数据处理，将数据存入数据数组中
void Key_process(void)
{

    key_data[0] = (uint8_t)key_state[move];
    key_data[1] = (uint8_t)key_state[inmove];
    key_data[2] = (uint8_t)key_state[Clockwise];
    key_data[3] = (uint8_t)key_state[counterclockwise];

    key_data[4] = key_count[SPEED_DOWN];
    key_data[5] = key_count[SPEED_UP];
    key_data[6] = key_count[WSPEED_UP];
    key_data[7] = key_count[WSPEED_DOWN];
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//int8_t key_index;
    Key_Init();

	if (GPIO_Pin == IRQ_Pin) {
		nrf24l01_irq_callback();
		return;
	}


	// key_index = key_index_from_pin(GPIO_Pin);
	// if (key_index < 0 || key_index >= KEY_COUNT) {
	// 	return;
	// }

	// key_buffer[(uint8_t)key_index] ++;

}
