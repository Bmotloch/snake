#pragma once
#ifndef SNAKEBOARD_H_
#define SNAKEBOARD_H_
#include <iostream>
#include <vector>

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

public:
    SnakeBoard(int height, int width);
    ~SnakeBoard();
    int getBoardHeight() const;
    int getBoardWidth() const;
    TileContent getTileData(int x, int y) const;
};

#endif