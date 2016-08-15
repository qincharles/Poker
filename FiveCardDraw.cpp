#include "stdafx.h"
#include "FiveCardDraw.h"

using namespace std;

FiveCardDraw::FiveCardDraw(){ 
	dealer = 0;
	_deck =  Deck(1); //creates all 52 cards in _deck
	_trash = Deck();
}

//tokenizes input strings and pushes them back into a vector of strings, returns the vector
// a ' ' is used as a deliminator
vector<string> FiveCardDraw::tokenize(string input){
	string token;
	istringstream iss(input);
	vector<string> vs;
	while ( getline(iss, token, ' ') ) //gets the whole line and splits up tokens by a ' ' (space)
	{
		vs.push_back(token);
	}
	return vs ;
}

//Shows user their hand, and prompts user which cards they would like to discard
//Tokenizes the input and reads input ints, puts the valid ints into a vector
//Valid inputs = '1' '2' '3' '4' '5' (with spaces in between)
vector<int> FiveCardDraw::discardPrompt(Player & p){
	cout << "Your hand is: " << endl;
	cout << p._hand << endl;

	cout << "What cards do you want to discard?" << endl;
	string input;
	getline(cin, input); 

	vector<string> vs = tokenize(input);

	vector<int> vi;
	for(unsigned int i =0; i < vs.size(); i++){ //iterates through vector of strings
		int temp = atoi(vs[i].c_str()); //converts strings to ints
		if(0 < temp  && temp < 6){
			vi.push_back(temp);
		}
		else{ //if the input is invalid, tell user and prompt again.
			cout << "Invalid input! Please type the card numbers you want to discard again" << endl
				<< "e.g. '1 3 5'" << endl;
			discardPrompt(p);
		}
	}
	return vi;
}

//Tells whose turn it is, shows discard prompt, removes ints from vector of cards
int FiveCardDraw::before_turn(Player & p){
	cout << "~~~~~~~~~~~ " << p._name << " ~~~~~~~~~~~" << endl;
	cout << "It is your turn: " << p._name << endl;
	vector<int> vi = discardPrompt(p);
	//sort discarding ints in decreasing order
	sort(vi.begin(), vi.end(), greater<int>()); //this is so the vector is deleted properly, so the lower indices of the vector will not change
	//remove duplicates
	vi.erase( unique(vi.begin(), vi.end()), vi.end());
	//now remove the cards at these indices from hand
	for(int i = 0; i < (int)vi.size(); i++){
		int card_index = vi[i] - 1;
		_trash.add_card(p._hand[card_index]);
		p._hand.remove_card(card_index);
	}
	return Success;
}

//refills the hand to 5 cards
int FiveCardDraw::turn(Player & p){
	
	while(p._hand.size() != HAND_SIZE){
		try{
			p._hand << _deck;
		}
		catch(ERRORS & e){
			cout << "Error caught" << endl;

			if(e == EMPTY){
				cout << "main deck empty" << endl;
				_trash.shuffle(100); //shuffling a 100 times
				p._hand << _trash;

			}

		}
	}
	return Success;

}

//prints a player hand
int FiveCardDraw::after_turn(Player & p){
	cout << p._name<< ": Your hand is: " << endl;
	try{
		cout << p._hand << endl;
	}
	catch(...){
		return No_Hand;
	}
	return Success;
}

//Deals 5 cards to each player hand, and calls before_turn() on each player on each player in the correct order
int FiveCardDraw::before_round(){
	cout << "~~~~~~~~~~~ Round " << Game::round_num << " ~~~~~~~~~~~" << endl;
	_deck.shuffle(100);
	size_t current = dealer + 1;
	size_t end = _players.size() - 1;
	size_t begin = 0;

	cout << "Dealer is " << _players[dealer]->_name << endl;
	try{
	for(int i = 0; i < HAND_SIZE; i++){
		//deal 5 cards to all players
		while(current <= end) {
			_players[current]->_hand << _deck;
			current++;
		}
		current = 0;
		while(current <= dealer){
			_players[current]->_hand << _deck;
			current++;
		}
	}
	}
	catch(...){
		cout << "There are too many players in the game. Please try again with 1 to 10 players." << endl;
		throw Too_many_players;
	}

	//call before_turn for each player
	current = dealer +1;
	while(current <= end) { //from dealer+1 to the end of the vector
		int success = before_turn(*_players[current]);
		current++;
	}
	current = 0;
	while(current <= dealer){ //from begining of vector to the dealer, dealer is dealt last
		int success = before_turn(*_players[current]);
		if(success != 0)
		{
			return success;
		}
		current++;
	}
	return Success;
}

//Calls turn on each player in the correct order
int FiveCardDraw::round(){
	size_t current = dealer + 1;
	size_t end = _players.size() - 1;
	size_t begin = 0;
	while(current <= end) {//from dealer+1 to the end of the vector
		int bool1 = turn(*_players[current]);
		if(bool1 != 0){
			return bool1;
		}
		int bool2 = after_turn(*_players[current]);
		if(bool2 != 0){
			return bool2;
		}
		current++;
	}
	current = 0;
	while(current <= dealer){ //from begining of vector to the dealer, dealer is dealt last
		int bool1 = turn(*_players[current]);
		if(bool1 != 0){
			return bool1;
		}
		int bool2 = after_turn(*_players[current]);
		if(bool2 != 0){
			return bool2;
		}
		current++;
	}
	return Success;
}

//Defines which users won and lost the round
// puts all the hands into the trash deck
// puts the trash deck back into the deck, throw errors if this doesn't work properly
int FiveCardDraw::after_round(){ 
	cout << "~~~~~~~~~~~ After Round " << round_num << " ~~~~~~~~~~~" << endl;
	round_num++;

	vector<Player *> temp = _players;
	sort(temp.begin(), temp.end(), Poker::poker_rank_players); //sorts the players by their hand using poker_rank_players, highest player at the end

	Player * winner = temp.back(); //sets the winner to the last element of the vector 

	for(int i = (int)_players.size() - 1; i >= 0; i--){
		if(Poker::is_tied_players(_players[i], winner)){ //accounts for players who tie
			_players[i]->won++;								//sets the winner's wins +1
			cout << _players[i]->_name << " WON!" << endl;
			cout << "With hand: " << _players[i]->_hand << endl;
			Poker::HAND_TIER tier = Poker::get_tier(_players[i]->_hand);
			cout << "With tier: " << Poker::tier_to_string(tier) << endl;
		}
		else{
			_players[i]->lost++;						//sets losers loss + 1
			cout << _players[i]->_name << " LOST!" << endl;
			cout << "With hand: " << _players[i]->_hand << endl;
			Poker::HAND_TIER tier = Poker::get_tier(_players[i]->_hand);
			cout << "With tier: " << Poker::tier_to_string(tier) << endl;
		}
	}
	
	for(int i = (int)_players.size()-1; i >= 0; i--){
		_trash << _players[i]->_hand; 
	}

	_deck << _trash;
	
	if(_deck.size() != 52 ){  
		cout << "Deck does not have 52 Cards." << endl;
		cout << "deck size = " << _deck.size() << endl;
		cout << "trash size = " << _trash.size() << endl;
		throw Deck_is_not_Full;
	}

	if(_trash.size() != 0){
		cout << "Trash is not Emptied" << endl;
		throw Trash_is_not_Emptied;
	}
	
	//prints out wins-loss records of all players
	cout << endl; 
	cout << "Here are the win-loss records:" << endl;
	print_players();
	cout << endl;

	players_leaving_game();

	players_joining_game();
	
	//increments dealer
	if( dealer < _players.size()){
		dealer++;
	}

	else{
		dealer = 0;
	}
	
	return Success;
}

//Prompts if any player wants to leve the game
void FiveCardDraw::players_leaving_game(){

	cout << "Does any player want to leave the game?" << endl;
	cout << "If so, please type your name or names. If not, type no." << endl;
	string input;
	getline(cin, input);

	vector<string> vs = tokenize(input);
	string temp = vs[0];
	transform(temp.begin(), temp.end(), temp.begin(), ::toupper);

	if ( temp == NO_TOKEN ){ 
		//do nothing. Done!
	}
	else {

		//this loop removes the players that were inputted, as long as they were in the game
		for(unsigned int i = 0; i < vs.size(); i++){ 

			Player * leaving = find_player(vs[i]); 


			if ( leaving == 0 ){
				cout << "Invalid Player name. Name doesn't exist. Valid Player names are:" << endl;
				print_players();
			}
			else{
				print_leaving_players(leaving);


				_game->leave_player(leaving);

				//delete on the players pointer address
				delete leaving; 
				leaving = NULL;

			}
		}
		if( _players.size() != 0){ //if there are no players, don't reprompt 
			players_leaving_game();
		}
	}

}

//prints leaving players info to a file for return usage
void FiveCardDraw::print_leaving_players ( Player * leaving ){

	ofstream file (leaving->_name + ".txt");
	if (file.is_open())
	{
		file << leaving->_name << " " << leaving->won << " " << leaving->lost << endl;
		file.close();
	}
	else cout << "Unable to open file" << endl;

}

//Prompts if any player wants to join the game
void FiveCardDraw::players_joining_game( ){
	cout << "Does anyone want to join the game?" << endl;
	cout << "If so, please type your name or names. If not, type 'no'." << endl;
	string input;
	getline(cin, input);

	vector<string> vs = tokenize(input);
	string temp = vs[0];
	transform(temp.begin(), temp.end(), temp.begin(), ::toupper);

	if ( temp == NO_TOKEN ){ 
		//do nothing. Done! ; ^ )
	}
	else {
		for(unsigned int i = 0; i < vs.size(); i++){

			try{
				_game->add_player(vs[i]);

			}
			catch(ERRORS & e){
				if( e = Player_name_already_exists){
					cout << "Player name is already in use." << endl;
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
		players_joining_game();	
	}


}