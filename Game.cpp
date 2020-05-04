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
#include <string>

#define MAXFRAMEX 119
#define MAXFRAMEY 29

using namespace std;

///////// CLASSES AND METHODS FOR THE BATTLESHIP GAME //////////
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 10;
const int SHIP_TYPES = 5;

const char isWATER = 247; //ASCII Character Code
const char isHIT = 'X';
const char isSHIP = 'S';
const char isMISS = '0';

struct PNT {
	//A location on the grid defined
	//by X(horizontal) Y(vertical) coordinates
	int X;
	int Y;
};

struct SHIP {
	//Ship name
	string name;
	//Total points on the grid
	int length;
	//Coordinates of those points
	PNT onGrid[5]; //0-4 max length of biggest ship
	//Whether or not those points are a "hit"
	bool hitFlag[5];
}ship[SHIP_TYPES];

struct PLAYER {
	char grid[BOARD_WIDTH][BOARD_HEIGHT];
}player[3]; //Ignore player 0, just using player's 1 & 2

enum DIRECTION {HORIZONTAL,VERTICAL};

struct PLACESHIPS {
	DIRECTION direction;
	SHIP shipType;
};

bool gameRunning = false;

//Functions
void LoadShips();
void ResetBoard();
void DrawBoard(int);
PLACESHIPS UserInputShipPlacement();
bool UserInputAttack(int&,int&,int);
bool GameOverCheck(int);



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
  cout << "Please enter A, B, or C: ";
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

        cout << "Would you like to save your game? Answer with yes or no." << endl;

        cin >> saveOption;

        if (saveOption == "yes" || saveOption == "Yes") {
            
            ratio.create_out_file(wins, losses);
        }

        cout << "Would you like to try again? Answer with yes or no." << endl;

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

	return 0;
  }
  
  else if (option == 'C' || option == 'c'){
    LoadShips();
	ResetBoard();

	//"PLACE SHIPS" phase of game
	//Loop through each player... 
	for (int plyr=1; plyr<3; ++plyr){
		//Loop through each ship type to place
		for (int thisShip=0; thisShip<SHIP_TYPES; ++thisShip){
			//Display gameboard for player
			system("cls");
			DrawBoard(plyr);
			//Give instructions
			cout << "\n";
			cout << "INSTRUCTIONS (Player " << plyr << ")\n\n";
			cout << "You are about to place your ships.  Format should be:\n";
			cout << "Facing (0:Horizontal,1:Vertical), X (top-row) coords, Y (left-side) coords\n";
			cout << "Example: 0 7 2    This would place a ship beginning at X:7 Y:2 going horizontal\n\n";
			cout << "Ship to place: " << ship[thisShip].name << " which has a length of " << ship[thisShip].length  << "\n";
			cout << "Where do you want it placed? ";
			
			//Get input from user and loop until good data is returned
			PLACESHIPS aShip;
			
			aShip.shipType.onGrid[0].X = -1;
			
			while (aShip.shipType.onGrid[0].X == -1){
				aShip = UserInputShipPlacement();
			}

			//Combine user data with "this ship" data
			aShip.shipType.length = ship[thisShip].length;
			aShip.shipType.name = ship[thisShip].name;

			//Add the FIRST grid point to the current player's game board
			player[plyr].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y] = isSHIP;

			//Determine ALL grid points based on length and direction
			for (int i=1; i<aShip.shipType.length; ++i){
				if (aShip.direction == HORIZONTAL){
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X+1;
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y; 
					}
					
				if (aShip.direction == VERTICAL){
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y+1;
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X; 
					}

				//Add the REMAINING grid points to our current players game board
				player[plyr].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y] = isSHIP;
			}
			//Loop back through each ship type
		}
		//Loop back through each player
	}

	

	//Ready to play the game
	gameRunning = true;
	int thisPlayer = 1;
	do {
		//Because we are ATTACKING now, the 
		//opposite players board is the display board
		int enemyPlayer;
		
		if (thisPlayer == 1){ 
			enemyPlayer = 2;
		}
		
		if (thisPlayer == 2){
			enemyPlayer = 1;
		}
		
		system("cls");
		DrawBoard(enemyPlayer);

		//Get attack coords from this player
		bool goodInput = false;
		int x,y;
		while (goodInput == false) {
			goodInput = UserInputAttack(x,y,thisPlayer);
		}

		//Check board; if a ship is there, set as HIT.. otherwise MISS
		if (player[enemyPlayer].grid[x][y] == isSHIP){
			player[enemyPlayer].grid[x][y] = isHIT;
			cout << "A ship has been hit!" << endl;
		}
		
		if (player[enemyPlayer].grid[x][y] == isWATER){
			player[enemyPlayer].grid[x][y] = isMISS;
			cout << "You missed!" << endl;
		}
		
		//Check to see if the game is over
		//If 0 is returned, nobody has won yet
		int aWin = GameOverCheck(enemyPlayer);
		if (aWin != 0) {
			gameRunning = false;
			break;
		}
		//Alternate between each player as we loop back around
		thisPlayer = (thisPlayer == 1) ? 2 : 1;
	} while (gameRunning);

	system("cls");
	cout << "\n\nCONGRATULATIONS!!!  PLAYER " << thisPlayer << " HAS WON THE GAME!\n\n\n\n";

	system("pause");
	return 0;
    
  }
  
  else {
    cout << "Invalid option, please enter A or B" << endl;
  }

  return 0;
}


bool GameOverCheck(int enemyPLAYER){
	bool winner = true;
	//Loop through enemy board
	for(int w=0; w<BOARD_WIDTH; ++w){
			for (int h=0; h<BOARD_HEIGHT; ++h){
				//If any ships remain, game is NOT over
				if (player[enemyPLAYER].grid[w][h] = isSHIP){
						winner = false;
						return winner;
				}
			}
	}
	//game over
	return winner;
}


bool UserInputAttack(int& x, int& y, int theplayer){
	cout << "\nPLAYER " << theplayer << ", ENTER COORDINATES TO ATTACK: ";
	bool goodInput = false;
	cin >> x >> y;
	
	while(x > BOARD_WIDTH || y > BOARD_HEIGHT){ //error check
		cout << "Invalid coordinate! Please try again and make sure it is in range of the board!" << endl;
		cin >> x >> y;
	}
	
	if (x<0 || x>=BOARD_WIDTH){
		return goodInput;
	}
	
	if (y<0 || y>=BOARD_HEIGHT){
		return goodInput;
	}
	
	goodInput = true; 
	return goodInput;
}

PLACESHIPS UserInputShipPlacement(){
	
	int d, x, y;
	PLACESHIPS tmp;
	//Using this as a bad return
	//tmp.shipType.onGrid[0].X = -1;
	//Get 3 integers from user for ship placement
	cin >> d >> x >> y;
	while(d!=0 && d!=1 || x<0 || x>=BOARD_WIDTH || y<0 || y>=BOARD_HEIGHT){ //error check
		cout << "Invalid value! Make sure values are in range with the board! Try again." << endl;
		cin >> d >> x >> y;
	}
		
	tmp.direction = (DIRECTION)d;
	tmp.shipType.onGrid[0].X = x;
	tmp.shipType.onGrid[0].Y = y;
	return tmp;
}

void LoadShips(){
	
	//Sets the default data for the ships
	//must match SHIP_TYPES Default = 5 (0-4)
	ship[0].name = "Cruiser"; ship[0].length = 2;
	ship[1].name = "Frigate"; ship[1].length = 3;
	ship[2].name = "Submarine"; ship[2].length = 3;
	ship[3].name = "Escort"; ship[3].length = 4;
	ship[4].name = "Battleship"; ship[4].length = 5;
}

void ResetBoard()
{
	//Loop through each player
	for (int plyr=1; plyr<3; ++plyr)
	{
		//For each grid point, set contents to water
		for (int w=0; w<BOARD_WIDTH; ++w){
			for (int h=0; h<BOARD_HEIGHT; ++h){
				player[plyr].grid[w][h] = isWATER;
		}}
		//Loop back to next player
	}
}

void DrawBoard(int thisPlayer)
{
	//Draws the board for a player (thisPlayer)
	cout << "PLAYER " << thisPlayer << "'s GAME BOARD\n";
	cout << "----------------------\n";

	//Loop through top row (board_width) and number columns
	cout << "   ";
	for (int w=0; w<BOARD_WIDTH; ++w) {
		if (w < 10)
			//Numbers only 1 character long, add two spaces after
			cout << w << "  ";
		else if (w >= 10)
			//Numbers 2 characters long, add only 1 space after
			cout << w << " ";
	}
	cout << "\n";

	//Loop through each grid point and display to console
	for (int h=0; h<BOARD_HEIGHT; ++h){
		for (int w=0; w<BOARD_WIDTH; ++w){
			
			//If this is the grid point, number the grid first
			if (w==0) cout << h << " ";
			//If h was 1 character long, add an extra space to keep numbers lined up
			if (w<10 && w==0) cout << " ";
			//Display contents of this grid (if game isn't running yet, we are placing ships
			//so display the ships
			if (gameRunning == false) cout << player[thisPlayer].grid[w][h] << "  ";
			//Don't show ships, BUT show damage if it's hit
			if (gameRunning == true && player[thisPlayer].grid[w][h] != isSHIP) 
			{cout << player[thisPlayer].grid[w][h] << "  ";}
			else if (gameRunning == true && player[thisPlayer].grid[w][h] == isSHIP)
			{cout << isWATER << "  ";}	
			//If we have reached the border.. line feed
			if (w == BOARD_WIDTH-1) cout << "\n";
		}
	}
}
