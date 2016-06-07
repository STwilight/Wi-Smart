#include <config.h>
#include <logic.h>
#include <uart.h>

#include <user_config.h>
#include <SmingCore/SmingCore.h>

void uartInit()
{
	/* ����� ��������� UART-������ */

	// ����� ���������� ���������� �� UART
	Serial.systemDebugOutput(UART_DEBUG);

	// ��������� UART-������ � ������� �������� ������
	if(UART_ENABLE) {
		Serial.begin(UART_SPEED);
	}
}
