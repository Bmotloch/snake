#include "Snake.hpp"
#include "SnakeTextBoard.hpp"
#include "SnakeControls.hpp"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <height> <width> <game_speed_in_Hz>\n"
                  << std::endl;
        return 1;
    }
    if (std::stoi(argv[1]) > 25 || std::stoi(argv[2]) > 50 || std::stoi(argv[1]) < 5 || std::stoi(argv[2]) < 5)
    {
        std::cerr << "The minimal board size is 5x5\n"
                  << "The maximal board size is 25x50\n";
        return 1;
    }
    if (std::stod(argv[3]) < 0 || std::stod(argv[3]) > 10)
    {
        std::cerr << "The speed of game is in range [1,10]\n";
        return 1;
    }
    srand(time(0));
    int width{std::stoi(argv[1])};
    int height{std::stoi(argv[2])};
    double frameRate{std::stod(argv[3])};
    system("clear");
    bool valid = false;
    std::string nick;
    std::cout << "Enter your nick: ";
    while (!valid)
    {
        std::cin >> nick;
        if (nick.size() < 15 && nick.size() > 0)
        {
            valid = true;
            break;
        }
        else
        {
            std::cout << "Provided Nick is too long or too short" << std::endl;
        }
    }
    SnakeBoard board(width, height);
    SnakeTextBoard textboard(board, nick);
    SnakeControls game(board, textboard, frameRate);
    game.play();
    return 0;
}