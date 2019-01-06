// OthelloBoard.hpp
//
// An OthelloBoard is an object that tracks the current state of an
// Othello board.  A board is a collection of cells, each identified
// by an (x, y) position, where 0 <= x <= width-1 and 0 <= y <= height-1.


#ifndef OTHELLOBOARD_HPP
#define OTHELLOBOARD_HPP

#include <vector>
#include <iostream>
#include "OthelloCell.hpp"
#include "OthelloException.hpp"

//宽，自然是第一个里面输入的
const int BoardWidth = 8;
//高，所以是第二个里面输入的
const int BoardHeight = 8; 

class OthelloBoard
{
private:
	std::vector<std::vector<OthelloCell>> BasicBoard;

public:
	OthelloBoard(): BasicBoard(BoardWidth, std::vector<OthelloCell>(BoardHeight, OthelloCell::empty)) {}
	~OthelloBoard() {}

	// width() returns the width of this Othello board.
	int width() const noexcept { return BoardWidth;}

	// height() returns the height of this Othello board.
	int height() const noexcept { return BoardHeight;}

	// isValidCell() returns true if the given (x, y) is a valid
	// cell on this board -- i.e., if (x, y) is in the boundaries
	// of the board -- or false otherwise.
	bool isValidCell(int x, int y) const noexcept
	{
		if (x > 7 || x < 0 || y > 7 || y < 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	// cellAt() returns the contents of the cell (x, y).  The cell
	// will either be empty, black, or white.
	OthelloCell cellAt(int x, int y) const
	{
		return BasicBoard[x][y];
	}

	// setCellAt() changes the contents of the cell (x, y) to the
	// given new value (empty, black, or white).
	void setCellAt(int x, int y, OthelloCell cell)
	{
		BasicBoard[x][y] = cell;
	}

	// placeTile() places a tile on the board in cell (x, y),
	// provided that cell (x, y) is currently empty.  If the
	// cell is not currently empty, an OthelloException is
	// thrown.
	void placeTile(int x, int y, OthelloCell cell)
	{
		if (cellAt(x, y) != OthelloCell::empty)
		{
			throw OthelloException::OthelloException("Current Cell is not empty!!");
		}
		else
		{
			setCellAt(x, y, cell);
		}
	}

	// flipTile() flips the tile on the board in cell (x, y),
	// if any -- so, if it was white, it is now black; if it
	// was black, it is now white.  If the cell is not currently
	// occupied by a tile, an OthelloException is thrown.
	void flipTile(int x, int y)
	{
		if (cellAt(x, y) == OthelloCell::empty)
		{
			throw OthelloException::OthelloException("Current Cell is empty, which can not be fliped!!");
		}
		else
		{
			if (cellAt(x, y) == OthelloCell::black)
			{
				setCellAt(x, y, OthelloCell::white);
			}
			else
			{
				setCellAt(x, y, OthelloCell::black);
			}
		}
	}

	bool isBoardFull() const noexcept
	{
		for (int i = 0; i < BoardWidth; i++)
		{
			for (int j = 0; j < BoardHeight; j++)
			{
				if (cellAt(i, j) == OthelloCell::empty)
				{
					return false;
				}
			}
		}
		return true;
	}

	void printBoard()
	{
		std::cout<< "Black is X, White is O, Empty is . " << std::endl;

		std::vector<std::vector<char>> trans(8, std::vector<char>(8, 'e'));
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (BasicBoard[i][j] == OthelloCell::black)
				{
					trans[i][j] = 'b';
				}
				if (BasicBoard[i][j] == OthelloCell::white)
				{
					trans[i][j] = 'w';
				}
			}
		}

		for (int i = 0; i < 19; i++)
		{
			for (int j = 0; j < 19; j++)
			{
				if (i % 2 == 0)
				{
					if (j % 2 == 0)
					{
						std::cout << "+";
					}
					else
					{
						std::cout << "---";
					}
				}
				else
				{
					if (j % 2 == 0)  //所有偶数列都填上东西了
					{
						std::cout << "| ";
					}
					else 
					{
						if (i == 1)//第一行是标题行
						{
							if (j == 1)
							{
								std::cout << "  ";
							}
							else
							{
								std::cout<< j / 2 << " ";
							}
						}
						else//除了第一行开始
						{
							if (j == 1)//第一列的情况
							{
								std::cout<< i / 2 << " ";
							}
							else
							{
								if (trans[i / 2 - 1][j / 2 - 1] == 'b')
								{
									std::cout << "X ";
								}
								else if (trans[i / 2 - 1][j / 2 - 1] == 'w')
								{
									std::cout << "O ";
								}
								else
								{
									std::cout <<". ";
								}
								

							}
							
						}

						
					}
				}
			}
			std::cout << std::endl;
		}



	}
};



#endif // OTHELLOBOARD_HPP