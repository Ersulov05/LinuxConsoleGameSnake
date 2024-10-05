# Игра змейка в консольном варианте на C++ (Linux)

## Установка библиотек
### ncurses
```bash
sudo apt install libncurses5-dev libncursesw5-dev
```

## Компиляция и запуск
### Скомпилировать проект
```bash
g++ GameSnake.cpp -o GameSnake -l ncurses
```
### Запустить проект
```bash
./GameSnake
```