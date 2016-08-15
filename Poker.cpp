#include "stdafx.h"
#include "Poker.h"

using namespace std;


Card::_RANK Poker::irank_to_erank(int n)
{
	 switch( n )
	 {
		case 0 :  return Card::nullRank;
		case 2 : return Card::two;
		case 3 : return Card::three;
		case 4 : return Card::four;
		case 5 : return Card::five;
		case 6 : return Card::six;
		case 7 : return Card::seven;
		case 8 : return Card::eight;
		case 9 : return Card::nine;
		case 10 : return Card::ten;
		case 11 : return Card::jack;
		case 12 : return Card::queen;
		case 13 : return Card::king;
		case 14 : return Card::ace;
		default: throw;
	}
}

string Poker::tier_to_string(HAND_TIER tier)
{
	 switch( tier )
	 {
		case nullTier :  return "null";
		case high_card : return "high card";
		case pairr : return "pair";
		case two_pair : return "two pair";
		case three_of_a_kind : return "three of a kind";
		case straight : return "straight";
		case flushh : return "flush";
		case full_house : return "full house";
		case four_of_a_kind : return "four of a kind";
		case straight_flush : return "straight flush";
		default: throw;
	}
}

void Poker::assessHandValue(Hand h, HAND_TIER & tier, vector<Card::_RANK> & tieBreakers)
{
	int ranks[Card::NUM_RANKS];
	int suits[Card::NUM_SUITS];
	//initialize to 0s
	for(int i = 0; i < Card::NUM_RANKS; i++){
		ranks[i] = 0;
	}
	for(int i = 0; i < Card::NUM_SUITS; i++){
		suits[i] = 0;
	}
	//pass through counting all the rank and suit hits
	Card card;
	for(int i = 0; i < (int)h.get_cards().size(); i++){
		card = h.get_cards()[i];
		int erank = card.get_erank();
		int esuit = card.get_esuit();
		ranks[erank]++;
		suits[esuit]++;
	}
	//find best hand tier

	//test for straights and flushes
	bool flushb = false;
	bool straightb = false;
	int straightCounter = 0;
	Card::_RANK straightHighRank = h.get_cards()[(int)h.get_cards().size() - 1].get_erank();
	int flushCounter = 0;
	Card::_SUIT flushType = h.get_cards()[(int)h.get_cards().size() - 1].get_esuit();
	for(int i = 0; i < (int)h.get_cards().size(); i++){
		if(h.get_cards()[i].get_erank() == (straightHighRank - ((int)h.get_cards().size() - 1) + i)){
			straightCounter++;
		}
		if(h.get_cards()[i].get_esuit() == flushType){
			flushCounter++;
		}
	}
	if(straightCounter == (int)h.get_cards().size()){
		//cout << "found a straight!" << endl;
		string rankString = Card::convertErank(straightHighRank);
		//cout << "straight highest rank = " << rankString << endl;
		straightb = true;
	}
	if(flushCounter == (int)h.get_cards().size()){
		//cout << "found a flush!" << endl;
		string suitString = Card::convertEsuit(flushType);
		//cout << "flush suit = " << suitString << endl;
		flushb = true;
	}

	if(flushb && straightb){
		tier = straight_flush;
		tieBreakers[0] = straightHighRank;
	}
	else if(flushb){
		tier = flushh;
		int tieBreakersIndex = 0;
		for(int i = Card::NUM_RANKS - 1; i >= 0; i--){
			int matches = ranks[i];
			if (matches == 1){
				tieBreakers[tieBreakersIndex] = irank_to_erank(i);
				tieBreakersIndex++;
			}
		}
	}
	else if(straightb){
		tier = straight;
		tieBreakers[0] = straightHighRank;
	}
	else{
		//detect pairs, 3 of a kind, ect.
		int matches1 = 0;
		int matches2 = 0;
		int rank1 = 0;
		int rank2 = 0;
		for(int i = 0; i < Card::NUM_RANKS; i++){
			int matches = ranks[i];
			if(matches >= matches1){
				matches2 = matches1;
				rank2 = rank1;

				matches1 = matches;
				rank1 = i;
			}
			else if(matches >= matches2){
				matches2 = matches;
				rank2 = i;
			}
		}
		if(matches1 == 4){
			tier = four_of_a_kind;
			tieBreakers[0] = irank_to_erank(rank1);
			int tieBreakersIndex = 1;
			for(int i = Card::NUM_RANKS - 1; i >= 0; i--){
				int matches = ranks[i];
				if (matches == 1){
					tieBreakers[tieBreakersIndex] = irank_to_erank(i);
					tieBreakersIndex++;
				}
			}
		}
		else if(matches1 == 3){
			if(matches2 == 2){
				tier = full_house;
				tieBreakers[0] = irank_to_erank(rank1);
				tieBreakers[1] = irank_to_erank(rank2);
			}
			else{
				tier = three_of_a_kind;
				tieBreakers[0] = irank_to_erank(rank1);
				int tieBreakersIndex = 1;
				for(int i = Card::NUM_RANKS - 1; i >= 0; i--){
					int matches = ranks[i];
					if (matches == 1){
						tieBreakers[tieBreakersIndex] = irank_to_erank(i);
						tieBreakersIndex++;
					}
				}
			}
		}
		else if(matches1 == 2){
			if(matches2 == 2){
				tier = two_pair;
				tieBreakers[0] = irank_to_erank(rank1);
				tieBreakers[1] = irank_to_erank(rank2);
				int tieBreakersIndex = 2;
				for(int i = Card::NUM_RANKS - 1; i >= 0; i--){
					int matches = ranks[i];
					if (matches == 1){
						tieBreakers[tieBreakersIndex] = irank_to_erank(i);
						tieBreakersIndex++;
					}
				}
			}
			else{
				tier = pairr;
				tieBreakers[0] = irank_to_erank(rank1);
				int tieBreakersIndex = 1;
				for(int i = Card::NUM_RANKS - 1; i >= 0; i--){
					int matches = ranks[i];
					if (matches == 1){
						tieBreakers[tieBreakersIndex] = irank_to_erank(i);
						tieBreakersIndex++;
					}
				}
			}
		}
		else{
			tier = high_card;
			int tieBreakersIndex = 0;
			for(int i = Card::NUM_RANKS - 1; i >= 0; i--){
				int matches = ranks[i];
				if (matches == 1){
					tieBreakers[tieBreakersIndex] = irank_to_erank(i);
					tieBreakersIndex++;
				}
			}
		}
	}
}

bool Poker::poker_rank(const Hand & hand1, const Hand & hand2)
{
	HAND_TIER tier1 = nullTier;
	HAND_TIER tier2 = nullTier;
	vector<Card::_RANK> tieBreakers1(HAND_SIZE);
	vector<Card::_RANK> tieBreakers2(HAND_SIZE);
	for(int i = 0; i < HAND_SIZE; i++){
		tieBreakers1[i] = Card::nullRank;
		tieBreakers2[i] = Card::nullRank;
	}
	assessHandValue(hand1, tier1, tieBreakers1);
	assessHandValue(hand2, tier2, tieBreakers2);

	if(tier1 < tier2){
		return true;
	}
	else if(tier1 > tier2){
		return false;
	}
	else{
		for(int i = 0; i < (int)tieBreakers1.size(); i++){
			if(tieBreakers1[i] < tieBreakers2[i])
				return true;
			else if(tieBreakers1[i] > tieBreakers2[i])
				return false;
		}
		//the hands are equal
		return false;
	}
}


bool Poker::poker_rank_players(const Player * p1, const Player * p2){
	if( p1 == 0 || p2 == 0){
		throw Null_Player; 
	}
	else{
		return poker_rank(p1->_hand, p2->_hand);
	}
}


bool Poker::is_tied_players(const Player * p1, const Player * p2)
{

	if( p1 == 0 || p2 == 0){
		throw Null_Player;
	}
	else{
			
		HAND_TIER tier1 = nullTier;
		HAND_TIER tier2 = nullTier;
		vector<Card::_RANK> tieBreakers1(HAND_SIZE);
		vector<Card::_RANK> tieBreakers2(HAND_SIZE);
		for(int i = 0; i < HAND_SIZE; i++){
			tieBreakers1[i] = Card::nullRank;
			tieBreakers2[i] = Card::nullRank;
		}
		assessHandValue(p1->_hand, tier1, tieBreakers1);
		assessHandValue(p2->_hand, tier2, tieBreakers2);

		if(tier1 < tier2){
			return false;
		}
		else if(tier1 > tier2){
			return false;
		}
		else{
			for(int i = 0; i < (int)tieBreakers1.size(); i++){
				if(tieBreakers1[i] < tieBreakers2[i])
					return false;
				else if(tieBreakers1[i] > tieBreakers2[i])
					return false;
			}
			//the hands are equal
			return true;
		}
	}
}

Poker::HAND_TIER Poker::get_tier(Hand hand)
{
	
	HAND_TIER tier = Poker::nullTier;
	vector<Card::_RANK> tieBreakers;
	for(int i = 0; i < HAND_SIZE; i++){
		tieBreakers.push_back(Card::nullRank);
	}
	
	assessHandValue(hand, tier, tieBreakers);
	
	return tier;
}

vector<Card::_RANK> Poker::get_tiebreakers(Hand hand)
{
	
	HAND_TIER tier = Poker::nullTier;
	vector<Card::_RANK> tieBreakers;
	for(int i = 0; i < HAND_SIZE; i++){
		tieBreakers.push_back(Card::nullRank);
	}
	assessHandValue(hand, tier, tieBreakers);
	return tieBreakers;
}