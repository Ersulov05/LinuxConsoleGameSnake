#include <utility>
#include <vector>

class Apple
{
private:
    const char APPLE_SYMBOL = '#';
    std::pair<int, int> coords;
public:
    Apple();
    ~Apple();
    std::pair<int, int> getCoords();
    void setCoords(std::pair<int, int> newCoords);
    void draw(std::vector<std::vector<char>> &field);
};