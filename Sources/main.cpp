/* Header file with all the essential definitions for a given type of MCU */

#include "Snake.h"


/* Constants specifying delay loop duration */
#define	tdelay1			10000
#define tdelay2 		50//20

enum Buttons
{   
	Left = 1<<26,
	Right = 1<<12,
	Top = 1<<10,
	Bottom = 1<<27,
	All = Left|Right|Top|Bottom
};

Snake::Direction dir = Snake::Direction::Right;
volatile bool changed = false;


/* Variable delay loop */
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
}

void PORTE_IRQHandler(void) {
	//delay(1000, 1);
	if ((PORTE->ISFR & Left) && !(PTE->PDIR & Left) && dir!= Snake::Direction::Right)
	{
		dir = Snake::Direction::Left;
	}	
	if ((PORTE->ISFR & Right) && !(PTE->PDIR&Right)&& dir != Snake::Direction::Left)
	{
		dir = Snake::Direction::Right;
	}	
	if ((PORTE->ISFR & Top) && !(PTE->PDIR&Top)&& dir != Snake::Direction::Down)
	{
		dir = Snake::Direction::Up;
	}	
	if ((PORTE->ISFR & Bottom) && !(PTE->PDIR&Bottom)&& dir != Snake::Direction::Up)
	{
		dir = Snake::Direction::Down;
	}
	changed = true;
	PORTE->ISFR |= All;
}

void ChangedDir()
{
	if (changed)return;
	if (!(PTE->PDIR & Left) && dir != Snake::Direction::Right)
	{
		changed = true;
			dir = Snake::Direction::Left;
	}	
	if (!(PTE->PDIR & Right) && dir != Snake::Direction::Left)
	{
		changed = true;
			dir = Snake::Direction::Right;
	}	
	if (!(PTE->PDIR & Top) && dir != Snake::Direction::Down)
	{
		changed = true;
			dir = Snake::Direction::Up;
	}	
	if (!(PTE->PDIR & Bottom) && dir != Snake::Direction::Up)
	{
		changed = true;
			dir = Snake::Direction::Down;
	}
}
bool RestartPressed()
{
	return !(PTE->PDIR & (1 << 11));
}


int main(void)
{
	SystemConfig();
	SetInterrupts();
	Snake s;
	
	bool render = true;
	
    for (unsigned long long i = 0;;i++) {
    	if(RestartPressed())
    	{
    		render = true;
    		s.Reset();
    		i = 0;
    	}
	    if (!render)continue;
	    ChangedDir();//IRQ failed
	    if(i == 2000){
		    if (!s.CheckState())render = false;
		    if (changed) { 
			    s.ChangeDir(dir);
			    changed = false;
		    }
		    s.Step();
		    i = 0;
	    }
	    s.Render();
    }
    /* Never leave main */
    return 0;
}
