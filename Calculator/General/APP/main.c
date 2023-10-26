/*
 * main.c
 *
 * Created: 10/20/2023 11:49:57 AM
 *  Author: omnia
 */ 
#define F_CPU 8000000UL
#include "../MCAL/DIO/DIO.h"
#include "../HAL/Calculator/Calculator.h"

int main()
{
	Calc_init();
	
	while(1)
	{
		Calculator(); _delay_ms(250);
	}
		
}
