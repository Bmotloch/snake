#include "SnakeControls.hpp"

SnakeControls::SnakeControls(SnakeBoard &board, const SnakeTextBoard &textboard, double framerate)
    : m_textboard{textboard}, m_board{board}, m_frameRate{framerate}
{
    setTerminalMode(true);
    setNonBlockingInput(true);
}

SnakeControls::~SnakeControls()
{
    setTerminalMode(false);
    setNonBlockingInput(false);
}

void SnakeControls::play()
{
    pressStart();
    update();
    scoreboard();
}

void SnakeControls::pressStart() const
{
    m_textboard.displayWaitingScreen();
    while (m_board.getGameState() == GameState::NOT_STARTED)
    {
        if (isKeyPressed())
        {
            char key;
            std::cin >> key;
            if (key == 's')
            {
                m_board.setGameState(GameState::RUNNING);
                return;
            }
        }
    }
}

void SnakeControls::update()
{
    std::thread displayThread(&SnakeControls::displayFunction, this);
    std::thread inputThread(&SnakeControls::inputFunction, this);
    std::thread moveThread(&SnakeControls::moveFunction, this);
    displayThread.join();
    inputThread.join();
    moveThread.join();
}

void SnakeControls::setTerminalMode(bool enabled) const
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    if (enabled)
    {
        t.c_lflag &= ~(ICANON | ECHO);
    }
    else
    {
        t.c_lflag |= (ICANON | ECHO);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void SnakeControls::setNonBlockingInput(bool enabled) const
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (enabled)
    {
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }
    else
    {
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    }
}

bool SnakeControls::isKeyPressed() const
{
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    int bytesWaiting;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);

    return bytesWaiting > 0;
}

void SnakeControls::move()
{
    if (hasMoved)
    {
        return;
    }

    int oldHeadX = m_board.getSnakeHeadX();
    int oldHeadY = m_board.getSnakeHeadY();
    Direction option = m_board.getSnakeDirection();

    int newHeadX = oldHeadX;
    int newHeadY = oldHeadY;

    switch (option)
    {
    case Direction::Up:
        newHeadY--;
        break;
    case Direction::Down:
        newHeadY++;
        break;
    case Direction::Left:
        newHeadX--;
        break;
    case Direction::Right:
        newHeadX++;
        break;
    }

    if (newHeadX < 0 || newHeadX >= m_board.getBoardWidth() || newHeadY < 0 || newHeadY >= m_board.getBoardHeight())
    {
        m_board.setGameState(GameState::FINISHED_LOSS); // Hit the wall
        return;
    }

    TileContent tileContent = m_board.getTileData(newHeadX, newHeadY);

    if (tileContent == TileContent::Empty)
    {
        m_board.modifySnakePart(oldHeadX, oldHeadY, newHeadX, newHeadY);
        m_board.setTileData(newHeadX, newHeadY, m_board.getHeadTileContent(option));
        m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
        m_board.updateSnakeBody();
        m_board.eraseTail();
    }
    else if (tileContent == TileContent::Apple)
    {
        m_board.modifySnakePart(oldHeadX, oldHeadY, newHeadX, newHeadY);
        m_board.setTileData(newHeadX, newHeadY, m_board.getHeadTileContent(option));
        m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
        m_board.addBodyPart();
        m_board.updateSnakeBody();
        m_board.eraseTail();
        score += 20 * m_frameRate;
        if (!m_board.checkForWin())
        {
            m_board.addApple();
        }
    }
    else
    {
        m_board.setGameState(GameState::FINISHED_LOSS);
    }

    hasMoved = true;
}

void SnakeControls::changeDirection()
{
    if (hasRotated)
    {
        return;
    }

    char key;
    if (read(STDIN_FILENO, &key, 1) == 1)
    {
        if (key == 'q') // exit at any time of the game
        {
            m_board.setGameState(GameState::FINISHED_LOSS);
            std::cout << "Exiting..." << std::endl;
            return;
        }
        else if (key == 27) // Escape key
        {
            if (read(STDIN_FILENO, &key, 1) == 1 && key == '[')
            {
                if (read(STDIN_FILENO, &key, 1) == 1) // direction key D: left     C:right
                {
                    if (key == 'D')
                    {
                        switch (m_board.getSnakeDirection())
                        {
                        case Direction::Up:
                            m_board.setSnakeDirection(Direction::Left);
                            break;
                        case Direction::Down:
                            m_board.setSnakeDirection(Direction::Right);
                            break;
                        case Direction::Left:
                            m_board.setSnakeDirection(Direction::Down);
                            break;
                        case Direction::Right:
                            m_board.setSnakeDirection(Direction::Up);
                            break;
                        }

                        int headX = m_board.getSnake().front().x;
                        int headY = m_board.getSnake().front().y;

                        m_board.setTileData(headX, headY, m_board.getHeadTileContent(m_board.getSnakeDirection()));
                        hasRotated = true;
                        return;
                    }
                    else if (key == 'C')
                    {
                        switch (m_board.getSnakeDirection())
                        {
                        case Direction::Up:
                            m_board.setSnakeDirection(Direction::Right);
                            break;
                        case Direction::Down:
                            m_board.setSnakeDirection(Direction::Left);
                            break;
                        case Direction::Left:
                            m_board.setSnakeDirection(Direction::Up);
                            break;
                        case Direction::Right:
                            m_board.setSnakeDirection(Direction::Down);
                            break;
                        }

                        int headX = m_board.getSnake().front().x;
                        int headY = m_board.getSnake().front().y;

                        m_board.setTileData(headX, headY, m_board.getHeadTileContent(m_board.getSnakeDirection()));
                        hasRotated = true;
                        return;
                    }
                    else
                    {
                        switch (m_board.getSnakeDirection())
                        {
                        case Direction::Up:
                            m_board.setSnakeDirection(Direction::Up);
                            break;
                        case Direction::Down:
                            m_board.setSnakeDirection(Direction::Down);
                            break;
                        case Direction::Left:
                            m_board.setSnakeDirection(Direction::Left);
                            break;
                        case Direction::Right:
                            m_board.setSnakeDirection(Direction::Right);
                            break;
                        }

                        int headX = m_board.getSnake().front().x;
                        int headY = m_board.getSnake().front().y;

                        m_board.setTileData(headX, headY, m_board.getHeadTileContent(m_board.getSnakeDirection()));
                        hasRotated = true;
                        return;
                    }
                }
            }
        }
    }
}

void SnakeControls::displayFunction()
{
    std::string upper_border;
    std::string text_score{"score: "};
    for (int i = 0; i < m_board.getBoardWidth(); i++)
    {
        upper_border += "\u2550\u2550";
    }

    while (m_board.getGameState() == GameState::RUNNING)
    {
        auto startTime = std::chrono::steady_clock::now();
        hasRotated = false;
        hasMoved = false;
        score--;
        m_textboard.display();
        std::string padded_score = padScoreWithSpaces(text_score + std::to_string(score), m_board.getBoardWidth() * 2);
        std::cout << "\u2551" << padded_score << "\u2551" << std::endl;
        std::cout << "\u255A" << upper_border << "\u255D" << std::endl;
        auto endTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        double remainingTime = m_frameDuration * 1000 - elapsedTime;
        if (remainingTime > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(remainingTime)));
        }
    }
}

void SnakeControls::inputFunction()
{
    while (m_board.getGameState() == GameState::RUNNING)
    {
        changeDirection();
    }
}

void SnakeControls::moveFunction()
{
    while (m_board.getGameState() == GameState::RUNNING)
    {
        move();
    }
}

void SnakeControls::scoreboard() const
{
    m_textboard.displayScoreboard(score);
}

std::string SnakeControls::padScoreWithSpaces(std::string score, int desiredLength) const
{
    std::string paddedString = score;
    int currentLength = paddedString.length();

    if (currentLength <= desiredLength)
    {
        int spacesToAdd = desiredLength - currentLength;
        paddedString.append(spacesToAdd, ' ');
    }

    return paddedString;
}