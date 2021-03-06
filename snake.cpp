/*
	Game: Snake
*/


#define MAXFRAMEX 119
#define MAXFRAMEY 29

#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>
#include <mmsystem.h>
#include <cstdlib>

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

// hides cursor
void setcursor(bool visible, DWORD size) { // set bool visible = 0 -invisible, bool visible = 1 - visible

	if (size == 0) {
		size = 20; // default cursor size changing to numbers from 1 to 20, decreases cursor width
	}

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

class Point {
	private:
		int x;
		int y;
	
	public:
		Point() {
			x = y = 10;
		}

		Point(int x, int y) {
			this->x = x;
			this->y = y;
		}

		void SetPoint(int x, int y) {
			this->x = x;
			this->y = y;
		}

		int getX() {
			return x;
		}

		int getY() {
			return y;
		}

		void MoveUp() {
			y--;
			if (y < 0) {
				y = MAXFRAMEY;
			}
		}

		void MoveDown() {
			y++;
			if (y > MAXFRAMEY) {
				y = 0;
			}
		}

		void MoveLeft() {
			x--;
			if (x < 0) {
				x = MAXFRAMEX;
			}
		}

		void MoveRight() {
			x++;
			if (x > MAXFRAMEX) {
				x = 0;
			}
		}

		void Draw(char ch = 'O') {
			gotoxy(x, y);
			cout << ch;
		}

		void Erase() {
			gotoxy(x,y);
			cout << " ";
		}

		void CopyPos(Point* p) {
			p->x = x;
			p->y = y;
		}

		int IsEqual(Point* p) {
			if (p->x == x && p->y == y) {
				return 1;
			}
			return 0;
		}

		void Debug() {
			cout << "(" << x << "," << y << ")";
		}
};

class Snake {
	private:
		Point* cell[100]; // array of points to represent snake
		int size; // current size of the snake
		char dir; // current direction of the snake
		Point fruit;
		Point poison;
		int state; // shows if snake is alive or dead
		int started;
		int blink; // fruit blinking
		int time_counter = 0;
	
	public:
		Snake() {
			size = 1; // default size
			cell[0] = new Point(20,20); // default position
			
			for (int i = 1; i < 100; i++) {
				cell[i] = NULL;
			}

			fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
			poison.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
			state = 0;
			started = 0;
		}

		void addCell(int x, int y) {
			cell[size++] = new Point(x, y);
		}

		void subCell(int x, int y) {
			cell[size--] = new Point(x, y);
		}

		void Up() {
			if (dir != 's')
			dir = 'w'; // w for moving up
		}

		void Down() {
			if (dir != 'w')
			dir = 's'; // s for moving down
		}
		
		void Left() {
			if (dir != 'd')
			dir = 'a'; // a for moving left
		}

		void Right() {
			if (dir != 'a')
			dir = 'd'; // d for moving right
		}

		void WelcomeScreen() {
			SetConsoleTextAttribute(console, 15);
			cout << "\n\n\n\n\n\n\t\t\t\t\t\t~~~WELCOME TO SNAKE~~~\n\n\n" << endl;
			cout << "\t\t\t\t\t\tInstructions" << endl;
			cout << "\t\t\t\t\t\tMovements: " << endl;
			cout << "\t\t\t\t\t\tw - Up" << endl;
			cout << "\t\t\t\t\t\ts - Down" << endl;
			cout << "\t\t\t\t\t\ta - Left" << endl;
			cout << "\t\t\t\t\t\td - Right" << endl << endl;

			cout << "\t\t\t\t\t\tEat * to increase in size" << endl;
			cout << "\t\t\t\t\t\tAvoid X" << endl;
			cout << "\n\t\t\t\t\t\tPress 'e' to exit the game" << endl;
		}

		void Move() {
			// Clear screen
			system("cls");

			if (state == 0) {
				if (!started) {
					WelcomeScreen();
					cout << "\n\t\t\t\t\t\tPress any key to start";
					_getch();
					started = 1;
					state = 1;
				}
				else {
					cout << "\n\t\t\t\t\t\tGame Over" << endl;
					cout << "\n\t\t\t\t\t\tPress any key to start";
					_getch();
					state = 1;
					size = 1;
				}
			}

			// making snake body follow
			for (int i = size - 1; i > 0; i--) {
				cell[i - 1]->CopyPos(cell[i]);
			}

			// turning the snake
			switch (dir) {
			case 'w':
				cell[0]->MoveUp();
				break;
			case 's':
				cell[0]->MoveDown();
				break;
			case 'a':
				cell[0]->MoveLeft();
				break;
			case 'd':
				cell[0]->MoveRight();
				break;
			}

			if (SelfCollision()) {
				state = 0;
			}

			// Collision with poison
			if (poison.getX() == cell[0]->getX() && poison.getY() == cell[0]->getY()) {
				subCell(0, 0);
				poison.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
				time_counter++;
			}

			// Make poison spawn randomly
			if (time_counter == 3) {
				poison.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
				time_counter = 0;
			}

			// Collision with fruit
			if (fruit.getX() == cell[0]->getX() && fruit.getY() == cell[0]->getY()) {
				addCell(0, 0);
				fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
				time_counter++;
			}

			// drawing the snake
			for (int i = 0; i < size; i++) {
				cell[i]->Draw();
			}

			SetConsoleTextAttribute(console, 252);
		
			if (!blink) {
				fruit.Draw('*');
			}

			if (!blink) {
				poison.Draw('X');
			}

			blink = !blink;
		
			SetConsoleTextAttribute(console, 242);
			//Debug();
			Sleep(10);
		}

		int SelfCollision() {
			for (int i = 1; i < size; i++) {
				if (cell[0]->IsEqual(cell[i])) {
					return 1;
				}
			}
			return 0;
		}

		void Debug() {
			for (int i = 0; i < size; i++) {
				cell[i]->Debug(); 
			}
		}
};

int main() {
	srand((unsigned)time(NULL));

	setcursor(0, 0);

	//PlaySound(TEXT("snake.wav"), NULL, SND_SYNC);

	// testing
	Snake snake;
	char op = 'l';

	do {
		if (_kbhit()) {
			op = _getch();
		}

		switch (op) {
		case 'w':
		case 'W':
			snake.Up();
			break;
		case 's':
		case 'S':
			snake.Down();
			break;
		case 'a':
		case 'A':
			snake.Left();
			break;
		case 'd':
		case 'D':
			snake.Right();
			break;
		}
		snake.Move();

	} while (op != 'e');
	
	return 0;
}
