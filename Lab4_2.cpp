// Lab4_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <vector>

#include "Card.h"
#include "Hand.h"
#include "Deck.h"
#include "Player.h"
#include "Utility.h"
#include "FiveCardDraw.h"



int explainUsage(char * argv[]){
	cout << "Welcome to " << argv[0] << ". Please retype " << argv[0] 
	<< " and the name of the game you'd like to play (ie. 'FiveCardDraw'), along with the players' names." << endl; 
	return Usage_Message;
}

Game * Game::_game = NULL;

// method to start a FiveCardDraw game if there is not already a game started
void Game::start_game(string start) {
	if (_game != 0) {
		throw game_already_started;		// if game != 0, then a game has been instanciated
	} else if (start.compare("FiveCardDraw")) {
		throw unknown_game;				// for this lab, only FiveCardDraw games are known
	} else {
		FiveCardDraw * fcd = new FiveCardDraw( );  //
		_game = fcd;  
		//_game = 0;
	}
}

int main(int argc, char* argv[])
{

	//process args
	string gamename = "";
	if (argc <= 2 ){
		return explainUsage(argv);
	}
	else if (argc > 2){
		gamename = argv[1];
		try{
			Game::start_game(gamename);
		}
		catch(ERRORS & e){
			if( e == unknown_game){
				cout << "An unknown game was inputted. The Program was stopped." << endl;
				explainUsage(argv);
				return e;
			}
			else if ( e == game_already_started){
				cout << "A game is already started." << endl;
				return e;
			}
			else{
				cout << "A fatal error occured and the game was stopped" << endl;
				return e;
			}
		}
		catch(...){
			cout << "An unknown fatal error has occured." << endl;
			return Unspecified;
		}

	}

	//Game * game = new FiveCardDraw();

	Game * game = Game::instance();

	for(int i = 2; i < argc; i++){
		try{
		game->add_player(argv[i]);
		}
		catch(ERRORS & e){
			if(e == already_playing){
				cout << "Player " << argv[i] << " is already playing. " << endl;
				cout << "Therefore, was not added again." << endl;
			}
			else{
				cout << "A fatal error occured and the game was stopped" << endl;
				Game::stop_game();
				return e;
			}
		}
		catch(...){
			Game::stop_game();
			cout << "An unknown fatal error has occured." << endl;
			return Unspecified;



		}
	}

	while( game->players_size() > 0){
		try{
			int success = game->before_round();
			if( success != 0){
				Game::stop_game();
				cout << "Have a Nice Day!" << endl;
				return success;
			}
		}
		catch(ERRORS & e){
			cout << "A fatal error occured and the game was stopped" << endl;
			Game::stop_game();
			return e;
		}
		catch(...){
			Game::stop_game();
			cout << "An unknown fatal error has occured." << endl;
			return Unspecified;
		}
		try{
			int success =  game->round();
			if( success != 0){
				Game::stop_game();
				cout << "Have a Nice Day!" << endl;
				return success;
			}
		}
		catch(ERRORS & e){
			cout << "A fatal error occured and the game was stopped" << endl;
			Game::stop_game();
			return e;
		}
		catch(...){
			Game::stop_game();
			cout << "An unknown fatal error has occured." << endl;
			return Unspecified;
		}
		try{
			int success = game->after_round();
			if( success != 0){
				Game::stop_game();
				cout << "Have a Nice Day!" << endl;
				return success;
			}
		}
		catch(ERRORS & e){
			cout << "A fatal error occured and the game was stopped" << endl;
			Game::stop_game();
			return e;
		}
		catch(...){
			Game::stop_game();
			cout << "An unknown fatal error has occured." << endl;
			return Unspecified;
		}
	}
	Game::stop_game();
	cout << "Have a Nice Day!" << endl;


//string junk;
//cout << "Type anything and hit enter to quit" << endl;
//cin >> junk;
	return Success;
}