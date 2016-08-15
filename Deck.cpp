#include "stdafx.h"
#include "Deck.h"

Deck::Deck(void)
{
}

//Deck Constructor that creates cards and adds them to the deck
//takes in an int to determine the number of decks (52 cards)x(int decks) to be created and added to Deck
Deck::Deck(int decks){
	for(int i=0; i < decks; i++){
		for( int j = Card::two; j <= Card::ace; j++){
			for ( int k = Card::club; k <= Card::spade; k++){
				Card::_RANK jj = (Card::_RANK)j;
				Card::_SUIT kk = (Card::_SUIT)k;
				Card c = Card(jj,kk);
				add_card(c);
			}
		}
	}
	cout << "Created All 52 Cards" << endl;
	shuffle(decks*100);
}

Deck::~Deck(void)
{
	//vector automatically cleans up after itself
}

void Deck::add_card(const Card & card)
{
	_cards.push_back(card);
}

size_t Deck::size(){
	return _cards.size();
}

//shuffles the vector of cards num_swaps times
void Deck::shuffle(int num_swaps)
{
	//const int NUM_SHUFFLES = 10;
	int size = (int)_cards.size();
	srand((unsigned int)time(0));
	for(int i = 0; i < num_swaps; i++){
		int r1 = rand() % size;
		int r2 = rand() % size;
		Card temp = _cards[r1];
		_cards[r1] = _cards[r2];
		_cards[r2] = temp;
	}
}

/*
Throws error EMPTY to signify that the deck is empty
Otherwise, returns the last card in the deck
*/
Card Deck::deal() 
{
	Card card;
	if(_cards.size() > 0){ //if deck is not empty
		card = _cards.back();
		_cards.pop_back();
	}
	else{
		throw EMPTY;
	}
	return card;
}

//non-member stuff
Hand & operator<<(Hand & hand, Deck & deck)
{
	Card card = deck.deal();
	hand.insert(card);
	return hand;
}


Deck & operator<<(Deck & deck, Hand & hand)
{
	int size = hand.size();
	for( int i = 0; i < size; i++){
		deck.add_card(hand[0]);
		hand.remove_card(0);
	}
	return deck;
}


Deck & operator<<(Deck & deck1, Deck & deck2)
{

	while( true ){
		try{
			deck1.add_card(deck2.deal());

		}
		catch(ERRORS & e){
			if( e == EMPTY){
					return deck1;
				}
				else{
					cout << "Caught error code: " << e << endl;
					throw e;
				}
		}
		catch(...){
			cout << "Unknown Error caught" << endl;
			ERRORS e = Unspecified;
			throw e;
		}
	}
}
