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
#include <list>

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
    Body,
    HeadUp,
    HeadDown,
    HeadLeft,
    HeadRight,
    Apple
};

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

struct BodyPart
{
    int x;
    int y;
    TileContent content;
};
class SnakeBoard
{
private:
    int m_width;
    int m_height;
    Direction m_snakeDirection;
    Direction m_snakePreviousDirection;
    GameState m_state;
    std::vector<std::vector<BodyPart>> m_board;
    std::vector<BodyPart> m_snake;
    void createSnake();

public:
    SnakeBoard(int width, int height);
    ~SnakeBoard();
    int getBoardHeight() const;
    int getBoardWidth() const;
    TileContent getTileData(int x, int y) const;
    GameState getGameState() const;
    int getX(int x, int y) const;
    int getY(int x, int y) const;
    Direction getSnakeDirection() const;
    void setSnakeDirection(Direction direction);
    void setGameState(GameState state);
    void addBodyPart();
    void setTileData(int x, int y, TileContent data);
    int getSnakeHeadX();
    int getSnakeHeadY();
    void addApple();
    TileContent getHeadTileContent(Direction direction);
    void eraseTail();
    void modifySnakePart(int x, int y, int newX, int newY);
    std::vector<BodyPart> &getSnake();
    void updateSnakeBody();
    bool checkForWin();
};
#endif