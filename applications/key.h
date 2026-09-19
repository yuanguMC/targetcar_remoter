#ifndef KEY_H
#define KEY_H

#include <stdint.h>

#define KEY_COUNT 9

extern uint8_t key[KEY_COUNT];
extern uint8_t key_data[KEY_COUNT];

void Key_Init(void);
uint8_t Key_Get(uint8_t key_code);
void Key_Clear(uint8_t key_code);
void Key_process(void);

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
