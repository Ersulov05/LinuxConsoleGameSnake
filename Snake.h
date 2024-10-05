#include <deque>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Config.h"

class Snake
{
private:
    const char SNAKE_HEAD_SIMBOL = '@'; 
    const char SNAKE_BODY_SIMBOL = '0'; 

    std::deque<std::pair<int, int>> snake;
    Direction currentDirection = RIGHT;
    bool isIncrease = false;
public:
    Snake();
    ~Snake();
    std::pair<int, int> getHead();
    void setHead(std::pair<int, int> coords);
    void moveHead();
    void moveHeadRollback();
    void increase();
    void moveTail();
    int getSize();
    Direction getCurrentDirection();
    bool findElement(std::pair<int, int> element, bool head = true);
    void rotate(Direction direction);
    void draw(std::vector<std::vector<char>> &field);
};