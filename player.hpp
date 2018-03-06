#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <stdlib.h>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
	Player(Side side);
	~Player();

	Move *smartHeuristic();
	Move *doMove(Move *opponentsMove, int msLeft);
	Move *randMove();
	Move *greedyMove();
    // Flag to tell if the player is running within the test_minimax context
	bool testingMinimax;

private:
	Board brd;
	Side myside;
	Side otherside;
	bool checked[8][8];

};

#endif
