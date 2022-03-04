#include "pico/stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "c64.h"

#include "st7735.h"
#include "gfx.h"
#include "ps2.h"

#include "Tiny3x3a2pt7b.h"

#define UPDATE_INTERVAL 35

#define BORDER_OFFSET 16

extern uint8_t RAM[RAM_SIZE];
extern uint8_t colorRAM[1024];
extern uint8_t backgroundColor;
extern uint8_t borderColor;

extern const uint8_t regular_kernal[];
extern const uint8_t sx_kernal[];
extern const uint8_t *kernal;

const uint16_t colorMapping[] = {
	ST77XX_BLACK,
	ST77XX_WHITE, 0x8800, 0xAFFD, 0xCA39, 0x066A, 0x0015, 0xEF6E, 0xDC4A, 0x6220,
	0xFBAE, 0x3186, 0x73AE, 0xAFEC, 0x045F, 0xBDD7};

char hello[] =
	"10 FOR I=0 TO 15\r20 POKE 646,I\r30 PRINT \"HELLO WORLD!\"\r40 NEXT I\r50 POKE 646,1\r";
char maze[] = "POKE 646,1\r10 PRINT CHR$(205.5+RND(1)); : GOTO 10\r";

char waitForKey()
{
	while (!PS2_keyAvailable())
		;
	char c = PS2_readKey();
	return c;
}

void clearRam()
{
	for (int i = 0; i < RAM_SIZE; i++)
		RAM[i] = 0;
}

void bootC64()
{
	GFX_setTextColor(ST77XX_WHITE);
	GFX_setFont(0);
	GFX_setCursor(0, 0);
	GFX_clearScreen();

	GFX_printf("Press S to boot SX-64\nAny other key for C64\n");
	GFX_flush();

	char c = waitForKey();

	if (c == 's')
		kernal = sx_kernal;
	else
		kernal = regular_kernal;

	if (kernal == regular_kernal)
		GFX_printf("C64 booting...\n");
	else
		GFX_printf("SX-64 booting...\n");

	GFX_flush();

	GFX_setFont(&Tiny3x3a2pt7b);

	clearRam();
	reset6502();

	for (int i = 1; i <= 600; i++)
	{
		exec6502(1000);
	}
}

void run64()
{
	static int s, prevS;
	for (int i = 1; i <= 500; i++)
	{
		readKeyboard();
		exec6502(5);
	}
	s = to_ms_since_boot(get_absolute_time()) / UPDATE_INTERVAL;
	if (s - prevS > 1)
	{
		redrawScreen();
		prevS = s;
	}
}

void writePetscii(uint8_t petscii, uint8_t row, uint8_t col)
{
	if (petscii == 77)
		petscii = '\\';
	else if (petscii == 78)
		petscii = '/';
	else if (petscii < 32)
		petscii = petscii + 64;
	uint8_t *cRam = colorRAM;
	uint16_t sc = colorMapping[*(cRam + (40 * row) + col)];
	GFX_drawChar(col * 4, row * 4 + BORDER_OFFSET, petscii,
				 sc, 0, 1, 1);
}

void drawBackAndBorder()
{
	int offset = 25 * 4 + (BORDER_OFFSET - 2);

	GFX_fillRect(0, BORDER_OFFSET - 3, 160,
				 128 - (BORDER_OFFSET - 3) - (128 - offset),
				 colorMapping[backgroundColor]);

	GFX_fillRect(0, 0, 160, BORDER_OFFSET - 3, colorMapping[borderColor]);
	GFX_fillRect(0, offset, 160, 128 - offset, colorMapping[borderColor]);
}

void redrawScreen()
{
	drawBackAndBorder();
	uint8_t *screenRam = RAM + 1024;
	for (uint8_t row = 0; row < 25; row++)
	{
		for (uint8_t col = 0; col < 40; col++)
		{
			writePetscii(*(screenRam++), row, col);
		}
	}
	GFX_flush();
}

void pokeKey(uint8_t key)
{
	key = toupper(key);
	// address 198: number of keyboard entries
	// address 631-640 : character buffer
	if (RAM[198] < 10)
	{
		RAM[198]++;
		RAM[631 + RAM[198] - 1] = key;
	}
}

void typeString(char *c)
{
	while (*c)
	{
		pokeKey(*c++);
		exec6502(5000);
		redrawScreen();
	}
}

void readKeyboard()
{
	if (PS2_keyAvailable())
	{
		int curkey = PS2_readKey() & 0x7F;

		if (curkey == PS2_ESC) // Escape
		{
			escapeMenu();
			return;
		}
		else if (curkey == PS2_PAGEUP) // PgUp
		{
			curkey = 147; // petscii clear screen
		}
		else if (curkey == PS2_BACKSPACE) // backspace or delete
		{
			curkey = 20;
		}
		pokeKey(curkey);
	}
}

void escapeMenu()
{
	GFX_setTextColor(ST77XX_WHITE);
	GFX_setFont(0);
	GFX_setCursor(0, 0);
	GFX_clearScreen();
	GFX_printf("Escape menu\n\nR for reset\n\n");
	GFX_printf("1 for Hello World\n2 for maze\n\n3 for border poke\n4 for background poke\n5 for text color poke\n");
	GFX_flush();
	GFX_setFont(&Tiny3x3a2pt7b);

	char c = waitForKey();
	if (c == 'r')
		bootC64();
	else if (c == '1')
		typeString(hello);
	else if (c == '2')
		typeString(maze);
	else if (c == '3')
		typeString("POKE 53280,");
	else if (c == '4')
		typeString("POKE 53281,");
	else if (c == '5')
		typeString("POKE 646,");
}

void splashScreen()
{
	GFX_clearScreen();
	GFX_printf("Crude64\nC64-ish emulator\n\n\n");
	GFX_printf("Press any key to continue\n\n\n\n\n");
	GFX_printf("\n\n\n\nBuilt on:\n%s\n", __TIMESTAMP__);

	GFX_flush();
	char c = waitForKey();
	sleep_ms(100);
}
