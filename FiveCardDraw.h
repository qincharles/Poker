#ifndef FiveCardDraw_h
#define FiveCardDraw_h

#include "stdafx.h"
#include <algorithm>
#include <string>

#include "game.h"
#include "Poker.h"
#include "Utility.h"

using namespace std;

class FiveCardDraw: public Game {

public:
	FiveCardDraw();
	virtual int before_turn(Player & p);
	virtual int turn(Player & p);
	virtual int after_turn(Player & p);
	virtual int before_round();
	virtual int round();
	virtual int after_round();

protected:
	size_t dealer;
	Deck _trash;

private:
	void FiveCardDraw::print_leaving_players ( Player * leaving );
	void FiveCardDraw::players_joining_game();
	void FiveCardDraw::players_leaving_game();
	void players_leave_game();
	vector<string> tokenize(string input);
	vector<int> discardPrompt(Player & p);
	
};

#endif