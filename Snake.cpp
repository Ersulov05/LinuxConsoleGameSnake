#include "Snake.h"

Snake::Snake()
{
    snake.clear();
    snake.push_front(std::pair<int, int>(0, 0));
    snake.push_front(std::pair<int, int>(1, 0));
};
Snake::~Snake() {};

std::pair<int, int> Snake::getHead()
{
    if (!snake.empty()) return snake[0];
    throw std::runtime_error("the length of the snake is zero");
};

void Snake::setHead(std::pair<int, int> coords)
{
    if (!snake.empty()) 
    {
        snake[0] = coords;
    } else 
    {
        snake.push_front(coords);
    }
};

void Snake::moveHead()
{
    std::pair<int, int> newHead;
    switch (currentDirection)
    {
        case UP:
            newHead = snake[0];
            newHead.second--;
            break;
        case DOWN:
            newHead = snake[0];
            newHead.second++;
            break;
        case LEFT:
            newHead = snake[0];
            newHead.first--;
            break;
        case RIGHT:
            newHead = snake[0];
            newHead.first++;
            break;
        default:
            break;
    }
    snake.push_front(newHead);
};

void Snake::moveHeadRollback()
{
    snake.pop_front();
};

void Snake::increase()
{
    isIncrease = true;
};

void Snake::moveTail()
{
    if (snake.empty()) throw std::runtime_error("the length of the snake is zero");
    snake.pop_back();
};

int Snake::getSize()
{
    return snake.size();
};

Direction Snake::getCurrentDirection()
{
    return currentDirection;
};

bool Snake::findElement(std::pair<int, int> element, bool head)
{
    if (head)
    {   
        return std::find(snake.begin(), snake.end(), element) != snake.end();
    }
    return std::find(std::next(snake.begin()), snake.end(), element) != snake.end();
};

void Snake::rotate(Direction direction)
{
    if ((currentDirection == LEFT || currentDirection == RIGHT) && (direction == UP || direction == DOWN) ||
        (currentDirection == UP || currentDirection == DOWN) && (direction == LEFT || direction == RIGHT))
    {
        currentDirection = direction;
    }
};

void Snake::draw(std::vector<std::vector<char>> &field)
{
    for (int i = 1; i < snake.size(); i++)
    {
        field[snake[i].second][snake[i].first] = SNAKE_BODY_SIMBOL; 
    }
    field[snake[0].second][snake[0].first] = SNAKE_HEAD_SIMBOL;
};