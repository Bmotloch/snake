#pragma once

#ifndef SNAKEBOARD_H_
#define SNAKEBOARD_H_

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
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

class SnakeBoard
{
private:
    int m_height;
    int m_width;
    GameState m_state;
    std::vector<std::vector<TileContent>> board;
    std::vector<int> notEmptyTiles;
    void fillBoard(std::vector<std::vector<TileContent>> &board, std::vector<int> &notEmptyTiles);
    void createSnake(std::vector<std::vector<TileContent>> &board, std::vector<int> &notEmptyTiles);

public:
    SnakeBoard(int height, int width);
    ~SnakeBoard();
    int getBoardHeight() const;
    int getBoardWidth() const;
    TileContent getTileData(int x, int y) const;
    GameState getGameState() const;
    void setGameState(GameState state);
};

#endif