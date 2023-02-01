#include <math.h>
#include <ncurses.h>
#include <wchar.h>
#include <unistd.h>

// variables
int nScreenWidth = 240;
int nScreenHeight = 80;
int nMapWidth = 16;
int nMapHeight = 16;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;
float fFOV = 3.141592f/2.0f;
float fDepth = 16.0f;
float fSpeed = 5.0f;

char distanceToWallBuff[30];

int inputchar;

int main()
{

// ncurses init	
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);
	nodelay(stdscr, TRUE);

	wchar_t *map = 
		L"#########......."
		L"#..............."
		L"#.......########"
		L"#..............#"
		L"#......##......#"
		L"#......##......#"
		L"#..............#"
		L"###............#"
		L"##.............#"
		L"#......####..###"
		L"#......#.......#"
		L"#......#.......#"
		L"#..............#"
		L"#......#########"
		L"#..............#"
		L"################";


	while (1)
	{
// user input 
		if ((inputchar = getch()) != ERR)
		{
			switch (inputchar)
			{
				case 'a': fPlayerA -= 3.141592f/4.0f; break;
				case 'd': fPlayerA += 3.141592f/4.0f; break;
				case 'w': fPlayerX += sinf(fPlayerA) * 1;
						  fPlayerY += cosf(fPlayerA) * 1; 
						  break;
				case 's': fPlayerX -= sinf(fPlayerA) * 1;
						  fPlayerY -= cosf(fPlayerA) * 1; 
						  break;
			}
		}

// ray casting 

		for (int x=0; x<nScreenWidth; x++)
		{
			float fRayAngle = (fPlayerA - fFOV/2.0f) + ((float)x / (float)nScreenWidth) * fFOV;
			float fStepSize = 0.1f;
			float fDistanceToWall = 0.0f;
			bool bHitWall = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
				
				// Ray out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{
					if (map[nTestX * nMapWidth + nTestY] == '#')
					{
						bHitWall = true;
					}
				}
			}

			int nCeiling = (float)(nScreenHeight/2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;
			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y <= nCeiling)
					mvprintw(y,x," ");
				else if (y > nCeiling && y <= nFloor)
					mvprintw(y,x,"#");
				else
					mvprintw(y,x," ");
			}
			refresh();
		}
	}

	endwin();
	return 0;
}
