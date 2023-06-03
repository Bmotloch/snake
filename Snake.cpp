#include "Snake.hpp"

// compilation without cmake: g++ main.cpp
// to run: <exec_name> <board_height> <board_width> <game_speed>

SnakeBoard::SnakeBoard(int height, int width) : m_height{height}, m_width{width}
{
    setGameState(GameState::NOT_STARTED);
    board.resize(m_height, std::vector<TileContent>(m_width));
    for (int x = 0; x < m_height; ++x)
    {
        for (int y = 0; y < m_width; ++y)
        {
            board[x][y] = TileContent::Empty;
        }
    }
    createSnake(board, notEmptyTiles);
    fillBoard(board, notEmptyTiles);
}

SnakeBoard::~SnakeBoard()
{
}

void SnakeBoard::createSnake(std::vector<std::vector<TileContent>> &board, std::vector<int> &notEmptyTiles)
{
    int center_y{getBoardHeight() / 2}; // head on center or on the closest lower right position to center
    int center_x{getBoardWidth() / 2};
    m_snakeHeadX = center_x;
    m_snakeHeadY = center_y;
    m_snakeDirection = Direction::Up;
    board[center_y][center_x] = TileContent::HeadUp;
    board[center_y + 1][center_x] = TileContent::VerticalBody; // adding two body blocks
    board[center_y + 2][center_x] = TileContent::VerticalBody;
    notEmptyTiles.push_back(center_y * getBoardWidth() + center_x); // pushing to the vector
    notEmptyTiles.push_back((center_y + 1) * getBoardWidth() + center_x);
    notEmptyTiles.push_back((center_y + 2) * getBoardWidth() + center_x);
}

void SnakeBoard::fillBoard(std::vector<std::vector<TileContent>> &board, std::vector<int> &notEmptyTiles)
{
    for (int i = 0; i < 4; i++) // Starting ammount of apples
    {
        int row = rand() % getBoardHeight();
        int col = rand() % getBoardWidth();
        if (std::find(notEmptyTiles.begin(), notEmptyTiles.end(), row * getBoardWidth() + col) != notEmptyTiles.end())
        {
            i--;
            continue;
        }
        board[row][col] = TileContent::Apple;
        notEmptyTiles.push_back(row * getBoardWidth() + col);
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

GameState SnakeBoard::getGameState() const
{
    return m_state;
}

void SnakeBoard::setGameState(GameState state)
{
    m_state = state;
}

TileContent SnakeBoard::getTileData(int x, int y) const
{
    return board[x][y];
}

int SnakeBoard::getSnakeHeadX() const
{
    return m_snakeHeadX;
}

int SnakeBoard::getSnakeHeadY() const
{
    return m_snakeHeadY;
}

Direction SnakeBoard::getSnakeDirection() const
{
    return m_snakeDirection;
}

void SnakeBoard::setSnakeHeadX(int x)
{
    m_snakeHeadX += x;
}

void SnakeBoard::setSnakeHeadY(int y)
{
    m_snakeHeadY += y;
}

void SnakeBoard::setSnakeDirection(Direction direction)
{
    m_snakeDirection = direction;
}

void SnakeBoard::setTileData(int x, int y, TileContent data)
{
    board[y][x] = data;
}