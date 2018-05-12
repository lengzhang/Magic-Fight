/*
 * MagicFight.cpp
 *
 * Created: 5/18/2016 7:05:28 PM
 * Author : Leng
 */ 

#include <avr/io.h>
#include <avr/io.h>
#include "timer.h"
#include "tasks.h"



int main(void)
{
	
	
	DDRA = 0xFF;	PORTA = 0x01;
	DDRB = 0x00;	PORTB = 0xFF;
	DDRD = 0x00;	PORTD = 0xFF;
	
	unsigned long int Game_Timer_Period = 2;
	unsigned long int Game_Display_Period = 1;
	
	// define the 5 LCD Data pins: SCE, RST, DC, DATA, CLK
	lcd.lcd_init(&PORTC, PC7, &PORTC, PC6, &PORTC, PC5, &PORTC, PC4, &PORTC, PC3);
	
	task game_tasks[2];
	int num_game_task = 2;
	
	game_tasks[0].state = -1;
	game_tasks[0].period = Game_Timer_Period;
	game_tasks[0].elapsedTime = Game_Timer_Period;
	game_tasks[0].TickFct = Game_Timer_Tick;
	
	game_tasks[1].state = -1;
	game_tasks[1].period = Game_Display_Period;
	game_tasks[1].elapsedTime = Game_Display_Period;
	game_tasks[1].TickFct = Game_Display_Tick;
	
	TimerSet(50);
	TimerOn();
    while (1) 
    {
		/*
		tmpA = (~PINA) & 0x01;
		tmpB = (~PINB) & 0x1F;
		tmpD = (~PIND) & 0x1F;
		
		POWER_CONTROL();
		if (power == 1)
		{
			RUN_GAME_TASKS();
		}
		*/
		for (unsigned short int i = 0; i < num_game_task; i++)
		{
			// Task is ready to tick
			if ( game_tasks[i].elapsedTime == game_tasks[i].period ) {
				// Setting next state for task
				game_tasks[i].state = game_tasks[i].TickFct(game_tasks[i].state);
				// Reset the elapsed time for next tick.
				game_tasks[i].elapsedTime = 0;
			}
			game_tasks[i].elapsedTime++;
		}
		
		while (!TimerFlag);
		TimerFlag = 0;
    }
	return 0;
}

