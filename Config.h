#ifndef CONFIG_H
#define CONFIG_H
inline const char EMPTY_CELL_SYMBOL = '-';
inline const int NUMBER_OF_FIELD_ROWS = 15;
inline const int NUMBER_OF_FIELD_COLUMNS = 15;
inline const int SPEED = 100;
inline const float FPS = 0.1 * SPEED;
inline const int ONE_SECOND_IN_MICROSECONDS = 1000000;
inline const int ONE_SECOND_IN_MILLISECONDS = 1000;
using FieldType = std::vector<std::vector<char>>;
inline const FieldType EMPTY_FIELD(NUMBER_OF_FIELD_ROWS, std::vector<char>(NUMBER_OF_FIELD_COLUMNS, EMPTY_CELL_SYMBOL));
enum Direction {UP, RIGHT, DOWN, LEFT};
struct Config
{
    bool collisionWithWalls;
    bool collisionWithWallsCutTail;
    bool collisionWithWallsHaveBehaviour;
    bool collisionWithWallsSwartBehaviour;
    Config(bool collisionWithWalls = true, bool collisionWithWallsCutTail = false, 
        bool collisionWithWallsHaveBehaviour = false, bool collisionWithWallsSwartBehaviour = false):
        collisionWithWalls(collisionWithWalls),
        collisionWithWallsCutTail(collisionWithWallsCutTail),
        collisionWithWallsHaveBehaviour(collisionWithWallsHaveBehaviour),
        collisionWithWallsSwartBehaviour(collisionWithWallsSwartBehaviour) {};
};
#endif