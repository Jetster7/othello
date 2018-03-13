#ifndef __COMMON_H__
#define __COMMON_H__

enum Side { 
    WHITE, BLACK
};

class Move {

public:
    int x, y;
    Move(int x, int y) {
        this->x = x;
        this->y = y;        
    }
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

class MoveValue {

public:
    int value;
    Move * move;
    MoveValue()
    {
        this->value = 0;
        this->move = nullptr;
    }
    MoveValue(int tempvalue)
    {
        this->value = tempvalue;
        this->move = nullptr;
    }
    MoveValue(int tempvalue, Move *tempmove)
    {
        this->value = tempvalue;
        this->move = tempmove;
    }
};

#endif
