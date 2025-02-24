// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "OthelloBoard.hpp"
#include "OthelloCell.hpp"
#include "OthelloGameState.hpp"
#include "OthelloGameStateFactory.hpp"
#include "OthelloAIcyy.hpp"
#include <memory>
#include <iostream>

using namespace std;

void printBoard(OthelloBoard& board);
int main(int argc, char** argv)
{
	int height = 8;
	int width = 8;
	OthelloGameStateFactory statefactory;
	OthelloAIcyy ai;
	std::pair<int, int> aimove;
	int gamecount = 1;
	int blackwin = 0;
	int whitewin = 0;
	int draw = 0;
	std::cout << "Choose black or white. Please input 'b' or 'w'" << std::endl;
	char choice;
	std::cin >> choice;

	//输出board行数和列数 
	//std::cout << "Row:" << height << "  Column:" << width << std::endl;

	//开始游戏，产生初始state，board， 需要定义AI是黑棋还是白棋
	//如果是黑棋，则AI先下，如果是白棋，则等人输入一个坐标
	//OthelloGameState state;


	std::unique_ptr<OthelloGameState> state = statefactory.makeNewGameState();
	printBoard(state->board());
	int blackscore = 0;
	int whitescore = 0;

	while (!state->isGameOver()) {
		if (choice == 'b') {//玩家选择黑，则AI为白
			std::vector<std::pair<int, int>> possibilities = ai.moves(state);
			if (possibilities.size() == 0) {
				state->flipTurn();
				std::cout << "There is no valid move for you." << endl;
				continue;
			}
			std::cout << "Please input your move as a coordinate" << std::endl;
			int a, b;
			std::cin >> a >> b;
			
			while (!state->isValidMove(a, b)) {
				std::cout << "The place you decided to move is: (" << a << ", " << b << ")" << std::endl;
				std::cout << "The move is not valid. Please input again." << std::endl;
				std::cin >> a >> b;
			}
			std::cout << "The place you decided to move is: (" << a << ", " << b << ")" << std::endl;
			state->makeMove(a, b);
			state->flipTurn();
			printBoard(state->board());
			ai.Iamblack = true;
			aimove = ai.chooseMove(*state);
			if (aimove.first == -1) {
				state->flipTurn();
				std::cout << "There is no valid move." << endl;
			}
			else {
				state->makeMove(aimove.first, aimove.second);
				state->flipTurn();
			}

			printBoard(state->board());
		}
		else if (choice == 'w') {
			ai.Iamblack = true;
			aimove = ai.chooseMove(*state);
			if (aimove.first == -1) {
				state->flipTurn();
				std::cout << "There is no valid move." << endl;
			}
			else {
				std::cout << aimove.first << "  " << aimove.second << std::endl;
				state->makeMove(aimove.first, aimove.second);
				state->flipTurn();
			}
			printBoard(state->board());


			std::vector<std::pair<int, int>> possibilities = ai.moves(state);
			if (possibilities.size() == 0) {
				state->flipTurn();
				std::cout << "There is no valid move for you." << endl;
				continue;
			}


			std::cout << "Please input your move as a coordinate" << std::endl;
			int a, b;
			std::cin >> a >> b;
			while (!state->isValidMove(a, b)) {
				std::cout << "The place you decided to move is: (" << a << ", " << b << ")" << std::endl;
				std::cout << "The move is not valid. Please input again." << std::endl;
				std::cin >> a >> b;
			}

			state->makeMove(a, b);
			state->flipTurn();



			/*
			oppoai.Iamblack = false;
			oppoaimove = oppoai.chooseMove(*state);
			if (oppoaimove.first == -1) {
				state->flipTurn();
				std::cout << "There is no valid move." << endl;
			}
			else {
				state->makeMove(oppoaimove.first, oppoaimove.second);
				state->flipTurn();
			};*/
			printBoard(state->board());
		}
		else {
			std::cout << "wrong choice" << std::endl;
		}
	}
	std::cout << "GAME OVER" << std::endl;
	blackscore = state->blackScore();
	whitescore = state->whiteScore();
	std::cout << "black score" << blackscore << "  ";
	std::cout << "white score" << whitescore << endl;


	std::system("pause");
	return 0;
}
void printBoard(OthelloBoard& board) {
	std::cout << "   0  1  2  3  4  5  6  7" << endl;
	std::cout << "  -------------------------" << std::endl;

	for (int j = 0; j < board.height(); j++) {
		std::cout << j << "|" << " ";
		for (int k = 0; k < board.width(); k++) {
			if (board.cellAt(k, j) == OthelloCell::black) {
				std::cout << 'X' << " |";
			}
			else if (board.cellAt(k, j) == OthelloCell::white)
			{
				std::cout << 'O' << " |";
			}
			else {
				std::cout << ' ' << " |";
			}

		}
		std::cout << " " << std::endl;
		std::cout << "  -------------------------" << std::endl;

	}
}
