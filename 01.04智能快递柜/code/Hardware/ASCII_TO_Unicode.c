#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ASCII_TO_Unicode.h>

void ASCII_TO_Unicode(char* ASCII, char* Uincode)
{
	int length;
	int i = 0;
	int j = 0;
	memset(Uincode, '\0', sizeof((char*)Uincode));
	length = strlen(ASCII);

	for ( i = 0; i < length; i++)
	{
		Uincode[j++] = '0';
		Uincode[j++] = '0';

		Uincode[j++] = (ASCII[i] / 16) + 0x30;
		Uincode[j++] = (ASCII[i] %16) + 0x30;
	}
	Uincode[j++] = '\r';
	Uincode[j++] = '\n';
}

