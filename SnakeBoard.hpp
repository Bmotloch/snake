#pragma once
#ifndef SNAKEBOARD_H_
#define SNAKEBOARD_H_
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

enum class GameState
{
    RUNNING,
    NOT_STARTED,
    FINISHED_LOSS,
    FINISHED_WIN // very unlikely
};

enum class TileContent
{
    Empty,
    VerticalBody,
    HorizontalBody,
    LowerRightBody,
    LowerLeftBody,
    UpperRightBody,
    UpperLeftBody,
    HeadUp,
    HeadDown,
    HeadLeft,
    HeadRight,
    Apple,
    Poison
};
struct Tile
{
    int x;
    int y;
    TileContent Content;
};
class SnakeBoard
{
private:
    int m_height;
    int m_width;
    std::vector<std::vector<Tile>> board;
    std::vector<int> notEmpty;
    void fillBoard(std::vector<std::vector<Tile>> &board, std::vector<int> &notEmpty);
    void createSnake(std::vector<std::vector<Tile>> &board, std::vector<int> &notEmpty);

public:
    SnakeBoard(int height, int width);
    ~SnakeBoard();
    int getBoardHeight() const;
    int getBoardWidth() const;
    TileContent getTileData(int x, int y) const;
};

#endif