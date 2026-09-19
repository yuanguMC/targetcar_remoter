#include "oled_printf.h"
#include <stdarg.h>
#include <stdio.h>

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


