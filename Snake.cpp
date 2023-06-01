#include "Snake.hpp"

// compilation without cmake: g++ main.cpp
// to run: <exec_name> <board_height> <board_width> <game_speed>

SnakeBoard::SnakeBoard(int height, int width) : m_height{height}, m_width{width}
{
    board.resize(m_height, std::vector<TileContent>(m_width));
    for (int x = 0; x < m_height; ++x)
    {
        for (int y = 0; y < m_width; ++y)
        {
            board[x][y] = TileContent::Empty;
        }
    }
    createSnake(board, notEmpty);
    fillBoard(board, notEmpty);
}

SnakeBoard::~SnakeBoard()
{
}

void SnakeBoard::createSnake(std::vector<std::vector<TileContent>> &board, std::vector<int> &notEmpty)
{
    int center_y{getBoardHeight() / 2}; // head on center or on the closest lower right position to center
    int center_x{getBoardWidth() / 2};
    board[center_y][center_x] = TileContent::HeadUp;
    board[center_y + 1][center_x] = TileContent::VerticalBody; // adding two body blocks
    board[center_y + 2][center_x] = TileContent::VerticalBody;
    notEmpty.push_back(center_y * getBoardWidth() + center_x); // pushing to the vector
    notEmpty.push_back((center_y + 1) * getBoardWidth() + center_x);
    notEmpty.push_back((center_y + 2) * getBoardWidth() + center_x);
}

void SnakeBoard::fillBoard(std::vector<std::vector<TileContent>> &board, std::vector<int> &notEmpty)
{
    for (int i = 0; i < 4; i++) // Starting ammount of apples
    {
        int row = rand() % getBoardHeight();
        int col = rand() % getBoardWidth();
        if (std::find(notEmpty.begin(), notEmpty.end(), row * getBoardWidth() + col) != notEmpty.end())
        {
            i--;
            continue;
        }
        board[row][col] = TileContent::Apple;
        notEmpty.push_back(row * getBoardWidth() + col);
    }
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
    return board[x][y];
}
