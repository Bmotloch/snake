#include "SnakeBoard.hpp"

// compilation without cmake: g++ main.cpp
// to run: <exec_name> <board_height> <board_width> <game_speed>

SnakeBoard::SnakeBoard(int height, int width) : m_height{height}, m_width{width}
{
    board.resize(m_height, std::vector<Tile>(m_width));
    for (int x = 0; x < m_height; ++x)
    {
        for (int y = 0; y < m_width; ++y)
        {
            board[x][y].x = x;
            board[x][y].y = y;
            board[x][y].Content = TileContent::Poison;
        }
    }
}

SnakeBoard::~SnakeBoard()
{
}

int SnakeBoard::getBoardHeight() const
{
    return m_height;
};

int SnakeBoard::getBoardWidth() const
{
    return m_width;
};

TileContent SnakeBoard::getTileData(int x, int y) const
{
    return board[x][y].Content;
}