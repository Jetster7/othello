#include "player.hpp"
#define AI 3
//Small change made to player.cpp by Jethin Gowda
//Matthew was here!!!!


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.

    testingMinimax = true;
    brd = new Board();

    cerr << "My color is (0: white  1:black): " << side << endl;

    myside = side;

    if(myside == BLACK)
    {
        otherside = WHITE;
    }
    else
    {
        otherside = BLACK;
    }

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

    //processing opponenet's move

    //processing my move

    brd->doMove(opponentsMove, otherside);


    if (brd->isDone() || !brd->hasMoves(myside))
    {
        return nullptr;
    }

    if(AI == 0)
    {
        return randMove();
    }
    else if(AI == 1)
    {
        return greedyMove(brd, myside, otherside);
    }
    else if(AI == 2)
    {
        return smartHeuristic();
    }

    else if (AI == 3)
    {
        return minimax();
    }

}

Move *Player::randMove()
{
    while(true)
    {
        int x = rand() % 8;
        int y = rand() % 8;
        Move *m = new Move(x, y);
        if (brd->checkMove(m, myside))
        {
            brd->doMove(m, myside);
            return m;
        }

    }
}

Move *Player::greedyMove(Board *board, Side side1, Side side2)
{
    int score = board->count(side1) - board->count(side2);
    Move *move = nullptr;
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            Move *m = new Move(x,y);
            if (board->checkMove(m, side1))
            {
                Board *tempbrd = board->copy();
                tempbrd->doMove(m, side1);
                int tempscore = tempbrd->count(side1) - tempbrd->count(side2);
                if(tempscore > score)
                {
                    score = tempscore;
                    move = m;
                }
                delete tempbrd;
            }
        }
    }
    board->doMove(move, side1);
    return move;
}

Move *Player::smartHeuristic()
{
    int score = -999;
    bool xcorn = false;
    bool ycorn = false;
    bool xedge = false;
    bool yedge = false;
    Move *move = nullptr;
    for(int x = 0; x < 8; x++)
    {
        if(x == 1 || x == 6)
        {
            xedge = true;
            xcorn = false;
        }
        else if(x == 0 || x == 7)
        {
            xcorn = true;
            xedge = false;
        }
        else
        {
            xedge = false;
            xcorn = false;
        }

        for(int y = 0; y < 8; y++)
        {
            if(y == 1 || y == 6)
            {
                yedge = true;
                ycorn = false;
            }
            else if(y == 0 || y == 7)
            {
                ycorn = true;
                yedge = false;
            }
            else
            {
                yedge = false;
                ycorn = false;
            }

            Move *m = new Move(x,y);
            if (brd->checkMove(m, myside))
            {
                Board *tempbrd = brd->copy();
                tempbrd->doMove(m, myside);
                int tempscore = tempbrd->count(myside) - tempbrd->count(otherside);
                if(xcorn && ycorn && tempscore > 0)
                {
                    tempscore *= 3;
                }
                else if(((xedge && ycorn) || (xcorn && yedge)) && tempscore > 0)
                {
                    tempscore *= -2;
                }
                else if(xedge && yedge && tempscore > 0)
                {
                    tempscore *= -3;
                }
                else if((xedge || yedge) && tempscore > 0)
                {
                    tempscore *= 2;
                }
                if(tempscore > score)
                {
                    score = tempscore;
                    move = m;
                }
                delete tempbrd;
            }
        }
    }
    brd->doMove(move, myside);
    return move;
}


//minimax for 2-ply
Move *Player::minimax() 
{
    vector<Move*> first = validMoves(brd, myside);
    vector<int> lowest;

    for(int x = 0; x < first.size(); x++)
    {
        Board *tempbrd = brd->copy();
        tempbrd->doMove(first[x], myside);
        vector<Move*> second = validMoves(tempbrd, otherside);
        int min = 999;

        for(int y = 0; y<second.size(); y++)
        {
            Board *tempbrd1 = tempbrd->copy();
            tempbrd1->doMove(second[y], otherside);
            int score1 = tempbrd1->count(myside) - tempbrd1->count(otherside);
            if(score1 < min)
            {
                min = score1;
            }

            delete tempbrd1;

        }

        lowest.push_back(min);

        delete tempbrd;

    }

    int moveindex = 0;
    int biggest = -999;

    for(int k = 0; k < lowest.size(); k++)
    {
        if(lowest[k] > biggest)
        {
            biggest = lowest[k];
            moveindex = k;
        }


    }

    brd->doMove(first[moveindex], myside);

    return first[moveindex];


}


vector<Move*> Player::validMoves(Board* b, Side s)
{
    vector<Move*> allvMoves;

    for (int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            Move *m = new Move(x,y);
            if (b->checkMove(m, s))
            {
                allvMoves.push_back(m);
            }

        }
    }

    return allvMoves;

}

void Player::setBoard(Board* board)
{
    brd = board;
}


