#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream> // string stream ex. std::ostringsteam
#include <fstream> // file stream ex. ifstream and ofstream

using namespace std;

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


int main()
{
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