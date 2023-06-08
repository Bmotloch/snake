#include "SnakeControls.hpp"

SnakeControls::SnakeControls(SnakeBoard &board, const SnakeTextBoard &textboard, double framerate) : m_textboard{textboard}, m_board{board}, m_frameRate{framerate}
{
    setTerminalMode(true);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

SnakeControls::~SnakeControls()
{
    setTerminalMode(false);
}

void SnakeControls::play()
{   
    pressStart();
    update();
    setTerminalMode(false);
    resetTerminalMode();
    scoreboard();
}

void SnakeControls::resetTerminalMode()
{

    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    fflush(STDIN_FILENO);
}

void SnakeControls::pressStart()
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

void SnakeControls::setTerminalMode(bool enabled)
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

// Function to check if a key is pressed
bool SnakeControls::isKeyPressed()
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
    int oldHeadX{m_board.getSnakeHeadX()};
    int oldHeadY{m_board.getSnakeHeadY()};
    Direction option = m_board.getSnakeDirection();
    switch (option)
    {
    case Direction::Up:
    {
        if (oldHeadY - 1 < 0)
        {
            m_board.setGameState(GameState::FINISHED_LOSS); // hit the wall
            return;
        }
        if (m_board.getTileData(m_board.getX(oldHeadX, oldHeadY - 1), m_board.getY(oldHeadX, oldHeadY - 1)) == TileContent::Empty)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX, oldHeadY - 1);                    // head pos update
            m_board.setTileData(oldHeadX, oldHeadY - 1, m_board.getHeadTileContent(Direction::Up)); // proper head display
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);                             // old head pos is now body
            m_board.updateSnakeBody();
            m_board.eraseTail();
            hasMoved = true;
            return;
        }
        else if (m_board.getTileData(m_board.getX(oldHeadX, oldHeadY - 1), m_board.getY(oldHeadX, oldHeadY - 1)) == TileContent::Apple)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX, oldHeadY - 1);
            m_board.setTileData(oldHeadX, oldHeadY - 1, m_board.getHeadTileContent(Direction::Up));
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
            m_board.addBodyPart();
            m_board.updateSnakeBody();
            m_board.eraseTail();
            score += 20 * m_frameRate;
            if (!m_board.checkForWin())
            {
                m_board.addApple();
            }
            hasMoved = true;
            return;
        }
        else
        {
            m_board.setGameState(GameState::FINISHED_LOSS);
            return;
        }
    }
    case Direction::Down:
    {
        if (oldHeadY + 1 >= m_board.getBoardHeight())
        {
            m_board.setGameState(GameState::FINISHED_LOSS); // hit the wall
            return;
        }
        if (m_board.getTileData(m_board.getX(oldHeadX, oldHeadY + 1), m_board.getY(oldHeadX, oldHeadY + 1)) == TileContent::Empty)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX, oldHeadY + 1);
            m_board.setTileData(oldHeadX, oldHeadY + 1, m_board.getHeadTileContent(Direction::Down));
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
            m_board.updateSnakeBody();
            m_board.eraseTail();
            hasMoved = true;
            return;
        }
        else if (m_board.getTileData(m_board.getX(oldHeadX, oldHeadY + 1), m_board.getY(oldHeadX, oldHeadY + 1)) == TileContent::Apple)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX, oldHeadY + 1);
            m_board.setTileData(oldHeadX, oldHeadY + 1, m_board.getHeadTileContent(Direction::Down));
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
            m_board.addBodyPart();
            m_board.updateSnakeBody();
            m_board.eraseTail();
            score += 20 * m_frameRate;
            if (!m_board.checkForWin())
            {
                m_board.addApple();
            }
            hasMoved = true;
            return;
        }
        else
        {
            m_board.setGameState(GameState::FINISHED_LOSS);
            return;
        }
    }
    case Direction::Left:
    {
        if (oldHeadX - 1 < 0)
        {
            m_board.setGameState(GameState::FINISHED_LOSS); // hit the wall
            return;
        }
        if (m_board.getTileData(m_board.getX(oldHeadX - 1, oldHeadY), m_board.getY(oldHeadX - 1, oldHeadY)) == TileContent::Empty)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX - 1, oldHeadY);
            m_board.setTileData(oldHeadX - 1, oldHeadY, m_board.getHeadTileContent(Direction::Left));
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
            m_board.updateSnakeBody();
            m_board.eraseTail();
            hasMoved = true;
            return;
        }
        else if (m_board.getTileData(m_board.getX(oldHeadX - 1, oldHeadY), m_board.getY(oldHeadX - 1, oldHeadY)) == TileContent::Apple)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX - 1, oldHeadY);
            m_board.setTileData(oldHeadX - 1, oldHeadY, m_board.getHeadTileContent(Direction::Left));
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
            m_board.addBodyPart();
            m_board.updateSnakeBody();
            m_board.eraseTail();
            score += 20 * m_frameRate;
            if (!m_board.checkForWin())
            {
                m_board.addApple();
            }
            hasMoved = true;
            return;
        }
        else
        {
            m_board.setGameState(GameState::FINISHED_LOSS);
            return;
        }
    }
    case Direction::Right:
    {
        if (oldHeadX + 1 >= m_board.getBoardWidth())
        {
            m_board.setGameState(GameState::FINISHED_LOSS); // hit the wall
            return;
        }
        if (m_board.getTileData(m_board.getX(oldHeadX + 1, oldHeadY), m_board.getY(oldHeadX + 1, oldHeadY)) == TileContent::Empty)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX + 1, oldHeadY);
            m_board.setTileData(oldHeadX + 1, oldHeadY, m_board.getHeadTileContent(Direction::Right));
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
            m_board.updateSnakeBody();
            m_board.eraseTail();
            hasMoved = true;
            return;
        }
        else if (m_board.getTileData(m_board.getX(oldHeadX + 1, oldHeadY), m_board.getY(oldHeadX + 1, oldHeadY)) == TileContent::Apple)
        {
            m_board.modifySnakePart(oldHeadX, oldHeadY, oldHeadX + 1, oldHeadY);
            m_board.setTileData(oldHeadX + 1, oldHeadY, m_board.getHeadTileContent(Direction::Right));
            m_board.setTileData(oldHeadX, oldHeadY, TileContent::Body);
            m_board.addBodyPart();
            m_board.updateSnakeBody();
            m_board.eraseTail();
            score += 20 * m_frameRate;
            if (!m_board.checkForWin())
            {
                m_board.addApple();
            }
            hasMoved = true;
            return;
        }
        else
        {
            m_board.setGameState(GameState::FINISHED_LOSS);
            return;
        }
    }
    }
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
            std::cout << "Exiting...";
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
    while (m_board.getGameState() == GameState::RUNNING)
    {
        auto startTime = std::chrono::steady_clock::now();
        // Update the console screen here
        hasRotated = false;
        hasMoved = false;
        m_textboard.display();

        std::cout << "score: " << score << std::endl;
        score--; // every move costs 1 point
        // Get the current time after updating the screen
        auto endTime = std::chrono::steady_clock::now();

        // Calculate the time taken for updating the screen
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        // Calculate the remaining time until the next frame
        double remainingTime = m_frameDuration * 1000 - elapsedTime;

        // Check if the remaining time is positive
        if (remainingTime > 0)
        {
            // Sleep for the remaining time to achieve the desired frame rate
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

void SnakeControls::scoreboard()
{
    m_textboard.displayScoreboard(score);
}
