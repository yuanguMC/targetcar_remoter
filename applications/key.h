#ifndef KEY_H
#define KEY_H

#include <stdint.h>
#include "main.h"

#define KEY_COUNT 9

typedef struct {
    GPIO_TypeDef * key_sys;
    int16_t    key_id;
} KeyInfo;


extern uint8_t key[KEY_COUNT];
extern uint8_t key_data[KEY_COUNT];
extern KeyInfo keyinfo;

void Key_Init(void);
uint8_t Key_Get(uint8_t key_code);
void Key_Clear(uint8_t key_code);
void Key_process(void);
uint8_t Key_GPIO_Get(uint16_t gpio_pin);



enum Key_Code
{
    SPEED_UP,
    SPEED_DOWN,
    WSPEED_UP,
    WSPEED_DOWN,
    Clockwise,
    move,
    counterclockwise,
    inmove,
    KEY_E,
};

#endif
