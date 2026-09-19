#include "debug_printf.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>




void debug_printf(UART_HandleTypeDef huart, const char *fmt, ...);



/**
 * @brief 使用串口发送调试信息
 * 
 * @param fmt 格式化字符串，支持printf风格的格式说明符
 * @param ... 可变参数列表，与格式化字符串中的占位符对应
 * 
 * @note 内部使用128字节缓冲区，超长内容会被截断
 * @note UART传输超时时间为1000ms
 */

void debug_printf(UART_HandleTypeDef huart, const char *fmt, ...) {
			char buffer[128];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buffer, sizeof(buffer), fmt, args);
			va_end(args);
                HAL_UART_Transmit(&huart, (uint8_t*)buffer, strlen(buffer), 1000);
            
}

