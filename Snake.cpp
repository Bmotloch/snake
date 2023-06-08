#include "Snake.hpp"

// compilation without cmake: g++ main.cpp
// to run: <exec_name> <board_height> <board_width> <game_speed>

SnakeBoard::SnakeBoard(int width, int height) : m_width{width}, m_height{height}
{
    setGameState(GameState::NOT_STARTED);
    setSnakeDirection(Direction::Up);
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
    createSnake();
    addApple();
}

SnakeBoard::~SnakeBoard()
{
}

void SnakeBoard::createSnake()
{
    int center_y = getBoardHeight() / 2; // head on center or on the closest lower right position to center
    int center_x = getBoardWidth() / 2;
    setTileData(center_x, center_y, TileContent::HeadUp);
    m_snake.push_back({center_x, center_y, TileContent::HeadUp});
}

void SnakeBoard::addApple()
{
    int i{0};
    while (i != 1) // Starting ammount of apples
    {
        i++;
        int row = rand() % getBoardHeight();
        int col = rand() % getBoardWidth();
        if (m_board[row][col].content != TileContent::Empty)
        {
            i--;
            continue;
        }
        m_board[row][col].content = TileContent::Apple;
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

int SnakeBoard::getSnakeHeadX() const
{
    return m_snake.front().x;
}

int SnakeBoard::getSnakeHeadY() const
{
    return m_snake.front().y;
}

TileContent SnakeBoard::getHeadTileContent(Direction direction) const
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
    return TileContent::Empty; // wreturn
}

void SnakeBoard::eraseTail()
{
    for (int i = 0; i < getBoardHeight(); i++)
    {
        for (int j = 0; j < getBoardWidth(); j++)
        {
            bool isSnakePart = false;
            for (const BodyPart &part : m_snake)
            {
                if (part.x == getX(j, i) && part.y == getY(j, i))
                {
                    isSnakePart = true;
                    break;
                }
            }

            if (isSnakePart)
            {
                continue;
            }
            else if (getTileData(j, i) == TileContent::Body)
            {
                setTileData(j, i, TileContent::Empty);
            }
        }
    }
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

std::vector<BodyPart> &SnakeBoard::getSnake()
{
    return m_snake;
}

void SnakeBoard::updateSnakeBody()
{
    for (int i = m_snake.size() - 1; i > 0; i--)
    {
        m_snake[i].x = m_snake[i - 1].x;
        m_snake[i].y = m_snake[i - 1].y;
        m_snake[i].content = TileContent::Body;
    }
}

void SnakeBoard::addBodyPart()
{
    TileContent option = m_snake.front().content;
    switch (option)
    {
    case TileContent::HeadDown:
        m_snake.insert(m_snake.begin() + 1, {getSnakeHeadX(), getSnakeHeadY() - 1, TileContent::Body});
        break;
    case TileContent::HeadUp:
        m_snake.insert(m_snake.begin() + 1, {getSnakeHeadX(), getSnakeHeadY() + 1, TileContent::Body});
        break;
    case TileContent::HeadLeft:
        m_snake.insert(m_snake.begin() + 1, {getSnakeHeadX() + 1, getSnakeHeadY(), TileContent::Body});
        break;
    case TileContent::HeadRight:
        m_snake.insert(m_snake.begin() + 1, {getSnakeHeadX() - 1, getSnakeHeadY(), TileContent::Body});
        break;
    default:
        break;
    }
}

bool SnakeBoard::checkForWin()
{
    if (static_cast<int>(m_snake.size()) == getBoardHeight() * getBoardWidth())
    {
        setGameState(GameState::FINISHED_WIN);
        return true;
    }
    return false;
}