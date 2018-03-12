#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <stdlib.h>
#include <vector>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
	Player(Side side);
	~Player();

	Move *doMove(Move *opponentsMove, int msLeft);
	Move *randMove();
	Move *greedyMove(Board *board, Side side1, Side side2);
	Move *smartHeuristic();
	Move *minimax(int depth, int max_depth); //2-ply
	Move *minimax_nply(int depth, int max_depth); //n ply
	vector<Move*> validMoves(Board* b, Side s);
	void setBoard(Board* board);
	int score(Board *board)



    // Flag to tell if the player is running within the test_minimax context
	bool testingMinimax;

private:
	Board* brd;
	Side myside;
	Side otherside;
	

};

#endif
