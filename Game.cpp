#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>
#include <mmsystem.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>

#define MAXFRAMEX 119
#define MAXFRAMEY 29

using namespace std;

///////// CLASSES AND METHODS FOR THE SNAKE GAME ////////////

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

/////////// CLASSES FOR HIGHER AND LOWER CARD GAME ///////////

class command_line_error : public runtime_error {

public:

    explicit command_line_error(const char* what_arg) : runtime_error(what_arg) {}

    explicit command_line_error(const string& what_arg) : runtime_error(what_arg) {}
};

class Card {
private:

    int rank;
    int suit;

public:

    Card() {
        srand(time(NULL));
        rank = (1 + (rand() % 13));
        suit = (1 + (rand() % 4));
    }

    int get_rank() {
        return rank;
    }

    int get_suit() {
        return suit;
    }

    string get_rank_string() {
        if (rank == 1) {
            return "Ace";
        }
        else if (rank == 2) {
            return "Two";
        }
        else if (rank == 3) {
            return "Three";
        }
        else if (rank == 4) {
            return "Four";
        }
        else if (rank == 5) {
            return "Five";
        }
        else if (rank == 6) {
            return "Six";
        }
        else if (rank == 7) {
            return "Seven";
        }
        else if (rank == 8) {
            return "Eight";
        }
        else if (rank == 9) {
            return "Nine";
        }
        else if (rank == 10) {
            return "Ten";
        }
        else if (rank == 11) {
            return "Jack";
        }
        else if (rank == 12) {
            return "Queen";
        }
        else if (rank == 13) {
            return "King";
        }
        else {
            return "Unknown Rank";
        }
    }

    string get_suit_string() {
        if (suit == 1) {
            return "Diamonds";
        }
        else if (suit == 2) {
            return "Hearts";
        }
        else if (suit == 3) {
            return "Spades";
        }
        else if (suit == 4) {
            return "Clubs";
        }
        else {
            return "Unknown Suit";
        }
    }
};

class OutputFile {
public:

    void create_out_file(int winData, int lossData) {
        fstream f;

        f.open("Results.txt", fstream::app);
        if (!f.is_open()) {
            ostringstream msg;
            msg << "Failed to open the output file" << endl;
            throw runtime_error(msg.str());
        }
        
        f << "These are the amount of wins you have: " << winData << endl;
        f << "These are the amount of losses you have: " << lossData << endl;
        f << endl;

        f.close();
    }

};



int main(){

  srand((unsigned)time(NULL));
  char option = ' ';

  cout << "\nPlease choose what game you would like to play" << endl;
  cout << "A. Higher or Lower" << endl;
  cout << "B. Snake" << endl;
  cout << "C. Battleship" << endl;
  cout << "Please enter A or B: ";
  cin >> option;
  
  if (option == 'A' || option == 'a'){
  	int house_rank;
    int player_rank;
    char choice;
    string saveOption;
    string gameOption;
    int wins = 0;
    int losses = 0;
    Card card;
    OutputFile ratio;

    card = Card();
    house_rank = card.get_rank();

    do {
        cout << "You have drawn a " << card.get_rank_string() << " of " << card.get_suit_string() << endl;
        cout << "Will the next card be higher or lower? " << "Enter 'h' for higher or 'l' for lower: " << endl;
        cin >> choice;

        if ((choice == 'l') || (choice == 'h')) {

            card = Card();
            player_rank = card.get_rank();

            cout << "You drew a " << card.get_rank_string() << " of " << card.get_suit_string() << endl;

            if ((choice == 'h' && house_rank > player_rank) || (choice == 'l' && house_rank < player_rank)) {
                cout << "Sorry, but you lose" << endl;
                losses++;
            }
            else {
                cout << "Congratulations you win!" << endl;
                wins++;
            }
        }
        else {
            cout << "Incorrect letter. Make sure there is no capitalization!" << endl;
            return 0;
        }

        cout << "Would you like to save your game?" << endl;

        cin >> saveOption;

        if (saveOption == "yes" || saveOption == "Yes") {
            
            ratio.create_out_file(wins, losses);
        }

        cout << "Would you like to try again?" << endl;

        cin >> gameOption;
       
    } while (gameOption != "no" && gameOption != "No");

    cout << "Thank you for playing!" << endl;
    cout << "Check out your 'Results' files to see your win/loss ratio!" << endl;

    return 0;
 
  }

  else if (option == 'B' || option == 'b'){
    // launch snake
    
    setcursor(0, 0);
    
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

	} while(op != 'e');
  }
  
  else if (option == 'C' || option == 'c'){
    // launch battleship
    
  }
  
  else {
    cout << "Invalid option, please enter A or B" << endl;
  }

  return 0;
}
