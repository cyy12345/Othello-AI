#include "OthelloException.hpp"
#include <iostream>
#include "OthelloBoard.hpp"
#include "OthelloGameState.hpp"
#include <string>
#include <algorithm>
#include <limits.h>
#include <ctime>

#include <vector>
#include "OthelloGameState.hpp"

#ifndef OTHELLOAICYY_HPP
#define OTHELLOAICYY_HPP

class OthelloAIcyy
{
public:

	std::pair<int, int> chooseMove(OthelloGameState& state);

	clock_t start;
	clock_t end;
	double accumulatedTime = 0;

	int DEPTH = 4;
	std::vector<int> search_values;
	int index;                //Index of the move correlated with evaluation of said move

	bool Iamblack;  


	bool myTurn(OthelloGameState s);


	int eval(std::unique_ptr<OthelloGameState>& s);





	std::vector<std::pair<int, int>> moves(std::unique_ptr<OthelloGameState>& s);

	// search using alpha beta pruning
	int search1(std::unique_ptr<OthelloGameState>& s, int depth, int alpha, int beta);
	// MIN/MAX search not using alpha beta pruning
	int search(std::unique_ptr<OthelloGameState>& s, int depth);

};


std::pair<int, int> OthelloAIcyy::chooseMove(OthelloGameState& state) { 
	start = clock();
	int depth = DEPTH;
	search_values.clear();
	int alpha = INT_MIN;
	int beta = INT_MAX;
	std::unique_ptr<OthelloGameState> s = state.clone();

	std::vector<std::pair<int, int>> possibilities = moves(s);

	if (possibilities.size() == 0) {
		return std::make_pair(-1, -1);
	}

	int search_value = search1(s, depth, alpha, beta); //using alpha beta
	//int search_value = search(s, depth); // not using alpha-beta


										 //find the index of best move in the possibilities
	index = std::find(search_values.begin(), search_values.end(), search_value) - search_values.begin();
	end = clock();
	accumulatedTime += (double)(end-start)/1000;
	std::cout << "searchtime" << accumulatedTime << std::endl;
	return possibilities[index];

	/*
	for (int i = 0; i < possibilities.size(); i++) {  //»Áπ˚’‚¿Ô√ª”–validmove‘Ú÷±Ω”Ã¯π˝

	std::unique_ptr<OthelloGameState> newBoard = s->clone();
	newBoard->makeMove(possibilities[i].first, possibilities[i].second);
	newBoard->flipTurn();
	//std::cout << s->isBlackTurn() <<  std::endl;
	//std::cout << newBoard->isBlackTurn() << std::endl;
	//std::cout << "depth" << depth << std::endl;
	//newBoard->board().printBoard();

	int search_value = search(newBoard, depth,alpha,beta);
	alpha = std::max(alpha, search_value);

	std::cout << "search value:"<<search_value << std::endl;
	if (search_value > Maximum) {
	index = i;                            //finds index of where max pair is in vector
	Maximum = search_value;
	}
	//else if(search_value<Minimum){
	//    index = i;                            //finds index of where min pair is in vector
	//
	//    Minimum = search_value;
	//}
	}*/

}


int OthelloAIcyy::eval(std::unique_ptr<OthelloGameState>& s) { //¥´»Î÷∏’Îµƒ“˝”√
	int evaluationValue = 0;

	int cornerScore = 0;
	int blackCorners = 0;
	int whiteCorners = 0;

	if (s->board().cellAt(0, 0) == OthelloCell::black) blackCorners++;
	if (s->board().cellAt(0, 7) == OthelloCell::black) blackCorners++;
	if (s->board().cellAt(7, 0) == OthelloCell::black) blackCorners++;
	if (s->board().cellAt(7, 7) == OthelloCell::black) blackCorners++;
	if (s->board().cellAt(0, 0) == OthelloCell::white) whiteCorners++;
	if (s->board().cellAt(7, 0) == OthelloCell::white) whiteCorners++;
	if (s->board().cellAt(0, 7) == OthelloCell::white) whiteCorners++;
	if (s->board().cellAt(7, 7) == OthelloCell::white) whiteCorners++;

	if (Iamblack) {
		if (blackCorners>whiteCorners) cornerScore = 1000 * (blackCorners ^ 2);
		if (blackCorners == whiteCorners) cornerScore = 0;
		else cornerScore = -1000 * (blackCorners ^ 2);
	}

	if (!Iamblack) {
		if (blackCorners>whiteCorners) cornerScore = -1000 * (blackCorners ^ 2);
		if (blackCorners == whiteCorners) cornerScore = 0;
		else cornerScore = 1000 * (blackCorners ^ 2);
	}
	/*

	int numberScore;
	if (Iamblack) {
	numberScore = 200 * (s->blackScore());
	}
	else numberScore = -200 * (s->whiteScore());


	*/

	std::vector<std::pair<int, int>> possibilities = moves(s);
	int mobility = possibilities.size();
	int mobilityScore = 0;

	if (Iamblack) {

		if (s->isWhiteTurn() && mobility == 0) mobilityScore = 500;
		if (s->isWhiteTurn() && mobility != 0) mobilityScore = -20 * mobility;
	}

	if (!Iamblack) {
		if (s->isBlackTurn() && mobility == 0) mobilityScore = 500;
		if (s->isBlackTurn() && mobility != 0) mobilityScore = -20 * mobility;
	}

	int boardScore = 0;
	int blackScore = 0;
	int whiteScore = 0;
	/*
	std::vector<std::vector<int>> boardWeights = {
	{ 200,-100, 100,  50,  50, 100,-100, 200 },
	{ -100,-300, -50, -50, -50, -50,-300,-100 },
	{ 100, -50, 100,   0,   0, 100, -50, 100 },
	{ 50, -50,  0,    0,   0,   0, -50,  50 },
	{ 50, -50,  0,    0,   0,   0, -50,  50 },
	{ 100, -50, 100,   0,   0, 100, -50, 100 },
	{ -100,-300, -50, -50, -50, -50,-300,-100 },
	{ 200,-100, 100,  50,  50, 100,-100, 200 }
	};
	std::vector<std::vector<int>> boardWeights = {
	{ 90,-60, 10,  10,  10, 10,-60, 90 },
	{ -60,-80, 5, 5, 5, 5,-80,-60 },
	{ 10, 5, 1,   1,   1, 1, 5, 10 },
	{ 10, 5,  1,    1,   1,   1, 5,  10 },
	{ 10, 5,  1,    1,   1,   1, 5,  10},
	{ 10, 5, 1,   1,   1, 1, 5, 10 },
	{ -60,-80, 5, 5, 5, 5,-80,-60 },
	{ 90,-60, 10,  10,  10, 10,-60, 90 }
	};*/
	std::vector<std::vector<int>> boardWeights = {
		{ 300,-50, 150,  150,  150, 150,-50, 300 },
	{ -50,-200, -50, -50, -50, -50,-200,-50 },
	{ 150, -50, 100,   0,   0, 100, -50, 150 },
	{ 150, -50,  0,    0,   0,   0, -50,  150 },
	{ 150, -50,  0,    0,   0,   0, -50,  150 },
	{ 150, -50, 100,   0,   0, 100, -50, 150 },
	{ -50,-200, -50, -50, -50, -50,-200,-50 },
	{ 300,-50, 150,  150,  150, 150,-50, 300 }
	};

	if (s->board().cellAt(0, 0) != OthelloCell::empty) {
		boardWeights[0][1] = 0;
		boardWeights[0][2] = 0;
		boardWeights[0][3] = 0;

		boardWeights[1][0] = 0;
		boardWeights[1][1] = 0;
		boardWeights[1][2] = 0;
		boardWeights[1][3] = 0;

		boardWeights[2][0] = 0;
		boardWeights[2][1] = 0;
		boardWeights[2][2] = 0;

		boardWeights[3][0] = 0;
		boardWeights[3][1] = 0;
	}

	if (s->board().cellAt(7, 0) != OthelloCell::empty) {
		boardWeights[0][4] = 0;
		boardWeights[0][5] = 0;
		boardWeights[0][6] = 0;

		boardWeights[1][4] = 0;
		boardWeights[1][5] = 0;
		boardWeights[1][6] = 0;
		boardWeights[1][7] = 0;

		boardWeights[2][5] = 0;
		boardWeights[2][6] = 0;
		boardWeights[2][7] = 0;

		boardWeights[3][6] = 0;
		boardWeights[3][7] = 0;
	}
	if (s->board().cellAt(0, 7) != OthelloCell::empty) {
		boardWeights[7][1] = 0;
		boardWeights[7][2] = 0;
		boardWeights[7][3] = 0;

		boardWeights[6][0] = 0;
		boardWeights[6][1] = 0;
		boardWeights[6][2] = 0;
		boardWeights[6][3] = 0;

		boardWeights[5][0] = 0;
		boardWeights[5][1] = 0;
		boardWeights[5][2] = 0;

		boardWeights[4][0] = 0;
		boardWeights[4][1] = 0;
	}
	if (s->board().cellAt(7, 7) != OthelloCell::empty) {
		boardWeights[7][4] = 0;
		boardWeights[7][5] = 0;
		boardWeights[7][6] = 0;

		boardWeights[6][4] = 0;
		boardWeights[6][5] = 0;
		boardWeights[6][6] = 0;
		boardWeights[6][7] = 0;

		boardWeights[5][5] = 0;
		boardWeights[5][6] = 0;
		boardWeights[5][7] = 0;

		boardWeights[4][6] = 0;
		boardWeights[4][7] = 0;
	}


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (s->board().cellAt(i, j) == OthelloCell::black) {
				blackScore += boardWeights[i][j];
			}
			else if (s->board().cellAt(i, j) == OthelloCell::white) {
				whiteScore += boardWeights[i][j];
			}
		}
	}
	if (Iamblack) boardScore = blackScore - whiteScore;
	else boardScore = whiteScore - blackScore;

	evaluationValue = cornerScore + boardScore + mobilityScore;
	return evaluationValue;




}


std::vector<std::pair<int, int>> OthelloAIcyy::moves(std::unique_ptr<OthelloGameState>& s) {
	int boardWidth = s->board().width();
	int boardHeight = s->board().height();


	std::vector<std::pair<int, int>> possibilities;

	for (int i = 0; i < boardWidth; i++) { //±È¿˙¡–
		for (int j = 0; j < boardHeight; j++) {//±È¿˙––
			if (s->isValidMove(i, j) && !s->isGameOver()) {
				possibilities.push_back(std::pair<int, int>(i, j));
			}
		}
	}
	return possibilities;

}

bool OthelloAIcyy::myTurn(OthelloGameState s) {//’‚¿Ô–Ë“÷µ¿Œ“ «∫⁄ªπ «∞◊£¨≥ı º◊¥Ã¨∏¯≥ˆ
	if (Iamblack) {
		if (s.isBlackTurn()) {
			return true;
		}
		else if (s.isWhiteTurn()) {
			return false;
		}
	}
	else {
		if (s.isBlackTurn()) {
			return false;
		}
		else if (s.isWhiteTurn()) {
			return true;
		}
	}
	return true;
}
//not using alpha-beta pruning
int OthelloAIcyy::search(std::unique_ptr<OthelloGameState>& s, int depth) {

	int max = INT_MIN;
	int min = INT_MAX;
	if (depth == 0) {
		return eval(s);
	}

	else {
		std::vector<std::pair<int, int>> possibilities = moves(s);

		if (possibilities.size() == 0 && s->isGameOver()) {
			s->flipTurn();
			possibilities = moves(s);
		}
		if (myTurn(*s)) {

			for (int i = 0; i < possibilities.size(); i++) {
				std::unique_ptr<OthelloGameState> boardCopy = s->clone();

				boardCopy->makeMove(possibilities[i].first, possibilities[i].second);
				boardCopy->flipTurn();

				//boardCopy->board().printBoard();
				int search_value = search(boardCopy, depth - 1);
				if (search_value > max) {
					max = search_value;
				}



				if (depth == DEPTH) {

				}
			}

			return max;
		}
		else {

			for (int i = 0; i < possibilities.size(); i++) {
				std::unique_ptr<OthelloGameState> boardCopy = s->clone();
				boardCopy->makeMove(possibilities[i].first, possibilities[i].second);
				boardCopy->flipTurn();

				//boardCopy->board().printBoard();
				min = std::min(min, search(boardCopy, depth - 1));


			}
			if (depth == DEPTH - 1) {
				search_values.push_back(min);
			}
			return min;
		}
	}

}
// using alpha-beta pruning
int OthelloAIcyy::search1(std::unique_ptr<OthelloGameState>& s, int depth, int alpha, int beta) {

	int max = INT_MIN;
	int min = INT_MAX;

	if (depth == 0) {
		return eval(s);
	}

	else {
		std::vector<std::pair<int, int>> possibilities = moves(s);
		//std::cout << "possibilities:"<<possibilities.size() << std::endl;
		if (possibilities.size() == 0 && s->isGameOver()) {
			s->flipTurn();
			possibilities = moves(s);
		}
		if (myTurn(*s)) {

			for (int i = 0; i < possibilities.size(); i++) {
				std::unique_ptr<OthelloGameState> boardCopy = s->clone();

				boardCopy->makeMove(possibilities[i].first, possibilities[i].second);
				boardCopy->flipTurn();

				//boardCopy->board().printBoard();

				max = std::max(max, search1(boardCopy, depth - 1, alpha, beta));

				alpha = std::max(alpha, max);
				if (alpha >= beta) {

					break;
				}
				if (depth == DEPTH) {

				}
			}
			return max;
		}
		else {

			for (int i = 0; i < possibilities.size(); i++) {
				std::unique_ptr<OthelloGameState> boardCopy = s->clone();
				boardCopy->makeMove(possibilities[i].first, possibilities[i].second);
				boardCopy->flipTurn();

				//boardCopy->board().printBoard();
				min = std::min(min, search1(boardCopy, depth - 1, alpha, beta));

				beta = std::min(beta, min);
				if (alpha >= beta) {

					break;
				}

			}
			if (depth == DEPTH - 1) {
				search_values.push_back(min);
			}
			return min;
		}
	}

}



#endif