// OthelloGameStateFactory.hpp


#ifndef OTHELLOGAMESTATEFACTORY_HPP
#define OTHELLOGAMESTATEFACTORY_HPP

#include <memory>
#include <vector>
#include "OthelloGameState.hpp"
#include "othelloboard.hpp"



class OthelloGameStateFactory
{
public:

    // makeNewGameState() takes a width and a height and creates a new
    // Othello game board with that given width and height.  The width
    // and height must both be at least 4, or else an OthelloException
    // is thrown.
    std::unique_ptr<OthelloGameState> makeNewGameState();


    // makeGameState() takes a complete game state -- described by the
    // given parameters -- and builds an OthelloGameState object
    // with those parameters.  (This is primarily used in the Othello
    // tournament.)
    //std::unique_ptr<OthelloGameState> makeGameState(std::unique_ptr<OthelloGameState>& state,
     //std::pair<int,int> move);
};
/*
std::unique_ptr<OthelloGameState> OthelloGameStateFactory::makeGameState(std::unique_ptr<OthelloGameState>& state,
	std::pair<int, int> move) {
	std::unique_ptr<OthelloGameState> s = state->clone();

	int i = move.first; //column
	int j = move.second;  //row

	
	return s;
}
*/
std::unique_ptr<OthelloGameState> OthelloGameStateFactory::makeNewGameState() {
	std::unique_ptr<OthelloGameState> s(new OthelloGameState);
	OthelloBoard &othelloboard = s->board();
	
	for (int i = 0; i < othelloboard.width(); i++) {
		for (int j = 0; j < othelloboard.height(); j++) {
			if ((i == 3 && j == 3) || (i == 4 && j == 4)) {
				OthelloCell a = OthelloCell::white;
				othelloboard.setCellAt(i, j, a);
			}
			else  if ((i == 4 && j == 3) || (i == 3 && j == 4)) {
				OthelloCell a = OthelloCell::black;
				othelloboard.setCellAt(i, j, a);
			}
			else {
				OthelloCell a = OthelloCell::empty;
				othelloboard.setCellAt(i, j, a);
			}

		}
	}
	s->Turn = OthelloCell::black;
	return s;
}



#endif // OTHELLOGAMESTATEFACTORY_HPP

