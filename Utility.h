#ifndef Utility_h
#define Utility_h


static enum ERRORS {Unspecified = -1, Success = 0, Usage_Message, Invalid_File, Player_name_already_exists,
	Deck_is_not_Full, Trash_is_not_Emptied, No_Hand, Both_Decks_empty, EMPTY, Null_Player,
	instance_not_available, game_already_started, unknown_game, no_game_in_progress, 
	already_playing, Invalid_Index, Too_many_players

};

static string NO_TOKEN = "NO";

#endif