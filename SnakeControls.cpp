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
    /* while (m_board.getGameState() == GameState::RUNNING)
    {
        if (isKeyPressed())
        {
            char key;
            if (read(STDIN_FILENO, &key, 1) == 1 && key == 'q')
            {
                break;
            }
        }
        }*/
    displayThread.join();
    inputThread.join();
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
}

void SnakeControls::changeDirection()
{
    char key;
    if (read(STDIN_FILENO, &key, 1) == 1)
    {
        if (key == 27) // Escape key
        {
            if (read(STDIN_FILENO, &key, 1) == 1 && key == '[')
            {
                if (read(STDIN_FILENO, &key, 1) == 1) // direction key D: left     C:right
                {
                    if (rotationPerformed)
                    {
                        return;
                    }
                    else if (key == 'D')
                    {
                        switch (m_board.getSnakeDirection())
                        {
                        case Direction::Up:
                        {
                            m_board.setSnakeDirection(Direction::Left);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadLeft);
                            rotationPerformed = true;
                        }
                        break;
                        case Direction::Down:
                        {
                            m_board.setSnakeDirection(Direction::Right);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadRight);
                            rotationPerformed = true;
                        }
                        break;
                        case Direction::Left:
                        {
                            m_board.setSnakeDirection(Direction::Down);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadDown);
                            rotationPerformed = true;
                        }
                        break;
                        case Direction::Right:
                        {
                            m_board.setSnakeDirection(Direction::Up);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadUp);
                            rotationPerformed = true;
                        }
                        break;
                            return;
                        }
                    }
                    else if (key == 'C')
                    {
                        switch (m_board.getSnakeDirection())
                        {
                        case Direction::Up:
                        {
                            m_board.setSnakeDirection(Direction::Right);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadRight);
                            rotationPerformed = true;
                        }
                        break;
                        case Direction::Down:
                        {
                            m_board.setSnakeDirection(Direction::Left);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadLeft);
                            rotationPerformed = true;
                        }
                        break;
                        case Direction::Left:
                        {
                            m_board.setSnakeDirection(Direction::Up);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadUp);
                            rotationPerformed = true;
                        }
                        break;
                        case Direction::Right:
                        {
                            m_board.setSnakeDirection(Direction::Down);
                            m_board.setTileData(m_board.getBoardWidth() / 2, m_board.getBoardHeight() / 2, TileContent::HeadDown);
                            rotationPerformed = true;
                        }
                        break;
                        }
                        return;
                    }
                }
            }
        }
    }
}

void SnakeControls::updateSnakePosition()
{
    switch (m_board.getSnakeDirection())
    {
    case Direction::Up:
        m_board.setSnakeHeadY(1);
        break;
    case Direction::Down:
        m_board.setSnakeHeadY(-1);
        break;
    case Direction::Left:
        m_board.setSnakeHeadX(-1);
        break;
    case Direction::Right:
        m_board.setSnakeHeadX(1);
        break;
    }
}

void SnakeControls::displayFunction()
{
    int counter{0};
    while (m_board.getGameState() == GameState::RUNNING)
    {
        auto startTime = std::chrono::steady_clock::now();
        // Update the console screen here
        system("clear");
        m_textboard.display();
        rotationPerformed=false;
        std::cout << "iter: " << counter << std::endl;
        counter++;
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
        move();
    }
}