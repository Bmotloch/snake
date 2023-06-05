#include "Snake.hpp"

// compilation without cmake: g++ main.cpp
// to run: <exec_name> <board_height> <board_width> <game_speed>

SnakeBoard::SnakeBoard(int width, int height) : m_width{width}, m_height{height}
{
    setGameState(GameState::NOT_STARTED);
    setSnakeDirection(Direction::Up);
    setSnakePreviousDirection(Direction::Up);
    board.resize(m_height, std::vector<BodyPart>(m_width));
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            board[y][x].x = x;
            board[y][x].y = y;
            board[y][x].content = TileContent::Empty;
        }
    }
    createSnake(snake);
    fillBoard(board);
}

SnakeBoard::~SnakeBoard()
{
}

void SnakeBoard::createSnake(std::list<BodyPart> &snake)
{
    int center_y{getBoardHeight() / 2}; // head on center or on the closest lower right position to center
    int center_x{getBoardWidth() / 2};

    for (int i = center_y; i <= (center_y + 2); i++)
    {
        if (i == center_y)
        {
            setTileData(center_x, i, TileContent::HeadUp);
            snake.push_back({center_x, i, TileContent::HeadUp}); // pushing to the list
            continue;
        }
        else
        {
            setTileData(center_x, i, TileContent::VerticalBody);
            snake.push_back({center_x, i, TileContent::VerticalBody}); // pushing to the list}
        }
    }
}
void SnakeBoard::fillBoard(std::vector<std::vector<BodyPart>> &board)
{
    for (int i = 0; i < 4; i++) // Starting ammount of apples
    {
        int row = rand() % getBoardHeight();
        int col = rand() % getBoardWidth();
        if (board[row][col].content != TileContent::Empty)
        {
            i--;
            continue;
        }
        board[row][col].content = TileContent::Apple;
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
    return board[y][x].content;
}

int SnakeBoard::getX(int x, int y) const
{
    return board[y][x].x;
}

int SnakeBoard::getY(int x, int y) const
{
    return board[y][x].y;
}

Direction SnakeBoard::getSnakeDirection() const
{
    return m_snakeDirection;
}

void SnakeBoard::setX(int x, int y)
{
    board[y][x].x = x;
}

void SnakeBoard::setY(int x, int y)
{
    board[y][x].y = y;
}

void SnakeBoard::setSnakeDirection(Direction direction)
{
    m_snakeDirection = direction;
}

void SnakeBoard::setTileData(int x, int y, TileContent content)
{
    board[y][x].content = content;
}

int SnakeBoard::getSnakeHeadX()
{
    return snake.front().x;
}

int SnakeBoard::getSnakeHeadY()
{
    return snake.front().y;
}

TileContent SnakeBoard::getSnakeHeadContent()
{
    return snake.front().content;
}

void SnakeBoard::eraseTail()
{
    snake.pop_back();
}

void SnakeBoard::setSnakePreviousDirection(Direction direction)
{
    m_snakePreviousDirection = direction;
}

/*
void SnakeBoard::addBodyPart(){
   TileContent option=snake.front().content;
   switch (opt)
   {
   case TileContent::HeadDown:
break;

default:
break;
}
snake.insert(2, {});
}
*/