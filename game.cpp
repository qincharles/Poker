#include "stdafx.h"
#include "game.h"

using namespace std;

int Game::round_num = 1; //Keeps track of the round number of the game, incremented after every round in FCD

//Game constructor
Game::Game(){
	_deck = Deck();
}

//returned member variable game * _game
Game * Game::instance() {
	if (_game == 0) {
		throw instance_not_available;
	} 
	else {
		return _game;
	}
}


int Game::players_size(){
	return _players.size();
}

//// method to start a FiveCardDraw game if there is not already a game started
//void Game::start_game(string start) { //moved to main.cpp
//}

//method to stop a game in progress
void Game::stop_game() {
	if (_game == 0) {
		throw no_game_in_progress;		// throw exception if there is no game in progress
	} else {
		delete _game;					// if there is a game, delete it and set the pointer to 0
		_game = 0;
	}
}

// method to add a player to the game as long as there is no player by that name
void Game::add_player(const string & name) {
	for (unsigned int i = 0; i < _players.size(); i++) {			// search through the players vector to compare all existing players' names to the parameter
		if (_players[i]->_name == name) {
			throw already_playing;			// if any are the same, throw an exception
		}
	}
	_players.push_back(new Player(name));	// if none are the same, add the new player to the vector
}

//prints all players in the game, with wins and losses
void Game::print_players(){
	for(unsigned int i = 0; i < _players.size(); i++){
		cout << *_players[i] << endl;
	}
}

// method to return a player from the game as long as there is a player by that name
Player * Game::find_player(string name) {
	for (unsigned int i = 0; i < _players.size(); i++) {			// search through the players vector to compare the names
		if (_players[i]->_name == name) {
			return _players[i];				// if there is a match, return it
		}
	}
	return 0;								// if there are no matches, return 0
}

//erases the leaving player from the players vector
//iterates through all players and deletes the leaving player pointer from vector
void Game::leave_player(Player * leaving){
	for(unsigned int i = 0; i < _players.size(); i++){
		if( _players[i] == leaving){
			_players.erase(_players.begin() + i);
			return;
		}
	}

}

//Game Deconstructor
Game::~Game() {
	while (_players.size() != 0) {		// for every player in the game, delete the player
		_players.pop_back();
	}
}

//undefined copy constructor and operator=
Game::Game(Game * game){}
void Game::operator=(Game * game){}