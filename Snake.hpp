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

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};
class SnakeBoard
{
private:
    int m_height;
    int m_width;
    int m_snakeHeadX;
    int m_snakeHeadY;
    Direction m_snakeDirection;
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
    int getSnakeHeadX() const;
    int getSnakeHeadY() const;
    Direction getSnakeDirection() const;
    void setSnakeHeadX(int x);
    void setSnakeHeadY(int y);
    void setSnakeDirection(Direction direction);
    void setGameState(GameState state);
    void addSnakePart(int x, int y);
    void setTileData(int x, int y, TileContent data);
};

#endif