#include "player.hpp"
#define AI 4
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
    else if(AI == 4)
    {
        MoveValue move = minimax_nply(brd, -1000, 1000, 0, myside);
        brd->doMove(move.move, myside);
        return move.move;
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

int Player::hscore(Board *board, Move *m)
{
    bool xcorn = false;
    bool ycorn = false;
    bool xedge = false;
    bool yedge = false;
    int x = m->getX();
    int y = m->getY();

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
    int tempscore = board->count(myside) - board->count(otherside);
    if(xcorn && ycorn)
    {
        if(tempscore > 0)
            {tempscore *= 3;}
        else
            {tempscore *= -3;}
    }
    else if(((xedge && ycorn) || (xcorn && yedge)) && tempscore > 0)
    {
        if(tempscore > 0)
            {tempscore *= -2;}
        else
            {tempscore *= 2;}
    }
    else if(xedge && yedge && tempscore > 0)
    {
        if(tempscore > 0)
            {tempscore *= -3;}
        else
            {tempscore *= 3;}
    }
    else if((xedge || yedge) && tempscore > 0)
    {
        if(tempscore > 0)
            {tempscore *= 2;}
        else
            {tempscore *= -2;}
    }


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
            vector<Move*> third = validMoves(tempbrd1, myside);
            for(int z = 0; z < third.size(); z++)
            {
                Board *tempbrd2 = tempbrd1->copy();
                tempbrd2->doMove(third[z], myside);
                vector<Move*> fourth = validMoves(tempbrd2, otherside);
                for(int w = 0; w < fourth.size(); w++)
                {
                    Board *tempbrd3 = tempbrd2->copy();
                    tempbrd3->doMove(fourth[w],otherside);
                    vector<Move*> fifth = validMoves(tempbrd3, myside);
                    for(int a = 0; a < fifth.size(); a++)
                    {
                        Board *tempbrd4 = tempbrd3->copy();
                        tempbrd4->doMove(fifth[a],myside);
                        int score1 = hscore(tempbrd4, fifth[a]);
                        if(score1 < min)
                        {
                            min = score1;
                        }
                        delete tempbrd4;
                    }
                    delete tempbrd3;
                }
                delete tempbrd2;
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

MoveValue Player::minimax_nply(Board *board, int alpha, int beta, int max_depth, Side s)
{
    vector<Move*> moves = validMoves(board, s); //set of valid moves
    bool ismax = (s == myside); //tells which side you are on
    MoveValue returnMove;
    MoveValue bestMove;

    if(max_depth == 4 || !board->hasMoves(s)) //depth meeting condition
    {
        if(ismax)
        {
            return MoveValue(score(board));
        }
        else
        {
            return MoveValue(-score(board));
        }
    }

    if(ismax)
    {
        for(int x = 0; x < moves.size(); x++)
        {
            Move *currentMove = moves[x];
            Board *tempbrd = board->copy();
            tempbrd->doMove(moves[x], myside);
            returnMove = minimax_nply(tempbrd, alpha, beta, max_depth + 1, otherside);
            delete tempbrd;
            if(returnMove.value > alpha)
            {
                alpha = returnMove.value;
                bestMove.move = currentMove;
                bestMove.value = alpha;
            }
            if(beta <= alpha)
            {
                break;
            }
        }
        return bestMove;
    }
    else
    {
        for(int x = 0; x < moves.size(); x++)
        {
            Move *currentMove = moves[x];
            Board *tempbrd = board->copy();
            tempbrd->doMove(moves[x], myside);
            returnMove = minimax_nply(tempbrd, alpha, beta, max_depth + 1, myside);
            delete tempbrd;
            if(returnMove.value < beta)
            {
                beta = returnMove.value;
                bestMove.move = currentMove;
                bestMove.value = beta;
            }
            if(beta <= alpha)
            {
                break;
            }
        }
        return bestMove;
    }
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

int Player::score(Board *board)
{
    return board->count(myside) - board->count(otherside);
}
