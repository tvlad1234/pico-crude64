#include "pico/stdlib.h"

#include "st7735.h"
#include "gfx.h"
#include "ps2.h"

#include "c64.h"

int main()
{
	PS2_init(3, 2);

	LCD_initDisplay(INITR_BLACKTAB);
	LCD_setRotation(1);
	GFX_createFramebuf();

	splashScreen();
	bootC64();

	while (true)
	{
		run64();
	}
}
