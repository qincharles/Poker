#ifndef Game_h
#define Game_h

#include "stdafx.h"
#include "Deck.h"
#include "Player.h"
#include "Utility.h"
#include <string>
#include <vector>

using namespace std;

class FiveCardDraw; //foward declaration
class Game {

public:
	Game();
	static Game * instance();
	static void start_game(string start);
	static void stop_game();
	void add_player(const string & name);
	void Game::leave_player(Player * player);
	void print_players();
	Player * find_player(string name); 
	virtual ~Game();

	virtual int before_turn(Player & player) = 0;
	virtual int turn(Player & player) = 0;
	virtual int after_turn(Player & player) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;

	int players_size();

	static int round_num;

protected:

	static Game *_game;
	Deck _deck;
	vector<Player *> _players;

private:
	
	Game(Game * game); //copy constructor is private, so only one game 
	void operator=(Game * game); //assignment operator is private, for same reason

};

#endif