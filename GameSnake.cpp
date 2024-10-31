#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <unistd.h>
#include <ncurses.h>
#include "Config.h"
#include "Snake.cpp"
#include "Apple.cpp"

void initField(FieldType &field)
{
    field = EMPTY_FIELD;
}

void drawField(FieldType &field)
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

bool checkCollisionOfHeadSnakeAndWall(Snake &snake) 
{
    std::pair<int, int> headCoords = snake.getHead();
    return headCoords.first < 0 
        || headCoords.first > NUMBER_OF_FIELD_COLUMNS - 1 
        || headCoords.second < 0 
        || headCoords.second > NUMBER_OF_FIELD_ROWS - 1;
}

void processingSimpleWallCollisionBahavior(Snake &snake) {
    snake.moveHeadRollback();
    snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 3) % 4));
    snake.moveHead();
}

void processingSmartWallCollisionBahavior(Snake &snake) {
    snake.moveHeadRollback();
    snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 1) % 4));
    snake.moveHead();
    std::pair<int, int> headCoords = snake.getHead();
    if (!checkCollisionOfHeadSnakeAndWall(snake) || snake.findElement(snake.getHead(), false)) return;
    snake.moveHeadRollback();
    snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 1) % 4));
    snake.rotate(static_cast<Direction>((snake.getCurrentDirection() + 1) % 4));
    snake.moveHead();
}

void processingOfPassageThroughTheWall(Snake &snake)
{
    std::pair<int, int> headCoords = snake.getHead();
    if (headCoords.first < 0) 
        snake.setHead(std::pair<int, int>(NUMBER_OF_FIELD_COLUMNS - 1, headCoords.second));
    if (headCoords.second < 0) 
        snake.setHead(std::pair<int, int>(headCoords.first, NUMBER_OF_FIELD_ROWS - 1));
    if (headCoords.first > NUMBER_OF_FIELD_COLUMNS - 1) 
        snake.setHead(std::pair<int, int>(0, headCoords.second));
    if (headCoords.second > NUMBER_OF_FIELD_ROWS - 1) 
        snake.setHead(std::pair<int, int>(headCoords.first, 0));
}

void processingWallCollisionBehavior(Snake &snake, Config &config)
{
    if (config.collisionWithWallsCutTail) 
    {
        if (snake.getSize() == 2) return;
        snake.moveTail();
    }
    if (config.collisionWithWalls)
    {
        if (!config.collisionWithWallsHaveBehaviour) return;
        if (config.collisionWithWallsSwartBehaviour) processingSmartWallCollisionBahavior(snake);
        else processingSimpleWallCollisionBahavior(snake);
    } 
    else processingOfPassageThroughTheWall(snake);
}

bool checkCollisionOfHeadSnakeAndApple(Snake &snake,Apple &apple)
{
    bool collision = snake.getHead() == apple.getCoords();
    return collision;
}

int generateRandomValue(int begin, int end)
{
    std::random_device rd;
    std::mt19937 gen(rd());  
    std::uniform_int_distribution<> distr(begin, end);
    return distr(gen);
}

std::pair<int, int> getNewPosAppleConsideringPositionSnake(Snake &snake)
{
    int snakeSize = snake.getSize();
    int fieldSize = NUMBER_OF_FIELD_COLUMNS * NUMBER_OF_FIELD_ROWS;
    int numberOfAvailablePositions = fieldSize - snakeSize;
    int randomNumberX = generateRandomValue(0, NUMBER_OF_FIELD_COLUMNS - 1);
    int randomNumberY = generateRandomValue(0, NUMBER_OF_FIELD_ROWS - 1);
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

void initConsoleSettingsWithKeyboard()
{
    initscr();             // Инициализация ncurses
    cbreak();              // Отключение буферизации ввода
    noecho();              // Отключение отображения нажатых клавиш
    nodelay(stdscr, TRUE); // Устанавливаем нон-блокирующий режим
    keypad(stdscr, TRUE); // Включаем поддержку функциональных клавиш и стрелок
}

void delay(double delayMicro, double pastDelayMicro = 0) 
{
    if (pastDelayMicro < delayMicro)
    {
        usleep((delayMicro - pastDelayMicro));
    }
}

void keyPressHandler(Snake &snake)
{
    int key = getch();
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
}

bool checkWin(Snake snake) 
{
    return snake.getSize() == NUMBER_OF_FIELD_COLUMNS * NUMBER_OF_FIELD_ROWS;
}

void gameDraw(FieldType &field, Snake &snake, Apple &apple) 
{
    clear();
    initField(field);
    apple.draw(field);
    snake.draw(field);
    drawField(field);
    refresh();
}

bool gameLoop(Config config)
{
    initConsoleSettingsWithKeyboard();
    FieldType field;
    Snake snake(NUMBER_OF_FIELD_ROWS);
    Apple apple;
    apple.setCoords(getNewPosAppleConsideringPositionSnake(snake));
    while (true)
    {
        auto start = std::chrono::high_resolution_clock::now();
        keyPressHandler(snake);
        snake.moveHead();
        if (checkCollisionOfHeadSnakeAndApple(snake, apple))
        {
            if (checkWin(snake)) 
            {
                return true;
            }
            apple.setCoords(getNewPosAppleConsideringPositionSnake(snake));
        } 
        else snake.moveTail();

        if (checkCollisionOfHeadSnakeAndWall(snake)) {
            processingWallCollisionBehavior(snake, config);
        }
        if (checkCollisionOfHeadSnakeAndWall(snake) || snake.findElement(snake.getHead(), false)) return false;

        gameDraw(field, snake, apple);
        delay(ONE_SECOND_IN_MICROSECONDS/FPS, (std::chrono::high_resolution_clock::now() - start).count()/ONE_SECOND_IN_MILLISECONDS);
    }
    return false;
}

int main()
{
    Config config(false, true);
    bool isWin = gameLoop(config);
    endwin();
    (isWin)
        ? std::cout << "you win\n"
        : std::cout << "you lose\n";
    return 0;
}