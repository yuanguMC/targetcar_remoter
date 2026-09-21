
#include "cmsis_os.h"
#include <stdarg.h>
#include <stdio.h>
#include "oled.h"
#include "remoter.h"

void oled_task(void const * argument);

void oled_printf(uint8_t x, uint8_t y, const char *format, ...)
{
    char buffer[128];
    va_list args;

    va_start(args, format);
    (void)vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    OLED_NewFrame();
    OLED_PrintString(x, y, buffer, &font16x16_1, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
}

void oled_task(void const * argument)
{
    char speed_text[32];
    char rotation_text[32];

    osDelay(100);
    OLED_Init();
    for(;;)
    {
        (void)snprintf(speed_text, sizeof(speed_text), "当前速度:%.2f", move_speed);
        (void)snprintf(rotation_text, sizeof(rotation_text), "当前旋转:%.2f", rotation_speed);
        OLED_NewFrame();
        OLED_PrintString(0, 0, speed_text, &font16x16_1, OLED_COLOR_NORMAL);
        OLED_PrintString(0, 24, rotation_text, &font16x16_1, OLED_COLOR_NORMAL);
        OLED_ShowFrame();
        osDelay(1000);
    }
}

