#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <unistd.h>
#include <ncurses.h>
#include "Config.h"
#include "Snake.cpp"
#include "Apple.cpp"

void initField(std::vector<std::vector<char>> &field)
{
    field = EMPTY_FIELD;//std::vector<std::vector<char>>(NUMBER_OF_FIELD_ROWS, std::vector<char>(NUMBER_OF_FIELD_COLUMNS, EMPTY_CELL_SYMBOL));
}

void drawField(std::vector<std::vector<char>> &field)
{
    for (int i = 0; i < NUMBER_OF_FIELD_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_OF_FIELD_COLUMNS; j++)
        {
            printw("%c ", field[i][j]);
        }
        printw("\n");
    }
}

bool checkCollisionOfHeadSnakeAndWall(Snake &snake, Config &config)
{
    std::pair<int, int> headCoords = snake.getHead();
    if (headCoords.first < 0 || headCoords.first > NUMBER_OF_FIELD_COLUMNS - 1 ||
        headCoords.second < 0 || headCoords.second > NUMBER_OF_FIELD_ROWS - 1)
    {
        if (config.collisionWithWallsCutTail) 
        {
            if (snake.getSize() == 2) return true;
            snake.moveTail();
        }
    
        if (config.collisionWithWalls)
        {
            if (!config.collisionWithWallsHaveBehaviour) return true;
            if (config.collisionWithWallsSwartBehaviour)
            {
                snake.moveHeadRollback();
                snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 1) % 4));
                snake.moveHead();
                headCoords = snake.getHead();
                if (!(headCoords.first < 0 || headCoords.first > NUMBER_OF_FIELD_COLUMNS - 1 ||
                    headCoords.second < 0 || headCoords.second > NUMBER_OF_FIELD_ROWS - 1 || 
                    snake.findElement(snake.getHead(), false)))
                {
                    return false;
                }
                snake.moveHeadRollback();
                snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 1) % 4));
                snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 1) % 4));
                snake.moveHead();
                headCoords = snake.getHead();
                if (headCoords.first < 0 || headCoords.first > NUMBER_OF_FIELD_COLUMNS - 1 ||
                    headCoords.second < 0 || headCoords.second > NUMBER_OF_FIELD_ROWS - 1 || 
                    snake.findElement(snake.getHead(), false))
                {
                    return true;
                }
                return false;
            }
            else
            {
                snake.moveHeadRollback();
                snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 3) % 4));
                snake.moveHead();
                headCoords = snake.getHead();
                if (headCoords.first < 0 || headCoords.first > NUMBER_OF_FIELD_COLUMNS - 1 ||
                    headCoords.second < 0 || headCoords.second > NUMBER_OF_FIELD_ROWS - 1)
                {
                    return true;
                }
                return false;
            }
        }
        else
        {
            if (headCoords.first < 0) 
                snake.setHead(std::pair<int, int>(NUMBER_OF_FIELD_COLUMNS - 1, headCoords.second));
            if (headCoords.second < 0) 
                snake.setHead(std::pair<int, int>(headCoords.first, NUMBER_OF_FIELD_ROWS - 1));
            if (headCoords.first > NUMBER_OF_FIELD_COLUMNS - 1) 
                snake.setHead(std::pair<int, int>(0, headCoords.second));
            if (headCoords.second > NUMBER_OF_FIELD_ROWS - 1) 
                snake.setHead(std::pair<int, int>(headCoords.first, 0));
            return false;
        }
        return true;
    }
    return false;
}

bool checkCollisionOfHeadSnakeAndApple(Snake &snake,Apple &apple)
{
    bool collision = snake.getHead() == apple.getCoords();
    return collision;
}

std::pair<int, int> getNewPosAppleConsideringPositionSnake(Snake &snake)
{
    int snakeSize = snake.getSize();
    int fieldSize = NUMBER_OF_FIELD_COLUMNS * NUMBER_OF_FIELD_ROWS;
    int numberOfAvailablePositions = fieldSize - snakeSize;
    std::random_device rd;
    std::mt19937 gen(rd());  
    std::uniform_int_distribution<> distr1(0, NUMBER_OF_FIELD_COLUMNS - 1);
    std::uniform_int_distribution<> distr2(0, NUMBER_OF_FIELD_ROWS - 1);
    int randomNumberX = distr1(gen);
    int randomNumberY = distr2(gen);
    std::pair<int, int> newPos(randomNumberX, randomNumberY);

    while (snake.findElement(newPos))
    {
        if (++newPos.first == NUMBER_OF_FIELD_COLUMNS)
        {
            newPos.first = 0;
            newPos.second++;
        }
        if (newPos.second == NUMBER_OF_FIELD_ROWS) newPos = std::pair<int, int>(0, 0);
    }
    return newPos;
}

bool GameLoop(Config config)
{
    initscr();             // Инициализация ncurses
    cbreak();              // Отключение буферизации ввода
    noecho();              // Отключение отображения нажатых клавиш
    nodelay(stdscr, TRUE); // Устанавливаем нон-блокирующий режим
    keypad(stdscr, TRUE); // Включаем поддержку функциональных клавиш и стрелок
    std::vector<std::vector<char>> field;
    Snake snake;
    Apple apple;
    apple.setCoords(getNewPosAppleConsideringPositionSnake(snake));
    bool isWin = false;
    while (true)
    {
        auto start = std::chrono::high_resolution_clock::now();
        clear();
        initField(field);
        int key = getch(); // Проверяем, была ли нажата клавиша
        
        switch (key) {
            case KEY_UP:
                snake.rotate(UP);
                break;
            case KEY_DOWN:
                snake.rotate(DOWN);
                break;
            case KEY_LEFT:
                snake.rotate(LEFT);
                break;
            case KEY_RIGHT:
                snake.rotate(RIGHT);
                break;
            default:
                break;
        }
        snake.moveHead();
        if (checkCollisionOfHeadSnakeAndApple(snake, apple))
        {
            if (snake.getSize() == NUMBER_OF_FIELD_COLUMNS * NUMBER_OF_FIELD_ROWS) 
            {
                isWin = true;
                break;
            }
            apple.setCoords(getNewPosAppleConsideringPositionSnake(snake));
        } 
        else
        {
            snake.moveTail();
        }
        if (checkCollisionOfHeadSnakeAndWall(snake, config) || snake.findElement(snake.getHead(), false)) break;
        apple.draw(field);
        snake.draw(field);
        drawField(field);
        refresh();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        if (elapsed.count() < ONE_SECOND_IN_MILLISECONDS/FPS)
        {
            usleep(static_cast<useconds_t>((ONE_SECOND_IN_MILLISECONDS/FPS - elapsed.count()) * ONE_SECOND_IN_MILLISECONDS));
        }
    }
    endwin();
    return isWin;
}

int main()
{
    Config config;
    (GameLoop(config))
        ? std::cout << "you win\n"
        : std::cout << "you lose\n";
    return 0;
}