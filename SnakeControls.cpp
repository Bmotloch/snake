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
    int counter{0};
    while (m_board.getGameState() == GameState::RUNNING)
    {
        // Get the current time before updating the screen
        auto startTime = std::chrono::steady_clock::now();

        // Update the console screen here

        system("clear");
        m_textboard.display();
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
