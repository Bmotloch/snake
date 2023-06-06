#include "Snake.hpp"

// compilation without cmake: g++ main.cpp
// to run: <exec_name> <board_height> <board_width> <game_speed>

SnakeBoard::SnakeBoard(int width, int height) : m_width{width}, m_height{height}
{
    setGameState(GameState::NOT_STARTED);
    setSnakeDirection(Direction::Up);
    setSnakePreviousDirection(Direction::Up);
    m_board.resize(m_height, std::vector<BodyPart>(m_width));
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_board[y][x].x = x;
            m_board[y][x].y = y;
            m_board[y][x].content = TileContent::Empty;
        }
    }
    createSnake(m_snake);
   // fillBoard(m_board);
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
    return m_board[y][x].content;
}

int SnakeBoard::getX(int x, int y) const
{
    return m_board[y][x].x;
}

int SnakeBoard::getY(int x, int y) const
{
    return m_board[y][x].y;
}

Direction SnakeBoard::getSnakeDirection() const
{
    return m_snakeDirection;
}



void SnakeBoard::setSnakeDirection(Direction direction)
{
    m_snakeDirection = direction;
}

void SnakeBoard::setTileData(int x, int y, TileContent content)
{
    m_board[y][x].content = content;
}

int SnakeBoard::getSnakeHeadX()
{
    return m_snake.front().x;
}

int SnakeBoard::getSnakeHeadY()
{
    return m_snake.front().y;
}
TileContent SnakeBoard::getHeadTileContent(Direction direction) // useless? only current and previous direction needed
{
    switch (direction)
    {
    case Direction::Up:
        return TileContent::HeadUp;
    case Direction::Right:
        return TileContent::HeadRight;
    case Direction::Left:
        return TileContent::HeadLeft;
    case Direction::Down:
        return TileContent::HeadDown;
    }
    return TileContent::Empty; //wreturn
}

void SnakeBoard::eraseTail()
{
    m_snake.pop_back();
}

void SnakeBoard::setSnakePreviousDirection(Direction direction)
{
    m_snakePreviousDirection = direction;
}

std::list<BodyPart>::iterator SnakeBoard::getSnakeIter(int x, int y)
{
    for (std::list<BodyPart>::iterator it = m_snake.begin(); it != m_snake.end(); ++it)
    {
        if (it->x == x && it->y == y)
        {
            return it; // Return the iterator when the coordinates match
        }
    }
    return m_snake.end();
}

void SnakeBoard::modifySnakePart(int oldX, int oldY, int newX, int newY)
{
    for (auto &part : m_snake)
    {
        if (part.x == oldX && part.y == oldY)
        {
            part.x = newX;
            part.y = newY;
            break;
        }
    }
}

std::list<BodyPart> &SnakeBoard::getSnake()
{
    return m_snake;
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