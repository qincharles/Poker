#ifndef Poker_h
#define Poker_h

#include "stdafx.h"
#include "Card.h"
#include "Hand.h"
#include "Player.h"
#include "Utility.h"
#include <string>
#include <iostream>


const static int HAND_SIZE = 5;
class Poker{

public:

	enum HAND_TIER {nullTier = 0, high_card = 1, pairr = 2, two_pair = 3, three_of_a_kind = 4,
		straight = 5, flushh = 6, full_house = 7, four_of_a_kind = 8, straight_flush = 9};

	static bool poker_rank(const Hand & hand1, const Hand & hand2);
	static bool poker_rank_players(const Player * p1, const Player * p2);
	static bool is_tied_players(const Player * p1, const Player * p2);
	static HAND_TIER get_tier(Hand hand);
	static vector<Card::_RANK> get_tiebreakers(Hand hand);
	static string Poker::tier_to_string(HAND_TIER tier);

private:

	static void assessHandValue(Hand h, HAND_TIER & tier, vector<Card::_RANK> & tieBreakers);
	static Card::_RANK irank_to_erank(int n);
};

#endif 