#include <config.h>
#include <logic.h>
#include <uart.h>

#include <user_config.h>
#include <SmingCore/SmingCore.h>

void uartInit()
{
	/* метод настройки UART-модуля */

	// вывод отладочной информации по UART
	Serial.systemDebugOutput(UART_DEBUG);

	// включение UART-модуля и задание скорости обмена
	if(UART_ENABLE) {
		Serial.begin(UART_SPEED);
	}
}
