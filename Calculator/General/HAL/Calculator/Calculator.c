/*
 * Calculator.c
 *
 * Created: 10/25/2023 10:31:47 AM
 *  Author: omnia
 */ 
#include "Calculator.h"
int exp_arr[80] = {0} ;
u8 exp_arr_index = 0;
u8 readValu = 0xff;
u8 rawnum = 1;
u8 colmnum = 1;
u8 globalFlag = 0;

void Calc_init(void)
{
	DIO_init();
	LCD_INIT();
	_delay_ms(1000);
	KEYPAD_INIT();
}

void  shiftArr(u8 index)
{
	for(u8 i = index; i<=exp_arr_index; i++)
	{
		exp_arr[i] = exp_arr[i+2];
	}
	exp_arr_index-=2;
	return;
}
void calc_CalculateResult()
{
	//exp_arr_index--;
	for(u8 i=0; i<=exp_arr_index; i++)
	{
		if(exp_arr[i] == '*') {exp_arr[i-1] *= exp_arr[i+1]; shiftArr(i); i=0;}
		else if(exp_arr[i] == '/') {exp_arr[i-1] /= exp_arr[i+1]; shiftArr(i); i=0;}
	}
	for(u8 i=0; i<=exp_arr_index; i++)
	{
		if(exp_arr[i] == '+') {exp_arr[i-1] += exp_arr[i+1]; shiftArr(i); i=0;}
		else if(exp_arr[i] == '-') {exp_arr[i-1] -= exp_arr[i+1]; shiftArr(i); i=0;}
	}
	return;
}
void calc_DisplayResult()
{
	
	LCD_MOVE_CURSOR(rawnum , colmnum);
	LCD_write_char(readValu);
	_delay_ms(1500);
	LCD_instruction(CLEAR_DISPLAY);
	rawnum=1; colmnum=1;
	LCD_MOVE_CURSOR(rawnum , colmnum);
	LCD_SEND_STRING("Result = ");
	LCD_MOVE_CURSOR(1,10);
	//7th digit
	LCD_write_char((exp_arr[exp_arr_index] / 1000000) +48);
	//6th digit
	LCD_write_char(((exp_arr[exp_arr_index] %1000000)/100000) +48);
	//5th digit
	LCD_write_char(((exp_arr[exp_arr_index] %100000)/10000) +48);
	//4th digit
	LCD_write_char(((exp_arr[exp_arr_index]%10000)/1000) +48);
	//3rd digit
	LCD_write_char(((exp_arr[exp_arr_index]%1000)/100) +48);
	//2nd digit
	LCD_write_char(((exp_arr[exp_arr_index]%100)/10) +48);
	//1st digit
	LCD_write_char(((exp_arr[exp_arr_index]%10)/1) +48);
	
	globalFlag = 1;
	return;
}

void calc_toint(u8 digit)
{
	digit -= 48;
	exp_arr[exp_arr_index] *= 10;
	exp_arr[exp_arr_index] += digit;
	return;
}

void calc_CheckNum(u8 num)
{
	if((num!='*') && (num!='/') && (num!='+') && (num!='-'))
	{
		calc_toint(num);
	}
	else
	{
		exp_arr_index++;
		exp_arr[exp_arr_index] = num;
		exp_arr_index++;
	}
	return;
}

void Calculator()
{
	readValu = KEYPAD_READ();
	if(readValu == '=')
		 {
			calc_CalculateResult();
			calc_DisplayResult();
		 }
	while( (readValu != 0xff)  &&  (readValu != '=') )
	{
		if(globalFlag == 1) 
		{LCD_instruction(CLEAR_DISPLAY); 
			globalFlag = 0; 
			exp_arr_index=0;
			for(u8 i=0; i<80; i++) {exp_arr[i]=0;}
		}
		
		calc_CheckNum(readValu);
		LCD_MOVE_CURSOR(rawnum , colmnum);
		LCD_write_char(readValu);
		colmnum++;
		if(colmnum == 21){ rawnum++;  }
		 colmnum%=21;rawnum %= 4;
		readValu = 0xff;
	}
}