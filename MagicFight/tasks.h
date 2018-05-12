
#ifndef TASKS_H
#define TASKS_H
#include "5110/5110.h"
#include "screens.h"
#include "scheduler.h"

LCD_5110 lcd;
unsigned char screen [504];
unsigned char tmpA, tmpB, tmpD;

unsigned char game_time;
unsigned char screen_update = 0;

unsigned char player_1_hp, player_2_hp;
unsigned char player_1_towers [5];
unsigned char player_2_towers [5];

enum Game_Timer_SM {Count};
int Game_Timer_Tick (int state)
{
	//static unsigned char i = 0;
	switch (state)
	{
		case Count:
			if (game_time > 0)
			{
				game_time--;
				//screen_update = 1;
			}
			break;
		default:
			game_time = 99;
			player_1_hp = 100;
			player_2_hp = 100;
			for (int i=0; i<5; i++)
			{
				player_1_towers[i] = 1;
				player_2_towers[i] = 1;
			}
			//screen_update = 1;
			state = Count;
			break;
	}
	return state;
}

unsigned char play_1_xy [2];
unsigned char play_2_xy [2];
unsigned char play_1_status;
unsigned char play_2_status;

unsigned char character [4] [2] [6] = {
	{{0x56, 0x3A, 0x56, 0x10, 0x0C, 0x00}, {0x56, 0x3A, 0x56, 0x10, 0x10, 0x10}},
	{{0x57, 0x3A, 0x54, 0x14, 0x0C, 0x04}, {0x57, 0x3A, 0x54, 0x10, 0x10, 0x38}},
	{{0x54, 0x3A, 0x54, 0x18, 0x0C, 0x18}, {0x54, 0x3A, 0x54, 0x10, 0x38, 0x10}},
	{{0x56, 0x3A, 0x54, 0x14, 0x08, 0x04}, {0x56, 0x3A, 0x54, 0x10, 0x10, 0x28}}
};

enum Character_Move_SM {Move};
int Character_Move_Tick (int state)
{
	switch (state)
	{
		case Move:
			//Player 1
			if ((tmpB & 0x01) == 0x00)
			{
				if ((tmpB & 0x06) == 0x02)	//UP
				{
					if (play_1_xy[0] > 1)
					{
						play_1_xy[0]--;
						screen_update = 1;
					}
				}
				else if ((tmpB & 0x06) == 0x04)	//DOWN
				{
					if (play_1_xy[0] < 5)
					{
						play_1_xy[0]++;
						screen_update = 1;
					}
				}
				
				if ((tmpB & 0x18) == 0x08)	//LEFT
				{
					if (play_1_xy[1] > 3)
					{
						play_1_xy[1]--;
						screen_update = 1;
					}
				}
				else if ((tmpB & 0x18) == 0x10)	//RIGHT
				{
					if (play_1_xy[1] < 31)
					{
						play_1_xy[1]++;
						screen_update = 1;
					}
				}
			}
			//Player 2
			if ((tmpD & 0x01) == 0x00)
			{
				if ((tmpD & 0x06) == 0x02)	//UP
				{
					if (play_2_xy[0] > 1)
					{
						play_2_xy[0]--;
						screen_update = 1;
					}
				}
				else if ((tmpD & 0x06) == 0x04)	//DOWN
				{
					if (play_2_xy[0] < 5)
					{
						play_2_xy[0]++;
						screen_update = 1;
					}
				}
			
				if ((tmpD & 0x18) == 0x08)	//LEFT
				{
					if (play_2_xy[1] > 52)
					{
						play_2_xy[1]--;
						screen_update = 1;
					}
				}
				else if ((tmpD & 0x18) == 0x10)	//RIGHT
				{
					if (play_2_xy[1] < 80)
					{
						play_2_xy[1]++;
						screen_update = 1;
					}
				}
			}
			break;
		default:
			play_1_xy[0] = 5;
			play_1_xy[1] = 3;
			play_2_xy[0] = 5;
			play_2_xy[1] = 80;
			state = Move;
			break;
	}
	return state;
}

enum Character_Action_SM {Action};
int Character_Action_Tick (int state)
{
	static unsigned char play_1_holding;
	static unsigned char play_2_holding;
	switch (state)
	{
		case Action:
			if (tmpB == 0x01)
			{
				play_1_holding++;
				play_1_status = 1;
				screen_update = 1;
			}
			else
			{
				play_1_holding = 0;
				play_1_status = 0;
				screen_update = 1;
			}
			if (tmpB == 0x01)
			{
				play_2_holding++;
				play_2_status = 1;
				screen_update = 1;
			}
			else
			{
				play_2_holding = 0;
				play_2_status = 0;
				screen_update = 1;
			}
			break;
		default:
			play_1_status = 0;
			play_2_status = 0;
			play_1_holding = 0;
			play_2_holding = 0;
			state = Action;
			break;
	}
	return state;
}

//Display Game Time
void Display_Game_Time ();
//Display Plays' HP
void Display_Players_HP ();
//Display Towers
void Display_Towers ();
//Display Character
void Display_Character ();

//Display
enum Game_Display_SM {Display};
int Game_Display_Tick (int state)
{
	switch (state)
	{
		case Display:
		//if (screen_update)
		//{
			for (int i=0; i<504; i++)
			{
				screen[i] = game_frame[i];
			}
			//Display Game Time
			Display_Game_Time();
			//Display Players' hp
			//Display_Players_HP();
			//Display Towers
			//Display_Towers ();
			//Display Character
			//Display_Character ();
			
			lcd.lcd_goto_xy(0,0);
			for (int i=0; i<504; i++)
			{
				lcd.lcd_col(screen[i]);
			}
			
			screen_update = 0;
		//}
		break;
		default:
		state = Display;
		break;
	}
	return state;
}

//Display Game Time
void Display_Game_Time ()
{
	unsigned char tmpTime = game_time;
	switch (tmpTime % 10)
	{
		case 0:
			screen[43] = 0xBE; screen[44] = 0xA2; screen[45] = 0xBE;
			break;
		case 1:
			screen[43] = 0x80; screen[44] = 0x80; screen[45] = 0xBE;
			break;
		case 2:
			screen[43] = 0xBA; screen[44] = 0xAA; screen[45] = 0xAE;
			break;
		case 3:
			screen[43] = 0xAA; screen[44] = 0xAA; screen[45] = 0xBE;
			break;
		case 4:
			screen[43] = 0x8E; screen[44] = 0x88; screen[45] = 0xBE;
			break;
		case 5:
			screen[43] = 0xAE; screen[44] = 0xAA; screen[45] = 0xBA;
			break;
		case 6:
			screen[43] = 0xBE; screen[44] = 0xAA; screen[45] = 0xBA;
			break;
		case 7:
			screen[43] = 0x82; screen[44] = 0x82; screen[45] = 0xBE;
			break;
		case 8:
			screen[43] = 0xBE; screen[44] = 0xAA; screen[45] = 0xBE;
			break;
		case 9:
			screen[43] = 0xAE; screen[44] = 0xAA; screen[45] = 0xBE;
			break;
		default:
			break;
	}
	tmpTime = tmpTime / 10;
	switch (tmpTime % 10)
	{
		case 0:
			screen[38] = 0xBE; screen[39] = 0xA2; screen[40] = 0xBE;
			break;
		case 1:
			screen[38] = 0x80; screen[39] = 0x80; screen[40] = 0xBE;
			break;
		case 2:
			screen[38] = 0xBA; screen[39] = 0xAA; screen[40] = 0xAE;
			break;
		case 3:
			screen[38] = 0xAA; screen[39] = 0xAA; screen[40] = 0xBE;
			break;
		case 4:
			screen[38] = 0x8E; screen[39] = 0x88; screen[40] = 0xBE;
			break;
		case 5:
			screen[38] = 0xAE; screen[39] = 0xAA; screen[40] = 0xBA;
			break;
		case 6:
			screen[38] = 0xBE; screen[39] = 0xAA; screen[40] = 0xBA;
			break;
		case 7:
			screen[38] = 0x82; screen[39] = 0x82; screen[40] = 0xBE;
			break;
		case 8:
			screen[38] = 0xBE; screen[39] = 0xAA; screen[40] = 0xBE;
			break;
		case 9:
			screen[38] = 0xAE; screen[39] = 0xAA; screen[40] = 0xBE;
			break;
		default:
			break;
	}
}

//Display Plays' HP
void Display_Players_HP ()
{
	for (int i=0; i<(33*player_1_hp/100); i++)
		screen[2+i] = 0xBD;
	for (int i=0; i<(33*player_2_hp/100); i++)
		screen[81-i] = 0xBD;
}

//Display Towers
void Display_Towers ()
{
	for (int i=0; i<5; i++)
	{
		if (player_1_towers[i])
		{
			screen[84*(i+1)] = 0x7E;
			screen[84*(i+1)+1] = 0x3C;
			screen[84*(i+1)+2] = 0x18;
		}
		if (player_2_towers[i])
		{
			screen[84*(i+2)-1] = 0x7E;
			screen[84*(i+2)-2] = 0x3C;
			screen[84*(i+2)-3] = 0x18;
		}
	}
}

//Display Character
void Display_Character ()
{
	for (int i=0; i<6; i++)
	{
		screen[84*play_1_xy[0]+play_1_xy[1]+i] = character[0][play_1_status][i];
		screen[84*play_2_xy[0]+play_2_xy[1]-i] = character[1][play_2_status][i];
	}
}

unsigned char power = 0;

task game_tasks[4];
int num_game_task = 4;

void GAME_INIT()
{
	game_tasks[0].state = -1;
	game_tasks[0].period = 1;
	game_tasks[0].elapsedTime = 1;
	game_tasks[0].TickFct = Game_Timer_Tick;
	
	game_tasks[1].state = -1;
	game_tasks[1].period = 1;
	game_tasks[1].elapsedTime = 1;
	game_tasks[1].TickFct = Character_Move_Tick;
	
	game_tasks[2].state = -1;
	game_tasks[2].period = 1;
	game_tasks[2].elapsedTime = 1;
	game_tasks[2].TickFct = Character_Action_Tick;
	
	game_tasks[3].state = -1;
	game_tasks[3].period = 1;
	game_tasks[3].elapsedTime = 1;
	game_tasks[3].TickFct = Game_Display_Tick;
}

void RUN_GAME_TASKS()
{
	for (int i = 0; i < num_game_task; i++)
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
}

void POWER_CONTROL()
{
	if (tmpA != 0x00 && power == 0)
	{
		PORTA = 0x03;
		// define the 5 LCD Data pins: SCE, RST, DC, DATA, CLK
		lcd.lcd_init(&PORTC, PC7, &PORTC, PC6, &PORTC, PC5, &PORTC, PC4, &PORTC, PC3);
		GAME_INIT();
		power = 1;
	}
	else if (tmpA == 0x00 && power == 1)
	{
		PORTA = 0x01;
		power = 0;
	}
}
#endif //TASKS_H