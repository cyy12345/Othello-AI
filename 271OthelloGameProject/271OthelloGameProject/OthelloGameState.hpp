// OthelloGameState.hpp
//
// An OthelloGameState describes the current state of an Othello game:
// what the board looks like, what the score is, whether the game is
// over, whose turn it is, etc.
//
// Every cell on the game board is identified by a pair (x, y), where
// x selects a column and y selects a row.  Both x and y are zero-based,
// so (0, 0) is the top-left cell on the board, (width-1, height-1) is
// the bottom-right cell.

#ifndef OTHELLOGAMESTATE_HPP
#define OTHELLOGAMESTATE_HPP

#include <memory>
#include "OthelloBoard.hpp"
#include <vector>



class OthelloGameState
{
private: 
	OthelloBoard GameBoard;

	// 判断各个方向的状态，一共八个方向
    std::vector<int> cellDir(int x, int y, const OthelloCell& type, const OthelloBoard& GameBoard)
    {
		std::vector<int> xCor{ 1, 1, 0, -1, -1, -1, 0, 1 };
		std::vector<int> yCor{ 0, -1, -1, -1, 0, 1, 1, 1 };
        
        std::vector<int> dir(8,0);
        for(int i = 0; i< 8; i++)
        {
            int tx = x + xCor[i];
            int ty = y + yCor[i];
            int step =0;
            while(1)
            {
                if(!GameBoard.isValidCell(tx, ty))
                {
                    break;
                }
                if(GameBoard.cellAt(tx, ty) == OthelloCell::empty)
                {
                    break;
                }
                if(GameBoard.cellAt(tx, ty)!=type)
                {
                    step++;
                }
                else
                {
                    if(step>0)
                    {
                        dir[i] = 1;
                        break;
					}
					else {
						break;
					}
                }
                tx = tx + xCor[i];
                ty = ty + yCor[i];
            }
        }
        return dir;
    }
	//返回该步吃掉子的数量，我觉得可以放在另一个类里面去写
	void flipAllTile(int x, int y, const OthelloCell& type, const std::vector<int>& dir,OthelloBoard& GameBoard)
    {
        std::vector<int> xCor = {1,1,0,-1,-1,-1,0,1};
        std::vector<int> yCor = {0,-1,-1,-1,0,1,1,1};
        for(int i = 0; i< 8; i++)
        {
            int tx = x + xCor[i];
            int ty = y + yCor[i];
            
            if(dir[i] == 1)
            {
                while(GameBoard.cellAt(tx, ty)!=type)
                {
                    GameBoard.flipTile(tx, ty);
                    tx = tx + xCor[i];
                    ty = ty + yCor[i];
                }
            }
            
        }
    }


public:
	OthelloCell Turn;
	OthelloGameState() {}
    OthelloGameState(const OthelloGameState& GS) :GameBoard(GS.GameBoard),Turn(GS.Turn)
    {
    }
	~OthelloGameState() {}

	// board() returns a reference to a constant OthelloBoard describing
	// what's on the board in this game state.
	OthelloBoard& board() 
	{
		return GameBoard;
	}

	// blackScore() returns the black player's current score.
	//traverse the board see if it is a black dic
	int blackScore() const noexcept
	{
		int score = 0;
		for (int i = 0; i < BoardWidth; i++)
		{
			for (int j = 0; j < BoardHeight; j++)
			{
				if (GameBoard.cellAt(i, j) == OthelloCell::black)
					score++;
			}
		}
		return score;
	}

	// whiteScore() returns the white player's current score.
	int whiteScore() const noexcept
	{
		int score = 0;
		for (int i = 0; i < BoardWidth; i++)
		{
			for (int j = 0; j < BoardHeight; j++)
			{
				if (GameBoard.cellAt(i, j) == OthelloCell::white)
					score++;
			}
		}
		return score;
	}

	// isGameOver() returns true if the game is over in this game state,
	// false otherwise.
	bool isGameOver() 
	{
		// game is over in two case
		// 1:no more empty space
		// 2: no place for black or white to make a valid move
		if (GameBoard.isBoardFull())
		{
			return true;
		}
		else
		{
            for(int i =0; i< 8; i++)
            {
                for(int j = 0; j<8;j++)
                {
                    if(isValidMove(i,j))
                    {
                        return false;
                    }
                    else
                    {
                        flipTurn();
                        if(isValidMove(i,j))
                        {
                            flipTurn();
                            return false;
                        }
						flipTurn();
                    }
                }
            }
            return true;
		}
	}

	// isBlackTurn() returns true if it is currently the black player's
	// turn in this game state, false otherwise.
	bool isBlackTurn() const noexcept
    {
        if(Turn == OthelloCell::black)
            return true;
        return false;
    }

	// isWhiteTurn() returns true if it is currently the white player's
	// turn in this game state, false otherwise.
	bool isWhiteTurn() const noexcept
    {
        if(Turn == OthelloCell::white)
            return true;
        return false;
    }
    

	// isValidMove() returns true if the current player whose turn it is
	// can make a valid move at cell (x, y).
	bool isValidMove(int x, int y) 
    {
        if(!GameBoard.isValidCell(x, y))
        {
            return false;
        }
        if(GameBoard.cellAt(x,y) != OthelloCell::empty)
        {
            return false;
        }
        std::vector<int> dir;
        if(isBlackTurn())
        {
             dir = cellDir(x, y, OthelloCell::black, GameBoard);
        }
        else
        {
            dir = cellDir(x, y, OthelloCell::white, GameBoard);
        }
        for(int i = 0; i < dir.size(); i++)
        {
            if(dir[i] == 1)
                return true;
        }
        return false;
    }

	// makeMove() modifies this game state by making a move on behalf of
	// the current player at cell (x, y).  If the move is invalid, an
	// OthelloException is thrown.
	void makeMove(int x, int y)
    {
        if(!GameBoard.isValidCell(x, y))
        {
            throw OthelloException::OthelloException("in makeMove Function cell is not valid");
        }
        OthelloCell type = OthelloCell::empty;
        if(isBlackTurn())
        {
            type = OthelloCell::black;
        }
        else
        {
            type = OthelloCell::white;
        }
        std::vector<int> dir = cellDir(x, y, type, GameBoard);
        GameBoard.setCellAt(x, y, type);
        flipAllTile(x, y, type, dir,GameBoard);
    }
    void flipTurn()
    {
        if(isBlackTurn())
        {
            Turn = OthelloCell::white;
        }else if(isWhiteTurn())
        {
            Turn = OthelloCell::black;
        }
    }

	// clone() makes a copy of this game state and returns a unique_ptr
	// to it.
	std::unique_ptr<OthelloGameState> clone() {
		std::unique_ptr<OthelloGameState> s(new OthelloGameState);
		s->GameBoard = this->board();
		s->Turn = this->Turn;
		return s;
	}
};



#endif // OTHELLOGAMESTATE_HPP

