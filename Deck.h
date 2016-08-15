#include <vector>
#include <ctime>
#include "Card.h"
#include "Hand.h"
#include "Utility.h"

#pragma once
class Deck
{
public:
	Deck(void);
	Deck(int decks);
	~Deck(void);
	void add_card(const Card & card);
	size_t Deck::size();
	void shuffle(int num_swaps);
	Card deal();


private:
	vector<Card> _cards;
};

Hand & operator<<(Hand & hand, Deck & deck);
Deck & operator<<(Deck & deck, Hand & hand);
Deck & operator<<(Deck & deck1, Deck & deck2);

